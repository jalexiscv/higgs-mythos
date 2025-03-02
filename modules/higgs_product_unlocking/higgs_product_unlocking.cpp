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

#ifdef HIGGS_PRODUCT_UNLOCKING_H_INCLUDED
 /* When you add this cpp file to your project, you mustn't include it in a file where you've
    already included any other headers - just put it inside a file on its own, possibly with your config
    flags preceding it, but don't include anything else. That also includes avoiding any automatic prefix
    header files that the compiler may be using.
 */
 #error "Incorrect use of HIGGS cpp file"
#endif

#define HIGGS_CORE_INCLUDE_JNI_HELPERS    1
#define HIGGS_CORE_INCLUDE_OBJC_HELPERS   1
#define HIGGS_CORE_INCLUDE_NATIVE_HEADERS 1

#ifdef HIGGS_IN_APP_PURCHASES_USE_SANDBOX_ENVIRONMENT
 HIGGS_COMPILER_WARNING ("HIGGS_IN_APP_PURCHASES_USE_SANDBOX_ENVIRONMENT is no longer supported")
#endif

#include "higgs_product_unlocking.h"

#if HIGGS_IOS || HIGGS_MAC
 #import <StoreKit/StoreKit.h>
#endif

#if HIGGS_IN_APP_PURCHASES
 #if HIGGS_ANDROID
  #include "native/higgs_InAppPurchases_android.cpp"
 #elif HIGGS_IOS || HIGGS_MAC
  #include "native/higgs_InAppPurchases_ios.cpp"
 #endif

 #include "in_app_purchases/higgs_InAppPurchases.cpp"
#endif

#include "marketplace/higgs_OnlineUnlockStatus.cpp"

#if HIGGS_MODULE_AVAILABLE_higgs_data_structures
 #include "marketplace/higgs_TracktionMarketplaceStatus.cpp"
#endif

#if HIGGS_MODULE_AVAILABLE_higgs_gui_extra
 #include "marketplace/higgs_OnlineUnlockForm.cpp"
#endif
