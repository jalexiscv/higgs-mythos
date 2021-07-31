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

Drawable::Drawable (const Drawable& other)
    : transform (other.transform),
      clipPath (other.clipPath != nullptr ? other.clipPath->createCopy() : nullptr),
      name (other.name),
      visible (other.visible)
{
}

void Drawable::applyDrawableClipPath (Graphics& g) const
{
    if (auto* drawable = clipPath.get())
    {
        auto path = drawable->getOutlineAsPath();

        if (! path.isEmpty())
            g.getInternalContext().clipToPath (path, {});
    }
}

//==============================================================================
void Drawable::draw (Graphics& g, float opacity, const AffineTransform& drawTransform) const
{
    Graphics::ScopedSaveState ss (g);

    g.addTransform (getTransform().followedBy (drawTransform));

    applyDrawableClipPath (g);

    if (! g.isClipEmpty())
    {
        if (opacity < 1.0f)
        {
            g.beginTransparencyLayer (opacity);
            paint (g);
            g.endTransparencyLayer();
        }
        else
        {
            paint (g);
        }
    }
}

void Drawable::drawAt (Graphics& g, float x, float y, float opacity) const
{
    draw (g, opacity, AffineTransform::translation (x, y));
}

void Drawable::drawWithin (Graphics& g, Rectangle<float> destArea,
                           RectanglePlacement placement, float opacity) const
{
    draw (g, opacity, placement.getTransformToFit (getDrawableBounds(), destArea));
}

//==============================================================================
void Drawable::setClipPath (std::unique_ptr<Drawable> clipPathIn)
{
    clipPath = std::move (clipPathIn);
}

//==============================================================================
bool Drawable::replaceColour (Colour, Colour)
{
    return false;
}

void Drawable::setDrawableTransform (const AffineTransform& newTransform)
{
    transform = newTransform;
}

void Drawable::setDrawableTransformToFit (const Rectangle<float>& area, RectanglePlacement placement)
{
    if (! area.isEmpty())
        setDrawableTransform (placement.getTransformToFit (getDrawableBoundsUntransformed(), area));
}

AffineTransform Drawable::getDrawableTransform() const
{
    return transform;
}

AffineTransform Drawable::getTransform() const
{
    return transform;
}

//==============================================================================
std::unique_ptr<Drawable> Drawable::createFromImageData (const void* data, const size_t numBytes)
{
    auto image = ImageFileFormat::loadFrom (data, numBytes);

    if (image.isValid())
        return std::make_unique<DrawableImage> (image);

    if (const auto s = String::createStringFromData (data, (int) numBytes); s.contains ("<svg"))
        return Drawable::createFromSVGString (s);

    return {};
}

std::unique_ptr<Drawable> Drawable::createFromImageDataStream (InputStream& dataSource)
{
    MemoryOutputStream mo;
    mo << dataSource;

    return createFromImageData (mo.getData(), mo.getDataSize());
}

std::unique_ptr<Drawable> Drawable::createFromImageFile (const File& file)
{
    FileInputStream fin (file);

    if (fin.openedOk())
        return createFromImageDataStream (fin);

    return {};
}

Rectangle<float> Drawable::getDrawableBounds() const
{
    return getDrawableBoundsUntransformed().transformedBy (getDrawableTransform());
}

Rectangle<float> Drawable::getContentBounds() const
{
    return getContentBoundsUntransformed().transformedBy (getDrawableTransform());
}

Rectangle<float> Drawable::getContentBoundsUntransformed() const
{
    return getDrawableBoundsUntransformed();
}

} // namespace higgs
