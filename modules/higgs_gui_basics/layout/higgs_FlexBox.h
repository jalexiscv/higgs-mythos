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

/**
    Represents a FlexBox container, which contains and manages the layout of a set
    of FlexItem objects.

    To use this class, set its parameters appropriately (you can search online for
    more help on exactly how the FlexBox protocol works!), then add your sub-items
    to the items array, and call performLayout() in the resized() function of your
    Component.

    @see FlexItem

    @tags{GUI}
*/
class HIGGS_API  FlexBox  final
{
public:
    /** Possible values for the flexDirection property. */
    enum class Direction
    {
        row,                  /**< Set the main axis direction from left to right. */
        rowReverse,           /**< Set the main axis direction from right to left. */
        column,               /**< Set the main axis direction from top to bottom. */
        columnReverse         /**< Set the main axis direction from bottom to top. */
    };

    /** Possible values for the flexWrap property. */
    enum class Wrap
    {
        noWrap,               /**< Items are forced into a single line. */
        wrap,                 /**< Items are wrapped onto multiple lines from top to bottom. */
        wrapReverse           /**< Items are wrapped onto multiple lines from bottom to top. */
    };

    /** Possible values for the alignContent property. */
    enum class AlignContent
    {
        stretch,              /**< Lines of items are stretched from start to end of the cross axis. */
        flexStart,            /**< Lines of items are aligned towards the start of the cross axis. */
        flexEnd,              /**< Lines of items are aligned towards the end of the cross axis. */
        center,               /**< Lines of items are aligned towards the center of the cross axis. */
        spaceBetween,         /**< Lines of items are evenly spaced along the cross axis with spaces between them. */
        spaceAround           /**< Lines of items are evenly spaced along the cross axis with spaces around them. */
    };

    /** Possible values for the alignItems property. */
    enum class AlignItems
    {
        stretch,              /**< Items are stretched from start to end of the cross axis. */
        flexStart,            /**< Items are aligned towards the start of the cross axis. */
        flexEnd,              /**< Items are aligned towards the end of the cross axis. */
        center                /**< Items are aligned towards the center of the cross axis. */
    };

    /** Possible values for the justifyContent property. */
    enum class JustifyContent
    {
        flexStart,            /**< Items are justified towards the start of the main axis. */
        flexEnd,              /**< Items are justified towards the end of the main axis. */
        center,               /**< Items are justified towards the center of the main axis. */
        spaceBetween,         /**< Items are evenly spaced along the main axis with spaces between them. */
        spaceAround           /**< Items are evenly spaced along the main axis with spaces around them. */
    };

    //==============================================================================
    /** Creates an empty FlexBox container with default parameters. */
    FlexBox() noexcept = default;

    /** Creates an empty FlexBox container with these parameters. */
    FlexBox (Direction, Wrap, AlignContent, AlignItems, JustifyContent) noexcept;

    /** Creates an empty FlexBox container with the given content-justification mode. */
    FlexBox (JustifyContent) noexcept;

    //==============================================================================
    /** Lays-out the box's items within the given rectangle. */
    void performLayout (Rectangle<float> targetArea);

    /** Lays-out the box's items within the given rectangle. */
    void performLayout (Rectangle<int> targetArea);

    //==============================================================================
    /** Specifies how flex items are placed in the flex container, and defines the
        direction of the main axis.
    */
    Direction flexDirection = Direction::row;

    /** Specifies whether items are forced into a single line or can be wrapped onto multiple lines.
        If wrapping is allowed, this property also controls the direction in which lines are stacked.
    */
    Wrap flexWrap = Wrap::noWrap;

    /** Specifies how a flex container's lines are placed within the flex container when
        there is extra space on the cross-axis.
        This property has no effect on single line layouts.
    */
    AlignContent alignContent = AlignContent::stretch;

    /** Specifies the alignment of flex items along the cross-axis of each line. */
    AlignItems alignItems = AlignItems::stretch;

    /** Defines how the container distributes space between and around items along the main-axis.
        The alignment is done after the lengths and auto margins are applied, so that if there is at
        least one flexible element, with flex-grow different from 0, it will have no effect as there
        won't be any available space.
    */
    JustifyContent justifyContent = JustifyContent::flexStart;

    /** The set of items to lay-out. */
    Array<FlexItem> items;

private:
    HIGGS_LEAK_DETECTOR (FlexBox)
};

} // namespace higgs
