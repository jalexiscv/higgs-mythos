# higgs_audio_utils

Utilidades de audio con interfaz y misceláneas: `AudioAppComponent`, `AudioDeviceSelectorComponent`, `AudioThumbnail`, `AudioVisualiserComponent`, teclados MIDI y MPE, emparejamiento de dispositivos MIDI Bluetooth, reproductores de audio y lectura/grabación de CD.

## Ficha

| Campo | Valor |
|---|---|
| Identificador | `higgs_audio_utils` |
| Versión | 9.0.5 |
| Nombre declarado | Higgs Mythos extra audio utility classes |
| Descripción declarada | Classes for audio-related GUI and miscellaneous tasks. |
| Estándar mínimo de C++ | C++17 |
| Licencia | AGPLv3 |
| Dependencias directas | [`higgs_audio_processors`](higgs_audio_processors.md), [`higgs_audio_formats`](higgs_audio_formats.md), [`higgs_audio_devices`](higgs_audio_devices.md) |
| Dependencias indirectas | [`higgs_core`](higgs_core.md), [`higgs_events`](higgs_events.md), [`higgs_data_structures`](higgs_data_structures.md), [`higgs_graphics`](higgs_graphics.md), [`higgs_gui_basics`](higgs_gui_basics.md), [`higgs_gui_extra`](higgs_gui_extra.md), [`higgs_audio_basics`](higgs_audio_basics.md), [`higgs_audio_processors_headless`](higgs_audio_processors_headless.md) |
| Frameworks de macOS | `CoreAudioKit`, `DiscRecording` |
| Frameworks de iOS | `CoreAudioKit` |
| Código fuente | [`modules/higgs_audio_utils`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_utils) |

## Uso

```cmake
target_link_libraries(MiTarget PRIVATE higgs::higgs_audio_utils)
```

```cpp
#include <higgs_audio_utils/higgs_audio_utils.h>
```

Guías relacionadas: [Audio y dispositivos](../guias/audio.md).

## Opciones de configuración

Se cambian con `target_compile_definitions(MiTarget PUBLIC OPCION=valor)` o en la sección *Modules* de Mythos Studio.

| Opción | Por defecto | Descripción |
|---|---|---|
| `HIGGS_USE_CDREADER` | `0` | Habilita la clase `AudioCDReader` (en las plataformas compatibles). |
| `HIGGS_USE_CDBURNER` | `0` | Habilita la clase `AudioCDBurner` (en las plataformas compatibles). |

## Catálogo de clases

13 clases y estructuras públicas declaradas en las cabeceras que incluye `higgs_audio_utils.h`, agrupadas por carpeta. La descripción es la primera frase de su comentario de documentación en el código.

### `gui`

| Clase | Descripción | Cabecera |
|---|---|---|
| `AudioDeviceSelectorComponent` | Un componente con controles que permiten al usuario cambiar la configuración de audio de un objeto AudioDeviceManager. | [`higgs_AudioDeviceSelectorComponent.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_utils/gui/higgs_AudioDeviceSelectorComponent.h) |
| `AudioThumbnailBase` | Proporciona una base para clases que pueden almacenar y dibujar vistas escaladas de una forma de onda de audio. | [`higgs_AudioThumbnailBase.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_utils/gui/higgs_AudioThumbnailBase.h) |
| `AudioThumbnail` | Facilita dibujar rápidamente vistas escaladas de la forma de onda de un archivo de audio. | [`higgs_AudioThumbnail.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_utils/gui/higgs_AudioThumbnail.h) |
| `AudioThumbnailCache` | Una instancia de esta clase se usa para gestionar múltiples objetos AudioThumbnail. | [`higgs_AudioThumbnailCache.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_utils/gui/higgs_AudioThumbnailCache.h) |
| `AudioVisualiserComponent` | Componente sencillo que se puede usar para mostrar una forma de onda desplazable de datos de audio. | [`higgs_AudioVisualiserComponent.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_utils/gui/higgs_AudioVisualiserComponent.h) |
| `KeyboardComponentBase` | Una clase base para dibujar un componente de teclado MIDI personalizado. | [`higgs_KeyboardComponentBase.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_utils/gui/higgs_KeyboardComponentBase.h) |
| `MidiKeyboardComponent` | Un componente que muestra un teclado de piano en cuyas notas se puede hacer clic. | [`higgs_MidiKeyboardComponent.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_utils/gui/higgs_MidiKeyboardComponent.h) |
| `MPEKeyboardComponent` | Un componente que muestra un teclado compatible con MPE, en cuyas notas se puede hacer clic. | [`higgs_MPEKeyboardComponent.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_utils/gui/higgs_MPEKeyboardComponent.h) |
| `AudioAppComponent` | Una clase base para escribir aplicaciones de audio que transmiten audio desde los dispositivos de E/S de audio. | [`higgs_AudioAppComponent.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_utils/gui/higgs_AudioAppComponent.h) |
| `BluetoothMidiDevicePairingDialogue` | Abre un diálogo de emparejamiento MIDI por Bluetooth que permite al usuario ver los dispositivos MIDI Bluetooth que se encuentran cerca y conectarse a ellos. | [`higgs_BluetoothMidiDevicePairingDialogue.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_utils/gui/higgs_BluetoothMidiDevicePairingDialogue.h) |

### `players`

| Clase | Descripción | Cabecera |
|---|---|---|
| `SoundPlayer` | Un reproductor de sonido simple que se puede agregar al AudioDeviceManager para reproducir sonidos sencillos. | [`higgs_SoundPlayer.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_utils/players/higgs_SoundPlayer.h) |
| `AudioProcessorPlayer` | Un objeto AudioIODeviceCallback que transmite audio a través de un AudioProcessor. | [`higgs_AudioProcessorPlayer.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_utils/players/higgs_AudioProcessorPlayer.h) |

### `audio_cd`

| Clase | Descripción | Cabecera |
|---|---|---|
| `AudioCDReader` | Un tipo de AudioFormatReader que lee desde un CD de audio. | [`higgs_AudioCDReader.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_utils/audio_cd/higgs_AudioCDReader.h) |
