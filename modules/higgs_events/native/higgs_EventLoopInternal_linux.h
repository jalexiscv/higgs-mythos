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

struct LinuxEventLoopInternal
{
    /**
        @internal

        Receives notifications when a fd callback is added or removed.

        This is useful for VST3 plugins that host other VST3 plugins. In this scenario, the 'inner'
        plugin may want to register additional file descriptors on top of those registered by the
        'outer' plugin. In order for this to work, the outer plugin must in turn pass this request
        on to the host, to indicate that the outer plugin wants to receive FD callbacks for both the
        inner and outer plugin.
    */
    struct Listener
    {
        virtual ~Listener() = default;
        virtual void fdCallbacksChanged() = 0;
    };

    /** @internal */
    static void registerLinuxEventLoopListener (Listener&);
    /** @internal */
    static void deregisterLinuxEventLoopListener (Listener&);
    /** @internal */
    static void invokeEventLoopCallbackForFd (int);
    /** @internal */
    static std::vector<int> getRegisteredFds();
};

} // namespace higgs
