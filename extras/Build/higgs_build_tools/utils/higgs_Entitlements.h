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

    struct EntitlementOptions final
    {
        String getEntitlementsFileContent() const;

        ProjectType::Target::Type type      = ProjectType::Target::GUIApp;

        bool isiOS                          = false;
        bool isAudioPluginProject           = false;
        bool shouldEnableIAA                = false;
        bool isAUPluginHost                 = false;
        bool isiCloudPermissionsEnabled     = false;
        bool isPushNotificationsEnabled     = false;
        bool isAppGroupsEnabled             = false;
        bool isHardenedRuntimeEnabled       = false;
        bool isAppSandboxEnabled            = false;
        bool isAppSandboxInhertianceEnabled = false;
        bool isNetworkingMulticastEnabled   = false;

        String appGroupIdString;

        StringArray hardenedRuntimeOptions;
        StringArray appSandboxOptions;

        struct KeyAndStringArray
        {
            String key;
            StringArray values;
        };

        std::vector<KeyAndStringArray> appSandboxTemporaryPaths;
        StringArray appSandboxExceptionIOKit;

    private:
        StringPairArray getEntitlements() const;
    };

} // namespace higgs::build_tools
