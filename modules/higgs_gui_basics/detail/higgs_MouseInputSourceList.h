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

class MouseInputSourceList  : public Timer
{
public:
    MouseInputSourceList()
    {
      #if HIGGS_ANDROID || HIGGS_IOS
        auto mainMouseInputType = MouseInputSource::InputSourceType::touch;
      #else
        auto mainMouseInputType = MouseInputSource::InputSourceType::mouse;
      #endif

        addSource (0, mainMouseInputType);
    }

    MouseInputSource* addSource (int index, MouseInputSource::InputSourceType type)
    {
        auto* s = new MouseInputSourceImpl (index, type);
        sources.add (s);
        sourceArray.add (MouseInputSource (s));

        return &sourceArray.getReference (sourceArray.size() - 1);
    }

    MouseInputSource* getMouseSource (int index) noexcept
    {
        return isPositiveAndBelow (index, sourceArray.size()) ? &sourceArray.getReference (index)
                                                              : nullptr;
    }

    MouseInputSource* getOrCreateMouseInputSource (MouseInputSource::InputSourceType type, int touchIndex = 0)
    {
        if (type == MouseInputSource::InputSourceType::mouse
            || type == MouseInputSource::InputSourceType::pen)
        {
            for (auto& m : sourceArray)
                if (type == m.getType())
                    return &m;

            addSource (0, type);
        }
        else if (type == MouseInputSource::InputSourceType::touch)
        {
            jassert (0 <= touchIndex && touchIndex < 100); // sanity-check on number of fingers

            for (auto& m : sourceArray)
                if (type == m.getType() && touchIndex == m.getIndex())
                    return &m;

            if (canUseTouch())
                return addSource (touchIndex, type);
        }

        return nullptr;
    }

    int getNumDraggingMouseSources() const noexcept
    {
        int num = 0;

        for (auto* s : sources)
            if (s->isDragging())
                ++num;

        return num;
    }

    MouseInputSource* getDraggingMouseSource (int index) noexcept
    {
        int num = 0;

        for (auto& s : sourceArray)
        {
            if (s.isDragging())
            {
                if (index == num)
                    return &s;

                ++num;
            }
        }

        return nullptr;
    }

    void beginDragAutoRepeat (int interval)
    {
        if (interval > 0)
        {
            if (getTimerInterval() != interval)
                startTimer (interval);
        }
        else
        {
            stopTimer();
        }
    }

    void timerCallback() override
    {
        bool anyDragging = false;

        for (auto* s : sources)
        {
            // NB: when doing auto-repeat, we need to force an update of the current position and button state,
            // because on some OSes the queue can get overloaded with messages so that mouse-events don't get through.
            if (s->isDragging() && ComponentPeer::getCurrentModifiersRealtime().isAnyMouseButtonDown())
            {
                s->lastPointerState.position = s->getRawScreenPosition();
                s->triggerFakeMove();
                anyDragging = true;
            }
        }

        if (! anyDragging)
            stopTimer();
    }

    OwnedArray<MouseInputSourceImpl> sources;
    Array<MouseInputSource> sourceArray;

private:
    bool addSource();
    bool canUseTouch() const;
};

} // namespace higgs::detail
