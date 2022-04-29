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

FilenameComponent::FilenameComponent (const String& name,
                                      const File& currentFile,
                                      bool canEditFilename,
                                      bool isDirectory,
                                      bool isForSaving,
                                      const String& fileBrowserWildcard,
                                      const String& suffix,
                                      const String& textWhenNothingSelected)
    : Component (name),
      isDir (isDirectory),
      isSaving (isForSaving),
      wildcard (fileBrowserWildcard),
      enforcedSuffix (suffix)
{
    addAndMakeVisible (filenameBox);
    filenameBox.setEditableText (canEditFilename);
    filenameBox.setTextWhenNothingSelected (textWhenNothingSelected);
    filenameBox.setTextWhenNoChoicesAvailable (TRANS ("(no recently selected files)"));
    filenameBox.onChange = [this] { setCurrentFile (getCurrentFile(), true); };

    setBrowseButtonText ("...");

    setCurrentFile (currentFile, true, dontSendNotification);
}

FilenameComponent::~FilenameComponent()
{
}

//==============================================================================
void FilenameComponent::paintOverChildren (Graphics& g)
{
    if (isFileDragOver)
    {
        g.setColour (Colours::red.withAlpha (0.2f));
        g.drawRect (getLocalBounds(), 3);
    }
}

void FilenameComponent::resized()
{
    getLookAndFeel().layoutFilenameComponent (*this, &filenameBox, browseButton.get());
}

std::unique_ptr<ComponentTraverser> FilenameComponent::createKeyboardFocusTraverser()
{
    // This prevents the sub-components from grabbing focus if the
    // FilenameComponent has been set to refuse focus.
    return getWantsKeyboardFocus() ? Component::createKeyboardFocusTraverser() : nullptr;
}

void FilenameComponent::setBrowseButtonText (const String& newBrowseButtonText)
{
    browseButtonText = newBrowseButtonText;
    lookAndFeelChanged();
}

void FilenameComponent::lookAndFeelChanged()
{
    browseButton.reset();
    browseButton.reset (getLookAndFeel().createFilenameComponentBrowseButton (browseButtonText));
    addAndMakeVisible (browseButton.get());
    browseButton->setConnectedEdges (Button::ConnectedOnLeft);
    browseButton->onClick = [this] { showChooser(); };
    resized();
}

void FilenameComponent::setTooltip (const String& newTooltip)
{
    SettableTooltipClient::setTooltip (newTooltip);
    filenameBox.setTooltip (newTooltip);
}

void FilenameComponent::setDefaultBrowseTarget (const File& newDefaultDirectory)
{
    defaultBrowseFile = newDefaultDirectory;
}

File FilenameComponent::getLocationToBrowse()
{
    if (lastFilename.isEmpty() && defaultBrowseFile != File())
        return defaultBrowseFile;

    return getCurrentFile();
}

void FilenameComponent::showChooser()
{
    chooser = std::make_unique<FileChooser> (isDir ? TRANS ("Choose a new directory")
                                                   : TRANS ("Choose a new file"),
                                             getLocationToBrowse(),
                                             wildcard);

    auto chooserFlags = isDir ? FileBrowserComponent::openMode | FileBrowserComponent::canSelectDirectories
                              : FileBrowserComponent::canSelectFiles | (isSaving ? FileBrowserComponent::saveMode
                                                                                 : FileBrowserComponent::openMode);

    chooser->launchAsync (chooserFlags, [this] (const FileChooser&)
    {
        if (chooser->getResult() == File{})
            return;

        setCurrentFile (chooser->getResult(), true);
    });
}

bool FilenameComponent::isInterestedInFileDrag (const StringArray&)
{
    return true;
}

void FilenameComponent::filesDropped (const StringArray& filenames, int, int)
{
    isFileDragOver = false;
    repaint();

    const File f (filenames[0]);

    if (f.exists() && (f.isDirectory() == isDir))
        setCurrentFile (f, true);
}

void FilenameComponent::fileDragEnter (const StringArray&, int, int)
{
    isFileDragOver = true;
    repaint();
}

void FilenameComponent::fileDragExit (const StringArray&)
{
    isFileDragOver = false;
    repaint();
}

//==============================================================================
String FilenameComponent::getCurrentFileText() const
{
    return filenameBox.getText();
}

File FilenameComponent::getCurrentFile() const
{
    auto f = File::getCurrentWorkingDirectory().getChildFile (getCurrentFileText());

    if (enforcedSuffix.isNotEmpty())
        f = f.withFileExtension (enforcedSuffix);

    return f;
}

void FilenameComponent::setCurrentFile (File newFile,
                                        const bool addToRecentlyUsedList,
                                        NotificationType notification)
{
    if (enforcedSuffix.isNotEmpty())
        newFile = newFile.withFileExtension (enforcedSuffix);

    if (newFile.getFullPathName() != lastFilename)
    {
        lastFilename = newFile.getFullPathName();

        if (addToRecentlyUsedList)
            addRecentlyUsedFile (newFile);

        filenameBox.setText (lastFilename, dontSendNotification);

        if (notification != dontSendNotification)
        {
            triggerAsyncUpdate();

            if (notification == sendNotificationSync)
                handleUpdateNowIfNeeded();
        }
    }
}

void FilenameComponent::setFilenameIsEditable (const bool shouldBeEditable)
{
    filenameBox.setEditableText (shouldBeEditable);
}

StringArray FilenameComponent::getRecentlyUsedFilenames() const
{
    StringArray names;

    for (int i = 0; i < filenameBox.getNumItems(); ++i)
        names.add (filenameBox.getItemText (i));

    return names;
}

void FilenameComponent::setRecentlyUsedFilenames (const StringArray& filenames)
{
    if (filenames != getRecentlyUsedFilenames())
    {
        filenameBox.clear();

        for (int i = 0; i < jmin (filenames.size(), maxRecentFiles); ++i)
            filenameBox.addItem (filenames[i], i + 1);
    }
}

void FilenameComponent::setMaxNumberOfRecentFiles (const int newMaximum)
{
    maxRecentFiles = jmax (1, newMaximum);

    setRecentlyUsedFilenames (getRecentlyUsedFilenames());
}

void FilenameComponent::addRecentlyUsedFile (const File& file)
{
    auto files = getRecentlyUsedFilenames();

    if (file.getFullPathName().isNotEmpty())
    {
        files.removeString (file.getFullPathName(), true);
        files.insert (0, file.getFullPathName());

        setRecentlyUsedFilenames (files);
    }
}

//==============================================================================
void FilenameComponent::addListener (FilenameComponentListener* const listener)
{
    listeners.add (listener);
}

void FilenameComponent::removeListener (FilenameComponentListener* const listener)
{
    listeners.remove (listener);
}

void FilenameComponent::handleAsyncUpdate()
{
    Component::BailOutChecker checker (this);
    listeners.callChecked (checker, [this] (FilenameComponentListener& l) { l.filenameComponentChanged (this); });
}

} // namespace higgs
