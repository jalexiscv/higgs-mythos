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

struct ThreadPriorities
{
    struct Entry
    {
        Thread::Priority priority;
        int native;
    };

   #if HIGGS_ANDROID
    enum AndroidThreadPriority
    {
        THREAD_PRIORITY_AUDIO          = -16,
        THREAD_PRIORITY_FOREGROUND     = -2,
        THREAD_PRIORITY_MORE_FAVORABLE = -1,
        THREAD_PRIORITY_DEFAULT        = 0,
        THREAD_PRIORITY_LESS_FAVORABLE = 1,
        THREAD_PRIORITY_BACKGROUND     = 10,
        THREAD_PRIORITY_LOWEST         = 19
    };
   #endif

    static inline constexpr Entry table[]
    {
       #if HIGGS_ANDROID
        { Thread::Priority::highest,    AndroidThreadPriority::THREAD_PRIORITY_AUDIO },
        { Thread::Priority::high,       AndroidThreadPriority::THREAD_PRIORITY_FOREGROUND },
        { Thread::Priority::normal,     AndroidThreadPriority::THREAD_PRIORITY_DEFAULT },
        { Thread::Priority::low,        AndroidThreadPriority::THREAD_PRIORITY_BACKGROUND - 5 },
        { Thread::Priority::background, AndroidThreadPriority::THREAD_PRIORITY_BACKGROUND },
       #endif

       #if HIGGS_LINUX || HIGGS_BSD
        { Thread::Priority::highest,    0 },
        { Thread::Priority::high,       0 },
        { Thread::Priority::normal,     0 },
        { Thread::Priority::low,        0 },
        { Thread::Priority::background, 0 },
       #endif

       #if HIGGS_MAC || HIGGS_IOS
        { Thread::Priority::highest,    4 },
        { Thread::Priority::high,       3 },
        { Thread::Priority::normal,     2 },
        { Thread::Priority::low,        1 },
        { Thread::Priority::background, 0 },
       #endif

       #if HIGGS_WINDOWS
        { Thread::Priority::highest,    THREAD_PRIORITY_TIME_CRITICAL },
        { Thread::Priority::high,       THREAD_PRIORITY_HIGHEST },
        { Thread::Priority::normal,     THREAD_PRIORITY_NORMAL },
        { Thread::Priority::low,        THREAD_PRIORITY_LOWEST },
        { Thread::Priority::background, THREAD_PRIORITY_IDLE },
       #endif
    };

    static_assert (std::size (table) == 5,
                   "The platform may be unsupported or there may be a priority entry missing.");

    static Thread::Priority getHiggsPriority (const int value)
    {
        const auto iter = std::min_element (std::begin (table),
                                            std::end   (table),
                                            [value] (const auto& a, const auto& b)
                                            {
                                                return std::abs (a.native - value) < std::abs (b.native - value);
                                            });

        jassert (iter != std::end (table));
        return iter != std::end (table) ? iter->priority : Thread::Priority{};
    }

    static int getNativePriority (const Thread::Priority value)
    {
        const auto iter = std::find_if (std::begin (table),
                                        std::end   (table),
                                        [value] (const auto& entry) { return entry.priority == value; });

        jassert (iter != std::end (table));
        return iter != std::end (table) ? iter->native : 0;
    }
};

} // namespace higgs
