/*
  ==============================================================================

   This file is part of the HIGGS framework examples.
   Copyright (c) Raw Material Software Limited

   The code included in this file is provided under the terms of the ISC license
   http://www.isc.org/downloads/software-support-policy/isc-license. Permission
   to use, copy, modify, and/or distribute this software for any purpose with or
   without fee is hereby granted provided that the above copyright notice and
   this permission notice appear in all copies.

   THE SOFTWARE IS PROVIDED "AS IS" AND THE AUTHOR DISCLAIMS ALL WARRANTIES WITH
   REGARD TO THIS SOFTWARE INCLUDING ALL IMPLIED WARRANTIES OF MERCHANTABILITY
   AND FITNESS. IN NO EVENT SHALL THE AUTHOR BE LIABLE FOR ANY SPECIAL, DIRECT,
   INDIRECT, OR CONSEQUENTIAL DAMAGES OR ANY DAMAGES WHATSOEVER RESULTING FROM
   LOSS OF USE, DATA OR PROFITS, WHETHER IN AN ACTION OF CONTRACT, NEGLIGENCE OR
   OTHER TORTIOUS ACTION, ARISING OUT OF OR IN CONNECTION WITH THE USE OR
   PERFORMANCE OF THIS SOFTWARE.

  ==============================================================================
*/

/*******************************************************************************
 The block below describes the properties of this PIP. A PIP is a short snippet
 of code that can be read by the MythosStudio and used to generate a HIGGS project.

 BEGIN_HIGGS_PIP_METADATA

 name:             GainDemo
 version:          1.0.0
 vendor:           HIGGS
 website:          http://higgsmythos.com
 description:      Gain demo using the DSP module.

 dependencies:     higgs_audio_basics, higgs_audio_devices, higgs_audio_formats,
                   higgs_audio_processors, higgs_audio_utils, higgs_core,
                   higgs_data_structures, higgs_dsp, higgs_events, higgs_graphics,
                   higgs_gui_basics, higgs_gui_extra, higgs_audio_processors_headless
 exporters:        xcode_mac, vs2022, vs2026, linux_make

 moduleFlags:      HIGGS_STRICT_REFCOUNTEDPOINTER=1

 type:             Component
 mainClass:        GainDemo

 useLocalCopy:     1

 END_HIGGS_PIP_METADATA

*******************************************************************************/

#pragma once

#include "../Assets/DemoUtilities.h"
#include "../Assets/DSPDemos_Common.h"

using namespace dsp;

//==============================================================================
struct GainDemoDSP
{
    void prepare (const ProcessSpec&)
    {
        gain.setGainDecibels (-6.0f);
    }

    void process (const ProcessContextReplacing<float>& context)
    {
        gain.process (context);
    }

    void reset()
    {
        gain.reset();
    }

    void updateParameters()
    {
        gain.setGainDecibels (static_cast<float> (gainParam.getCurrentValue()));
    }

    //==============================================================================
    Gain<float> gain;
    SliderParameter gainParam { { -100.0, 20.0 }, 3.0, -6.0, "Gain", "dB" };

    std::vector<DSPDemoParameterBase*> parameters { &gainParam };
};

struct GainDemo final : public Component
{
    GainDemo()
    {
        addAndMakeVisible (fileReaderComponent);
        setSize (750, 500);
    }

    void resized() override
    {
        fileReaderComponent.setBounds (getLocalBounds());
    }

    AudioFileReaderComponent<GainDemoDSP> fileReaderComponent;
};
