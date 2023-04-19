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

namespace higgs::universal_midi_packets
{

class Input::Impl final : private Consumer,
                          private DisconnectionListener
{
public:
    class Native
    {
    public:
        virtual ~Native() = default;

        /*  Returns the ID of the endpoint to which this connection is connected. */
        virtual EndpointId getEndpointId() const = 0;

        /*  The protocol to which incoming messages are converted. */
        virtual PacketProtocol getProtocol() const = 0;
    };

    ~Impl() override
    {
        HIGGS_ASSERT_MESSAGE_THREAD
    }

    EndpointId getEndpointId() const
    {
        return identifier;
    }

    PacketProtocol getProtocol() const
    {
        return protocol;
    }

    void addConsumer (Consumer& x)
    {
        consumers.add (x);
    }

    void removeConsumer (Consumer& x)
    {
        consumers.remove (x);
    }

    void addDisconnectionListener (DisconnectionListener& x)
    {
        disconnectListeners.add (&x);
    }

    void removeDisconnectionListener (DisconnectionListener& x)
    {
        disconnectListeners.remove (&x);
    }

    bool isAlive() const
    {
        return connected && native != nullptr;
    }

    template <typename Callback>
    static Input makeInput (Callback&& callback)
    {
        auto impl = rawToUniquePtr (new Impl);
        impl->native = callback (static_cast<DisconnectionListener&> (*impl), static_cast<Consumer&> (*impl));

        if (impl->native == nullptr)
            return {};

        impl->identifier = impl->native->getEndpointId();
        impl->protocol = impl->native->getProtocol();
        return Input { std::move (impl) };
    }

private:
    Impl()
    {
        HIGGS_ASSERT_MESSAGE_THREAD
    }

    void consume (Iterator b, Iterator e, double t) override
    {
        consumers.call ([&] (auto& l) { l.consume (b, e, t); });
    }

    void disconnected() override
    {
        HIGGS_ASSERT_MESSAGE_THREAD
        connected = false;
        disconnectListeners.call ([] (auto& x) { x.disconnected(); });
    }

    ListenerList<DisconnectionListener> disconnectListeners;
    WaitFreeListeners<Consumer> consumers;
    EndpointId identifier;
    PacketProtocol protocol;
    std::unique_ptr<Native> native;
    std::atomic<bool> connected { true };
};

Input::Input() = default;
Input::~Input() = default;
Input::Input (std::unique_ptr<Impl> x) : impl (std::move (x)) {}
Input::Input (Input&&) noexcept = default;
Input& Input::operator= (Input&&) noexcept = default;

EndpointId Input::getEndpointId() const
{
    if (impl != nullptr)
        return impl->getEndpointId();

    return {};
}

PacketProtocol Input::getProtocol() const
{
    // You should ensure that isAlive() returns true before calling other member functions!
    jassert (isAlive());

    if (impl != nullptr)
        return impl->getProtocol();

    return {};
}

void Input::addConsumer (Consumer& x)
{
    // You should ensure that isAlive() returns true before calling other member functions!
    jassert (isAlive());

    // This function should only be called on the main thread!
    HIGGS_ASSERT_MESSAGE_THREAD

    if (impl != nullptr)
        impl->addConsumer (x);
}

void Input::removeConsumer (Consumer& x)
{
    // This function should only be called on the main thread!
    HIGGS_ASSERT_MESSAGE_THREAD

    if (impl != nullptr)
        impl->removeConsumer (x);
}

void Input::addDisconnectionListener (DisconnectionListener& x)
{
    // You should ensure that isAlive() returns true before calling other member functions!
    jassert (isAlive());

    // This function should only be called on the main thread!
    HIGGS_ASSERT_MESSAGE_THREAD

    if (impl != nullptr)
        impl->addDisconnectionListener (x);
}

void Input::removeDisconnectionListener (DisconnectionListener& x)
{
    // This function should only be called on the main thread!
    HIGGS_ASSERT_MESSAGE_THREAD

    if (impl != nullptr)
        impl->removeDisconnectionListener (x);
}

bool Input::isAlive() const
{
    if (impl != nullptr)
        return impl->isAlive();

    return false;
}

}
