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
    A component which lists all menu items and groups them into categories
    by their respective parent menus. This kind of component is often used
    for so-called "burger" menus in mobile apps.

    @see MenuBarModel

    @tags{GUI}
*/
class BurgerMenuComponent     : public Component,
                                private ListBoxModel,
                                private MenuBarModel::Listener
{
public:
    //==============================================================================
    /** Creates a burger menu component.

        @param model    the model object to use to control this burger menu. You can
                        set the parameter or pass nullptr into this if you like,
                        and set the model later using the setModel() method.

        @see setModel
     */
    BurgerMenuComponent (MenuBarModel* model = nullptr);

    /** Destructor. */
    ~BurgerMenuComponent() override;

    //==============================================================================
    /** Changes the model object to use to control the burger menu.

        This can be a nullptr, in which case the bar will be empty. This object will not be
        owned by the BurgerMenuComponent so it is up to you to manage its lifetime.
        Don't delete the object that is passed-in while it's still being used by this MenuBar.
        Any submenus in your MenuBarModel will be recursively flattened and added to the
        top-level burger menu section.
     */
    void setModel (MenuBarModel* newModel);

    /** Returns the current burger menu model being used. */
    MenuBarModel* getModel() const noexcept;

    /** @internal */
    void lookAndFeelChanged() override;
    /** @internal */
    std::unique_ptr<AccessibilityHandler> createAccessibilityHandler() override;

private:
    //==============================================================================
    struct Row
    {
        bool isMenuHeader;
        int topLevelMenuIndex;
        PopupMenu::Item item;
    };

    void refresh();
    void paint (Graphics&) override;
    int getNumRows() override;
    void paintListBoxItem (int, Graphics&, int, int, bool) override;
    void listBoxItemClicked (int, const MouseEvent&) override;
    Component* refreshComponentForRow (int, bool, Component*) override;
    void resized() override;
    void menuBarItemsChanged (MenuBarModel*) override;
    void menuCommandInvoked (MenuBarModel*, const ApplicationCommandTarget::InvocationInfo&) override;
    void mouseUp (const MouseEvent&) override;
    void handleCommandMessage (int) override;
    void addMenuBarItemsForMenu (PopupMenu&, int);
    static bool hasSubMenu (const PopupMenu::Item&);

    //==============================================================================
    MenuBarModel* model = nullptr;
    ListBox listBox    {"BurgerMenuListBox", this};
    Array<Row> rows;

    int lastRowClicked = -1, inputSourceIndexOfLastClick = -1, topLevelIndexClicked = -1;

    //==============================================================================
    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (BurgerMenuComponent)
};

} // namespace higgs
