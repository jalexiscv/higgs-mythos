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

/** An abstract interface which represents a UI element that supports a table interface.

    Examples of UI elements which typically support a table interface are lists, tables,
    and trees.

    @tags{Accessibility}
*/
class HIGGS_API  AccessibilityTableInterface
{
public:
    /** Destructor. */
    virtual ~AccessibilityTableInterface() = default;

    /** Returns the total number of rows in the table. */
    virtual int getNumRows() const = 0;

    /** Returns the total number of columns in the table. */
    virtual int getNumColumns() const = 0;

    /** Returns the AccessibilityHandler for one of the cells in the table, or
        nullptr if there is no cell at the specified position.
    */
    virtual const AccessibilityHandler* getCellHandler (int row, int column) const = 0;

    /** Returns the AccessibilityHandler for a row in the table, or nullptr if there is
        no row at this index.

        The row component should have a child component for each column in the table.
    */
    virtual const AccessibilityHandler* getRowHandler (int row) const = 0;

    /** Returns the AccessibilityHandler for the header, or nullptr if there is
        no header.

        If you supply a header, it must have exactly the same number of children
        as there are columns in the table.
    */
    virtual const AccessibilityHandler* getHeaderHandler() const = 0;

    /** A simple span of elements. */
    struct Span { int begin, num; };

    /** Given the handler of one of the cells in the table, returns the rows covered
        by that cell, or null if the cell does not exist in the table.

        This function replaces the getRowIndex and getRowSpan
        functions from AccessibilityCellInterface. Most of the time, it's easier for the
        table itself to keep track of cell locations, than to delegate to the individual
        cells.
    */
    virtual Optional<Span> getRowSpan (const AccessibilityHandler&) const = 0;

    /** Given the handler of one of the cells in the table, returns the columns covered
        by that cell, or null if the cell does not exist in the table.

        This function replaces the getColumnIndex and getColumnSpan
        functions from AccessibilityCellInterface. Most of the time, it's easier for the
        table itself to keep track of cell locations, than to delegate to the individual
        cells.
    */
    virtual Optional<Span> getColumnSpan (const AccessibilityHandler&) const = 0;

    /** Attempts to scroll the table (if necessary) so that the cell with the given handler
        is visible.
    */
    virtual void showCell (const AccessibilityHandler&) const = 0;
};

} // namespace higgs
