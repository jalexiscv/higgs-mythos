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

#pragma once

#include "mythos_OpenDocumentManager.h"

//==============================================================================
class DocumentEditorComponent : public Component,
                                private OpenDocumentManager::DocumentCloseListener
{
public:
    //==============================================================================
    DocumentEditorComponent (OpenDocumentManager::Document* document);
    ~DocumentEditorComponent() override;

    OpenDocumentManager::Document* getDocument() const              { return document; }

protected:
    OpenDocumentManager::Document* document;
    bool lastEditedState = false;

    void setEditedState (bool hasBeenEdited);

private:
    bool documentAboutToClose (OpenDocumentManager::Document*) override;

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (DocumentEditorComponent)
};
