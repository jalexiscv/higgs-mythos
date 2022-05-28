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
    A menu bar component.

    @see MenuBarModel

    @tags{GUI}
*/
class HIGGS_API  MenuBarComponent  : public Component,
                                    private MenuBarModel::Listener,
                                    private Timer
{
public:
    //==============================================================================
    /** Creates a menu bar.

        @param model    the model object to use to control this bar. You can
                        pass omit the parameter or pass nullptr into this if you like,
                        and set the model later using the setModel() method.
    */
    MenuBarComponent (MenuBarModel* model = nullptr);

    /** Destructor. */
    ~MenuBarComponent() override;

    //==============================================================================
    /** Changes the model object to use to control the bar.

        This can be a null pointer, in which case the bar will be empty. Don't delete the object
        that is passed-in while it's still being used by this MenuBar.
    */
    void setModel (MenuBarModel* newModel);

    /** Returns the current menu bar model being used. */
    MenuBarModel* getModel() const noexcept;

    //==============================================================================
    /** Pops up one of the menu items.

        This lets you manually open one of the menus - it could be triggered by a
        key shortcut, for example.
    */
    void showMenu (int menuIndex);

    //==============================================================================
    /** @internal */
    void paint (Graphics&) override;
    /** @internal */
    void resized() override;
    /** @internal */
    void mouseEnter (const MouseEvent&) override;
    /** @internal */
    void mouseExit (const MouseEvent&) override;
    /** @internal */
    void mouseDown (const MouseEvent&) override;
    /** @internal */
    void mouseDrag (const MouseEvent&) override;
    /** @internal */
    void mouseUp (const MouseEvent&) override;
    /** @internal */
    void mouseMove (const MouseEvent&) override;
    /** @internal */
    void handleCommandMessage (int commandId) override;
    /** @internal */
    bool keyPressed (const KeyPress&) override;
    /** @internal */
    void menuBarItemsChanged (MenuBarModel*) override;
    /** @internal */
    void menuCommandInvoked (MenuBarModel*, const ApplicationCommandTarget::InvocationInfo&) override;
    /** @internal */
    std::unique_ptr<AccessibilityHandler> createAccessibilityHandler() override;

private:
    //==============================================================================
    class AccessibleItemComponent;

    //==============================================================================
    void timerCallback() override;

    int getItemAt (Point<int>);
    void setItemUnderMouse (int);
    void setOpenItem (int);
    void updateItemUnderMouse (Point<int>);
    void repaintMenuItem (int);
    void menuDismissed (int, int);

    void updateItemComponents (const StringArray&);
    int indexOfItemComponent (AccessibleItemComponent*) const;

    //==============================================================================
    MenuBarModel* model = nullptr;
    std::vector<std::unique_ptr<AccessibleItemComponent>> itemComponents;

    Point<int> lastMousePos;
    int itemUnderMouse = -1, currentPopupIndex = -1, topLevelIndexDismissed = 0;
    int numActiveMenus = 0;

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MenuBarComponent)
};

} // namespace higgs
