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

ArrowButton::ArrowButton (const String& name, float arrowDirectionInRadians, Colour arrowColour)
   : Button (name), colour (arrowColour)
{
    path.addTriangle (0.0f, 0.0f, 0.0f, 1.0f, 1.0f, 0.5f);
    path.applyTransform (AffineTransform::rotation (MathConstants<float>::twoPi * arrowDirectionInRadians, 0.5f, 0.5f));
}

ArrowButton::~ArrowButton() {}

void ArrowButton::paintButton (Graphics& g, bool /*shouldDrawButtonAsHighlighted*/, bool shouldDrawButtonAsDown)
{
    Path p (path);

    const float offset = shouldDrawButtonAsDown ? 1.0f : 0.0f;
    p.applyTransform (path.getTransformToScaleToFit (offset, offset, (float) getWidth() - 3.0f, (float) getHeight() - 3.0f, false));

    DropShadow (Colours::black.withAlpha (0.3f), shouldDrawButtonAsDown ? 2 : 4, Point<int>()).drawForPath (g, p);

    g.setColour (colour);
    g.fillPath (p);
}

} // namespace higgs
