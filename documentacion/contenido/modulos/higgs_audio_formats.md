# higgs_audio_formats

Lectura y escritura de archivos de audio: WAV, AIFF, FLAC, Ogg Vorbis, Opus, MP3, CoreAudio, Windows Media y LAME; gestor de formatos, lectores y escritores (incluido el escritor en hilo de fondo) y un sampler sencillo.

## Ficha

| Campo | Valor |
|---|---|
| Identificador | `higgs_audio_formats` |
| Versión | 9.0.5 |
| Nombre declarado | Higgs Mythos audio file format codecs |
| Descripción declarada | Classes for reading and writing various audio file formats. |
| Estándar mínimo de C++ | C++17 |
| Licencia | AGPLv3 |
| Dependencias directas | [`higgs_audio_basics`](higgs_audio_basics.md) |
| Dependencias indirectas | [`higgs_core`](higgs_core.md) |
| Módulos que dependen de él | [`higgs_audio_utils`](higgs_audio_utils.md), [`higgs_dsp`](higgs_dsp.md) |
| Frameworks de macOS | `CoreAudio`, `CoreMIDI`, `QuartzCore`, `AudioToolbox` |
| Frameworks de iOS | `AudioToolbox`, `QuartzCore` |
| Código fuente | [`modules/higgs_audio_formats`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_formats) |

## Uso

```cmake
target_link_libraries(MiTarget PRIVATE higgs::higgs_audio_formats)
```

```cpp
#include <higgs_audio_formats/higgs_audio_formats.h>
```

Guías relacionadas: [Audio y dispositivos](../guias/audio.md#leer-y-reproducir-archivos).

## Opciones de configuración

Se cambian con `target_compile_definitions(MiTarget PUBLIC OPCION=valor)` o en la sección *Modules* de Mythos Studio.

| Opción | Por defecto | Descripción |
|---|---|---|
| `HIGGS_USE_FLAC` | `1` | Habilita las clases del códec de audio FLAC (disponibles en todas las plataformas). Si su aplicación no necesita leer archivos FLAC, quizá le convenga deshabilitar esta opción para reducir el tamaño de su código base y el tiempo de compilación. |
| `HIGGS_INCLUDE_FLAC_CODE` | `1` | Esta opción permite deshabilitar el código de terceros de `flaclib` integrado en HIGGS. Es posible que deba ajustarla si enlaza su aplicación con una biblioteca `flaclib` externa, pero en aplicaciones normales esta opción debe dejarse como está. Si la deshabilita, quizá también le convenga establecer un valor para `HIGGS_FLAC_INCLUDE_PATH`, para especificar la ruta donde se encuentran sus encabezados de `flaclib`. Esta opción de configuración solo tiene efecto cuando `HIGGS_USE_FLAC` está habilitado. |
| `HIGGS_USE_OGGVORBIS` | `1` | Habilita las clases del códec de audio Ogg-Vorbis (disponibles en todas las plataformas). Si su aplicación no necesita leer archivos Ogg-Vorbis, quizá le convenga deshabilitar esta opción para reducir el tamaño de su código base y el tiempo de compilación. |
| `HIGGS_USE_OPUS` | `1` | Habilita las clases del códec de audio Opus (disponibles en todas las plataformas). Si su aplicación no necesita leer ni escribir archivos Opus, quizá le convenga deshabilitar esta opción para reducir el tamaño de su código base y el tiempo de compilación. |
| `HIGGS_INCLUDE_OPUS_CODE` | `1` | Esta opción permite deshabilitar el código de terceros de `opus`, `opusfile` y `libopusenc` integrado en HIGGS. Es posible que deba ajustarla si enlaza su aplicación con copias externas de estas bibliotecas, pero en aplicaciones normales esta opción debe dejarse como está. La `libogg` integrada se compila cuando esta opción o `HIGGS_INCLUDE_OGGVORBIS_CODE` está habilitada. Si solo una de ellas lo está, puede entrar en conflicto con la `libogg` de la que depende la biblioteca externa. Esta opción de configuración solo tiene efecto cuando `HIGGS_USE_OPUS` está habilitado. |
| `HIGGS_USE_MP3AUDIOFORMAT` | `1` | Habilita la clase `MP3AudioFormat`, basada en software. Si su aplicación no necesita leer archivos MP3, quizá le convenga deshabilitar esta opción para reducir el tamaño de su código base y el tiempo de compilación. |
| `HIGGS_USE_LAME_AUDIO_FORMAT` | `0` | Habilita la clase `LameEncoderAudioFormat`. |
| `HIGGS_USE_WINDOWS_MEDIA_FORMAT` | `1` | Habilita los códecs del Windows Media SDK. |

## Código de terceros incluido

| Componente | Licencia | Ubicación |
|---|---|---|
| [FLAC](https://xiph.org/flac) | `BSD-3-Clause` | `modules/higgs_audio_formats/codecs/flac` |
| [libogg](https://xiph.org/ogg) | `BSD-3-Clause` | `modules/higgs_audio_formats/codecs/ogg` |
| [libvorbis](https://xiph.org/vorbis) | `BSD-3-Clause` | `modules/higgs_audio_formats/codecs/vorbis` |
| [Opus](https://opus-codec.org) | `BSD-3-Clause` | `modules/higgs_audio_formats/codecs/opus/opus` |
| [opusfile](https://opus-codec.org) | `BSD-3-Clause` | `modules/higgs_audio_formats/codecs/opus/opusfile` |
| [libopusenc](https://opus-codec.org) | `BSD-3-Clause` | `modules/higgs_audio_formats/codecs/opus/libopusenc` |

## Catálogo de clases

22 clases y estructuras públicas declaradas en las cabeceras que incluye `higgs_audio_formats.h`, agrupadas por carpeta. La descripción es la primera frase de su comentario de documentación en el código.

### `format`

| Clase | Descripción | Cabecera |
|---|---|---|
| `AudioFormatReader` | Lee muestras de un flujo de archivo de audio. | [`higgs_AudioFormatReader.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_formats/format/higgs_AudioFormatReader.h) |
| `AudioFormatWriterOptions` | Opciones que afectan el formato de los datos de salida producidos por un AudioFormatWriter. | [`higgs_AudioFormatWriterOptions.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_formats/format/higgs_AudioFormatWriterOptions.h) |
| `AudioFormatWriter` | Escribe muestras en un flujo de archivo de audio. | [`higgs_AudioFormatWriter.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_formats/format/higgs_AudioFormatWriter.h) |
| `MemoryMappedAudioFormatReader` | Un tipo especializado de AudioFormatReader que usa un MemoryMappedFile para leer directamente de un archivo de audio. | [`higgs_MemoryMappedAudioFormatReader.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_formats/format/higgs_MemoryMappedAudioFormatReader.h) |
| `AudioFormat` | Las subclases de AudioFormat se usan para leer y escribir distintos formatos de archivo de audio. | [`higgs_AudioFormat.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_formats/format/higgs_AudioFormat.h) |
| `AudioFormatManager` | Una clase para mantener una lista de los formatos de audio disponibles y decidir cuál usar para abrir un archivo determinado. | [`higgs_AudioFormatManager.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_formats/format/higgs_AudioFormatManager.h) |
| `AudioFormatReaderSource` | Un tipo de AudioSource que lee desde un AudioFormatReader. | [`higgs_AudioFormatReaderSource.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_formats/format/higgs_AudioFormatReaderSource.h) |
| `AudioSubsectionReader` | Esta clase se usa para envolver un AudioFormatReader y leer solo una subsección del archivo. | [`higgs_AudioSubsectionReader.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_formats/format/higgs_AudioSubsectionReader.h) |
| `BufferingAudioReader` | Un AudioFormatReader que usa un hilo en segundo plano para leer por adelantado datos de otro lector. | [`higgs_BufferingAudioFormatReader.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_formats/format/higgs_BufferingAudioFormatReader.h) |
| `ARAAudioSourceReader` | Subclase de AudioFormatReader que lee muestras de una única fuente de audio ARA. | [`higgs_ARAAudioReaders.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_formats/format/higgs_ARAAudioReaders.h) |
| `ARAPlaybackRegionReader` | Subclase de AudioFormatReader que lee muestras de un grupo de regiones de reproducción. | [`higgs_ARAAudioReaders.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_formats/format/higgs_ARAAudioReaders.h) |

### `codecs`

| Clase | Descripción | Cabecera |
|---|---|---|
| `AiffAudioFormat` | Lee y escribe archivos de audio en formato AIFF. | [`higgs_AiffAudioFormat.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_formats/codecs/higgs_AiffAudioFormat.h) |
| `CoreAudioFormat` | Solo para OSX e iOS: usa el framework AudioToolbox para leer cualquier formato de audio para el que el sistema tenga un códec. | [`higgs_CoreAudioFormat.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_formats/codecs/higgs_CoreAudioFormat.h) |
| `FlacAudioFormat` | Lee y escribe el formato de audio FLAC con compresión sin pérdida. | [`higgs_FlacAudioFormat.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_formats/codecs/higgs_FlacAudioFormat.h) |
| `LAMEEncoderAudioFormat` | Clase AudioFormat que puede usar una versión instalada del codificador de mp3 LAME para codificar un archivo. | [`higgs_LAMEEncoderAudioFormat.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_formats/codecs/higgs_LAMEEncoderAudioFormat.h) |
| `MP3AudioFormat` | Formato de decodificación de MP3 basado en software (actualmente no proporciona un codificador). | [`higgs_MP3AudioFormat.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_formats/codecs/higgs_MP3AudioFormat.h) |
| `OggVorbisAudioFormat` | Lee y escribe el formato de audio Ogg-Vorbis. | [`higgs_OggVorbisAudioFormat.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_formats/codecs/higgs_OggVorbisAudioFormat.h) |
| `OpusAudioFormat` | Lee y escribe el formato de audio Opus (archivos '.opus'). | [`higgs_OpusAudioFormat.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_formats/codecs/higgs_OpusAudioFormat.h) |
| `WavAudioFormat` | Lee y escribe archivos de audio en formato WAV. | [`higgs_WavAudioFormat.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_formats/codecs/higgs_WavAudioFormat.h) |
| `WindowsMediaAudioFormat` | Formato de audio que utiliza los códecs de Windows Media (solo en Windows). | [`higgs_WindowsMediaAudioFormat.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_formats/codecs/higgs_WindowsMediaAudioFormat.h) |

### `sampler`

| Clase | Descripción | Cabecera |
|---|---|---|
| `SamplerSound` | Subclase de SynthesiserSound que representa un clip de audio muestreado. | [`higgs_Sampler.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_formats/sampler/higgs_Sampler.h) |
| `SamplerVoice` | Una subclase de SynthesiserVoice que puede reproducir un SamplerSound. | [`higgs_Sampler.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_formats/sampler/higgs_Sampler.h) |
