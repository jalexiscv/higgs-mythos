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

class LockingAsyncUpdater::Impl : public CallbackMessage
{
public:
    explicit Impl (std::function<void()> cb)
        : callback (std::move (cb)) {}

    void clear()
    {
        const ScopedLock lock (mutex);
        deliver = false;
        callback = nullptr;
    }

    void trigger()
    {
        {
            const ScopedLock lock (mutex);

            if (deliver)
                return;

            deliver = true;
        }

        if (! post())
            cancel();
    }

    void cancel()
    {
        const ScopedLock lock (mutex);
        deliver = false;
    }

    bool isPending()
    {
        const ScopedLock lock (mutex);
        return deliver;
    }

    void messageCallback() override
    {
        const ScopedLock lock (mutex);

        if (std::exchange (deliver, false))
            NullCheckedInvocation::invoke (callback);
    }

private:
    CriticalSection mutex;
    std::function<void()> callback;
    bool deliver = false;
};

//==============================================================================
LockingAsyncUpdater::LockingAsyncUpdater (std::function<void()> callbackToUse)
    : impl (new Impl (std::move (callbackToUse))) {}

LockingAsyncUpdater::LockingAsyncUpdater (LockingAsyncUpdater&& other) noexcept
    : impl (std::exchange (other.impl, nullptr)) {}

LockingAsyncUpdater& LockingAsyncUpdater::operator= (LockingAsyncUpdater&& other) noexcept
{
    LockingAsyncUpdater temp { std::move (other) };
    std::swap (temp.impl, impl);
    return *this;
}

LockingAsyncUpdater::~LockingAsyncUpdater()
{
    if (impl != nullptr)
        impl->clear();
}

void LockingAsyncUpdater::triggerAsyncUpdate()
{
    if (impl != nullptr)
        impl->trigger();
    else
        jassertfalse; // moved-from!
}

void LockingAsyncUpdater::cancelPendingUpdate() noexcept
{
    if (impl != nullptr)
        impl->cancel();
    else
        jassertfalse; // moved-from!
}

void LockingAsyncUpdater::handleUpdateNowIfNeeded()
{
    if (impl != nullptr)
        impl->messageCallback();
    else
        jassertfalse; // moved-from!
}

bool LockingAsyncUpdater::isUpdatePending() const noexcept
{
    if (impl != nullptr)
        return impl->isPending();

    jassertfalse; // moved-from!
    return false;
}

} // namespace higgs
