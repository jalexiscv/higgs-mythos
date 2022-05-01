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
    Controls the order in which keyboard focus moves between components.

    The default behaviour of this class uses a FocusTraverser object internally to
    determine the default/next/previous component until it finds one which wants
    keyboard focus, as set by the Component::setWantsKeyboardFocus() method.

    If you need keyboard focus traversal in a more customised way, you can create
    a subclass of ComponentTraverser that uses your own algorithm, and use
    Component::createKeyboardFocusTraverser() to create it.

    @see FocusTraverser, ComponentTraverser, Component::createKeyboardFocusTraverser

    @tags{GUI}
*/
class HIGGS_API  KeyboardFocusTraverser  : public ComponentTraverser
{
public:
    /** Destructor. */
    ~KeyboardFocusTraverser() override = default;

    /** Returns the component that should receive keyboard focus by default within the
        given parent component.

        The default implementation will return the foremost focusable component (as
        determined by FocusTraverser) that also wants keyboard focus, or nullptr if
        there is no suitable component.
    */
    Component* getDefaultComponent (Component* parentComponent) override;

    /** Returns the component that should be given keyboard focus after the specified
        one when moving "forwards".

        The default implementation will return the next focusable component (as
        determined by FocusTraverser) that also wants keyboard focus, or nullptr if
        there is no suitable component.
    */
    Component* getNextComponent (Component* current) override;

    /** Returns the component that should be given keyboard focus after the specified
        one when moving "backwards".

        The default implementation will return the previous focusable component (as
        determined by FocusTraverser) that also wants keyboard focus, or nullptr if
        there is no suitable component.
    */
    Component* getPreviousComponent (Component* current) override;

    /** Returns all of the components that can receive keyboard focus within the given
        parent component in traversal order.

        The default implementation will return all focusable child components (as
        determined by FocusTraverser) that also wants keyboard focus.
    */
    std::vector<Component*> getAllComponents (Component* parentComponent) override;
};

} // namespace higgs
