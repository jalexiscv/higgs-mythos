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
    An OSC bundle.

    An OSCBundle contains an OSCTimeTag and zero or more OSCBundle Elements.
    The elements of a bundle can be OSC messages or other OSC bundles (this
    means that OSC bundles can be nested).

    This is an advanced OSC structure useful to bundle OSC messages together
    whose effects must occur simultaneously at some given time. For most
    use cases it is probably enough to send and receive plain OSC messages.

    @tags{OSC}
*/
class HIGGS_API  OSCBundle
{
public:
    //==============================================================================
    /** Constructs an OSCBundle with no content and a default time tag ("immediately"). */
    OSCBundle();

    /** Constructs an OSCBundle with no content and a given time tag. */
    OSCBundle (OSCTimeTag timeTag);

    //==============================================================================
    /** Sets the OSCBundle's OSC time tag. */
    void setTimeTag (OSCTimeTag newTimeTag) noexcept        { timeTag = newTimeTag; }

    /** Returns the OSCBundle's OSC time tag. */
    OSCTimeTag getTimeTag() const noexcept                  { return timeTag; }

    //==============================================================================
    /**
        An OSC bundle element.

        An OSCBundle Element contains either one OSCMessage or one OSCBundle.
     */
    class HIGGS_API  Element
    {
    public:
        //==============================================================================
        /** Constructs an OSCBundle Element from an OSCMessage. */
        Element (OSCMessage message);

        /** Constructs an OSCBundle Element from an OSCBundle. */
        Element (OSCBundle bundle);

        /** Copy constructor. */
        Element (const Element& other);

        /** Destructor. */
        ~Element();

        /** Returns true if the OSCBundle element is an OSCMessage. */
        bool isMessage() const noexcept;

        /** Returns true if the OSCBundle element is an OSCBundle. */
        bool isBundle() const noexcept;

        /** Returns a reference to the contained OSCMessage.
            If the OSCBundle element is not an OSCMessage, behaviour is undefined.
         */
        const OSCMessage& getMessage() const;

        /** Returns a reference to the contained OSCBundle.
            If the OSCBundle element is not an OSCBundle, behaviour is undefined.
        */
        const OSCBundle& getBundle() const;

    private:
        //==============================================================================
        std::unique_ptr<OSCMessage> message;
        std::unique_ptr<OSCBundle> bundle;
    };

    //==============================================================================
    /** Returns the number of elements contained in the bundle. */
    int size() const noexcept                                    { return elements.size(); }

    /** Returns true if the bundle contains no elements; false otherwise. */
    bool isEmpty() const noexcept                                { return elements.isEmpty(); }

    /** Returns a reference to the OSCBundle element at index i in this bundle.
        This method does not check the range and results in undefined behaviour
        in case i < 0 or i >= size().
    */
    OSCBundle::Element& operator[] (const int i) noexcept
    {
        return elements.getReference (i);
    }

    const OSCBundle::Element& operator[] (const int i) const noexcept
    {
        return elements.getReference (i);
    }

    /** Adds an OSCBundleElement to the OSCBundle's content. s*/
    void addElement (const OSCBundle::Element& element)          { elements.add (element); }

    /** Returns a pointer to the first element of the OSCBundle. */
    OSCBundle::Element* begin() noexcept                         { return elements.begin(); }

    /** Returns a pointer to the first element of the OSCBundle. */
    const OSCBundle::Element* begin() const noexcept             { return elements.begin(); }

    /** Returns a pointer past the last element of the OSCBundle. */
    OSCBundle::Element* end() noexcept                           { return elements.end(); }

    /** Returns a pointer past the last element of the OSCBundle. */
    const OSCBundle::Element* end() const noexcept               { return elements.end(); }

private:
    //==============================================================================
    Array<OSCBundle::Element> elements;
    OSCTimeTag timeTag;
};

} // namespace higgs
