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

namespace higgs
{

class BytestreamSysexExtractorTest : public UnitTest
{
public:
    BytestreamSysexExtractorTest()
        : UnitTest ("BytestreamSysexExtractor", UnitTestCategories::midi)
    {
    }

    void runTest() override
    {
        testCase ("Passing empty buffer while no message is in progress does nothing", [&]
        {
            BytestreamSysexExtractor extractor;
            bool called = false;
            extractor.push ({}, [&] (auto&&...) { called = true; });

            expect (! called);
        });

        testCase ("Passing sysex with no payload reports an empty message", [&]
        {
            BytestreamSysexExtractor extractor;
            bool called = false;
            const std::byte message[] { std::byte (0xf0), std::byte (0xf7) };
            extractor.push (message, [&] (auto status, auto bytes)
            {
                called = true;
                expect (status == SysexExtractorCallbackKind::lastSysex);
                expect (bytes.size() == 2 && bytes[0] == std::byte (0xf0) && bytes[1] == std::byte (0xf7));
            });

            expect (called);
        });

        testCase ("Sending only the sysex starting byte reports an ongoing message", [&]
        {
            BytestreamSysexExtractor extractor;
            int numCalls = 0;
            const std::byte message[] { std::byte (0xf0) };
            extractor.push (message, [&] (auto status, auto bytes)
            {
                ++numCalls;
                expect (status == SysexExtractorCallbackKind::ongoingSysex);
                expect (bytes.size() == 1 && bytes[0] == std::byte (0xf0));
            });

            expect (numCalls == 1);

            // Sending a subsequent empty span should report an ongoing message
            extractor.push (Span<const std::byte>{}, [&] (auto status, auto bytes)
            {
                ++numCalls;
                expect (status == SysexExtractorCallbackKind::ongoingSysex);
                expect (bytes.empty());
            });

            expect (numCalls == 2);
        });

        testCase ("Sending sysex interspersed with realtime messages filters out the realtime messages", [&]
        {
            BytestreamSysexExtractor extractor;
            const std::byte message[] { std::byte (0xf0),
                                        std::byte (0x50), // first data byte
                                        std::byte (0xfe), // active sensing
                                        std::byte (0x60), // second data byte
                                        std::byte (0x70), // third data byte
                                        std::byte (0xf7) };
            std::vector<std::vector<std::byte>> vectors;
            extractor.push (message, [&] (auto, auto bytes) { vectors.emplace_back (bytes.begin(), bytes.end()); });

            expect (vectors == std::vector { std::vector { std::byte (0xf0), std::byte (0x50) },
                                             std::vector { std::byte (0xfe) },
                                             std::vector { std::byte (0x60), std::byte (0x70), std::byte (0xf7) } });
        });

        testCase ("Sending a second f0 byte during an ongoing sysex terminates the previous sysex", [&]
        {
            BytestreamSysexExtractor extractor;
            const std::byte message[] { std::byte (0xf0), // start of first sysex
                                        std::byte (0x00),
                                        std::byte (0x01),
                                        std::byte (0xf0), // start of second sysex
                                        std::byte (0x02),
                                        std::byte (0x03) };

            std::vector<std::vector<std::byte>> vectors;
            extractor.push (message, [&] (auto, auto bytes) { vectors.emplace_back (bytes.begin(), bytes.end()); });

            expect (vectors == std::vector { std::vector { std::byte (0xf0), std::byte (0x00), std::byte (0x01) },
                                             std::vector { std::byte (0xf0), std::byte (0x02), std::byte (0x03) } });
        });

        testCase ("Status bytes truncate ongoing sysex", [&]
        {
            BytestreamSysexExtractor extractor;
            const std::byte message[] { std::byte (0xf0), // start of first sysex
                                        std::byte (0x10),
                                        std::byte (0x20),
                                        std::byte (0x30),
                                        std::byte (0x80), // status byte
                                        std::byte (0x00),
                                        std::byte (0x00) };

            std::vector<std::vector<std::byte>> vectors;
            extractor.push (message, [&] (auto, auto bytes) { vectors.emplace_back (bytes.begin(), bytes.end()); });

            expect (vectors == std::vector { std::vector { std::byte (0xf0), std::byte (0x10), std::byte (0x20), std::byte (0x30) },
                                             std::vector { std::byte (0x80), std::byte (0x00), std::byte (0x00) } });
        });

        testCase ("Running status is preserved between calls", [&]
        {
            BytestreamSysexExtractor extractor;
            const std::byte message[] { std::byte (0x90), // note on
                                        std::byte (0x10),
                                        std::byte (0x20),
                                        std::byte (0x30),
                                        std::byte (0x40),
                                        std::byte (0x50) };

            std::vector<std::vector<std::byte>> vectors;
            const auto callback = [&] (auto status, auto bytes)
            {
                expect (status == SysexExtractorCallbackKind::notSysex);
                vectors.emplace_back (bytes.begin(), bytes.end());
            };
            extractor.push (message, callback);
            extractor.push (std::array { std::byte (0x60) }, callback);

            expect (vectors == std::vector { std::vector { std::byte (0x90), std::byte (0x10), std::byte (0x20) },
                                             std::vector { std::byte (0x90), std::byte (0x30), std::byte (0x40) },
                                             std::vector { std::byte (0x90), std::byte (0x50), std::byte (0x60) } });
        });

        testCase ("Realtime messages can intersperse bytes of non-sysex messages", [&]
        {
            BytestreamSysexExtractor extractor;
            const std::byte message[] { std::byte (0xd0),   // channel pressure
                                        std::byte (0xfe),   // active sensing
                                        std::byte (0x70),   // pressure cont.
                                        std::byte (0xfe),   // active sensing
                                        std::byte (0x60),   // second pressure message
                                        std::byte (0xfe),   // active sensing
                                        std::byte (0x50) }; // third pressure message

            std::vector<std::vector<std::byte>> vectors;
            extractor.push (message, [&] (auto, auto bytes)
            {
                vectors.emplace_back (bytes.begin(), bytes.end());
            });

            expect (vectors == std::vector { std::vector { std::byte (0xfe) },
                                             std::vector { std::byte (0xd0), std::byte (0x70) },
                                             std::vector { std::byte (0xfe) },
                                             std::vector { std::byte (0xd0), std::byte (0x60) },
                                             std::vector { std::byte (0xfe) },
                                             std::vector { std::byte (0xd0), std::byte (0x50) } });
        });

        testCase ("Non-status bytes with no associated running status are ignored", [&]
        {
            BytestreamSysexExtractor extractor;
            const std::byte message[] { std::byte (0x10),
                                        std::byte (0x2e),
                                        std::byte (0x30),
                                        std::byte (0x4e),
                                        std::byte (0x80),   // note off
                                        std::byte (0x0e),
                                        std::byte (0x00),
                                        std::byte (0xf0),   // sysex
                                        std::byte (0xf7),   // end sysex
                                        std::byte (0x00),   // sysex resets running status
                                        std::byte (0x10), };

            std::vector<std::vector<std::byte>> vectors;
            extractor.push (message, [&] (auto, auto bytes)
            {
                vectors.emplace_back (bytes.begin(), bytes.end());
            });

            expect (vectors == std::vector { std::vector { std::byte (0x80), std::byte (0x0e), std::byte (0x00) },
                                             std::vector { std::byte (0xf0), std::byte (0xf7) } });
        });

        testCase ("Realtime messages after a single-byte system common message do not repeat it", [&]
        {
            BytestreamSysexExtractor extractor;
            const std::byte message[] { std::byte (0xf6),
                                        std::byte (0xf8),
                                        std::byte (0xfa),
                                        std::byte (0xfe) };

            {
                std::vector<std::vector<std::byte>> vectors;
                extractor.push (message, [&] (auto, auto bytes)
                {
                    vectors.emplace_back (bytes.begin(), bytes.end());
                });

                expect (vectors == std::vector { std::vector { std::byte (0xf6) },
                                                 std::vector { std::byte (0xf8) },
                                                 std::vector { std::byte (0xfa) },
                                                 std::vector { std::byte (0xfe) } });
            }

            {
                std::vector<std::vector<std::byte>> vectors;

                for (const auto byte : message)
                {
                    const std::byte singleByte[] { byte };
                    extractor.push (singleByte, [&] (auto, auto bytes)
                    {
                        vectors.emplace_back (bytes.begin(), bytes.end());
                    });
                }

                expect (vectors == std::vector { std::vector { std::byte (0xf6) },
                                                 std::vector { std::byte (0xf8) },
                                                 std::vector { std::byte (0xfa) },
                                                 std::vector { std::byte (0xfe) } });
            }
        });

        testCase ("Realtime messages after a system reset message do not repeat it", [&]
        {
            BytestreamSysexExtractor extractor;
            const std::byte message[] { std::byte (0xff),
                                        std::byte (0xf8),
                                        std::byte (0xfa),
                                        std::byte (0xfe) };

            std::vector<std::vector<std::byte>> vectors;
            extractor.push (message, [&] (auto, auto bytes)
            {
                vectors.emplace_back (bytes.begin(), bytes.end());
            });

            expect (vectors == std::vector { std::vector { std::byte (0xff) },
                                             std::vector { std::byte (0xf8) },
                                             std::vector { std::byte (0xfa) },
                                             std::vector { std::byte (0xfe) } });
        });

        testCase ("System common messages cancel running status", [&]
        {
            BytestreamSysexExtractor extractor;
            const std::byte message[] { std::byte (0x90),   // set running status
                                        std::byte (0xf8),   // realtime byte does no interrupt running status
                                        std::byte (0x00),
                                        std::byte (0x00),
                                        std::byte (0xf3),   // system common clears running status
                                        std::byte (0x05),
                                        std::byte (0x07),   // this byte has no status
                                        std::byte (0x08),   // this byte has no status
                                        std::byte (0x90),   // set running status
                                        std::byte (0x00),
                                        std::byte (0x00) };

            std::vector<std::vector<std::byte>> vectors;
            extractor.push (message, [&] (auto, auto bytes)
            {
                vectors.emplace_back (bytes.begin(), bytes.end());
            });

            expect (vectors == std::vector<std::vector<std::byte>> { std::vector { std::byte (0xf8) },
                                                                     std::vector { std::byte (0x90), std::byte(), std::byte() },
                                                                     std::vector { std::byte (0xf3), std::byte (0x05) },
                                                                     std::vector { std::byte (0x90), std::byte(), std::byte() } });
        });
    }
};

static BytestreamSysexExtractorTest bytestreamSysexExtractorTest;

} // namespace higgs
