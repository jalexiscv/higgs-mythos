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

/** An abstract interface representing the value of an accessibility element.

    Values should be used when information needs to be conveyed which cannot
    be represented by the accessibility element's label alone. For example, a
    gain slider with the label "Gain" needs to also provide a value for its
    position whereas a "Save" button does not.

    This class allows for full control over the value text/numeric conversion,
    ranged, and read-only properties but in most cases you'll want to use one
    of the derived classes below which handle some of this for you.

    @see AccessibilityTextValueInterface, AccessibilityNumericValueInterface,
         AccessibilityRangedNumericValueInterface

    @tags{Accessibility}
*/
class HIGGS_API  AccessibilityValueInterface
{
public:
    /** Destructor. */
    virtual ~AccessibilityValueInterface() = default;

    /** Returns true if the value is read-only and cannot be modified by an
        accessibility client.

        @see setValue, setValueAsString
    */
    virtual bool isReadOnly() const = 0;

    /** Returns the current value as a double. */
    virtual double getCurrentValue() const = 0;

    /** Returns the current value as a String. */
    virtual String getCurrentValueAsString() const = 0;

    /** Sets the current value to a new double value. */
    virtual void setValue (double newValue) = 0;

    /** Sets the current value to a new String value. */
    virtual void setValueAsString (const String& newValue) = 0;

    /** Represents the range of this value, if supported.

        Return one of these from the `getRange()` method, providing a minimum,
        maximum, and interval value for the range to indicate that this is a
        ranged value.

        The default state is an "invalid" range, indicating that the accessibility
        element does not support ranged values.

        @see AccessibilityRangedNumericValueInterface

        @tags{Accessibility}
    */
    class HIGGS_API  AccessibleValueRange
    {
    public:
        /** Constructor.

            Creates a default, "invalid" range that can be returned from
            `AccessibilityValueInterface::getRange()` to indicate that the value
            interface does not support ranged values.
        */
        AccessibleValueRange() = default;

        /** The minimum and maximum values for this range, inclusive. */
        struct HIGGS_API  MinAndMax  { double min, max; };

        /** Constructor.

            Creates a valid AccessibleValueRange with the provided minimum, maximum,
            and interval values.
        */
        AccessibleValueRange (MinAndMax valueRange, double interval)
            : valid (true),
              range (valueRange),
              stepSize (interval)
        {
            jassert (range.min < range.max);
        }

        /** Returns true if this represents a valid range. */
        bool isValid() const noexcept            { return valid; }

        /** Returns the minimum value for this range. */
        double getMinimumValue() const noexcept  { return range.min; }

        /** Returns the maximum value for this range. */
        double getMaximumValue() const noexcept  { return range.max; }

        /** Returns the interval for this range. */
        double getInterval() const noexcept      { return stepSize; }

    private:
        bool valid = false;
        MinAndMax range {};
        double stepSize = 0.0;
    };

    /** If this is a ranged value, this should return a valid AccessibleValueRange
        object representing the supported numerical range.
    */
    virtual AccessibleValueRange getRange() const = 0;
};

//==============================================================================
/** A value interface that represents a text value.

    @tags{Accessibility}
*/
class HIGGS_API  AccessibilityTextValueInterface  : public AccessibilityValueInterface
{
public:
    /** Returns true if the value is read-only and cannot be modified by an
        accessibility client.

        @see setValueAsString
    */
    bool isReadOnly() const override = 0;

    /** Returns the current value. */
    String getCurrentValueAsString() const override = 0;

    /** Sets the current value to a new value. */
    void setValueAsString (const String& newValue) override = 0;

    /** @internal */
    double getCurrentValue() const final         { return getCurrentValueAsString().getDoubleValue(); }
    /** @internal */
    void setValue (double newValue) final        { setValueAsString (String (newValue)); }
    /** @internal */
    AccessibleValueRange getRange() const final  { return {}; }
};

//==============================================================================
/** A value interface that represents a non-ranged numeric value.

    @tags{Accessibility}
*/
class HIGGS_API  AccessibilityNumericValueInterface  : public AccessibilityValueInterface
{
public:
    /** Returns true if the value is read-only and cannot be modified by an
        accessibility client.

        @see setValue
    */
    bool isReadOnly() const override = 0;

    /** Returns the current value. */
    double getCurrentValue() const override = 0;

    /** Sets the current value to a new value. */
    void setValue (double newValue) override = 0;

    /** @internal */
    String getCurrentValueAsString() const final          { return String (getCurrentValue()); }
    /** @internal */
    void setValueAsString (const String& newValue) final  { setValue (newValue.getDoubleValue()); }
    /** @internal */
    AccessibleValueRange getRange() const final           { return {}; }
};

//==============================================================================
/** A value interface that represents a ranged numeric value.

    @tags{Accessibility}
*/
class HIGGS_API  AccessibilityRangedNumericValueInterface  : public AccessibilityValueInterface
{
public:
    /** Returns true if the value is read-only and cannot be modified by an
        accessibility client.

        @see setValueAsString
    */
    bool isReadOnly() const override = 0;

    /** Returns the current value. */
    double getCurrentValue() const override = 0;

    /** Sets the current value to a new value. */
    void setValue (double newValue) override = 0;

    /** Returns the range. */
    AccessibleValueRange getRange() const override = 0;

    /** @internal */
    String getCurrentValueAsString() const final          { return String (getCurrentValue()); }
    /** @internal */
    void setValueAsString (const String& newValue) final  { setValue (newValue.getDoubleValue()); }
};

} // namespace higgs
