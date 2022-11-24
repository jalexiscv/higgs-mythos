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

class AndroidViewComponent::Pimpl  : public ComponentMovementWatcher
{
public:
    Pimpl (const LocalRef<jobject>& v, Component& comp)
        : ComponentMovementWatcher (&comp),
          view (v),
          owner (comp)
    {
        if (owner.isShowing())
            componentPeerChanged();
    }

    ~Pimpl() override
    {
        removeFromParent();
    }

    void componentMovedOrResized (bool /*wasMoved*/, bool /*wasResized*/) override
    {
        auto* topComp = owner.getTopLevelComponent();

        if (topComp->getPeer() != nullptr)
        {
            auto pos = topComp->getLocalPoint (&owner, Point<int>());

            Rectangle<int> r (pos.x, pos.y, owner.getWidth(), owner.getHeight());
            r *= Desktop::getInstance().getDisplays().getPrimaryDisplay()->scale;

            getEnv()->CallVoidMethod (view, AndroidView.layout, r.getX(), r.getY(),
                                      r.getRight(), r.getBottom());
        }
    }

    void componentPeerChanged() override
    {
        auto* peer = owner.getPeer();

        if (currentPeer != peer)
        {
            removeFromParent();
            currentPeer = peer;

            addToParent();
        }

        enum
        {
            VISIBLE   = 0,
            INVISIBLE = 4
        };

        getEnv()->CallVoidMethod (view, AndroidView.setVisibility, owner.isShowing() ? VISIBLE : INVISIBLE);
     }

    void componentVisibilityChanged() override
    {
        componentPeerChanged();
    }

    void componentBroughtToFront (Component& comp) override
    {
        ComponentMovementWatcher::componentBroughtToFront (comp);
    }

    Rectangle<int> getViewBounds() const
    {
        auto* env = getEnv();

        int width  = env->CallIntMethod (view, AndroidView.getWidth);
        int height = env->CallIntMethod (view, AndroidView.getHeight);

        return Rectangle<int> (width, height);
    }

    GlobalRef view;

private:
    void addToParent()
    {
        if (currentPeer != nullptr)
        {
            jobject peerView = (jobject) currentPeer->getNativeHandle();

            // NB: Assuming a parent is always of ViewGroup type
            auto* env = getEnv();

            env->CallVoidMethod (peerView, AndroidViewGroup.addView, view.get());
            componentMovedOrResized (false, false);
        }
    }

    void removeFromParent()
    {
        auto* env = getEnv();
        auto parentView = env->CallObjectMethod (view, AndroidView.getParent);

        if (parentView != nullptr)
        {
            // Assuming a parent is always of ViewGroup type
            env->CallVoidMethod (parentView, AndroidViewGroup.removeView, view.get());
        }
    }

    Component& owner;
    ComponentPeer* currentPeer = nullptr;

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Pimpl)
};

//==============================================================================
AndroidViewComponent::AndroidViewComponent()
{
}

AndroidViewComponent::~AndroidViewComponent()
{
    AccessibilityHandler::setNativeChildForComponent (*this, nullptr);
}

void AndroidViewComponent::setView (void* view)
{
    if (view != getView())
    {
        pimpl.reset();

        if (view != nullptr)
        {
            // explicitly create a new local ref here so that we don't
            // delete the users pointer
            auto* env = getEnv();
            auto localref = LocalRef<jobject> (env->NewLocalRef ((jobject) view));

            pimpl.reset (new Pimpl (localref, *this));

            AccessibilityHandler::setNativeChildForComponent (*this, getView());
        }
        else
        {
            AccessibilityHandler::setNativeChildForComponent (*this, nullptr);
        }
    }
}

void* AndroidViewComponent::getView() const
{
    return pimpl == nullptr ? nullptr : (void*) pimpl->view;
}

void AndroidViewComponent::resizeToFitView()
{
    if (pimpl != nullptr)
        setBounds (pimpl->getViewBounds());
}

void AndroidViewComponent::paint (Graphics&) {}

std::unique_ptr<AccessibilityHandler> AndroidViewComponent::createAccessibilityHandler()
{
    return std::make_unique<AccessibilityHandler> (*this, AccessibilityRole::group);
}

} // namespace higgs
