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

ButtonTracker::ButtonTracker (Button& buttonToTrack,
                              const String& triggeredEventName,
                              const StringPairArray& triggeredEventParameters,
                              int triggeredEventType)
    : button (buttonToTrack),
      eventName (triggeredEventName),
      eventParameters (triggeredEventParameters),
      eventType (triggeredEventType)
{
    button.addListener (this);
}

ButtonTracker::~ButtonTracker()
{
    button.removeListener (this);
}

void ButtonTracker::buttonClicked (Button* b)
{
    if (b == &button)
    {
        auto params = eventParameters;

        if (button.getClickingTogglesState())
            params.set ("ButtonState", button.getToggleState() ? "On" : "Off");

        Analytics::getInstance()->logEvent (eventName, params, eventType);
    }
}


} // namespace higgs
