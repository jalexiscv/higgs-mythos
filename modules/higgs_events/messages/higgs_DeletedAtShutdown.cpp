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

static SpinLock deletedAtShutdownLock; // use a spin lock because it can be statically initialised

static Array<DeletedAtShutdown*>& getDeletedAtShutdownObjects()
{
    static Array<DeletedAtShutdown*> objects;
    return objects;
}

DeletedAtShutdown::DeletedAtShutdown()
{
    const SpinLock::ScopedLockType sl (deletedAtShutdownLock);
    getDeletedAtShutdownObjects().add (this);
}

DeletedAtShutdown::~DeletedAtShutdown()
{
    const SpinLock::ScopedLockType sl (deletedAtShutdownLock);
    getDeletedAtShutdownObjects().removeFirstMatchingValue (this);
}

// Disable unreachable code warning, in case the compiler manages to figure out that
// you have no classes of DeletedAtShutdown that could throw an exception in their destructor.
HIGGS_BEGIN_IGNORE_WARNINGS_MSVC (4702)

void DeletedAtShutdown::deleteAll()
{
    // make a local copy of the array, so it can't get into a loop if something
    // creates another DeletedAtShutdown object during its destructor.
    Array<DeletedAtShutdown*> localCopy;

    {
        const SpinLock::ScopedLockType sl (deletedAtShutdownLock);
        localCopy = getDeletedAtShutdownObjects();
    }

    for (int i = localCopy.size(); --i >= 0;)
    {
        HIGGS_TRY
        {
            auto* deletee = localCopy.getUnchecked (i);

            // double-check that it's not already been deleted during another object's destructor
            {
                const SpinLock::ScopedLockType sl (deletedAtShutdownLock);

                if (! getDeletedAtShutdownObjects().contains (deletee))
                    deletee = nullptr;
            }

            delete deletee;
        }
        HIGGS_CATCH_EXCEPTION
    }

    // if this fails, then it's likely that some new DeletedAtShutdown objects were
    // created while executing the destructors of the other ones.
    jassert (getDeletedAtShutdownObjects().isEmpty());

    getDeletedAtShutdownObjects().clear(); // just to make sure the array doesn't have any memory still allocated
}

HIGGS_END_IGNORE_WARNINGS_MSVC

} // namespace higgs
