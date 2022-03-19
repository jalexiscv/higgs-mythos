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

HIGGSApplication::HIGGSApplication() {}
HIGGSApplication::~HIGGSApplication() {}

//==============================================================================
HIGGSApplication* HIGGS_CALLTYPE HIGGSApplication::getInstance() noexcept
{
    return dynamic_cast<HIGGSApplication*> (HIGGSApplicationBase::getInstance());
}

bool HIGGSApplication::moreThanOneInstanceAllowed()  { return true; }
void HIGGSApplication::anotherInstanceStarted (const String&) {}

void HIGGSApplication::suspended() {}
void HIGGSApplication::resumed() {}

void HIGGSApplication::systemRequestedQuit()         { quit(); }

void HIGGSApplication::unhandledException (const std::exception*, const String&, int)
{
    jassertfalse;
}

//==============================================================================
ApplicationCommandTarget* HIGGSApplication::getNextCommandTarget()
{
    return nullptr;
}

void HIGGSApplication::getAllCommands (Array<CommandID>& commands)
{
    commands.add (StandardApplicationCommandIDs::quit);
}

void HIGGSApplication::getCommandInfo (const CommandID commandID, ApplicationCommandInfo& result)
{
    if (commandID == StandardApplicationCommandIDs::quit)
    {
        result.setInfo (TRANS ("Quit"),
                        TRANS ("Quits the application"),
                        "Application", 0);

        result.defaultKeypresses.add (KeyPress ('q', ModifierKeys::commandModifier, 0));
    }
}

bool HIGGSApplication::perform (const InvocationInfo& info)
{
    if (info.commandID == StandardApplicationCommandIDs::quit)
    {
        systemRequestedQuit();
        return true;
    }

    return false;
}

//==============================================================================
bool HIGGSApplication::initialiseApp()
{
    if (HIGGSApplicationBase::initialiseApp())
    {
       #if HIGGS_MAC
        initialiseMacMainMenu(); // (needs to get the app's name)
       #endif

        return true;
    }

    return false;
}

} // namespace higgs
