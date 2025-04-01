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

#ifdef HIGGS_BUILD_TOOLS_H_INCLUDED
 /* When you add this cpp file to your project, you mustn't include it in a file where you've
    already included any other headers - just put it inside a file on its own, possibly with your config
    flags preceding it, but don't include anything else. That also includes avoiding any automatic prefix
    header files that the compiler may be using.
 */
 #error "Incorrect use of HIGGS cpp file"
#endif

#include "higgs_build_tools.h"

#include <numeric>

#include "utils/higgs_CppTokeniserFunctions.cpp"
#include "utils/higgs_BuildHelperFunctions.cpp"
#include "utils/higgs_BinaryResourceFile.cpp"
#include "utils/higgs_Icons.cpp"
#include "utils/higgs_PlistOptions.cpp"
#include "utils/higgs_ResourceFileHelpers.cpp"
#include "utils/higgs_ResourceRc.cpp"
#include "utils/higgs_VersionNumbers.cpp"
#include "utils/higgs_Entitlements.cpp"
