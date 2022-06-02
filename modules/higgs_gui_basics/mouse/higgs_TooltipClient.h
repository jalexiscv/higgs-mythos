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
    Components that want to use pop-up tooltips should implement this interface.

    A TooltipWindow will wait for the mouse to hover over a component that
    implements the TooltipClient interface, and when it finds one, it will display
    the tooltip returned by its getTooltip() method.

    @see TooltipWindow, SettableTooltipClient

    @tags{GUI}
*/
class HIGGS_API  TooltipClient
{
public:
    /** Destructor. */
    virtual ~TooltipClient() = default;

    /** Returns the string that this object wants to show as its tooltip. */
    virtual String getTooltip() = 0;
};


//==============================================================================
/**
    An implementation of TooltipClient that stores the tooltip string and a method
    for changing it.

    This makes it easy to add a tooltip to a custom component, by simply adding this
    as a base class and calling setTooltip().

    Many of the HIGGS widgets already use this as a base class to implement their
    tooltips. See the TooltipWindow docs for more information about implementing
    tooltips.

    @see TooltipClient, TooltipWindow

    @tags{GUI}
*/
class HIGGS_API  SettableTooltipClient   : public TooltipClient
{
public:
    //==============================================================================
    /** Destructor. */
    ~SettableTooltipClient() override = default;

    //==============================================================================
    /** Assigns a new tooltip to this object. */
    virtual void setTooltip (const String& newTooltip)              { tooltipString = newTooltip; }

    /** Returns the tooltip assigned to this object. */
    String getTooltip() override                                    { return tooltipString; }

protected:
    SettableTooltipClient() = default;

private:
    String tooltipString;
};

} // namespace higgs
