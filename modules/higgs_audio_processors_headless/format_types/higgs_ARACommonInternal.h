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

#if HIGGS_INTERNAL_HAS_ARA

#include <ARA_API/ARAInterface.h>

namespace higgs
{

static inline void dummyARAInterfaceAssert (ARA::ARAAssertCategory, const void*, const char*)
{}

static inline ARA::ARAInterfaceConfiguration createInterfaceConfig (const ARA::ARAFactory* araFactory)
{
    static auto* assertFunction = &dummyARAInterfaceAssert;

   #if ARA_VALIDATE_API_CALLS
    assertFunction = &::ARA::ARAInterfaceAssert;
    static std::once_flag flag;
    std::call_once (flag, [] { ARA::ARASetExternalAssertReference (&assertFunction); });
   #endif

    return makeARASizedStruct (&ARA::ARAInterfaceConfiguration::assertFunctionAddress,
                               jmin (araFactory->highestSupportedApiGeneration, (ARA::ARAAPIGeneration) ARA::kARAAPIGeneration_2_X_Draft),
                               &assertFunction);
}

/*  If the provided ARAFactory is not yet in use it constructs a new shared_ptr that will call the
    provided onDelete function inside the custom deleter of this new shared_ptr instance.

    The onDelete function is responsible for releasing the resources that guarantee the validity of
    the wrapped ARAFactory*.

    If however the ARAFactory is already in use the function will just return a copy of the already
    existing shared_ptr and call the onDelete function immediately. This is to ensure that the
    ARAFactory is only uninitialised when no plugin instance can be using it.

    On both platforms the onDelete function is used to release resources that ensure that the module
    providing the ARAFactory* remains loaded.
*/
static inline std::shared_ptr<const ARA::ARAFactory> getOrCreateARAFactory (const ARA::ARAFactory* ptr,
                                                                            std::function<void()> onDelete)
{
    HIGGS_ASSERT_MESSAGE_THREAD

    static std::unordered_map<const ARA::ARAFactory*, std::weak_ptr<const ARA::ARAFactory>> cache;

    auto& cachePtr = cache[ptr];

    if (const auto obj = cachePtr.lock())
    {
        onDelete();
        return obj;
    }

    const auto interfaceConfig = createInterfaceConfig (ptr);
    ptr->initializeARAWithConfiguration (&interfaceConfig);
    const auto obj = std::shared_ptr<const ARA::ARAFactory> (ptr, [deleter = std::move (onDelete)] (const ARA::ARAFactory* factory)
                                                             {
                                                                 factory->uninitializeARA();
                                                                 deleter();
                                                             });
    cachePtr = obj;
    return obj;
}

}

#endif
