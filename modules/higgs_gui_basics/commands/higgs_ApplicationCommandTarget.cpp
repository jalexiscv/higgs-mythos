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

class ApplicationCommandTarget::CommandMessage final : public MessageManager::MessageBase
{
public:
    CommandMessage (ApplicationCommandTarget* const target, const InvocationInfo& inf)
        : owner (target), info (inf)
    {
    }

    void messageCallback() override
    {
        if (ApplicationCommandTarget* const target = owner)
            target->tryToInvoke (info, false);
    }

private:
    WeakReference<ApplicationCommandTarget> owner;
    const InvocationInfo info;

    HIGGS_DECLARE_NON_COPYABLE (CommandMessage)
};

//==============================================================================
ApplicationCommandTarget::ApplicationCommandTarget() {}
ApplicationCommandTarget::~ApplicationCommandTarget() {}

//==============================================================================
bool ApplicationCommandTarget::tryToInvoke (const InvocationInfo& info, const bool async)
{
    if (isCommandActive (info.commandID))
    {
        if (async)
        {
            (new CommandMessage (this, info))->post();
            return true;
        }

        if (perform (info))
            return true;

        // Your target claimed that it could perform this command, but failed to do so.
        // If it can't do it at the moment for some reason, it should clear the 'isActive' flag
        // when it returns the command's info.
        jassertfalse;
    }

    return false;
}

ApplicationCommandTarget* ApplicationCommandTarget::findFirstTargetParentComponent()
{
    if (Component* const c = dynamic_cast<Component*> (this))
        return c->findParentComponentOfClass<ApplicationCommandTarget>();

    return nullptr;
}

ApplicationCommandTarget* ApplicationCommandTarget::getTargetForCommand (const CommandID commandID)
{
    ApplicationCommandTarget* target = this;
    int depth = 0;

    while (target != nullptr)
    {
        Array<CommandID> commandIDs;
        target->getAllCommands (commandIDs);

        if (commandIDs.contains (commandID))
            return target;

        target = target->getNextCommandTarget();

        ++depth;
        jassert (depth < 100); // could be a recursive command chain??
        jassert (target != this); // definitely a recursive command chain!

        if (depth > 100 || target == this)
            break;
    }

    if (target == nullptr)
    {
        target = HIGGSApplication::getInstance();

        if (target != nullptr)
        {
            Array<CommandID> commandIDs;
            target->getAllCommands (commandIDs);

            if (commandIDs.contains (commandID))
                return target;
        }
    }

    return nullptr;
}

bool ApplicationCommandTarget::isCommandActive (const CommandID commandID)
{
    ApplicationCommandInfo info (commandID);
    info.flags = ApplicationCommandInfo::isDisabled;

    getCommandInfo (commandID, info);

    return (info.flags & ApplicationCommandInfo::isDisabled) == 0;
}

//==============================================================================
bool ApplicationCommandTarget::invoke (const InvocationInfo& info, const bool async)
{
    ApplicationCommandTarget* target = this;
    int depth = 0;

    while (target != nullptr)
    {
        if (target->tryToInvoke (info, async))
            return true;

        target = target->getNextCommandTarget();

        ++depth;
        jassert (depth < 100); // could be a recursive command chain??
        jassert (target != this); // definitely a recursive command chain!

        if (depth > 100 || target == this)
            break;
    }

    if (target == nullptr)
    {
        target = HIGGSApplication::getInstance();

        if (target != nullptr)
            return target->tryToInvoke (info, async);
    }

    return false;
}

bool ApplicationCommandTarget::invokeDirectly (const CommandID commandID, const bool asynchronously)
{
    ApplicationCommandTarget::InvocationInfo info (commandID);
    info.invocationMethod = ApplicationCommandTarget::InvocationInfo::direct;

    return invoke (info, asynchronously);
}

//==============================================================================
ApplicationCommandTarget::InvocationInfo::InvocationInfo (const CommandID command)
    : commandID (command),
      commandFlags (0),
      invocationMethod (direct),
      originatingComponent (nullptr),
      isKeyDown (false),
      millisecsSinceKeyPressed (0)
{
}

} // namespace higgs
