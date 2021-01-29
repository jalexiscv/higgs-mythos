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

#if HIGGS_ENABLE_ALLOCATION_HOOKS

namespace higgs
{

class AllocationHooks
{
public:
    struct Listener
    {
        virtual ~Listener() noexcept = default;
        virtual void newOrDeleteCalled() noexcept = 0;
    };

    void addListener    (Listener* l)          { listenerList.add (l); }
    void removeListener (Listener* l) noexcept { listenerList.remove (l); }

private:
    friend void notifyAllocationHooksForThread();
    LightweightListenerList<Listener> listenerList;
};

//==============================================================================
/** Scoped checker which will cause a unit test failure if any new/delete calls
    are made during the lifetime of the UnitTestAllocationChecker.
*/
class UnitTestAllocationChecker  : private AllocationHooks::Listener
{
public:
    /** Create a checker which will log a failure to the passed test if
        any calls to new/delete are made.

        Remember to call `UnitTest::beginTest` before constructing this checker!
    */
    explicit UnitTestAllocationChecker (UnitTest& test);

    /** Will add a failure to the test if the number of new/delete calls during
        this object's lifetime was greater than zero.
    */
    ~UnitTestAllocationChecker() noexcept override;

private:
    void newOrDeleteCalled() noexcept override;

    UnitTest& unitTest;
    size_t calls = 0;
};

}

#endif
