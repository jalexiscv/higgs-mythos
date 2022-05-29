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

class DropShadower::ShadowWindow final : public Component
{
public:
    ShadowWindow (Component* comp, const DropShadow& ds)
        : target (comp), shadow (ds)
    {
        if (comp == nullptr)
        {
            jassertfalse;
            return;
        }

        setVisible (true);
        setAccessible (false);
        setInterceptsMouseClicks (false, false);

        if (comp->isOnDesktop())
        {
           #if HIGGS_WINDOWS
            const ScopedThreadDPIAwarenessSetter scope { comp != nullptr ? comp->getWindowHandle() : nullptr };
           #endif

            setSize (1, 1); // to keep the OS happy by not having zero-size windows
            addToDesktop (DesktopWindowOptions().withStyleFlags (ComponentPeer::windowIgnoresMouseClicks
                                                                 | ComponentPeer::windowIsTemporary
                                                                 | ComponentPeer::windowIgnoresKeyPresses)
                                                .withAssociatedComponent (comp));

            if (auto* compPeer = comp->getPeer())
                if (auto* selfPeer = getPeer())
                    selfPeer->setCustomPlatformScaleFactor (compPeer->getPlatformScaleFactor());
        }
        else if (Component* const parent = comp->getParentComponent())
        {
            parent->addChildComponent (this);
        }
    }

    void paint (Graphics& g) override
    {
        if (Component* c = target)
            shadow.drawForRectangle (g, getLocalArea (c, c->getLocalBounds()));
    }

    float getDesktopScaleFactor() const override
    {
        if (target != nullptr)
            return target->getDesktopScaleFactor();

        return Component::getDesktopScaleFactor();
    }

private:
    WeakReference<Component> target;
    DropShadow shadow;

    HIGGS_DECLARE_NON_COPYABLE (ShadowWindow)
};

class DropShadower::ParentVisibilityChangedListener final : public ComponentListener
{
public:
    ParentVisibilityChangedListener (Component& r, ComponentListener& l)
        : root (&r), listener (&l)
    {
        updateParentHierarchy();
    }

    ~ParentVisibilityChangedListener() override
    {
        for (auto& compEntry : observedComponents)
            if (auto* comp = compEntry.get())
                comp->removeComponentListener (this);
    }

    void componentVisibilityChanged (Component& component) override
    {
        if (root != &component)
            listener->componentVisibilityChanged (*root);
    }

    void componentParentHierarchyChanged (Component& component) override
    {
        if (root == &component)
            updateParentHierarchy();
    }

private:
    class ComponentWithWeakReference
    {
    public:
        explicit ComponentWithWeakReference (Component& c)
            : ptr (&c), ref (&c) {}

        Component* get() const { return ref.get(); }

        bool operator< (const ComponentWithWeakReference& other) const { return ptr < other.ptr; }

    private:
        Component* ptr;
        WeakReference<Component> ref;
    };

    void updateParentHierarchy()
    {
        const auto lastSeenComponents = std::exchange (observedComponents, std::invoke ([&]
        {
            std::set<ComponentWithWeakReference> result;

            for (auto node = root; node != nullptr; node = node->getParentComponent())
                result.emplace (*node);

            return result;
        }));

        const auto withDifference = [] (const auto& rangeA, const auto& rangeB, auto&& callback)
        {
            std::vector<ComponentWithWeakReference> result;
            std::set_difference (rangeA.begin(), rangeA.end(), rangeB.begin(), rangeB.end(), std::back_inserter (result));

            for (const auto& item : result)
                if (auto* c = item.get())
                    callback (*c);
        };

        withDifference (lastSeenComponents, observedComponents, [this] (auto& comp) { comp.removeComponentListener (this); });
        withDifference (observedComponents, lastSeenComponents, [this] (auto& comp) { comp.addComponentListener (this); });
    }

    Component* root = nullptr;
    ComponentListener* listener = nullptr;
    std::set<ComponentWithWeakReference> observedComponents;

    HIGGS_DECLARE_NON_COPYABLE (ParentVisibilityChangedListener)
    HIGGS_DECLARE_NON_MOVEABLE (ParentVisibilityChangedListener)
};

class DropShadower::ScaleWatcher
{
public:
    explicit ScaleWatcher (DropShadower& shadower)
    {
        if (shadower.owner == nullptr)
            return;

        notifier.emplace (shadower.owner.get(), [&shadower] (auto x)
        {
            for (auto sw : shadower.shadowWindows)
            {
                if (! sw->isOnDesktop())
                    continue;

                if (auto* peer = sw->getPeer())
                    peer->setCustomPlatformScaleFactor (x);
            }

            shadower.updateShadows();
        });
    }

private:
    std::optional<NativeScaleFactorNotifier> notifier;
};

//==============================================================================
DropShadower::DropShadower (const DropShadow& ds)  : shadow (ds)  {}

DropShadower::~DropShadower()
{
    if (owner != nullptr)
    {
        owner->removeComponentListener (this);
        owner = nullptr;
    }

    updateParent();

    const ScopedValueSetter<bool> setter (reentrant, true);
    shadowWindows.clear();
}

void DropShadower::setOwner (Component* componentToFollow)
{
    if (componentToFollow != owner)
    {
        if (owner != nullptr)
            owner->removeComponentListener (this);

        // (the component can't be null)
        jassert (componentToFollow != nullptr);

        owner = componentToFollow;
        jassert (owner != nullptr);

        updateParent();
        owner->addComponentListener (this);

        // The visibility of `owner` is transitively affected by the visibility of its parents. Thus we need to trigger the
        // componentVisibilityChanged() event in case it changes for any of the parents.
        visibilityChangedListener = std::make_unique<ParentVisibilityChangedListener> (*owner,
                                                                                       static_cast<ComponentListener&> (*this));

        scaleWatcher = std::make_unique<ScaleWatcher> (*this);

        updateShadows();
    }
}

void DropShadower::updateParent()
{
    if (Component* p = lastParentComp)
        p->removeComponentListener (this);

    lastParentComp = owner != nullptr ? owner->getParentComponent() : nullptr;

    if (Component* p = lastParentComp)
        p->addComponentListener (this);
}

void DropShadower::componentMovedOrResized (Component& c, bool /*wasMoved*/, bool /*wasResized*/)
{
    if (owner == &c)
        updateShadows();
}

void DropShadower::componentBroughtToFront (Component& c)
{
    if (owner == &c)
        updateShadows();
}

void DropShadower::componentChildrenChanged (Component&)
{
    updateShadows();
}

void DropShadower::componentParentHierarchyChanged (Component& c)
{
    if (owner == &c)
    {
        updateParent();
        updateShadows();
    }
}

void DropShadower::componentVisibilityChanged (Component& c)
{
    if (owner == &c)
        updateShadows();
}

void DropShadower::updateShadows()
{
    if (reentrant)
        return;

    const ScopedValueSetter<bool> setter (reentrant, true);

    if (owner != nullptr
        && owner->isShowing()
        && owner->getWidth() > 0 && owner->getHeight() > 0
        && (Desktop::canUseSemiTransparentWindows() || owner->getParentComponent() != nullptr))
    {
        while (shadowWindows.size() < 4)
            shadowWindows.add (new ShadowWindow (owner, shadow));

        const auto shadowEdge = jmax (shadow.offset.x, shadow.offset.y) + shadow.radius;
        const auto w = owner->getWidth();
        const auto h = owner->getHeight();

        for (int i = 4; --i >= 0;)
        {
            // there seem to be rare situations where the dropshadower may be deleted by
            // callbacks during this loop, so use a weak ref to watch out for this
            const WeakReference sw (shadowWindows[i]);

            if (sw != nullptr)
            {
                sw->setAlwaysOnTop (owner->isAlwaysOnTop());

                if (sw == nullptr)
                    return;

                const auto shadowBounds = std::invoke ([&]
                {
                    switch (i)
                    {
                        case 0: return Rectangle { shadowEdge, h + shadowEdge + shadowEdge }.withPosition ({ -shadowEdge, -shadowEdge });
                        case 1: return Rectangle { shadowEdge, h + shadowEdge + shadowEdge }.withPosition ({ w, -shadowEdge });
                        case 2: return Rectangle { w, shadowEdge }.withPosition ({ 0, -shadowEdge });
                        case 3: return Rectangle { w, shadowEdge }.withPosition ({ 0, h });
                        default: break;
                    }

                    return Rectangle<int>{};
                });

                {
                    using Scope = decltype (std::declval<ComponentPeer>().setMultimonitorPositionOverride ({}));
                    const auto scope = std::invoke ([&]() -> std::optional<Scope>
                    {
                        auto* peer = owner->isOnDesktop() ? owner->getPeer() : nullptr;

                        if (peer == nullptr)
                            return std::nullopt;

                        auto* shadowPeer = sw->getPeer();

                        if (shadowPeer == nullptr)
                            return std::nullopt;

                        using SH = detail::ScalingHelpers;
                        const auto localPos = SH::scaledScreenPosToUnscaled (*owner, shadowBounds.getPosition().toFloat());
                        const auto multimonitorPos = peer->localToMultimonitor (localPos);

                        return shadowPeer->setMultimonitorPositionOverride (multimonitorPos.roundToInt());
                    });

                    sw->setBounds (shadowBounds + owner->getPosition());
                }

                if (sw == nullptr)
                    return;

                sw->toBehind (i == 3 ? owner.get() : shadowWindows.getUnchecked (i + 1));
            }
        }
    }
    else
    {
        shadowWindows.clear();
    }
}

} // namespace higgs
