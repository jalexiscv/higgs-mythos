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

namespace higgs::dsp
{

static CommonSmoothedValueTests <LogRampedValue <float>> commonLogRampedValueTests;

class LogRampedValueTests final : public UnitTest
{
public:
    LogRampedValueTests()
        : UnitTest ("LogRampedValueTests", UnitTestCategories::dsp)
    {}

    void runTest() override
    {
        beginTest ("Curve");
        {
            Array<double> levels = { -0.12243, -1.21245, -12.2342, -22.4683, -30.0, -61.18753 };

            for (auto level : levels)
            {
                Array<Range<double>> ranges = { Range<double> (0.0,    1.0),
                                                Range<double> (-2.345, 0.0),
                                                Range<double> (-2.63,  3.56),
                                                Range<double> (3.3,    -0.2) };

                for (auto range : ranges)
                {
                    LogRampedValue<double> slowStart { range.getStart() } , fastStart { range.getEnd() };

                    auto numSamples = 12;
                    slowStart.reset (numSamples);
                    fastStart.reset (numSamples);

                    slowStart.setLogParameters (level, true);
                    fastStart.setLogParameters (level, false);

                    slowStart.setTargetValue (range.getEnd());
                    fastStart.setTargetValue (range.getStart());

                    AudioBuffer<double> results (2, numSamples + 1);

                    results.setSample (0, 0, slowStart.getCurrentValue());
                    results.setSample (1, 0, fastStart.getCurrentValue());

                    for (int i = 1; i < results.getNumSamples(); ++i)
                    {
                        results.setSample (0, i, slowStart.getNextValue());
                        results.setSample (1, i, fastStart.getNextValue());
                    }

                    for (int i = 0; i < results.getNumSamples(); ++i)
                        expectWithinAbsoluteError (results.getSample (0, i),
                                                   results.getSample (1, results.getNumSamples() - (i + 1)),
                                                   1.0e-7);

                    auto expectedMidpoint = range.getStart() + (range.getLength() * Decibels::decibelsToGain (level));
                    expectWithinAbsoluteError (results.getSample (0, numSamples / 2),
                                               expectedMidpoint,
                                               1.0e-7);
                }
            }
        }
    }
};

static LogRampedValueTests LogRampedValueTests;

} // namespace higgs::dsp
