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

#include "../../Utility/UI/PropertyComponents/mythos_LabelPropertyComponent.h"

//==============================================================================
class GlobalPathsWindowComponent final : public Component,
                                         private Timer,
                                         private Value::Listener,
                                         private ChangeListener
{
public:
    GlobalPathsWindowComponent()
    {
        addChildComponent (rescanHIGGSPathButton);
        rescanHIGGSPathButton.onClick = [this]
        {
            MythosStudioApplication::getApp().rescanHIGGSPathModules();
            lastHIGGSModulePath = getAppSettings().getStoredPath (Ids::defaultHiggsModulePath, TargetOS::getThisOS()).get();
        };

        addChildComponent (rescanUserPathButton);
        rescanUserPathButton.onClick = [this]
        {
            MythosStudioApplication::getApp().rescanUserPathModules();
            lastUserModulePath = getAppSettings().getStoredPath (Ids::defaultUserModulePath, TargetOS::getThisOS()).get();
        };

        addChildComponent (warnAboutHIGGSPathButton);
        warnAboutHIGGSPathButton.setToggleState (MythosStudioApplication::getApp().shouldPromptUserAboutIncorrectHIGGSPath(),
                                                dontSendNotification);
        warnAboutHIGGSPathButton.onClick = [this]
        {
            MythosStudioApplication::getApp().setShouldPromptUserAboutIncorrectHIGGSPath (warnAboutHIGGSPathButton.getToggleState());
        };

        getGlobalProperties().addChangeListener (this);

        addAndMakeVisible (resetToDefaultsButton);
        resetToDefaultsButton.onClick = [this] { resetCurrentOSPathsToDefaults(); };

        addAndMakeVisible (propertyViewport);
        propertyViewport.setViewedComponent (&propertyGroup, false);

        auto os = TargetOS::getThisOS();

        if      (os == TargetOS::osx)     selectedOSValue = "osx";
        else if (os == TargetOS::windows) selectedOSValue = "windows";
        else if (os == TargetOS::linux)   selectedOSValue = "linux";

        selectedOSValue.addListener (this);

        buildProps();

        lastHIGGSModulePath = getAppSettings().getStoredPath (Ids::defaultHiggsModulePath, TargetOS::getThisOS()).get();
        lastUserModulePath = getAppSettings().getStoredPath (Ids::defaultUserModulePath, TargetOS::getThisOS()).get();
    }

    ~GlobalPathsWindowComponent() override
    {
        getGlobalProperties().removeChangeListener (this);

        auto higgsValue = getAppSettings().getStoredPath (Ids::defaultHiggsModulePath, TargetOS::getThisOS());
        auto userValue = getAppSettings().getStoredPath (Ids::defaultUserModulePath, TargetOS::getThisOS());

        if (higgsValue.get() != lastHIGGSModulePath)  MythosStudioApplication::getApp().rescanHIGGSPathModules();
        if (userValue.get() != lastUserModulePath)  MythosStudioApplication::getApp().rescanUserPathModules();
    }

    void paint (Graphics& g) override
    {
        g.fillAll (findColour (backgroundColourId));
    }

    void paintOverChildren (Graphics& g) override
    {
        g.setColour (findColour (defaultHighlightColourId).withAlpha (flashAlpha));
        g.fillRect (boundsToHighlight);
    }

    void resized() override
    {
        auto b = getLocalBounds().reduced (10);

        auto bottomBounds = b.removeFromBottom (80);
        auto buttonBounds = bottomBounds.removeFromBottom (50);

        rescanHIGGSPathButton.setBounds (buttonBounds.removeFromLeft (150).reduced (5, 10));
        rescanUserPathButton.setBounds (buttonBounds.removeFromLeft (150).reduced (5, 10));

        resetToDefaultsButton.setBounds (buttonBounds.removeFromRight (150).reduced (5, 10));
        warnAboutHIGGSPathButton.setBounds (bottomBounds.reduced (0, 5));
        warnAboutHIGGSPathButton.changeWidthToFitText();

        propertyGroup.updateSize (0, 0, getWidth() - 20 - propertyViewport.getScrollBarThickness());
        propertyViewport.setBounds (b);
    }

    void highlightHIGGSPath()
    {
        if (isTimerRunning() || ! isSelectedOSThisOS())
            return;

        const auto findHiggsPathPropertyComponent = [this]() -> PropertyComponent*
        {
            for (const auto& prop : propertyGroup.getProperties())
                if (prop->getName() == "Path to HIGGS")
                    return prop.get();

            return nullptr;
        };

        if (auto* propComponent = findHiggsPathPropertyComponent())
        {
            boundsToHighlight = getLocalArea (nullptr, propComponent->getScreenBounds());
            flashAlpha = 0.0f;
            hasFlashed = false;

            startTimer (25);
        }
    }

private:
    //==============================================================================
    void timerCallback() override
    {
        flashAlpha += (hasFlashed ? -0.05f : 0.05f);

        if (flashAlpha > 0.75f)
        {
            hasFlashed = true;
        }
        else if (flashAlpha < 0.0f)
        {
            flashAlpha = 0.0f;
            boundsToHighlight = {};

            stopTimer();
        }

        repaint();
    }

    void valueChanged (Value&) override
    {
        buildProps();
        resized();
    }

    void changeListenerCallback (ChangeBroadcaster*) override
    {
        warnAboutHIGGSPathButton.setToggleState (MythosStudioApplication::getApp().shouldPromptUserAboutIncorrectHIGGSPath(),
                                                dontSendNotification);
    }

    //==============================================================================
    bool isSelectedOSThisOS()    { return TargetOS::getThisOS() == getSelectedOS(); }

    TargetOS::OS getSelectedOS() const
    {
        auto val = selectedOSValue.getValue();

        if      (val == "osx")      return TargetOS::osx;
        else if (val == "windows")  return TargetOS::windows;
        else if (val == "linux")    return TargetOS::linux;

        jassertfalse;
        return TargetOS::unknown;
    }

    //==============================================================================
    void buildProps()
    {
        updateValues();

        PropertyListBuilder builder;
        auto isThisOS = isSelectedOSThisOS();

        builder.add (new ChoicePropertyComponent (selectedOSValue, "OS", { "OSX", "Windows", "Linux" }, { "osx", "windows", "linux" }),
                     "Use this dropdown to set the global paths for different OSes. "
                     "\nN.B. These paths are stored locally and will only be used when "
                     "saving a project on this machine. Other machines will have their own "
                     "locally stored paths.");

        builder.add (new LabelPropertyComponent ("HIGGS"), {});

        builder.add (new FilePathPropertyComponent (higgsPathValue, "Path to HIGGS", true, isThisOS),
                     "This should be the path to the top-level directory of your HIGGS folder. "
                     "This path will be used when searching for the HIGGS examples and DemoRunner application.");

        builder.add (new FilePathPropertyComponent (higgsModulePathValue, "HIGGS Modules", true, isThisOS),
                     String ("This should be the path to the folder containing the HIGGS modules that you wish to use, typically the \"modules\" directory of your HIGGS folder.")
                     + (isThisOS ? " Use the button below to re-scan a new path." : ""));
        builder.add (new FilePathPropertyComponent (userModulePathValue, "User Modules", true, isThisOS),
                     String ("A path to a folder containing any custom modules that you wish to use.")
                     + (isThisOS ? " Use the button below to re-scan new paths." : ""));

        builder.add (new LabelPropertyComponent ("SDKs"), {});

        builder.add (new FilePathPropertyComponent (vstPathValue,  "VST (Legacy) SDK", true, isThisOS),
                     "If you are building a legacy VST plug-in then this path should point to a VST2 SDK. "
                     "The VST2 SDK can be obtained from the vstsdk3610_11_06_2018_build_37 (or older) VST3 SDK or HIGGS version 5.3.2. "
                     "You also need a VST2 license from Steinberg to distribute VST2 plug-ins.");
        builder.add (new FilePathPropertyComponent (araPathValue, "ARA SDK", true, isThisOS),
                     "If you are building ARA enabled plug-ins, this should be the path to the ARA SDK folder.");

        if (getSelectedOS() != TargetOS::linux)
        {
            builder.add (new FilePathPropertyComponent (aaxPathValue, "AAX SDK", true, isThisOS),
                         "If you need to use a custom version of the AAX SDK, this should be the path to the AAX SDK folder. "
                         "HIGGS bundles a copy of the AAX SDK, so you normally shouldn't need to set this.");
        }

        builder.add (new FilePathPropertyComponent (androidSDKPathValue, "Android SDK", true, isThisOS),
                     "This path will be used when writing the local.properties file of an Android project and should point to the Android SDK folder.");

        if (isThisOS)
        {
            builder.add (new LabelPropertyComponent ("Other"), {});

           #if HIGGS_MAC
            String exeLabel ("app");
           #elif HIGGS_WINDOWS
            String exeLabel ("executable");
           #else
            String exeLabel ("startup script");
           #endif

            builder.add (new FilePathPropertyComponent (androidStudioExePathValue, "Android Studio " + exeLabel, false, isThisOS),
                         "This path will be used for the \"Save Project and Open in IDE...\" option of the Android Studio exporter.");
        }

        rescanHIGGSPathButton.setVisible (isThisOS);
        rescanUserPathButton.setVisible (isThisOS);
        warnAboutHIGGSPathButton.setVisible (isThisOS);

        propertyGroup.setProperties (builder);
    }

    void updateValues()
    {
        auto& settings = getAppSettings();
        auto os = getSelectedOS();

        higgsPathValue             = settings.getStoredPath (Ids::higgsPath, os);
        higgsModulePathValue       = settings.getStoredPath (Ids::defaultHiggsModulePath, os);
        userModulePathValue       = settings.getStoredPath (Ids::defaultUserModulePath, os);
        vstPathValue              = settings.getStoredPath (Ids::vstLegacyPath, os);
        aaxPathValue              = settings.getStoredPath (Ids::aaxPath, os);
        araPathValue              = settings.getStoredPath (Ids::araPath, os);
        androidSDKPathValue       = settings.getStoredPath (Ids::androidSDKPath, os);
        androidStudioExePathValue = settings.getStoredPath (Ids::androidStudioExePath, os);
    }

    void resetCurrentOSPathsToDefaults()
    {
        higgsPathValue            .resetToDefault();
        higgsModulePathValue      .resetToDefault();
        userModulePathValue      .resetToDefault();
        vstPathValue             .resetToDefault();
        aaxPathValue             .resetToDefault();
        araPathValue             .resetToDefault();
        androidSDKPathValue      .resetToDefault();
        androidStudioExePathValue.resetToDefault();

        repaint();
    }

    //==============================================================================
    Value selectedOSValue;

    ValueTreePropertyWithDefault higgsPathValue, higgsModulePathValue, userModulePathValue,
                                 vstPathValue, aaxPathValue, araPathValue, androidSDKPathValue,
                                 androidStudioExePathValue;

    Viewport propertyViewport;
    PropertyGroupComponent propertyGroup  { "Global Paths", { getIcons().openFolder, Colours::transparentBlack } };

    ToggleButton warnAboutHIGGSPathButton { "Warn about incorrect HIGGS path" };
    TextButton rescanHIGGSPathButton  { "Re-scan HIGGS Modules" },
               rescanUserPathButton  { "Re-scan User Modules" },
               resetToDefaultsButton { "Reset to Defaults" };

    Rectangle<int> boundsToHighlight;
    float flashAlpha = 0.0f;
    bool hasFlashed = false;

    var lastHIGGSModulePath, lastUserModulePath;

    //==============================================================================
    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GlobalPathsWindowComponent)
};
