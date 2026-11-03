# Audio y dispositivos

El audio en Higgs Mythos se reparte entre varios módulos:

| Módulo | Contenido |
|---|---|
| `higgs_audio_basics` | `AudioBuffer`, conversiones de formato de muestra, fuentes de audio (`AudioSource`), sintetizador, MIDI, MPE, utilidades (`Decibels`, `SmoothedValue`, `ADSR`, `Reverb`, interpoladores) |
| `higgs_audio_devices` | Dispositivos de audio y MIDI: `AudioDeviceManager`, `AudioIODevice`, `MidiInput`/`MidiOutput`, UMP |
| `higgs_audio_formats` | Lectura y escritura de archivos: WAV, AIFF, FLAC, Ogg Vorbis, Opus, MP3, CoreAudio, Windows Media; `Sampler` |
| `higgs_audio_utils` | Componentes de interfaz de audio (`AudioDeviceSelectorComponent`, `AudioThumbnail`, `MidiKeyboardComponent`, `AudioAppComponent`), reproductores, CD |

## El búfer de audio

`AudioBuffer<float>` (alias `AudioSampleBuffer`) almacena varios canales de muestras no
entrelazadas:

```cpp
higgs::AudioBuffer<float> buffer (2, 512);   // 2 canales, 512 muestras
buffer.clear();

auto* izquierdo = buffer.getWritePointer (0);
for (int i = 0; i < buffer.getNumSamples(); ++i)
    izquierdo[i] = std::sin (2.0f * higgs::MathConstants<float>::pi * 440.0f * (float) i / 48000.0f);

buffer.copyFrom (1, 0, buffer, 0, 0, buffer.getNumSamples());   // copia al canal derecho
buffer.applyGain (higgs::Decibels::decibelsToGain (-6.0f));
auto rms = buffer.getRMSLevel (0, 0, buffer.getNumSamples());
```

`FloatVectorOperations` ofrece operaciones vectorizadas (SIMD) sobre arrays de muestras, y
`AudioData` convierte entre formatos (entero de 16/24/32 bits, coma flotante, entrelazado…).

## Una aplicación de audio mínima

`AudioAppComponent` combina un componente con un `AudioDeviceManager` y una `AudioSource`:

```cpp
class Sintetizador final : public higgs::AudioAppComponent
{
public:
    Sintetizador()
    {
        setSize (400, 200);
        setAudioChannels (0, 2);    // sin entradas, dos salidas
    }

    ~Sintetizador() override { shutdownAudio(); }

    void prepareToPlay (int /*muestrasPorBloque*/, double frecuenciaMuestreo) override
    {
        incrementoFase = 2.0 * higgs::MathConstants<double>::pi * 440.0 / frecuenciaMuestreo;
    }

    void getNextAudioBlock (const higgs::AudioSourceChannelInfo& info) override
    {
        for (int i = 0; i < info.numSamples; ++i)
        {
            const auto muestra = (float) (0.2 * std::sin (fase));
            fase += incrementoFase;

            for (int canal = 0; canal < info.buffer->getNumChannels(); ++canal)
                info.buffer->setSample (canal, info.startSample + i, muestra);
        }
    }

    void releaseResources() override {}

private:
    double fase = 0.0, incrementoFase = 0.0;
};
```

## Gestionar dispositivos

`AudioDeviceManager` abre y configura el dispositivo de audio (tipo de driver, frecuencia de
muestreo, tamaño de búfer, canales) y también gestiona las entradas MIDI:

```cpp
higgs::AudioDeviceManager gestor;
gestor.initialiseWithDefaultDevices (2, 2);   // 2 entradas, 2 salidas

auto config = gestor.getAudioDeviceSetup();
config.bufferSize = 256;
gestor.setAudioDeviceSetup (config, true);

gestor.addAudioCallback (&miCallback);        // un AudioIODeviceCallback
```

Para guardar la configuración elegida por el usuario usa `createStateXml()` y pásala de nuevo en
`initialise()` la próxima vez. `AudioDeviceSelectorComponent` es un panel listo para que el usuario
elija dispositivo, frecuencia y búfer.

Drivers disponibles por plataforma y las opciones que los controlan:

| Plataforma | Drivers | Opciones |
|---|---|---|
| macOS / iOS | CoreAudio (nueva implementación desde 9.0.0) | `HIGGS_DISABLE_AUDIO_MIXING_WITH_OTHER_APPS` (iOS) |
| Windows | WASAPI, DirectSound, ASIO | `HIGGS_WASAPI`, `HIGGS_DIRECTSOUND`, `HIGGS_ASIO`, `HIGGS_ASIO_USE_EXTERNAL_SDK` |
| Linux | ALSA, JACK | `HIGGS_ALSA`, `HIGGS_JACK` |
| Android | Oboe, OpenSL ES | `HIGGS_USE_ANDROID_OBOE`, `HIGGS_USE_OBOE_STABILIZED_CALLBACK`, `HIGGS_USE_ANDROID_OPENSLES` |

!!! warning "ASIO"
    Al activar `HIGGS_ASIO` tu programa depende de archivos del SDK de ASIO de Steinberg, y debe
    cumplir los términos de su licencia. Por defecto se usan las cabeceras de ASIO incluidas en el
    repositorio.

## Fuentes de audio encadenables

Las `AudioSource` se componen como bloques:

| Clase | Función |
|---|---|
| `AudioFormatReaderSource` | Reproduce un `AudioFormatReader` (un archivo) |
| `AudioTransportSource` | Añade reproducción/pausa/posición, re-muestreo y lectura en segundo plano |
| `ResamplingAudioSource` | Cambia la frecuencia de muestreo |
| `MixerAudioSource` | Mezcla varias fuentes |
| `ChannelRemappingAudioSource` | Reasigna canales |
| `BufferingAudioSource` | Lee por adelantado en otro hilo |
| `ToneGeneratorAudioSource` | Genera un tono senoidal |
| `IIRFilterAudioSource`, `ReverbAudioSource` | Filtrado y reverberación |
| `AudioSourcePlayer` | Conecta una fuente a un dispositivo (es un `AudioIODeviceCallback`) |

## Leer y reproducir archivos

```cpp
higgs::AudioFormatManager formatos;
formatos.registerBasicFormats();     // WAV, AIFF, FLAC, Ogg, Opus, MP3… según la configuración

if (auto* lector = formatos.createReaderFor (archivo))
{
    fuenteLector = std::make_unique<higgs::AudioFormatReaderSource> (lector, true);
    transporte.setSource (fuenteLector.get(), 32768, &hiloLectura, lector->sampleRate);
    transporte.start();
}
```

Donde `transporte` es un `AudioTransportSource` conectado al dispositivo mediante un
`AudioSourcePlayer` y `hiloLectura` un `TimeSliceThread` en marcha.

## Escribir archivos

```cpp
std::unique_ptr<higgs::OutputStream> flujo = archivo.createOutputStream();

higgs::WavAudioFormat wav;
auto escritor = wav.createWriterFor (flujo,
                                     higgs::AudioFormatWriterOptions{}.withSampleRate (48000.0)
                                                                      .withNumChannels (2)
                                                                      .withBitsPerSample (24));
if (escritor != nullptr)
    escritor->writeFromAudioSampleBuffer (buffer, 0, buffer.getNumSamples());
```

Para grabar desde el hilo de audio sin bloquearlo, usa `AudioFormatWriter::ThreadedWriter` (véase
`AudioRecordingDemo`).

### Formatos y opciones

| Formato | Clase | Lectura | Escritura | Opción |
|---|---|---|---|---|
| WAV | `WavAudioFormat` | ✓ | ✓ | — |
| AIFF | `AiffAudioFormat` | ✓ | ✓ | — |
| FLAC | `FlacAudioFormat` | ✓ | ✓ | `HIGGS_USE_FLAC` |
| Ogg Vorbis | `OggVorbisAudioFormat` | ✓ | ✓ | `HIGGS_USE_OGGVORBIS` |
| Opus | `OpusAudioFormat` | ✓ | ✓ | `HIGGS_USE_OPUS` (desde 9.0.3) |
| MP3 | `MP3AudioFormat` | ✓ | — | `HIGGS_USE_MP3AUDIOFORMAT` (activado por defecto desde 9.0.2) |
| MP3 vía LAME | `LameEncoderAudioFormat` | — | ✓ | `HIGGS_USE_LAME_AUDIO_FORMAT` |
| CoreAudio | `CoreAudioFormat` | ✓ | ✓ | macOS/iOS |
| Windows Media | `WindowsMediaAudioFormat` | ✓ | — | `HIGGS_USE_WINDOWS_MEDIA_FORMAT` |

## Visualizar audio

`AudioThumbnail` genera y dibuja una forma de onda reducida de un archivo; `AudioThumbnailCache`
guarda miniaturas ya calculadas. `AudioVisualiserComponent` muestra una señal en tiempo real.

```cpp
higgs::AudioThumbnailCache cache { 5 };
higgs::AudioThumbnail miniatura { 512, formatos, cache };

miniatura.setSource (new higgs::FileInputSource (archivo));
// en paint():
miniatura.drawChannels (g, getLocalBounds(), 0.0, miniatura.getTotalLength(), 1.0f);
```

## Síntesis

`Synthesiser` gestiona un conjunto de voces (`SynthesiserVoice`) y sonidos (`SynthesiserSound`),
reparte las notas MIDI entre las voces y mezcla su salida:

```cpp
higgs::Synthesiser synth;
for (int i = 0; i < 8; ++i)
    synth.addVoice (new MiVoz());
synth.addSound (new MiSonido());

synth.setCurrentPlaybackSampleRate (sampleRate);
synth.renderNextBlock (buffer, midiEntrante, 0, buffer.getNumSamples());
```

Para instrumentos expresivos con MPE usa `MPESynthesiser` y `MPESynthesiserVoice` (véase
[MIDI](midi.md)). Para muestras, `SamplerSound` y `SamplerVoice` de `higgs_audio_formats`.

Utilidades de síntesis y procesamiento en `higgs_audio_basics`: `ADSR`, `SmoothedValue`,
`Reverb`, `IIRFilter`, `Interpolators` (Lagrange, Catmull-Rom, Windowed Sinc…), `Decibels`.

## Demos relacionadas

`AudioAppDemo`, `AudioSettingsDemo`, `AudioPlaybackDemo`, `AudioRecordingDemo`,
`AudioSynthesiserDemo`, `AudioLatencyDemo`, `AudioWorkgroupDemo`, `PluckedStringsDemo`,
`SimpleFFTDemo`.
