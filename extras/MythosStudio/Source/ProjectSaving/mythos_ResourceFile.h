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

#pragma once

#include "../Project/mythos_Project.h"

//==============================================================================
class MythosResourceFile
{
public:
    //==============================================================================
    explicit MythosResourceFile (Project& project);
    ~MythosResourceFile();

    //==============================================================================
    void setClassName (const String& className)         { resourceFile.setClassName (className); }
    String getClassName() const                         { return resourceFile.getClassName(); }

    void addFile (const File& file)                     { resourceFile.addFile (file); }
    String getDataVariableFor (const File& file) const  { return resourceFile.getDataVariableFor (file); }
    String getSizeVariableFor (const File& file) const  { return resourceFile.getSizeVariableFor (file); }

    int getNumFiles() const                             { return resourceFile.getNumFiles(); }
    const File& getFile (int index) const               { return resourceFile.getFile (index); }

    int64 getTotalDataSize() const                      { return resourceFile.getTotalDataSize(); }

    build_tools::ResourceFile::WriteResult write (int maxFileSize)
    {
        return resourceFile.write (maxFileSize,
                                   project.getProjectLineFeed(),
                                   project.getBinaryDataHeaderFile(),
                                   [this] (int index) { return project.getBinaryDataCppFile (index); });
    }

    //==============================================================================
private:
    void addResourcesFromProjectItem (const Project::Item& node);

    Project& project;
    build_tools::ResourceFile resourceFile;

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MythosResourceFile)
};
