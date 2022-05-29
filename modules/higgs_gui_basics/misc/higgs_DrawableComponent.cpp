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

DrawableComponent::DrawableComponent (Drawable& drawableIn, BoundsToEnclose boundsToEncloseIn)
    : drawable (drawableIn), boundsToEnclose (boundsToEncloseIn)
{
    drawable.addListener (this);
    resetComponentBoundsTo (boundsToEnclose == BoundsToEnclose::drawableBounds ? drawable.getDrawableBounds()
                                                                               : drawable.getContentBounds());

    setInterceptsMouseClicks (false, false);
    setPaintingIsUnclipped (true);
    setAccessible (false);
}

DrawableComponent::DrawableComponent (Drawable& drawableIn)
    : DrawableComponent (drawableIn, BoundsToEnclose::drawableBounds)
{
}

DrawableComponent::~DrawableComponent()
{
    drawable.removeListener (this);
}

void DrawableComponent::setTransformToFit (const Rectangle<float>& areaInParent, RectanglePlacement placement)
{
    if (areaInParent.isEmpty())
        return;

    const auto boundsToFit = boundsToEnclose == BoundsToEnclose::drawableBounds ? drawable.getDrawableBounds()
                                                                                : drawable.getContentBounds();
    setTransform (placement.getTransformToFit (boundsToFit, areaInParent));
}

Path DrawableComponent::getOutlineAsPath() const
{
    auto p = drawable.getOutlineAsPath();
    p.applyTransform (getTransform());
    return p;
}

void DrawableComponent::paint (Graphics& g)
{
    g.setOrigin (originRelativeToComponent);
    drawable.draw (g, 1.0f);
}

bool DrawableComponent::hitTest (int x, int y)
{
    if (Component::hitTest (x, y))
        return true;

    bool allowsClicksOnThisComponent, allowsClicksOnChildComponents;
    getInterceptsMouseClicks (allowsClicksOnThisComponent, allowsClicksOnChildComponents);

    if (! allowsClicksOnChildComponents)
        return false;

    const Point<int> p { x - originRelativeToComponent.x, y - originRelativeToComponent.y };
    return drawable.hitTest (p.toFloat());
}

class DrawableTextAccessibilityHandler final : public AccessibilityHandler
{
public:
    DrawableTextAccessibilityHandler (Component& c, const String& textToWrap)
        : AccessibilityHandler (c, AccessibilityRole::staticText),
          text (textToWrap)
    {
    }

    String getTitle() const override
    {
        return text;
    }

private:
    String text;
};

std::unique_ptr<AccessibilityHandler> DrawableComponent::createAccessibilityHandler()
{
    if (drawable.isImage())
        return std::make_unique<AccessibilityHandler> (*this, AccessibilityRole::image);

    if (auto t = drawable.getContainedText(); t.has_value())
        return std::make_unique<DrawableTextAccessibilityHandler> (*this, *t);

    return Component::createAccessibilityHandler();
}

void DrawableComponent::resetComponentBoundsTo (Rectangle<float> bounds)
{
    const auto drawableBounds = bounds.getSmallestIntegerContainer();
    originRelativeToComponent = -drawableBounds.getPosition();
    setBounds (drawableBounds);
}

void DrawableComponent::drawableBoundsChanged (Drawable* d)
{
    if (boundsToEnclose == BoundsToEnclose::drawableBounds)
        resetComponentBoundsTo (d->getDrawableBounds());
}

} // namespace higgs
