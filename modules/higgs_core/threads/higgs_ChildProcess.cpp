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

ChildProcess::ChildProcess() {}
ChildProcess::~ChildProcess() {}

bool ChildProcess::isRunning() const
{
    return activeProcess != nullptr && activeProcess->isRunning();
}

int ChildProcess::readProcessOutput (void* dest, int numBytes)
{
    return activeProcess != nullptr ? activeProcess->read (dest, numBytes) : 0;
}

bool ChildProcess::kill()
{
    return activeProcess == nullptr || activeProcess->killProcess();
}

uint32 ChildProcess::getExitCode() const
{
    return activeProcess != nullptr ? activeProcess->getExitCode() : 0;
}

bool ChildProcess::waitForProcessToFinish (const int timeoutMs) const
{
    auto timeoutTime = Time::getMillisecondCounter() + (uint32) timeoutMs;

    do
    {
        if (! isRunning())
            return true;

        Thread::sleep (2);
    }
    while (timeoutMs < 0 || Time::getMillisecondCounter() < timeoutTime);

    return false;
}

String ChildProcess::readAllProcessOutput()
{
    MemoryOutputStream result;

    for (;;)
    {
        char buffer[512];
        auto num = readProcessOutput (buffer, sizeof (buffer));

        if (num <= 0)
            break;

        result.write (buffer, (size_t) num);
    }

    return result.toString();
}


//==============================================================================
//==============================================================================
#if HIGGS_UNIT_TESTS

class ChildProcessTests final : public UnitTest
{
public:
    ChildProcessTests()
        : UnitTest ("ChildProcess", UnitTestCategories::threads)
    {}

    void runTest() override
    {
        beginTest ("Child Processes");

      #if HIGGS_WINDOWS || HIGGS_MAC || HIGGS_LINUX || HIGGS_BSD
        ChildProcess p;

       #if HIGGS_WINDOWS
        expect (p.start ("tasklist"));
       #else
        expect (p.start ("ls /"));
       #endif

        auto output = p.readAllProcessOutput();
        expect (output.isNotEmpty());
      #endif
    }
};

static ChildProcessTests childProcessUnitTests;

#endif

} // namespace higgs
