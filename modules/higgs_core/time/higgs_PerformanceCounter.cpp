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

static void appendToFile (const File& f, const String& s)
{
    if (f.getFullPathName().isNotEmpty())
    {
        FileOutputStream out (f);

        if (! out.failedToOpen())
            out << s << newLine;
    }
}

PerformanceCounter::PerformanceCounter (const String& name, int runsPerPrintout, const File& loggingFile)
    : runsPerPrint (runsPerPrintout), startTime (0), outputFile (loggingFile)
{
    stats.name = name;
    appendToFile (outputFile, "**** Counter for \"" + name + "\" started at: " + Time::getCurrentTime().toString (true, true));
}

PerformanceCounter::~PerformanceCounter()
{
    if (stats.numRuns > 0)
        printStatistics();
}

PerformanceCounter::Statistics::Statistics() noexcept
    : averageSeconds(), maximumSeconds(), minimumSeconds(), totalSeconds(), numRuns()
{
}

void PerformanceCounter::Statistics::clear() noexcept
{
    averageSeconds = maximumSeconds = minimumSeconds = totalSeconds = 0;
    numRuns = 0;
}

void PerformanceCounter::Statistics::addResult (double elapsed) noexcept
{
    if (numRuns == 0)
    {
        maximumSeconds = elapsed;
        minimumSeconds = elapsed;
    }
    else
    {
        maximumSeconds = jmax (maximumSeconds, elapsed);
        minimumSeconds = jmin (minimumSeconds, elapsed);
    }

    ++numRuns;
    totalSeconds += elapsed;
}

static String timeToString (double secs)
{
    return String ((int64) (secs * (secs < 0.01 ? 1000000.0 : 1000.0) + 0.5))
                    + (secs < 0.01 ? " microsecs" : " millisecs");
}

String PerformanceCounter::Statistics::toString() const
{
    MemoryOutputStream s;

    s << "Performance count for \"" << name << "\" over " << numRuns << " run(s)" << newLine
      << "Average = "   << timeToString (averageSeconds)
      << ", minimum = " << timeToString (minimumSeconds)
      << ", maximum = " << timeToString (maximumSeconds)
      << ", total = "   << timeToString (totalSeconds);

    return s.toString();
}

void PerformanceCounter::start() noexcept
{
    startTime = Time::getHighResolutionTicks();
}

bool PerformanceCounter::stop()
{
    stats.addResult (Time::highResolutionTicksToSeconds (Time::getHighResolutionTicks() - startTime));

    if (stats.numRuns < runsPerPrint)
        return false;

    printStatistics();
    return true;
}

void PerformanceCounter::printStatistics()
{
    const String desc (getStatisticsAndReset().toString());

    Logger::writeToLog (desc);
    appendToFile (outputFile, desc);
}

PerformanceCounter::Statistics PerformanceCounter::getStatisticsAndReset()
{
    Statistics s (stats);
    stats.clear();

    if (s.numRuns > 0)
        s.averageSeconds = s.totalSeconds / (float) s.numRuns;

    return s;
}

} // namespace higgs
