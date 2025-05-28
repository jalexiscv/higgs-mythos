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

#include "../Application/mythos_Headers.h"
#include "mythos_DocumentEditorComponent.h"
#include "../Application/mythos_Application.h"
#include "../Project/UI/mythos_ProjectContentComponent.h"

//==============================================================================
DocumentEditorComponent::DocumentEditorComponent (OpenDocumentManager::Document* doc)
    : document (doc)
{
    MythosStudioApplication::getApp().openDocumentManager.addListener (this);
}

DocumentEditorComponent::~DocumentEditorComponent()
{
    MythosStudioApplication::getApp().openDocumentManager.removeListener (this);
}

bool DocumentEditorComponent::documentAboutToClose (OpenDocumentManager::Document* closingDoc)
{
    if (document == closingDoc)
    {
        jassert (document != nullptr);

        if (auto* pcc = findParentComponentOfClass<ProjectContentComponent>())
            pcc->hideDocument (document);
    }

    return true;
}

void DocumentEditorComponent::setEditedState (bool hasBeenEdited)
{
    if (hasBeenEdited != lastEditedState)
    {
        if (auto* pcc = findParentComponentOfClass<ProjectContentComponent>())
            pcc->refreshProjectTreeFileStatuses();

        lastEditedState = hasBeenEdited;
    }
}
