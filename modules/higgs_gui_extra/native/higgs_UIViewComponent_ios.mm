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

class UIViewComponent::Pimpl  : public ComponentMovementWatcher
{
public:
    Pimpl (UIView* v, Component& comp)
        : ComponentMovementWatcher (&comp),
          view (v),
          owner (comp)
    {
        [view retain];

        if (owner.isShowing())
            componentPeerChanged();
    }

    ~Pimpl() override
    {
        [view removeFromSuperview];
        [view release];
    }

    void componentMovedOrResized (bool /*wasMoved*/, bool /*wasResized*/) override
    {
        auto* topComp = owner.getTopLevelComponent();

        if (topComp->getPeer() != nullptr)
        {
            auto pos = topComp->getLocalPoint (&owner, Point<int>());

            [view setFrame: CGRectMake ((float) pos.x, (float) pos.y,
                                        (float) owner.getWidth(), (float) owner.getHeight())];
        }
    }

    void componentPeerChanged() override
    {
        auto* peer = owner.getPeer();

        if (currentPeer != peer)
        {
            if ([view superview] != nil)
                [view removeFromSuperview]; // Must be careful not to call this unless it's required - e.g. some Apple AU views
                                            // override the call and use it as a sign that they're being deleted, which breaks everything.
            currentPeer = peer;

            if (peer != nullptr)
            {
                UIView* peerView = (UIView*) peer->getNativeHandle();
                [peerView addSubview: view];
                componentMovedOrResized (false, false);
            }
        }

        [view setHidden: ! owner.isShowing()];
     }

    void componentVisibilityChanged() override
    {
        componentPeerChanged();
    }

    Rectangle<int> getViewBounds() const
    {
        CGRect r = [view frame];
        return Rectangle<int> ((int) r.size.width, (int) r.size.height);
    }

    UIView* const view;

private:
    Component& owner;
    ComponentPeer* currentPeer = nullptr;

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Pimpl)
};

//==============================================================================
UIViewComponent::UIViewComponent() {}
UIViewComponent::~UIViewComponent()
{
    AccessibilityHandler::setNativeChildForComponent (*this, nullptr);
}

void UIViewComponent::setView (void* view)
{
    if (view != getView())
    {
        pimpl.reset();

        if (view != nullptr)
            pimpl.reset (new Pimpl ((UIView*) view, *this));

        AccessibilityHandler::setNativeChildForComponent (*this, getView());
    }
}

void* UIViewComponent::getView() const
{
    return pimpl == nullptr ? nullptr : pimpl->view;
}

void UIViewComponent::resizeToFitView()
{
    if (pimpl != nullptr)
        setBounds (pimpl->getViewBounds());
}

void UIViewComponent::paint (Graphics&) {}

std::unique_ptr<AccessibilityHandler> UIViewComponent::createAccessibilityHandler()
{
    return std::make_unique<AccessibilityHandler> (*this, AccessibilityRole::group);
}

} // namespace higgs
