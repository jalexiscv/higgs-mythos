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

namespace
{
    uint16 generateNoteID (int midiChannel, int midiNoteNumber) noexcept
    {
        jassert (midiChannel > 0 && midiChannel <= 16);
        jassert (midiNoteNumber >= 0 && midiNoteNumber < 128);

        return uint16 ((midiChannel << 7) + midiNoteNumber);
    }
}

//==============================================================================
MPENote::MPENote (int midiChannel_,
                  int initialNote_,
                  MPEValue noteOnVelocity_,
                  MPEValue pitchbend_,
                  MPEValue pressure_,
                  MPEValue timbre_,
                  KeyState keyState_) noexcept
    : noteID (generateNoteID (midiChannel_, initialNote_)),
      midiChannel (uint8 (midiChannel_)),
      initialNote (uint8 (initialNote_)),
      noteOnVelocity (noteOnVelocity_),
      pitchbend (pitchbend_),
      pressure (pressure_),
      initialTimbre (timbre_),
      timbre (timbre_),
      keyState (keyState_)
{
    jassert (keyState != MPENote::off);
    jassert (isValid());
}

MPENote::MPENote() noexcept {}

//==============================================================================
bool MPENote::isValid() const noexcept
{
    return midiChannel > 0 && midiChannel <= 16 && initialNote < 128;
}

//==============================================================================
double MPENote::getFrequencyInHertz (double frequencyOfA) const noexcept
{
    auto pitchInSemitones = double (initialNote) + totalPitchbendInSemitones;
    return frequencyOfA * std::pow (2.0, (pitchInSemitones - 69.0) / 12.0);
}

//==============================================================================
bool MPENote::operator== (const MPENote& other) const noexcept
{
    jassert (isValid() && other.isValid());
    return noteID == other.noteID;
}

bool MPENote::operator!= (const MPENote& other) const noexcept
{
    jassert (isValid() && other.isValid());
    return noteID != other.noteID;
}


//==============================================================================
//==============================================================================
#if HIGGS_UNIT_TESTS

class MPENoteTests final : public UnitTest
{
public:
    MPENoteTests()
        : UnitTest ("MPENote class", UnitTestCategories::midi)
    {}

    //==============================================================================
    void runTest() override
    {
        beginTest ("getFrequencyInHertz");
        {
            MPENote note;
            note.initialNote = 60;
            note.totalPitchbendInSemitones = -0.5;
            expectEqualsWithinOneCent (note.getFrequencyInHertz(), 254.178);
        }
    }

private:
    //==============================================================================
    void expectEqualsWithinOneCent (double frequencyInHertzActual,
                                    double frequencyInHertzExpected)
    {
        double ratio = frequencyInHertzActual / frequencyInHertzExpected;
        double oneCent = 1.0005946;
        expect (ratio < oneCent);
        expect (ratio > 1.0 / oneCent);
    }
};

static MPENoteTests MPENoteUnitTests;

#endif

} // namespace higgs
