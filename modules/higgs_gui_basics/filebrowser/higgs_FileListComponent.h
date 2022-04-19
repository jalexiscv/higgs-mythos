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
    A component that displays the files in a directory as a listbox.

    This implements the DirectoryContentsDisplayComponent base class so that
    it can be used in a FileBrowserComponent.

    To attach a listener to it, use its DirectoryContentsDisplayComponent base
    class and the FileBrowserListener class.

    @see DirectoryContentsList, FileTreeComponent

    @tags{GUI}
*/
class HIGGS_API  FileListComponent  : private ListBoxModel,
                                     public ListBox,
                                     public DirectoryContentsDisplayComponent,
                                     private ChangeListener
{
public:
    //==============================================================================
    /** Creates a listbox to show the contents of a specified directory. */
    FileListComponent (DirectoryContentsList& listToShow);

    /** Destructor. */
    ~FileListComponent() override;

    //==============================================================================
    /** Returns the number of files the user has got selected.
        @see getSelectedFile
    */
    int getNumSelectedFiles() const override;

    /** Returns one of the files that the user has currently selected.
        The index should be in the range 0 to (getNumSelectedFiles() - 1).
        @see getNumSelectedFiles
    */
    File getSelectedFile (int index = 0) const override;

    /** Deselects any files that are currently selected. */
    void deselectAllFiles() override;

    /** Scrolls to the top of the list. */
    void scrollToTop() override;

    /** If the specified file is in the list, it will become the only selected item
        (and if the file isn't in the list, all other items will be deselected). */
    void setSelectedFile (const File&) override;

private:
    //==============================================================================
    File lastDirectory, fileWaitingToBeSelected;
    class ItemComponent;

    void changeListenerCallback (ChangeBroadcaster*) override;
    int getNumRows() override;
    String getNameForRow (int rowNumber) override;
    void paintListBoxItem (int, Graphics&, int, int, bool) override;
    Component* refreshComponentForRow (int rowNumber, bool isRowSelected, Component*) override;
    void selectedRowsChanged (int row) override;
    void deleteKeyPressed (int currentSelectedRow) override;
    void returnKeyPressed (int currentSelectedRow) override;

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FileListComponent)
};

} // namespace higgs
