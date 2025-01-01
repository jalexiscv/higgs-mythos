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
    Similar to AnimatorUpdater, but automatically calls update() whenever the screen refreshes.

    @tags{Animations}
*/
class HIGGS_API  VBlankAnimatorUpdater : private AnimatorUpdater
{
public:
    /** Constructs a VBlankAnimatorUpdater that is synchronised to the refresh rate of the monitor
        that the provided Component is being displayed on.
    */
    explicit VBlankAnimatorUpdater (Component* c)
        : vBlankAttachment (c, [this] (double timestampSec)
                               {
                                   update (timestampSec * 1000.0);
                               })
    {
    }

    using AnimatorUpdater::addAnimator, AnimatorUpdater::removeAnimator;

private:
    VBlankAttachment vBlankAttachment;
};

} // namespace higgs
