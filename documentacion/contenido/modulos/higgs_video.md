# higgs_video

Reproducción de vídeo (`VideoComponent`) y captura de cámara (`CameraDevice`) en macOS, iOS, Windows y Android.

## Ficha

| Campo | Valor |
|---|---|
| Identificador | `higgs_video` |
| Versión | 9.0.5 |
| Nombre declarado | Higgs Mythos video playback and capture classes |
| Descripción declarada | Classes for playing video and capturing camera input. |
| Estándar mínimo de C++ | C++17 |
| Licencia | AGPLv3 |
| Dependencias directas | [`higgs_gui_extra`](higgs_gui_extra.md) |
| Dependencias indirectas | [`higgs_core`](higgs_core.md), [`higgs_events`](higgs_events.md), [`higgs_data_structures`](higgs_data_structures.md), [`higgs_graphics`](higgs_graphics.md), [`higgs_gui_basics`](higgs_gui_basics.md) |
| Frameworks de macOS | `AVKit`, `AVFoundation`, `CoreMedia` |
| Frameworks de iOS | `AVKit`, `AVFoundation`, `CoreMedia` |
| Código fuente | [`modules/higgs_video`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_video) |

## Uso

```cmake
target_link_libraries(MiTarget PRIVATE higgs::higgs_video)
```

```cpp
#include <higgs_video/higgs_video.h>
```

Guías relacionadas: [Vídeo y cámara](../guias/video.md).

## Opciones de configuración

Se cambian con `target_compile_definitions(MiTarget PUBLIC OPCION=valor)` o en la sección *Modules* de Mythos Studio.

| Opción | Por defecto | Descripción |
|---|---|---|
| `HIGGS_USE_CAMERA` | `0` | Habilita la compatibilidad con cámaras mediante la clase `CameraDevice` (Mac, Windows, iOS, Android). |
| `HIGGS_SYNC_VIDEO_VOLUME_WITH_OS_MEDIA_VOLUME` | `1` | Habilita la sincronización entre el volumen de reproducción de video y el volumen multimedia del sistema operativo. Actualmente solo es compatible con Android. |

## Catálogo de clases

2 clases y estructuras públicas declaradas en las cabeceras que incluye `higgs_video.h`, agrupadas por carpeta. La descripción es la primera frase de su comentario de documentación en el código.

### `playback`

| Clase | Descripción | Cabecera |
|---|---|---|
| `VideoComponent` | Un componente que puede reproducir un video. | [`higgs_VideoComponent.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_video/playback/higgs_VideoComponent.h) |

### `capture`

| Clase | Descripción | Cabecera |
|---|---|---|
| `CameraDevice` | Controla los dispositivos de captura de video que puedan estar disponibles. | [`higgs_CameraDevice.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_video/capture/higgs_CameraDevice.h) |
