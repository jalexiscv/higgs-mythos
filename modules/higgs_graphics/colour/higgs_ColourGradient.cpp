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

ColourGradient::ColourGradient() noexcept  : isRadial (false)
{
   #if HIGGS_DEBUG
    point1.setX (987654.0f);
    #define HIGGS_COLOURGRADIENT_CHECK_COORDS_INITIALISED jassert (! exactlyEqual (point1.x, 987654.0f));
   #else
    #define HIGGS_COLOURGRADIENT_CHECK_COORDS_INITIALISED
   #endif
}

ColourGradient::ColourGradient (Colour colour1, float x1, float y1,
                                Colour colour2, float x2, float y2, bool radial)
    : ColourGradient (colour1, Point<float> (x1, y1),
                      colour2, Point<float> (x2, y2), radial)
{
}

ColourGradient::ColourGradient (Colour colour1, Point<float> p1,
                                Colour colour2, Point<float> p2, bool radial)
    : point1 (p1),
      point2 (p2),
      isRadial (radial)
{
    colours.add (ColourPoint { 0.0, colour1 },
                 ColourPoint { 1.0, colour2 });
}

ColourGradient ColourGradient::vertical (Colour c1, float y1, Colour c2, float y2)
{
    return { c1, 0, y1, c2, 0, y2, false };
}

ColourGradient ColourGradient::horizontal (Colour c1, float x1, Colour c2, float x2)
{
    return { c1, x1, 0, c2, x2, 0, false };
}

struct PointComparisons
{
    auto tie() const { return std::tie (point->x, point->y); }

    bool operator== (const PointComparisons& other) const { return tie() == other.tie(); }
    bool operator!= (const PointComparisons& other) const { return tie() != other.tie(); }
    bool operator<  (const PointComparisons& other) const { return tie() <  other.tie(); }

    const Point<float>* point = nullptr;
};

struct ColourGradient::ColourPointArrayComparisons
{
    bool operator== (const ColourPointArrayComparisons& other) const { return *array == *other.array; }
    bool operator!= (const ColourPointArrayComparisons& other) const { return *array != *other.array; }

    bool operator<  (const ColourPointArrayComparisons& other) const
    {
        return std::lexicographical_compare (array->begin(), array->end(), other.array->begin(), other.array->end());
    }

    const Array<ColourGradient::ColourPoint>* array = nullptr;
};

auto ColourGradient::tie() const
{
    return std::tuple (PointComparisons { &point1 },
                       PointComparisons { &point2 },
                       isRadial,
                       ColourPointArrayComparisons { &colours });
}

bool ColourGradient::operator== (const ColourGradient& other) const noexcept { return tie() == other.tie(); }
bool ColourGradient::operator!= (const ColourGradient& other) const noexcept { return tie() != other.tie(); }

bool ColourGradient::operator<  (const ColourGradient& other) const noexcept { return tie() <  other.tie(); }
bool ColourGradient::operator<= (const ColourGradient& other) const noexcept { return tie() <= other.tie(); }
bool ColourGradient::operator>  (const ColourGradient& other) const noexcept { return tie() >  other.tie(); }
bool ColourGradient::operator>= (const ColourGradient& other) const noexcept { return tie() >= other.tie(); }

//==============================================================================
void ColourGradient::clearColours()
{
    colours.clear();
}

int ColourGradient::addColour (const double proportionAlongGradient, Colour colour)
{
    // must be within the two end-points
    jassert (proportionAlongGradient >= 0 && proportionAlongGradient <= 1.0);

    if (proportionAlongGradient <= 0)
    {
        colours.set (0, { 0.0, colour });
        return 0;
    }

    auto pos = jmin (1.0, proportionAlongGradient);

    int i;
    for (i = 0; i < colours.size(); ++i)
        if (colours.getReference (i).position > pos)
            break;

    colours.insert (i, { pos, colour });
    return i;
}

void ColourGradient::removeColour (int index)
{
    jassert (isPositiveAndBelow (index, colours.size()));
    colours.remove (index);
}

void ColourGradient::multiplyOpacity (const float multiplier) noexcept
{
    for (auto& c : colours)
        c.colour = c.colour.withMultipliedAlpha (multiplier);
}

//==============================================================================
int ColourGradient::getNumColours() const noexcept
{
    return colours.size();
}

double ColourGradient::getColourPosition (int index) const noexcept
{
    if (isPositiveAndBelow (index, colours.size()))
        return colours.getReference (index).position;

    return 0;
 }

Colour ColourGradient::getColour (int index) const noexcept
{
    if (isPositiveAndBelow (index, colours.size()))
        return colours.getReference (index).colour;

    return {};
}

void ColourGradient::setColour (int index, Colour newColour) noexcept
{
    if (isPositiveAndBelow (index, colours.size()))
        colours.getReference (index).colour = newColour;
}

Colour ColourGradient::getColourAtPosition (double position) const noexcept
{
    jassert (approximatelyEqual (colours.getReference (0).position, 0.0)); // the first colour specified has to go at position 0

    if (position <= 0 || colours.size() <= 1)
        return colours.getReference (0).colour;

    int i = colours.size() - 1;
    while (position < colours.getReference (i).position)
        --i;

    auto& p1 = colours.getReference (i);

    if (i >= colours.size() - 1)
        return p1.colour;

    auto& p2 = colours.getReference (i + 1);

    return p1.colour.interpolatedWith (p2.colour, (float) ((position - p1.position) / (p2.position - p1.position)));
}

//==============================================================================
void ColourGradient::createLookupTable (PixelARGB* const lookupTable, const int numEntries) const noexcept
{
    HIGGS_COLOURGRADIENT_CHECK_COORDS_INITIALISED // Trying to use this object without setting its coordinates?
    jassert (colours.size() >= 2);
    jassert (numEntries > 0);

    auto paddedColours = colours;
    const auto colour0 = colours.getReference (0);

    if (! approximatelyEqual (colour0.position, 0.0))
        paddedColours.insert (0, { 0.0, colour0.colour });

    int index = 0;

    for (int j = 0; j < paddedColours.size() - 1; ++j)
    {
        const auto& o = paddedColours.getReference (j + 0);
        const auto& p = paddedColours.getReference (j + 1);
        const auto numToDo = roundToInt (p.position * (numEntries - 1)) - index;
        const auto pix1 = o.colour.getNonPremultipliedPixelARGB();
        const auto pix2 = p.colour.getNonPremultipliedPixelARGB();

        for (auto i = 0; i < numToDo; ++i)
        {
            auto blended = pix1;
            blended.tween (pix2, (uint32) ((i << 8) / numToDo));
            blended.premultiply();

            jassert (0 <= index && index < numEntries);
            lookupTable[index++] = blended;
        }
    }

    std::fill (lookupTable + index, lookupTable + numEntries, paddedColours.getLast().colour.getPixelARGB());
}

int ColourGradient::createLookupTable (const AffineTransform& transform, HeapBlock<PixelARGB>& lookupTable) const
{
    HIGGS_COLOURGRADIENT_CHECK_COORDS_INITIALISED // Trying to use this object without setting its coordinates?
    jassert (colours.size() >= 2);

    const auto maxExtent = std::invoke ([&]
    {
        if (! isRadial)
        {
            return 3 * (int) point1.transformedBy (transform)
                                   .getDistanceFrom (point2.transformedBy (transform));
        }

        detail::RadialGradientView rg { this };

        // If the transform is skewed the maximum extent could be in any direction
        const auto r = rg.getEndCircle().r;
        Point<float> extentX { r, 0.0f };
        Point<float> extentY { 0.0f, r };

        return roundToInt (jmax (extentX.transformedBy (transform).getDistanceFromOrigin(),
                                 extentY.transformedBy (transform).getDistanceFromOrigin()));
    });

    const auto numEntries = jlimit (1, jmax (1, (colours.size() - 1) << 8), 3 * (int) maxExtent);
    lookupTable.malloc (numEntries);
    createLookupTable (lookupTable, numEntries);
    return numEntries;
}

bool ColourGradient::isOpaque() const noexcept
{
    for (auto& c : colours)
        if (! c.colour.isOpaque())
            return false;

    return true;
}

bool ColourGradient::isInvisible() const noexcept
{
    for (auto& c : colours)
        if (! c.colour.isTransparent())
            return false;

    return true;
}

auto ColourGradient::ColourPoint::tie() const
{
    return std::tuple (position, colour.getPixelARGB().getNativeARGB());
}

bool ColourGradient::ColourPoint::operator== (ColourPoint other) const noexcept
{
    return tie() == other.tie();
}

bool ColourGradient::ColourPoint::operator!= (ColourPoint other) const noexcept
{
    return tie() != other.tie();
}

bool ColourGradient::ColourPoint::operator<  (ColourPoint other) const noexcept
{
    return tie() <  other.tie();
}

} // namespace higgs
