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

#if HIGGS_WINDOWS
 #undef _WIN32_WINNT
 #define _WIN32_WINNT _WIN32_WINNT_WIN10
 #undef STRICT
 #define STRICT 1
 #include <windows.h>
 #include <float.h>
 #if HIGGS_MSVC
  #pragma warning (disable : 4312 4355)
 #endif
 #ifdef __INTEL_COMPILER
  #pragma warning (disable : 1899)
 #endif
#elif HIGGS_LINUX || HIGGS_BSD
 #include <float.h>
 #include <sys/time.h>
 #include <arpa/inet.h>
#elif HIGGS_MAC || HIGGS_IOS
 #ifdef __OBJC__
  #if HIGGS_MAC
   #include <Cocoa/Cocoa.h>
  #elif HIGGS_IOS
   #include <UIKit/UIKit.h>
  #else
   #error
  #endif
 #endif

 #include <objc/runtime.h>
 #include <objc/objc.h>
 #include <objc/message.h>
#endif
