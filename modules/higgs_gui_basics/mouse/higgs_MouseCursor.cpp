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

class MouseCursor::SharedCursorHandle
{
public:
    explicit SharedCursorHandle (const MouseCursor::StandardCursorType type)
        : handle (type),
          standardType (type),
          standard (true)
    {
    }

    SharedCursorHandle (const ScaledImage& image, Point<int> hotSpot)
        : info { image, hotSpot },
          handle (info),
          standardType (MouseCursor::NormalCursor),
          standard (false)
    {
        // your hotspot needs to be within the bounds of the image!
        jassert (image.getScaledBounds().toNearestInt().contains (hotSpot));
    }

    static std::shared_ptr<SharedCursorHandle> createStandard (const MouseCursor::StandardCursorType type)
    {
        if (! isPositiveAndBelow (type, MouseCursor::NumStandardCursorTypes))
            return nullptr;

        static SpinLock mutex;
        static std::array<std::weak_ptr<SharedCursorHandle>, MouseCursor::NumStandardCursorTypes> cursors;

        const SpinLock::ScopedLockType sl (mutex);

        auto& weak = cursors[type];

        if (auto strong = weak.lock())
            return strong;

        auto strong = std::make_shared<SharedCursorHandle> (type);
        weak = strong;
        return strong;
    }

    bool isStandardType (MouseCursor::StandardCursorType type) const noexcept
    {
        return type == standardType && standard;
    }

    PlatformSpecificHandle* getHandle() noexcept                { return &handle; }
    MouseCursor::StandardCursorType getType() const noexcept    { return standardType; }

private:
    detail::CustomMouseCursorInfo info;
    PlatformSpecificHandle handle;
    const MouseCursor::StandardCursorType standardType;
    const bool standard;

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SharedCursorHandle)
};

//==============================================================================
MouseCursor::MouseCursor() noexcept = default;

MouseCursor::MouseCursor (const StandardCursorType type)
    : cursorHandle (type != MouseCursor::NormalCursor ? SharedCursorHandle::createStandard (type) : nullptr)
{
}

MouseCursor::MouseCursor (const Image& image, int hotSpotX, int hotSpotY)
    : MouseCursor (ScaledImage (image), { hotSpotX, hotSpotY })
{
}

MouseCursor::MouseCursor (const Image& image, int hotSpotX, int hotSpotY, float scaleFactor)
    : MouseCursor (ScaledImage (image, scaleFactor), { hotSpotX, hotSpotY })
{
}

MouseCursor::MouseCursor (const ScaledImage& image, Point<int> hotSpot)
        : cursorHandle (std::make_shared<SharedCursorHandle> (image, hotSpot))
{
}

MouseCursor::MouseCursor (const MouseCursor&) = default;

MouseCursor::~MouseCursor() = default;

MouseCursor& MouseCursor::operator= (const MouseCursor&) = default;

MouseCursor::MouseCursor (MouseCursor&&) noexcept = default;

MouseCursor& MouseCursor::operator= (MouseCursor&&) noexcept = default;

bool MouseCursor::operator== (const MouseCursor& other) const noexcept
{
    return getHandle() == other.getHandle();
}

bool MouseCursor::operator== (StandardCursorType type) const noexcept
{
    return cursorHandle != nullptr ? cursorHandle->isStandardType (type)
                                   : (type == NormalCursor);
}

bool MouseCursor::operator!= (const MouseCursor& other) const noexcept  { return ! operator== (other); }
bool MouseCursor::operator!= (StandardCursorType type)  const noexcept  { return ! operator== (type); }

void MouseCursor::showWaitCursor()
{
    Desktop::getInstance().getMainMouseSource().showMouseCursor (MouseCursor::WaitCursor);
}

void MouseCursor::hideWaitCursor()
{
    Desktop::getInstance().getMainMouseSource().revealCursor();
}

MouseCursor::PlatformSpecificHandle* MouseCursor::getHandle() const noexcept
{
    return cursorHandle != nullptr ? cursorHandle->getHandle() : nullptr;
}

void MouseCursor::showInWindow (ComponentPeer* peer) const
{
    PlatformSpecificHandle::showInWindow (getHandle(), peer);
}

} // namespace higgs
