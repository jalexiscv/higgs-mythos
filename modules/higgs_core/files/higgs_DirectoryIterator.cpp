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

StringArray DirectoryIterator::parseWildcards (const String& pattern)
{
    StringArray s;
    s.addTokens (pattern, ";,", "\"'");
    s.trim();
    s.removeEmptyStrings();
    return s;
}

bool DirectoryIterator::fileMatches (const StringArray& wildcards, const String& filename)
{
    for (auto& w : wildcards)
        if (filename.matchesWildcard (w, ! File::areFileNamesCaseSensitive()))
            return true;

    return false;
}

bool DirectoryIterator::next()
{
    return next (nullptr, nullptr, nullptr, nullptr, nullptr, nullptr);
}

HIGGS_BEGIN_IGNORE_DEPRECATION_WARNINGS

bool DirectoryIterator::next (bool* isDirResult, bool* isHiddenResult, int64* fileSize,
                              Time* modTime, Time* creationTime, bool* isReadOnly)
{
    for (;;)
    {
        hasBeenAdvanced = true;

        if (subIterator != nullptr)
        {
            if (subIterator->next (isDirResult, isHiddenResult, fileSize, modTime, creationTime, isReadOnly))
                return true;

            subIterator.reset();
        }

        String filename;
        bool isDirectory, isHidden = false, shouldContinue = false;

        while (fileFinder.next (filename, &isDirectory,
                                (isHiddenResult != nullptr || (whatToLookFor & File::ignoreHiddenFiles) != 0) ? &isHidden : nullptr,
                                fileSize, modTime, creationTime, isReadOnly))
        {
            ++index;

            if (! filename.containsOnly ("."))
            {
                const auto fullPath = File::createFileWithoutCheckingPath (path + filename);
                bool matches = false;

                if (isDirectory)
                {
                    const auto mayRecurseIntoPossibleHiddenDir = [this, &isHidden]
                    {
                        return (whatToLookFor & File::ignoreHiddenFiles) == 0 || ! isHidden;
                    };

                    const auto mayRecurseIntoPossibleSymlink = [this, &fullPath]
                    {
                        return followSymlinks == File::FollowSymlinks::yes
                            || ! fullPath.isSymbolicLink()
                            || (followSymlinks == File::FollowSymlinks::noCycles
                                && knownPaths->find (fullPath.getLinkedTarget()) == knownPaths->end());
                    };

                    if (isRecursive && mayRecurseIntoPossibleHiddenDir() && mayRecurseIntoPossibleSymlink())
                        subIterator.reset (new DirectoryIterator (fullPath, true, wildCard, whatToLookFor, followSymlinks, knownPaths));

                    matches = (whatToLookFor & File::findDirectories) != 0;
                }
                else
                {
                    matches = (whatToLookFor & File::findFiles) != 0;
                }

                // if we're not relying on the OS iterator to do the wildcard match, do it now
                if (matches && (isRecursive || wildCards.size() > 1))
                    matches = fileMatches (wildCards, filename);

                if (matches && (whatToLookFor & File::ignoreHiddenFiles) != 0)
                    matches = ! isHidden;

                if (matches)
                {
                    currentFile = fullPath;
                    if (isHiddenResult != nullptr)     *isHiddenResult = isHidden;
                    if (isDirResult != nullptr)        *isDirResult = isDirectory;

                    return true;
                }

                if (subIterator != nullptr)
                {
                    shouldContinue = true;
                    break;
                }
            }
        }

        if (! shouldContinue)
            return false;
    }
}

HIGGS_END_IGNORE_DEPRECATION_WARNINGS

const File& DirectoryIterator::getFile() const
{
    if (subIterator != nullptr && subIterator->hasBeenAdvanced)
        return subIterator->getFile();

    // You need to call DirectoryIterator::next() before asking it for the file that it found!
    jassert (hasBeenAdvanced);

    return currentFile;
}

float DirectoryIterator::getEstimatedProgress() const
{
    if (totalNumFiles < 0)
        totalNumFiles = File (path).getNumberOfChildFiles (File::findFilesAndDirectories);

    if (totalNumFiles <= 0)
        return 0.0f;

    auto detailedIndex = (subIterator != nullptr) ? (float) index + subIterator->getEstimatedProgress()
                                                  : (float) index;

    return jlimit (0.0f, 1.0f, detailedIndex / (float) totalNumFiles);
}

} // namespace higgs
