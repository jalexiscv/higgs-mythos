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
#define UNITY_AUDIO_PLUGIN_API_VERSION 0x010401

#if HIGGS_WINDOWS
 #define UNITY_INTERFACE_API __stdcall
 #define UNITY_INTERFACE_EXPORT __declspec (dllexport)
#else
 #define UNITY_INTERFACE_API
 #define UNITY_INTERFACE_EXPORT __attribute__ ((visibility ("default")))
#endif

//==============================================================================
struct UnityAudioEffectState;

typedef int  (UNITY_INTERFACE_API * createCallback)              (UnityAudioEffectState* state);
typedef int  (UNITY_INTERFACE_API * releaseCallback)             (UnityAudioEffectState* state);
typedef int  (UNITY_INTERFACE_API * resetCallback)               (UnityAudioEffectState* state);

typedef int  (UNITY_INTERFACE_API * processCallback)             (UnityAudioEffectState* state, float* inBuffer, float* outBuffer, unsigned int bufferSize,
                                                                  int numInChannels, int numOutChannels);

typedef int  (UNITY_INTERFACE_API * setPositionCallback)         (UnityAudioEffectState* state, unsigned int pos);

typedef int  (UNITY_INTERFACE_API * setFloatParameterCallback)   (UnityAudioEffectState* state, int index, float value);
typedef int  (UNITY_INTERFACE_API * getFloatParameterCallback)   (UnityAudioEffectState* state, int index, float* value, char* valuestr);
typedef int  (UNITY_INTERFACE_API * getFloatBufferCallback)      (UnityAudioEffectState* state, const char* name, float* buffer, int numsamples);

typedef int  (UNITY_INTERFACE_API * distanceAttenuationCallback) (UnityAudioEffectState* state, float distanceIn, float attenuationIn, float* attenuationOut);

typedef void (UNITY_INTERFACE_API * renderCallback)              (int eventId);

//==============================================================================
enum UnityAudioEffectDefinitionFlags
{
    isSideChainTarget = 1,
    isSpatializer = 2,
    isAmbisonicDecoder = 4,
    appliesDistanceAttenuation = 8
};

enum UnityAudioEffectStateFlags
{
    stateIsPlaying = 1,
    stateIsPaused = 2,
    stateIsMuted = 8,
    statIsSideChainTarget = 16
};

enum UnityEventModifiers
{
    shift = 1,
    control = 2,
    alt = 4,
    command = 8,
    numeric = 16,
    capsLock = 32,
    functionKey = 64
};

//==============================================================================
#ifndef DOXYGEN

struct UnityAudioSpatializerData
{
    float                          listenerMatrix[16];
    float                          sourceMatrix[16];
    float                          spatialBlend;
    float                          reverbZoneMix;
    float                          spread;
    float                          stereoPan;
    distanceAttenuationCallback    attenuationCallback;
    float                          minDistance;
    float                          maxDistance;
};

struct UnityAudioAmbisonicData
{
    float                          listenerMatrix[16];
    float                          sourceMatrix[16];
    float                          spatialBlend;
    float                          reverbZoneMix;
    float                          spread;
    float                          stereoPan;
    distanceAttenuationCallback    attenuationCallback;
    int                            ambisonicOutChannels;
    float                          volume;
};

struct UnityAudioEffectState
{
    higgs::uint32               structSize;
    higgs::uint32               sampleRate;
    higgs::uint64               dspCurrentTick;
    higgs::uint64               dspPreviousTick;
    float*                     sidechainBuffer;
    void*                      effectData;
    higgs::uint32               flags;
    void*                      internal;

    UnityAudioSpatializerData* spatializerData;
    higgs::uint32               dspBufferSize;
    higgs::uint32               hostAPIVersion;

    UnityAudioAmbisonicData*   ambisonicData;

    template <typename T>
    inline T* getEffectData() const
    {
        jassert (effectData != nullptr);
        jassert (internal != nullptr);

        return (T*) effectData;
    }
};

struct UnityAudioParameterDefinition
{
    char        name[16];
    char        unit[16];
    const char* description;
    float       min;
    float       max;
    float       defaultVal;
    float       displayScale;
    float       displayExponent;
};

struct UnityAudioEffectDefinition
{
    higgs::uint32                   structSize;
    higgs::uint32                   parameterStructSize;
    higgs::uint32                   apiVersion;
    higgs::uint32                   pluginVersion;
    higgs::uint32                   channels;
    higgs::uint32                   numParameters;
    higgs::uint64                   flags;
    char                           name[32];
    createCallback                 create;
    releaseCallback                release;
    resetCallback                  reset;
    processCallback                process;
    setPositionCallback            setPosition;
    UnityAudioParameterDefinition* parameterDefintions;
    setFloatParameterCallback      setFloatParameter;
    getFloatParameterCallback      getFloatParameter;
    getFloatBufferCallback         getFloatBuffer;
};

#endif

//==============================================================================
// Unity callback
extern "C" UNITY_INTERFACE_EXPORT int  UNITY_INTERFACE_API UnityGetAudioEffectDefinitions (UnityAudioEffectDefinition*** definitionsPtr);

// GUI script callbacks
extern "C" UNITY_INTERFACE_EXPORT renderCallback UNITY_INTERFACE_API getRenderCallback();

extern "C" UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API unityInitialiseTexture (int id, void* textureHandle, int w, int h);

extern "C" UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API unityMouseDown (int id, float x, float y, UnityEventModifiers mods, int button);
extern "C" UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API unityMouseDrag (int id, float x, float y, UnityEventModifiers mods, int button);
extern "C" UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API unityMouseUp   (int id, float x, float y, UnityEventModifiers mods);

extern "C" UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API unityKeyEvent (int id, int code, UnityEventModifiers mods, const char* name);

extern "C" UNITY_INTERFACE_EXPORT void UNITY_INTERFACE_API unitySetScreenBounds (int id, float x, float y, float w, float h);
