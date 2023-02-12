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

/** @cond */
namespace higgs::universal_midi_packets
{
    /**
        Allows conversion from bytestream- or Universal MIDI Packet-formatted
        messages to MIDI 1.0 messages in UMP format.

        @tags{Audio}
    */
    struct ToUMP1Converter
    {
        template <typename Fn>
        void convert (const BytesOnGroup& m, Fn&& fn)
        {
            Conversion::toMidi1 (m, std::forward<Fn> (fn));
        }

        template <typename Fn>
        void convert (const View& v, Fn&& fn)
        {
            Conversion::midi2ToMidi1DefaultTranslation (v, std::forward<Fn> (fn));
        }

        void reset() {}
    };

    /**
        Allows conversion from bytestream- or Universal MIDI Packet-formatted
        messages to MIDI 2.0 messages in UMP format.

        @tags{Audio}
    */
    struct ToUMP2Converter
    {
        template <typename Fn>
        void convert (const BytesOnGroup& m, Fn&& fn)
        {
            Conversion::toMidi1 (m, [&] (const View& v)
            {
                translator.dispatch (v, fn);
            });
        }

        template <typename Fn>
        void convert (const View& v, Fn&& fn)
        {
            translator.dispatch (v, std::forward<Fn> (fn));
        }

        void reset()
        {
            translator.reset();
        }

        Midi1ToMidi2DefaultTranslator translator;
    };

    /**
        Allows conversion from bytestream- or Universal MIDI Packet-formatted
        messages to UMP format.

        The packet protocol can be selected using the constructor parameter.

        @tags{Audio}
    */
    class GenericUMPConverter
    {
        using Converters = std::variant<ToUMP1Converter, ToUMP2Converter>;

        template <typename This, typename Fn>
        static void visit (This& t, Fn&& fn)
        {
            if (auto* converter1 = std::get_if<ToUMP1Converter> (&t.converters))
                fn (*converter1);
            else if (auto* converter2 = std::get_if<ToUMP2Converter> (&t.converters))
                fn (*converter2);
        }

    public:
        explicit GenericUMPConverter (PacketProtocol m)
            : converters (m == PacketProtocol::MIDI_1_0 ? Converters (ToUMP1Converter()) : Converters (ToUMP2Converter())) {}

        void reset()
        {
            visit (*this, [] (auto& c) { c.reset(); });
        }

        template <typename Converter, typename Fn>
        static void convertImpl (Converter& converter, const BytesOnGroup& m, Fn&& fn)
        {
            converter.convert (m, std::forward<Fn> (fn));
        }

        template <typename Converter, typename Fn>
        static void convertImpl (Converter& converter, const View& m, Fn&& fn)
        {
            converter.convert (m, std::forward<Fn> (fn));
        }

        template <typename Converter, typename Fn>
        static void convertImpl (Converter& converter, Iterator b, Iterator e, Fn&& fn)
        {
            std::for_each (b, e, [&] (const auto& v)
            {
                convertImpl (converter, v, fn);
            });
        }

        template <typename Converter, typename Fn>
        static void convertImpl (Converter& converter, const Packets& packets, Fn&& fn)
        {
            convertImpl (converter, packets.begin(), packets.end(), std::forward<Fn> (fn));
        }

        template <typename Fn>
        void convert (const BytesOnGroup& m, Fn&& fn)
        {
            visit (*this, [&] (auto& c) { convertImpl (c, m, std::forward<Fn> (fn)); });
        }

        template <typename Fn>
        void convert (const View& v, Fn&& fn)
        {
            visit (*this, [&] (auto& c) { convertImpl (c, v, std::forward<Fn> (fn)); });
        }

        template <typename Fn>
        void convert (Iterator begin, Iterator end, Fn&& fn)
        {
            visit (*this, [&] (auto& c) { convertImpl (c, begin, end, std::forward<Fn> (fn)); });
        }

        template <typename Fn>
        void convert (const Packets& packets, Fn&& fn)
        {
            visit (*this, [&] (auto& c) { convertImpl (c, packets, std::forward<Fn> (fn)); });
        }

        PacketProtocol getProtocol() const noexcept
        {
            return std::holds_alternative<ToUMP1Converter> (converters) ? PacketProtocol::MIDI_1_0
                                                                        : PacketProtocol::MIDI_2_0;
        }

    private:
        Converters converters;
    };

    /**
        Allows conversion from bytestream- or Universal MIDI Packet-formatted
        messages to bytestream format.

        @tags{Audio}
    */
    struct ToBytestreamConverter
    {
        explicit ToBytestreamConverter (int storageSize)
            : translator (storageSize) {}

        template <typename Fn>
        void convert (const MidiMessage& m, Fn&& fn)
        {
            fn (m);
        }

        template <typename Fn>
        void convert (const View& v, double time, Fn&& fn)
        {
            Conversion::midi2ToMidi1DefaultTranslation (v, [&] (const View& midi1)
            {
                translator.dispatch (midi1, time, fn);
            });
        }

        void reset() { translator.reset(); }

        SingleGroupMidi1ToBytestreamTranslator translator;
    };
} // namespace higgs::universal_midi_packets
/** @endcond */
