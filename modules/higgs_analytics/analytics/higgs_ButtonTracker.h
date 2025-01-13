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

//==============================================================================
/**
    A class that automatically sends analytics events to the Analytics singleton
    when a button is clicked.

    @see Analytics, AnalyticsDestination::AnalyticsEvent

    @tags{Analytics}
*/
class HIGGS_API  ButtonTracker   : private Button::Listener
{
public:
    //==============================================================================
    /**
        Creating one of these automatically sends analytics events to the Analytics
        singleton when the corresponding button is clicked.

        The name and parameters of the analytics event will be populated from the
        variables supplied here. If clicking changes the button's state then the
        parameters will have a {"ButtonState", "On"/"Off"} entry added.

        @param buttonToTrack               the button to track
        @param triggeredEventName          the name of the generated event
        @param triggeredEventParameters    the parameters to add to the generated
                                           event
        @param triggeredEventType          (optional) an integer to indicate the event
                                           type, which will be set to 0 if not supplied.

        @see Analytics, AnalyticsDestination::AnalyticsEvent
    */
    ButtonTracker (Button& buttonToTrack,
                   const String& triggeredEventName,
                   const StringPairArray& triggeredEventParameters = {},
                   int triggeredEventType = 0);

    /** Destructor. */
    ~ButtonTracker() override;

private:
    /** @internal */
    void buttonClicked (Button*) override;

    Button& button;
    const String eventName;
    const StringPairArray eventParameters;
    const int eventType;

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ButtonTracker)
};

} // namespace higgs
