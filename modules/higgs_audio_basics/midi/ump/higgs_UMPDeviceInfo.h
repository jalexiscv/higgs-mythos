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
    Holds MIDI device info that may be required by certain UMP messages and
    MIDI-CI messages.

    @tags{Audio}
*/
struct DeviceInfo
{
    std::array<std::byte, 3> manufacturer;
    std::array<std::byte, 2> family;        ///< LSB first
    std::array<std::byte, 2> modelNumber;   ///< LSB first
    std::array<std::byte, 4> revision;

private:
    auto tie() const;

public:
    bool operator== (const DeviceInfo& other) const;
    bool operator!= (const DeviceInfo& other) const;

    static constexpr auto marshallingVersion = std::nullopt;

    template <typename Archive, typename This>
    static auto serialise (Archive& archive, This& t)
    {
        return archive (named ("manufacturer", t.manufacturer),
                        named ("family", t.family),
                        named ("modelNumber", t.modelNumber),
                        named ("revision", t.revision));
    }
};

} // namespace higgs::universal_midi_packets
