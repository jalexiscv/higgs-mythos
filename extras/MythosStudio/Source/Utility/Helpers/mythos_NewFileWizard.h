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

#include "../../Project/mythos_Project.h"

//==============================================================================
class NewFileWizard
{
public:
    //==============================================================================
    NewFileWizard();
    ~NewFileWizard();

    //==============================================================================
    class Type
    {
    public:
        virtual ~Type() = default;

        //==============================================================================
        virtual String getName() = 0;
        virtual void createNewFile (Project&, Project::Item projectGroupToAddTo) = 0;

        void showFailedToWriteMessage (const File& file);

    protected:
        //==============================================================================
        void askUserToChooseNewFile (const String& suggestedFilename, const String& wildcard,
                                     const Project::Item& projectGroupToAddTo,
                                     std::function<void (File)> callback);

    private:
        std::unique_ptr<FileChooser> chooser;
        ScopedMessageBox messageBox;
    };

    //==============================================================================
    void addWizardsToMenu (PopupMenu&) const;
    bool runWizardFromMenu (int chosenMenuItemID, Project&,
                            const Project::Item& projectGroupToAddTo) const;

    void registerWizard (Type*);

private:
    OwnedArray<Type> wizards;
};
