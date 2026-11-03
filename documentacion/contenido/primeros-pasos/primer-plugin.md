# Tu primer plug-in de audio

En esta página construimos un plug-in de **ganancia** con un parámetro automatizable, un editor con
un deslizador y guardado/restauración de estado. Partimos de la plantilla
`examples/CMake/AudioPlugin` y la ampliamos con `AudioProcessorValueTreeState`, la forma
recomendada de gestionar parámetros.

## Cómo funciona un plug-in en Higgs Mythos

Un plug-in se compone de dos clases:

- Un **procesador**, derivado de `higgs::AudioProcessor`, que procesa el audio en
  `processBlock()`, declara los buses de entrada/salida y los parámetros, y guarda su estado.
- Un **editor**, derivado de `higgs::AudioProcessorEditor`, que es la interfaz gráfica que muestra
  el host. Es opcional.

El módulo `higgs_audio_plugin_client` contiene los *wrappers* que adaptan ese procesador a cada
formato (VST3, AU, AUv3, AAX, LV2, Unity, Standalone). Tu código es el mismo para todos: los
wrappers buscan la función global `createPluginFilter()` para crear tu procesador.

```mermaid
flowchart LR
    Host[Host / DAW] -->|VST3, AU, AAX, LV2…| W[Wrapper del formato<br/>higgs_audio_plugin_client]
    W -->|createPluginFilter()| P[Tu AudioProcessor]
    P -->|createEditor()| E[Tu AudioProcessorEditor]
```

## `CMakeLists.txt`

```cmake title="CMakeLists.txt"
cmake_minimum_required(VERSION 3.22)

project(AUDIO_PLUGIN_EXAMPLE VERSION 0.0.1)

add_subdirectory(HIGGS)

higgs_add_plugin(AudioPluginExample
    COMPANY_NAME "Mi Empresa"
    IS_SYNTH FALSE                       # ¿sintetizador o efecto?
    NEEDS_MIDI_INPUT FALSE
    NEEDS_MIDI_OUTPUT FALSE
    COPY_PLUGIN_AFTER_BUILD FALSE        # TRUE para instalarlo tras compilar
    PLUGIN_MANUFACTURER_CODE Higgs       # 4 caracteres, al menos una mayúscula
    PLUGIN_CODE Dem0                     # 4 caracteres, exactamente una mayúscula
    FORMATS AU VST3 Standalone           # también: AAX Unity VST AUv3 LV2
    PRODUCT_NAME "Audio Plugin Example")

target_sources(AudioPluginExample
    PRIVATE
        PluginEditor.cpp
        PluginProcessor.cpp)

target_compile_definitions(AudioPluginExample
    PUBLIC
        HIGGS_WEB_BROWSER=0
        HIGGS_USE_CURL=0
        HIGGS_VST3_CAN_REPLACE_VST2=0)

target_link_libraries(AudioPluginExample
    PRIVATE
        higgs::higgs_audio_utils
    PUBLIC
        higgs::higgs_recommended_config_flags
        higgs::higgs_recommended_lto_flags
        higgs::higgs_recommended_warning_flags)
```

`higgs_add_plugin` crea un target de **código compartido** (`AudioPluginExample`, una biblioteca
estática) y un target adicional por formato, por ejemplo `AudioPluginExample_VST3`,
`AudioPluginExample_AU` y `AudioPluginExample_Standalone`. Las definiciones `PUBLIC` se propagan a
todos ellos.

!!! note "Códigos del plug-in"
    `PLUGIN_MANUFACTURER_CODE` identifica a tu empresa y `PLUGIN_CODE` al plug-in. Para ser
    compatibles con AU, el código de fabricante debe contener al menos una mayúscula y el del
    plug-in exactamente una. GarageBand 10.3 exige además que la primera letra sea mayúscula y el
    resto minúsculas. Si no indicas `PLUGIN_CODE`, se genera uno aleatorio en cada configuración,
    así que **fíjalo siempre** antes de publicar.

## El procesador

```cpp title="PluginProcessor.h"
#pragma once

#include <higgs_audio_processors/higgs_audio_processors.h>

class GananciaProcessor final : public higgs::AudioProcessor
{
public:
    GananciaProcessor();

    void prepareToPlay (double sampleRate, int samplesPerBlock) override;
    void releaseResources() override {}
    bool isBusesLayoutSupported (const BusesLayout& layouts) const override;

    void processBlock (higgs::AudioBuffer<float>&, higgs::MidiBuffer&) override;
    using AudioProcessor::processBlock;

    higgs::AudioProcessorEditor* createEditor() override;
    bool hasEditor() const override                         { return true; }

    const higgs::String getName() const override            { return HiggsPlugin_Name; }
    bool acceptsMidi() const override                       { return false; }
    bool producesMidi() const override                      { return false; }
    bool isMidiEffect() const override                      { return false; }
    double getTailLengthSeconds() const override            { return 0.0; }

    int getNumPrograms() override                           { return 1; }
    int getCurrentProgram() override                        { return 0; }
    void setCurrentProgram (int) override                   {}
    const higgs::String getProgramName (int) override       { return {}; }
    void changeProgramName (int, const higgs::String&) override {}

    void getStateInformation (higgs::MemoryBlock& destData) override;
    void setStateInformation (const void* data, int sizeInBytes) override;

    higgs::AudioProcessorValueTreeState parametros;

private:
    static higgs::AudioProcessorValueTreeState::ParameterLayout crearParametros();

    std::atomic<float>* gananciaDb = nullptr;
    higgs::SmoothedValue<float> gananciaSuavizada;

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GananciaProcessor)
};
```

```cpp title="PluginProcessor.cpp"
#include "PluginProcessor.h"
#include "PluginEditor.h"

GananciaProcessor::GananciaProcessor()
    : AudioProcessor (BusesProperties()
                          .withInput  ("Input",  higgs::AudioChannelSet::stereo(), true)
                          .withOutput ("Output", higgs::AudioChannelSet::stereo(), true)),
      parametros (*this, nullptr, "PARAMETROS", crearParametros())
{
    gananciaDb = parametros.getRawParameterValue ("ganancia");
}

higgs::AudioProcessorValueTreeState::ParameterLayout GananciaProcessor::crearParametros()
{
    return { std::make_unique<higgs::AudioParameterFloat> (higgs::ParameterID { "ganancia", 1 },
                                                           "Ganancia",
                                                           higgs::NormalisableRange<float> (-60.0f, 12.0f, 0.1f),
                                                           0.0f) };
}

void GananciaProcessor::prepareToPlay (double sampleRate, int)
{
    gananciaSuavizada.reset (sampleRate, 0.02);   // rampa de 20 ms para evitar chasquidos
    gananciaSuavizada.setCurrentAndTargetValue (higgs::Decibels::decibelsToGain (gananciaDb->load()));
}

bool GananciaProcessor::isBusesLayoutSupported (const BusesLayout& layouts) const
{
    const auto salida = layouts.getMainOutputChannelSet();

    if (salida != higgs::AudioChannelSet::mono() && salida != higgs::AudioChannelSet::stereo())
        return false;

    return salida == layouts.getMainInputChannelSet();
}

void GananciaProcessor::processBlock (higgs::AudioBuffer<float>& buffer, higgs::MidiBuffer&)
{
    higgs::ScopedNoDenormals sinDesnormales;

    // Limpia los canales de salida que no tengan entrada correspondiente.
    for (auto i = getTotalNumInputChannels(); i < getTotalNumOutputChannels(); ++i)
        buffer.clear (i, 0, buffer.getNumSamples());

    gananciaSuavizada.setTargetValue (higgs::Decibels::decibelsToGain (gananciaDb->load()));

    if (gananciaSuavizada.isSmoothing())
    {
        for (int muestra = 0; muestra < buffer.getNumSamples(); ++muestra)
        {
            const auto g = gananciaSuavizada.getNextValue();

            for (int canal = 0; canal < getTotalNumInputChannels(); ++canal)
                buffer.getWritePointer (canal)[muestra] *= g;
        }
    }
    else
    {
        buffer.applyGain (gananciaSuavizada.getTargetValue());
    }
}

higgs::AudioProcessorEditor* GananciaProcessor::createEditor()
{
    return new GananciaEditor (*this);
}

void GananciaProcessor::getStateInformation (higgs::MemoryBlock& destData)
{
    if (auto xml = parametros.copyState().createXml())
        copyXmlToBinary (*xml, destData);
}

void GananciaProcessor::setStateInformation (const void* data, int sizeInBytes)
{
    if (auto xml = getXmlFromBinary (data, sizeInBytes))
        if (xml->hasTagName (parametros.state.getType()))
            parametros.replaceState (higgs::ValueTree::fromXml (*xml));
}

// Punto de entrada que usan todos los wrappers de formato.
higgs::AudioProcessor* HIGGS_CALLTYPE createPluginFilter()
{
    return new GananciaProcessor();
}
```

Reglas importantes de `processBlock()`:

- Se ejecuta en el **hilo de audio**, en tiempo real. No reserves memoria, no bloquees mutex, no
  hagas E/S de disco ni llames a funciones de la interfaz gráfica desde aquí.
- Lee los parámetros a través del `std::atomic<float>*` que devuelve `getRawParameterValue()`:
  es seguro hacerlo desde cualquier hilo.
- `ScopedNoDenormals` evita la pérdida de rendimiento causada por números desnormalizados.

## El editor

```cpp title="PluginEditor.h"
#pragma once

#include "PluginProcessor.h"

class GananciaEditor final : public higgs::AudioProcessorEditor
{
public:
    explicit GananciaEditor (GananciaProcessor& p)
        : AudioProcessorEditor (&p),
          adjunto (p.parametros, "ganancia", deslizador)
    {
        deslizador.setSliderStyle (higgs::Slider::RotaryHorizontalVerticalDrag);
        deslizador.setTextBoxStyle (higgs::Slider::TextBoxBelow, false, 80, 20);
        deslizador.setTextValueSuffix (" dB");
        addAndMakeVisible (deslizador);

        setResizable (true, true);
        setSize (300, 300);
    }

    void paint (higgs::Graphics& g) override
    {
        g.fillAll (getLookAndFeel().findColour (higgs::ResizableWindow::backgroundColourId));
    }

    void resized() override
    {
        deslizador.setBounds (getLocalBounds().reduced (30));
    }

private:
    higgs::Slider deslizador;
    higgs::AudioProcessorValueTreeState::SliderAttachment adjunto;

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (GananciaEditor)
};
```

El `SliderAttachment` sincroniza en ambos sentidos el deslizador y el parámetro, incluida la
notificación de gestos de automatización al host. Declara el adjunto **después** del componente
para que se destruya antes que él.

!!! tip "Editor genérico"
    Mientras desarrollas el procesamiento puedes devolver
    `new higgs::GenericAudioProcessorEditor (*this)` en `createEditor()`: genera automáticamente
    un control por cada parámetro, como hace el ejemplo `GainPluginDemo`.

## Compilar, probar e instalar

```bash
cmake -B build
cmake --build build --target AudioPluginExample_Standalone   # aplicación independiente
cmake --build build --target AudioPluginExample_VST3
cmake --build build --target AudioPluginExample_AU           # solo macOS
```

Los resultados se generan en `build/AudioPluginExample_artefacts/<Configuración>/<Formato>/`.

- El formato **Standalone** es la forma más rápida de probar: crea una aplicación que envuelve tu
  procesador y permite elegir el dispositivo de audio.
- Para probar dentro de un host sin salir del repositorio, compila el
  [AudioPluginHost](../herramientas/audio-plugin-host.md).
- Con `COPY_PLUGIN_AFTER_BUILD TRUE` el plug-in se copia en la carpeta estándar del sistema tras
  cada compilación (puede requerir permisos de administrador en Windows).
- Para VST3, `VST3_AUTO_MANIFEST` (activado por defecto) genera `moduleinfo.json` dentro del
  bundle, lo que acelera el escaneo en los hosts compatibles.

Continúa con la [guía completa de plug-ins](../guias/plugins.md) para buses, MIDI, parámetros
avanzados, formatos y firma.
