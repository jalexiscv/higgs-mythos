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

class MainHostWindow;
class GraphDocumentComponent;


//==============================================================================
class IOConfigurationWindow final : public  AudioProcessorEditor
{
public:
    IOConfigurationWindow (AudioProcessor&);
    ~IOConfigurationWindow() override;

    //==============================================================================
    void paint (Graphics& g) override;
    void resized() override;

private:
    class InputOutputConfig;

    AudioProcessor::BusesLayout currentLayout;
    Label title;
    std::unique_ptr<InputOutputConfig> inConfig, outConfig;

    InputOutputConfig* getConfig (bool isInput) noexcept    { return isInput ? inConfig.get() : outConfig.get(); }
    void update();

    MainHostWindow* getMainWindow() const;
    GraphDocumentComponent* getGraphEditor() const;
    AudioProcessorGraph* getGraph() const;
    AudioProcessorGraph::NodeID getNodeID() const;

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (IOConfigurationWindow)
};
