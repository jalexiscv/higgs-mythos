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
ScopedMessageBox::ScopedMessageBox() = default;

ScopedMessageBox::ScopedMessageBox (std::shared_ptr<detail::ScopedMessageBoxImpl> i)
    : impl (std::move (i)) {}

ScopedMessageBox::~ScopedMessageBox() noexcept
{
    close();
}

ScopedMessageBox::ScopedMessageBox (ScopedMessageBox&& other) noexcept
    : impl (std::exchange (other.impl, nullptr)) {}

ScopedMessageBox& ScopedMessageBox::operator= (ScopedMessageBox&& other) noexcept
{
    ScopedMessageBox temp (std::move (other));
    std::swap (temp.impl, impl);
    return *this;
}

void ScopedMessageBox::close()
{
    if (impl != nullptr)
        impl->close();

    impl.reset();
}

} // namespace higgs
