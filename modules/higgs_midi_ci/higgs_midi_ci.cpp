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

#ifdef HIGGS_MIDI_CI_H_INCLUDED
 /* When you add this cpp file to your project, you mustn't include it in a file where you've
    already included any other headers - just put it inside a file on its own, possibly with your config
    flags preceding it, but don't include anything else. That also includes avoiding any automatic prefix
    header files that the compiler may be using.
 */
 #error "Incorrect use of HIGGS cpp file"
#endif

#include "higgs_midi_ci.h"

#include <higgs_midi_ci/detail/higgs_CIMessageMeta.h>
#include <higgs_midi_ci/detail/higgs_CIMarshalling.h>
#include <higgs_midi_ci/detail/higgs_CIPropertyDataMessageChunker.h>
#include <higgs_midi_ci/detail/higgs_CIResponder.h>
#include <higgs_midi_ci/detail/higgs_CIMessageTypeUtils.h>
#include <higgs_midi_ci/detail/higgs_CIPropertyHostUtils.h>

#include <higgs_midi_ci/detail/higgs_CIPropertyDataMessageChunker.cpp>
#include <higgs_midi_ci/detail/higgs_CIResponder.cpp>

#include <higgs_midi_ci/ci/higgs_CIChannelAddress.cpp>
#include <higgs_midi_ci/ci/higgs_CIDevice.cpp>
#include <higgs_midi_ci/ci/higgs_CIEncodings.cpp>
#include <higgs_midi_ci/ci/higgs_CIMessages.cpp>
#include <higgs_midi_ci/ci/higgs_CIParser.cpp>
#include <higgs_midi_ci/ci/higgs_CIProfileAtAddress.cpp>
#include <higgs_midi_ci/ci/higgs_CIProfileHost.cpp>
#include <higgs_midi_ci/ci/higgs_CIProfileStates.cpp>
#include <higgs_midi_ci/ci/higgs_CIPropertyDelegate.cpp>
#include <higgs_midi_ci/ci/higgs_CIPropertyExchangeCache.cpp>
#include <higgs_midi_ci/ci/higgs_CIPropertyHost.cpp>
#include <higgs_midi_ci/ci/higgs_CIResponderOutput.cpp>
#include <higgs_midi_ci/ci/higgs_CISubscription.cpp>
#include <higgs_midi_ci/ci/higgs_CISubscriptionManager.cpp>
