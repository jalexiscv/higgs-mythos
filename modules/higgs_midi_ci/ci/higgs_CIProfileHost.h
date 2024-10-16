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

namespace higgs::midi_ci
{

/**
    Acting as a ResponderListener, instances of this class can formulate
    appropriate replies to profile transactions initiated by remote devices.

    ProfileHost instances also contains methods to inform remote devices about
    changes to local profile state.

    Stores the current state of profiles on the local device.

    @tags{Audio}
*/
class ProfileHost final : public ResponderDelegate
{
public:
    /** @internal

        Rather than constructing one of these objects yourself, you should configure
        a Device with profile support, and then use Device::getProfileHost()
        to retrieve a profile host that has been set up to work with that device.
    */
    ProfileHost (FunctionBlock fb, ProfileDelegate& d, BufferOutput& o)
        : functionBlock (fb), delegate (d), output (o) {}

    /** Adds support for a profile on the specified group/channel with a
        maximum number of channels that may be activated.
    */
    void addProfile     (ProfileAtAddress, int maxNumChannels = 1);

    /** Removes support for a profile on the specified group/channel.
    */
    void removeProfile  (ProfileAtAddress);

    /** Activates or deactivates a profile on the specified group/channel.

        The profile should previously have been added with addProfile().
        A positive value of numChannels will enable the profile, and a non-positive value
        will disable it. This includes group and function-block profiles; passing any positive
        value will enable the profile on the entire group or block.
    */
    void setProfileEnablement (ProfileAtAddress, int numChannels);

    /** Returns the profile states (supported/active) for all groups and channels.
    */
    const BlockProfileStates& getProfileStates() const { return states; }

    /** Returns the number of supported and active channels for the given
        profile on the specified group/channel.

        If the supported channels is 0, then the profile is not supported
        on the group/channel.

        If the active channels is 0, then the profile is inactive on the
        group/channel.
    */
    SupportedAndActive getState (ProfileAtAddress profileAtAddress) const
    {
        if (auto* state = states.getStateForDestination (profileAtAddress.address))
            return state->get (profileAtAddress.profile);

        return {};
    }

    /** @internal */
    bool tryRespond (ResponderOutput&, const Message::Parsed&) override;

private:
    class Visitor;

    void enableProfileImpl (ProfileAtAddress, int);
    void disableProfileImpl (ProfileAtAddress);

    template <typename Body>
    bool profileEnablementReceived (ResponderOutput&, const Body&);

    FunctionBlock functionBlock;
    ProfileDelegate& delegate;
    BufferOutput& output;
    BlockProfileStates states;
    bool isResponder = false;
    std::optional<ProfileAtAddress> currentEnablementMessage;
};

} // namespace higgs::midi_ci
