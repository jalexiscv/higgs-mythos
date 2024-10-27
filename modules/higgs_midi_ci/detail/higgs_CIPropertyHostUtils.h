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

namespace higgs::midi_ci::detail
{

struct PropertyHostUtils
{
    PropertyHostUtils() = delete;

    static void send (BufferOutput& output,
                      uint8_t group,
                      std::byte subID2,
                      MUID targetMuid,
                      std::byte requestID,
                      Span<const std::byte> header,
                      Span<const std::byte> body,
                      int chunkSize)
    {
        MemoryInputStream stream (body.data(), body.size(), false);
        const detail::PropertyDataMessageChunker chunker { output.getOutputBuffer(),
                                                           std::min (chunkSize, 1 << 16),
                                                           subID2,
                                                           requestID,
                                                           header,
                                                           output.getMuid(),
                                                           targetMuid,
                                                           stream };

        std::for_each (chunker.begin(), chunker.end(), [&] (auto) { output.send (group); });
    }
};
} // namespace higgs::midi_ci::detail
