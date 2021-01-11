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
/**
    Acts as an application-wide logging class.

    A subclass of Logger can be created and passed into the Logger::setCurrentLogger
    method and this will then be used by all calls to writeToLog.

    The logger class also contains methods for writing messages to the debugger's
    output stream.

    @see FileLogger

    @tags{Core}
*/
class HIGGS_API  Logger
{
public:
    //==============================================================================
    /** Destructor. */
    virtual ~Logger();

    //==============================================================================
    /** Sets the current logging class to use.

        Note that the object passed in will not be owned or deleted by the logger, so
        the caller must make sure that it is not deleted while still being used.
        A null pointer can be passed-in to reset the system to the default logger.
    */
    static void HIGGS_CALLTYPE setCurrentLogger (Logger* newLogger) noexcept;

    /** Returns the current logger, or nullptr if no custom logger has been set. */
    static Logger* HIGGS_CALLTYPE getCurrentLogger() noexcept;

    /** Writes a string to the current logger.

        This will pass the string to the logger's logMessage() method if a logger
        has been set.

        @see logMessage
    */
    static void HIGGS_CALLTYPE writeToLog (const String& message);


    //==============================================================================
    /** Writes a message to the standard error stream.

        This can be called directly, or by using the DBG() macro in
        higgs_PlatformDefs.h (which will avoid calling the method in non-debug builds).
    */
    static void HIGGS_CALLTYPE outputDebugString (const String& text);


protected:
    //==============================================================================
    Logger();

    /** This is overloaded by subclasses to implement custom logging behaviour.
        @see setCurrentLogger
    */
    virtual void logMessage (const String& message) = 0;

private:
    static Logger* currentLogger;
};

} // namespace higgs
