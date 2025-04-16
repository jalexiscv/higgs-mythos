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

struct ContentComponent;
struct ProjectTemplatesAndExamples;

//==============================================================================
class StartPageComponent final : public Component
{
public:
    StartPageComponent (std::function<void (std::unique_ptr<Project>&&)>&& newProjectCb,
                        std::function<void (const File&)>&& exampleCb);

    void paint (Graphics& g) override;
    void resized() override;

private:
    //==============================================================================
    std::unique_ptr<ContentComponent> content;
    std::unique_ptr<ProjectTemplatesAndExamples> tabs;

    TextButton openExistingButton { "Open Existing Project..." };

    //==============================================================================
    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StartPageComponent)
};
