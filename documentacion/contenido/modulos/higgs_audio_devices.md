# higgs_audio_devices

Entrada y salida de audio y MIDI: `AudioDeviceManager`, tipos de dispositivo por plataforma (CoreAudio, WASAPI, DirectSound, ASIO, ALSA, JACK, Oboe, OpenSL ES), `MidiInput`/`MidiOutput`, extremos y sesiones UMP, y fuentes de reproducción (`AudioTransportSource`).

## Ficha

| Campo | Valor |
|---|---|
| Identificador | `higgs_audio_devices` |
| Versión | 9.0.5 |
| Nombre declarado | Higgs Mythos audio and MIDI I/O device classes |
| Descripción declarada | Classes to play and record from audio and MIDI I/O devices |
| Estándar mínimo de C++ | C++17 |
| Licencia | AGPLv3 |
| Dependencias directas | [`higgs_audio_basics`](higgs_audio_basics.md), [`higgs_events`](higgs_events.md) |
| Dependencias indirectas | [`higgs_core`](higgs_core.md) |
| Módulos que dependen de él | [`higgs_audio_utils`](higgs_audio_utils.md) |
| Frameworks de macOS | `CoreAudio`, `CoreMIDI`, `AudioToolbox` |
| Frameworks de iOS | `CoreAudio`, `CoreMIDI`, `AudioToolbox`, `AVFoundation` |
| Paquetes pkg-config (Linux) | `alsa` |
| Código fuente | [`modules/higgs_audio_devices`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_devices) |

## Uso

```cmake
target_link_libraries(MiTarget PRIVATE higgs::higgs_audio_devices)
```

```cpp
#include <higgs_audio_devices/higgs_audio_devices.h>
```

Guías relacionadas: [Audio y dispositivos](../guias/audio.md), [MIDI, MPE y MIDI 2.0](../guias/midi.md).

## Opciones de configuración

Se cambian con `target_compile_definitions(MiTarget PUBLIC OPCION=valor)` o en la sección *Modules* de Mythos Studio.

| Opción | Por defecto | Descripción |
|---|---|---|
| `HIGGS_USE_WINRT_MIDI` | `0` | Habilita el uso de la API de Windows Runtime para MIDI, lo que permite conectarse a dispositivos Bluetooth Low Energy en Windows 10 versión 1809 (October 2018 Update) y posteriores. Si habilita este indicador, las versiones anteriores de Windows recurrirán automáticamente a la API MIDI de Win32 habitual. Para compilar necesitará la versión 10.0.14393.0 del Windows Standalone SDK, y es posible que deba agregar la ruta a los encabezados de WinRT. La ruta a los encabezados será similar a `C:\Program Files (x86)\Windows Kits\10\Include\10.0.14393.0\winrt`. |
| `HIGGS_USE_WINDOWS_MIDI_SERVICES` | `0` | Habilita el uso de Windows MIDI Services, lo que a su vez habilita la compatibilidad con MIDI 2.0 en algunas versiones de Windows. Al momento de escribir esto, solo funciona en la compilación Canary del programa Insider de Windows 11, pero con el tiempo también funcionará en la versión final de Windows 10. Para habilitar esta funcionalidad, las versiones de destino y mínima del Windows SDK deben establecerse como mínimo en 10.0.20348.0. Esta funcionalidad depende, en tiempo de compilación, de la herramienta CppWinRT y del paquete NuGet de Windows MIDI Services. Si usa Mythos Studio, esto se configurará automáticamente al habilitar este indicador. Si usa CMake, deberá pasar `NEEDS_WINDOWS_MIDI_SERVICES TRUE` a su llamada a `higgs_add_<target>`. Actualmente, el paquete NuGet de Windows MIDI Services no está alojado en el repositorio oficial. Para que NuGet y/o Visual Studio puedan encontrar el paquete, deberá configurar un repositorio local que apunte a una carpeta local que contenga el paquete. Puede hacerlo desde la interfaz de administración de paquetes de Visual Studio. Dado que esta API se implementa con código generado por la herramienta CppWinRT, solo pueden compilarla los compiladores que entienden el código generado. Lo más probable es que MSVC funcione; algunas versiones de Clang podrían funcionar, pero no está garantizado. Incluso una vez que Windows MIDI Services sea estable y esté habilitado en Windows, el servicio no podrá usarse hasta que se instale el paquete SDK del servicio. Por lo tanto, deberá asegurarse de que el instalador de su software también instale este paquete SDK, o de que informe al usuario cómo instalar la dependencia. Si el SDK no está disponible, HIGGS recurrirá a la API WinRT anterior (si está habilitada) o, en caso contrario, a la API Win32. Estas API no admiten MIDI 2.0. |
| `HIGGS_ASIO` | `0` | Habilita los dispositivos de audio ASIO (solo MS Windows). IMPORTANTE: activar esta opción hará que su programa dependa de archivos fuente del ASIO SDK de Steinberg, por lo que el programa resultante deberá cumplir los términos de licencia de esos archivos. De forma predeterminada, esta opción usará los encabezados ASIO integrados que se distribuyen junto con HIGGS. Consulte la documentación de `HIGGS_ASIO_USE_EXTERNAL_SDK` para saber cómo usar una versión diferente de esos encabezados. |
| `HIGGS_ASIO_USE_EXTERNAL_SDK` | `0` | El comportamiento predeterminado del indicador `HIGGS_ASIO` es incluir un encabezado integrado del ASIO SDK. Si necesita proporcionar su propia versión de este encabezado en lugar de usar la integrada, puede habilitar este indicador. En ese caso, también debe asegurarse de que el archivo `iasiodrv.h` del SDK pueda localizarse mediante las rutas de inclusión de encabezados de su proyecto. La opción solo tiene efecto cuando `HIGGS_ASIO` también está habilitado. |
| `HIGGS_WASAPI` | `1` | Habilita los dispositivos de audio WASAPI (Windows Vista y posteriores). |
| `HIGGS_DIRECTSOUND` | `1` | Habilita el audio DirectSound (solo MS Windows). |
| `HIGGS_ALSA` | `1` | Habilita los dispositivos de audio ALSA (solo Linux). |
| `HIGGS_JACK` | `0` | Habilita los dispositivos de audio JACK. |
| `HIGGS_USE_ANDROID_OBOE` | `1` | Habilita los dispositivos Oboe (solo Android). |
| `HIGGS_USE_OBOE_STABILIZED_CALLBACK` | `0` | Si `HIGGS_USE_ANDROID_OBOE` está habilitado, habilitar esta opción envolverá los flujos de audio de salida en la clase `oboe::StabilizedCallback`. Esta clase intenta mantener la CPU ocupada para evitar que se reduzca su frecuencia en ciertos dispositivos. (Solo Android). |
| `HIGGS_USE_ANDROID_OPENSLES` | 1 si `HIGGS_USE_ANDROID_OBOE` es 0; si no, 0 | Habilita los dispositivos OpenSLES (solo Android). |
| `HIGGS_DISABLE_AUDIO_MIXING_WITH_OTHER_APPS` | `0` | Activar esta opción otorga a su aplicación acceso exclusivo al audio del sistema en las plataformas que lo admiten (actualmente, solo iOS). |

## Código de terceros incluido

| Componente | Licencia | Ubicación |
|---|---|---|
| [Oboe](https://github.com/google/oboe) | `Apache-2.0` | `modules/higgs_audio_devices/native/oboe` |
| [ASIO SDK](https://www.steinberg.net/developers) | `LicenseRef-Steinberg-ASIO OR GPL-3.0-only` | `modules/higgs_audio_devices/native/asio` |

## Catálogo de clases

28 clases y estructuras públicas declaradas en las cabeceras que incluye `higgs_audio_devices.h`, agrupadas por carpeta. La descripción es la primera frase de su comentario de documentación en el código.

### `midi_io/ump`

| Clase | Descripción | Cabecera |
|---|---|---|
| `EndpointId` | Identifica un endpoint MIDI. | [`higgs_UMPEndpointId.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_devices/midi_io/ump/higgs_UMPEndpointId.h) |
| `StaticDeviceInfo` | Información estática sobre un dispositivo MIDI concreto que se puede consultar sin abrir una conexión con el dispositivo. | [`higgs_UMPStaticDeviceInfo.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_devices/midi_io/ump/higgs_UMPStaticDeviceInfo.h) |
| `Block` | Representa un bloque de función (FB) o un bloque terminal de grupo (GTB). | [`higgs_UMPBlock.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_devices/midi_io/ump/higgs_UMPBlock.h) |
| `Endpoint` | Representa un único endpoint MIDI, que puede tener como máximo una entrada y una salida. | [`higgs_UMPEndpoint.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_devices/midi_io/ump/higgs_UMPEndpoint.h) |
| `DisconnectionListener` | Una clase de interfaz para entidades interesadas en recibir notificaciones de desconexión. | [`higgs_UMPDisconnectionListener.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_devices/midi_io/ump/higgs_UMPDisconnectionListener.h) |
| `Consumer` | Una clase de interfaz para entidades que consumen Universal MIDI Packets provenientes de algún productor. | [`higgs_UMPInput.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_devices/midi_io/ump/higgs_UMPInput.h) |
| `Input` | Entrada (desde la perspectiva del proyecto HIGGS) que recibe mensajes enviados por un endpoint. | [`higgs_UMPInput.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_devices/midi_io/ump/higgs_UMPInput.h) |
| `Output` | Una salida (desde la perspectiva del proyecto HIGGS) que envía mensajes a un endpoint. | [`higgs_UMPOutput.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_devices/midi_io/ump/higgs_UMPOutput.h) |
| `LegacyVirtualInput` | Representa un puerto de entrada MIDI 1.0 virtual. | [`higgs_UMPLegacyVirtualInput.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_devices/midi_io/ump/higgs_UMPLegacyVirtualInput.h) |
| `LegacyVirtualOutput` | Representa un puerto de salida MIDI 1.0 virtual. | [`higgs_UMPLegacyVirtualOutput.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_devices/midi_io/ump/higgs_UMPLegacyVirtualOutput.h) |
| `VirtualEndpoint` | Representa un dispositivo virtual que permite a este programa anunciarse ante otras aplicaciones compatibles con MIDI del sistema. | [`higgs_UMPVirtualEndpoint.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_devices/midi_io/ump/higgs_UMPVirtualEndpoint.h) |
| `Session` | Permite crear nuevas conexiones con endpoints y también crear nuevos endpoints virtuales. | [`higgs_UMPSession.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_devices/midi_io/ump/higgs_UMPSession.h) |
| `EndpointsListener` | Una clase de interfaz para tipos interesados en recibir actualizaciones sobre cambios en los endpoints MIDI disponibles. | [`higgs_UMPEndpoints.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_devices/midi_io/ump/higgs_UMPEndpoints.h) |
| `Endpoints` | Endpoints conocidos por el sistema. | [`higgs_UMPEndpoints.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_devices/midi_io/ump/higgs_UMPEndpoints.h) |

### `midi_io`

| Clase | Descripción | Cabecera |
|---|---|---|
| `MidiDeviceListConnection` | Para saber cuándo cambian los dispositivos MIDI disponibles, llame a MidiDeviceListConnection::make() pasando una lambda que se invocará en cada cambio de configuración. | [`higgs_MidiDevices.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_devices/midi_io/higgs_MidiDevices.h) |
| `MidiDeviceInfo` | Esta estructura contiene información sobre un puerto de entrada o salida MIDI 1.0. | [`higgs_MidiDevices.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_devices/midi_io/higgs_MidiDevices.h) |
| `MidiInput` | Representa un dispositivo de entrada MIDI que usa el antiguo formato de flujo de bytes. | [`higgs_MidiDevices.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_devices/midi_io/higgs_MidiDevices.h) |
| `MidiInputCallback` | Recibe mensajes entrantes de un dispositivo de entrada MIDI físico. | [`higgs_MidiDevices.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_devices/midi_io/higgs_MidiDevices.h) |
| `MidiOutput` | Representa un dispositivo de salida MIDI que usa el antiguo formato de flujo de bytes. | [`higgs_MidiDevices.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_devices/midi_io/higgs_MidiDevices.h) |
| `MidiMessageCollector` | Recopila los mensajes MIDI entrantes en tiempo real y los convierte en bloques adecuados para su procesamiento por un callback de audio basado en bloques. | [`higgs_MidiMessageCollector.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_devices/midi_io/higgs_MidiMessageCollector.h) |

### `audio_io`

| Clase | Descripción | Cabecera |
|---|---|---|
| `AudioIODeviceCallbackContext` | Información adicional que puede pasarse al AudioIODeviceCallback. | [`higgs_AudioIODevice.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_devices/audio_io/higgs_AudioIODevice.h) |
| `AudioIODeviceCallback` | Uno de estos se pasa a un objeto AudioIODevice para transmitir los datos de audio de entrada y salida. | [`higgs_AudioIODevice.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_devices/audio_io/higgs_AudioIODevice.h) |
| `AudioIODevice` | Clase base para un dispositivo de audio con canales de entrada y salida sincronizados. | [`higgs_AudioIODevice.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_devices/audio_io/higgs_AudioIODevice.h) |
| `AudioIODeviceType` | Representa un tipo de controlador de audio, como DirectSound, ASIO, CoreAudio, etc. | [`higgs_AudioIODeviceType.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_devices/audio_io/higgs_AudioIODeviceType.h) |
| `SystemAudioVolume` | Contiene funciones para controlar el volumen maestro del sistema. | [`higgs_SystemAudioVolume.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_devices/audio_io/higgs_SystemAudioVolume.h) |
| `AudioDeviceManager` | Gestiona el estado de algunos dispositivos de E/S de audio y MIDI. | [`higgs_AudioDeviceManager.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_devices/audio_io/higgs_AudioDeviceManager.h) |

### `sources`

| Clase | Descripción | Cabecera |
|---|---|---|
| `AudioSourcePlayer` | Clase envoltorio para transmitir audio de forma continua desde una fuente de audio hacia un AudioIODevice. | [`higgs_AudioSourcePlayer.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_devices/sources/higgs_AudioSourcePlayer.h) |
| `AudioTransportSource` | AudioSource que toma un PositionableAudioSource y permite reproducirlo, detenerlo, iniciarlo, etc. | [`higgs_AudioTransportSource.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_devices/sources/higgs_AudioTransportSource.h) |
