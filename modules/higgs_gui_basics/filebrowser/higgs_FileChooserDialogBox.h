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
    A file open/save dialog box.

    This is a Higgs-based file dialog box; to use a native file chooser, see the
    FileChooser class.

    @code
    {
        wildcardFilter = std::make_unique<WildcardFileFilter> ("*.foo", String(), "Foo files");

        browser = std::make_unique<FileBrowserComponent> (FileBrowserComponent::canSelectFiles,
                                                          File(),
                                                          wildcardFilter.get(),
                                                          nullptr);

        dialogBox = std::make_unique<FileChooserDialogBox> ("Open some kind of file",
                                                            "Please choose some kind of file that you want to open...",
                                                            *browser,
                                                            false,
                                                            Colours::lightgrey);

        auto onFileSelected = [this] (int r)
        {
            modalStateFinished (r);

            auto selectedFile = browser->getSelectedFile (0);

            ...etc...
        };

        dialogBox->centreWithDefaultSize (nullptr);
        dialogBox->enterModalState (true,
                                    ModalCallbackFunction::create (onFileSelected),
                                    false);
    }
    @endcode

    @see FileChooser

    @tags{GUI}
*/
class HIGGS_API  FileChooserDialogBox : public ResizableWindow,
                                       private FileBrowserListener
{
public:
    //==============================================================================
    /** Creates a file chooser box.

        @param title            the main title to show at the top of the box
        @param instructions     an optional longer piece of text to show below the title in
                                a smaller font, describing in more detail what's required.
        @param browserComponent a FileBrowserComponent that will be shown inside this dialog
                                box. Make sure you delete this after (but not before!) the
                                dialog box has been deleted.
        @param warnAboutOverwritingExistingFiles     if true, then the user will be asked to confirm
                                if they try to select a file that already exists. (This
                                flag is only used when saving files)
        @param backgroundColour the background colour for the top level window
        @param parentComponent  an optional component which should be the parent
                                for the file chooser. If this is a nullptr then the
                                dialog box will be a top-level window. AUv3s on iOS
                                must specify this parameter as opening a top-level window
                                in an AUv3 is forbidden due to sandbox restrictions.

        @see FileBrowserComponent, FilePreviewComponent
    */
    FileChooserDialogBox (const String& title,
                          const String& instructions,
                          FileBrowserComponent& browserComponent,
                          bool warnAboutOverwritingExistingFiles,
                          Colour backgroundColour,
                          Component* parentComponent = nullptr);

    /** Destructor. */
    ~FileChooserDialogBox() override;

    //==============================================================================
   #if HIGGS_MODAL_LOOPS_PERMITTED
    /** Displays and runs the dialog box modally.

        This will show the box with the specified size, returning true if the user
        pressed 'ok', or false if they cancelled.

        Leave the width or height as 0 to use the default size
    */
    bool show (int width = 0, int height = 0);

    /** Displays and runs the dialog box modally.

        This will show the box with the specified size at the specified location,
        returning true if the user pressed 'ok', or false if they cancelled.

        Leave the width or height as 0 to use the default size.
    */
    bool showAt (int x, int y, int width, int height);
   #endif

    /** Sets the size of this dialog box to its default and positions it either in the
        centre of the screen, or centred around a component that is provided.
    */
    void centreWithDefaultSize (Component* componentToCentreAround = nullptr);

    //==============================================================================
    /** A set of colour IDs to use to change the colour of various aspects of the box.

        These constants can be used either via the Component::setColour(), or LookAndFeel::setColour()
        methods.

        @see Component::setColour, Component::findColour, LookAndFeel::setColour, LookAndFeel::findColour
    */
    enum ColourIds
    {
        titleTextColourId      = 0x1000850, /**< The colour to use to draw the box's title. */
    };

private:
    class ContentComponent;
    ContentComponent* content;
    const bool warnAboutOverwritingExistingFiles;

    void closeButtonPressed();
    void selectionChanged() override;
    void fileClicked (const File&, const MouseEvent&) override;
    void fileDoubleClicked (const File&) override;
    void browserRootChanged (const File&) override;
    int getDefaultWidth() const;

    void okButtonPressed();
    void createNewFolder();
    void createNewFolderConfirmed (const String& name);

    static void createNewFolderCallback (int result, FileChooserDialogBox*, Component::SafePointer<AlertWindow>);

    ScopedMessageBox messageBox;

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (FileChooserDialogBox)
};

} // namespace higgs
