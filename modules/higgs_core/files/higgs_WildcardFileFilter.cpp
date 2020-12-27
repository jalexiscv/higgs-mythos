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

static void parseWildcard (const String& pattern, StringArray& result)
{
    result.addTokens (pattern.toLowerCase(), ";,", "\"'");
    result.trim();
    result.removeEmptyStrings();

    // special case for *.*, because people use it to mean "any file", but it
    // would actually ignore files with no extension.
    for (auto& r : result)
        if (r == "*.*")
            r = "*";
}

static bool matchWildcard (const File& file, const StringArray& wildcards)
{
    auto filename = file.getFileName();

    for (auto& w : wildcards)
        if (filename.matchesWildcard (w, true))
            return true;

    return false;
}

WildcardFileFilter::WildcardFileFilter (const String& fileWildcardPatterns,
                                        const String& directoryWildcardPatterns,
                                        const String& desc)
    : FileFilter (desc.isEmpty() ? fileWildcardPatterns
                                 : (desc + " (" + fileWildcardPatterns + ")"))
{
    parseWildcard (fileWildcardPatterns, fileWildcards);
    parseWildcard (directoryWildcardPatterns, directoryWildcards);
}

WildcardFileFilter::~WildcardFileFilter()
{
}

bool WildcardFileFilter::isFileSuitable (const File& file) const
{
    return matchWildcard (file, fileWildcards);
}

bool WildcardFileFilter::isDirectorySuitable (const File& file) const
{
    return matchWildcard (file, directoryWildcards);
}

} // namespace higgs
