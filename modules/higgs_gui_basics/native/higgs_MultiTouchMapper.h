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

HIGGS_BEGIN_IGNORE_WARNINGS_GCC_LIKE ("-Wzero-as-null-pointer-constant")

namespace higgs
{

template <typename IDType>
class MultiTouchMapper
{
public:
    MultiTouchMapper() {}

    int getIndexOfTouch (ComponentPeer* peer, IDType touchID)
    {
        jassert (touchID != 0); // need to rethink this if IDs can be 0!
        TouchInfo info {touchID, peer};

        int touchIndex = currentTouches.indexOf (info);

        if (touchIndex < 0)
        {
            auto emptyTouchIndex = currentTouches.indexOf ({});
            touchIndex = (emptyTouchIndex >= 0 ? emptyTouchIndex : currentTouches.size());

            currentTouches.set (touchIndex, info);
        }

        return touchIndex;
    }

    void clear()
    {
        currentTouches.clear();
    }

    void clearTouch (int index)
    {
        currentTouches.set (index, {});
    }

    bool areAnyTouchesActive() const noexcept
    {
        for (auto& t : currentTouches)
            if (t.touchId != 0)
                return true;

        return false;
    }

    void deleteAllTouchesForPeer (ComponentPeer* peer)
    {
        for (auto& t : currentTouches)
            if (t.owner == peer)
                t.touchId = 0;
    }

private:
    //==============================================================================
    struct TouchInfo
    {
        TouchInfo() noexcept  : touchId (0), owner (nullptr) {}
        TouchInfo (IDType idToUse, ComponentPeer* peer) noexcept  : touchId (idToUse), owner (peer) {}

        TouchInfo (const TouchInfo&) = default;
        TouchInfo& operator= (const TouchInfo&) = default;
        TouchInfo (TouchInfo&&) noexcept = default;
        TouchInfo& operator= (TouchInfo&&) noexcept = default;

        IDType touchId;
        ComponentPeer* owner;

        bool operator== (const TouchInfo& o) const noexcept     { return (touchId == o.touchId); }
    };

    //==============================================================================
    Array<TouchInfo> currentTouches;

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MultiTouchMapper)
};

} // namespace higgs

HIGGS_END_IGNORE_WARNINGS_GCC_LIKE
