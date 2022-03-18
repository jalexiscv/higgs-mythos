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

/** A list of events that can be notified to any subscribed accessibility clients.

    To post a notification, call `AccessibilityHandler::notifyAccessibilityEvent`
    on the associated handler with the appropriate `AccessibilityEvent` type and
    listening clients will be notified.

    @tags{Accessibility}
*/
enum class AccessibilityEvent
{
    /** Indicates that the UI element's value has changed.

        This should be called on the handler that implements `AccessibilityValueInterface`
        for the UI element that has changed.
    */
    valueChanged,

    /** Indicates that the title of the UI element has changed.

        This should be called on the handler whose title has changed.
    */
    titleChanged,

    /** Indicates that the structure of the UI elements has changed in a
        significant way.

        This should be called on the top-level handler whose structure has changed.
    */
    structureChanged,

    /** Indicates that the selection of a text element has changed.

        This should be called on the handler that implements `AccessibilityTextInterface`
        for the text element that has changed.
    */
    textSelectionChanged,

    /** Indicates that the visible text of a text element has changed.

        This should be called on the handler that implements `AccessibilityTextInterface`
        for the text element that has changed.
    */
    textChanged,

    /** Indicates that the selection of rows in a list or table has changed.

        This should be called on the handler that implements `AccessibilityTableInterface`
        for the UI element that has changed.
    */
    rowSelectionChanged
};

} // namespace higgs
