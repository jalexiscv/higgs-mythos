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

/** An abstract interface which represents a UI element that supports a text interface.

    A UI element can use this interface to provide extended textual information which
    cannot be conveyed using just the title, description, and help text properties of
    AccessibilityHandler. This is typically for text that an accessibility client might
    want to read line-by-line, or provide text selection and input for.

    @tags{Accessibility}
*/
class HIGGS_API  AccessibilityTextInterface
{
public:
    /** Destructor. */
    virtual ~AccessibilityTextInterface() = default;

    /** Returns true if the text being displayed is protected and should not be
        exposed to the user, for example a password entry field.
    */
    virtual bool isDisplayingProtectedText() const = 0;

    /** Returns true if the text being displayed is read-only or false if editable. */
    virtual bool isReadOnly() const = 0;

    /** Returns the total number of characters in the text element. */
    virtual int getTotalNumCharacters() const = 0;

    /** Returns the range of characters that are currently selected, or an empty
        range if nothing is selected.
    */
    virtual Range<int> getSelection() const = 0;

    /** Selects a section of the text. */
    virtual void setSelection (Range<int> newRange) = 0;

    /** Gets the current text insertion position, if supported. */
    virtual int getTextInsertionOffset() const = 0;

    /** Returns a section of text. */
    virtual String getText (Range<int> range) const = 0;

    /** Returns the full text. */
    String getAllText() const { return getText ({ 0, getTotalNumCharacters() }); }

    /** Replaces the text with a new string. */
    virtual void setText (const String& newText) = 0;

    /** Returns the bounding box in screen coordinates for a range of text.
        As the range may span multiple lines, this method returns a RectangleList.
    */
    virtual RectangleList<int> getTextBounds (Range<int> textRange) const = 0;

    /** Returns the index of the character at a given position in screen coordinates. */
    virtual int getOffsetAtPoint (Point<int> point) const = 0;
};

} // namespace higgs
