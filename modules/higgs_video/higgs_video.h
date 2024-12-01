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

  ID:                 higgs_video
  vendor:             higgs
  version:            9.0.5
  name:               Higgs Mythos video playback and capture classes
  description:        Classes for playing video and capturing camera input.
  website:            https://github.com/jalexiscv/higgs-mythos
  license:            AGPLv3
  minimumCppStandard: 17

  dependencies:       higgs_gui_extra
  OSXFrameworks:      AVKit AVFoundation CoreMedia
  iOSFrameworks:      AVKit AVFoundation CoreMedia

 END_HIGGS_MODULE_DECLARATION

*******************************************************************************/


#pragma once
#define HIGGS_VIDEO_H_INCLUDED

//==============================================================================
#include <higgs_gui_extra/higgs_gui_extra.h>

//==============================================================================
/** Config: HIGGS_USE_CAMERA
    Enables camera support using the CameraDevice class (Mac, Windows, iOS, Android).
*/
#ifndef HIGGS_USE_CAMERA
 #define HIGGS_USE_CAMERA 0
#endif

#ifndef HIGGS_CAMERA_LOG_ENABLED
 #define HIGGS_CAMERA_LOG_ENABLED 0
#endif

#if HIGGS_CAMERA_LOG_ENABLED
 #define HIGGS_CAMERA_LOG(x) DBG(x)
#else
 #define HIGGS_CAMERA_LOG(x) {}
#endif

#if ! (HIGGS_MAC || HIGGS_WINDOWS || HIGGS_IOS || HIGGS_ANDROID)
 #undef HIGGS_USE_CAMERA
#endif

//==============================================================================
/** Config: HIGGS_SYNC_VIDEO_VOLUME_WITH_OS_MEDIA_VOLUME
    Enables synchronisation between video playback volume and OS media volume.
    Currently supported on Android only.
 */
#ifndef HIGGS_SYNC_VIDEO_VOLUME_WITH_OS_MEDIA_VOLUME
 #define HIGGS_SYNC_VIDEO_VOLUME_WITH_OS_MEDIA_VOLUME 1
#endif

#ifndef HIGGS_VIDEO_LOG_ENABLED
 #define HIGGS_VIDEO_LOG_ENABLED 1
#endif

#if HIGGS_VIDEO_LOG_ENABLED
 #define HIGGS_VIDEO_LOG(x) DBG(x)
#else
 #define HIGGS_VIDEO_LOG(x) {}
#endif

//==============================================================================
#include "playback/higgs_VideoComponent.h"
#include "capture/higgs_CameraDevice.h"
