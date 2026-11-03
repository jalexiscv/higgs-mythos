# higgs_dsp

Procesamiento digital de señal: filtros IIR, FIR y TPT, osciladores, ganancia, dinámica, *chorus*, *phaser*, reverberación, líneas de retardo, convolución, sobremuestreo, FFT, ventanas, diseño de filtros, aproximaciones matemáticas rápidas y registros SIMD, todo con el modelo `prepare`/`process`/`reset`.

## Ficha

| Campo | Valor |
|---|---|
| Identificador | `higgs_dsp` |
| Versión | 9.0.5 |
| Nombre declarado | Higgs Mythos DSP classes |
| Descripción declarada | Classes for audio buffer manipulation, digital audio processing, filtering, oversampling, fast math functions etc. |
| Estándar mínimo de C++ | C++17 |
| Licencia | AGPLv3 |
| Dependencias directas | [`higgs_audio_formats`](higgs_audio_formats.md) |
| Dependencias indirectas | [`higgs_core`](higgs_core.md), [`higgs_audio_basics`](higgs_audio_basics.md) |
| Frameworks de macOS | `Accelerate` |
| Frameworks de iOS | `Accelerate` |
| Código fuente | [`modules/higgs_dsp`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_dsp) |

## Uso

```cmake
target_link_libraries(MiTarget PRIVATE higgs::higgs_dsp)
```

```cpp
#include <higgs_dsp/higgs_dsp.h>
```

Guías relacionadas: [Procesamiento DSP](../guias/dsp.md).

## Opciones de configuración

Se cambian con `target_compile_definitions(MiTarget PUBLIC OPCION=valor)` o en la sección *Modules* de Mythos Studio.

| Opción | Por defecto | Descripción |
|---|---|---|
| `HIGGS_ASSERTION_FIRFILTER` | `1` | Cuando este indicador está habilitado, se generará una aserción durante la ejecución de las configuraciones DEBUG si usa una clase `FIRFilter` para procesar `FIRCoefficients` con un tamaño superior a 128, para indicarle que sería más eficiente usar la clase `Convolution` en su lugar. Está habilitado de forma predeterminada, pero quizá le convenga deshabilitarlo si realmente desea procesar un filtro de ese tipo en el dominio del tiempo. |
| `HIGGS_DSP_USE_INTEL_MKL` | `0` | Si este indicador está establecido, HIGGS usará la MKL de Intel para sus clases de FFT y convolución. Si usa el exportador de Visual Studio de Mythos Studio, también debe establecer la opción `Use MKL Library (oneAPI)` en `Sequential` o `Parallel` en la configuración del exportador. Si no usa el exportador de Visual Studio, la carpeta que contiene el encabezado `mkl_dfti.h` debe estar en sus rutas de búsqueda de encabezados, y debe enlazar con todas las bibliotecas MKL necesarias. |
| `HIGGS_DSP_USE_SHARED_FFTW` | `0` | Si este indicador está establecido, HIGGS buscará las bibliotecas compartidas de `fftw` en tiempo de ejecución y usará la biblioteca para sus clases de FFT y convolución. Si no se encuentra la biblioteca, se usarán las rutinas de FFT alternativas de HIGGS. Esto es especialmente útil en Linux, ya que `fftw` suele venir preinstalada en las distribuciones de Linux más populares. Debe respetar la licencia de FFTW al habilitar esta opción. |
| `HIGGS_DSP_USE_STATIC_FFTW` | `0` | Si este indicador está establecido, HIGGS usará las bibliotecas de `fftw` enlazadas estáticamente para sus clases de FFT y convolución. Debe agregar la carpeta de encabezados/bibliotecas de `fftw` a las rutas de búsqueda adicionales de encabezados/bibliotecas de su proyecto HIGGS. También debe agregar la propia biblioteca `fftw` a las bibliotecas adicionales que se proporcionan a su proyecto HIGGS durante el enlazado. Debe respetar la licencia de FFTW al habilitar esta opción. |
| `HIGGS_DSP_ENABLE_SNAP_TO_ZERO` | `1` | Habilita código en el módulo `dsp` para evitar los números desnormalizados de punto flotante durante el procesamiento de algunos de los filtros del módulo `dsp`. Habilitarlo añadirá una ligera sobrecarga de rendimiento a los filtros y algoritmos del módulo DSP. Si su aplicación de audio ya deshabilita por completo los números desnormalizados (por ejemplo, mediante la clase `ScopedNoDenormals` o el método `FloatVectorOperations::disableDenormalisedNumberSupport`), puede deshabilitar este indicador con seguridad para ahorrar algunos ciclos de CPU en los filtros y algoritmos del módulo DSP. |

## Catálogo de clases

54 clases y estructuras públicas declaradas en las cabeceras que incluye `higgs_dsp.h`, agrupadas por carpeta. La descripción es la primera frase de su comentario de documentación en el código.

### `containers`

| Clase | Descripción | Cabecera |
|---|---|---|
| `SIMDRegister` | Un envoltorio del tipo de registro SIMD nativo de la plataforma. | [`higgs_SIMDRegister.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_dsp/containers/higgs_SIMDRegister.h) |
| `AudioBlock` | Estructura de datos mínima y ligera que contiene una lista de punteros a canales con algún tipo de datos de muestra. | [`higgs_AudioBlock.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_dsp/containers/higgs_AudioBlock.h) |
| `SIMDInterleavingHelpers` | Funciones utilitarias para entrelazar y desentrelazar entre AudioBlock<float> y AudioBlock<SIMDRegister<float>>. | [`higgs_SIMDInterleavingHelpers.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_dsp/containers/higgs_SIMDInterleavingHelpers.h) |

### `maths`

| Clase | Descripción | Cabecera |
|---|---|---|
| `SpecialFunctions` | Contiene diversas funciones de diseño de filtros y de ventaneo. | [`higgs_SpecialFunctions.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_dsp/maths/higgs_SpecialFunctions.h) |
| `Matrix` | Clase general de matrices y vectores, pensada para la manipulación matemática clásica, como sumas, multiplicaciones y resolución de sistemas de ecuaciones lineales. | [`higgs_Matrix.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_dsp/maths/higgs_Matrix.h) |
| `Phase` | Representa un valor de fase creciente entre 0 y 2*pi. | [`higgs_Phase.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_dsp/maths/higgs_Phase.h) |
| `Polynomial` | Una clase que representa un polinomio. | [`higgs_Polynomial.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_dsp/maths/higgs_Polynomial.h) |
| `FastMathApproximations` | Esta clase contiene diversas aproximaciones rápidas de funciones matemáticas. | [`higgs_FastMathApproximations.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_dsp/maths/higgs_FastMathApproximations.h) |
| `LookupTable` | Clase para aproximar de forma eficiente operaciones aritméticas costosas. | [`higgs_LookupTable.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_dsp/maths/higgs_LookupTable.h) |
| `LookupTableTransform` | Clase para aproximar operaciones aritméticas costosas. | [`higgs_LookupTable.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_dsp/maths/higgs_LookupTable.h) |
| `LogRampedValue` | Clase de utilidad para valores lineales suavizados logarítmicamente. | [`higgs_LogRampedValue.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_dsp/maths/higgs_LogRampedValue.h) |

### `processors`

| Clase | Descripción | Cabecera |
|---|---|---|
| `ProcessSpec` | Esta estructura se pasa al método prepare() de un algoritmo DSP y contiene información sobre varios aspectos del contexto en el que puede esperar ser llamado. | [`higgs_ProcessContext.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_dsp/processors/higgs_ProcessContext.h) |
| `ProcessorState` | Esta es una clase base práctica para el estado de un procesador (como los valores de los parámetros), que normalmente se comparte entre varios procesadores. | [`higgs_ProcessContext.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_dsp/processors/higgs_ProcessContext.h) |
| `ProcessContextReplacing` | Contiene información de contexto que se pasa al método process de un algoritmo. | [`higgs_ProcessContext.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_dsp/processors/higgs_ProcessContext.h) |
| `ProcessContextNonReplacing` | Contiene información de contexto que se pasa al método process de un algoritmo. | [`higgs_ProcessContext.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_dsp/processors/higgs_ProcessContext.h) |
| `ProcessorBase` | Actúa como clase base polimórfica para procesadores. | [`higgs_ProcessorWrapper.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_dsp/processors/higgs_ProcessorWrapper.h) |
| `ProcessorWrapper` | Envuelve una instancia de una clase de procesador dada y la expone a través de la interfaz ProcessorBase. | [`higgs_ProcessorWrapper.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_dsp/processors/higgs_ProcessorWrapper.h) |
| `ProcessorChain` | Esta clase con plantillas variádicas permite unir cualquier número de clases de procesador en un único procesador que llamará a process() en todas ellas en secuencia. | [`higgs_ProcessorChain.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_dsp/processors/higgs_ProcessorChain.h) |
| `ProcessorDuplicator` | Convierte una clase de procesador mono en una versión multicanal duplicándola y aplicando búferes multicanal a un arreglo de instancias. | [`higgs_ProcessorDuplicator.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_dsp/processors/higgs_ProcessorDuplicator.h) |
| `ArrayCoefficients` | Un conjunto de coeficientes para usar en un objeto Filter. | [`higgs_IIRFilter.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_dsp/processors/higgs_IIRFilter.h) |
| `Coefficients` | Un conjunto de coeficientes para usar en un objeto Filter. | [`higgs_IIRFilter.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_dsp/processors/higgs_IIRFilter.h) |
| `Filter` | Una clase de procesamiento que puede aplicar filtrado IIR a una señal de audio, usando la estructura digital en forma directa II transpuesta. | [`higgs_IIRFilter.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_dsp/processors/higgs_IIRFilter.h) |
| `Filter` | Una clase de procesamiento que puede realizar filtrado FIR sobre una señal de audio en el dominio del tiempo. | [`higgs_FIRFilter.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_dsp/processors/higgs_FIRFilter.h) |
| `Coefficients` | Un conjunto de coeficientes para usar en un objeto FIR::Filter. | [`higgs_FIRFilter.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_dsp/processors/higgs_FIRFilter.h) |
| `Filter` | Filtro IIR que puede realizar filtrado pasa bajos, pasa banda y pasa altos sobre una señal de audio, con 12 dB de atenuación por octava, usando una estructura TPT, diseñado para modulación rápida (consulte la documentación de Vadim Zavalishin sobre estructuras TPT para más información). | [`higgs_StateVariableFilter.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_dsp/processors/higgs_StateVariableFilter.h) |
| `Parameters` | Estructura utilizada para los parámetros del filtro de variables de estado. | [`higgs_StateVariableFilter.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_dsp/processors/higgs_StateVariableFilter.h) |
| `FirstOrderTPTFilter` | Una clase de filtro de primer orden que usa la estructura TPT (transformación que preserva la topología). | [`higgs_FirstOrderTPTFilter.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_dsp/processors/higgs_FirstOrderTPTFilter.h) |
| `Panner` | Un procesador para realizar operaciones de paneo en búferes estéreo. | [`higgs_Panner.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_dsp/processors/higgs_Panner.h) |
| `None` | No se realizará interpolación entre muestras sucesivas en la línea de retardo. | [`higgs_DelayLine.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_dsp/processors/higgs_DelayLine.h) |
| `Linear` | Las muestras sucesivas de la línea de retardo se interpolarán linealmente. | [`higgs_DelayLine.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_dsp/processors/higgs_DelayLine.h) |
| `Lagrange3rd` | Las muestras sucesivas de la línea de retardo se interpolarán con un interpolador de Lagrange de tercer orden. | [`higgs_DelayLine.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_dsp/processors/higgs_DelayLine.h) |
| `Thiran` | Las muestras sucesivas en la línea de retardo se interpolarán mediante interpolación de Thiran de primer orden. | [`higgs_DelayLine.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_dsp/processors/higgs_DelayLine.h) |
| `DelayLine` | Un procesador de línea de retardo con varios algoritmos para el cálculo del retardo fraccionario, procesamiento por bloques y procesamiento muestra por muestra, útil para modular el retardo en tiempo real o crear un efecto de retardo estándar con realimentación. | [`higgs_DelayLine.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_dsp/processors/higgs_DelayLine.h) |
| `Oversampling` | Un procesador que realiza sobremuestreo multicanal. | [`higgs_Oversampling.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_dsp/processors/higgs_Oversampling.h) |
| `BallisticsFilter` | Un procesador para aplicar balística estándar de ataque/liberación a una señal de entrada. | [`higgs_BallisticsFilter.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_dsp/processors/higgs_BallisticsFilter.h) |
| `LinkwitzRileyFilter` | Clase de filtro diseñada para realizar separación multibanda usando la estructura TPT (Topology-Preserving Transform). | [`higgs_LinkwitzRileyFilter.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_dsp/processors/higgs_LinkwitzRileyFilter.h) |
| `DryWetMixer` | Un procesador para gestionar la mezcla dry/wet de dos señales de audio, en la que la señal wet puede tener latencia adicional. | [`higgs_DryWetMixer.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_dsp/processors/higgs_DryWetMixer.h) |
| `StateVariableTPTFilter` | Filtro IIR que puede realizar filtrado pasa bajos, pasa banda y pasa altos sobre una señal de audio, con 12 dB de atenuación por octava, usando una estructura TPT, diseñado para modulación rápida (consulte la documentación de Vadim Zavalishin sobre estructuras TPT para más información). | [`higgs_StateVariableTPTFilter.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_dsp/processors/higgs_StateVariableTPTFilter.h) |

### `frequency`

| Clase | Descripción | Cabecera |
|---|---|---|
| `FFT` | Realiza una transformada rápida de Fourier. | [`higgs_FFT.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_dsp/frequency/higgs_FFT.h) |
| `ConvolutionMessageQueue` | Utilizado por Convolution para despachar mensajes de actualización del motor en un hilo en segundo plano. | [`higgs_Convolution.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_dsp/frequency/higgs_Convolution.h) |
| `Convolution` | Realiza la convolución particionada estéreo de una señal de entrada con una respuesta al impulso en el dominio de la frecuencia, usando la clase FFT de HIGGS. | [`higgs_Convolution.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_dsp/frequency/higgs_Convolution.h) |
| `WindowingFunction` | Clase que proporciona múltiples funciones de ventana útiles para el diseño de filtros y analizadores de espectro. | [`higgs_Windowing.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_dsp/frequency/higgs_Windowing.h) |

### `filter_design`

| Clase | Descripción | Cabecera |
|---|---|---|
| `FilterDesign` | Esta clase proporciona un conjunto de funciones que generan FIR::Coefficients e IIR::Coefficients de filtros pasa bajos de orden alto. | [`higgs_FilterDesign.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_dsp/filter_design/higgs_FilterDesign.h) |

### `widgets`

| Clase | Descripción | Cabecera |
|---|---|---|
| `Reverb` | Envoltorio de procesador para higgs::Reverb que facilita su integración en ProcessorChain. | [`higgs_Reverb.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_dsp/widgets/higgs_Reverb.h) |
| `Bias` | Agrega un desplazamiento de DC (polarización de voltaje) a las muestras de audio. | [`higgs_Bias.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_dsp/widgets/higgs_Bias.h) |
| `Gain` | Aplica una ganancia a muestras de audio, ya sea como muestras individuales o como AudioBlocks. | [`higgs_Gain.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_dsp/widgets/higgs_Gain.h) |
| `WaveShaper` | Aplica waveshaping a muestras de audio, ya sea como muestras individuales o como AudioBlocks. | [`higgs_WaveShaper.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_dsp/widgets/higgs_WaveShaper.h) |
| `Oscillator` | Genera una señal a partir de una función proporcionada por el usuario. | [`higgs_Oscillator.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_dsp/widgets/higgs_Oscillator.h) |
| `LadderFilter` | Filtro multimodo basado en el filtro en escalera de Moog. | [`higgs_LadderFilter.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_dsp/widgets/higgs_LadderFilter.h) |
| `Compressor` | Un compresor sencillo con controles estándar de umbral, relación, tiempo de ataque y tiempo de liberación. | [`higgs_Compressor.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_dsp/widgets/higgs_Compressor.h) |
| `NoiseGate` | Una compuerta de ruido simple con controles estándar de umbral, relación, tiempo de ataque y tiempo de liberación. | [`higgs_NoiseGate.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_dsp/widgets/higgs_NoiseGate.h) |
| `Limiter` | Limitador sencillo con controles estándar de umbral y tiempo de liberación, que incluye dos compresores y un recortador duro a 0 dB. | [`higgs_Limiter.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_dsp/widgets/higgs_Limiter.h) |
| `Phaser` | Un phaser de 6 etapas que modula filtros pasa todo de primer orden para crear muescas de barrido en la respuesta en frecuencia de magnitud. | [`higgs_Phaser.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_dsp/widgets/higgs_Phaser.h) |
| `Chorus` | Un widget DSP de chorus sencillo que modula el retardo de una línea de retardo para crear muescas de barrido en la respuesta en frecuencia de magnitud. | [`higgs_Chorus.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_dsp/widgets/higgs_Chorus.h) |
