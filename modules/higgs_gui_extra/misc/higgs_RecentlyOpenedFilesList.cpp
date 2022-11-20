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

RecentlyOpenedFilesList::RecentlyOpenedFilesList()
    : maxNumberOfItems (10)
{
}

//==============================================================================
void RecentlyOpenedFilesList::setMaxNumberOfItems (const int newMaxNumber)
{
    maxNumberOfItems = jmax (1, newMaxNumber);

    files.removeRange (maxNumberOfItems, getNumFiles());
}

int RecentlyOpenedFilesList::getNumFiles() const
{
    return files.size();
}

File RecentlyOpenedFilesList::getFile (const int index) const
{
    return File (files [index]);
}

void RecentlyOpenedFilesList::clear()
{
    files.clear();
}

void RecentlyOpenedFilesList::addFile (const File& file)
{
    removeFile (file);
    files.insert (0, file.getFullPathName());

    setMaxNumberOfItems (maxNumberOfItems);
}

void RecentlyOpenedFilesList::removeFile (const File& file)
{
    files.removeString (file.getFullPathName());
}

void RecentlyOpenedFilesList::removeNonExistentFiles()
{
    for (int i = getNumFiles(); --i >= 0;)
        if (! getFile (i).exists())
            files.remove (i);
}

//==============================================================================
int RecentlyOpenedFilesList::createPopupMenuItems (PopupMenu& menuToAddTo,
                                                   const int baseItemId,
                                                   const bool showFullPaths,
                                                   const bool dontAddNonExistentFiles,
                                                   const File** filesToAvoid)
{
    int num = 0;

    for (int i = 0; i < getNumFiles(); ++i)
    {
        const File f (getFile (i));

        if ((! dontAddNonExistentFiles) || f.exists())
        {
            bool needsAvoiding = false;

            if (filesToAvoid != nullptr)
            {
                for (const File** avoid = filesToAvoid; *avoid != nullptr; ++avoid)
                {
                    if (f == **avoid)
                    {
                        needsAvoiding = true;
                        break;
                    }
                }
            }

            if (! needsAvoiding)
            {
                menuToAddTo.addItem (baseItemId + i,
                                     showFullPaths ? f.getFullPathName()
                                                   : f.getFileName());
                ++num;
            }
        }
    }

    return num;
}

//==============================================================================
String RecentlyOpenedFilesList::toString() const
{
    return files.joinIntoString ("\n");
}

void RecentlyOpenedFilesList::restoreFromString (const String& stringifiedVersion)
{
    clear();
    files.addLines (stringifiedVersion);

    setMaxNumberOfItems (maxNumberOfItems);
}


//==============================================================================
void RecentlyOpenedFilesList::registerRecentFileNatively ([[maybe_unused]] const File& file)
{
   #if HIGGS_MAC
    HIGGS_AUTORELEASEPOOL
    {
        [[NSDocumentController sharedDocumentController] noteNewRecentDocumentURL: createNSURLFromFile (file)];
    }
   #endif
}

void RecentlyOpenedFilesList::forgetRecentFileNatively ([[maybe_unused]] const File& file)
{
   #if HIGGS_MAC
    HIGGS_AUTORELEASEPOOL
    {
        // for some reason, OSX doesn't provide a method to just remove a single file
        // from the recent list, so we clear them all and add them back excluding
        // the specified file

        auto sharedDocController = [NSDocumentController sharedDocumentController];
        auto recentDocumentURLs  = [sharedDocController recentDocumentURLs];

        [sharedDocController clearRecentDocuments: nil];

        auto* nsFile = createNSURLFromFile (file);

        auto reverseEnumerator = [recentDocumentURLs reverseObjectEnumerator];

        for (NSURL* url : reverseEnumerator)
            if (! [url isEqual:nsFile])
                [sharedDocController noteNewRecentDocumentURL:url];
    }
   #endif
}

void RecentlyOpenedFilesList::clearRecentFilesNatively()
{
   #if HIGGS_MAC
    HIGGS_AUTORELEASEPOOL
    {
        [[NSDocumentController sharedDocumentController] clearRecentDocuments: nil];
    }
   #endif
}

} // namespace higgs
