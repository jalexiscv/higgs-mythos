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

//==============================================================================
/**
    A graphical effect filter that can be applied to components.

    An ImageEffectFilter can be applied to the image that a component
    paints before it hits the screen.

    This is used for adding effects like shadows, blurs, etc.

    @see Component::setComponentEffect

    @tags{Graphics}
*/
class HIGGS_API  ImageEffectFilter
{
public:
    //==============================================================================
    /** Overridden to render the effect.

        The implementation of this method must use the image that is passed in
        as its source, and should render its output to the graphics context passed in.

        @param sourceImage      the image that the source component has just rendered with
                                its paint() method. The image may or may not have an alpha
                                channel, depending on whether the component is opaque.
        @param destContext      the graphics context to use to draw the resultant image.
        @param scaleFactor      a scale factor that has been applied to the image - e.g. if
                                this is 2, then the image is actually scaled-up to twice the
                                original resolution
        @param alpha            the alpha with which to draw the resultant image to the
                                target context
    */
    virtual void applyEffect (Image& sourceImage,
                              Graphics& destContext,
                              float scaleFactor,
                              float alpha) = 0;

    /** Destructor. */
    virtual ~ImageEffectFilter() = default;

};

} // namespace higgs
