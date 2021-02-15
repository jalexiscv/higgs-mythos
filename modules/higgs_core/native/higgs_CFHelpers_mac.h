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

/* This file contains a few helper functions that are used internally but which
   need to be kept away from the public headers because they use obj-C symbols.
*/
namespace higgs
{

struct CFObjectDeleter
{
    void operator() (CFTypeRef object) const noexcept
    {
        if (object != nullptr)
            CFRelease (object);
    }
};

template <typename CFType>
struct CFRefRemover
{
    static_assert (std::is_pointer_v<CFType>, "CFType must be a Ref type");
    using Type = std::remove_pointer_t<CFType>;
};

template <typename CFType>
using CFRemoveRef = typename CFRefRemover<CFType>::Type;

template <typename CFType>
using CFUniquePtr = std::unique_ptr<CFRemoveRef<CFType>, CFObjectDeleter>;

template <typename CFType>
auto makeCFUniquePtr (CFType instance)
{
    return CFUniquePtr<CFType> { instance };
}

template <typename CFType>
struct CFObjectHolder
{
    CFObjectHolder() = default;
    explicit CFObjectHolder (CFType obj)  : object (obj) {}

    CFObjectHolder (const CFObjectHolder&) = delete;
    CFObjectHolder (CFObjectHolder&&) = delete;

    CFObjectHolder& operator= (const CFObjectHolder&) = delete;
    CFObjectHolder& operator= (CFObjectHolder&&) = delete;

    ~CFObjectHolder() noexcept
    {
        if (object != nullptr)
            CFRelease (object);
    }

    // Public to facilitate passing the pointer address to functions
    CFType object = nullptr;
};

} // namespace higgs
