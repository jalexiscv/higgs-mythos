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

#include <higgs_build_tools/higgs_build_tools.h>

#include <fstream>
#include <unordered_map>

namespace
{

constexpr auto headerTemplate = R"(/*
    IMPORTANT! This file is auto-generated.
    If you alter its contents, your changes may be overwritten!

    This is the header file that your files should include in order to get all the
    HIGGS library headers. You should avoid including the HIGGS headers directly in
    your own source files, because that wouldn't pick up the correct configuration
    options for your app.

*/

#pragma once

${HIGGS_INCLUDES}

#if HIGGS_TARGET_HAS_BINARY_DATA
 #include "BinaryData.h"
#endif

#if ! DONT_SET_USING_HIGGS_NAMESPACE
 // If your code uses a lot of HIGGS classes, then this will obviously save you
 // a lot of typing, but can be disabled by setting DONT_SET_USING_HIGGS_NAMESPACE.
 using namespace higgs;
#endif

#if ! HIGGS_DONT_DECLARE_PROJECTINFO
namespace ProjectInfo
{
    const char* const  projectName    = "${HIGGS_EXECUTABLE_NAME}";
    const char* const  companyName    = "${HIGGS_COMPANY_NAME}";
    const char* const  versionString  = "${HIGGS_PROJECT_VERSION}";
    const int          versionNumber  =  ${HIGGS_PROJECT_VERSION_HEX};
}
#endif
)";

int writeBinaryData (higgs::ArgumentList&& args)
{
    args.checkMinNumArguments (4);
    const auto namespaceName = args.arguments.removeAndReturn (0);
    const auto headerName    = args.arguments.removeAndReturn (0);
    const auto outFolder     = args.arguments.removeAndReturn (0).resolveAsExistingFolder();
    const auto inputFileList = args.arguments.removeAndReturn (0).resolveAsExistingFile();

    higgs::build_tools::ResourceFile resourceFile;

    resourceFile.setClassName (namespaceName.text);
    const auto lineEndings = args.removeOptionIfFound ("--windows") ? "\r\n" : "\n";

    const auto allLines = [&]
    {
        auto lines = higgs::StringArray::fromLines (inputFileList.loadFileAsString());
        lines.removeEmptyStrings();
        return lines;
    }();

    for (const auto& arg : allLines)
        resourceFile.addFile (higgs::File (arg));

    const auto result = resourceFile.write (0,
                                            lineEndings,
                                            outFolder.getChildFile (headerName.text),
                                            [&outFolder] (int index)
                                            {
                                                return outFolder.getChildFile ("./BinaryData" + higgs::String { index + 1 } + ".cpp");
                                            });

    if (result.result.failed())
        higgs::ConsoleApplication::fail (result.result.getErrorMessage(), 1);

    return 0;
}

struct IconParseResults
{
    higgs::build_tools::Icons icons;
    higgs::File output;
    higgs::String iconName;
};

enum class IconNameSpecified
{
    no,
    yes
};

IconParseResults parseIconArguments (higgs::ArgumentList&& args, IconNameSpecified iconNameSpecified)
{
    args.checkMinNumArguments (iconNameSpecified == IconNameSpecified::yes ? 3 : 2);
    const auto output = args.arguments.removeAndReturn (0);
    const auto iconName = std::invoke ([&]
    {
        if (iconNameSpecified == IconNameSpecified::yes)
            return args.arguments.removeAndReturn (0).text;

        return higgs::String{};
    });

    const auto popFile = [&args]() -> higgs::File
    {
        if (args.size() == 0)
            return {};

        return args.arguments.removeAndReturn (0).text;
    };

    const auto smallIcon = popFile();
    const auto bigIcon   = popFile();

    return { higgs::build_tools::Icons::fromFilesSmallAndBig (smallIcon, bigIcon), output.text, iconName };
}

int writeMacIcon (higgs::ArgumentList&& argumentList)
{
    const auto parsed = parseIconArguments (std::move (argumentList), IconNameSpecified::yes);
    higgs::build_tools::writeMacIcon (parsed.icons, parsed.output);
    return 0;
}

int writeiOSAssets (higgs::ArgumentList&& argumentList)
{
    const auto parsed = parseIconArguments (std::move (argumentList), IconNameSpecified::yes);
    higgs::build_tools::createXcassetsFolderFromIcons (parsed.icons,
                                                      parsed.output.getParentDirectory(),
                                                      parsed.output.getFileName(),
                                                      parsed.iconName);
    return 0;
}

int writeWinIcon (higgs::ArgumentList&& argumentList)
{
    const auto parsed = parseIconArguments (std::move (argumentList), IconNameSpecified::no);
    higgs::build_tools::writeWinIcon (parsed.icons, parsed.output);
    return 0;
}

std::unordered_map<higgs::String, higgs::String> parseProjectData (const higgs::File& file)
{
    constexpr auto recordSeparator = "\x1e";
    const auto contents = file.loadFileAsString();
    const auto lines    = higgs::StringArray::fromTokens (contents, recordSeparator, {});

    std::unordered_map<higgs::String, higgs::String> result;

    constexpr auto unitSeparator = "\x1f";

    for (const auto& line : lines)
    {
        if (line.isEmpty())
            continue;

        result.emplace (line.upToFirstOccurrenceOf (unitSeparator, false, false),
                        line.fromFirstOccurrenceOf (unitSeparator, false, false));
    }

    return result;
}

higgs::String getStringValue (const std::unordered_map<higgs::String, higgs::String>& dict,
                             higgs::StringRef key)
{
    const auto it = dict.find (key);
    return it != dict.cend() ? it->second : higgs::String{};
}

bool getBoolValue (const std::unordered_map<higgs::String, higgs::String>& dict, higgs::StringRef key)
{
    const auto str = getStringValue (dict, key);
    return str.equalsIgnoreCase ("yes")
        || str.equalsIgnoreCase ("true")
        || str.equalsIgnoreCase ("1")
        || str.equalsIgnoreCase ("on");
}

struct UpdateField
{
    const std::unordered_map<higgs::String, higgs::String>& dict;

    void operator() (higgs::StringRef key, higgs::String& value) const
    {
        value = getStringValue (dict, key);
    }

    void operator() (higgs::StringRef key, higgs::File& value) const
    {
        value = getStringValue (dict, key);
    }

    void operator() (higgs::StringRef key, bool& value) const
    {
        value = getBoolValue (dict, key);
    }

    void operator() (higgs::StringRef key, higgs::StringArray& value) const
    {
        value = higgs::StringArray::fromTokens (getStringValue (dict, key), ";", {});
    }
};

void setIfEmpty (higgs::String& field, higgs::StringRef fallback)
{
    if (field.isEmpty())
        field = fallback;
}

higgs::build_tools::PlistOptions parsePlistOptions (const higgs::File& file,
                                                   higgs::build_tools::ProjectType::Target::Type type)
{
    if (type == higgs::build_tools::ProjectType::Target::ConsoleApp)
        higgs::ConsoleApplication::fail ("Deduced project type does not require a plist", 1);

    const auto dict = parseProjectData (file);

    UpdateField updateField { dict };

    higgs::build_tools::PlistOptions result;

    updateField ("EXECUTABLE_NAME",                      result.executableName);
    updateField ("PLIST_TO_MERGE",                       result.plistToMerge);
    updateField ("IS_IOS",                               result.iOS);
    updateField ("MICROPHONE_PERMISSION_ENABLED",        result.microphonePermissionEnabled);
    updateField ("MICROPHONE_PERMISSION_TEXT",           result.microphonePermissionText);
    updateField ("CAMERA_PERMISSION_ENABLED",            result.cameraPermissionEnabled);
    updateField ("CAMERA_PERMISSION_TEXT",               result.cameraPermissionText);
    updateField ("BLUETOOTH_PERMISSION_ENABLED",         result.bluetoothPermissionEnabled);
    updateField ("BLUETOOTH_PERMISSION_TEXT",            result.bluetoothPermissionText);
    updateField ("LOCAL_NETWORK_PERMISSION_ENABLED",     result.localNetworkPermissionEnabled);
    updateField ("LOCAL_NETWORK_PERMISSION_TEXT",        result.localNetworkPermissionText);
    updateField ("SEND_APPLE_EVENTS_PERMISSION_ENABLED", result.sendAppleEventsPermissionEnabled);
    updateField ("SEND_APPLE_EVENTS_PERMISSION_TEXT",    result.sendAppleEventsPermissionText);
    updateField ("SHOULD_ADD_STORYBOARD",                result.shouldAddStoryboardToProject);
    updateField ("LAUNCH_STORYBOARD_FILE",               result.storyboardName);
    updateField ("PROJECT_NAME",                         result.projectName);
    updateField ("VERSION",                              result.marketingVersion);
    updateField ("BUILD_VERSION",                        result.currentProjectVersion);
    updateField ("COMPANY_COPYRIGHT",                    result.companyCopyright);
    updateField ("DOCUMENT_EXTENSIONS",                  result.documentExtensions);
    updateField ("FILE_SHARING_ENABLED",                 result.fileSharingEnabled);
    updateField ("DOCUMENT_BROWSER_ENABLED",             result.documentBrowserEnabled);
    updateField ("STATUS_BAR_HIDDEN",                    result.statusBarHidden);
    updateField ("REQUIRES_FULL_SCREEN",                 result.requiresFullScreen);
    updateField ("BACKGROUND_AUDIO_ENABLED",             result.backgroundAudioEnabled);
    updateField ("BACKGROUND_BLE_ENABLED",               result.backgroundBleEnabled);
    updateField ("PUSH_NOTIFICATIONS_ENABLED",           result.pushNotificationsEnabled);
    updateField ("PLUGIN_MANUFACTURER_CODE",             result.pluginManufacturerCode);
    updateField ("PLUGIN_CODE",                          result.pluginCode);
    updateField ("IPHONE_SCREEN_ORIENTATIONS",           result.iPhoneScreenOrientations);
    updateField ("IPAD_SCREEN_ORIENTATIONS",             result.iPadScreenOrientations);
    updateField ("PLUGIN_NAME",                          result.pluginName);
    updateField ("PLUGIN_MANUFACTURER",                  result.pluginManufacturer);
    updateField ("PLUGIN_DESCRIPTION",                   result.pluginDescription);
    updateField ("PLUGIN_AU_EXPORT_PREFIX",              result.pluginAUExportPrefix);
    updateField ("PLUGIN_AU_MAIN_TYPE",                  result.auMainType);
    updateField ("PLUGIN_AU_FRAMEWORK_BUNDLE_ID",        result.auv3FrameworkBundle);
    updateField ("IS_AU_SANDBOX_SAFE",                   result.isAuSandboxSafe);
    updateField ("IS_PLUGIN_SYNTH",                      result.isPluginSynth);
    updateField ("IS_PLUGIN_ARA_EFFECT",                 result.isPluginARAEffect);
    updateField ("SUPPRESS_AU_PLIST_RESOURCE_USAGE",     result.suppressResourceUsage);
    updateField ("BUNDLE_ID",                            result.bundleIdentifier);
    updateField ("ICON_FILE",                            result.iconFile);
    updateField ("ICON_COMPOSER_BUNDLE",                 result.iconComposerIcon);

    result.type = type;

    if (result.storyboardName.isNotEmpty())
        result.storyboardName = result.storyboardName.fromLastOccurrenceOf ("/", false, false)
                                                     .upToLastOccurrenceOf (".storyboard", false, false);

    setIfEmpty (result.microphonePermissionText,
                "This app requires audio input. If you do not have an audio interface connected it will use the built-in microphone.");
    setIfEmpty (result.cameraPermissionText,
                "This app requires access to the camera to function correctly.");
    setIfEmpty (result.bluetoothPermissionText,
                "This app requires access to Bluetooth to function correctly.");
    setIfEmpty (result.sendAppleEventsPermissionText,
                "This app requires the ability to send Apple events to function correctly.");
    setIfEmpty (result.localNetworkPermissionText,
                "This app requires access to the local network to function correctly.");

    result.documentExtensions = result.documentExtensions.replace (";", ",");

    // AUv3 needs a slightly different bundle ID
    if (type == higgs::build_tools::ProjectType::Target::Type::AudioUnitv3PlugIn)
    {
        const auto bundleIdSegments = higgs::StringArray::fromTokens (result.bundleIdentifier, ".", {});
        jassert (! bundleIdSegments.isEmpty());

        const auto last = bundleIdSegments.isEmpty() ? ""
                                                     : bundleIdSegments[bundleIdSegments.size() - 1];

        result.bundleIdentifier += "." + last + "AUv3";
    }

    return result;
}

int writePlist (higgs::ArgumentList&& args)
{
    args.checkMinNumArguments (3);
    const auto kind   = args.arguments.removeAndReturn (0);
    const auto input  = args.arguments.removeAndReturn (0);
    const auto output = args.arguments.removeAndReturn (0);
    parsePlistOptions (input.resolveAsExistingFile(),
                       higgs::build_tools::ProjectType::Target::typeFromName (kind.text))
        .write (output.resolveAsFile());
    return 0;
}

higgs::build_tools::EntitlementOptions parseEntitlementsOptions (const higgs::File& file,
                                                                higgs::build_tools::ProjectType::Target::Type type)
{
    const auto dict = parseProjectData (file);

    UpdateField updateField { dict };

    higgs::build_tools::EntitlementOptions result;

    updateField ("IS_IOS",                          result.isiOS);
    updateField ("IS_PLUGIN",                       result.isAudioPluginProject);
    updateField ("IS_AU_PLUGIN_HOST",               result.isAUPluginHost);
    updateField ("ICLOUD_PERMISSIONS_ENABLED",      result.isiCloudPermissionsEnabled);
    updateField ("PUSH_NOTIFICATIONS_ENABLED",      result.isPushNotificationsEnabled);
    updateField ("APP_GROUPS_ENABLED",              result.isAppGroupsEnabled);
    updateField ("APP_GROUP_IDS",                   result.appGroupIdString);
    updateField ("HARDENED_RUNTIME_ENABLED",        result.isHardenedRuntimeEnabled);
    updateField ("HARDENED_RUNTIME_OPTIONS",        result.hardenedRuntimeOptions);
    updateField ("APP_SANDBOX_ENABLED",             result.isAppSandboxEnabled);
    updateField ("APP_SANDBOX_INHERIT",             result.isAppSandboxInhertianceEnabled);
    updateField ("APP_SANDBOX_OPTIONS",             result.appSandboxOptions);
    updateField ("NETWORK_MULTICAST_ENABLED",       result.isNetworkingMulticastEnabled);

    struct SandboxTemporaryAccessKey
    {
        higgs::String cMakeVar, key;
    };

    SandboxTemporaryAccessKey sandboxTemporaryAccessKeys[]
    {
        { "APP_SANDBOX_FILE_ACCESS_HOME_RO", "home-relative-path.read-only" },
        { "APP_SANDBOX_FILE_ACCESS_HOME_RW", "home-relative-path.read-write" },
        { "APP_SANDBOX_FILE_ACCESS_ABS_RO",  "absolute-path.read-only" },
        { "APP_SANDBOX_FILE_ACCESS_ABS_RW",  "absolute-path.read-write" }
    };

    for (const auto& entry : sandboxTemporaryAccessKeys)
    {
        higgs::StringArray values;
        updateField (entry.cMakeVar, values);

        if (! values.isEmpty())
            result.appSandboxTemporaryPaths.push_back ({ "com.apple.security.temporary-exception.files." + entry.key,
                                                         std::move (values) });
    }

    {
        higgs::StringArray values;
        updateField ("APP_SANDBOX_EXCEPTION_IOKIT", values);

        if (! values.isEmpty())
            result.appSandboxExceptionIOKit = values;
    }

    result.type = type;

    return result;
}

int writeEntitlements (higgs::ArgumentList&& args)
{
    args.checkMinNumArguments (3);
    const auto kind   = args.arguments.removeAndReturn (0);
    const auto input  = args.arguments.removeAndReturn (0);
    const auto output = args.arguments.removeAndReturn (0);

    const auto options = parseEntitlementsOptions (input.resolveAsExistingFile(),
                                                   higgs::build_tools::ProjectType::Target::typeFromName (kind.text));
    higgs::build_tools::overwriteFileIfDifferentOrThrow (output.resolveAsFile(), options.getEntitlementsFileContent());
    return 0;
}

int createAndWrite (const higgs::File& file, higgs::StringRef text)
{
    if (file.create())
        return file.replaceWithText (text) ? 0 : 1;

    return 1;
}

int writePkgInfo (higgs::ArgumentList&& args)
{
    args.checkMinNumArguments (2);
    const auto kind   = args.arguments.removeAndReturn (0);
    const auto output = args.arguments.removeAndReturn (0);

    const auto projectType = higgs::build_tools::ProjectType::Target::typeFromName (kind.text);
    return createAndWrite (output.resolveAsFile(),
                           higgs::build_tools::getXcodePackageType (projectType)
                           + higgs::build_tools::getXcodeBundleSignature (projectType));
}

higgs::build_tools::ResourceRcOptions parseRcFileOptions (const higgs::File& file)
{
    const auto dict = parseProjectData (file);
    UpdateField updateField { dict };

    higgs::build_tools::ResourceRcOptions result;

    updateField ("VERSION",           result.version);
    updateField ("COMPANY_NAME",      result.companyName);
    updateField ("COMPANY_COPYRIGHT", result.companyCopyright);
    updateField ("PROJECT_NAME",      result.projectName);
    updateField ("ICON_FILE",         result.icon);

    return result;
}

int writeRcFile (higgs::ArgumentList&& args)
{
    args.checkMinNumArguments (2);
    const auto input  = args.arguments.removeAndReturn (0);
    const auto output = args.arguments.removeAndReturn (0);
    parseRcFileOptions (input.resolveAsExistingFile()).write (output.resolveAsFile());
    return 0;
}

higgs::String createDefineStatements (higgs::StringRef definitions)
{
    const auto split = higgs::StringArray::fromTokens (definitions, ";", "\"");

    higgs::String defineStatements;

    for (const auto& def : split)
    {
        if (! def.startsWith ("HiggsPlugin_"))
            continue;

        const auto defineName  = def.upToFirstOccurrenceOf ("=", false, false);
        const auto defineValue = def.fromFirstOccurrenceOf ("=", false, false);
        defineStatements += "#define " + defineName + " " + defineValue + '\n';
    }

    return defineStatements;
}

int writeAuPluginDefines (higgs::ArgumentList&& args)
{
    args.checkMinNumArguments (2);
    const auto input  = args.arguments.removeAndReturn (0);
    const auto output = args.arguments.removeAndReturn (0);

    const auto dict      = parseProjectData (input.resolveAsExistingFile());
    const auto getString = [&] (higgs::StringRef key) { return getStringValue (dict, key); };

    const auto defines = "#pragma once\n" + createDefineStatements (getString ("MODULE_DEFINITIONS"));
    return createAndWrite (output.resolveAsFile(), defines);
}

higgs::String createIncludeStatements (higgs::StringRef definitions)
{
    const auto split = higgs::StringArray::fromTokens (definitions, ";", "\"");

    higgs::String includeStatements;

    for (const auto& def : split)
    {
        constexpr auto moduleToken = "HIGGS_MODULE_AVAILABLE_";

        if (def.startsWith (moduleToken))
        {
            const auto moduleName = def.fromFirstOccurrenceOf (moduleToken, false, false)
                                       .upToFirstOccurrenceOf ("=", false, false);
            includeStatements += "#include <" + moduleName + "/" + moduleName + ".h>\n";
        }
    }

    return includeStatements;
}

int writeHeader (higgs::ArgumentList&& args)
{
    args.checkMinNumArguments (2);
    const auto input  = args.arguments.removeAndReturn (0);
    const auto output = args.arguments.removeAndReturn (0);

    const auto dict = parseProjectData (input.resolveAsExistingFile());

    const auto getString = [&] (higgs::StringRef key) { return getStringValue (dict, key); };

    const auto includes      = createIncludeStatements (getString ("MODULE_DEFINITIONS"));
    const auto projectName   = getString ("PROJECT_NAME");
    const auto name          = projectName.isEmpty() ? getString ("EXECUTABLE_NAME") : projectName;
    const auto versionString = getString ("VERSION");

    const auto headerText = higgs::String (headerTemplate)
                                .replace ("${HIGGS_INCLUDES}", includes)
                                .replace ("${HIGGS_EXECUTABLE_NAME}", name)
                                .replace ("${HIGGS_COMPANY_NAME}", getString ("COMPANY_NAME"))
                                .replace ("${HIGGS_PROJECT_VERSION}", versionString)
                                .replace ("${HIGGS_PROJECT_VERSION_HEX}", higgs::build_tools::getVersionAsHex (versionString));

    return createAndWrite (output.resolveAsFile(), headerText);
}

int printHIGGSVersion (higgs::ArgumentList&&)
{
    std::cout << higgs::SystemStats::getHIGGSVersion() << std::endl;
    return 0;
}

} // namespace

int main (int argc, char** argv)
{
    higgs::ScopedHiggsInitialiser_GUI libraryInitialiser;

    return higgs::ConsoleApplication::invokeCatchingFailures ([argc, argv]
    {
        if (argc < 1)
            higgs::ConsoleApplication::fail ("No arguments passed", 1);

        const auto getString = [&] (const char* text)
        {
            return higgs::String (higgs::CharPointer_UTF8 (text));
        };

        std::vector<higgs::String> arguments;
        std::transform (argv, argv + argc, std::back_inserter (arguments), getString);

        higgs::ArgumentList argumentList { arguments.front(),
                                          higgs::StringArray (arguments.data() + 1, (int) arguments.size() - 1) };

        using Fn = int (*) (higgs::ArgumentList&&);

        const std::unordered_map<higgs::String, Fn> commands
        {
            { "auplugindefines", writeAuPluginDefines },
            { "binarydata",      writeBinaryData },
            { "entitlements",    writeEntitlements },
            { "header",          writeHeader },
            { "iosassets",       writeiOSAssets },
            { "macicon",         writeMacIcon },
            { "pkginfo",         writePkgInfo },
            { "plist",           writePlist },
            { "rcfile",          writeRcFile },
            { "version",         printHIGGSVersion },
            { "winicon",         writeWinIcon }
        };

        argumentList.checkMinNumArguments (1);
        const auto mode = argumentList.arguments.removeAndReturn (0);
        const auto it   = commands.find (mode.text);

        if (it == commands.cend())
            higgs::ConsoleApplication::fail ("No matching mode", 1);

        try
        {
            return it->second (std::move (argumentList));
        }
        catch (const higgs::build_tools::SaveError& error)
        {
            higgs::ConsoleApplication::fail (error.message);
        }
        catch (const std::exception& ex)
        {
            higgs::ConsoleApplication::fail (ex.what());
        }
        catch (...)
        {
            higgs::ConsoleApplication::fail ("Unhandled exception");
        }

        return 1;
    });

    return 0;
}
