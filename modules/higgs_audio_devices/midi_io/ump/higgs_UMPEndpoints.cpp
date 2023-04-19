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

class Endpoints::Impl : private EndpointsListener
{
public:
    class Native
    {
    public:
        virtual ~Native() = default;
        virtual Backend getBackend() const = 0;
        virtual bool isVirtualMidiBytestreamServiceActive() const = 0;
        virtual bool isVirtualMidiUmpServiceActive() const = 0;
        virtual void setVirtualMidiBytestreamServiceActive (bool) {}
        virtual void setVirtualMidiUmpServiceActive (bool) {}
        virtual void getEndpoints (std::vector<EndpointId>&) const = 0;
        virtual std::optional<Endpoint> getEndpoint (const EndpointId&) const = 0;
        virtual std::optional<StaticDeviceInfo> getStaticDeviceInfo (const EndpointId&) const = 0;
        virtual std::unique_ptr<Session::Impl::Native> makeSession (const String&) = 0;

        static std::unique_ptr<Native> make (EndpointsListener&);
    };

    Backend getBackend() const
    {
        return native->getBackend();
    }

    bool isVirtualMidiBytestreamServiceActive() const
    {
        return native->isVirtualMidiBytestreamServiceActive();
    }

    bool isVirtualMidiUmpServiceActive() const
    {
        return native->isVirtualMidiUmpServiceActive();
    }

    void setVirtualMidiBytestreamServiceActive (bool x)
    {
        native->setVirtualMidiBytestreamServiceActive (x);
    }

    void setVirtualMidiUmpServiceActive (bool x)
    {
        native->setVirtualMidiUmpServiceActive (x);
    }

    void getEndpoints (std::vector<EndpointId>& x) const
    {
        native->getEndpoints (x);
    }

    std::optional<Endpoint> getEndpoint (const EndpointId& x) const
    {
        return native->getEndpoint (x);
    }

    std::optional<StaticDeviceInfo> getStaticDeviceInfo (const EndpointId& x) const
    {
        return native->getStaticDeviceInfo (x);
    }

    std::unique_ptr<Session::Impl::Native> makeSession (const String& x)
    {
        return native->makeSession (x);
    }

    void addListener (EndpointsListener& x)
    {
        listeners.add (&x);
    }

    void removeListener (EndpointsListener& x)
    {
        listeners.remove (&x);
    }

    static std::unique_ptr<Impl> make()
    {
        auto result = rawToUniquePtr (new Impl);
        result->native = Native::make (*result);

        if (result->native == nullptr)
            return {};

        return result;
    }

    static String getGlobalMidiClientName()
    {
        if (auto* app = HIGGSApplicationBase::getInstance())
            return app->getApplicationName();

        return "HIGGS";
    }

private:
    void endpointsChanged() override
    {
        listeners.call ([] (auto& l) { l.endpointsChanged(); });
    }

    void virtualMidiServiceActiveChanged() override
    {
        listeners.call ([] (auto& l) { l.virtualMidiServiceActiveChanged(); });
    }

    Impl() = default;

    ListenerList<EndpointsListener> listeners;
    std::unique_ptr<Native> native;
};

void Endpoints::getEndpoints (std::vector<EndpointId>& x) const
{
    x.clear();

    if (impl != nullptr)
        impl->getEndpoints (x);
}

std::vector<EndpointId> Endpoints::getEndpoints() const
{
    std::vector<EndpointId> result;
    getEndpoints (result);
    return result;
}

std::optional<Endpoint> Endpoints::getEndpoint (const EndpointId& x) const
{
    if (impl != nullptr)
        return impl->getEndpoint (x);

    return {};
}

std::optional<StaticDeviceInfo> Endpoints::getStaticDeviceInfo (const EndpointId& x) const
{
    if (impl != nullptr)
        return impl->getStaticDeviceInfo (x);

    return {};
}

void Endpoints::addListener (EndpointsListener& x)
{
    if (impl != nullptr)
        impl->addListener (x);
}

void Endpoints::removeListener (EndpointsListener& x)
{
    if (impl != nullptr)
        impl->removeListener (x);
}

Session Endpoints::makeSession (const String& x) const
{
    return Session::Impl::makeSession (impl != nullptr ? impl->makeSession (x) : nullptr);
}

std::optional<Backend> Endpoints::getBackend() const
{
    if (impl != nullptr)
        return impl->getBackend();

    return {};
}

bool Endpoints::isVirtualMidiBytestreamServiceActive() const
{
    if (impl != nullptr)
        return impl->isVirtualMidiBytestreamServiceActive();

    return false;
}

bool Endpoints::isVirtualMidiUmpServiceActive() const
{
    if (impl != nullptr)
        return impl->isVirtualMidiUmpServiceActive();

    return false;
}

void Endpoints::setVirtualMidiBytestreamServiceActive (bool x)
{
    if (impl != nullptr)
        impl->setVirtualMidiBytestreamServiceActive (x);
}

void Endpoints::setVirtualMidiUmpServiceActive (bool x)
{
    if (impl != nullptr)
        impl->setVirtualMidiUmpServiceActive (x);
}

Endpoints::Endpoints()
    : impl (Impl::make())
{
}

Endpoints::~Endpoints()
{
    clearSingletonInstance();
}

}
