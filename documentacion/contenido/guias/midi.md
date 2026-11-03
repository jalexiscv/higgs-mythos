# MIDI, MPE y MIDI 2.0

Higgs Mythos admite MIDI 1.0 clásico (flujo de bytes), MPE (*MIDI Polyphonic Expression*), los
paquetes universales de MIDI 2.0 (UMP) y MIDI-CI (*Capability Inquiry*).

| Área | Módulo | Clases principales |
|---|---|---|
| Mensajes y secuencias | `higgs_audio_basics` | `MidiMessage`, `MidiBuffer`, `MidiMessageSequence`, `MidiFile`, `MidiKeyboardState`, `MidiMessageCollector`, `MidiRPNDetector` |
| MPE | `higgs_audio_basics` | `MPEInstrument`, `MPEZoneLayout`, `MPESynthesiser`, `MPEValue`, `MPENote` |
| UMP (MIDI 2.0) | `higgs_audio_basics`, `higgs_audio_devices` | espacio de nombres `higgs::ump` (alias de `higgs::universal_midi_packets`): `Session`, `Input`, `Output`, `Endpoints`, conversores y vistas de paquetes |
| Dispositivos MIDI 1.0 | `higgs_audio_devices` | `MidiInput`, `MidiOutput`, `MidiDeviceInfo`, `MidiDeviceListConnection` |
| MIDI-CI | `higgs_midi_ci` | espacio de nombres `higgs::midi_ci`: `Device`, `DeviceOptions`, mensajes, perfiles y propiedades |
| Interfaz | `higgs_audio_utils` | `MidiKeyboardComponent`, `MPEKeyboardComponent`, `BluetoothMidiDevicePairingDialogue` |

## Mensajes

```cpp
auto on  = higgs::MidiMessage::noteOn (1, 60, (higgs::uint8) 100);   // canal 1, Do central
auto off = higgs::MidiMessage::noteOff (1, 60);
auto cc  = higgs::MidiMessage::controllerEvent (1, 74, 64);

if (on.isNoteOn())
    DBG ("Nota " << on.getNoteNumber() << " velocidad " << on.getVelocity());
```

## `MidiBuffer` en el procesamiento de audio

En `AudioProcessor::processBlock()` y en `Synthesiser::renderNextBlock()` los eventos MIDI de cada
bloque llegan en un `MidiBuffer`, cada uno con su posición en muestras dentro del bloque:

```cpp
void processBlock (higgs::AudioBuffer<float>& audio, higgs::MidiBuffer& midi) override
{
    higgs::MidiBuffer salida;

    for (const auto metadatos : midi)
    {
        auto mensaje = metadatos.getMessage();

        if (mensaje.isNoteOnOrOff())
            mensaje.setNoteNumber (higgs::jlimit (0, 127, mensaje.getNoteNumber() + 12));  // transporta una octava

        salida.addEvent (mensaje, metadatos.samplePosition);
    }

    midi.swapWith (salida);
}
```

Para que un plug-in reciba o envíe MIDI, declara `NEEDS_MIDI_INPUT TRUE` y/o
`NEEDS_MIDI_OUTPUT TRUE` en `higgs_add_plugin`, y devuelve los valores coherentes en
`acceptsMidi()` y `producesMidi()`.

## Dispositivos MIDI

```cpp
class Monitor final : private higgs::MidiInputCallback
{
public:
    Monitor()
    {
        for (const auto& info : higgs::MidiInput::getAvailableDevices())
        {
            if (auto entrada = higgs::MidiInput::openDevice (info.identifier, this))
            {
                entrada->start();
                entradas.push_back (std::move (entrada));
            }
        }
    }

private:
    void handleIncomingMidiMessage (higgs::MidiInput*, const higgs::MidiMessage& m) override
    {
        // ¡Atención! Se llama en un hilo de MIDI, no en el de mensajes.
        coleccion.addMessageToQueue (m);
    }

    std::vector<std::unique_ptr<higgs::MidiInput>> entradas;
    higgs::MidiMessageCollector coleccion;
};
```

- `MidiOutput::openDevice()` y `sendMessageNow()` / `sendBlockOfMessages()` envían mensajes.
- `MidiInput::createNewDevice()` y `MidiOutput::createNewDevice()` crean puertos virtuales
  (macOS, iOS, Linux).
- `AudioDeviceManager::setMidiInputDeviceEnabled()` y `addMidiInputDeviceCallback()` integran la
  entrada MIDI con la gestión de dispositivos de audio.
- `MidiDeviceListConnection` notifica cuando se conectan o desconectan dispositivos; desde 9.0.4
  existe además un API de escucha de desconexiones de dispositivos MIDI.

En Windows puedes elegir entre el API Win32 clásico, el API de Windows Runtime
(`HIGGS_USE_WINRT_MIDI`, con soporte de dispositivos Bluetooth LE) y **Windows MIDI Services**
(`HIGGS_USE_WINDOWS_MIDI_SERVICES` o la opción `NEEDS_WINDOWS_MIDI_SERVICES` de CMake), que habilita
MIDI 2.0 y exige C++20.

## Archivos MIDI

```cpp
higgs::MidiFile archivoMidi;
higgs::FileInputStream entrada (higgs::File ("/ruta/cancion.mid"));

if (entrada.openedOk() && archivoMidi.readFrom (entrada))
{
    archivoMidi.convertTimestampTicksToSeconds();

    for (int pista = 0; pista < archivoMidi.getNumTracks(); ++pista)
        for (auto* evento : *archivoMidi.getTrack (pista))
            DBG (evento->message.getDescription() << " @ " << evento->message.getTimeStamp());
}
```

## MPE

MPE asigna cada nota a su propio canal MIDI para que la inflexión de tono, la presión y el
«timbre» (CC74) puedan variar por nota. `MPEInstrument` interpreta el flujo MIDI según la
configuración de zonas (`MPEZoneLayout`) y mantiene el estado de cada `MPENote`.
`MPESynthesiser` y `MPESynthesiserVoice` construyen sintetizadores expresivos sobre esa base.
Véase el ejemplo `MPEDemo`.

## MIDI 2.0 y UMP

Los *Universal MIDI Packets* son el formato de transporte de MIDI 2.0. En
`higgs_audio_basics` encontrarás utilidades para construir, recorrer y convertir paquetes
(`ump::Factory`, `ump::Packets`, `ump::Iterator`, `ump::View`, conversores MIDI 1.0 ↔ 2.0 y
traductores a flujo de bytes). En `higgs_audio_devices`, `ump::Endpoints` enumera los extremos
disponibles y `ump::Session` abre conexiones `ump::Input`/`ump::Output` a ellos, convirtiendo los
mensajes entrantes al protocolo que pidas. Véase el ejemplo `UmpDemo`.

## MIDI-CI

El módulo `higgs_midi_ci` implementa *MIDI Capability Inquiry*: descubrimiento de dispositivos,
negociación de perfiles (*Profile Configuration*), intercambio de propiedades
(*Property Exchange*) y consultas de proceso (*Process Inquiry*). La clase central es `midi_ci::Device`, configurada con `midi_ci::DeviceOptions`. El
ejemplo `CapabilityInquiryDemo` es una aplicación completa para explorar y probar dispositivos
MIDI-CI.

## Demos relacionadas

`MidiDemo`, `UmpDemo`, `MPEDemo`, `CapabilityInquiryDemo`, `ArpeggiatorPluginDemo`,
`MidiLoggerPluginDemo`.
