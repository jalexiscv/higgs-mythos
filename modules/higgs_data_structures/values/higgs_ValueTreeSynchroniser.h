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
    This class can be used to watch for all changes to the state of a ValueTree,
    and to convert them to a transmittable binary encoding.

    The purpose of this class is to allow two or more ValueTrees to be remotely
    synchronised by transmitting encoded changes over some kind of transport
    mechanism.

    To use it, you'll need to implement a subclass of ValueTreeSynchroniser
    and implement the stateChanged() method to transmit the encoded change (maybe
    via a network or other means) to a remote destination, where it can be
    applied to a target tree.

    @tags{DataStructures}
*/
class HIGGS_API  ValueTreeSynchroniser  : private ValueTree::Listener
{
public:
    /** Creates a ValueTreeSynchroniser that watches the given tree.

        After creating an instance of this class and somehow attaching it to
        a target tree, you probably want to call sendFullSyncCallback() to
        get them into a common starting state.
    */
    ValueTreeSynchroniser (const ValueTree& tree);

    /** Destructor. */
    ~ValueTreeSynchroniser() override;

    /** This callback happens when the ValueTree changes and the given state-change message
        needs to be applied to any other trees that need to stay in sync with it.
        The data is an opaque blob of binary that you should transmit to wherever your
        target tree lives, and use applyChange() to apply this to the target tree.
    */
    virtual void stateChanged (const void* encodedChange, size_t encodedChangeSize) = 0;

    /** Forces the sending of a full state message, which may be large, as it
        encodes the entire ValueTree.

        This will internally invoke stateChanged() with the encoded version of the state.
    */
    void sendFullSyncCallback();

    /** Applies an encoded change to the given destination tree.

        When you implement a receiver for changes that were sent by the stateChanged()
        message, this is the function that you'll need to call to apply them to the
        target tree that you want to be synced.
    */
    static bool applyChange (ValueTree& target,
                             const void* encodedChangeData, size_t encodedChangeDataSize,
                             UndoManager* undoManager);

    /** Returns the root ValueTree that is being observed. */
    const ValueTree& getRoot() noexcept       { return valueTree; }

private:
    ValueTree valueTree;

    void valueTreePropertyChanged (ValueTree&, const Identifier&) override;
    void valueTreeChildAdded (ValueTree&, ValueTree&) override;
    void valueTreeChildRemoved (ValueTree&, ValueTree&, int) override;
    void valueTreeChildOrderChanged (ValueTree&, int, int) override;

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ValueTreeSynchroniser)
};

} // namespace higgs
