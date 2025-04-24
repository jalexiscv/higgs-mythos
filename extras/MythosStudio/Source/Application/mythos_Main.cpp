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

#ifdef MYTHOS_ENABLE_GPL_MODE
 #warning The flag MYTHOS_ENABLE_GPL_MODE has been removed
#endif

#include "mythos_Headers.h"

#include "mythos_Application.h"
#include "../CodeEditor/mythos_OpenDocumentManager.h"
#include "../CodeEditor/mythos_SourceCodeEditor.h"
#include "../Utility/UI/PropertyComponents/mythos_FilePathPropertyComponent.h"
#include "../Project/UI/mythos_ProjectContentComponent.h"
#include "../Project/UI/Sidebar/mythos_TreeItemTypes.h"
#include "Windows/mythos_UTF8WindowComponent.h"
#include "Windows/mythos_SVGPathDataWindowComponent.h"
#include "Windows/mythos_AboutWindowComponent.h"
#include "Windows/mythos_EditorColourSchemeWindowComponent.h"
#include "Windows/mythos_GlobalPathsWindowComponent.h"
#include "Windows/mythos_PIPCreatorWindowComponent.h"
#include "Windows/mythos_FloatingToolWindow.h"

#include "mythos_CommandLine.h"

#include "../Project/UI/mythos_ProjectContentComponent.cpp"
#include "mythos_Application.cpp"


START_HIGGS_APPLICATION (MythosStudioApplication)
