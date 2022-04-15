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

namespace higgs::detail
{

/*
    Instances of this type can show and dismiss a message box.

    This is an interface rather than a concrete type so that platforms can pick an implementation at
    runtime if necessary.
*/
struct ScopedMessageBoxInterface
{
    virtual ~ScopedMessageBoxInterface() = default;

    /*  Shows the message box.

        When the message box exits normally, it should send the result to the passed-in function.
        The passed-in function is safe to call from any thread at any time.
    */
    virtual void runAsync (std::function<void (std::optional<int>)>) = 0;

    /*  Shows the message box and blocks. */
    virtual std::optional<int> runSync() = 0;

    /*  Forcefully closes the message box.

        This will be called when the message box handle has fallen out of scope.
        If the message box has already been closed by the user, this shouldn't do anything.
    */
    virtual void close() = 0;

    /*  Implemented differently for each platform. */
    static std::unique_ptr<ScopedMessageBoxInterface> create (const MessageBoxOptions& options);
};

} // namespace higgs::detail
