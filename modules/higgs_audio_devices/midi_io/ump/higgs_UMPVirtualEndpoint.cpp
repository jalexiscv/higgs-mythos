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

class VirtualEndpoint::Impl final
{
public:
    class Native
    {
    public:
        virtual ~Native() = default;

        virtual EndpointId getId() const = 0;
        virtual bool setBlock (uint8_t, const Block&) = 0;
        virtual bool setName (const String&) = 0;
    };

    EndpointId getId() const
    {
        return identifier;
    }

    bool setBlock (uint8_t i, const Block& b)
    {
        if (native != nullptr)
            return native->setBlock (i, b);

        return false;
    }

    bool setName (const String& n)
    {
        if (native != nullptr)
            return native->setName (n);

        return false;
    }

    bool isAlive() const
    {
        return native != nullptr;
    }

    static VirtualEndpoint makeVirtualEndpoint (std::unique_ptr<Native> x)
    {
        if (x != nullptr)
            return VirtualEndpoint { rawToUniquePtr (new Impl (std::move (x))) };

        return {};
    }

private:
    explicit Impl (std::unique_ptr<Native> n)
        : native (std::move (n)),
          identifier (native->getId())
    {
    }

    std::unique_ptr<Native> native;
    EndpointId identifier;
};

VirtualEndpoint::VirtualEndpoint() = default;
VirtualEndpoint::~VirtualEndpoint() = default;
VirtualEndpoint::VirtualEndpoint (std::unique_ptr<Impl> x) : impl (std::move (x)) {}
VirtualEndpoint::VirtualEndpoint (VirtualEndpoint&&) noexcept = default;
VirtualEndpoint& VirtualEndpoint::operator= (VirtualEndpoint&&) noexcept = default;

EndpointId VirtualEndpoint::getId() const
{
    if (impl != nullptr)
        return impl->getId();

    return {};
}

bool VirtualEndpoint::setBlock (uint8_t i, const Block& b)
{
    if (impl != nullptr)
        return impl->setBlock (i, b);

    return false;
}

bool VirtualEndpoint::setName (const String& x)
{
    if (impl != nullptr)
        return impl->setName (x);

    return false;
}

bool VirtualEndpoint::isAlive() const
{
    if (impl != nullptr)
        return impl->isAlive();

    return false;
}

}
