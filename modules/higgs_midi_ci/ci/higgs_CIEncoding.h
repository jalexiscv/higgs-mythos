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

namespace higgs::midi_ci
{

#define HIGGS_ENCODINGS X(ascii, "ASCII") X(mcoded7, "Mcoded7") X(zlibAndMcoded7, "zlib+Mcoded7")

/**
    Identifies different encodings that may be used by property exchange messages.

    @tags{Audio}
*/
enum class Encoding
{
   #define X(name, unused) name,
    HIGGS_ENCODINGS
   #undef X
};

/**
    Utility functions for working with the Encoding enum.

    @tags{Audio}
*/
struct EncodingUtils
{
    EncodingUtils() = delete;

    /** Converts an Encoding to a human-readable string. */
    static const char* toString (Encoding e)
    {
        switch (e)
        {
           #define X(name, string) case Encoding::name: return string;
            HIGGS_ENCODINGS
           #undef X
        }

        return nullptr;
    }

    /** Converts an encoding string from a property exchange JSON header to
        an Encoding.
    */
    static std::optional<Encoding> toEncoding (const char* str)
    {
       #define X(name, string) if (std::string_view (str) == std::string_view (string)) return Encoding::name;
        HIGGS_ENCODINGS
       #undef X

        return {};
    }
};

#undef HIGGS_ENCODINGS

} // namespace higgs::midi_ci

/** @cond */
namespace higgs
{
    template <>
    struct SerialisationTraits<midi_ci::Encoding>
    {
        static constexpr auto marshallingVersion = std::nullopt;

        template <typename Archive>
        void load (Archive& archive, midi_ci::Encoding& t)
        {
            String encoding;
            archive (encoding);
            t = midi_ci::EncodingUtils::toEncoding (encoding.toRawUTF8()).value_or (midi_ci::Encoding{});
        }

        template <typename Archive>
        void save (Archive& archive, const midi_ci::Encoding& t)
        {
            archive (midi_ci::EncodingUtils::toString (t));
        }
    };

} // namespace higgs
/** @endcond */
