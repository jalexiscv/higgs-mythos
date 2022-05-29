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
struct OutlineWindowComponent final : public Component
{
    OutlineWindowComponent (Component* c, FocusOutline::OutlineWindowProperties& p)
      : target (c), props (p)
    {
        setVisible (true);
        setInterceptsMouseClicks (false, false);

        if (target->isOnDesktop())
        {
            setSize (1, 1);
            addToDesktop (DesktopWindowOptions().withStyleFlags (ComponentPeer::windowIgnoresMouseClicks
                                                                 | ComponentPeer::windowIsTemporary
                                                                 | ComponentPeer::windowIgnoresKeyPresses)
                                                .withAssociatedComponent (c));
        }
        else if (auto* parent = target->getParentComponent())
        {
            auto targetIndex = parent->getIndexOfChildComponent (target);
            parent->addChildComponent (this, targetIndex + 1);
        }
    }

    void paint (Graphics& g) override
    {
        if (target != nullptr)
            props.drawOutline (g, getWidth(), getHeight());
    }

    void resized() override
    {
        repaint();
    }

    float getDesktopScaleFactor() const override
    {
        return target != nullptr ? target->getDesktopScaleFactor()
                                 : Component::getDesktopScaleFactor();
    }

private:
    WeakReference<Component> target;
    FocusOutline::OutlineWindowProperties& props;

    HIGGS_DECLARE_NON_COPYABLE (OutlineWindowComponent)
};

//==============================================================================
FocusOutline::FocusOutline (std::unique_ptr<OutlineWindowProperties> props)
    : properties (std::move (props))
{
}

FocusOutline::~FocusOutline()
{
    if (owner != nullptr)
        owner->removeComponentListener (this);

    if (lastParentComp != nullptr)
        lastParentComp->removeComponentListener (this);
}

void FocusOutline::setOwner (Component* componentToFollow)
{
    if (componentToFollow != owner)
    {
        if (owner != nullptr)
            owner->removeComponentListener (this);

        owner = componentToFollow;

        if (owner != nullptr)
            owner->addComponentListener (this);

        updateParent();
        updateOutlineWindow();
    }
}

void FocusOutline::componentMovedOrResized (Component& c, bool, bool)
{
    if (owner == &c)
        updateOutlineWindow();
}

void FocusOutline::componentBroughtToFront (Component& c)
{
    if (owner == &c)
        updateOutlineWindow();
}

void FocusOutline::componentParentHierarchyChanged (Component& c)
{
    if (owner == &c)
    {
        updateParent();
        updateOutlineWindow();
    }
}

void FocusOutline::componentVisibilityChanged (Component& c)
{
    if (owner == &c)
        updateOutlineWindow();
}

void FocusOutline::updateParent()
{
    lastParentComp = (owner != nullptr ? owner->getParentComponent()
                                       : nullptr);
}

void FocusOutline::updateOutlineWindow()
{
    if (reentrant)
        return;

    const ScopedValueSetter<bool> setter (reentrant, true);

    if (owner == nullptr)
    {
        outlineWindow = nullptr;
        return;
    }

    if (owner->isShowing()
         && owner->getWidth() > 0 && owner->getHeight() > 0)
    {
        if (outlineWindow == nullptr)
            outlineWindow = std::make_unique<OutlineWindowComponent> (owner, *properties);

        WeakReference<Component> deletionChecker (outlineWindow.get());

        outlineWindow->setAlwaysOnTop (owner->isAlwaysOnTop());

        if (deletionChecker == nullptr)
            return;

        const auto windowBounds = [this]
        {
            const auto bounds = properties->getOutlineBounds (*owner);

            if (lastParentComp != nullptr)
                return lastParentComp->getLocalArea (nullptr, bounds);

            return bounds;
        }();

        outlineWindow->setBounds (windowBounds);
    }
    else
    {
        outlineWindow = nullptr;
    }
}

} // namespace higgs
