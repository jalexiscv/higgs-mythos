# higgs_audio_basics

Las clases de datos de audio y MIDI: `AudioBuffer`, conversiones de formato, operaciones vectoriales, fuentes de audio, sintetizador, MIDI 1.0, MPE, paquetes universales de MIDI 2.0 (UMP), información del cabezal de reproducción y utilidades (`SmoothedValue`, `ADSR`, `Reverb`, `Decibels`, interpoladores).

## Ficha

| Campo | Valor |
|---|---|
| Identificador | `higgs_audio_basics` |
| Versión | 9.0.5 |
| Nombre declarado | Higgs Mythos audio and MIDI data classes |
| Descripción declarada | Classes for audio buffer manipulation, midi message handling, synthesis, etc. |
| Estándar mínimo de C++ | C++17 |
| Licencia | AGPLv3 |
| Dependencias directas | [`higgs_core`](higgs_core.md) |
| Módulos que dependen de él | [`higgs_audio_devices`](higgs_audio_devices.md), [`higgs_audio_formats`](higgs_audio_formats.md), [`higgs_audio_processors_headless`](higgs_audio_processors_headless.md), [`higgs_midi_ci`](higgs_midi_ci.md) |
| Frameworks de macOS | `Accelerate` |
| Frameworks de iOS | `Accelerate` |
| Código fuente | [`modules/higgs_audio_basics`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_basics) |

## Uso

```cmake
target_link_libraries(MiTarget PRIVATE higgs::higgs_audio_basics)
```

```cpp
#include <higgs_audio_basics/higgs_audio_basics.h>
```

Guías relacionadas: [Audio y dispositivos](../guias/audio.md), [MIDI, MPE y MIDI 2.0](../guias/midi.md).

## Opciones de configuración

Este módulo no declara opciones de configuración.

## Catálogo de clases

62 clases y estructuras públicas declaradas en las cabeceras que incluye `higgs_audio_basics.h`, agrupadas por carpeta. La descripción es la primera frase de su comentario de documentación en el código.

### `buffers`

| Clase | Descripción | Cabecera |
|---|---|---|
| `AudioData` | Esta clase es un contenedor que agrupa todas las clases relacionadas con la clase de formato de muestras de audio AudioData::Pointer. | [`higgs_AudioDataConverters.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_basics/buffers/higgs_AudioDataConverters.h) |
| `AudioDataConverters` | Un conjunto de rutinas para convertir búferes de datos de punto flotante de 32 bits desde y hacia diversos formatos enteros. | [`higgs_AudioDataConverters.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_basics/buffers/higgs_AudioDataConverters.h) |
| `FloatVectorOperationsBase` | Una colección de operaciones vectoriales sencillas sobre arreglos de números de punto flotante, aceleradas con instrucciones SIMD cuando es posible, a las que normalmente se accede desde la clase FloatVectorOperations. | [`higgs_FloatVectorOperations.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_basics/buffers/higgs_FloatVectorOperations.h) |
| `FloatVectorOperations` | Una colección de operaciones vectoriales simples sobre arreglos de números de punto flotante, aceleradas con instrucciones SIMD cuando es posible, que proporciona todos los métodos de FloatVectorOperationsBase. | [`higgs_FloatVectorOperations.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_basics/buffers/higgs_FloatVectorOperations.h) |
| `ScopedNoDenormals` | Clase auxiliar que proporciona un mecanismo basado en RAII para desactivar temporalmente los números desnormalizados en la CPU. | [`higgs_FloatVectorOperations.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_basics/buffers/higgs_FloatVectorOperations.h) |
| `AudioBuffer` | Un búfer multicanal que contiene muestras de audio de punto flotante. | [`higgs_AudioSampleBuffer.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_basics/buffers/higgs_AudioSampleBuffer.h) |
| `AudioChannelSet` | Representa un conjunto de tipos de canales de audio. | [`higgs_AudioChannelSet.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_basics/buffers/higgs_AudioChannelSet.h) |
| `AudioProcessLoadMeasurer` | Mantiene una medición continua de la proporción de tiempo que se pasa dentro de un callback de audio. | [`higgs_AudioProcessLoadMeasurer.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_basics/buffers/higgs_AudioProcessLoadMeasurer.h) |

### `utilities`

| Clase | Descripción | Cabecera |
|---|---|---|
| `Decibels` | Esta clase contiene algunos métodos estáticos útiles para trabajar con valores en decibeles. | [`higgs_Decibels.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_basics/utilities/higgs_Decibels.h) |
| `IIRCoefficients` | Un conjunto de coeficientes para usar en un objeto IIRFilter. | [`higgs_IIRFilter.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_basics/utilities/higgs_IIRFilter.h) |
| `IIRFilterBase` | Un filtro IIR que puede realizar filtrado pasa bajos, pasa altos o pasa banda sobre una señal de audio. | [`higgs_IIRFilter.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_basics/utilities/higgs_IIRFilter.h) |
| `IIRFilter` | Un filtro IIR que puede realizar filtrado pasa bajos, pasa altos o pasa banda sobre una señal de audio, y que intenta implementar una seguridad para hilos básica. | [`higgs_IIRFilter.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_basics/utilities/higgs_IIRFilter.h) |
| `SingleThreadedIIRFilter` | Un filtro IIR que puede aplicar filtrado pasa bajos, pasa altos o pasa banda a una señal de audio, sin garantías de seguridad para hilos. | [`higgs_IIRFilter.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_basics/utilities/higgs_IIRFilter.h) |
| `GenericInterpolator` | Clase base de interpolador para remuestrear flujos de valores float. | [`higgs_GenericInterpolator.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_basics/utilities/higgs_GenericInterpolator.h) |
| `Interpolators` | Una colección de distintos interpoladores para remuestrear flujos de valores float. | [`higgs_Interpolators.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_basics/utilities/higgs_Interpolators.h) |
| `SmoothedValueBase` | Clase base para las clases de valores suavizados. | [`higgs_SmoothedValue.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_basics/utilities/higgs_SmoothedValue.h) |
| `Linear` | Se utiliza para indicar un suavizado lineal entre valores. | [`higgs_SmoothedValue.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_basics/utilities/higgs_SmoothedValue.h) |
| `Multiplicative` | Se usa para indicar un suavizado entre valores multiplicativos. | [`higgs_SmoothedValue.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_basics/utilities/higgs_SmoothedValue.h) |
| `SmoothedValue` | Una clase utilitaria para valores que necesitan suavizado para evitar glitches en el audio. | [`higgs_SmoothedValue.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_basics/utilities/higgs_SmoothedValue.h) |
| `Reverb` | Aplica un efecto de reverberación simple a un flujo de datos de audio. | [`higgs_Reverb.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_basics/utilities/higgs_Reverb.h) |
| `ADSR` | Una clase de envolvente ADSR muy sencilla. | [`higgs_ADSR.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_basics/utilities/higgs_ADSR.h) |
| `WorkgroupToken` | Creado por AudioWorkgroup para unir el hilo que realiza la llamada a un grupo de trabajo. | [`higgs_AudioWorkgroup.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_basics/utilities/higgs_AudioWorkgroup.h) |
| `AudioWorkgroup` | Un handle de un grupo de trabajo de audio, que es una colección de hilos de tiempo real que trabajan en conjunto para producir audio dentro de un plazo común. | [`higgs_AudioWorkgroup.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_basics/utilities/higgs_AudioWorkgroup.h) |

### `midi`

| Clase | Descripción | Cabecera |
|---|---|---|
| `MidiMessage` | Encapsula un mensaje MIDI. | [`higgs_MidiMessage.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_basics/midi/higgs_MidiMessage.h) |
| `MidiMessageMetadata` | Una vista de los datos de mensajes MIDI almacenados en un búfer contiguo. | [`higgs_MidiBuffer.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_basics/midi/higgs_MidiBuffer.h) |
| `MidiBufferIterator` | Un iterador para recorrer datos MIDI sin procesar contiguos, que permite iterar sobre un MidiBuffer con la sintaxis de for por rango de C++11. | [`higgs_MidiBuffer.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_basics/midi/higgs_MidiBuffer.h) |
| `MidiBuffer` | Contiene una secuencia de eventos MIDI con marca de tiempo. | [`higgs_MidiBuffer.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_basics/midi/higgs_MidiBuffer.h) |
| `MidiMessageSequence` | Secuencia de mensajes MIDI con marca de tiempo. | [`higgs_MidiMessageSequence.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_basics/midi/higgs_MidiMessageSequence.h) |
| `MidiFile` | Lee y escribe archivos en formato MIDI estándar. | [`higgs_MidiFile.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_basics/midi/higgs_MidiFile.h) |
| `MidiKeyboardState` | Representa un teclado de piano y lleva el registro de qué teclas están presionadas actualmente. | [`higgs_MidiKeyboardState.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_basics/midi/higgs_MidiKeyboardState.h) |
| `MidiRPNMessage` | Representa un mensaje MIDI RPN (número de parámetro registrado) o NRPN (número de parámetro no registrado). | [`higgs_MidiRPN.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_basics/midi/higgs_MidiRPN.h) |
| `MidiRPNDetector` | Analiza un flujo de datos MIDI para ensamblar mensajes RPN y NRPN a partir de los mensajes MIDI CC que los componen. | [`higgs_MidiRPN.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_basics/midi/higgs_MidiRPN.h) |
| `MidiRPNGenerator` | Genera una secuencia adecuada de mensajes MIDI CC para representar un mensaje RPN o NRPN. | [`higgs_MidiRPN.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_basics/midi/higgs_MidiRPN.h) |
| `MidiDataConcatenator` | Clase auxiliar que toma fragmentos de bytes MIDI entrantes, los empaqueta en mensajes y los despacha a un callback MIDI. | [`higgs_MidiDataConcatenator.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_basics/midi/higgs_MidiDataConcatenator.h) |

### `mpe`

| Clase | Descripción | Cabecera |
|---|---|---|
| `MPEValue` | Esta clase representa un único valor para cualquiera de las dimensiones de control de MPE. | [`higgs_MPEValue.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_basics/mpe/higgs_MPEValue.h) |
| `MPENote` | Esta estructura representa una nota MPE en reproducción. | [`higgs_MPENote.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_basics/mpe/higgs_MPENote.h) |
| `MPEZone` | Esta estructura representa una zona MPE. | [`higgs_MPEZoneLayout.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_basics/mpe/higgs_MPEZoneLayout.h) |
| `MPEZoneLayout` | Esta clase representa la distribución actual de zonas MPE de un dispositivo capaz de manejar MPE. | [`higgs_MPEZoneLayout.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_basics/mpe/higgs_MPEZoneLayout.h) |
| `MPEInstrument` | Esta clase representa un instrumento que gestiona MPE. | [`higgs_MPEInstrument.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_basics/mpe/higgs_MPEInstrument.h) |
| `MPEMessages` | Esta clase auxiliar contiene las funciones necesarias para generar mensajes MIDI exclusivos de MPE, como definir las zonas MPE superior e inferior y establecer los rangos de pitchbend por nota y maestro. | [`higgs_MPEMessages.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_basics/mpe/higgs_MPEMessages.h) |
| `MPESynthesiserBase` | Derive de esta clase para crear un generador de audio básico compatible con MPE. | [`higgs_MPESynthesiserBase.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_basics/mpe/higgs_MPESynthesiserBase.h) |
| `MPESynthesiserVoice` | Representa una voz MPE que un MPESynthesiser puede usar para reproducir un sonido. | [`higgs_MPESynthesiserVoice.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_basics/mpe/higgs_MPESynthesiserVoice.h) |
| `MPESynthesiser` | Clase base para un dispositivo musical compatible con MPE que puede reproducir sonidos. | [`higgs_MPESynthesiser.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_basics/mpe/higgs_MPESynthesiser.h) |
| `MPEChannelAssigner` | Esta clase gestiona la asignación de nuevas notas MIDI a los canales miembro de una zona MPE activa. | [`higgs_MPEUtils.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_basics/mpe/higgs_MPEUtils.h) |
| `MPEChannelRemapper` | Esta clase gestiona la lógica para reasignar mensajes de notas MIDI de múltiples fuentes MPE a una zona MPE especificada. | [`higgs_MPEUtils.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_basics/mpe/higgs_MPEUtils.h) |

### `sources`

| Clase | Descripción | Cabecera |
|---|---|---|
| `AudioSourceChannelInfo` | Usado por AudioSource::getNextAudioBlock(). | [`higgs_AudioSource.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_basics/sources/higgs_AudioSource.h) |
| `AudioSource` | Clase base para objetos que pueden producir un flujo continuo de audio. | [`higgs_AudioSource.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_basics/sources/higgs_AudioSource.h) |
| `PositionableAudioSource` | Tipo de AudioSource que se puede reposicionar. | [`higgs_PositionableAudioSource.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_basics/sources/higgs_PositionableAudioSource.h) |
| `BufferingAudioSource` | Un AudioSource que toma otra fuente como entrada y la almacena en un búfer usando un hilo. | [`higgs_BufferingAudioSource.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_basics/sources/higgs_BufferingAudioSource.h) |
| `ChannelRemappingAudioSource` | Un AudioSource que toma el audio de otra fuente y reasigna sus canales de entrada y salida a una disposición diferente. | [`higgs_ChannelRemappingAudioSource.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_basics/sources/higgs_ChannelRemappingAudioSource.h) |
| `IIRFilterAudioSource` | AudioSource que aplica un filtro IIR a otra fuente. | [`higgs_IIRFilterAudioSource.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_basics/sources/higgs_IIRFilterAudioSource.h) |
| `MemoryAudioSource` | Un AudioSource que recibe datos de audio en punto flotante como entrada. | [`higgs_MemoryAudioSource.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_basics/sources/higgs_MemoryAudioSource.h) |
| `MixerAudioSource` | Un AudioSource que mezcla la salida de un conjunto de otros AudioSources. | [`higgs_MixerAudioSource.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_basics/sources/higgs_MixerAudioSource.h) |
| `ResamplingAudioSource` | Tipo de AudioSource que toma una fuente de entrada y cambia su frecuencia de muestreo. | [`higgs_ResamplingAudioSource.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_basics/sources/higgs_ResamplingAudioSource.h) |
| `ReverbAudioSource` | Un AudioSource que usa la clase Reverb para aplicar reverberación a otro AudioSource. | [`higgs_ReverbAudioSource.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_basics/sources/higgs_ReverbAudioSource.h) |
| `ToneGeneratorAudioSource` | Un AudioSource sencillo que genera una onda sinusoidal. | [`higgs_ToneGeneratorAudioSource.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_basics/sources/higgs_ToneGeneratorAudioSource.h) |

### `synthesisers`

| Clase | Descripción | Cabecera |
|---|---|---|
| `SynthesiserSound` | Describe uno de los sonidos que puede reproducir un Synthesiser. | [`higgs_Synthesiser.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_basics/synthesisers/higgs_Synthesiser.h) |
| `SynthesiserVoice` | Representa una voz que un Synthesiser puede usar para reproducir un SynthesiserSound. | [`higgs_Synthesiser.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_basics/synthesisers/higgs_Synthesiser.h) |
| `Synthesiser` | Clase base para un dispositivo musical que puede reproducir sonidos. | [`higgs_Synthesiser.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_basics/synthesisers/higgs_Synthesiser.h) |

### `audio_play_head`

| Clase | Descripción | Cabecera |
|---|---|---|
| `AudioPlayHead` | Una subclase de AudioPlayHead puede proporcionar información sobre la posición y el estado de un cabezal de reproducción en movimiento durante la reproducción de audio. | [`higgs_AudioPlayHead.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_basics/audio_play_head/higgs_AudioPlayHead.h) |

### `midi/ump`

| Clase | Descripción | Cabecera |
|---|---|---|
| `BytesOnGroup` | Contiene un grupo UMP y un intervalo de bytes que se recibieron o que se van a enviar en ese grupo. | [`higgs_UMPBytesOnGroup.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_basics/midi/ump/higgs_UMPBytesOnGroup.h) |
| `DeviceInfo` | Contiene información del dispositivo MIDI que pueden requerir ciertos mensajes UMP y mensajes MIDI-CI. | [`higgs_UMPDeviceInfo.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_basics/midi/ump/higgs_UMPDeviceInfo.h) |
