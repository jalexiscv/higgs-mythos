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

Result::Result() noexcept {}

Result::Result (const String& message) noexcept
    : errorMessage (message)
{
}

Result::Result (const Result& other)
    : errorMessage (other.errorMessage)
{
}

Result& Result::operator= (const Result& other)
{
    errorMessage = other.errorMessage;
    return *this;
}

Result::Result (Result&& other) noexcept
    : errorMessage (std::move (other.errorMessage))
{
}

Result& Result::operator= (Result&& other) noexcept
{
    errorMessage = std::move (other.errorMessage);
    return *this;
}

bool Result::operator== (const Result& other) const noexcept
{
    return errorMessage == other.errorMessage;
}

bool Result::operator!= (const Result& other) const noexcept
{
    return errorMessage != other.errorMessage;
}

Result Result::fail (const String& errorMessage) noexcept
{
    return Result (errorMessage.isEmpty() ? "Unknown Error" : errorMessage);
}

const String& Result::getErrorMessage() const noexcept
{
    return errorMessage;
}

bool Result::wasOk() const noexcept         { return errorMessage.isEmpty(); }
Result::operator bool() const noexcept      { return errorMessage.isEmpty(); }
bool Result::failed() const noexcept        { return errorMessage.isNotEmpty(); }
bool Result::operator!() const noexcept     { return errorMessage.isNotEmpty(); }

} // namespace higgs
