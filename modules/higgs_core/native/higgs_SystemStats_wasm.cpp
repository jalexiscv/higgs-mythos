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

void Logger::outputDebugString (const String& text)
{
    std::cerr << text << std::endl;
}

//==============================================================================
SystemStats::OperatingSystemType SystemStats::getOperatingSystemType()  { return WASM; }
String SystemStats::getOperatingSystemName()    { return "WASM"; }
bool SystemStats::isOperatingSystem64Bit()      { return true; }
String SystemStats::getDeviceDescription()      { return "Web-browser"; }
String SystemStats::getDeviceManufacturer()     { return {}; }
String SystemStats::getCpuVendor()              { return {}; }
String SystemStats::getCpuModel()               { return {}; }
int SystemStats::getCpuSpeedInMegahertz()       { return 0; }
int SystemStats::getMemorySizeInMegabytes()     { return 0; }
int SystemStats::getPageSize()                  { return 0; }
String SystemStats::getLogonName()              { return {}; }
String SystemStats::getFullUserName()           { return {}; }
String SystemStats::getComputerName()           { return {}; }
String SystemStats::getUserLanguage()           { return {}; }
String SystemStats::getUserRegion()             { return {}; }
String SystemStats::getDisplayLanguage()        { return {}; }

//==============================================================================
void CPUInformation::initialise() noexcept
{
    numLogicalCPUs = 1;
    numPhysicalCPUs = 1;
}

//==============================================================================
uint32 higgs_millisecondsSinceStartup() noexcept
{
    return static_cast<uint32> (emscripten_get_now());
}

int64 Time::getHighResolutionTicks() noexcept
{
    return static_cast<int64> (emscripten_get_now() * 1'000.0);
}

int64 Time::getHighResolutionTicksPerSecond() noexcept
{
    return 1'000'000;  // (microseconds)
}

double Time::getMillisecondCounterHiRes() noexcept
{
    return emscripten_get_now();
}

bool Time::setSystemTimeToThisTime() const
{
    return false;
}

HIGGS_API bool HIGGS_CALLTYPE higgs_isRunningUnderDebugger() noexcept
{
    return false;
}

} // namespace higgs
