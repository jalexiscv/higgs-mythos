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

static int showNativeBoxUnmanaged (const MessageBoxOptions& opts,
                                   ModalComponentManager::Callback* cb)
{
    // If this is hit, your alert window has duplicate result codes!
    jassert (opts.areAllResultCodesUnique());

    auto implementation = detail::ScopedMessageBoxInterface::create (opts);
    return detail::ConcreteScopedMessageBoxImpl::showUnmanaged (std::move (implementation), cb);
}

#if HIGGS_MODAL_LOOPS_PERMITTED
void HIGGS_CALLTYPE NativeMessageBox::showMessageBox (MessageBoxIconType iconType,
                                                     const String& title, const String& message,
                                                     Component* associatedComponent)
{
    showNativeBoxUnmanaged (MessageBoxOptions().withIconType (iconType)
                                               .withTitle (title)
                                               .withMessage (message)
                                               .withButton (TRANS ("OK"), 0)
                                               .withAssociatedComponent (associatedComponent),
                            nullptr);
}

int HIGGS_CALLTYPE NativeMessageBox::showForOptions (const MessageBoxOptions& options)
{
    return showNativeBoxUnmanaged (options, nullptr);
}
#endif

void HIGGS_CALLTYPE NativeMessageBox::showMessageBoxAsync (MessageBoxIconType iconType,
                                                          const String& title, const String& message,
                                                          Component* associatedComponent,
                                                          ModalComponentManager::Callback* callback)
{
    auto options = MessageBoxOptions{}.withIconType (iconType)
                                      .withTitle (title)
                                      .withMessage (message)
                                      .withButton (TRANS ("OK"), 0)
                                      .withAssociatedComponent (associatedComponent);
    showNativeBoxUnmanaged (options, callback);
}

bool HIGGS_CALLTYPE NativeMessageBox::showOkCancelBox (MessageBoxIconType iconType,
                                                      const String& title, const String& message,
                                                      Component* associatedComponent,
                                                      ModalComponentManager::Callback* callback)
{
    auto options = MessageBoxOptions{}.withIconType (iconType)
                                      .withTitle (title)
                                      .withMessage (message)
                                      .withButton (TRANS ("OK"), 1)
                                      .withButton (TRANS ("Cancel"), 0)
                                      .withAssociatedComponent (associatedComponent);
    return showNativeBoxUnmanaged (options, callback) != 0;
}

int HIGGS_CALLTYPE NativeMessageBox::showYesNoCancelBox (MessageBoxIconType iconType,
                                                        const String& title, const String& message,
                                                        Component* associatedComponent,
                                                        ModalComponentManager::Callback* callback)
{
    auto options = MessageBoxOptions{}.withIconType (iconType)
                                      .withTitle (title)
                                      .withMessage (message)
                                      .withButton (TRANS ("Yes"), 1)
                                      .withButton (TRANS ("No"), 2)
                                      .withButton (TRANS ("Cancel"), 0)
                                      .withAssociatedComponent (associatedComponent);
    return showNativeBoxUnmanaged (options, callback);
}

int HIGGS_CALLTYPE NativeMessageBox::showYesNoBox (MessageBoxIconType iconType,
                                                  const String& title, const String& message,
                                                  Component* associatedComponent,
                                                  ModalComponentManager::Callback* callback)
{
    auto options = MessageBoxOptions{}.withIconType (iconType)
                                      .withTitle (title)
                                      .withMessage (message)
                                      .withButton (TRANS ("Yes"), 1)
                                      .withButton (TRANS ("No"), 0)
                                      .withAssociatedComponent (associatedComponent);
    return showNativeBoxUnmanaged (options, callback);
}

void HIGGS_CALLTYPE NativeMessageBox::showForOptionsAsync (const MessageBoxOptions& options,
                                                          ModalComponentManager::Callback* callback)
{
    showNativeBoxUnmanaged (options, callback);
}

void HIGGS_CALLTYPE NativeMessageBox::showForOptionsAsync (const MessageBoxOptions& options,
                                                          std::function<void (int)> callback)
{
    showForOptionsAsync (options, ModalCallbackFunction::create (callback));
}

ScopedMessageBox NativeMessageBox::showScopedAsync (const MessageBoxOptions& options, std::function<void (int)> callback)
{
    // If this is hit, your alert window has duplicate result codes!
    jassert (options.areAllResultCodesUnique());

    auto implementation = detail::ScopedMessageBoxInterface::create (options);
    return detail::ConcreteScopedMessageBoxImpl::show (std::move (implementation), std::move (callback));
}

} // namespace higgs
