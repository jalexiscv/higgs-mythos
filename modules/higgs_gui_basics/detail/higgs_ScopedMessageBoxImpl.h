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

//==============================================================================
class ScopedMessageBoxImpl
{
public:
    virtual ~ScopedMessageBoxImpl() = default;
    virtual void close() = 0;
};

//==============================================================================
class ConcreteScopedMessageBoxImpl : public ScopedMessageBoxImpl,
                                     public AsyncUpdater
{
public:
    static ScopedMessageBox show (std::unique_ptr<ScopedMessageBoxInterface>&& native,
                                  std::function<void (int)> callback)
    {
        return ScopedMessageBox (runAsync (std::move (native),
                                           rawToUniquePtr (ModalCallbackFunction::create (std::move (callback)))));
    }

    static int showUnmanaged (std::unique_ptr<ScopedMessageBoxInterface>&& native,
                              ModalComponentManager::Callback* cb)
    {
       #if HIGGS_MODAL_LOOPS_PERMITTED
        if (cb == nullptr)
            return runSync (std::move (native)).value_or (0);
       #endif

        runAsync (std::move (native), rawToUniquePtr (cb));

        return 0;
    }

    ~ConcreteScopedMessageBoxImpl() override
    {
        cancelPendingUpdate();
    }

    void close() override
    {
        cancelPendingUpdate();
        callback.reset();
        nativeImplementation->close();
        self.reset();
    }

private:
    static std::shared_ptr<ConcreteScopedMessageBoxImpl> runAsync (std::unique_ptr<ScopedMessageBoxInterface>&& p,
                                                                   std::unique_ptr<ModalComponentManager::Callback>&& c)
    {
        std::shared_ptr<ConcreteScopedMessageBoxImpl> result (new ConcreteScopedMessageBoxImpl (std::move (p), std::move (c)));
        result->self = result;
        result->triggerAsyncUpdate();
        return result;
    }

    static std::optional<int> runSync (std::unique_ptr<ScopedMessageBoxInterface>&& p)
    {
        auto local = std::move (p);
        return local != nullptr ? local->runSync() : std::nullopt;
    }

    explicit ConcreteScopedMessageBoxImpl (std::unique_ptr<ScopedMessageBoxInterface>&& p)
        : ConcreteScopedMessageBoxImpl (std::move (p), nullptr) {}

    ConcreteScopedMessageBoxImpl (std::unique_ptr<ScopedMessageBoxInterface>&& p,
                                  std::unique_ptr<ModalComponentManager::Callback>&& c)
        : callback (std::move (c)), nativeImplementation (std::move (p)) {}

    void handleAsyncUpdate() override
    {
        nativeImplementation->runAsync ([weakRecipient = std::weak_ptr<ConcreteScopedMessageBoxImpl> (self)] (std::optional<int> result)
        {
            const auto notifyRecipient = [result, weakRecipient]
            {
                if (const auto locked = weakRecipient.lock())
                {
                    if (auto* cb = locked->callback.get())
                        cb->modalStateFinished (result.value_or (0));

                    locked->self.reset();
                }
            };

            if (MessageManager::getInstance()->isThisTheMessageThread())
                notifyRecipient();
            else
                MessageManager::callAsync (notifyRecipient);
        });
    }

    std::unique_ptr<ModalComponentManager::Callback> callback;
    std::unique_ptr<ScopedMessageBoxInterface> nativeImplementation;

    /*  The 'old' native message box API doesn't have a concept of message box owners.
        Instead, message boxes have to clean up after themselves, once they're done displaying.
        To allow this mode of usage, the implementation keeps an owning reference to itself,
        which is cleared once the message box is closed or asked to quit. To display a native
        message box without a scoped lifetime, just create a Pimpl instance without using
        the ScopedMessageBox wrapper, and the Pimpl will destroy itself after it is dismissed.
    */
    std::shared_ptr<ConcreteScopedMessageBoxImpl> self;
};

} // namespace higgs::detail
