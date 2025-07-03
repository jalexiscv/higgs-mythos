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
template <typename Type>
class NumericValueSource final : public ValueSourceFilter
{
public:
    NumericValueSource (const Value& source)  : ValueSourceFilter (source) {}

    var getValue() const override
    {
        return (Type) sourceValue.getValue();
    }

    void setValue (const var& newValue) override
    {
        const Type newVal = static_cast<Type> (newValue);

        if (newVal != static_cast<Type> (getValue()))  // this test is important, because if a property is missing, it won't
            sourceValue = newVal;                      // create it (causing an unwanted undo action) when a control sets it to 0
    }

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NumericValueSource)
};
