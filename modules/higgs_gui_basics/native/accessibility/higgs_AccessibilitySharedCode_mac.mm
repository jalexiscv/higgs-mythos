/*
  ==============================================================================

   This file is part of the Higgs Mythos framework.
   Copyright (c) Raw Material Software Limited

   Higgs Mythos is free software: you can redistribute it and/or modify
   it under the terms of the GNU Affero General Public License as published
   by the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   Higgs Mythos is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU Affero General Public License for more details.

   You should have received a copy of the GNU Affero General Public License
   along with Higgs Mythos. If not, see <https://www.gnu.org/licenses/>.

  ==============================================================================
*/

namespace higgs
{

//==============================================================================
struct AccessibleObjCClassDeleter
{
    template <typename ElementType>
    void operator() (ElementType* element) const
    {
        higgsFreeAccessibilityPlatformSpecificData (element);

        object_setInstanceVariable (element, "handler", nullptr);
        [element release];
    }
};

template <typename Base>
class AccessibleObjCClass : public ObjCClass<Base>
{
public:
    using Holder = std::unique_ptr<Base, AccessibleObjCClassDeleter>;

protected:
    AccessibleObjCClass() : AccessibleObjCClass ("HIGGSAccessibilityElement_") {}

    explicit AccessibleObjCClass (const char* name)  : ObjCClass<Base> (name)
    {
        ObjCClass<Base>::template addIvar<AccessibilityHandler*> ("handler");
    }

    //==============================================================================
    static AccessibilityHandler* getHandler (id self)
    {
        return getIvar<AccessibilityHandler*> (self, "handler");
    }

    template <typename MemberFn>
    static auto getInterface (id self, MemberFn fn) noexcept -> decltype ((std::declval<AccessibilityHandler>().*fn)())
    {
        if (auto* handler = getHandler (self))
            return (handler->*fn)();

        return nullptr;
    }

    static AccessibilityTextInterface*  getTextInterface  (id self) noexcept  { return getInterface (self, &AccessibilityHandler::getTextInterface); }
    static AccessibilityValueInterface* getValueInterface (id self) noexcept  { return getInterface (self, &AccessibilityHandler::getValueInterface); }
    static AccessibilityTableInterface* getTableInterface (id self) noexcept  { return getInterface (self, &AccessibilityHandler::getTableInterface); }
    static AccessibilityCellInterface*  getCellInterface  (id self) noexcept  { return getInterface (self, &AccessibilityHandler::getCellInterface); }

    static bool hasEditableText (AccessibilityHandler& handler) noexcept
    {
        return handler.getRole() == AccessibilityRole::editableText
            && handler.getTextInterface() != nullptr
            && ! handler.getTextInterface()->isReadOnly();
    }

    static id getAccessibilityValueFromInterfaces (const AccessibilityHandler& handler)
    {
        if (auto* textInterface = handler.getTextInterface())
            return higgsStringToNS (textInterface->getText ({ 0, textInterface->getTotalNumCharacters() }));

        if (auto* valueInterface = handler.getValueInterface())
            return higgsStringToNS (valueInterface->getCurrentValueAsString());

        return nil;
    }

    //==============================================================================
    static BOOL getIsAccessibilityElement (id self, SEL)
    {
        if (auto* handler = getHandler (self))
            return ! handler->isIgnored() && handler->getRole() != AccessibilityRole::window;

        return NO;
    }

    static void setAccessibilityValue (id self, SEL, NSString* value)
    {
        if (auto* handler = getHandler (self))
        {
            if (hasEditableText (*handler))
            {
                handler->getTextInterface()->setText (nsStringToHiggs (value));
                return;
            }

            if (auto* valueInterface = handler->getValueInterface())
                if (! valueInterface->isReadOnly())
                    valueInterface->setValueAsString (nsStringToHiggs (value));
        }
    }

    static BOOL performActionIfSupported (id self, AccessibilityActionType actionType)
    {
        if (auto* handler = getHandler (self))
            if (handler->getActions().invoke (actionType))
                return YES;

        return NO;
    }

    static BOOL accessibilityPerformPress (id self, SEL)
    {
        if (auto* handler = getHandler (self))
            if (handler->getCurrentState().isCheckable() && handler->getActions().invoke (AccessibilityActionType::toggle))
                return YES;

        return performActionIfSupported (self, AccessibilityActionType::press);
    }

    static BOOL accessibilityPerformIncrement (id self, SEL)
    {
        if (auto* valueInterface = getValueInterface (self))
        {
            if (! valueInterface->isReadOnly())
            {
                auto range = valueInterface->getRange();

                if (range.isValid())
                {
                    valueInterface->setValue (jlimit (range.getMinimumValue(),
                                                      range.getMaximumValue(),
                                                      valueInterface->getCurrentValue() + range.getInterval()));
                    return YES;
                }
            }
        }

        return NO;
    }

    static BOOL accessibilityPerformDecrement (id self, SEL)
    {
        if (auto* valueInterface = getValueInterface (self))
        {
            if (! valueInterface->isReadOnly())
            {
                auto range = valueInterface->getRange();

                if (range.isValid())
                {
                    valueInterface->setValue (jlimit (range.getMinimumValue(),
                                                      range.getMaximumValue(),
                                                      valueInterface->getCurrentValue() - range.getInterval()));
                    return YES;
                }
            }
        }

        return NO;
    }

    static NSString* getAccessibilityTitle (id self, SEL)
    {
        if (auto* handler = getHandler (self))
        {
            auto title = handler->getTitle();

            if (title.isEmpty() && handler->getComponent().isOnDesktop())
                title = detail::AccessibilityHelpers::getApplicationOrPluginName();

            NSString* nsString = higgsStringToNS (title);

            if (nsString != nil && [[self accessibilityValue] isEqual: nsString])
                return @"";

            return nsString;
        }

        return nil;
    }

    static NSString* getAccessibilityHelp (id self, SEL)
    {
        if (auto* handler = getHandler (self))
            return higgsStringToNS (handler->getHelp());

        return nil;
    }

    static BOOL getIsAccessibilityModal (id self, SEL)
    {
        if (auto* handler = getHandler (self))
            return handler->getComponent().isCurrentlyModal();

        return NO;
    }

    static NSInteger getAccessibilityRowCount (id self, SEL)
    {
        if (auto* tableHandler = detail::AccessibilityHelpers::getEnclosingHandlerWithInterface (getHandler (self), &AccessibilityHandler::getTableInterface))
            if (auto* tableInterface = tableHandler->getTableInterface())
                return tableInterface->getNumRows();

        return 0;
    }

    static NSInteger getAccessibilityColumnCount (id self, SEL)
    {
        if (auto* tableHandler = detail::AccessibilityHelpers::getEnclosingHandlerWithInterface (getHandler (self), &AccessibilityHandler::getTableInterface))
            if (auto* tableInterface = tableHandler->getTableInterface())
                return tableInterface->getNumColumns();

        return 0;
    }

    template <typename Getter>
    static NSRange getCellDimensions (id self, Getter getter)
    {
        const auto notFound = NSMakeRange (NSNotFound, 0);

        auto* handler = getHandler (self);

        if (handler == nullptr)
            return notFound;

        auto* tableHandler = detail::AccessibilityHelpers::getEnclosingHandlerWithInterface (getHandler (self), &AccessibilityHandler::getTableInterface);

        if (tableHandler == nullptr)
            return notFound;

        auto* tableInterface = tableHandler->getTableInterface();

        if (tableInterface == nullptr)
            return notFound;

        const auto result = (tableInterface->*getter) (*handler);

        if (! result.hasValue())
            return notFound;

        return NSMakeRange ((NSUInteger) result->begin, (NSUInteger) result->num);
    }

    static NSRange getAccessibilityRowIndexRange (id self, SEL)
    {
        return getCellDimensions (self, &AccessibilityTableInterface::getRowSpan);
    }

    static NSRange getAccessibilityColumnIndexRange (id self, SEL)
    {
        return getCellDimensions (self, &AccessibilityTableInterface::getColumnSpan);
    }

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AccessibleObjCClass)
};

} // namespace higgs
