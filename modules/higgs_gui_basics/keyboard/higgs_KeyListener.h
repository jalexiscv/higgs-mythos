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
    Receives callbacks when keys are pressed.

    You can add a key listener to a component to be informed when that component
    gets key events. See the Component::addKeyListener method for more details.

    @see KeyPress, Component::addKeyListener, KeyPressMappingSet

    @tags{GUI}
*/
class HIGGS_API  KeyListener
{
public:
    /** Destructor. */
    virtual ~KeyListener() = default;

    //==============================================================================
    /** Called to indicate that a key has been pressed.

        If your implementation returns true, then the key event is considered to have
        been consumed, and will not be passed on to any other components. If it returns
        false, then the key will be passed to other components that might want to use it.

        @param key                      the keystroke, including modifier keys
        @param originatingComponent     the component that received the key event
        @see keyStateChanged, Component::keyPressed
    */
    virtual bool keyPressed (const KeyPress& key,
                             Component* originatingComponent) = 0;

    /** Called when any key is pressed or released.

        When this is called, classes that might be interested in
        the state of one or more keys can use KeyPress::isKeyCurrentlyDown() to
        check whether their key has changed.

        If your implementation returns true, then the key event is considered to have
        been consumed, and will not be passed on to any other components. If it returns
        false, then the key will be passed to other components that might want to use it.

        @param originatingComponent     the component that received the key event
        @param isKeyDown                true if a key is being pressed, false if one is being released
        @see KeyPress, Component::keyStateChanged
    */
    virtual bool keyStateChanged (bool isKeyDown, Component* originatingComponent);
};

} // namespace higgs
