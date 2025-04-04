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

namespace higgs:: build_tools
{

    String EntitlementOptions::getEntitlementsFileContent() const
    {
        String content =
            "<?xml version=\"1.0\" encoding=\"UTF-8\"?>\n"
            "<!DOCTYPE plist PUBLIC \"-//Apple//DTD PLIST 1.0//EN\" \"http://www.apple.com/DTDs/PropertyList-1.0.dtd\">\n"
            "<plist version=\"1.0\">\n"
            "<dict>\n";

        const auto entitlements = getEntitlements();

        for (auto& key : entitlements.getAllKeys())
            content += "\t<key>" + key + "</key>\n\t" + entitlements[key] + "\n";

        return content + "</dict>\n</plist>\n";
    }

    StringPairArray EntitlementOptions::getEntitlements() const
    {
        StringPairArray entitlements;

        if (isiOS)
        {
            // The Inter-App Audio entitlement is currently deprecated, but it
            // also "provides access to Audio Unit extensions". Without the
            // entitlement iOS apps are unable to access AUv3 plug-ins.
            if ((isAudioPluginProject && shouldEnableIAA) || isAUPluginHost)
                entitlements.set ("inter-app-audio", "<true/>");

            if (isiCloudPermissionsEnabled)
            {
                entitlements.set ("com.apple.developer.icloud-container-identifiers",
                                  "<array>\n"
                                  "        <string>iCloud.$(CFBundleIdentifier)</string>\n"
                                  "    </array>");

                entitlements.set ("com.apple.developer.icloud-services",
                                  "<array>\n"
                                  "        <string>CloudDocuments</string>\n"
                                  "    </array>");

                entitlements.set ("com.apple.developer.ubiquity-container-identifiers",
                                  "<array>\n"
                                  "        <string>iCloud.$(CFBundleIdentifier)</string>\n"
                                  "    </array>");
            }
        }

        if (isPushNotificationsEnabled)
            entitlements.set (isiOS ? "aps-environment"
                                    : "com.apple.developer.aps-environment",
                              "<string>development</string>");

        if (isAppGroupsEnabled)
        {
            auto appGroups = StringArray::fromTokens (appGroupIdString, ";", {});
            String groups = "<array>";

            for (auto group : appGroups)
                groups += "\n\t\t<string>" + group.trim() + "</string>";

            groups += "\n\t</array>";

            entitlements.set ("com.apple.security.application-groups", groups);
        }

        if (isHardenedRuntimeEnabled)
            for (auto& option : hardenedRuntimeOptions)
                entitlements.set (option, "<true/>");

        if (isAppSandboxEnabled || (! isiOS && isAudioPluginProject && type == ProjectType::Target::AudioUnitv3PlugIn))
        {
            entitlements.set ("com.apple.security.app-sandbox", "<true/>");

            if (isAppSandboxInhertianceEnabled)
            {
                // no other sandbox options can be specified if sandbox inheritance is enabled!
                jassert (appSandboxOptions.isEmpty());
                jassert (appSandboxTemporaryPaths.empty());

                entitlements.set ("com.apple.security.inherit", "<true/>");
            }

            if (isAppSandboxEnabled)
            {
                for (auto& option : appSandboxOptions)
                    entitlements.set (option, "<true/>");

                for (auto& option : appSandboxTemporaryPaths)
                {
                    String paths = "<array>";

                    for (const auto& path : option.values)
                        paths += "\n\t\t<string>" + path + "</string>";

                    paths += "\n\t</array>";
                    entitlements.set (option.key, paths);
                }

                if (! appSandboxExceptionIOKit.isEmpty())
                {
                    String ioKitClasses = "<array>";

                    for (const auto& c : appSandboxExceptionIOKit)
                        ioKitClasses += "\n\t\t<string>" + c + "</string>";

                    ioKitClasses += "\n\t</array>";
                    entitlements.set ("com.apple.security.temporary-exception.iokit-user-client-class", ioKitClasses);
                }
            }
        }

        if (isNetworkingMulticastEnabled)
            entitlements.set ("com.apple.developer.networking.multicast", "<true/>");

        return entitlements;
    }

} // namespace higgs::build_tools
