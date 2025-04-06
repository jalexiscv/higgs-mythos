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

namespace higgs::build_tools
{

    //==============================================================================
    /** Manipulates a cross-platform partial file path. (Needed because File is designed
        for absolute paths on the active OS)
    */
    class RelativePath
    {
    public:
        //==============================================================================
        enum RootFolder
        {
            unknown,
            projectFolder,
            buildTargetFolder
        };

        //==============================================================================
        RelativePath()
            : root (unknown)
        {}

        RelativePath (const String& relPath, const RootFolder rootType)
            : path (unixStylePath (relPath)), root (rootType)
        {}

        RelativePath (const File& file, const File& rootFolder, const RootFolder rootType)
            : path (unixStylePath (getRelativePathFrom (file, rootFolder))), root (rootType)
        {}

        RootFolder getRoot() const                              { return root; }

        String toUnixStyle() const                              { return unixStylePath (path); }
        String toWindowsStyle() const                           { return windowsStylePath (path); }

        String getFileName() const                              { return getFakeFile().getFileName(); }
        String getFileNameWithoutExtension() const              { return getFakeFile().getFileNameWithoutExtension(); }

        String getFileExtension() const                         { return getFakeFile().getFileExtension(); }
        bool hasFileExtension (StringRef extension) const       { return getFakeFile().hasFileExtension (extension); }
        bool isAbsolute() const                                 { return isAbsolutePath (path); }

        RelativePath withFileExtension (const String& extension) const
        {
            return RelativePath (path.upToLastOccurrenceOf (".", ! extension.startsWithChar ('.'), false) + extension, root);
        }

        RelativePath getParentDirectory() const
        {
            String p (path);
            if (path.endsWithChar ('/'))
                p = p.dropLastCharacters (1);

            return RelativePath (p.upToLastOccurrenceOf ("/", false, false), root);
        }

        RelativePath getChildFile (const String& subpath) const
        {
            if (isAbsolutePath (subpath))
                return RelativePath (subpath, root);

            String p (toUnixStyle());
            if (! p.endsWithChar ('/'))
                p << '/';

            return RelativePath (p + subpath, root);
        }

        RelativePath rebased (const File& originalRoot, const File& newRoot, const RootFolder newRootType) const
        {
            if (isAbsolute())
                return RelativePath (path, newRootType);

            return RelativePath (getRelativePathFrom (originalRoot.getChildFile (toUnixStyle()), newRoot), newRootType);
        }

    private:
        //==============================================================================
        String path;
        RootFolder root;

        File getFakeFile() const
        {
            const auto unixStylePath = toUnixStyle();
            const auto name = unixStylePath.substring (unixStylePath.lastIndexOfChar ('/') + 1);

            // This method gets called very often, so we'll cache this directory.
            static const File currentWorkingDirectory (File::getCurrentWorkingDirectory());
            return currentWorkingDirectory.getChildFile (name);
        }
    };

} // namespace higgs::build_tools
