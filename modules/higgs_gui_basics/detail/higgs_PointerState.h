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

namespace higgs::detail
{

class PointerState
{
    auto tie() const noexcept
    {
        return std::tie (position, pressure, orientation, rotation, tiltX, tiltY);
    }

public:
    PointerState() = default;

    bool operator== (const PointerState& other) const noexcept   { return tie() == other.tie(); }
    bool operator!= (const PointerState& other) const noexcept   { return tie() != other.tie(); }

    [[nodiscard]] PointerState withPositionOffset (Point<float> x)        const noexcept { return with (&PointerState::position, position + x); }
    [[nodiscard]] PointerState withPosition (Point<float> x)              const noexcept { return with (&PointerState::position, x); }
    [[nodiscard]] PointerState withPressure (float x)                     const noexcept { return with (&PointerState::pressure, x); }
    [[nodiscard]] PointerState withOrientation (float x)                  const noexcept { return with (&PointerState::orientation, x); }
    [[nodiscard]] PointerState withRotation (float x)                     const noexcept { return with (&PointerState::rotation, x); }
    [[nodiscard]] PointerState withTiltX (float x)                        const noexcept { return with (&PointerState::tiltX, x); }
    [[nodiscard]] PointerState withTiltY (float x)                        const noexcept { return with (&PointerState::tiltY, x); }

    Point<float> position;
    float pressure    = MouseInputSource::defaultPressure;
    float orientation = MouseInputSource::defaultOrientation;
    float rotation    = MouseInputSource::defaultRotation;
    float tiltX       = MouseInputSource::defaultTiltX;
    float tiltY       = MouseInputSource::defaultTiltY;

    bool isPressureValid()      const noexcept        { return 0.0f <= pressure && pressure <= 1.0f; }
    bool isOrientationValid()   const noexcept        { return 0.0f <= orientation && orientation <= MathConstants<float>::twoPi; }
    bool isRotationValid()      const noexcept        { return 0.0f <= rotation && rotation <= MathConstants<float>::twoPi; }
    bool isTiltValid (bool isX) const noexcept
    {
        return isX ? (-1.0f <= tiltX && tiltX <= 1.0f)
                   : (-1.0f <= tiltY && tiltY <= 1.0f);
    }

private:
    template <typename Value>
    PointerState with (Value PointerState::* member, Value item) const
    {
        auto copy = *this;
        copy.*member = std::move (item);
        return copy;
    }
};

inline auto makeMouseEvent (MouseInputSource source,
                            const PointerState& ps,
                            ModifierKeys modifiers,
                            Component* eventComponent,
                            Component* originator,
                            Time eventTime,
                            Point<float> mouseDownPos,
                            Time mouseDownTime,
                            int numberOfClicks,
                            bool mouseWasDragged)
{
    return MouseEvent (source,
                       ps.position,
                       modifiers,
                       ps.pressure,
                       ps.orientation,
                       ps.rotation,
                       ps.tiltX,
                       ps.tiltY,
                       eventComponent,
                       originator,
                       eventTime,
                       mouseDownPos,
                       mouseDownTime,
                       numberOfClicks,
                       mouseWasDragged);
}

} // namespace higgs::detail
