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

#if HiggsPlugin_Enable_ARA

// Include ARA SDK headers
HIGGS_BEGIN_IGNORE_WARNINGS_GCC_LIKE ("-Wgnu-zero-variadic-macro-arguments",
                                     "-Wunused-parameter",
                                     "-Wfloat-equal")
HIGGS_BEGIN_IGNORE_WARNINGS_MSVC (6387)

#include <ARA_Library/PlugIn/ARAPlug.h>

HIGGS_END_IGNORE_WARNINGS_GCC_LIKE
HIGGS_END_IGNORE_WARNINGS_MSVC

namespace higgs
{

using ARAViewSelection = ARA::PlugIn::ViewSelection;
using ARAContentUpdateScopes = ARA::ContentUpdateScopes;
using ARARestoreObjectsFilter = ARA::PlugIn::RestoreObjectsFilter;
using ARAStoreObjectsFilter = ARA::PlugIn::StoreObjectsFilter;

/** Converts an ARA::ARAUtf8String to a HIGGS String. */
inline String convertARAString (ARA::ARAUtf8String str)
{
    return String (CharPointer_UTF8 (str));
}

/** Converts a potentially NULL ARA::ARAUtf8String to a HIGGS String.

    Returns the HIGGS equivalent of the provided string if it's not nullptr, and the fallback string
    otherwise.
*/
inline String convertOptionalARAString (ARA::ARAUtf8String str, const String& fallbackString = String())
{
    return (str != nullptr) ? convertARAString (str) : fallbackString;
}

/** Converts an ARA::ARAColor* to a HIGGS Colour. */
inline uint32 convertARAColourARGB (const ARA::ARAColor* colour)
{
    static constexpr auto floatToByte = [] (float x)
    {
        return (uint32) jlimit (0, 255, roundToInt (x * 255.0f));
    };

    return 0xff000000
         | (floatToByte (colour->r) << 0x10)
         | (floatToByte (colour->g) << 0x08)
         | (floatToByte (colour->b) << 0x00);
}

/** Converts a potentially NULL ARA::ARAColor* to a HIGGS Colour.

    Returns the HIGGS equivalent of the provided colour if it's not nullptr, and the fallback colour
    otherwise.
*/
inline uint32 convertOptionalARAColour (const ARA::ARAColor* colour, uint32 fallbackColour = {})
{
    return (colour != nullptr) ? convertARAColourARGB (colour) : fallbackColour;
}

} // namespace higgs

#include "higgs_ARAModelObjects.h"
#include "higgs_ARADocumentController.h"
#include "higgs_AudioProcessor_ARAExtensions.h"
#include "higgs_ARAPlugInInstanceRoles.h"

#endif
