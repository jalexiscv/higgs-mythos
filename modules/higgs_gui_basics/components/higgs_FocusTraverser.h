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
    Controls the order in which focus moves between components.

    The algorithm used by this class to work out the order of traversal is as
    follows:
    - Only visible and enabled components are considered focusable.
    - If two components both have an explicit focus order specified then the
      one with the lowest number comes first (see the
      Component::setExplicitFocusOrder() method).
    - Any component with an explicit focus order greater than 0 comes before ones
      that don't have an order specified.
    - Components with their 'always on top' flag set come before those without.
    - Any unspecified components are traversed in a left-to-right, then
      top-to-bottom order.

    If you need focus traversal in a more customised way you can create a
    ComponentTraverser subclass that uses your own algorithm and return it
    from Component::createFocusTraverser().

    @see ComponentTraverser, Component::createFocusTraverser

    @tags{GUI}
*/
class HIGGS_API  FocusTraverser  : public ComponentTraverser
{
public:
    /** Controls whether the FocusTraverser will allow navigation to disabled components.
    */
    enum class SkipDisabledComponents
    {
        no,  ///< Disabled components can receive focus.
        yes  ///< Disabled components can't receive focus.
    };

    /** Creates a FocusTraverser that will not skip disabled components. */
    FocusTraverser() = default;

    /** Creates a FocusTraverser.

        @param skipDisabledComponents  If set to SkipDisabledComponents::yes, the traverser will ignore
                                       disabled components. This makes such components non-discoverable
                                       using screen readers.
    */
    explicit FocusTraverser (SkipDisabledComponents skipDisabledComponents);

    /** Destructor. */
    ~FocusTraverser() override = default;

    /** Returns the component that should receive focus by default within the given
        parent component.

        The default implementation will just return the foremost visible and enabled
        child component, and will return nullptr if there is no suitable component.
    */
    Component* getDefaultComponent (Component* parentComponent) override;

    /** Returns the component that should be given focus after the specified one when
        moving "forwards".

        The default implementation will return the next visible and enabled component
        which is to the right of or below this one, and will return nullptr if there
        is no suitable component.
    */
    Component* getNextComponent (Component* current) override;

    /** Returns the component that should be given focus after the specified one when
        moving "backwards".

        The default implementation will return the previous visible and enabled component
        which is to the left of or above this one, and will return nullptr if there
        is no suitable component.
    */
    Component* getPreviousComponent (Component* current) override;

    /** Returns all of the components that can receive focus within the given parent
        component in traversal order.

        The default implementation will return all visible and enabled child components.
    */
    std::vector<Component*> getAllComponents (Component* parentComponent) override;

private:
    SkipDisabledComponents skipDisabledComponents = SkipDisabledComponents::no;
};

} // namespace higgs
