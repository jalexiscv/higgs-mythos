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

void BorderedComponentBoundsConstrainer::checkBounds (Rectangle<int>& bounds,
                                                      const Rectangle<int>& previousBounds,
                                                      const Rectangle<int>& limits,
                                                      bool isStretchingTop,
                                                      bool isStretchingLeft,
                                                      bool isStretchingBottom,
                                                      bool isStretchingRight)
{
    if (auto* decorated = getWrappedConstrainer())
    {
        const auto border = getAdditionalBorder();
        const auto requestedBounds = bounds;

        border.subtractFrom (bounds);
        decorated->checkBounds (bounds,
                                border.subtractedFrom (previousBounds),
                                limits,
                                isStretchingTop,
                                isStretchingLeft,
                                isStretchingBottom,
                                isStretchingRight);
        border.addTo (bounds);
        bounds = bounds.withPosition (requestedBounds.getPosition());

        if (isStretchingTop && ! isStretchingBottom)
            bounds = bounds.withBottomY (previousBounds.getBottom());

        if (! isStretchingTop && isStretchingBottom)
            bounds = bounds.withY (previousBounds.getY());

        if (isStretchingLeft && ! isStretchingRight)
            bounds = bounds.withRightX (previousBounds.getRight());

        if (! isStretchingLeft && isStretchingRight)
            bounds = bounds.withX (previousBounds.getX());
    }
    else
    {
        ComponentBoundsConstrainer::checkBounds (bounds,
                                                 previousBounds,
                                                 limits,
                                                 isStretchingTop,
                                                 isStretchingLeft,
                                                 isStretchingBottom,
                                                 isStretchingRight);
    }
}

} // namespace higgs
