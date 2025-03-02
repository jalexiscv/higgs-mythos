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

  ID:                 higgs_product_unlocking
  vendor:             higgs
  version:            9.0.5
  name:               Higgs Mythos Online marketplace support
  description:        Classes for online product authentication
  website:            https://github.com/jalexiscv/higgs-mythos
  license:            AGPLv3
  minimumCppStandard: 17

  dependencies:       higgs_cryptography

 END_HIGGS_MODULE_DECLARATION

*******************************************************************************/


#pragma once
#define HIGGS_PRODUCT_UNLOCKING_H_INCLUDED

/**
    The higgs_product_unlocking module provides simple user-registration classes
    for allowing you to build apps/plugins with features that are unlocked by a
    user having a suitable account on a webserver.

    Although originally designed for use with products that are sold on the
    Tracktion Marketplace web-store, the module itself is fully open, and can
    be used to connect to your own web-store instead, if you implement your
    own compatible web-server back-end.

    In additional, the module supports in-app purchases both on iOS and Android
    platforms.
*/

//==============================================================================
#include <higgs_core/higgs_core.h>
#include <higgs_cryptography/higgs_cryptography.h>
#include <higgs_events/higgs_events.h>

#if HIGGS_MODULE_AVAILABLE_higgs_data_structures
 #include <higgs_data_structures/higgs_data_structures.h>
#endif

#if HIGGS_MODULE_AVAILABLE_higgs_gui_extra
 #include <higgs_gui_extra/higgs_gui_extra.h>
#endif

#if HIGGS_IN_APP_PURCHASES
 #include "in_app_purchases/higgs_InAppPurchases.h"
#endif

#if HIGGS_MODULE_AVAILABLE_higgs_data_structures
 #include "marketplace/higgs_OnlineUnlockStatus.h"
 #include "marketplace/higgs_TracktionMarketplaceStatus.h"
#endif

#include "marketplace/higgs_KeyFileGeneration.h"

#if HIGGS_MODULE_AVAILABLE_higgs_gui_extra
 #include "marketplace/higgs_OnlineUnlockForm.h"
#endif
