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

#include <map>
#include "mythos_AppearanceSettings.h"

//==============================================================================
class StoredSettings final : private ValueTree::Listener
{
public:
    StoredSettings();
    ~StoredSettings() override;

    PropertiesFile& getGlobalProperties();
    PropertiesFile& getProjectProperties (const String& projectUID);

    void flush();
    void reload();

    //==============================================================================
    RecentlyOpenedFilesList recentFiles;

    Array<File> getLastProjects();
    void setLastProjects (const Array<File>& files);

    //==============================================================================
    Array<Colour> swatchColours;

    struct ColourSelectorWithSwatches final : public ColourSelector
    {
        ColourSelectorWithSwatches();
        ~ColourSelectorWithSwatches() override;

        int getNumSwatches() const override;
        Colour getSwatchColour (int index) const override;
        void setSwatchColour (int index, const Colour& newColour) override;
    };

    //==============================================================================
    void addProjectDefaultsListener (ValueTree::Listener&);
    void removeProjectDefaultsListener (ValueTree::Listener&);

    void addFallbackPathsListener (ValueTree::Listener&);
    void removeFallbackPathsListener (ValueTree::Listener&);

    ValueTreePropertyWithDefault getStoredPath (const Identifier& key, DependencyPathOS os);
    bool isHIGGSPathIncorrect();

    //==============================================================================
    AppearanceSettings appearance;
    StringArray monospacedFontNames;
    File lastWizardFolder;

private:
    //==============================================================================
    void updateGlobalPreferences();
    void updateRecentFiles();
    void updateLastWizardFolder();
    void updateKeyMappings();

    void loadSwatchColours();
    void saveSwatchColours();

    void updateOldProjectSettingsFiles();
    void checkHIGGSPaths();

    //==============================================================================
    void changed (bool);

    void valueTreePropertyChanged (ValueTree& vt, const Identifier&) override  { changed (vt == projectDefaults); }
    void valueTreeChildAdded (ValueTree& vt, ValueTree&) override              { changed (vt == projectDefaults); }
    void valueTreeChildRemoved (ValueTree& vt, ValueTree&, int) override       { changed (vt == projectDefaults); }
    void valueTreeChildOrderChanged (ValueTree& vt, int, int) override         { changed (vt == projectDefaults); }
    void valueTreeParentChanged (ValueTree& vt) override                       { changed (vt == projectDefaults); }

    //==============================================================================
    OwnedArray<PropertiesFile> propertyFiles;
    ValueTree projectDefaults;
    ValueTree fallbackPaths;

    //==============================================================================
    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StoredSettings)
};

StoredSettings& getAppSettings();
PropertiesFile& getGlobalProperties();
