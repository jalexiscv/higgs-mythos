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

#include <higgs_core/system/higgs_TargetPlatform.h>

#if HiggsPlugin_Build_Standalone

#if ! HIGGS_MODULE_AVAILABLE_higgs_audio_utils
 #error To compile AudioUnitv3 and/or Standalone plug-ins, you need to add the higgs_audio_utils and higgs_audio_devices modules!
#endif

#include <higgs_core/system/higgs_TargetPlatform.h>
#include <higgs_audio_plugin_client/detail/higgs_CheckSettingMacros.h>

#include <higgs_audio_plugin_client/detail/higgs_IncludeSystemHeaders.h>
#include <higgs_audio_plugin_client/detail/higgs_IncludeModuleHeaders.h>
#include <higgs_gui_basics/native/higgs_WindowsHooks_windows.h>
#include <higgs_audio_plugin_client/detail/higgs_PluginUtilities.h>

#include <higgs_audio_devices/higgs_audio_devices.h>
#include <higgs_gui_extra/higgs_gui_extra.h>
#include <higgs_audio_utils/higgs_audio_utils.h>

// You can set this flag in your build if you need to specify a different
// standalone HIGGSApplication class for your app to use. If you don't
// set it then by default we'll just create a simple one as below.
#if ! HIGGS_USE_CUSTOM_PLUGIN_STANDALONE_APP

#include <higgs_audio_plugin_client/Standalone/higgs_StandaloneFilterWindow.h>

namespace higgs
{

//==============================================================================
class StandaloneFilterApp final : public HIGGSApplication
{
public:
    StandaloneFilterApp()
    {
        PropertiesFile::Options options;

        options.applicationName     = CharPointer_UTF8 (HiggsPlugin_Name);
        options.filenameSuffix      = ".settings";
        options.osxLibrarySubFolder = "Application Support";
       #if HIGGS_LINUX || HIGGS_BSD
        options.folderName          = "~/.config";
       #else
        options.folderName          = "";
       #endif

        appProperties.setStorageParameters (options);
    }

    const String getApplicationName() override              { return CharPointer_UTF8 (HiggsPlugin_Name); }
    const String getApplicationVersion() override           { return HiggsPlugin_VersionString; }
    bool moreThanOneInstanceAllowed() override              { return true; }
    void anotherInstanceStarted (const String&) override    {}

    StandaloneFilterWindow* createWindow()
    {
        if (Desktop::getInstance().getDisplays().displays.isEmpty())
        {
            // No displays are available, so no window will be created!
            jassertfalse;
            return nullptr;
        }

        return new StandaloneFilterWindow (getApplicationName(),
                                           LookAndFeel::getDefaultLookAndFeel().findColour (ResizableWindow::backgroundColourId),
                                           createPluginHolder());
    }

    std::unique_ptr<StandalonePluginHolder> createPluginHolder()
    {
        constexpr auto autoOpenMidiDevices =
       #if (HIGGS_ANDROID || HIGGS_IOS) && ! HIGGS_DONT_AUTO_OPEN_MIDI_DEVICES_ON_MOBILE
                true;
       #else
                false;
       #endif


       #ifdef HiggsPlugin_PreferredChannelConfigurations
        constexpr StandalonePluginHolder::PluginInOuts channels[] { HiggsPlugin_PreferredChannelConfigurations };
        const Array<StandalonePluginHolder::PluginInOuts> channelConfig (channels, higgs::numElementsInArray (channels));
       #else
        const Array<StandalonePluginHolder::PluginInOuts> channelConfig;
       #endif

        return std::make_unique<StandalonePluginHolder> (appProperties.getUserSettings(),
                                                         false,
                                                         String{},
                                                         nullptr,
                                                         channelConfig,
                                                         autoOpenMidiDevices);
    }

    //==============================================================================
    void initialise (const String&) override
    {
        mainWindow = rawToUniquePtr (createWindow());

        if (mainWindow != nullptr)
        {
           #if HIGGS_STANDALONE_FILTER_WINDOW_USE_KIOSK_MODE
            Desktop::getInstance().setKioskModeComponent (mainWindow.get(), false);
           #endif

            mainWindow->setVisible (true);
        }
        else
        {
            pluginHolder = createPluginHolder();
        }
    }

    void shutdown() override
    {
        pluginHolder = nullptr;
        mainWindow = nullptr;
        appProperties.saveIfNeeded();
    }

    //==============================================================================
    void systemRequestedQuit() override
    {
        if (pluginHolder != nullptr)
            pluginHolder->savePluginState();

        if (mainWindow != nullptr)
            mainWindow->pluginHolder->savePluginState();

        if (ModalComponentManager::getInstance()->cancelAllModalComponents())
        {
            Timer::callAfterDelay (100, []()
            {
                if (auto app = HIGGSApplicationBase::getInstance())
                    app->systemRequestedQuit();
            });
        }
        else
        {
            quit();
        }
    }

protected:
    ApplicationProperties appProperties;
    std::unique_ptr<StandaloneFilterWindow> mainWindow;

private:
    std::unique_ptr<StandalonePluginHolder> pluginHolder;
};

} // namespace higgs

#if HiggsPlugin_Build_Standalone && HIGGS_IOS

HIGGS_BEGIN_IGNORE_WARNINGS_GCC_LIKE ("-Wmissing-prototypes")

using namespace higgs;

bool HIGGS_CALLTYPE higgs_isInterAppAudioConnected()
{
    if (auto holder = StandalonePluginHolder::getInstance())
        return holder->isInterAppAudioConnected();

    return false;
}

void HIGGS_CALLTYPE higgs_switchToHostApplication()
{
    if (auto holder = StandalonePluginHolder::getInstance())
        holder->switchToHostApplication();
}

Image HIGGS_CALLTYPE higgs_getIAAHostIcon (int size)
{
    if (auto holder = StandalonePluginHolder::getInstance())
        return holder->getIAAHostIcon (size);

    return Image();
}

HIGGS_END_IGNORE_WARNINGS_GCC_LIKE

#endif

#endif

#if HIGGS_USE_CUSTOM_PLUGIN_STANDALONE_APP
 extern higgs::HIGGSApplicationBase* higgs_CreateApplication();

 #if HIGGS_IOS
  extern void* higgs_GetIOSCustomDelegateClass();
 #endif

#else
 HIGGS_CREATE_APPLICATION_DEFINE (higgs::StandaloneFilterApp)
#endif

#if ! HIGGS_USE_CUSTOM_PLUGIN_STANDALONE_ENTRYPOINT
 HIGGS_MAIN_FUNCTION_DEFINITION
#endif

#endif
