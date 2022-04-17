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

DirectoryContentsDisplayComponent::DirectoryContentsDisplayComponent (DirectoryContentsList& l)
    : directoryContentsList (l)
{
}

DirectoryContentsDisplayComponent::~DirectoryContentsDisplayComponent()
{
}

//==============================================================================
FileBrowserListener::~FileBrowserListener()
{
}

void DirectoryContentsDisplayComponent::addListener (FileBrowserListener* l)    { listeners.add (l); }
void DirectoryContentsDisplayComponent::removeListener (FileBrowserListener* l) { listeners.remove (l); }

void DirectoryContentsDisplayComponent::sendSelectionChangeMessage()
{
    Component::BailOutChecker checker (dynamic_cast<Component*> (this));
    listeners.callChecked (checker, [] (FileBrowserListener& l) { l.selectionChanged(); });
}

void DirectoryContentsDisplayComponent::sendMouseClickMessage (const File& file, const MouseEvent& e)
{
    if (directoryContentsList.getDirectory().exists())
    {
        Component::BailOutChecker checker (dynamic_cast<Component*> (this));
        listeners.callChecked (checker, [&] (FileBrowserListener& l) { l.fileClicked (file, e); });
    }
}

void DirectoryContentsDisplayComponent::sendDoubleClickMessage (const File& file)
{
    if (directoryContentsList.getDirectory().exists())
    {
        Component::BailOutChecker checker (dynamic_cast<Component*> (this));
        listeners.callChecked (checker, [&] (FileBrowserListener& l) { l.fileDoubleClicked (file); });
    }
}

} // namespace higgs
