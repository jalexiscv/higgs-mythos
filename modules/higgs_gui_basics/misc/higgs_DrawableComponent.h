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

/** Specifies the source bounds that a DrawableComponent should fit itself around.

    New projects should use contentBounds, as this will respect the designer's intent when
    specifying a viewBox in an SVG file. Even better, avoid using DrawableComponent
    entirely, which is meant as a backwards compatibility class for projects migrating from
    HIGGS 8.

    @see DrawableComponent
*/
enum class BoundsToEnclose
{
    drawableBounds, ///< The bounds returned by Drawable::getDrawableBounds(). These are the
                    ///< smallest rectangular bounds covering visible features of the Drawable.
                    ///< The Drawable class prior to HIGGS 9 used these bounds for fitting.
    contentBounds   ///< The bounds returned by Drawable::getContentBounds(). For Drawables
                    ///< that were created by parsing an SVG, this will return the viewBox, which
                    ///< expresses the designer's intent for fitting purposes. The fallback
                    ///< value is the same as the one returned by Drawable::getDrawableBounds().
};

class HIGGS_API DrawableComponent : public Component,
                                   private Drawable::Listener
{
public:
    /** Wraps a Drawable object in a Component. */
    explicit DrawableComponent (Drawable& drawableIn);

    /** Wraps a Drawable object in a Component. */
    DrawableComponent (Drawable& drawableIn, BoundsToEnclose boundsToEnclose);

    /** Destructor. */
    ~DrawableComponent() override;

    /** Sets a transform for this drawable that will position it within the specified
        area of its parent component.
    */
    void setTransformToFit (const Rectangle<float>& areaInParent, RectanglePlacement placement);

    /** Creates a path that describes the outline of this DrawableComponent. */
    Path getOutlineAsPath() const;

    /** Returns a reference to the Drawable object displayed by this Component. */
    Drawable& getDrawable()
    {
        return drawable;
    }

    /** Returns a reference to the Drawable object displayed by this Component. */
    const Drawable& getDrawable() const
    {
        return drawable;
    }

    //==============================================================================
    void paint (Graphics& g) override;

    bool hitTest (int x, int y) override;

    std::unique_ptr<AccessibilityHandler> createAccessibilityHandler() override;

    //==============================================================================
    /** @internal

        Intended to replace Drawable::setBoundsToEnclose which is only ever called in conjunction
        with getDrawableBounds().
    */
    void resetComponentBoundsTo (Rectangle<float> bounds);

private:
    void drawableBoundsChanged (Drawable*) override;

    Drawable& drawable;
    Point<int> originRelativeToComponent;
    BoundsToEnclose boundsToEnclose;
};

} // namespace higgs
