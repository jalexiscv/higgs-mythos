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
    An interface for handling analytics events collected by an Analytics object.

    For basic analytics logging you can implement this interface and add your
    class to an Analytics object.

    For more advanced logging you may want to subclass
    ThreadedAnalyticsDestination instead, which is more suitable for interacting
    with web servers and other time consuming destinations.

    @see Analytics, ThreadedAnalyticsDestination

    @tags{Analytics}
*/
struct HIGGS_API  AnalyticsDestination
{
    /** Contains information about an event to be logged. */
    struct AnalyticsEvent
    {
        /** The name of the event. */
        String name;

        /** An optional integer representing the type of the event. You can use
            this to indicate if the event was a screenview, session start,
            exception, etc.
        */
        int eventType;

        /**
            The timestamp of the event.

            Timestamps are automatically applied by an Analytics object and are
            derived from Time::getMillisecondCounter(). As such these timestamps
            do not represent absolute times, but relative timings of events for
            each user in each session will be accurate.
        */
        uint32 timestamp;

        /** The parameters of the event. */
        StringPairArray parameters;

        /** The user ID associated with the event. */
        String userID;

        /** Properties associated with the user. */
        StringPairArray userProperties;
    };

    /** Constructor. */
    AnalyticsDestination() = default;

    /** Destructor. */
    virtual ~AnalyticsDestination() = default;

    /**
        When an AnalyticsDestination is added to an Analytics object this method
        is called when an analytics event is triggered from the Analytics
        object.

        Override this method to log the event information somewhere useful.
    */
    virtual void logEvent (const AnalyticsEvent& event) = 0;

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AnalyticsDestination)
};


} // namespace higgs
