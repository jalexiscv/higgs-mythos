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

class LegacyVirtualOutput::Impl final
{
public:
    class Native
    {
    public:
        virtual ~Native() = default;

        virtual EndpointId getId() const = 0;
    };

    EndpointId getId() const
    {
        return identifier;
    }

    bool isAlive() const
    {
        return native != nullptr;
    }

    static LegacyVirtualOutput makeLegacyVirtualOutput (std::unique_ptr<Native> x)
    {
        if (x != nullptr)
            return LegacyVirtualOutput { rawToUniquePtr (new Impl (std::move (x))) };

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

LegacyVirtualOutput::LegacyVirtualOutput() = default;
LegacyVirtualOutput::~LegacyVirtualOutput() = default;
LegacyVirtualOutput::LegacyVirtualOutput (std::unique_ptr<Impl> x) : impl (std::move (x)) {}
LegacyVirtualOutput::LegacyVirtualOutput (LegacyVirtualOutput&&) noexcept = default;
LegacyVirtualOutput& LegacyVirtualOutput::operator= (LegacyVirtualOutput&&) noexcept = default;

EndpointId LegacyVirtualOutput::getId() const
{
    if (impl != nullptr)
        return impl->getId();

    return {};
}

bool LegacyVirtualOutput::isAlive() const
{
    if (impl != nullptr)
        return impl->isAlive();

    return false;
}

}
