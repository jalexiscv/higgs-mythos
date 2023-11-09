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

/** This wraps a context menu for a specific parameter, as provided by the host.

    You can choose to create a standard PopupMenu to display the host-provided
    options. Alternatively, you can ask the host to display a native menu at
    a specific location.

    @tags{Audio}
*/
struct HostProvidedContextMenu
{
    virtual ~HostProvidedContextMenu() = default;

    /** Get a PopupMenu holding entries specified by the host.

        Most hosts will populate this menu with options that relate to the
        parameter, such as displaying its automation lane. You are free
        to modify this menu before displaying it, if you wish to add additional
        options.
    */
    virtual PopupMenu getEquivalentPopupMenu() const = 0;

    /** Asks the host to display its native menu at a location relative
        to the top left corner of the editor.

        The position you provide should be in logical pixels. To display
        the menu next to the mouse cursor, call Component::getMouseXYRelative()
        on your editor and pass the result to this function.
    */
    virtual void showNativeMenu (Point<int> pos) const = 0;
};

/** Calling AudioProcessorEditor::getHostContext() may return a pointer to an
    instance of this class.

    At the moment, this can be used to retrieve context menus for parameters in
    compatible VST3 hosts. Additional extensions may be added here in the future.

    @tags{Audio}
*/
struct AudioProcessorEditorHostContext
{
    virtual ~AudioProcessorEditorHostContext() = default;

    /** Returns an object which can be used to display a context menu for the
        parameter with the given index.
    */
    virtual std::unique_ptr<HostProvidedContextMenu> getContextMenuForParameter (const AudioProcessorParameter *) const = 0;

    /** The naming of this function is misleading. Use getContextMenuForParameter() instead.

        Returns an object which can be used to display a context menu for the
        parameter with the given index.
    */
    [[deprecated ("The naming of this function has been fixed, use getContextMenuForParameter instead")]]
    virtual std::unique_ptr<HostProvidedContextMenu> getContextMenuForParameterIndex (const AudioProcessorParameter * p) const
    {
        return getContextMenuForParameter (p);
    }
};

} // namespace higgs
