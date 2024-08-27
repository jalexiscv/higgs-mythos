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

//==============================================================================
// This suppresses a warning caused by some of the Steinberg source code
#ifndef _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
 #define _SILENCE_CXX17_CODECVT_HEADER_DEPRECATION_WARNING
#endif

//==============================================================================
// This suppresses a warning in higgs_TargetPlatform.h
#ifndef HIGGS_GLOBAL_MODULE_SETTINGS_INCLUDED
 #define HIGGS_GLOBAL_MODULE_SETTINGS_INCLUDED 1
#endif

#include <higgs_core/system/higgs_CompilerWarnings.h>
#include <higgs_core/system/higgs_CompilerSupport.h>

#include <array>
#include <atomic>
#include <cassert>
#include <cctype>
#include <cstddef>
#include <cstring>

// There's a template specialisation to std::wstring_convert in
// wstring_convert.h that triggers a warning on Xcode 26.4.1 when used by the
// VST3 SDK. It cannot be suppressed at the call site. This file is included
// in <locale>, and would transitively be included by <functional>.
#if defined (__apple_build_version__)
HIGGS_BEGIN_IGNORE_WARNINGS_GCC_LIKE ("-Wdeprecated-declarations")
#include <locale>
HIGGS_END_IGNORE_WARNINGS_GCC_LIKE
#endif

#include <functional>
#include <iomanip>
#include <ios>
#include <memory>
#include <sstream>
#include <string>
#include <utility>
#include <vector>

//==============================================================================
HIGGS_BEGIN_IGNORE_WARNINGS_GCC_LIKE ("-Wc++98-compat-extra-semi",
                                     "-Wdeprecated-declarations",
                                     "-Wexpansion-to-defined",
                                     "-Wfloat-equal",
                                     "-Wformat",
                                     "-Wmissing-prototypes",
                                     "-Wpragma-pack",
                                     "-Wredundant-decls",
                                     "-Wshadow",
                                     "-Wshadow-field",
                                     "-Wshorten-64-to-32",
                                     "-Wsign-conversion",
                                     "-Wzero-as-null-pointer-constant")

HIGGS_BEGIN_IGNORE_WARNINGS_MSVC (4100 6031 6387)

#ifndef NOMINMAX
 #define NOMINMAX
#endif

#include <higgs_audio_processors_headless/format_types/VST3_SDK/pluginterfaces/base/coreiids.cpp>
#include <higgs_audio_processors_headless/format_types/VST3_SDK/pluginterfaces/base/funknown.cpp>
#include <higgs_audio_processors_headless/format_types/VST3_SDK/public.sdk/source/common/commonstringconvert.cpp>
#include <higgs_audio_processors_headless/format_types/VST3_SDK/public.sdk/source/common/memorystream.cpp>
#include <higgs_audio_processors_headless/format_types/VST3_SDK/public.sdk/source/common/readfile.cpp>
#include <higgs_audio_processors_headless/format_types/VST3_SDK/public.sdk/source/vst/hosting/module.cpp>
#include <higgs_audio_processors_headless/format_types/VST3_SDK/public.sdk/source/vst/moduleinfo/moduleinfocreator.cpp>
#include <higgs_audio_processors_headless/format_types/VST3_SDK/public.sdk/source/vst/moduleinfo/moduleinfoparser.cpp>
#include <higgs_audio_processors_headless/format_types/VST3_SDK/public.sdk/source/vst/utility/stringconvert.cpp>
#include <higgs_audio_processors_headless/format_types/VST3_SDK/public.sdk/source/vst/vstinitiids.cpp>

HIGGS_END_IGNORE_WARNINGS_MSVC
HIGGS_END_IGNORE_WARNINGS_GCC_LIKE

//==============================================================================
#if HiggsPlugin_Enable_ARA
 HIGGS_BEGIN_IGNORE_WARNINGS_GCC_LIKE ("-Wpragma-pack")
 #include <ARA_API/ARAVST3.h>
 HIGGS_END_IGNORE_WARNINGS_GCC_LIKE
#endif // HiggsPlugin_Enable_ARA

//==============================================================================
#define jassert(x) assert ((x))
#define jassertfalse assert (false)
#define DBG(x)
#define HIGGS_DECLARE_NON_COPYABLE(x)
#define HIGGS_API

#if __has_include ("HiggsPluginDefines.h")
 #include "HiggsPluginDefines.h"
#endif

#if __has_include ("AppConfig.h")
 #include "AppConfig.h"
#endif

#include <higgs_core/detail/higgs_IncrementRef.h>
#include <higgs_audio_processors_headless/format_types/higgs_VST3Utilities.h>
#include <higgs_audio_processors_headless/utilities/higgs_VST3Interface.h>
#include <higgs_audio_plugin_client/higgs_audio_plugin_client.h>
#include "higgs_VST3ModuleInfo.h"

//==============================================================================
class HiggsPluginModule : public VST3::Hosting::Module
{
public:
    HiggsPluginModule()
    {
        using namespace Steinberg;
        using namespace VST3::Hosting;
        PluginFactory tmp { owned (new higgs::HiggsPluginFactoryBase()) };
        factory = std::move (tmp);
    }

private:
    bool load (const std::string&, std::string&) final { return {}; }
};

static std::optional<Steinberg::ModuleInfo::CompatibilityList> loadCompatibilityFromModule (const VST3::Hosting::Module& pluginModule)
{
    const auto& factory = pluginModule.getFactory();
    const auto& infos = factory.classInfos();

    const auto iter = std::find_if (infos.begin(), infos.end(), [&] (const auto& info)
    {
        return info.category() == kPluginCompatibilityClass;
    });

    if (iter == infos.end())
        return {};

    const auto compatibility = factory.createInstance<Steinberg::IPluginCompatibility> (iter->ID());

    if (compatibility == nullptr)
        return {};

    Steinberg::MemoryStream stream;

    if (compatibility->getCompatibilityJSON (&stream) != Steinberg::kResultOk)
        return {};

    const std::string_view streamView (stream.getData(), (size_t) stream.getSize());

    return Steinberg::ModuleInfoLib::parseCompatibilityJson (streamView, nullptr);
}

//==============================================================================
int main()
{
    const HiggsPluginModule pluginModule;

    auto moduleInfo = Steinberg::ModuleInfoLib::createModuleInfo (pluginModule, false);

    if (auto compatibility = loadCompatibilityFromModule (pluginModule))
        moduleInfo.compatibility = *compatibility;

    moduleInfo.name = HiggsPlugin_Name;
    moduleInfo.version = HiggsPlugin_VersionString;

    std::stringstream output;
    Steinberg::ModuleInfoLib::outputJson (moduleInfo, output);
    std::cout << output.str() << std::endl;
    return 0;
}

//==============================================================================
namespace VST3::Hosting
{
    Module::SnapshotList Module::getSnapshots (const std::string&) { return {}; }
    Optional<std::string> Module::getModuleInfoPath (const std::string&) { return {}; }
    Module::Ptr Module::create (const std::string&, std::string&) { return {}; }
} // namespace VST3::Hosting
