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

    class ResourceFile
    {
    public:
        ResourceFile() = default;

        void setClassName (const String& className);

        String getClassName() const { return className; }

        void addFile (const File& file);

        String getDataVariableFor (const File& file) const;

        String getSizeVariableFor (const File& file) const;

        int getNumFiles() const { return files.size(); }

        const File& getFile (int index) const { return files.getReference (index); }

        int64 getTotalDataSize() const;

        struct WriteResult
        {
            Result result;
            Array<File> filesCreated;
        };

        WriteResult write (int maxFileSize,
                           String projectLineFeed,
                           File headerFile,
                           std::function<File (int)> getCppFile);

    private:
        Array<File> files;
        StringArray variableNames;
        String className { "BinaryData" };

        Result writeHeader (MemoryOutputStream&);

        Result writeCpp (MemoryOutputStream&, const File&, int&, int);

        HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ResourceFile)
    };

} // namespace higgs::build_tools
