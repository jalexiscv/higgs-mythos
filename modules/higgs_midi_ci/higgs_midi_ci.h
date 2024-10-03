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


/*******************************************************************************
 The block below describes the properties of this module, and is read by
 the MythosStudio to automatically generate project code that uses it.
 For details about the syntax and how to create or use a module, see the
 Higgs Mythos Module Format.md file.


 BEGIN_HIGGS_MODULE_DECLARATION

  ID:                 higgs_midi_ci
  vendor:             higgs
  version:            9.0.5
  name:               Higgs Mythos MIDI CI Classes
  description:        Classes facilitating communication via MIDI Capability Inquiry
  website:            https://github.com/jalexiscv/higgs-mythos
  license:            AGPLv3
  minimumCppStandard: 17

  dependencies:       higgs_audio_basics

 END_HIGGS_MODULE_DECLARATION

*******************************************************************************/


#pragma once
#define HIGGS_MIDI_CI_H_INCLUDED

#include <higgs_audio_basics/higgs_audio_basics.h>

#include <higgs_midi_ci/ci/higgs_CIFunctionBlock.h>
#include <higgs_midi_ci/ci/higgs_CIMuid.h>
#include <higgs_midi_ci/ci/higgs_CIEncoding.h>
#include <higgs_midi_ci/ci/higgs_CIEncodings.h>
#include <higgs_midi_ci/ci/higgs_CIMessages.h>
#include <higgs_midi_ci/ci/higgs_CIChannelAddress.h>
#include <higgs_midi_ci/ci/higgs_CIResponderOutput.h>
#include <higgs_midi_ci/ci/higgs_CIParser.h>
#include <higgs_midi_ci/ci/higgs_CISupportedAndActive.h>
#include <higgs_midi_ci/ci/higgs_CIResponderDelegate.h>
#include <higgs_midi_ci/ci/higgs_CIProfileStates.h>
#include <higgs_midi_ci/ci/higgs_CIProfileAtAddress.h>
#include <higgs_midi_ci/ci/higgs_CIProfileDelegate.h>
#include <higgs_midi_ci/ci/higgs_CIProfileHost.h>
#include <higgs_midi_ci/ci/higgs_CISubscription.h>
#include <higgs_midi_ci/ci/higgs_CIPropertyDelegate.h>
#include <higgs_midi_ci/ci/higgs_CIPropertyExchangeResult.h>
#include <higgs_midi_ci/ci/higgs_CIPropertyExchangeCache.h>
#include <higgs_midi_ci/ci/higgs_CIPropertyHost.h>
#include <higgs_midi_ci/ci/higgs_CIDeviceFeatures.h>
#include <higgs_midi_ci/ci/higgs_CIDeviceMessageHandler.h>
#include <higgs_midi_ci/ci/higgs_CIDeviceOptions.h>
#include <higgs_midi_ci/ci/higgs_CISubscriptionManager.h>
#include <higgs_midi_ci/ci/higgs_CIDeviceListener.h>
#include <higgs_midi_ci/ci/higgs_CIDevice.h>

namespace higgs
{
    namespace ci = midi_ci;
}
