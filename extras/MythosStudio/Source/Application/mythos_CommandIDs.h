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

/**
    A namespace to hold all the possible command IDs.
*/
namespace CommandIDs
{
    enum
    {
        newProject              = 0x300000,
        newProjectFromClipboard = 0x300001,
        newPIP                  = 0x300002,
        open                    = 0x300003,
        closeDocument           = 0x300004,
        saveDocument            = 0x300005,
        saveDocumentAs          = 0x300006,

        launchDemoRunner        = 0x300007,

        closeProject            = 0x300010,
        saveProject             = 0x300011,
        saveAll                 = 0x300012,
        openInIDE               = 0x300013,
        saveAndOpenInIDE        = 0x300014,
        createNewExporter       = 0x300015,

        showUTF8Tool            = 0x300020,
        showGlobalPathsWindow   = 0x300021,
        showTranslationTool     = 0x300022,
        showSVGPathTool         = 0x300023,
        showAboutWindow         = 0x300024,
        checkForNewVersion      = 0x300025,
        enableNewVersionCheck   = 0x300026,

        showProjectSettings     = 0x300030,
        showFileExplorerPanel   = 0x300033,
        showModulesPanel        = 0x300034,
        showExportersPanel      = 0x300035,
        showExporterSettings    = 0x300036,

        closeWindow             = 0x300040,
        closeAllWindows         = 0x300041,
        closeAllDocuments       = 0x300042,
        goToPreviousDoc         = 0x300043,
        goToNextDoc             = 0x300044,
        goToCounterpart         = 0x300045,
        deleteSelectedItem      = 0x300046,
        goToPreviousWindow      = 0x300047,
        goToNextWindow          = 0x300048,
        clearRecentFiles        = 0x300049,

        showFindPanel           = 0x300050,
        findSelection           = 0x300051,
        findNext                = 0x300052,
        findPrevious            = 0x300053,

        enableSnapToGrid        = 0x300070,
        zoomIn                  = 0x300071,
        zoomOut                 = 0x300072,
        zoomNormal              = 0x300073,
        spaceBarDrag            = 0x300074,

        showForum               = 0x300100,
        showAPIModules          = 0x300101,
        showAPIClasses          = 0x300102,
        showTutorials           = 0x300103,

        lastCommandIDEntry
    };
}

namespace CommandCategories
{
    static const char* const general       = "General";
    static const char* const editing       = "Editing";
    static const char* const view          = "View";
    static const char* const windows       = "Windows";
}
