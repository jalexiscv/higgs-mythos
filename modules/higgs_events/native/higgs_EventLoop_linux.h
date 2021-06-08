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

namespace higgs::LinuxEventLoop
{

    /** Registers a callback that will be called when a file descriptor is ready for I/O.

        This will add the given file descriptor to the internal set of file descriptors
        that will be passed to the poll() call. When this file descriptor has data to read
        the readCallback will be called.

        @param fd            the file descriptor to be monitored
        @param readCallback  a callback that will be called when the file descriptor has
                             data to read. The file descriptor will be passed as an argument
        @param eventMask     a bit mask specifying the events you are interested in for the
                             file descriptor. The possible values for this are defined in
                             <poll.h>
    */
    void registerFdCallback (int fd, std::function<void (int)> readCallback, short eventMask = 1 /*POLLIN*/);

    /** Unregisters a previously registered file descriptor.

        @see registerFdCallback
    */
    void unregisterFdCallback (int fd);

} // namespace higgs::LinuxEventLoop
