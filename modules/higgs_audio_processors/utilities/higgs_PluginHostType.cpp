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

#if HiggsPlugin_Enable_IAA && HiggsPlugin_Build_Standalone && HIGGS_IOS && (! HIGGS_USE_CUSTOM_PLUGIN_STANDALONE_APP)
 bool HIGGS_CALLTYPE higgs_isInterAppAudioConnected();
 void HIGGS_CALLTYPE higgs_switchToHostApplication();
 higgs::Image HIGGS_CALLTYPE higgs_getIAAHostIcon (int);
#endif

namespace higgs
{

Image HIGGS_API getIconFromApplication (const String&, const int);

AudioProcessor::WrapperType PluginHostType::higgsPlugInClientCurrentWrapperType = AudioProcessor::wrapperType_Undefined;
std::function<bool (AudioProcessor&)> PluginHostType::higgsPlugInIsRunningInAudioSuiteFn = nullptr;
String PluginHostType::hostIdReportedByWrapper;

bool PluginHostType::isInterAppAudioConnected() const
{
   #if HiggsPlugin_Enable_IAA && HiggsPlugin_Build_Standalone && HIGGS_IOS && (! HIGGS_USE_CUSTOM_PLUGIN_STANDALONE_APP)
    if (getPluginLoadedAs() == AudioProcessor::wrapperType_Standalone)
        return higgs_isInterAppAudioConnected();
   #endif

    return false;
}

void PluginHostType::switchToHostApplication() const
{
   #if HiggsPlugin_Enable_IAA && HiggsPlugin_Build_Standalone && HIGGS_IOS && (! HIGGS_USE_CUSTOM_PLUGIN_STANDALONE_APP)
    if (getPluginLoadedAs() == AudioProcessor::wrapperType_Standalone)
        higgs_switchToHostApplication();
   #endif
}

bool PluginHostType::isInAAXAudioSuite ([[maybe_unused]] AudioProcessor& processor)
{
   #if HiggsPlugin_Build_AAX
    if (PluginHostType::getPluginLoadedAs() == AudioProcessor::wrapperType_AAX
        && higgsPlugInIsRunningInAudioSuiteFn != nullptr)
    {
        return higgsPlugInIsRunningInAudioSuiteFn (processor);
    }
   #endif

    return false;
}

Image PluginHostType::getHostIcon ([[maybe_unused]] int size) const
{
   #if HiggsPlugin_Enable_IAA && HiggsPlugin_Build_Standalone && HIGGS_IOS && (! HIGGS_USE_CUSTOM_PLUGIN_STANDALONE_APP)
    if (isInterAppAudioConnected())
        return higgs_getIAAHostIcon (size);
   #endif

   #if HIGGS_MAC
    String bundlePath (getHostPath().upToLastOccurrenceOf (".app", true, true));
    return getIconFromApplication (bundlePath, size);
   #endif

    return Image();
}

const char* PluginHostType::getHostDescription() const noexcept
{
    switch (type)
    {
        case AbletonLive6:             return "Ableton Live 6";
        case AbletonLive7:             return "Ableton Live 7";
        case AbletonLive8:             return "Ableton Live 8";
        case AbletonLive9:             return "Ableton Live 9";
        case AbletonLive10:            return "Ableton Live 10";
        case AbletonLive11:            return "Ableton Live 11";
        case AbletonLiveGeneric:       return "Ableton Live";
        case AdobeAudition:            return "Adobe Audition";
        case AdobePremierePro:         return "Adobe Premiere";
        case AppleGarageBand:          return "Apple GarageBand";
        case AppleInfoHelper:          return "com.apple.audio.InfoHelper";
        case AppleLogic:               return "Apple Logic";
        case AppleMainStage:           return "Apple MainStage";
        case Ardour:                   return "Ardour";
        case AULab:                    return "AU Lab";
        case AUVal:                    return "auval";
        case AvidProTools:             return "ProTools";
        case BitwigStudio:             return "Bitwig Studio";
        case CakewalkSonar8:           return "Cakewalk Sonar 8";
        case CakewalkSonarGeneric:     return "Cakewalk Sonar";
        case CakewalkByBandlab:        return "Cakewalk by Bandlab";
        case DaVinciResolve:           return "DaVinci Resolve";
        case DigitalPerformer:         return "DigitalPerformer";
        case FenderStudioPro:          return "Fender Studio Pro";
        case FinalCut:                 return "Final Cut";
        case FruityLoops:              return "FL Studio";
        case HIGGSPluginHost:           return "HIGGS AudioPluginHost";
        case MagixSamplitude:          return "Magix Samplitude";
        case MagixSequoia:             return "Magix Sequoia";
        case pluginval:                return "pluginval";
        case MergingPyramix:           return "Pyramix";
        case MuseReceptorGeneric:      return "Muse Receptor";
        case Maschine:                 return "NI Maschine";
        case Reaper:                   return "Reaper";
        case Reason:                   return "Reason";
        case Renoise:                  return "Renoise";
        case SADiE:                    return "SADiE";
        case SteinbergCubase4:         return "Steinberg Cubase 4";
        case SteinbergCubase5:         return "Steinberg Cubase 5";
        case SteinbergCubase5Bridged:  return "Steinberg Cubase 5 Bridged";
        case SteinbergCubase6:         return "Steinberg Cubase 6";
        case SteinbergCubase7:         return "Steinberg Cubase 7";
        case SteinbergCubase8:         return "Steinberg Cubase 8";
        case SteinbergCubase8_5:       return "Steinberg Cubase 8.5";
        case SteinbergCubase9:         return "Steinberg Cubase 9";
        case SteinbergCubase9_5:       return "Steinberg Cubase 9.5";
        case SteinbergCubase10:        return "Steinberg Cubase 10";
        case SteinbergCubase10_5:      return "Steinberg Cubase 10.5";
        case SteinbergCubaseGeneric:   return "Steinberg Cubase";
        case SteinbergNuendo3:         return "Steinberg Nuendo 3";
        case SteinbergNuendo4:         return "Steinberg Nuendo 4";
        case SteinbergNuendo5:         return "Steinberg Nuendo 5";
        case SteinbergNuendoGeneric:   return "Steinberg Nuendo";
        case SteinbergWavelab5:        return "Steinberg Wavelab 5";
        case SteinbergWavelab6:        return "Steinberg Wavelab 6";
        case SteinbergWavelab7:        return "Steinberg Wavelab 7";
        case SteinbergWavelab8:        return "Steinberg Wavelab 8";
        case SteinbergWavelabGeneric:  return "Steinberg Wavelab";
        case SteinbergTestHost:        return "Steinberg TestHost";
        case StudioOne:                return "Studio One";
        case Tracktion3:               return "Tracktion 3";
        case TracktionGeneric:         return "Tracktion";
        case TracktionWaveform:        return "Tracktion Waveform";
        case VBVSTScanner:             return "VBVSTScanner";
        case ViennaEnsemblePro:        return "Vienna Ensemble Pro";
        case WaveBurner:               return "WaveBurner";
        case UnknownHost:
        default:                       break;
    }

    return "Unknown";
}

PluginHostType::HostType PluginHostType::getHostType()
{
    auto hostPath = getHostPath();
    auto hostFilename = File (hostPath).getFileName();

   #if HIGGS_MAC
    if (hostPath.containsIgnoreCase       ("Final Cut Pro.app"))        return FinalCut;
    if (hostPath.containsIgnoreCase       ("Final Cut Pro Trial.app"))  return FinalCut;
    if (hostPath.containsIgnoreCase       ("Live 6"))                   return AbletonLive6;
    if (hostPath.containsIgnoreCase       ("Live 7"))                   return AbletonLive7;
    if (hostPath.containsIgnoreCase       ("Live 8"))                   return AbletonLive8;
    if (hostPath.containsIgnoreCase       ("Live 9"))                   return AbletonLive9;
    if (hostPath.containsIgnoreCase       ("Live 10"))                  return AbletonLive10;
    if (hostPath.containsIgnoreCase       ("Live 11"))                  return AbletonLive11;
    if (hostFilename.containsIgnoreCase   ("Live"))                     return AbletonLiveGeneric;
    if (hostFilename.containsIgnoreCase   ("Audition"))                 return AdobeAudition;
    if (hostFilename.containsIgnoreCase   ("Adobe Premiere"))           return AdobePremierePro;
    if (hostFilename.containsIgnoreCase   ("GarageBand"))               return AppleGarageBand;
    if (hostFilename.containsIgnoreCase   ("Logic"))                    return AppleLogic;
    if (hostFilename.containsIgnoreCase   ("MainStage"))                return AppleMainStage;
    if (hostFilename.containsIgnoreCase   ("AU Lab"))                   return AULab;
    if (hostFilename.containsIgnoreCase   ("Pro Tools"))                return AvidProTools;
    if (hostFilename.containsIgnoreCase   ("Nuendo 3"))                 return SteinbergNuendo3;
    if (hostFilename.containsIgnoreCase   ("Nuendo 4"))                 return SteinbergNuendo4;
    if (hostFilename.containsIgnoreCase   ("Nuendo 5"))                 return SteinbergNuendo5;
    if (hostFilename.containsIgnoreCase   ("Nuendo"))                   return SteinbergNuendoGeneric;
    if (hostFilename.containsIgnoreCase   ("Cubase 4"))                 return SteinbergCubase4;
    if (hostFilename.containsIgnoreCase   ("Cubase 5"))                 return SteinbergCubase5;
    if (hostFilename.containsIgnoreCase   ("Cubase 6"))                 return SteinbergCubase6;
    if (hostFilename.containsIgnoreCase   ("Cubase 7"))                 return SteinbergCubase7;
    if (hostPath.containsIgnoreCase       ("Cubase 8.app"))             return SteinbergCubase8;
    if (hostPath.containsIgnoreCase       ("Cubase 8.5.app"))           return SteinbergCubase8_5;
    if (hostPath.containsIgnoreCase       ("Cubase 9.app"))             return SteinbergCubase9;
    if (hostPath.containsIgnoreCase       ("Cubase 9.5.app"))           return SteinbergCubase9_5;
    if (hostPath.containsIgnoreCase       ("Cubase 10.app"))            return SteinbergCubase10;
    if (hostPath.containsIgnoreCase       ("Cubase 10.5.app"))          return SteinbergCubase10_5;
    if (hostFilename.containsIgnoreCase   ("Cubase"))                   return SteinbergCubaseGeneric;
    if (hostPath.containsIgnoreCase       ("Wavelab 7"))                return SteinbergWavelab7;
    if (hostPath.containsIgnoreCase       ("Wavelab 8"))                return SteinbergWavelab8;
    if (hostFilename.containsIgnoreCase   ("Wavelab"))                  return SteinbergWavelabGeneric;
    if (hostFilename.containsIgnoreCase   ("WaveBurner"))               return WaveBurner;
    if (hostPath.containsIgnoreCase       ("Digital Performer"))        return DigitalPerformer;
    if (hostFilename.containsIgnoreCase   ("reaper"))                   return Reaper;
    if (hostFilename.containsIgnoreCase   ("Reason"))                   return Reason;
    if (hostPath.containsIgnoreCase       ("Studio One"))               return StudioOne;
    if (hostFilename.containsIgnoreCase   ("Studio Pro"))               return FenderStudioPro;
    if (hostFilename.startsWithIgnoreCase ("Waveform"))                 return TracktionWaveform;
    if (hostPath.containsIgnoreCase       ("Tracktion 3"))              return Tracktion3;
    if (hostFilename.containsIgnoreCase   ("Tracktion"))                return TracktionGeneric;
    if (hostFilename.containsIgnoreCase   ("Renoise"))                  return Renoise;
    if (hostFilename.containsIgnoreCase   ("Resolve"))                  return DaVinciResolve;
    if (hostFilename.startsWith           ("Bitwig"))                   return BitwigStudio;
    if (hostFilename.containsIgnoreCase   ("OsxFL"))                    return FruityLoops;
    if (hostFilename.containsIgnoreCase   ("pluginval"))                return pluginval;
    if (hostFilename.containsIgnoreCase   ("AudioPluginHost"))          return HIGGSPluginHost;
    if (hostFilename.containsIgnoreCase   ("Maschine"))                 return Maschine;
    if (hostFilename.containsIgnoreCase   ("Vienna Ensemble Pro"))      return ViennaEnsemblePro;
    if (hostFilename.containsIgnoreCase   ("auvaltool"))                return AUVal;
    if (hostFilename.containsIgnoreCase   ("com.apple.audio.infohelper")) return AppleInfoHelper;

    if (hostIdReportedByWrapper == "com.apple.logic.pro")               return AppleLogic;
    if (hostIdReportedByWrapper == "com.apple.garageband")              return AppleGarageBand;
    if (hostIdReportedByWrapper == "com.apple.mainstage")               return AppleMainStage;

    const auto procName = nsStringToHiggs ([[NSRunningApplication currentApplication] localizedName]);

    const auto matchesInOrder = [&] (const StringArray& strings)
    {
        return procName.matchesWildcard ("AUHostingService*(" + strings.joinIntoString ("*") + ")", false);
    };

    // Depending on localization settings, spaces are not always plain ascii spaces
    if (matchesInOrder ({ "Logic", "Pro" }))                            return AppleLogic;
    if (matchesInOrder ({ "GarageBand" }))                              return AppleGarageBand;
    if (matchesInOrder ({ "MainStage" }))                               return AppleMainStage;
    if (matchesInOrder ({ "Final", "Cut", "Pro" }))                     return FinalCut;

   #elif HIGGS_WINDOWS
    if (hostFilename.containsIgnoreCase   ("Live 6"))                return AbletonLive6;
    if (hostFilename.containsIgnoreCase   ("Live 7"))                return AbletonLive7;
    if (hostFilename.containsIgnoreCase   ("Live 8"))                return AbletonLive8;
    if (hostFilename.containsIgnoreCase   ("Live 9"))                return AbletonLive9;
    if (hostFilename.containsIgnoreCase   ("Live 10"))               return AbletonLive10;
    if (hostFilename.containsIgnoreCase   ("Live 11"))               return AbletonLive11;
    if (hostFilename.containsIgnoreCase   ("Live "))                 return AbletonLiveGeneric;
    if (hostFilename.containsIgnoreCase   ("Audition"))              return AdobeAudition;
    if (hostFilename.containsIgnoreCase   ("Adobe Premiere"))        return AdobePremierePro;
    if (hostFilename.containsIgnoreCase   ("ProTools"))              return AvidProTools;
    if (hostPath.containsIgnoreCase       ("SONAR 8"))               return CakewalkSonar8;
    if (hostFilename.containsIgnoreCase   ("SONAR"))                 return CakewalkSonarGeneric;
    if (hostFilename.containsIgnoreCase   ("Cakewalk.exe"))          return CakewalkByBandlab;
    if (hostFilename.containsIgnoreCase   ("GarageBand"))            return AppleGarageBand;
    if (hostFilename.containsIgnoreCase   ("Logic"))                 return AppleLogic;
    if (hostFilename.containsIgnoreCase   ("MainStage"))             return AppleMainStage;
    if (hostFilename.startsWithIgnoreCase ("Waveform"))              return TracktionWaveform;
    if (hostPath.containsIgnoreCase       ("Tracktion 3"))           return Tracktion3;
    if (hostFilename.containsIgnoreCase   ("Tracktion"))             return TracktionGeneric;
    if (hostFilename.containsIgnoreCase   ("reaper"))                return Reaper;
    if (hostFilename.containsIgnoreCase   ("Cubase4"))               return SteinbergCubase4;
    if (hostFilename.containsIgnoreCase   ("Cubase5"))               return SteinbergCubase5;
    if (hostFilename.containsIgnoreCase   ("Cubase6"))               return SteinbergCubase6;
    if (hostFilename.containsIgnoreCase   ("Cubase7"))               return SteinbergCubase7;
    if (hostFilename.containsIgnoreCase   ("Cubase8.exe"))           return SteinbergCubase8;
    if (hostFilename.containsIgnoreCase   ("Cubase8.5.exe"))         return SteinbergCubase8_5;
    // Later version of Cubase scan plug-ins with a separate executable "vst2xscanner"
    if (hostFilename.containsIgnoreCase   ("Cubase9.5.exe")
        || hostPath.containsIgnoreCase    ("Cubase 9.5"))            return SteinbergCubase9_5;
    if (hostFilename.containsIgnoreCase   ("Cubase9.exe")
        || hostPath.containsIgnoreCase    ("Cubase 9"))              return SteinbergCubase9;
    if (hostFilename.containsIgnoreCase   ("Cubase10.5.exe")
        || hostPath.containsIgnoreCase    ("Cubase 10.5"))           return SteinbergCubase10_5;
    if (hostFilename.containsIgnoreCase   ("Cubase10.exe")
        || hostPath.containsIgnoreCase    ("Cubase 10"))             return SteinbergCubase10;
    if (hostFilename.containsIgnoreCase   ("Cubase"))                return SteinbergCubaseGeneric;
    if (hostFilename.containsIgnoreCase   ("VSTBridgeApp"))          return SteinbergCubase5Bridged;
    if (hostPath.containsIgnoreCase       ("Wavelab 5"))             return SteinbergWavelab5;
    if (hostPath.containsIgnoreCase       ("Wavelab 6"))             return SteinbergWavelab6;
    if (hostPath.containsIgnoreCase       ("Wavelab 7"))             return SteinbergWavelab7;
    if (hostPath.containsIgnoreCase       ("Wavelab 8"))             return SteinbergWavelab8;
    if (hostPath.containsIgnoreCase       ("Nuendo"))                return SteinbergNuendoGeneric;
    if (hostFilename.containsIgnoreCase   ("Wavelab"))               return SteinbergWavelabGeneric;
    if (hostFilename.containsIgnoreCase   ("TestHost"))              return SteinbergTestHost;
    if (hostFilename.containsIgnoreCase   ("rm-host"))               return MuseReceptorGeneric;
    if (hostFilename.containsIgnoreCase   ("Maschine"))              return Maschine;
    if (hostFilename.startsWith           ("FL"))                    return FruityLoops;
    if (hostFilename.contains             ("ilbridge."))             return FruityLoops;
    if (hostPath.containsIgnoreCase       ("Studio One"))            return StudioOne;
    if (hostFilename.containsIgnoreCase   ("Studio Pro"))            return FenderStudioPro;
    if (hostPath.containsIgnoreCase       ("Digital Performer"))     return DigitalPerformer;
    if (hostFilename.containsIgnoreCase   ("VST_Scanner"))           return VBVSTScanner;
    if (hostPath.containsIgnoreCase       ("Merging Technologies"))  return MergingPyramix;
    if (hostFilename.startsWithIgnoreCase ("Sam"))                   return MagixSamplitude;
    if (hostFilename.startsWithIgnoreCase ("Sequoia"))               return MagixSequoia;
    if (hostFilename.containsIgnoreCase   ("Reason"))                return Reason;
    if (hostFilename.containsIgnoreCase   ("Renoise"))               return Renoise;
    if (hostFilename.containsIgnoreCase   ("Resolve"))               return DaVinciResolve;
    if (hostPath.containsIgnoreCase       ("Bitwig Studio"))         return BitwigStudio;
    if (hostFilename.containsIgnoreCase   ("Sadie"))                 return SADiE;
    if (hostFilename.containsIgnoreCase   ("pluginval"))             return pluginval;
    if (hostFilename.containsIgnoreCase   ("AudioPluginHost"))       return HIGGSPluginHost;
    if (hostFilename.containsIgnoreCase   ("Vienna Ensemble Pro"))   return ViennaEnsemblePro;

   #elif HIGGS_LINUX || HIGGS_BSD
    if (hostFilename.containsIgnoreCase   ("Ardour"))            return Ardour;
    if (hostFilename.startsWithIgnoreCase ("Waveform"))          return TracktionWaveform;
    if (hostFilename.containsIgnoreCase   ("Tracktion"))         return TracktionGeneric;
    if (hostFilename.startsWith           ("Bitwig"))            return BitwigStudio;
    if (hostFilename.containsIgnoreCase   ("pluginval"))         return pluginval;
    if (hostFilename.containsIgnoreCase   ("AudioPluginHost"))   return HIGGSPluginHost;
    if (hostFilename.containsIgnoreCase   ("reaper"))            return Reaper;

    if (   hostFilename.containsIgnoreCase ("Renoise")
        || File { hostPath }.getParentDirectory()
                            .getParentDirectory()
                            .getChildFile ("renoise")
                            .existsAsFile())
    {
        return Renoise;
    }

   #elif HIGGS_IOS
   #elif HIGGS_ANDROID
   #else
    #error
   #endif
    return UnknownHost;
}

} // namespace higgs
