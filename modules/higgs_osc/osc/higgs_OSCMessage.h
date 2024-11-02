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
    An OSC Message.

    An OSCMessage consists of an OSCAddressPattern and zero or more OSCArguments.

    OSC messages are the elementary objects that are used to exchange any data
    via OSC. An OSCSender can send OSCMessage objects to an OSCReceiver.

    @tags{OSC}
*/
class HIGGS_API  OSCMessage
{
public:

    //==============================================================================
    /** Constructs an OSCMessage object with the given address pattern and no
        arguments.

        @param ap    the address pattern of the message. This must be a valid OSC
                     address (starting with a forward slash) and may contain
                     OSC wildcard expressions. You can pass in a string literal
                     or a higgs String (they will be converted to an OSCAddressPattern
                     automatically).
    */
    OSCMessage (const OSCAddressPattern& ap) noexcept;


    /** Constructs an OSCMessage object with the given address pattern and list
        of arguments.

        @param ap    the address pattern of the message. This must be a valid OSC
                     address (starting with a forward slash) and may contain
                     OSC wildcard expressions. You can pass in a string literal
                     or a higgs String (they will be converted to an OSCAddressPattern
                     automatically).

        @param arg1  the first argument of the message.
        @param args  an optional list of further arguments to add to the message.
    */
    template <typename Arg1, typename... Args>
    OSCMessage (const OSCAddressPattern& ap, Arg1&& arg1, Args&&... args);

    /** Sets the address pattern of the OSCMessage.

        @param ap    the address pattern of the message. This must be a valid OSC
                     address (starting with a forward slash) and may contain
                     OSC wildcard expressions. You can pass in a string literal
                     or a higgs String (they will be converted to an OSCAddressPattern
                     automatically).
    */
    void setAddressPattern (const OSCAddressPattern& ap) noexcept;

    /** Returns the address pattern of the OSCMessage. */
    OSCAddressPattern getAddressPattern() const noexcept;

    /** Returns the number of OSCArgument objects that belong to this OSCMessage. */
    int size() const noexcept;

    /** Returns true if the OSCMessage contains no OSCArgument objects; false otherwise. */
    bool isEmpty() const noexcept;

    /** Returns a reference to the OSCArgument at index i in the OSCMessage object.
        This method does not check the range and results in undefined behaviour
        in case i < 0 or i >= size().
    */
    OSCArgument& operator[] (int i) noexcept;
    const OSCArgument& operator[] (int i) const noexcept;

    /** Returns a pointer to the first OSCArgument in the OSCMessage object.
        This method is provided for compatibility with standard C++ iteration mechanisms.
    */
    OSCArgument* begin() noexcept;

    /** Returns a pointer to the first OSCArgument in the OSCMessage object.
        This method is provided for compatibility with standard C++ iteration mechanisms.
    */
    const OSCArgument* begin() const noexcept;

    /** Returns a pointer to the last OSCArgument in the OSCMessage object.
        This method is provided for compatibility with standard C++ iteration mechanisms.
    */
    OSCArgument* end() noexcept;

    /** Returns a pointer to the last OSCArgument in the OSCMessage object.
        This method is provided for compatibility with standard C++ iteration mechanisms.
    */
    const OSCArgument* end() const noexcept;

    /** Removes all arguments from the OSCMessage. */
    void clear();

    //==============================================================================
    /** Creates a new OSCArgument of type int32 with the given value,
        and adds it to the OSCMessage object.
    */
    void addInt32 (int32 value);

    /** Creates a new OSCArgument of type float32 with the given value,
        and adds it to the OSCMessage object.
    */
    void addFloat32 (float value);

    /** Creates a new OSCArgument of type string with the given value,
        and adds it to the OSCMessage object.
    */
    void addString (const String& value);

    /** Creates a new OSCArgument of type blob with binary data content copied from
        the given MemoryBlock.

        Note: If the argument passed is an lvalue, this may copy the binary data.
    */
    void addBlob (MemoryBlock blob);

    /** Creates a new OSCArgument of type colour with the given value,
        and adds it to the OSCMessage object.
    */
    void addColour (OSCColour colour);

    /** Adds the OSCArgument argument to the OSCMessage object.

        Note: This method will result in a copy of the OSCArgument object if it is passed
        as an lvalue. If the OSCArgument is of type blob, this will also copy the underlying
        binary data. In general, you should use addInt32, addFloat32, etc. instead.
    */
    void addArgument (OSCArgument argument);

private:

    //==============================================================================
    template <typename Arg1, typename... Args>
    void addArguments (Arg1&& arg1, Args&&... args)
    {
        addArgument (arg1);
        addArguments (std::forward<Args> (args)...);
    }

    void addArguments() {}

    //==============================================================================
    OSCAddressPattern addressPattern;
    Array<OSCArgument> arguments;
};


//==============================================================================
template <typename Arg1, typename... Args>
OSCMessage::OSCMessage (const OSCAddressPattern& ap, Arg1&& arg1, Args&&... args)
    : addressPattern (ap)
{
    addArguments (std::forward<Arg1> (arg1), std::forward<Args> (args)...);
}

} // namespace higgs
