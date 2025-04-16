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

//==============================================================================
class StartPageTreeHolder final : public Component
{
public:
    enum class Open { no, yes };

    StartPageTreeHolder (const String& title,
                         const StringArray& headerNames,
                         const std::vector<StringArray>& itemNames,
                         std::function<void (int, int)>&& selectedCallback,
                         Open shouldBeOpen)
        : headers (headerNames),
          items (itemNames),
          itemSelectedCallback (std::move (selectedCallback))
    {
        jassert (headers.size() == (int) items.size());

        tree.setTitle (title);
        tree.setRootItem (new TreeRootItem (*this));
        tree.setRootItemVisible (false);
        tree.setIndentSize (15);
        tree.setDefaultOpenness (shouldBeOpen == Open::yes);

        addAndMakeVisible (tree);
    }

    ~StartPageTreeHolder() override
    {
        tree.deleteRootItem();
    }

    void paint (Graphics& g) override
    {
        g.fillAll (findColour (secondaryBackgroundColourId));
    }

    void resized() override
    {
        tree.setBounds (getLocalBounds());
    }

    void setSelectedItem (const String& category, int index)
    {
        auto* root = tree.getRootItem();

        for (int i = root->getNumSubItems(); --i >=0;)
        {
            if (auto* item = root->getSubItem (i))
            {
                if (item->getUniqueName() == category)
                    item->getSubItem (index)->setSelected (true, true);
            }
        }
    }

private:
    //==============================================================================
    class TreeSubItem final : public TreeViewItem
    {
    public:
        TreeSubItem (StartPageTreeHolder& o, const String& n, const StringArray& subItemsIn)
            : owner (o), name (n), isHeader (subItemsIn.size() > 0)
        {
            for (auto& s : subItemsIn)
                addSubItem (new TreeSubItem (owner, s, {}));
        }

        bool mightContainSubItems() override    { return isHeader; }
        bool canBeSelected() const override     { return ! isHeader; }

        int getItemWidth() const override       { return -1; }
        int getItemHeight() const override      { return 25; }

        String getUniqueName() const override   { return name; }
        String getAccessibilityName() override  { return getUniqueName(); }

        void paintOpenCloseButton (Graphics& g, const Rectangle<float>& area, Colour, bool isMouseOver) override
        {
            g.setColour (getOwnerView()->findColour (isSelected() ? defaultHighlightedTextColourId
                                                                  : treeIconColourId));

            TreeViewItem::paintOpenCloseButton (g, area, getOwnerView()->findColour (defaultIconColourId), isMouseOver);
        }

        void paintItem (Graphics& g, int w, int h) override
        {
            Rectangle<int> bounds (w, h);

            auto shouldBeHighlighted = isSelected();

            if (shouldBeHighlighted)
            {
                g.setColour (getOwnerView()->findColour (defaultHighlightColourId));
                g.fillRect (bounds);
            }

            g.setColour (shouldBeHighlighted ? getOwnerView()->findColour (defaultHighlightedTextColourId)
                                             : getOwnerView()->findColour (defaultTextColourId));

            g.drawFittedText (name, bounds.reduced (5).withTrimmedLeft (10), Justification::centredLeft, 1);
        }

        void itemClicked (const MouseEvent& e) override
        {
            if (isSelected())
                itemSelectionChanged (true);

            if (e.mods.isPopupMenu() && mightContainSubItems())
                setOpen (! isOpen());
        }

        void itemSelectionChanged (bool isNowSelected) override
        {
            jassert (! isHeader);

            if (isNowSelected)
                owner.itemSelectedCallback (getParentItem()->getIndexInParent(), getIndexInParent());
        }

    private:
        StartPageTreeHolder& owner;
        String name;
        bool isHeader = false;

        //==============================================================================
        HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TreeSubItem)
    };

    struct TreeRootItem final : public TreeViewItem
    {
        explicit TreeRootItem (StartPageTreeHolder& o)
            : owner (o)
        {
            for (int i = 0; i < owner.headers.size(); ++i)
                addSubItem (new TreeSubItem (owner, owner.headers[i], owner.items[(size_t) i]));
        }

        bool mightContainSubItems() override { return ! owner.headers.isEmpty();}

        StartPageTreeHolder& owner;

        //==============================================================================
        HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (TreeRootItem)
    };

    //==============================================================================
    TreeView tree;
    StringArray headers;
    std::vector<StringArray> items;

    std::function<void (int, int)> itemSelectedCallback;

    //==============================================================================
    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (StartPageTreeHolder)
};
