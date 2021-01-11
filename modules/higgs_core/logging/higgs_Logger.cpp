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

Logger::Logger() {}

Logger::~Logger()
{
    // You're deleting this logger while it's still being used!
    // Always call Logger::setCurrentLogger (nullptr) before deleting the active logger.
    jassert (currentLogger != this);
}

Logger* Logger::currentLogger = nullptr;

void Logger::setCurrentLogger (Logger* const newLogger) noexcept    { currentLogger = newLogger; }
Logger* Logger::getCurrentLogger()  noexcept                        { return currentLogger; }

void Logger::writeToLog (const String& message)
{
    if (currentLogger != nullptr)
        currentLogger->logMessage (message);
    else
        outputDebugString (message);
}

#if HIGGS_LOG_ASSERTIONS || HIGGS_DEBUG
void HIGGS_API HIGGS_CALLTYPE logAssertion (const char* const filename, const int lineNum) noexcept
{
    String m ("HIGGS Assertion failure in ");
    m << File::createFileWithoutCheckingPath (CharPointer_UTF8 (filename)).getFileName() << ':' << lineNum;

   #if HIGGS_LOG_ASSERTIONS
    Logger::writeToLog (m);
   #else
    DBG (m);
   #endif
}
#endif

} // namespace higgs
