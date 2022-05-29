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

/**
    Helper class that owns a Drawable object and exposes it as a DrawableComponent. This class is
    meant to ease the transition from HIGGS 8 to HIGGS 9, where Drawable no longer inherits from
    Component and DrawableComponent takes no ownership of the Drawable object.

    New projects should avoid using this class if possible.

    The new suggested way to work with Drawable objects is to use them directly and draw them using
    the Drawable::draw() method. This way a heavy dependency on the Component class, and potentially
    the higgs_gui_basics module can be avoided.

    @see Drawable

    @tags{GUI}
*/
class HIGGS_API  OwningDrawableComponent : private std::unique_ptr<Drawable>,
                                          public DrawableComponent
{
public:
    /** Creates an OwningDrawableComponent that takes ownership of the underlying Drawable object.
    */
    static std::unique_ptr<OwningDrawableComponent> create (std::unique_ptr<Drawable> d, BoundsToEnclose boundsToEnclose)
    {
        if (d == nullptr)
            return {};

        return rawToUniquePtr (new OwningDrawableComponent (std::move (d), boundsToEnclose));
    }

    /** Creates an OwningDrawableComponent that takes ownership of the underlying Drawable object.
    */
    static std::unique_ptr<OwningDrawableComponent> create (std::unique_ptr<Drawable> d)
    {
        return create (std::move (d), BoundsToEnclose::drawableBounds);
    }

    /** Creates an OwningDrawableComponent that copies a Drawable and takes ownership of the copied
        object.
    */
    static std::unique_ptr<OwningDrawableComponent> createFromCopy (const Drawable* const d, BoundsToEnclose boundsToEnclose)
    {
        if (d == nullptr)
            return {};

        auto copy = d->createCopy();

        if (copy == nullptr)
            return {};

        return rawToUniquePtr (new OwningDrawableComponent (std::move (copy), boundsToEnclose));
    }

    /** Creates an OwningDrawableComponent that copies a Drawable and takes ownership of the copied
        object.
    */
    static std::unique_ptr<OwningDrawableComponent> createFromCopy (const Drawable* const d)
    {
        return createFromCopy (d, BoundsToEnclose::drawableBounds);
    }

    /** Attempts to parse an SVG (Scalable Vector Graphics) document from a file.
    */
    static std::unique_ptr<OwningDrawableComponent> createFromSVGFile (const File& svgFile)
    {
        return create (Drawable::createFromSVGFile (svgFile));
    }

    /** Attempts to parse an SVG (Scalable Vector Graphics) document from a string.
    */
    static std::unique_ptr<OwningDrawableComponent> createFromSVGString (const String& svgString)
    {
        return create (Drawable::createFromSVGString (svgString));
    }

private:
    OwningDrawableComponent (std::unique_ptr<Drawable> drawableIn, BoundsToEnclose boundsToEncloseIn)
        : unique_ptr (std::move (drawableIn)),
          DrawableComponent (*get(), boundsToEncloseIn)
    {
    }

    OwningDrawableComponent (std::unique_ptr<Drawable> drawableIn)
        : OwningDrawableComponent (std::move (drawableIn), BoundsToEnclose::drawableBounds)
    {
    }
};

} // namespace higgs
