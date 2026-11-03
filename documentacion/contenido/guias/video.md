# Vídeo y cámara

El módulo `higgs_video` reproduce vídeo y captura imágenes de la cámara usando las API nativas de
cada plataforma (AVFoundation/AVKit en macOS e iOS, y las implementaciones propias de Windows y
Android). Hay implementaciones para **macOS, iOS, Windows y Android**; no para Linux.

## Reproducir vídeo

```cpp
higgs::VideoComponent video { true };   // true = mostrar controles nativos (si la plataforma los ofrece)
addAndMakeVisible (video);

video.loadAsync (higgs::URL (archivo), [this] (const higgs::URL&, higgs::Result resultado)
{
    if (resultado.wasOk())
        video.play();
    else
        DBG (resultado.getErrorMessage());
});
```

`load()` (síncrono) acepta un `File` o una `URL`; en Android usa siempre `loadAsync()`. Otros
métodos: `play()`, `stop()`, `setPlayPosition()`, `setPlaySpeed()`, `setAudioVolume()`,
`getVideoDuration()` y `getVideoNativeSize()`. La opción `HIGGS_SYNC_VIDEO_VOLUME_WITH_OS_MEDIA_VOLUME`
(Android) sincroniza el volumen con el del sistema.

## Cámara

Activa `HIGGS_USE_CAMERA=1` (macOS, Windows, iOS y Android). En macOS e iOS añade además
`CAMERA_PERMISSION_ENABLED TRUE` y `CAMERA_PERMISSION_TEXT "…"` a tu target de CMake.

```cpp
auto nombres = higgs::CameraDevice::getAvailableDevices();

higgs::CameraDevice::openDeviceAsync (0, [this] (higgs::CameraDevice* dispositivo, const higgs::String& error)
{
    if (dispositivo == nullptr) { DBG (error); return; }

    camara.reset (dispositivo);
    visor.reset (camara->createViewerComponent());
    addAndMakeVisible (visor.get());
    resized();
});

// Hacer una foto:
camara->takeStillPicture ([] (const higgs::Image& foto) { /* guardar o mostrar */ });
```

En Android e iOS usa `openDeviceAsync()`, ya que el sistema puede pedir permiso al usuario.
`CameraDevice` también permite grabar vídeo a archivo (`startRecordingToFile`, `stopRecording`).

## Demos relacionadas

`VideoDemo` y `CameraDemo` (no se compilan en Linux).
