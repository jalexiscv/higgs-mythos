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

#include "../Application/mythos_Headers.h"
#include "mythos_ResourceFile.h"

//==============================================================================
MythosResourceFile::MythosResourceFile (Project& p)    : project (p)
{
    addResourcesFromProjectItem (project.getMainGroup());
}

MythosResourceFile::~MythosResourceFile()
{
}

//==============================================================================
void MythosResourceFile::addResourcesFromProjectItem (const Project::Item& projectItem)
{
    if (projectItem.isGroup())
    {
        for (int i = 0; i < projectItem.getNumChildren(); ++i)
            addResourcesFromProjectItem (projectItem.getChild (i));
    }
    else
    {
        if (projectItem.shouldBeAddedToBinaryResources())
            addFile (projectItem.getFile());
    }
}
