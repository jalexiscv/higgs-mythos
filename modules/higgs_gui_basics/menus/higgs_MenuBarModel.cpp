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

MenuBarModel::MenuBarModel() noexcept
    : manager (nullptr)
{
}

MenuBarModel::~MenuBarModel()
{
    setApplicationCommandManagerToWatch (nullptr);
}

//==============================================================================
void MenuBarModel::menuItemsChanged()
{
    triggerAsyncUpdate();
}

void MenuBarModel::setApplicationCommandManagerToWatch (ApplicationCommandManager* newManager)
{
    if (manager != newManager)
    {
        if (manager != nullptr)
            manager->removeListener (this);

        manager = newManager;

        if (manager != nullptr)
            manager->addListener (this);
    }
}

void MenuBarModel::addListener (Listener* newListener)
{
    listeners.add (newListener);
}

void MenuBarModel::removeListener (Listener* listenerToRemove)
{
    // Trying to remove a listener that isn't on the list!
    // If this assertion happens because this object is a dangling pointer, make sure you've not
    // deleted this menu model while it's still being used by something (e.g. by a MenuBarComponent)
    jassert (listeners.contains (listenerToRemove));

    listeners.remove (listenerToRemove);
}

//==============================================================================
void MenuBarModel::handleAsyncUpdate()
{
    listeners.call ([this] (Listener& l) { l.menuBarItemsChanged (this); });
}

void MenuBarModel::applicationCommandInvoked (const ApplicationCommandTarget::InvocationInfo& info)
{
    listeners.call ([this, &info] (Listener& l) { l.menuCommandInvoked (this, info); });
}

void MenuBarModel::applicationCommandListChanged()
{
    menuItemsChanged();
}

void MenuBarModel::handleMenuBarActivate (bool isActive)
{
    menuBarActivated (isActive);
    listeners.call ([this, isActive] (Listener& l) { l.menuBarActivated (this, isActive); });
}

void MenuBarModel::menuBarActivated (bool) {}
void MenuBarModel::Listener::menuBarActivated (MenuBarModel*, bool) {}

} // namespace higgs
