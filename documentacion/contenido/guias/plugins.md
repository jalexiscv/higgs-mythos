# Plug-ins de audio

Con Higgs Mythos escribes un plug-in una sola vez —un `AudioProcessor` y, opcionalmente, un
`AudioProcessorEditor`— y lo compilas en todos los formatos soportados. Esta guía amplía el
[primer plug-in](../primeros-pasos/primer-plugin.md) con todo lo que necesitarás en un proyecto real.

## Módulos implicados

| Módulo | Contenido |
|---|---|
| `higgs_audio_processors_headless` | `AudioProcessor`, parámetros (`AudioParameterFloat`, `…Int`, `…Bool`, `…Choice`, grupos), `AudioProcessorGraph`, `AudioPluginInstance`, formatos de hosting sin interfaz, extensiones específicas de VST2/VST3/AAX |
| `higgs_audio_processors` | Todo lo anterior más interfaz: `AudioProcessorEditor`, `GenericAudioProcessorEditor`, `AudioProcessorValueTreeState` y sus *attachments*, escaneo de plug-ins |
| `higgs_audio_plugin_client` | Los *wrappers* de VST2, VST3, AU, AUv3, AAX, LV2, Unity y Standalone |
| `higgs_audio_utils` | Componentes de audio para editores y la aplicación Standalone |

## Formatos

| Formato | Valor en `FORMATS` | Plataformas | Notas |
|---|---|---|---|
| VST3 | `VST3` | macOS, Windows, Linux | SDK de VST3 3.8.0 incluido |
| Audio Unit v2 | `AU` | macOS | Solo se activa en macOS |
| Audio Unit v3 | `AUv3` | macOS, iOS | Requiere el generador de Xcode |
| AAX | `AAX` | macOS, Windows | Para Pro Tools; requiere firma (véase abajo) |
| LV2 | `LV2` | Linux, macOS, Windows | Identificado por `LV2URI` |
| VST2 (heredado) | `VST` | macOS, Windows, Linux | Necesita `higgs_set_vst2_sdk_path` antes de añadir el target |
| Unity | `Unity` | macOS, Windows, Linux | Plug-in nativo de audio para Unity; requiere `UNITY_COPY_DIR` si se copia tras compilar |
| Standalone | `Standalone` | Todas | Aplicación independiente que aloja tu procesador |

`higgs_add_plugin` crea un target de código compartido con el nombre que indiques y un target por
formato con sufijo: `MiPlugin_VST3`, `MiPlugin_AU`, `MiPlugin_AUv3`, `MiPlugin_AAX`,
`MiPlugin_LV2`, `MiPlugin_VST`, `MiPlugin_Unity` y `MiPlugin_Standalone`.

## Macros generadas para tu código

`higgs_add_plugin` traduce sus argumentos en definiciones de preprocesador que puedes usar en el
código del procesador:

| Macro | Origen |
|---|---|
| `HiggsPlugin_Name` | `PLUGIN_NAME` |
| `HiggsPlugin_IsSynth` | `IS_SYNTH` |
| `HiggsPlugin_WantsMidiInput` | `NEEDS_MIDI_INPUT` |
| `HiggsPlugin_ProducesMidiOutput` | `NEEDS_MIDI_OUTPUT` |
| `HiggsPlugin_IsMidiEffect` | `IS_MIDI_EFFECT` |
| `HiggsPlugin_EditorRequiresKeyboardFocus` | `EDITOR_WANTS_KEYBOARD_FOCUS` |
| `HiggsPlugin_ManufacturerCode`, `HiggsPlugin_PluginCode` | `PLUGIN_MANUFACTURER_CODE`, `PLUGIN_CODE` |
| `HiggsPlugin_Enable_ARA` | `IS_ARA_EFFECT` |

`AudioProcessor::wrapperType` indica en tiempo de ejecución en qué formato se ha cargado el
plug-in, y `PluginHostType` permite detectar el host (útil para sortear peculiaridades de algunos
DAW).

## Buses y canales

Los buses se declaran en el constructor con `BusesProperties`. Cada bus tiene un nombre, un
`AudioChannelSet` por defecto y si está activado:

```cpp
MiProcesador()
    : AudioProcessor (BusesProperties()
                          .withInput  ("Entrada",    higgs::AudioChannelSet::stereo(), true)
                          .withOutput ("Salida",     higgs::AudioChannelSet::stereo(), true)
                          .withInput  ("Sidechain",  higgs::AudioChannelSet::stereo(), false))
{}

bool isBusesLayoutSupported (const BusesLayout& layouts) const override
{
    // Principal: mono o estéreo, y entrada igual a salida
    const auto principal = layouts.getMainOutputChannelSet();
    if (principal != higgs::AudioChannelSet::mono() && principal != higgs::AudioChannelSet::stereo())
        return false;

    return layouts.getMainInputChannelSet() == principal;
}
```

En `processBlock()` puedes obtener la porción del búfer correspondiente a cada bus con
`getBusBuffer (buffer, true, 1)` (entrada del bus 1, el *sidechain* en este caso).
`AudioChannelSet` define disposiciones desde mono hasta formatos envolventes e inmersivos
(5.1, 7.1.4, Ambisonics…); véase `SurroundPluginDemo`.

!!! note "Evita `HiggsPlugin_PreferredChannelConfigurations`"
    Esta definición antigua usa llaves y puede malinterpretarse en compilaciones con Ninja o
    Makefiles. Usa siempre el API de buses.

## Parámetros

Tipos de parámetro disponibles:

| Clase | Valor | Ejemplo |
|---|---|---|
| `AudioParameterFloat` | `float` en un `NormalisableRange` | Frecuencia, ganancia, mezcla |
| `AudioParameterInt` | entero en un rango | Número de voces |
| `AudioParameterBool` | `bool` | Bypass, activación |
| `AudioParameterChoice` | índice en una lista de opciones | Forma de onda, modo |

```cpp
static higgs::AudioProcessorValueTreeState::ParameterLayout crearParametros()
{
    using namespace higgs;

    auto filtro = std::make_unique<AudioProcessorParameterGroup> ("filtro", "Filtro", "|");

    filtro->addChild (std::make_unique<AudioParameterFloat> (
        ParameterID { "corte", 1 }, "Corte",
        NormalisableRange<float> (20.0f, 20000.0f, 0.0f, 0.25f),   // asimétrico (skew)
        1000.0f,
        AudioParameterFloatAttributes().withLabel ("Hz")));

    filtro->addChild (std::make_unique<AudioParameterChoice> (
        ParameterID { "tipo", 1 }, "Tipo", StringArray { "Paso bajo", "Paso banda", "Paso alto" }, 0));

    AudioProcessorValueTreeState::ParameterLayout layout;
    layout.add (std::move (filtro));
    layout.add (std::make_unique<AudioParameterBool> (ParameterID { "bypass", 1 }, "Bypass", false));
    return layout;
}
```

Reglas importantes:

- El **identificador** (`ParameterID`) es lo que el host usa para guardar la automatización. No
  lo cambies una vez publicado el plug-in.
- El segundo campo de `ParameterID` es una *pista de versión* (necesaria para AU y VST3): súbela
  cuando añadas parámetros en una versión nueva, y deja la de los existentes igual.
- Lee los valores en el hilo de audio mediante `getRawParameterValue()` (un `std::atomic<float>*`)
  o el operador de conversión del parámetro (`*parametroFloat`).
- Si cambias un parámetro desde la interfaz sin un *attachment*, rodea la operación con
  `beginChangeGesture()`, `setValueNotifyingHost()` y `endChangeGesture()`.
- Para recibir cambios usa `AudioProcessorValueTreeState::addParameterListener()` o un
  `ParameterAttachment`.
- Si tu plug-in tiene un parámetro de bypass, devuélvelo en `getBypassParameter()` para que el
  host lo use como bypass nativo; si no, implementa `processBlockBypassed()`.

Opciones de compatibilidad de parámetros del módulo `higgs_audio_plugin_client`:
`HIGGS_VST3_CAN_REPLACE_VST2`, `HIGGS_FORCE_USE_LEGACY_PARAM_IDS`,
`HIGGS_FORCE_LEGACY_PARAMETER_AUTOMATION_TYPE`, `HIGGS_USE_STUDIO_ONE_COMPATIBLE_PARAMETERS` y
`HIGGS_AU_WRAPPERS_SAVE_PROGRAM_STATES`. Consulta su descripción completa en
[Opciones de configuración](../compilacion/opciones.md) antes de cambiarlas en un plug-in ya
publicado: pueden romper la compatibilidad con proyectos guardados.

## Información del host: tempo y transporte

```cpp
void processBlock (higgs::AudioBuffer<float>& buffer, higgs::MidiBuffer& midi) override
{
    if (auto* cabezal = getPlayHead())
    {
        if (auto pos = cabezal->getPosition())
        {
            if (auto bpm = pos->getBpm())
                tempo = *bpm;

            reproduciendo = pos->getIsPlaying();

            if (auto ppq = pos->getPpqPosition())
                posicionEnNegras = *ppq;
        }
    }
    // ...
}
```

`PositionInfo` usa `Optional` en los campos que el host puede no proporcionar. Otros datos
útiles: `getSampleRate()`, `getBlockSize()`, `isNonRealtime()` (renderizado sin conexión) y
`setLatencySamples()` para notificar la latencia que introduces.

## Estado y presets

El host llama a `getStateInformation()` al guardar el proyecto y a `setStateInformation()` al
cargarlo. Con `AudioProcessorValueTreeState` basta con serializar su `ValueTree` (véase el
[primer plug-in](../primeros-pasos/primer-plugin.md#el-procesador)). Puedes añadir al mismo árbol
cualquier dato no automatizable (rutas de muestras, tamaño del editor, etc.).

Los «programas» (`getNumPrograms()`, `setCurrentProgram()`…) son el mecanismo de presets de
fábrica que muestran algunos hosts. Devuelve al menos 1 programa aunque no los uses: algunos hosts
no se comportan bien con 0.

## El editor

- Hereda de `AudioProcessorEditor` y guarda una referencia a tu procesador.
- Conecta controles y parámetros con `SliderAttachment`, `ButtonAttachment` y
  `ComboBoxAttachment` (de `AudioProcessorValueTreeState`) o con `SliderParameterAttachment` y
  similares sobre un `RangedAudioParameter`.
- Usa `setResizable()`, `setResizeLimits()` y `getConstrainer()` para que el host permita
  redimensionarlo; la escala HiDPI se aplica automáticamente.
- **Nunca accedas al editor desde el hilo de audio**, y recuerda que el host puede destruirlo y
  recrearlo en cualquier momento mientras el procesador sigue vivo.
- Para mostrar niveles o espectros, el procesador publica datos (atómicos o FIFO) y el editor los
  lee con un `Timer`.
- Para interfaces HTML/JavaScript usa `WebBrowserComponent` con los *relays* de parámetros
  (véase [Interfaces web](web.md)).

## Formato AAX y firma digital

Los plug-ins AAX deben firmarse digitalmente con las herramientas de PACE Anti-Piracy para
ejecutarse en las versiones comerciales de Pro Tools. Las herramientas son gratuitas y las
proporciona Avid. El proceso es:

1. Regístrate como desarrollador AAX en <https://developer.avid.com/aax/>.
2. Solicita un código de activación del *Pro Tools Developer Bundle* escribiendo a
   <devauth@avid.com>.
3. Descarga la última versión de Pro Tools Developer desde tu cuenta de desarrollador de Avid y
   prueba allí tus plug-ins sin firmar.
4. Cuando estén probados, escribe a <audiosdk@avid.com> con el asunto
   *"PACE Eden Signing Tools Request"*, incluyendo una descripción de cada plug-in, una grabación
   de pantalla del plug-in funcionando en Pro Tools Developer (con audio si es posible), el nombre
   de la empresa, el nombre completo del administrador y un teléfono.
5. PACE Anti-Piracy se pondrá en contacto contigo para la firma.

Argumentos de CMake relacionados: `AAX_IDENTIFIER`, `AAX_CATEGORY`, `DISABLE_AAX_BYPASS`,
`DISABLE_AAX_MULTI_MONO` y `AAX_COPY_DIR`.

## Particularidades de otros formatos

- **VST3**: `VST3_CATEGORIES`, `VST_NUM_MIDI_INS`/`OUTS`, `VST3_AUTO_MANIFEST` y
  `higgs_enable_vst3_manifest_step`. Las extensiones específicas se exponen con
  `VST3ClientExtensions`.
- **AU / AUv3**: `AU_MAIN_TYPE`, `AU_EXPORT_PREFIX`, `AU_SANDBOX_SAFE`,
  `SUPPRESS_AU_PLIST_RESOURCE_USAGE`. AUv3 se empaqueta como extensión dentro de una aplicación
  contenedora (véase `AUv3SynthPluginDemo`).
- **LV2**: `LV2URI` debe cambiar si haces cambios incompatibles (quitar o reordenar parámetros,
  cambiar el formato de presets…); `LV2_PLUGIN_CLASS` define la categoría.
- **Standalone**: la ventana (`StandaloneFilterWindow`) incluye ajustes de audio/MIDI;
  `HIGGS_STANDALONE_FILTER_WINDOW_USE_KIOSK_MODE` activa el modo quiosco (por defecto en iOS y
  Android).
- **VST2**: formato heredado; necesitas tu propia copia del SDK.

Todos estos argumentos se describen en el [API de CMake](../compilacion/cmake-api.md#higgs_add_target).

## Instalación y prueba

- `COPY_PLUGIN_AFTER_BUILD TRUE` (o la propiedad de directorio `HIGGS_COPY_PLUGIN_AFTER_BUILD`)
  copia los plug-ins a las carpetas estándar tras compilar; `VST3_COPY_DIR`, `AU_COPY_DIR`,
  `AAX_COPY_DIR`, etc., cambian el destino.
- Si necesitas firmar o modificar el bundle antes de copiarlo, desactiva la copia automática,
  añade tus pasos con `add_custom_command(TARGET … POST_BUILD)` y llama después a
  `higgs_enable_copy_plugin_step`. La ruta del artefacto está en la propiedad
  `HIGGS_PLUGIN_ARTEFACT_FILE` del target de formato.
- Prueba en el [AudioPluginHost](../herramientas/audio-plugin-host.md) y en el formato Standalone
  antes de pasar a un DAW.

Carpetas de destino por defecto (definidas en `extras/Build/CMake/HIGGSUtils.cmake`; se pueden
cambiar con las propiedades `HIGGS_<FORMATO>_COPY_DIR` del directorio o del target):

| Formato | macOS | Windows (64 bits) | Linux / BSD |
|---|---|---|---|
| VST3 | `~/Library/Audio/Plug-Ins/VST3` | `%CommonProgramW6432%/VST3` | `~/.vst3` |
| AU | `~/Library/Audio/Plug-Ins/Components` | — | — |
| AAX | `/Library/Application Support/Avid/Audio/Plug-Ins` | `%CommonProgramW6432%/Avid/Audio/Plug-Ins` | — |
| LV2 | `~/Library/Audio/Plug-Ins/LV2` | `%APPDATA%/LV2` | `~/.lv2` |
| VST2 | `~/Library/Audio/Plug-Ins/VST` | `%ProgramW6432%/Steinberg/Vstplugins` | `~/.vst` |
| Unity | `~/Library/Audio/Plug-Ins/Unity` | `%APPDATA%/Unity` | `~/.unity` |

## Demos relacionadas

`GainPluginDemo`, `AudioPluginDemo`, `NoiseGatePluginDemo`, `MultiOutSynthPluginDemo`,
`SurroundPluginDemo`, `ArpeggiatorPluginDemo`, `MidiLoggerPluginDemo`, `DSPModulePluginDemo`,
`SamplerPluginDemo`, `AUv3SynthPluginDemo`, `WebViewPluginDemo`, `HostPluginDemo`,
`ARAPluginDemo`, `ReaperEmbeddedViewPluginDemo`.
