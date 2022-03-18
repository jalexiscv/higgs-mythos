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

/** The list of available roles for an AccessibilityHandler object.

    When creating a custom AccessibilityHandler you should select the role that
    best describes the UI element being represented.

    @tags{Accessibility}
*/
enum class AccessibilityRole
{
    button,
    toggleButton,
    radioButton,
    comboBox,
    image,
    slider,
    label,
    staticText,
    editableText,
    menuItem,
    menuBar,
    popupMenu,
    table,
    tableHeader,
    column,
    row,
    cell,
    hyperlink,
    list,
    listItem,
    tree,
    treeItem,
    progressBar,
    group,
    dialogWindow,
    window,
    scrollBar,
    tooltip,
    splashScreen,
    ignored,
    unspecified
};

} // namespace higgs
