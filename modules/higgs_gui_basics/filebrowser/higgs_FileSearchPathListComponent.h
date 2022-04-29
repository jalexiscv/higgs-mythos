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
    Shows a set of file paths in a list, allowing them to be added, removed or
    re-ordered.

    @see FileSearchPath

    @tags{GUI}
*/
class HIGGS_API  FileSearchPathListComponent  : public Component,
                                               public SettableTooltipClient,
                                               public FileDragAndDropTarget,
                                               private ListBoxModel
{
public:
    //==============================================================================
    /** Creates an empty FileSearchPathListComponent. */
    FileSearchPathListComponent();

    //==============================================================================
    /** Returns the path as it is currently shown. */
    const FileSearchPath& getPath() const noexcept                  { return path; }

    /** Changes the current path. */
    void setPath (const FileSearchPath& newPath);

    /** Sets a file or directory to be the default starting point for the browser to show.

        This is only used if the current file hasn't been set.
    */
    void setDefaultBrowseTarget (const File& newDefaultDirectory);

    /** A set of colour IDs to use to change the colour of various aspects of the label.

        These constants can be used either via the Component::setColour(), or LookAndFeel::setColour()
        methods.

        @see Component::setColour, Component::findColour, LookAndFeel::setColour, LookAndFeel::findColour
    */
    enum ColourIds
    {
        backgroundColourId      = 0x1004100, /**< The background colour to fill the component with.
                                                  Make this transparent if you don't want the background to be filled. */
    };

    //==============================================================================
    /** @internal */
    int getNumRows() override;
    /** @internal */
    void paintListBoxItem (int rowNumber, Graphics& g, int width, int height, bool rowIsSelected) override;
    /** @internal */
    void deleteKeyPressed (int lastRowSelected) override;
    /** @internal */
    void returnKeyPressed (int lastRowSelected) override;
    /** @internal */
    void listBoxItemDoubleClicked (int row, const MouseEvent&) override;
    /** @internal */
    void selectedRowsChanged (int lastRowSelected) override;
    /** @internal */
    void resized() override;
    /** @internal */
    void paint (Graphics&) override;
    /** @internal */
    bool isInterestedInFileDrag (const StringArray&) override;
    /** @internal */
    void filesDropped (const StringArray& files, int, int) override;

private:
    //==============================================================================
    FileSearchPath path;
    File defaultBrowseTarget;
    std::unique_ptr<FileChooser> chooser;

    ListBox listBox;
    TextButton addButton, removeButton, changeButton;
    DrawableButton upButton, downButton;

    void changed();
    void updateButtons();

    void addPath();
    void deleteSelected();
    void editSelected();
    void moveSelection (int);

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FileSearchPathListComponent)
};

} // namespace higgs
