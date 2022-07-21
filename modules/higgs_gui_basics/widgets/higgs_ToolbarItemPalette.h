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
    A component containing a list of toolbar items, which the user can drag onto
    a toolbar to add them.

    You can use this class directly, but it's a lot easier to call Toolbar::showCustomisationDialog(),
    which automatically shows one of these in a dialog box with lots of extra controls.

    @see Toolbar

    @tags{GUI}
*/
class HIGGS_API  ToolbarItemPalette    : public Component,
                                        public DragAndDropContainer
{
public:
    //==============================================================================
    /** Creates a palette of items for a given factory, with the aim of adding them
        to the specified toolbar.

        The ToolbarItemFactory::getAllToolbarItemIds() method is used to create the
        set of items that are shown in this palette.

        The toolbar and factory must not be deleted while this object exists.
    */
    ToolbarItemPalette (ToolbarItemFactory& factory,
                        Toolbar& toolbar);

    /** Destructor. */
    ~ToolbarItemPalette() override;

    //==============================================================================
    /** @internal */
    void resized() override;
    /** @internal */
    std::unique_ptr<AccessibilityHandler> createAccessibilityHandler() override;

private:
    ToolbarItemFactory& factory;
    Toolbar& toolbar;
    Viewport viewport;
    OwnedArray<ToolbarItemComponent> items;

    friend class Toolbar;
    void replaceComponent (ToolbarItemComponent&);
    void addComponent (int itemId, int index);

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ToolbarItemPalette)
};

} // namespace higgs
