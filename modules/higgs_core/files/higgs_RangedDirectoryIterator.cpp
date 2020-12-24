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

HIGGS_BEGIN_IGNORE_DEPRECATION_WARNINGS

float DirectoryEntry::getEstimatedProgress() const
{
    if (auto it = iterator.lock())
        return it->getEstimatedProgress();

    return 0.0f;
}

// We implement this in terms of the deprecated DirectoryIterator,
// but the old DirectoryIterator might go away in the future!
RangedDirectoryIterator::RangedDirectoryIterator (const File& directory,
                                                  bool isRecursive,
                                                  const String& wildCard,
                                                  int whatToLookFor,
                                                  File::FollowSymlinks followSymlinks)
    : iterator (new DirectoryIterator (directory,
                                       isRecursive,
                                       wildCard,
                                       whatToLookFor,
                                       followSymlinks))
{
    entry.iterator = iterator;
    increment();
}

bool RangedDirectoryIterator::next()
{
    const auto result = iterator->next (&entry.directory,
                                        &entry.hidden,
                                        &entry.fileSize,
                                        &entry.modTime,
                                        &entry.creationTime,
                                        &entry.readOnly);
    if (result)
        entry.file = iterator->getFile();
    else
        entry = {};

    return result;
}

void RangedDirectoryIterator::increment()
{
    if (iterator != nullptr && ! next())
        iterator = nullptr;
}

HIGGS_END_IGNORE_DEPRECATION_WARNINGS

} // namespace higgs
