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

#pragma once

class ProjectContentComponent;
class Project;

//==============================================================================
class MythosTreeViewBase : public TreeViewItem,
                          public TooltipClient
{
public:
    MythosTreeViewBase();
    ~MythosTreeViewBase() override = default;

    int getItemWidth() const override                   { return -1; }
    int getItemHeight() const override                  { return 25; }

    void paintOpenCloseButton (Graphics&, const Rectangle<float>& area, Colour backgroundColour, bool isMouseOver) override;
    void paintItem (Graphics& g, int width, int height) override;
    void itemClicked (const MouseEvent& e) override;
    void itemSelectionChanged (bool isNowSelected) override;
    void itemDoubleClicked (const MouseEvent&) override;
    std::unique_ptr<Component> createItemComponent() override;
    String getTooltip() override            { return {}; }
    String getAccessibilityName() override  { return getDisplayName(); }

    void cancelDelayedSelectionTimer();

    //==============================================================================
    virtual bool isRoot() const                                   { return false; }
    virtual Font getFont() const;
    virtual String getRenamingName() const = 0;
    virtual String getDisplayName() const = 0;
    virtual void setName (const String& newName) = 0;
    virtual bool isMissing() const = 0;
    virtual bool hasWarnings() const                              { return false; }
    virtual Icon getIcon() const = 0;
    virtual bool isIconCrossedOut() const                         { return false; }
    virtual void paintIcon (Graphics& g, Rectangle<float> area);
    virtual void paintContent (Graphics& g, Rectangle<int> area);
    virtual int getRightHandButtonSpace() { return 0; }
    virtual Colour getContentColour (bool isIcon) const;
    virtual int getMillisecsAllowedForDragGesture()               { return 120; }
    virtual File getDraggableFile() const                         { return {}; }

    void refreshSubItems();
    void showRenameBox();

    virtual void deleteItem()                              {}
    virtual void deleteAllSelectedItems()                  {}
    virtual void showDocument()                            {}
    virtual void showMultiSelectionPopupMenu (Point<int>)  {}
    virtual void showPopupMenu (Point<int>)                {}
    virtual void showAddMenu (Point<int>)                  {}
    virtual void handlePopupMenuResult (int)               {}
    virtual void setSearchFilter (const String&)           {}

    void launchPopupMenu (PopupMenu&, Point<int>); // runs asynchronously, and produces a callback to handlePopupMenuResult().

    //==============================================================================
    // To handle situations where an item gets deleted before openness is
    // restored for it, this OpennessRestorer keeps only a pointer to the
    // topmost tree item.
    struct WholeTreeOpennessRestorer final : public OpennessRestorer
    {
        WholeTreeOpennessRestorer (TreeViewItem& item)  : OpennessRestorer (getTopLevelItem (item))
        {}

    private:
        static TreeViewItem& getTopLevelItem (TreeViewItem& item)
        {
            if (TreeViewItem* const p = item.getParentItem())
                return getTopLevelItem (*p);

            return item;
        }
    };

    int textX = 0;

protected:
    ProjectContentComponent* getProjectContentComponent() const;
    virtual void addSubItems() {}

private:
    class ItemSelectionTimer;
    std::unique_ptr<Timer> delayedSelectionTimer;

    void invokeShowDocument();

    HIGGS_DECLARE_WEAK_REFERENCEABLE (MythosTreeViewBase)
};

//==============================================================================
class TreePanelBase : public Component
{
public:
    TreePanelBase (const Project* p, const String& treeviewID)
        : project (p), opennessStateKey (treeviewID)
    {
        addAndMakeVisible (tree);

        tree.setRootItemVisible (true);
        tree.setDefaultOpenness (true);
        tree.setColour (TreeView::backgroundColourId, Colours::transparentBlack);
        tree.setIndentSize (14);
        tree.getViewport()->setScrollBarThickness (6);

        tree.addMouseListener (this, true);
    }

    ~TreePanelBase() override
    {
        tree.setRootItem (nullptr);
    }

    void setRoot (std::unique_ptr<MythosTreeViewBase>);
    void saveOpenness();

    virtual void deleteSelectedItems()
    {
        if (rootItem != nullptr)
            rootItem->deleteAllSelectedItems();
    }

    void setEmptyTreeMessage (const String& newMessage)
    {
        if (emptyTreeMessage != newMessage)
        {
            emptyTreeMessage = newMessage;
            repaint();
        }
    }

    static void drawEmptyPanelMessage (Component& comp, Graphics& g, const String& message)
    {
        const int fontHeight = 13;
        const Rectangle<int> area (comp.getLocalBounds());
        g.setColour (comp.findColour (defaultTextColourId));
        g.setFont ((float) fontHeight);
        g.drawFittedText (message, area.reduced (4, 2), Justification::centred, area.getHeight() / fontHeight);
    }

    void paint (Graphics& g) override
    {
        if (emptyTreeMessage.isNotEmpty() && (rootItem == nullptr || rootItem->getNumSubItems() == 0))
            drawEmptyPanelMessage (*this, g, emptyTreeMessage);
    }

    void resized() override
    {
        tree.setBounds (getAvailableBounds());
    }

    Rectangle<int> getAvailableBounds() const
    {
        return Rectangle<int> (0, 2, getWidth() - 2, getHeight() - 2);
    }

    void mouseDown (const MouseEvent& e) override
    {
        if (e.eventComponent == &tree)
        {
            tree.clearSelectedItems();

            if (e.mods.isRightButtonDown())
                rootItem->showPopupMenu (e.getMouseDownScreenPosition());
        }
    }

    const Project* project;
    TreeView tree;
    std::unique_ptr<MythosTreeViewBase> rootItem;

private:
    String opennessStateKey, emptyTreeMessage;
};

//==============================================================================
class TreeItemComponent final : public Component
{
public:
    TreeItemComponent (MythosTreeViewBase& i)  : item (&i)
    {
        setAccessible (false);
        setInterceptsMouseClicks (false, true);
        item->textX = iconWidth;
    }

    void paint (Graphics& g) override
    {
        if (item == nullptr)
            return;

        auto bounds = getLocalBounds().toFloat();
        auto iconBounds = bounds.removeFromLeft ((float) iconWidth).reduced (7, 5);

        bounds.removeFromRight ((float) buttons.size() * bounds.getHeight());

        item->paintIcon    (g, iconBounds);
        item->paintContent (g, bounds.toNearestInt());
    }

    void resized() override
    {
        auto r = getLocalBounds();

        for (int i = buttons.size(); --i >= 0;)
            buttons.getUnchecked (i)->setBounds (r.removeFromRight (r.getHeight()));
    }

    void addRightHandButton (Component* button)
    {
        buttons.add (button);
        addAndMakeVisible (button);
    }

    WeakReference<MythosTreeViewBase> item;
    OwnedArray<Component> buttons;

    const int iconWidth = 25;

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TreeItemComponent)
};
