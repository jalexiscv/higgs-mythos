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
    A singleton class to manage analytics data.

    Use an Analytics object to manage sending analytics data to one or more
    AnalyticsDestinations.

    @see AnalyticsDestination, ThreadedAnalyticsDestination,
         AnalyticsDestination::AnalyticsEvent

    @tags{Analytics}
*/
class HIGGS_API  Analytics   : public DeletedAtShutdown
{
public:
    //==============================================================================
    /** Adds an AnalyticsDestination to the list of AnalyticsDestinations
        managed by this Analytics object.

        The Analytics class will take ownership of the AnalyticsDestination
        passed to this function.

        @param destination          the AnalyticsDestination to manage
    */
    void addDestination (AnalyticsDestination* destination);

    /** Returns the array of AnalyticsDestinations managed by this class.

        If you have added any subclasses of ThreadedAnalyticsDestination to
        this class then you can remove them from this list to force them to
        flush any pending events.
    */
    OwnedArray<AnalyticsDestination>& getDestinations();

    /** Sets a user ID that will be added to all AnalyticsEvents sent to
        AnalyticsDestinations.

        @param newUserId            the userId to add to AnalyticsEvents
    */
    void setUserId (String newUserId);

    /** Sets some user properties that will be added to all AnalyticsEvents sent
        to AnalyticsDestinations.

        @param properties           the userProperties to add to AnalyticsEvents
    */
    void setUserProperties (StringPairArray properties);

    /** Sends an AnalyticsEvent to all AnalyticsDestinations.

        The AnalyticsEvent will be timestamped, and will have the userId and
        userProperties populated by values previously set by calls to
        setUserId and setUserProperties. The name, parameters and type will be
        populated by the arguments supplied to this function.

        @param eventName            the event name
        @param parameters           the event parameters
        @param eventType            (optional) an integer to indicate the event
                                    type, which will be set to 0 if not supplied.
    */
    void logEvent (const String& eventName, const StringPairArray& parameters, int eventType = 0);

    /** Suspends analytics submissions to AnalyticsDestinations.

        @param shouldBeSuspended    if event submission should be suspended
    */
    void setSuspended (bool shouldBeSuspended);

    /** @cond */
    HIGGS_DECLARE_SINGLETON_INLINE (Analytics, false)
    /** @endcond */

private:
    //==============================================================================
    Analytics() = default;
    ~Analytics() override  { clearSingletonInstance(); }

    String userId;
    StringPairArray userProperties;

    bool isSuspended = false;

    OwnedArray<AnalyticsDestination> destinations;

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (Analytics)
};

} // namespace higgs
