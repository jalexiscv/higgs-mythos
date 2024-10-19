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
    An interface for types that implement responses for certain message types.

    @tags{Audio}
*/
class ResponderDelegate
{
public:
    ResponderDelegate() = default;
    virtual ~ResponderDelegate() = default;

    /** If the message is processed successfully, and a response sent, then
        this returns true. Otherwise, returns false, allowing other ResponderDelegates
        to attempt to handle the message if necessary.
    */
    virtual bool tryRespond (ResponderOutput& output, const Message::Parsed& message) = 0;

    HIGGS_DECLARE_NON_COPYABLE (ResponderDelegate)
    HIGGS_DECLARE_NON_MOVEABLE (ResponderDelegate)
};

} // namespace higgs::midi_ci
