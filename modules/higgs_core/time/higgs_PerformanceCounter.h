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

//==============================================================================
/** A timer for measuring performance of code and dumping the results to a file.

    e.g. @code

        PerformanceCounter pc ("fish", 50, "/temp/myfishlog.txt");

        for (;;)
        {
            pc.start();

            doSomethingFishy();

            pc.stop();
        }
    @endcode

    In this example, the time of each period between calling start/stop will be
    measured and averaged over 50 runs, and the results printed to a file
    every 50 times round the loop.

    @tags{Core}
*/
class HIGGS_API  PerformanceCounter
{
public:
    //==============================================================================
    /** Creates a PerformanceCounter object.

        @param counterName      the name used when printing out the statistics
        @param runsPerPrintout  the number of start/stop iterations before calling
                                printStatistics()
        @param loggingFile      a file to dump the results to - if this is File(),
                                the results are just written to the debugger output
    */
    PerformanceCounter (const String& counterName,
                        int runsPerPrintout = 100,
                        const File& loggingFile = File());

    /** Destructor. */
    ~PerformanceCounter();

    //==============================================================================
    /** Starts timing.
        @see stop
    */
    void start() noexcept;

    /** Stops timing and prints out the results.

        The number of iterations before doing a printout of the
        results is set in the constructor.

        @see start
    */
    bool stop();

    /** Dumps the current metrics to the debugger output and to a file.

        As well as using Logger::outputDebugString to print the results,
        this will write then to the file specified in the constructor (if
        this was valid).
    */
    void printStatistics();

    /** Holds the current statistics. */
    struct Statistics
    {
        Statistics() noexcept;

        void clear() noexcept;
        String toString() const;

        void addResult (double elapsed) noexcept;

        String name;
        double averageSeconds;
        double maximumSeconds;
        double minimumSeconds;
        double totalSeconds;
        int64 numRuns;
    };

    /** Returns a copy of the current stats, and resets the internal counter. */
    Statistics getStatisticsAndReset();

private:
    //==============================================================================
    Statistics stats;
    int64 runsPerPrint, startTime;
    File outputFile;

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PerformanceCounter)
};


//==============================================================================
/**
    Simple RAII class for measuring the time spent in a scope.

    Example:

    {
        double timeSec;

        {
            ScopedTimeMeasurement m (timeSec);
            doSomething();
        }

        Logger::writeToLog ("doSomething() took " + String (timeSec) + "seconds");
    }

    @param resultInSeconds The result of the measurement will be stored in this variable.

    @tags{Core}
*/
class HIGGS_API [[nodiscard]] ScopedTimeMeasurement
{
public:
    ScopedTimeMeasurement (double& resultInSeconds) noexcept
        : result (resultInSeconds)
    {
        result = 0.0;
    }

    ~ScopedTimeMeasurement()
    {
        const auto now = Time::getHighResolutionTicks();
        result = Time::highResolutionTicksToSeconds (now - startTimeTicks);
    }

private:
    int64 startTimeTicks = Time::getHighResolutionTicks();
    double& result;

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ScopedTimeMeasurement)
};

} // namespace higgs
