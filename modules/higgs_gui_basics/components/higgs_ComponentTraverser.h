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
    Base class for traversing components.

    If you need custom focus or keyboard focus traversal for a component you can
    create a subclass of ComponentTraverser and return it from
    Component::createFocusTraverser() or Component::createKeyboardFocusTraverser().

    @see Component::createFocusTraverser, Component::createKeyboardFocusTraverser

    @tags{GUI}
*/
class HIGGS_API  ComponentTraverser
{
public:
    /** Destructor. */
    virtual ~ComponentTraverser() = default;

    /** Returns the component that should be used as the traversal entry point
        within the given parent component.

        This must return nullptr if there is no default component.
    */
    virtual Component* getDefaultComponent (Component* parentComponent) = 0;

    /** Returns the component that comes after the specified one when moving "forwards".

        This must return nullptr if there is no next component.
    */
    virtual Component* getNextComponent (Component* current) = 0;

    /** Returns the component that comes after the specified one when moving "backwards".

        This must return nullptr if there is no previous component.
    */
    virtual Component* getPreviousComponent (Component* current) = 0;

    /** Returns all of the traversable components within the given parent component in
        traversal order.
    */
    virtual std::vector<Component*> getAllComponents (Component* parentComponent) = 0;
};

} // namespace higgs
