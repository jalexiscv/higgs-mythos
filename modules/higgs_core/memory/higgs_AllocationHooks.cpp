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

static AllocationHooks& getAllocationHooksForThread()
{
    thread_local AllocationHooks hooks;
    return hooks;
}

void notifyAllocationHooksForThread()
{
    getAllocationHooksForThread().listenerList.call ([] (AllocationHooks::Listener& l)
    {
        l.newOrDeleteCalled();
    });
}

}

void* operator new (size_t s)
{
    higgs::notifyAllocationHooksForThread();
    return std::malloc (s);
}

void* operator new[] (size_t s)
{
    higgs::notifyAllocationHooksForThread();
    return std::malloc (s);
}

void operator delete (void* p) noexcept
{
    higgs::notifyAllocationHooksForThread();
    std::free (p);
}

void operator delete[] (void* p) noexcept
{
    higgs::notifyAllocationHooksForThread();
    std::free (p);
}

void operator delete (void* p, size_t) noexcept
{
    higgs::notifyAllocationHooksForThread();
    std::free (p);
}

void operator delete[] (void* p, size_t) noexcept
{
    higgs::notifyAllocationHooksForThread();
    std::free (p);
}

namespace higgs
{

//==============================================================================
UnitTestAllocationChecker::UnitTestAllocationChecker (UnitTest& test)
    : unitTest (test)
{
    getAllocationHooksForThread().addListener (this);
}

UnitTestAllocationChecker::~UnitTestAllocationChecker() noexcept
{
    getAllocationHooksForThread().removeListener (this);
    unitTest.expectEquals ((int) calls, 0, "new or delete was incorrectly called while allocation checker was active");
}

void UnitTestAllocationChecker::newOrDeleteCalled() noexcept { ++calls; }

}

#endif
