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
/**
    Wraps a ValueTreePropertyWithDefault object that has a default which depends on a global value.
*/
class ValueTreePropertyWithDefaultWrapper final : private Value::Listener
{
public:
    ValueTreePropertyWithDefaultWrapper() = default;

    void init (const ValueTreePropertyWithDefault& v,
               ValueTreePropertyWithDefault global,
               TargetOS::OS targetOS)
    {
        wrappedValue = v;
        globalValue = global.getPropertyAsValue();
        globalIdentifier = global.getPropertyID();
        os = targetOS;

        if (wrappedValue.get() == var())
            wrappedValue.resetToDefault();

        globalValue.addListener (this);
        valueChanged (globalValue);
    }

    ValueTreePropertyWithDefault& getWrappedValueTreePropertyWithDefault()
    {
        return wrappedValue;
    }

    var getCurrentValue() const
    {
        return wrappedValue.get();
    }

private:
    void valueChanged (Value&) override
    {
        wrappedValue.setDefault (getAppSettings().getStoredPath (globalIdentifier, os).get());
    }

    ValueTreePropertyWithDefault wrappedValue;
    Value globalValue;

    Identifier globalIdentifier;
    TargetOS::OS os;
};
