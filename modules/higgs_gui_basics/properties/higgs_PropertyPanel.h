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
    A panel that holds a list of PropertyComponent objects.

    This panel displays a list of PropertyComponents, and allows them to be organised
    into collapsible sections.

    To use, simply create one of these and add your properties to it with addProperties()
    or addSection().

    @see PropertyComponent

    @tags{GUI}
*/
class HIGGS_API  PropertyPanel  : public Component
{
public:
    //==============================================================================
    /** Creates an empty property panel. */
    PropertyPanel();

    /** Creates an empty property panel. */
    PropertyPanel (const String& name);

    /** Destructor. */
    ~PropertyPanel() override;

    //==============================================================================
    /** Deletes all property components from the panel. */
    void clear();

    /** Adds a set of properties to the panel.

        The components in the list will be owned by this object and will be automatically
        deleted later on when no longer needed.

        These properties are added without them being inside a named section. If you
        want them to be kept together in a collapsible section, use addSection() instead.
    */
    void addProperties (const Array<PropertyComponent*>& newPropertyComponents,
                        int extraPaddingBetweenComponents = 0);

    /** Adds a set of properties to the panel.

        These properties are added under a section heading with a plus/minus button that
        allows it to be opened and closed. If indexToInsertAt is < 0 then it will be added
        at the end of the list, or before the given index if the index is non-zero.

        The components in the list will be owned by this object and will be automatically
        deleted later on when no longer needed.

        To add properties without them being in a section, use addProperties().
    */
    void addSection (const String& sectionTitle,
                     const Array<PropertyComponent*>& newPropertyComponents,
                     bool shouldSectionInitiallyBeOpen = true,
                     int indexToInsertAt = -1,
                     int extraPaddingBetweenComponents = 0);

    /** Calls the refresh() method of all PropertyComponents in the panel */
    void refreshAll() const;

    /** Returns true if the panel contains no properties. */
    bool isEmpty() const;

    /** Returns the height that the panel needs in order to display all of its content
        without scrolling.
    */
    int getTotalContentHeight() const;

    //==============================================================================
    /** Returns a list of all the names of sections in the panel.
        These are the sections that have been added with addSection().
    */
    StringArray getSectionNames() const;

    /** Returns true if the section at this index is currently open.
        The index is from 0 up to the number of items returned by getSectionNames().
    */
    bool isSectionOpen (int sectionIndex) const;

    /** Opens or closes one of the sections.
        The index is from 0 up to the number of items returned by getSectionNames().
    */
    void setSectionOpen (int sectionIndex, bool shouldBeOpen);

    /** Enables or disables one of the sections.
        The index is from 0 up to the number of items returned by getSectionNames().
    */
    void setSectionEnabled (int sectionIndex, bool shouldBeEnabled);

    /** Remove one of the sections using the section index.
        The index is from 0 up to the number of items returned by getSectionNames().
    */
    void removeSection (int sectionIndex);

    //==============================================================================
    /** Saves the current state of open/closed sections so it can be restored later.
        To restore this state, use restoreOpennessState().
        @see restoreOpennessState
    */
    std::unique_ptr<XmlElement> getOpennessState() const;

    /** Restores a previously saved arrangement of open/closed sections.

        This will try to restore a snapshot of the panel's state that was created by
        the getOpennessState() method. If any of the sections named in the original
        XML aren't present, they will be ignored.

        @see getOpennessState
    */
    void restoreOpennessState (const XmlElement& newState);

    //==============================================================================
    /** Sets a message to be displayed when there are no properties in the panel.
        The default message is "nothing selected".
    */
    void setMessageWhenEmpty (const String& newMessage);

    /** Returns the message that is displayed when there are no properties.
        @see setMessageWhenEmpty
    */
    const String& getMessageWhenEmpty() const noexcept;

    //==============================================================================
    /** Returns the PropertyPanel's internal Viewport. */
    Viewport& getViewport() noexcept        { return viewport; }

    //==============================================================================
    /** @internal */
    void paint (Graphics&) override;
    /** @internal */
    void resized() override;

private:
    Viewport viewport;
    struct SectionComponent;
    struct PropertyHolderComponent;
    PropertyHolderComponent* propertyHolderComponent;
    String messageWhenEmpty;

    void init();
    void updatePropHolderLayout() const;
    void updatePropHolderLayout (int width) const;

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (PropertyPanel)
};

} // namespace higgs
