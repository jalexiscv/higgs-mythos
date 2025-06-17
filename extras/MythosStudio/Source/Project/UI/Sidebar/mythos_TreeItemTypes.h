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

#include "../../../ProjectSaving/mythos_ProjectExporter.h"
#include "../../../Application/Windows/mythos_TranslationToolWindowComponent.h"
#include "../../../Utility/UI/mythos_MythosTreeViewBase.h"
#include "../../../Utility/Helpers/mythos_NewFileWizard.h"
#include "../mythos_ContentViewComponents.h"
#include "../mythos_FileGroupInformationComponent.h"
#include "../mythos_ModulesInformationComponent.h"

struct TreeItemTypes
{
    #include "mythos_ProjectTreeItemBase.h"
    #include "mythos_ExporterTreeItems.h"
    #include "mythos_ModuleTreeItems.h"
    #include "mythos_FileTreeItems.h"
};
