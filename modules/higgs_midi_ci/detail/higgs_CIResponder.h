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

namespace higgs::midi_ci::detail
{

/*
    Parses individual messages, and additionally gives ResponderDelegates a chance to formulate
    a response to any message that would normally necessitate a reply.
*/
struct Responder
{
    Responder() = delete;

    /*  Parses the message, then calls tryParse on each ResponderDelegate in
        turn until one returns true, indicating that the message has been
        handled. Most 'inquiry' messages should emit one or more reply messages.
        These replies will be written to the provided BufferOutput.
        If none of the provided delegates are able to handle the message, then
        a generic NAK will be written to the BufferOutput.
    */
    static Parser::Status processCompleteMessage (BufferOutput& output,
                                                  ump::BytesOnGroup message,
                                                  Span<ResponderDelegate* const> delegates);
};

} // namespace higgs::midi_ci
