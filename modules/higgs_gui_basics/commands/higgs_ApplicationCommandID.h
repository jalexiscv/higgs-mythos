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
/** A type used to hold the unique ID for an application command.

    This is a numeric type, so it can be stored as an integer.

    @see ApplicationCommandInfo, ApplicationCommandManager,
         ApplicationCommandTarget, KeyPressMappingSet
*/
using CommandID = int;


//==============================================================================
/** A set of general-purpose application command IDs.

    Because these commands are likely to be used in most apps, they're defined
    here to help different apps to use the same numeric values for them.

    Of course you don't have to use these, but some of them are used internally by
    HIGGS - e.g. the quit ID is recognised as a command by the HIGGSApplication class.

    @see ApplicationCommandInfo, ApplicationCommandManager,
         ApplicationCommandTarget, KeyPressMappingSet
*/
namespace StandardApplicationCommandIDs
{
    enum
    {
        /** This command ID should be used to send a "Quit the App" command.

            This command is recognised by the HIGGSApplication class, so if it is invoked
            and no other ApplicationCommandTarget handles the event first, the HIGGSApplication
            object will catch it and call HIGGSApplicationBase::systemRequestedQuit().
        */
        quit           = 0x1001,

        /** The command ID that should be used to send a "Delete" command. */
        del            = 0x1002,

        /** The command ID that should be used to send a "Cut" command. */
        cut            = 0x1003,

        /** The command ID that should be used to send a "Copy to clipboard" command. */
        copy           = 0x1004,

        /** The command ID that should be used to send a "Paste from clipboard" command. */
        paste          = 0x1005,

        /** The command ID that should be used to send a "Select all" command. */
        selectAll      = 0x1006,

        /** The command ID that should be used to send a "Deselect all" command. */
        deselectAll    = 0x1007,

        /** The command ID that should be used to send a "undo" command. */
        undo           = 0x1008,

        /** The command ID that should be used to send a "redo" command. */
        redo           = 0x1009
    };
}

} // namespace higgs
