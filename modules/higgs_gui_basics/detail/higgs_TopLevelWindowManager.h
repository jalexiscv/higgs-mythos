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

/* Keeps track of the active top level window. */
class TopLevelWindowManager  : private Timer,
                               private DeletedAtShutdown
{
public:
    TopLevelWindowManager() = default;

    ~TopLevelWindowManager() override
    {
        clearSingletonInstance();
    }

    HIGGS_DECLARE_SINGLETON_SINGLETHREADED_MINIMAL_INLINE (TopLevelWindowManager)

    static void checkCurrentlyFocusedTopLevelWindow()
    {
        if (auto* wm = TopLevelWindowManager::getInstanceWithoutCreating())
            wm->checkFocusAsync();
    }

    void checkFocusAsync()
    {
        startTimer (10);
    }

    void checkFocus()
    {
        startTimer (jmin (1731, getTimerInterval() * 2));

        auto* newActive = findCurrentlyActiveWindow();

        if (newActive != currentActive)
        {
            currentActive = newActive;

            for (int i = windows.size(); --i >= 0;)
                if (auto* tlw = windows[i])
                    tlw->setWindowActive (isWindowActive (tlw));

            Desktop::getInstance().triggerFocusCallback();
        }
    }

    bool addWindow (TopLevelWindow* const w)
    {
        windows.add (w);
        checkFocusAsync();

        return isWindowActive (w);
    }

    void removeWindow (TopLevelWindow* const w)
    {
        checkFocusAsync();

        if (currentActive == w)
            currentActive = nullptr;

        windows.removeFirstMatchingValue (w);

        if (windows.isEmpty())
            deleteInstance();
    }

    Array<TopLevelWindow*> windows;

private:
    TopLevelWindow* currentActive = nullptr;

    void timerCallback() override
    {
        checkFocus();
    }

    bool isWindowActive (TopLevelWindow* const tlw) const
    {
        return (tlw == currentActive
                || tlw->isParentOf (currentActive)
                || tlw->hasKeyboardFocus (true))
               && tlw->isShowing();
    }

    TopLevelWindow* findCurrentlyActiveWindow() const
    {
        if (Process::isForegroundProcess())
        {
            auto* focusedComp = Component::getCurrentlyFocusedComponent();
            auto* w = dynamic_cast<TopLevelWindow*> (focusedComp);

            if (w == nullptr && focusedComp != nullptr)
                w = focusedComp->findParentComponentOfClass<TopLevelWindow>();

            if (w == nullptr)
                w = currentActive;

            if (w != nullptr && w->isShowing())
                return w;
        }

        return nullptr;
    }

    HIGGS_DECLARE_NON_COPYABLE (TopLevelWindowManager)
};

} // namespace higgs::detail
