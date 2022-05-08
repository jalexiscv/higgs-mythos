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

GridItem::Property::Property() noexcept : isAuto (true)
{
}

GridItem::Property::Property (GridItem::Keyword keyword) noexcept : isAuto (keyword == GridItem::Keyword::autoValue)
{
    jassert (keyword == GridItem::Keyword::autoValue);
}

GridItem::Property::Property (const char* lineNameToUse) noexcept : GridItem::Property (String (lineNameToUse))
{
}

GridItem::Property::Property (const String& lineNameToUse) noexcept : name (lineNameToUse), number (1)
{
}

GridItem::Property::Property (int numberToUse) noexcept : number (numberToUse)
{
}

GridItem::Property::Property (int numberToUse, const String& lineNameToUse) noexcept
    : name (lineNameToUse), number (numberToUse)
{
}

GridItem::Property::Property (Span spanToUse) noexcept
    : name (spanToUse.name), number (spanToUse.number), isSpan (true)
{
}


//==============================================================================
GridItem::Margin::Margin() noexcept : left(), right(), top(), bottom() {}

GridItem::Margin::Margin (int v) noexcept : GridItem::Margin::Margin (static_cast<float> (v)) {}

GridItem::Margin::Margin (float v) noexcept : left (v), right (v), top (v), bottom (v) {}

GridItem::Margin::Margin (float t, float r, float b, float l) noexcept : left (l), right (r), top (t), bottom (b) {}

//==============================================================================
GridItem::GridItem() noexcept = default;
GridItem::GridItem (Component& componentToUse) noexcept  : associatedComponent (&componentToUse) {}
GridItem::GridItem (Component* componentToUse) noexcept  : associatedComponent (componentToUse) {}

void GridItem::setArea (Property rowStart, Property columnStart, Property rowEnd, Property columnEnd)
{
    column.start = columnStart;
    column.end = columnEnd;
    row.start = rowStart;
    row.end = rowEnd;
}

void GridItem::setArea (Property rowStart, Property columnStart)
{
    column.start = columnStart;
    row.start = rowStart;
}

void GridItem::setArea (const String& areaName)
{
    area = areaName;
}

GridItem GridItem::withArea (Property rowStart, Property columnStart, Property rowEnd, Property columnEnd) const noexcept
{
    auto gi = *this;
    gi.setArea (rowStart, columnStart, rowEnd, columnEnd);
    return gi;
}

GridItem GridItem::withArea (Property rowStart, Property columnStart) const noexcept
{
    auto gi = *this;
    gi.setArea (rowStart, columnStart);
    return gi;
}

GridItem GridItem::withArea (const String& areaName) const noexcept
{
    auto gi = *this;
    gi.setArea (areaName);
    return gi;
}

GridItem GridItem::withRow (StartAndEndProperty newRow) const noexcept
{
    auto gi = *this;
    gi.row = newRow;
    return gi;
}

GridItem GridItem::withColumn (StartAndEndProperty newColumn) const noexcept
{
    auto gi = *this;
    gi.column = newColumn;
    return gi;
}

GridItem GridItem::withAlignSelf (AlignSelf newAlignSelf) const noexcept
{
    auto gi = *this;
    gi.alignSelf = newAlignSelf;
    return gi;
}

GridItem GridItem::withJustifySelf (JustifySelf newJustifySelf) const noexcept
{
    auto gi = *this;
    gi.justifySelf = newJustifySelf;
    return gi;
}

GridItem GridItem::withWidth (float newWidth) const noexcept
{
    auto gi = *this;
    gi.width = newWidth;
    return gi;
}

GridItem GridItem::withHeight (float newHeight) const noexcept
{
    auto gi = *this;
    gi.height = newHeight;
    return gi;
}

GridItem GridItem::withSize (float newWidth, float newHeight) const noexcept
{
    auto gi = *this;
    gi.width = newWidth;
    gi.height = newHeight;
    return gi;
}

GridItem GridItem::withMargin (Margin newHeight) const noexcept
{
    auto gi = *this;
    gi.margin = newHeight;
    return gi;
}

GridItem GridItem::withOrder (int newOrder) const noexcept
{
    auto gi = *this;
    gi.order = newOrder;
    return gi;
}

} // namespace higgs
