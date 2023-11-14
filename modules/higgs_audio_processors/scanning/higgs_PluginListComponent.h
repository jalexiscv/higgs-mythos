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
    A component displaying a list of plugins, with options to scan for them,
    add, remove and sort them.

    @tags{Audio}
*/
class HIGGS_API  PluginListComponent   : public Component,
                                        public FileDragAndDropTarget,
                                        private ChangeListener
{
public:
    //==============================================================================
    /**
        Creates the list component.

        For info about the deadMansPedalFile, see the PluginDirectoryScanner constructor.
        The properties file, if supplied, is used to store the user's last search paths.
    */
    PluginListComponent (AudioPluginFormatManager& formatManager,
                         KnownPluginList& listToRepresent,
                         const File& deadMansPedalFile,
                         PropertiesFile* propertiesToUse,
                         bool allowPluginsWhichRequireAsynchronousInstantiation = false);

    /** Destructor. */
    ~PluginListComponent() override;

    /** Changes the text in the panel's options button. */
    void setOptionsButtonText (const String& newText);

    /** Returns a pop-up menu that contains all the options for scanning and updating the list. */
    PopupMenu createOptionsMenu();

    /** Returns a menu that can be shown if a row is right-clicked, containing actions
        like "remove plugin" or "show folder" etc.
    */
    PopupMenu createMenuForRow (int rowNumber);

    /** Changes the text in the progress dialog box that is shown when scanning. */
    void setScanDialogText (const String& textForProgressWindowTitle,
                            const String& textForProgressWindowDescription);

    /** Sets how many threads to simultaneously scan for plugins.
        If this is 0, then all scanning happens on the message thread (this is the default when
        allowPluginsWhichRequireAsynchronousInstantiation is false). If
        allowPluginsWhichRequireAsynchronousInstantiation is true then numThreads must not
        be zero (it is one by default).
    */
    void setNumberOfThreadsForScanning (int numThreads);

    /** Returns the last search path stored in a given properties file for the specified format. */
    static FileSearchPath getLastSearchPath (PropertiesFile&, AudioPluginFormat&);

    /** Stores a search path in a properties file for the given format. */
    static void setLastSearchPath (PropertiesFile&, AudioPluginFormat&, const FileSearchPath&);

    /** Triggers an asynchronous scan for the given format. */
    void scanFor (AudioPluginFormat&);

    /** Triggers an asynchronous scan for the given format and scans only the given files or identifiers.
        @see AudioPluginFormat::searchPathsForPlugins
    */
    void scanFor (AudioPluginFormat&, const StringArray& filesOrIdentifiersToScan);

    /** Returns true if there's currently a scan in progress. */
    bool isScanning() const noexcept;

    /** Removes the plugins currently selected in the table. */
    void removeSelectedPlugins();

    /** Sets a custom table model to be used.
        This will take ownership of the model and delete it when no longer needed.
     */
    void setTableModel (TableListBoxModel*);

    /** Returns the table used to display the plugin list. */
    TableListBox& getTableListBox() noexcept            { return table; }

    /** Returns the button used to display the options menu - you can make this invisible
        if you want to hide it and use some other method for showing the menu.
    */
    TextButton& getOptionsButton()                      { return optionsButton; }

    /** @internal */
    void resized() override;

private:
    //==============================================================================
    AudioPluginFormatManager& formatManager;
    KnownPluginList& list;
    File deadMansPedalFile;
    TableListBox table;
    TextButton optionsButton;
    PropertiesFile* propertiesToUse;
    String dialogTitle, dialogText;
    bool allowAsync;
    int numThreads;

    class TableModel;
    std::unique_ptr<TableListBoxModel> tableModel;

    class Scanner;
    std::shared_ptr<Scanner> currentScanner;

    ScopedMessageBox messageBox;

    void scanFinished (const StringArray&, const std::vector<String>&);
    void updateList();
    void removeMissingPlugins();
    void removePluginItem (int index);

    bool isInterestedInFileDrag (const StringArray&) override;
    void filesDropped (const StringArray&, int, int) override;
    void changeListenerCallback (ChangeBroadcaster*) override;

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PluginListComponent)
};

} // namespace higgs
