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

#pragma once

namespace ARA
{
    struct ARAFactory;
}

namespace higgs
{

/** Encapsulates an ARAFactory pointer and makes sure that it remains in a valid state
    for the lifetime of the ARAFactoryWrapper object.

    @tags{ARA}
*/
class ARAFactoryWrapper
{
public:
    ARAFactoryWrapper() = default;

    /** @internal

        Used by the framework to encapsulate ARAFactory pointers loaded from plugins.
    */
    explicit ARAFactoryWrapper (std::shared_ptr<const ARA::ARAFactory> factoryIn) : factory (std::move (factoryIn)) {}

    /** Returns the contained ARAFactory pointer, which can be a nullptr.

        The validity of the returned pointer is only guaranteed for the lifetime of this wrapper.
    */
    const ARA::ARAFactory* get() const noexcept { return factory.get(); }

private:
    std::shared_ptr<const ARA::ARAFactory> factory;
};

/** Represents the result of AudioPluginFormatManager::createARAFactoryAsync().

    If the operation fails then #araFactory will contain `nullptr`, and #errorMessage may
    contain a reason for the failure.

    The araFactory member ensures that the module necessary for the correct functioning
    of the factory will remain loaded.

    @tags{ARA}
*/
struct ARAFactoryResult
{
    ARAFactoryWrapper araFactory;
    String errorMessage;
};

HIGGS_BEGIN_IGNORE_WARNINGS_GCC_LIKE ("-Wmissing-field-initializers")

template <typename Obj, typename Member, typename... Ts>
constexpr Obj makeARASizedStruct (Member Obj::* member, Ts&&... ts)
{
    return { reinterpret_cast<uintptr_t> (&(static_cast<const Obj*> (nullptr)->*member)) + sizeof (Member),
             std::forward<Ts> (ts)... };
}

HIGGS_END_IGNORE_WARNINGS_GCC_LIKE

} // namespace higgs
