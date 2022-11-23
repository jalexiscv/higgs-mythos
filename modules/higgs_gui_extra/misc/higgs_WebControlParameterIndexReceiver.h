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

namespace higgs
{

#if HIGGS_WEB_BROWSER || DOXYGEN

/** This is a helper class for implementing AudioProcessorEditor::getControlParameterIndex with GUIs
    using a WebBrowserComponent.

    Create an instance of this class and attach it to the WebBrowserComponent by using
    WebBrowserComponent::Options::withOptionsFrom.

    In your frontend code you can use the ControlParameterIndexUpdater class, that emits
    controlParameterIndexChanged events based on the mouse movement, and control parameter index
    annotations attached to DOM elements.

    @tags{GUI}
*/
class HIGGS_API  WebControlParameterIndexReceiver : public OptionsBuilder<WebBrowserComponent::Options>
{
public:
    /*  Returns the control parameter index last reported by the WebBrowserComponent GUI to be
        active.
    */
    int getControlParameterIndex() const { return controlParameterIndex; }

    //==============================================================================
    WebBrowserComponent::Options buildOptions (const WebBrowserComponent::Options& initialOptions) override
    {
        return initialOptions.withEventListener ("__higgs__controlParameterIndexChanged",
                                                 [this] (auto newIndex)
                                                 {
                                                     controlParameterIndex = (int) newIndex;
                                                 });
    }

private:
    int controlParameterIndex = -1;
};

#endif

}
