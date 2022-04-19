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
    A listener for user selection events in a file browser.

    This is used by a FileBrowserComponent or FileListComponent.

    @tags{GUI}
*/
class HIGGS_API  FileBrowserListener
{
public:
    //==============================================================================
    /** Destructor. */
    virtual ~FileBrowserListener();

    //==============================================================================
    /** Callback when the user selects a different file in the browser. */
    virtual void selectionChanged() = 0;

    /** Callback when the user clicks on a file in the browser. */
    virtual void fileClicked (const File& file, const MouseEvent& e) = 0;

    /** Callback when the user double-clicks on a file in the browser. */
    virtual void fileDoubleClicked (const File& file) = 0;

    /** Callback when the browser's root folder changes. */
    virtual void browserRootChanged (const File& newRoot) = 0;
};

} // namespace higgs
