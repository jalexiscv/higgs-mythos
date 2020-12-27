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

static File createTempFile (const File& parentDirectory, String name,
                            const String& suffix, int optionFlags)
{
    if ((optionFlags & TemporaryFile::useHiddenFile) != 0)
        name = "." + name;

    return parentDirectory.getNonexistentChildFile (name, suffix, (optionFlags & TemporaryFile::putNumbersInBrackets) != 0);
}

TemporaryFile::TemporaryFile() : TemporaryFile { String{} } {}

TemporaryFile::TemporaryFile (const String& suffix) : TemporaryFile { suffix, 0 } {}

TemporaryFile::TemporaryFile (const File& target) : TemporaryFile { target, 0 } {}

TemporaryFile::TemporaryFile (const String& suffix, const int optionFlags)
    : temporaryFile (createTempFile (File::getSpecialLocation (File::tempDirectory),
                                     "temp_" + String::toHexString (Random::getSystemRandom().nextInt()),
                                     suffix, optionFlags)),
      targetFile()
{
}

TemporaryFile::TemporaryFile (const File& target, const int optionFlags)
    : temporaryFile (createTempFile (target.getParentDirectory(),
                                     target.getFileNameWithoutExtension()
                                       + "_temp" + String::toHexString (Random::getSystemRandom().nextInt()),
                                     target.getFileExtension(), optionFlags)),
      targetFile (target)
{
    // If you use this constructor, you need to give it a valid target file!
    jassert (targetFile != File());
}

TemporaryFile::TemporaryFile (const File& target, const File& temporary)
    : temporaryFile (temporary), targetFile (target)
{
}

TemporaryFile::~TemporaryFile()
{
    if (! deleteTemporaryFile())
    {
        /* Failed to delete our temporary file! The most likely reason for this would be
           that you've not closed an output stream that was being used to write to file.

           If you find that something beyond your control is changing permissions on
           your temporary files and preventing them from being deleted, you may want to
           call TemporaryFile::deleteTemporaryFile() to detect those error cases and
           handle them appropriately.
        */
        jassertfalse;
    }
}

//==============================================================================
bool TemporaryFile::overwriteTargetFileWithTemporary() const
{
    // This method only works if you created this object with the constructor
    // that takes a target file!
    jassert (targetFile != File());

    if (temporaryFile.exists())
    {
        // Have a few attempts at overwriting the file before giving up.
        for (int i = 5; --i >= 0;)
        {
            if (temporaryFile.replaceFileIn (targetFile))
                return true;

            Thread::sleep (100);
        }
    }
    else
    {
        // There's no temporary file to use. If your write failed, you should
        // probably check, and not bother calling this method.
        jassertfalse;
    }

    return false;
}

bool TemporaryFile::deleteTemporaryFile() const
{
    // Have a few attempts at deleting the file before giving up.
    for (int i = 5; --i >= 0;)
    {
        if (temporaryFile.isDirectory() ? temporaryFile.deleteRecursively() : temporaryFile.deleteFile())
            return true;

        Thread::sleep (50);
    }

    return false;
}

} // namespace higgs
