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

#if HIGGS_WINDOWS
using NativeFileHandle = void*;
#else
class NativeFileHandle
{
    static constexpr int invalidFd = -1;

public:
    int get() const noexcept { return fd; }
    bool isValid() const noexcept { return 0 <= fd; }

    void set (int newFd) noexcept { fd = newFd; }
    void invalidate() noexcept { fd = invalidFd; }

private:
    int fd = invalidFd;
};
#endif

} // namespace higgs::detail
