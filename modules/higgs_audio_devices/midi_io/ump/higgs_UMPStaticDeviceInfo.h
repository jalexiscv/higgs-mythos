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

/**
    Static information about a particular MIDI device that can be queried without opening a
    connection to the device.

    This information differs from the detailed information in the Endpoint struct, in that
    the StaticDeviceInformation is communicated out-of-band, whereas Endpoint information is
    communicated in-band, i.e. it is sent as MIDI messages after establishing a connection to the
    device.

    @tags{Audio}
*/
class StaticDeviceInfo
{
public:
    [[nodiscard]] StaticDeviceInfo withName           (const String& x) const { return withMember (*this, &StaticDeviceInfo::name, x); }
    [[nodiscard]] StaticDeviceInfo withManufacturer   (const String& x) const { return withMember (*this, &StaticDeviceInfo::manufacturer, x); }
    [[nodiscard]] StaticDeviceInfo withProduct        (const String& x) const { return withMember (*this, &StaticDeviceInfo::product, x); }
    [[nodiscard]] StaticDeviceInfo withHasSource      (bool x)          const { return withMember (*this, &StaticDeviceInfo::source, x); }
    [[nodiscard]] StaticDeviceInfo withHasDestination (bool x)          const { return withMember (*this, &StaticDeviceInfo::destination, x); }
    [[nodiscard]] StaticDeviceInfo withTransport      (Transport x)     const { return withMember (*this, &StaticDeviceInfo::transport, x); }

    [[nodiscard]] StaticDeviceInfo withLegacyIdentifiersSrc (Span<const String, 16> x) const
    {
        auto copy = *this;
        std::copy (x.begin(), x.end(), copy.identifierSrc.begin());
        return copy;
    }

    [[nodiscard]] StaticDeviceInfo withLegacyIdentifiersDst (Span<const String, 16> x) const
    {
        auto copy = *this;
        std::copy (x.begin(), x.end(), copy.identifierDst.begin());
        return copy;
    }

    [[nodiscard]] StaticDeviceInfo withLegacyIdentifiers (IOKind k, Span<const String, 16> x) const
    {
        return k == IOKind::src ? withLegacyIdentifiersSrc (x) : withLegacyIdentifiersDst (x);
    }

    String getName()            const { return name; }
    String getManufacturer()    const { return manufacturer; }
    String getProduct()         const { return product; }
    bool hasSource()            const { return source; }
    bool hasDestination()       const { return destination; }
    Transport getTransport()    const { return transport; }

    /** Returns an identifier to uniquely identify each group, for use with the legacy MIDI API. */
    Span<const String, 16> getLegacyIdentifiersSrc() const& { return identifierSrc; }
    /** Returns an identifier to uniquely identify each group, for use with the legacy MIDI API. */
    Span<const String, 16> getLegacyIdentifiersDst() const& { return identifierDst; }

    Span<const String, 16> getLegacyIdentifiers (IOKind k) const&
    {
        return k == IOKind::src ? getLegacyIdentifiersSrc() : getLegacyIdentifiersDst();
    }

    Span<const String, 16> getLegacyIdentifiersSrc() const&& = delete;
    Span<const String, 16> getLegacyIdentifiersDst() const&& = delete;
    Span<const String, 16> getLegacyIdentifiers (IOKind) const&& = delete;

    bool operator== (const StaticDeviceInfo&) const;
    bool operator!= (const StaticDeviceInfo& other) const { return ! operator== (other); }

private:
    auto tie() const;

    std::array<String, 16> identifierSrc, identifierDst;
    String name;                        ///< The full human-readable name of this device
    String manufacturer;                ///< The name of the organisation that produced this device
    String product;                     ///< The human-readable product name
    Transport transport{};              ///< The format used for MIDI messages in transit
    uint8_t source = false;             ///< True if the device can send messages
    uint8_t destination = false;        ///< True if the device can receive messages
};

}
