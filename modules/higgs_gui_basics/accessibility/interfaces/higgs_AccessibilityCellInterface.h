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

/** An abstract interface which represents a UI element that supports a cell interface.

    This typically represents a single cell inside of a UI element which implements an
    AccessibilityTableInterface.

    @tags{Accessibility}
*/
class HIGGS_API  AccessibilityCellInterface
{
public:
    /** Destructor. */
    virtual ~AccessibilityCellInterface() = default;

    /** Returns the indentation level for the cell. */
    virtual int getDisclosureLevel() const = 0;

    /** Returns the AccessibilityHandler of the table which contains the cell. */
    virtual const AccessibilityHandler* getTableHandler() const = 0;

    /** Returns a list of the accessibility elements that are disclosed by this element, if any. */
    virtual std::vector<const AccessibilityHandler*> getDisclosedRows() const { return {}; }
};

} // namespace higgs
