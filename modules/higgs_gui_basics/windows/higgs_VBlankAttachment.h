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
    Helper class to synchronise Component updates to the vertical blank event of the display that
    the Component is presented on. This is useful when animating the Component's contents.

    @tags{Animations}
*/
class HIGGS_API  VBlankAttachment final  : private ComponentPeer::VBlankListener,
                                          private ComponentListener
{
public:
    /** Default constructor for creating an empty object. */
    VBlankAttachment() = default;

    /** Constructor. Creates an attachment that will call the passed in function at every vertical
        blank event of the display that the passed in Component is currently visible on.

        The Component must be valid for the entire lifetime of the VBlankAttachment.

        You should prefer the other constructor, where the callback also receives a timestamp
        parameter. This constructor is only provided for compatibility with the earlier HIGGS
        implementation.
    */
    VBlankAttachment (Component* c, std::function<void()> callbackIn);

    /** Constructor. Creates an attachment that will call the passed in function at every vertical
        blank event of the display that the passed in Component is currently visible on.

        The Component must be valid for the entire lifetime of the VBlankAttachment.

        The provided callback is called with a monotonically increasing value expressed in seconds
        that corresponds to the time of the next frame to be presented. Use this value to
        synchronise drawing across all classes using a VBlankAttachment.
    */
    VBlankAttachment (Component* c, std::function<void (double)> callbackIn);
    VBlankAttachment (VBlankAttachment&& other);
    VBlankAttachment& operator= (VBlankAttachment&& other);

    /** Destructor. */
    ~VBlankAttachment() override;

    /** Returns true for a default constructed object. */
    bool isEmpty() const { return owner == nullptr; }

private:
    //==============================================================================
    void onVBlank (double timestampSec) override;

    //==============================================================================
    void componentParentHierarchyChanged (Component&) override;

    void updateOwner();
    void updatePeer();
    void cleanup();

    Component* owner = nullptr;
    Component* lastOwner = nullptr;
    std::function<void (double)> callback;
    ComponentPeer* lastPeer = nullptr;

    HIGGS_DECLARE_NON_COPYABLE (VBlankAttachment)
};

} // namespace higgs
