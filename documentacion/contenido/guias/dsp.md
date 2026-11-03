# Procesamiento DSP

El módulo `higgs_dsp` (espacio de nombres `higgs::dsp`) contiene bloques de procesamiento de señal
listos para usar y la infraestructura para combinarlos: filtros IIR/FIR/TPT, osciladores,
ganancia, compresor, limitador, puerta de ruido, *chorus*, *phaser*, reverberación, convolución,
sobremuestreo, FFT, ventanas, aproximaciones matemáticas rápidas y registros SIMD.

## El modelo `prepare` / `process` / `reset`

Todos los procesadores de `dsp` siguen la misma interfaz:

```cpp
void prepare (const higgs::dsp::ProcessSpec& spec);   // frecuencia, tamaño máximo de bloque, canales
template <typename Context>
void process (const Context& context) noexcept;        // procesa un bloque
void reset() noexcept;                                 // borra el estado interno
```

- `ProcessSpec` contiene `sampleRate`, `maximumBlockSize` y `numChannels`.
- `AudioBlock<float>` es una vista ligera (no propietaria) sobre los canales de un `AudioBuffer`.
- `ProcessContextReplacing` procesa *in situ*; `ProcessContextNonReplacing` escribe en un bloque
  de salida distinto.

```cpp
void prepareToPlay (double sampleRate, int samplesPerBlock) override
{
    higgs::dsp::ProcessSpec spec { sampleRate, (higgs::uint32) samplesPerBlock,
                                   (higgs::uint32) getTotalNumOutputChannels() };
    filtro.prepare (spec);
}

void processBlock (higgs::AudioBuffer<float>& buffer, higgs::MidiBuffer&) override
{
    higgs::dsp::AudioBlock<float> bloque (buffer);
    filtro.process (higgs::dsp::ProcessContextReplacing<float> (bloque));
}
```

## Cadenas de procesadores

`ProcessorChain` encadena procesadores en tiempo de compilación, sin coste de llamadas virtuales:

```cpp
using Filtro = higgs::dsp::StateVariableTPTFilter<float>;

higgs::dsp::ProcessorChain<higgs::dsp::Gain<float>,
                           Filtro,
                           higgs::dsp::Reverb> cadena;

void prepareToPlay (double sampleRate, int samplesPerBlock) override
{
    cadena.prepare ({ sampleRate, (higgs::uint32) samplesPerBlock, 2 });

    auto& ganancia = cadena.get<0>();
    ganancia.setGainDecibels (-3.0f);
    ganancia.setRampDurationSeconds (0.02);

    auto& filtro = cadena.get<1>();
    filtro.setType (higgs::dsp::StateVariableTPTFilterType::lowpass);
    filtro.setCutoffFrequency (1200.0f);
}

void processBlock (higgs::AudioBuffer<float>& buffer, higgs::MidiBuffer&) override
{
    higgs::dsp::AudioBlock<float> bloque (buffer);
    cadena.process (higgs::dsp::ProcessContextReplacing<float> (bloque));
}
```

Cada elemento de la cadena puede saltarse con `cadena.setBypassed<1> (true)`.

## Filtros

| Clase | Descripción |
|---|---|
| `IIR::Filter` + `IIR::Coefficients` | Filtro IIR genérico; los coeficientes se crean con `makeLowPass`, `makeHighPass`, `makeBandPass`, `makePeakFilter`, `makeLowShelf`, `makeHighShelf`, `makeNotch`, `makeAllPass`… |
| `FIR::Filter` + `FIR::Coefficients` | Filtro FIR; los coeficientes pueden diseñarse con `FilterDesign` |
| `StateVariableTPTFilter` | Filtro de variable de estado (paso bajo, banda y alto) con modulación estable |
| `FirstOrderTPTFilter` | Filtro TPT de primer orden |
| `LinkwitzRileyFilter` | Divisor de bandas Linkwitz-Riley |
| `LadderFilter` | Filtro multimodo tipo «ladder» |
| `BallisticsFilter` | Envolvente de ataque/liberación |
| `ProcessorDuplicator` | Convierte un procesador mono en multicanal duplicando su estado |

```cpp
using Mono = higgs::dsp::IIR::Filter<float>;
using Coefs = higgs::dsp::IIR::Coefficients<float>;

higgs::dsp::ProcessorDuplicator<Mono, Coefs> pasoBajo;

// en prepareToPlay:
*pasoBajo.state = *Coefs::makeLowPass (sampleRate, 800.0f);
pasoBajo.prepare (spec);
```

!!! tip "FIR largos"
    En depuración, `HIGGS_ASSERTION_FIRFILTER` avisa si usas un `FIR::Filter` con más de 128
    coeficientes: a partir de ese tamaño es más eficiente usar `Convolution`.

## Efectos y generadores

| Clase | Función |
|---|---|
| `Gain` | Ganancia con rampa |
| `Bias` | Desplazamiento de continua |
| `WaveShaper` | Saturación mediante una función |
| `Oscillator` | Oscilador a partir de una función o tabla de ondas |
| `Compressor`, `Limiter`, `NoiseGate` | Dinámica |
| `Chorus`, `Phaser` | Modulación |
| `Reverb` | Envoltorio de la reverberación de `higgs_audio_basics` |
| `DelayLine` | Línea de retardo con interpolación fraccionaria (Lagrange, Thiran, lineal…) |
| `DryWetMixer` | Mezcla seco/húmedo con compensación de latencia |
| `Panner` | Panorámica estéreo con varias leyes |

```cpp
higgs::dsp::Oscillator<float> osc;
osc.initialise ([] (float x) { return std::sin (x); }, 128);   // tabla de 128 puntos
osc.setFrequency (440.0f);
```

## Convolución

`Convolution` aplica respuestas al impulso (reverberaciones, simulaciones de altavoz) con
convolución particionada uniforme o no uniforme. La carga de la respuesta se hace en un hilo de
fondo, de modo que puede cambiarse durante la reproducción sin cortes:

```cpp
higgs::dsp::Convolution convolucion;

convolucion.loadImpulseResponse (higgs::File ("/ruta/sala.wav"),
                                 higgs::dsp::Convolution::Stereo::yes,
                                 higgs::dsp::Convolution::Trim::yes,
                                 0);   // 0 = usar toda la respuesta
```

## Sobremuestreo

```cpp
higgs::dsp::Oversampling<float> sobremuestreo { 2, 2,   // 2 canales, factor 2^2 = 4x
    higgs::dsp::Oversampling<float>::filterHalfBandPolyphaseIIR };

// prepareToPlay:
sobremuestreo.initProcessing ((size_t) samplesPerBlock);

// processBlock:
auto bloqueAlto = sobremuestreo.processSamplesUp (bloque);
saturador.process (higgs::dsp::ProcessContextReplacing<float> (bloqueAlto));
sobremuestreo.processSamplesDown (bloque);
```

Informa al host de la latencia añadida con `setLatencySamples ((int) sobremuestreo.getLatencyInSamples())`.

## Análisis espectral

```cpp
constexpr int orden = 11;                              // 2^11 = 2048 puntos
higgs::dsp::FFT fft { orden };
higgs::dsp::WindowingFunction<float> ventana { 1 << orden, higgs::dsp::WindowingFunction<float>::hann };

std::array<float, 2 << orden> datos {};               // el doble del tamaño de la FFT
// ...copia 2048 muestras en datos...
ventana.multiplyWithWindowingTable (datos.data(), 1 << orden);
fft.performFrequencyOnlyForwardTransform (datos.data());
```

El backend de FFT puede ser la implementación propia, Apple vDSP, Intel MKL
(`HIGGS_DSP_USE_INTEL_MKL`) o FFTW (`HIGGS_DSP_USE_SHARED_FFTW`, `HIGGS_DSP_USE_STATIC_FFTW`).

## Utilidades matemáticas

- `SIMDRegister<T>`: envoltorio portable sobre los registros SIMD nativos (SSE, AVX, NEON).
- `FastMathApproximations`, `LookupTable`, `LookupTableTransform`: aproximaciones rápidas de
  funciones costosas.
- `Matrix`, `Polynomial`, `Phase`, `SpecialFunctions`, `LogRampedValue`.
- `HIGGS_DSP_ENABLE_SNAP_TO_ZERO` evita números desnormalizados en algunos filtros a cambio de
  un pequeño coste.

## Demos relacionadas

`GainDemo`, `IIRFilterDemo`, `FIRFilterDemo`, `StateVariableFilterDemo`, `OscillatorDemo`,
`OverdriveDemo`, `WaveShaperTanhDemo`, `ConvolutionDemo`, `SIMDRegisterDemo` y el plug-in
`DSPModulePluginDemo`.
