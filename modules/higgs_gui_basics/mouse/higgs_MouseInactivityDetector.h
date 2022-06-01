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
    This object watches for mouse-events happening within a component, and if
    the mouse remains still for long enough, triggers an event to indicate that
    it has become inactive.

    You'd use this for situations where e.g. you want to hide the mouse-cursor
    when the user's not actively using the mouse.

    After creating an instance of this, use addListener to get callbacks when
    the activity status changes.

    @tags{GUI}
*/
class HIGGS_API  MouseInactivityDetector  : private Timer,
                                           private MouseListener
{
public:
    /** Creates an inactivity watcher, attached to the given component.
        The target component must not be deleted while this - it will be monitored
        for any mouse events in it or its child components.
    */
    MouseInactivityDetector (Component& target);

    /** Destructor. */
    ~MouseInactivityDetector() override;

    /** Sets the time for which the mouse must be still before the callback
        is triggered.
    */
    void setDelay (int newDelayMilliseconds) noexcept;

    /** Sets the number of pixels by which the cursor is allowed to drift before it is
        considered to be actively moved.
    */
    void setMouseMoveTolerance (int pixelsNeededToTrigger) noexcept;

    //==============================================================================
    /** Classes should implement this to receive callbacks from a MouseInactivityDetector
        when the mouse becomes active or inactive.
    */
    class Listener
    {
    public:
        virtual ~Listener() = default;

        /** Called when the mouse is moved or clicked for the first time
            after a period of inactivity. */
        virtual void mouseBecameActive() = 0;

        /** Called when the mouse hasn't been moved for the timeout period. */
        virtual void mouseBecameInactive() = 0;
    };

    /** Registers a listener. */
    void addListener (Listener* listener);

    /** Removes a previously-registered listener. */
    void removeListener (Listener* listener);

private:
    //==============================================================================
    Component& targetComp;
    ListenerList<Listener> listenerList;
    Point<int> lastMousePos;
    int delayMs = 1500, toleranceDistance = 15;
    bool isActive = true;

    void timerCallback() override;
    void wakeUp (const MouseEvent&, bool alwaysWake);
    void setActive (bool);

    void mouseMove  (const MouseEvent& e) override   { wakeUp (e, false); }
    void mouseEnter (const MouseEvent& e) override   { wakeUp (e, false); }
    void mouseExit  (const MouseEvent& e) override   { wakeUp (e, false); }
    void mouseDown  (const MouseEvent& e) override   { wakeUp (e, true); }
    void mouseDrag  (const MouseEvent& e) override   { wakeUp (e, true); }
    void mouseUp    (const MouseEvent& e) override   { wakeUp (e, true); }
    void mouseWheelMove (const MouseEvent& e, const MouseWheelDetails&) override  { wakeUp (e, true); }

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MouseInactivityDetector)
};

} // namespace higgs
