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

#include "../../Application/mythos_Headers.h"
#include "mythos_CodeHelpers.h"

//==============================================================================
namespace FileHelpers
{
    bool containsAnyNonHiddenFiles (const File& folder)
    {
        for (const auto& di : RangedDirectoryIterator (folder, false))
            if (! di.getFile().isHidden())
                return true;

        return false;
    }

    bool shouldPathsBeRelative (String path1, String path2)
    {
        path1 = build_tools::unixStylePath (path1);
        path2 = build_tools::unixStylePath (path2);

        const int len = jmin (path1.length(), path2.length());
        int commonBitLength = 0;

        for (int i = 0; i < len; ++i)
        {
            if (CharacterFunctions::toLowerCase (path1[i]) != CharacterFunctions::toLowerCase (path2[i]))
                break;

            ++commonBitLength;
        }

        return path1.substring (0, commonBitLength).removeCharacters ("/:").isNotEmpty();
    }

    // removes "/../" bits from the middle of the path
    String simplifyPath (String::CharPointerType p)
    {
       #if HIGGS_WINDOWS
        if (CharacterFunctions::indexOf (p, CharPointer_ASCII ("/../")) >= 0
             || CharacterFunctions::indexOf (p, CharPointer_ASCII ("\\..\\")) >= 0)
       #else
        if (CharacterFunctions::indexOf (p, CharPointer_ASCII ("/../")) >= 0)
       #endif
        {
            StringArray toks;

           #if HIGGS_WINDOWS
            toks.addTokens (p, "\\/", StringRef());
           #else
            toks.addTokens (p, "/", StringRef());
           #endif

            while (toks[0] == ".")
                toks.remove (0);

            for (int i = 1; i < toks.size(); ++i)
            {
                if (toks[i] == ".." && toks [i - 1] != "..")
                {
                    toks.removeRange (i - 1, 2);
                    i = jmax (0, i - 2);
                }
            }

            return toks.joinIntoString ("/");
        }

        return p;
    }

    String simplifyPath (const String& path)
    {
       #if HIGGS_WINDOWS
        if (path.contains ("\\..\\") || path.contains ("/../"))
       #else
        if (path.contains ("/../"))
       #endif
            return simplifyPath (path.getCharPointer());

        return path;
    }
}
