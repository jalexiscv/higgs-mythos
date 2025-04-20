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

#include "../Utility/Helpers/mythos_VersionInfo.h"

class DownloadAndInstallThread;

class LatestVersionCheckerAndUpdater final : public DeletedAtShutdown,
                                             private Thread
{
public:
    LatestVersionCheckerAndUpdater();
    ~LatestVersionCheckerAndUpdater() override;

    void checkForNewVersion (bool isBackgroundCheck);

    //==============================================================================
    HIGGS_DECLARE_SINGLETON_SINGLETHREADED_MINIMAL_INLINE (LatestVersionCheckerAndUpdater)

private:
    //==============================================================================
    void run() override;
    void askUserAboutNewVersion (const String&, const String&, const VersionInfo::Asset&);
    void askUserForLocationToDownload (const VersionInfo::Asset&);
    void downloadAndInstall (const VersionInfo::Asset&, const File&);

    void showDialogWindow (const String&, const String&, const VersionInfo::Asset&);
    void addNotificationToOpenProjects (const VersionInfo::Asset&);

    //==============================================================================
    bool backgroundCheck = false;

    std::unique_ptr<DownloadAndInstallThread> installer;
    std::unique_ptr<Component> dialogWindow;
    std::unique_ptr<FileChooser> chooser;
    ScopedMessageBox messageBox;

    HIGGS_DECLARE_WEAK_REFERENCEABLE (LatestVersionCheckerAndUpdater)
};
