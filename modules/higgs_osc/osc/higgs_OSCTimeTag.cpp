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

const OSCTimeTag OSCTimeTag::immediately;

static const uint64 millisecondsBetweenOscAndHiggsEpochs = 2208988800000ULL;
static const uint64 rawTimeTagRepresentingImmediately = 0x0000000000000001ULL;

//==============================================================================
OSCTimeTag::OSCTimeTag() noexcept  : rawTimeTag (rawTimeTagRepresentingImmediately)
{
}

OSCTimeTag::OSCTimeTag (uint64 t) noexcept  : rawTimeTag (t)
{
}

OSCTimeTag::OSCTimeTag (Time time) noexcept
{
    const uint64 milliseconds = (uint64) time.toMilliseconds() + millisecondsBetweenOscAndHiggsEpochs;

    uint64 seconds = milliseconds / 1000;
    uint32 fractionalPart = uint32 (4294967.296 * (milliseconds % 1000));

    rawTimeTag = (seconds << 32) + fractionalPart;
}

//==============================================================================
Time OSCTimeTag::toTime() const noexcept
{
    const uint64 seconds = rawTimeTag >> 32;
    const uint32 fractionalPart = (rawTimeTag & 0x00000000FFFFFFFFULL);

    const auto fractionalPartInMillis = (double) fractionalPart / 4294967.296;

    // now using signed integer, because this is allowed to become negative:
    const auto higgsTimeInMillis = (int64) (seconds * 1000)
                                + (int64) roundToInt (fractionalPartInMillis)
                                - (int64) millisecondsBetweenOscAndHiggsEpochs;

    return Time (higgsTimeInMillis);
}

//==============================================================================
bool OSCTimeTag::isImmediately() const noexcept
{
    return rawTimeTag == rawTimeTagRepresentingImmediately;
}

//==============================================================================
//==============================================================================
#if HIGGS_UNIT_TESTS

class OSCTimeTagTests final : public UnitTest
{
public:
    OSCTimeTagTests()
        : UnitTest ("OSCTimeTag class", UnitTestCategories::osc)
    {}

    void runTest() override
    {
        beginTest ("Basics");

        {
            OSCTimeTag tag;
            expect (tag.isImmediately());
        }
        {
            OSCTimeTag tag (3535653);
            expect (! tag.isImmediately());

            OSCTimeTag otherTag;
            otherTag = tag;
            expect (! otherTag.isImmediately());

            OSCTimeTag copyTag (tag);
            expect (! copyTag.isImmediately());
        }

        beginTest ("Conversion to/from HIGGS Time");

        {
            Time time;
            OSCTimeTag tag (time);
            expect (! tag.isImmediately());
        }
        {
            OSCTimeTag tag;
            Time time = tag.toTime();
            expect (time < Time::getCurrentTime());
        }
        {
            Time currentTime (Time::currentTimeMillis());

            // Make sure we use a double that has a representation that will always
            // truncate to what we expect when multipled by 1000 and turned into
            // integer milliseconds.
            double deltaInSeconds = 1.562;

            RelativeTime delta (deltaInSeconds);
            Time laterTime = currentTime + delta;

            OSCTimeTag currentTimeTag (currentTime);
            OSCTimeTag laterTimeTag (laterTime);

            uint64 currentTimeTagRaw = currentTimeTag.getRawTimeTag();
            uint64 laterTimeTagRaw = laterTimeTag.getRawTimeTag();

            // in the raw time tag, the most significant 32 bits are seconds,
            // so let's verify that the difference is right:
            uint64 diff = laterTimeTagRaw - currentTimeTagRaw;
            double acceptableErrorInSeconds = 0.000001; // definitely not audible anymore

            expect ((float) diff / float (1ULL << 32) < deltaInSeconds + acceptableErrorInSeconds );
            expect ((float) diff / float (1ULL << 32) > deltaInSeconds - acceptableErrorInSeconds );

            // round trip:

            Time currentTime2 = currentTimeTag.toTime();
            Time laterTime2 = laterTimeTag.toTime();
            RelativeTime delta2 = laterTime2 - currentTime2;

            expect (currentTime2 == currentTime);
            expect (laterTime2 == laterTime);
            expect (delta2 == delta);
        }
    }
};

static OSCTimeTagTests OSCTimeTagUnitTests;

#endif

} // namespace higgs
