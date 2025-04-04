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

    class PlistOptions final
    {
    public:
        void write (const File& infoPlistFile) const;

        //==============================================================================
        ProjectType::Target::Type type          = ProjectType::Target::Type::GUIApp;

        String executableName;
        String bundleIdentifier;

        String plistToMerge;

        bool iOS                                = false;

        bool microphonePermissionEnabled        = false;
        String microphonePermissionText;

        bool cameraPermissionEnabled            = false;
        String cameraPermissionText;

        bool bluetoothPermissionEnabled         = false;
        String bluetoothPermissionText;

        bool sendAppleEventsPermissionEnabled   = false;
        String sendAppleEventsPermissionText;

        bool shouldAddStoryboardToProject       = false;
        String storyboardName;

        bool localNetworkPermissionEnabled      = false;
        String localNetworkPermissionText;

        File iconFile;
        File iconComposerIcon;
        String projectName;
        String marketingVersion;
        String currentProjectVersion;
        String companyCopyright;

        String applicationCategory;

        StringPairArray allPreprocessorDefs;
        String documentExtensions;

        bool fileSharingEnabled                 = false;
        bool documentBrowserEnabled             = false;
        bool statusBarHidden                    = false;
        bool requiresFullScreen                 = false;
        bool backgroundAudioEnabled             = false;
        bool backgroundBleEnabled               = false;
        bool pushNotificationsEnabled           = false;

        bool enableIAA                          = false;
        String IAAPluginName;
        String pluginManufacturerCode;
        String IAATypeCode;
        String pluginCode;

        StringArray iPhoneScreenOrientations;
        StringArray iPadScreenOrientations;

        String pluginName;
        String pluginManufacturer;
        String pluginDescription;
        String pluginAUExportPrefix;
        String auMainType;
        String auv3FrameworkBundle;
        bool isAuSandboxSafe                    = false;
        bool isPluginSynth                      = false;
        bool suppressResourceUsage              = false;
        bool isPluginARAEffect                  = false;

    private:
        void write (MemoryOutputStream&) const;
        std::unique_ptr<XmlElement> createXML() const;
        void addIosScreenOrientations (XmlElement&) const;
        void addIosBackgroundModes (XmlElement&) const;
        Array<XmlElement> createExtraAudioUnitTargetPlistOptions() const;
        Array<XmlElement> createExtraAudioUnitV3TargetPlistOptions() const;
    };

} // namespace higgs::build_tools
