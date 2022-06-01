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

MouseEvent::MouseEvent (MouseInputSource inputSource,
                        Point<float> pos,
                        ModifierKeys modKeys,
                        float force,
                        float o, float r,
                        float tX, float tY,
                        Component* const eventComp,
                        Component* const originator,
                        Time time,
                        Point<float> downPos,
                        Time downTime,
                        const int numClicks,
                        const bool mouseWasDragged) noexcept
    : position (pos),
      x (roundToInt (pos.x)),
      y (roundToInt (pos.y)),
      mods (modKeys),
      pressure (force),
      orientation (o), rotation (r),
      tiltX (tX), tiltY (tY),
      mouseDownPosition (downPos),
      eventComponent (eventComp),
      originalComponent (originator),
      eventTime (time),
      mouseDownTime (downTime),
      source (inputSource),
      numberOfClicks ((uint8) numClicks),
      wasMovedSinceMouseDown ((uint8) (mouseWasDragged ? 1 : 0))
{
}

//==============================================================================
MouseEvent MouseEvent::getEventRelativeTo (Component* const otherComponent) const noexcept
{
    jassert (otherComponent != nullptr);

    return MouseEvent (source, otherComponent->getLocalPoint (eventComponent, position),
                       mods, pressure, orientation, rotation, tiltX, tiltY,
                       otherComponent, originalComponent, eventTime,
                       otherComponent->getLocalPoint (eventComponent, mouseDownPosition),
                       mouseDownTime, numberOfClicks, wasMovedSinceMouseDown != 0);
}

MouseEvent MouseEvent::withNewPosition (Point<float> newPosition) const noexcept
{
    return MouseEvent (source, newPosition, mods, pressure, orientation, rotation, tiltX, tiltY,
                       eventComponent, originalComponent, eventTime, mouseDownPosition, mouseDownTime,
                       numberOfClicks, wasMovedSinceMouseDown != 0);
}

MouseEvent MouseEvent::withNewPosition (Point<int> newPosition) const noexcept
{
    return MouseEvent (source, newPosition.toFloat(), mods, pressure, orientation, rotation,
                       tiltX, tiltY, eventComponent,  originalComponent, eventTime, mouseDownPosition,
                       mouseDownTime, numberOfClicks, wasMovedSinceMouseDown != 0);
}

//==============================================================================
bool MouseEvent::mouseWasDraggedSinceMouseDown() const noexcept
{
    return wasMovedSinceMouseDown != 0;
}

bool MouseEvent::mouseWasClicked() const noexcept
{
    return ! mouseWasDraggedSinceMouseDown();
}

int MouseEvent::getLengthOfMousePress() const noexcept
{
    if (mouseDownTime.toMilliseconds() > 0)
        return jmax (0, (int) (eventTime - mouseDownTime).inMilliseconds());

    return 0;
}

//==============================================================================
Point<int> MouseEvent::getPosition() const noexcept             { return Point<int> (x, y); }
Point<int> MouseEvent::getScreenPosition() const                { return eventComponent->localPointToGlobal (getPosition()); }

Point<int> MouseEvent::getMouseDownPosition() const noexcept    { return mouseDownPosition.roundToInt(); }
Point<int> MouseEvent::getMouseDownScreenPosition() const       { return eventComponent->localPointToGlobal (mouseDownPosition).roundToInt(); }

Point<int> MouseEvent::getOffsetFromDragStart() const noexcept  { return (position - mouseDownPosition).roundToInt(); }
int MouseEvent::getDistanceFromDragStart() const noexcept       { return roundToInt (mouseDownPosition.getDistanceFrom (position)); }

int MouseEvent::getMouseDownX() const noexcept                  { return roundToInt (mouseDownPosition.x); }
int MouseEvent::getMouseDownY() const noexcept                  { return roundToInt (mouseDownPosition.y); }

int MouseEvent::getDistanceFromDragStartX() const noexcept      { return getOffsetFromDragStart().x; }
int MouseEvent::getDistanceFromDragStartY() const noexcept      { return getOffsetFromDragStart().y; }

int MouseEvent::getScreenX() const                              { return getScreenPosition().x; }
int MouseEvent::getScreenY() const                              { return getScreenPosition().y; }

int MouseEvent::getMouseDownScreenX() const                     { return getMouseDownScreenPosition().x; }
int MouseEvent::getMouseDownScreenY() const                     { return getMouseDownScreenPosition().y; }

bool MouseEvent::isPressureValid() const noexcept               { return pressure > 0.0f && pressure < 1.0f; }
bool MouseEvent::isOrientationValid() const noexcept            { return orientation >= 0.0f && orientation <= MathConstants<float>::twoPi; }
bool MouseEvent::isRotationValid() const noexcept               { return rotation >= 0 && rotation <= MathConstants<float>::twoPi; }
bool MouseEvent::isTiltValid (bool isX) const noexcept          { return isX ? (tiltX >= -1.0f && tiltX <= 1.0f) : (tiltY >= -1.0f && tiltY <= 1.0f); }

//==============================================================================
static int doubleClickTimeOutMs = 400;

int MouseEvent::getDoubleClickTimeout() noexcept                        { return doubleClickTimeOutMs; }
void MouseEvent::setDoubleClickTimeout (const int newTime) noexcept     { doubleClickTimeOutMs = newTime; }

} // namespace higgs
