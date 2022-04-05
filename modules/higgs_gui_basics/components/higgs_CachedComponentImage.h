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
    Base class used internally for structures that can store cached images of
    component state.

    Most people are unlikely to ever need to know about this class - it's really
    only for power-users!

    @see Component::setCachedComponentImage

    @tags{GUI}
*/
class HIGGS_API  CachedComponentImage
{
public:
    CachedComponentImage() = default;
    virtual ~CachedComponentImage() = default;

    //==============================================================================
    /** Called as part of the parent component's paint method, this must draw
        the given component into the target graphics context, using the cached
        version where possible.
    */
    virtual void paint (Graphics&) = 0;

    /** Invalidates all cached image data.
        @returns true if the peer should also be repainted, or false if this object
                 handles all repaint work internally.
    */
    virtual bool invalidateAll() = 0;

    /** Invalidates a section of the cached image data.
        @returns true if the peer should also be repainted, or false if this object
                 handles all repaint work internally.
    */
    virtual bool invalidate (const Rectangle<int>& area) = 0;

    /** Called to indicate that the component is no longer active, so
        any cached data should be released if possible.
    */
    virtual void releaseResources() = 0;
};

} // namespace higgs
