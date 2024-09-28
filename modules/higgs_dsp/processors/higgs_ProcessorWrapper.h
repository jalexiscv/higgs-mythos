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

namespace higgs::dsp
{

/**
    Acts as a polymorphic base class for processors.
    This exposes the same set of methods that a processor must implement as virtual
    methods, so that you can use the ProcessorWrapper class to wrap an instance of
    a subclass, and then pass that around using ProcessorBase as a base class.
    @see ProcessorWrapper

    @tags{DSP}
*/
struct ProcessorBase
{
    ProcessorBase() = default;
    virtual ~ProcessorBase() = default;

    virtual void prepare (const ProcessSpec&)  = 0;
    virtual void process (const ProcessContextReplacing<float>&) = 0;
    virtual void reset() = 0;
};


//==============================================================================
/**
    Wraps an instance of a given processor class, and exposes it through the
    ProcessorBase interface.
    @see ProcessorBase

    @tags{DSP}
*/
template <typename ProcessorType>
struct ProcessorWrapper  : public ProcessorBase
{
    void prepare (const ProcessSpec& spec) override
    {
        processor.prepare (spec);
    }

    void process (const ProcessContextReplacing<float>& context) override
    {
        processor.process (context);
    }

    void reset() override
    {
        processor.reset();
    }

    ProcessorType processor;
};

} // namespace higgs::dsp
