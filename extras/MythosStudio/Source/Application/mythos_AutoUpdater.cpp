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
#include "mythos_Application.h"
#include "mythos_AutoUpdater.h"

class DownloadAndInstallThread final : private ThreadWithProgressWindow
{
public:
    DownloadAndInstallThread  (const VersionInfo::Asset& a, const File& t, std::function<void (Result)>&& cb)
        : ThreadWithProgressWindow ("Downloading New Version", true, true),
          asset (a), targetFolder (t), completionCallback (std::move (cb))
    {
        launchThread (Priority::low);
    }

private:
    void run() override
    {
        setProgress (-1.0);

        MemoryBlock zipData;
        auto result = download (zipData);

        if (result.wasOk() && ! threadShouldExit())
            result = install (zipData);

        MessageManager::callAsync ([result, callback = completionCallback]
        {
            callback (result);
        });
    }

    Result download (MemoryBlock& dest)
    {
        setStatusMessage ("Downloading...");

        int statusCode = 0;
        auto inStream = VersionInfo::createInputStreamForAsset (asset, statusCode);

        if (inStream != nullptr && statusCode == 200)
        {
            int64 total = 0;
            MemoryOutputStream mo (dest, true);

            for (;;)
            {
                if (threadShouldExit())
                    return Result::fail ("Cancelled");

                auto written = mo.writeFromInputStream (*inStream, 8192);

                if (written == 0)
                    break;

                total += written;

                setStatusMessage ("Downloading... " + File::descriptionOfSizeInBytes (total));
            }

            return Result::ok();
        }

        return Result::fail ("Failed to download from: " + asset.url);
    }

    Result install (const MemoryBlock& data)
    {
        setStatusMessage ("Installing...");

        MemoryInputStream input (data, false);
        ZipFile zip (input);

        if (zip.getNumEntries() == 0)
            return Result::fail ("The downloaded file was not a valid HIGGS file!");

        struct ScopedDownloadFolder
        {
            explicit ScopedDownloadFolder (const File& installTargetFolder)
            {
                folder = installTargetFolder.getSiblingFile (installTargetFolder.getFileNameWithoutExtension() + "_download").getNonexistentSibling();
                jassert (folder.createDirectory());
            }

            ~ScopedDownloadFolder()   { folder.deleteRecursively(); }

            File folder;
        };

        ScopedDownloadFolder unzipTarget (targetFolder);

        if (! unzipTarget.folder.isDirectory())
            return Result::fail ("Couldn't create a temporary folder to unzip the new version!");

        auto r = zip.uncompressTo (unzipTarget.folder);

        if (r.failed())
            return r;

        if (threadShouldExit())
            return Result::fail ("Cancelled");

       #if HIGGS_LINUX || HIGGS_BSD || HIGGS_MAC
        r = setFilePermissions (unzipTarget.folder, zip);

        if (r.failed())
            return r;

        if (threadShouldExit())
            return Result::fail ("Cancelled");
       #endif

        if (targetFolder.exists())
        {
            auto oldFolder = targetFolder.getSiblingFile (targetFolder.getFileNameWithoutExtension() + "_old").getNonexistentSibling();

            if (! targetFolder.moveFileTo (oldFolder))
                return Result::fail ("Could not remove the existing folder!\n\n"
                                     "This may happen if you are trying to download into a directory that requires administrator privileges to modify.\n"
                                     "Please select a folder that is writable by the current user.");
        }

        if (! unzipTarget.folder.getChildFile ("HIGGS").moveFileTo (targetFolder))
            return Result::fail ("Could not overwrite the existing folder!\n\n"
                                 "This may happen if you are trying to download into a directory that requires administrator privileges to modify.\n"
                                 "Please select a folder that is writable by the current user.");

        return Result::ok();
    }

    Result setFilePermissions (const File& root, const ZipFile& zip)
    {
        constexpr uint32 executableFlag = (1 << 22);

        for (int i = 0; i < zip.getNumEntries(); ++i)
        {
            auto* entry = zip.getEntry (i);

            if ((entry->externalFileAttributes & executableFlag) != 0 && entry->filename.getLastCharacter() != '/')
            {
                auto exeFile = root.getChildFile (entry->filename);

                if (! exeFile.exists())
                    return Result::fail ("Failed to find executable file when setting permissions " + exeFile.getFileName());

                if (! exeFile.setExecutePermission (true))
                    return Result::fail ("Failed to set executable file permission for " + exeFile.getFileName());
            }
        }

        return Result::ok();
    }

    VersionInfo::Asset asset;
    File targetFolder;
    std::function<void (Result)> completionCallback;
};

//==============================================================================
LatestVersionCheckerAndUpdater::LatestVersionCheckerAndUpdater()
    : Thread ("VersionChecker")
{
}

LatestVersionCheckerAndUpdater::~LatestVersionCheckerAndUpdater()
{
    stopThread (6000);
    clearSingletonInstance();
}

void LatestVersionCheckerAndUpdater::checkForNewVersion (bool background)
{
    if (! isThreadRunning())
    {
        backgroundCheck = background;
        startThread (Priority::low);
    }
}

//==============================================================================
void LatestVersionCheckerAndUpdater::run()
{
    auto info = VersionInfo::fetchLatestFromUpdateServer();

    if (info == nullptr)
    {
        if (! backgroundCheck)
        {
            auto options = MessageBoxOptions::makeOptionsOk (MessageBoxIconType::WarningIcon,
                                                             "Update Server Communication Error",
                                                             "Failed to communicate with the HIGGS update server.\n"
                                                             "Please try again in a few minutes.\n\n"
                                                             "If this problem persists you can download the latest version of HIGGS from higgsmythos.com");
            messageBox = AlertWindow::showScopedAsync (options, nullptr);
        }

        return;
    }

    if (! info->isNewerVersionThanCurrent())
    {
        if (! backgroundCheck)
        {
            auto options = MessageBoxOptions::makeOptionsOk (MessageBoxIconType::InfoIcon,
                                                             "No New Version Available",
                                                             "Your HIGGS version is up to date.");
            messageBox = AlertWindow::showScopedAsync (options, nullptr);
        }
        return;
    }

    auto osString = []
    {
       #if HIGGS_MAC
        return "osx";
       #elif HIGGS_WINDOWS
        return "windows";
       #elif HIGGS_LINUX
        return "linux";
       #elif HIGGS_BSD
        return "bsd";
       #else
        jassertfalse;
        return "Unknown";
       #endif
    }();

    String requiredFilename ("higgs-" + info->versionString + "-" + osString + ".zip");

    for (auto& asset : info->assets)
    {
        if (asset.name == requiredFilename)
        {
            auto versionString = info->versionString;
            auto releaseNotes  = info->releaseNotes;

            MessageManager::callAsync ([this, versionString, releaseNotes, asset]
            {
                askUserAboutNewVersion (versionString, releaseNotes, asset);
            });

            return;
        }
    }

    if (! backgroundCheck)
    {
        auto options = MessageBoxOptions::makeOptionsOk (MessageBoxIconType::WarningIcon,
                                                         "Failed to find any new downloads",
                                                         "Please try again in a few minutes.");
        messageBox = AlertWindow::showScopedAsync (options, nullptr);
    }
}

//==============================================================================
class UpdateDialog final : public Component
{
public:
    UpdateDialog (const String& newVersion, const String& releaseNotes)
    {
        titleLabel.setText ("HIGGS version " + newVersion, dontSendNotification);
        titleLabel.setFont (FontOptions { 15.0f, Font::bold });
        titleLabel.setJustificationType (Justification::centred);
        addAndMakeVisible (titleLabel);

        contentLabel.setText ("A new version of HIGGS is available - would you like to download it?", dontSendNotification);
        contentLabel.setFont (FontOptions { 15.0f });
        contentLabel.setJustificationType (Justification::topLeft);
        addAndMakeVisible (contentLabel);

        releaseNotesLabel.setText ("Release notes:", dontSendNotification);
        releaseNotesLabel.setFont (FontOptions { 15.0f });
        releaseNotesLabel.setJustificationType (Justification::topLeft);
        addAndMakeVisible (releaseNotesLabel);

        releaseNotesEditor.setMultiLine (true);
        releaseNotesEditor.setReadOnly (true);
        releaseNotesEditor.setText (releaseNotes);
        addAndMakeVisible (releaseNotesEditor);

        addAndMakeVisible (chooseButton);
        chooseButton.onClick = [this] { exitModalStateWithResult (1); };

        addAndMakeVisible (cancelButton);
        cancelButton.onClick = [this]
        {
            MythosStudioApplication::getApp().setAutomaticVersionCheckingEnabled (! dontAskAgainButton.getToggleState());
            exitModalStateWithResult (-1);
        };

        dontAskAgainButton.setToggleState (! MythosStudioApplication::getApp().isAutomaticVersionCheckingEnabled(), dontSendNotification);
        addAndMakeVisible (dontAskAgainButton);

        higgsIcon = Drawable::createFromImageData (BinaryData::higgs_icon_png,
                                                  BinaryData::higgs_icon_pngSize);
        updateLookAndFeel();

        setSize (500, 280);
    }

    void resized() override
    {
        auto b = getLocalBounds().reduced (10);

        auto topSlice = b.removeFromTop (higgsIconBounds.getHeight())
                         .withTrimmedLeft (higgsIconBounds.getWidth());

        titleLabel.setBounds (topSlice.removeFromTop (25));
        topSlice.removeFromTop (5);
        contentLabel.setBounds (topSlice.removeFromTop (25));

        auto buttonBounds = b.removeFromBottom (60);
        buttonBounds.removeFromBottom (25);
        chooseButton.setBounds (buttonBounds.removeFromLeft (buttonBounds.getWidth() / 2).reduced (20, 0));
        cancelButton.setBounds (buttonBounds.reduced (20, 0));
        dontAskAgainButton.setBounds (cancelButton.getBounds().withY (cancelButton.getBottom() + 5).withHeight (20));

        releaseNotesEditor.setBounds (b.reduced (0, 10));
    }

    void paint (Graphics& g) override
    {
        g.fillAll (findColour (backgroundColourId));

        if (higgsIcon != nullptr)
            higgsIcon->drawWithin (g, higgsIconBounds.toFloat(),
                                  RectanglePlacement::stretchToFit, 1.0f);
    }

    static std::unique_ptr<DialogWindow> launchDialog (const String& newVersionString,
                                                       const String& releaseNotes)
    {
        DialogWindow::LaunchOptions options;

        options.dialogTitle = "Download HIGGS version " + newVersionString + "?";
        options.resizable = false;

        auto* content = new UpdateDialog (newVersionString, releaseNotes);
        options.content.set (content, true);

        std::unique_ptr<DialogWindow> dialog (options.create());

        content->setParentWindow (dialog.get());
        dialog->enterModalState (true, nullptr, true);

        return dialog;
    }

private:
    void updateLookAndFeel()
    {
        cancelButton.setColour (TextButton::buttonColourId, findColour (secondaryButtonBackgroundColourId));
        releaseNotesEditor.applyFontToAllText (releaseNotesEditor.getFont());
    }

    void lookAndFeelChanged() override
    {
        updateLookAndFeel();
    }

    void setParentWindow (DialogWindow* parent)
    {
        parentWindow = parent;
    }

    void exitModalStateWithResult (int result)
    {
        if (parentWindow != nullptr)
            parentWindow->exitModalState (result);
    }

    Label titleLabel, contentLabel, releaseNotesLabel;
    TextEditor releaseNotesEditor;
    TextButton chooseButton { "Choose Location..." }, cancelButton { "Cancel" };
    ToggleButton dontAskAgainButton { "Don't ask again" };
    std::unique_ptr<Drawable> higgsIcon;
    Rectangle<int> higgsIconBounds { 10, 10, 64, 64 };

    DialogWindow* parentWindow = nullptr;
};

void LatestVersionCheckerAndUpdater::askUserForLocationToDownload (const VersionInfo::Asset& asset)
{
    chooser = std::make_unique<FileChooser> ("Please select the location into which you would like to install the new version",
                                             File { getAppSettings().getStoredPath (Ids::higgsPath, TargetOS::getThisOS()).get() });

    chooser->launchAsync (FileBrowserComponent::openMode | FileBrowserComponent::canSelectDirectories,
                          [weakThis = WeakReference<LatestVersionCheckerAndUpdater> { this }, asset] (const FileChooser& fc)
    {
        auto targetFolder = fc.getResult();

        if (targetFolder == File{})
            return;

        // By default we will install into 'targetFolder/HIGGS', but we should install into
        // 'targetFolder' if that is an existing HIGGS directory.
        bool willOverwriteHiggsFolder = [&targetFolder]
        {
            if (isHIGGSFolder (targetFolder))
                return true;

            targetFolder = targetFolder.getChildFile ("HIGGS");

            return isHIGGSFolder (targetFolder);
        }();

        auto targetFolderPath = targetFolder.getFullPathName();

        const auto onResult = [weakThis, asset, targetFolder] (int result)
        {
            if (weakThis == nullptr || result == 0)
                return;

            weakThis->downloadAndInstall (asset, targetFolder);
        };

        if (willOverwriteHiggsFolder)
        {
            if (targetFolder.getChildFile (".git").isDirectory())
            {
                auto options = MessageBoxOptions::makeOptionsOk (MessageBoxIconType::WarningIcon,
                                                                 "Downloading New HIGGS Version",
                                                                 targetFolderPath + "\n\n"
                                                                 "is a GIT repository!\n\n"
                                                                 "You should use a \"git pull\" to update it to the latest version.");
                if (weakThis != nullptr)
                    weakThis->messageBox = AlertWindow::showScopedAsync (options, nullptr);

                return;
            }

            auto options = MessageBoxOptions{}.withIconType (MessageBoxIconType::WarningIcon)
                                              .withTitle ("Overwrite Existing HIGGS Folder?")
                                              .withMessage ("Do you want to replace the folder\n\n" + targetFolderPath + "\n\n"
                                                            "with the latest version from higgsmythos.com?\n\n"
                                                            "This will move the existing folder to " + targetFolderPath + "_old.\n\n"
                                                            "Replacing the folder that contains the currently running MythosStudio executable may not work on Windows.")
                                              .withButton (TRANS ("OK"), 1)
                                              .withButton (TRANS ("Cancel"), 0);
            if (weakThis != nullptr)
                weakThis->messageBox = AlertWindow::showScopedAsync (options, onResult);

            return;
        }

        if (targetFolder.exists())
        {
            auto options = MessageBoxOptions{}.withIconType (MessageBoxIconType::WarningIcon)
                                              .withTitle ("Existing File Or Directory")
                                              .withMessage ("Do you want to move\n\n" + targetFolderPath + "\n\n"
                                                            "to\n\n" + targetFolderPath + "_old?")
                                              .withButton (TRANS ("OK"), 1)
                                              .withButton (TRANS ("Cancel"), 0);
            if (weakThis != nullptr)
                weakThis->messageBox = AlertWindow::showScopedAsync (options, onResult);

            return;
        }

        if (weakThis != nullptr)
            weakThis->downloadAndInstall (asset, targetFolder);
    });
}

void LatestVersionCheckerAndUpdater::askUserAboutNewVersion (const String& newVersionString,
                                                             const String& releaseNotes,
                                                             const VersionInfo::Asset& asset)
{
    if (backgroundCheck)
        addNotificationToOpenProjects (asset);
    else
        showDialogWindow (newVersionString, releaseNotes, asset);
}

void LatestVersionCheckerAndUpdater::showDialogWindow (const String& newVersionString,
                                                       const String& releaseNotes,
                                                       const VersionInfo::Asset& asset)
{
    dialogWindow = UpdateDialog::launchDialog (newVersionString, releaseNotes);

    if (auto* mm = ModalComponentManager::getInstance())
    {
        mm->attachCallback (dialogWindow.get(),
                            ModalCallbackFunction::create ([this, asset] (int result)
                                                           {
                                                               if (result == 1)
                                                                    askUserForLocationToDownload (asset);

                                                                dialogWindow.reset();
                                                            }));
    }
}

void LatestVersionCheckerAndUpdater::addNotificationToOpenProjects (const VersionInfo::Asset& asset)
{
    for (auto* window : MythosStudioApplication::getApp().mainWindowList.windows)
    {
        if (auto* project = window->getProject())
        {
            auto ignore = [safeWindow = Component::SafePointer<MainWindow> { window }]
            {
                if (safeWindow != nullptr)
                    safeWindow->getProject()->removeProjectMessage (ProjectMessages::Ids::newVersionAvailable);
            };

            auto dontAskAgain = [ignore]
            {
                ignore();
                MythosStudioApplication::getApp().setAutomaticVersionCheckingEnabled (false);
            };

            project->addProjectMessage (ProjectMessages::Ids::newVersionAvailable,
                                        { { "Download", [this, asset] { askUserForLocationToDownload (asset); } },
                                          { "Ignore", std::move (ignore) },
                                          { "Don't ask again", std::move (dontAskAgain) } });
        }
    }
}

//==============================================================================
static void restartProcess (const File& targetFolder)
{
   #if HIGGS_MAC || HIGGS_LINUX || HIGGS_BSD
    #if HIGGS_MAC
     auto newProcess = targetFolder.getChildFile ("MythosStudio.app").getChildFile ("Contents").getChildFile ("MacOS").getChildFile ("MythosStudio");
    #elif HIGGS_LINUX || HIGGS_BSD
     auto newProcess = targetFolder.getChildFile ("MythosStudio");
    #endif

    StringArray command ("/bin/sh", "-c", "while killall -0 MythosStudio; do sleep 5; done; " + newProcess.getFullPathName().quoted());
   #elif HIGGS_WINDOWS
    auto newProcess = targetFolder.getChildFile ("MythosStudio.exe");

    auto command = "cmd.exe /c\"@echo off & for /l %a in (0) do ( tasklist | find \"MythosStudio\" >nul & ( if errorlevel 1 ( "
                    + targetFolder.getChildFile ("MythosStudio.exe").getFullPathName().quoted() + " & exit /b ) else ( timeout /t 10 >nul ) ) )\"";
   #endif

    if (newProcess.existsAsFile())
    {
        ChildProcess restartProcess;
        restartProcess.start (command, 0);

        MythosStudioApplication::getApp().systemRequestedQuit();
    }
}

void LatestVersionCheckerAndUpdater::downloadAndInstall (const VersionInfo::Asset& asset, const File& targetFolder)
{
    installer.reset (new DownloadAndInstallThread (asset, targetFolder, [this, targetFolder] (const auto result)
    {
        if (result.failed())
        {
            auto options = MessageBoxOptions::makeOptionsOk (MessageBoxIconType::WarningIcon,
                                                             "Installation Failed",
                                                             result.getErrorMessage());

            messageBox = AlertWindow::showScopedAsync (options, nullptr);
        }
        else
        {
            installer.reset();
            restartProcess (targetFolder);
        }
    }));
}
