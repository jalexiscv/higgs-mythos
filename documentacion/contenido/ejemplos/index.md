# Catálogo de ejemplos

El repositorio incluye **85 ejemplos** en `examples/`, bajo licencia ISC. Cada uno es un [proyecto PIP](../compilacion/pip.md) de un solo archivo. Se pueden compilar por separado o todos juntos en el **DemoRunner** (véase [Compilar los ejemplos](../primeros-pasos/compilar-ejemplos.md)).

```bash
cmake . -B cmake-build -DHIGGS_BUILD_EXAMPLES=ON
cmake --build cmake-build --target DemoRunner
cmake --build cmake-build --target <nombre del ejemplo>
```

Además, `examples/CMake` contiene las plantillas `GuiApp`, `ConsoleApp` y `AudioPlugin`, y `examples/Assets` los recursos compartidos (audio, imágenes, modelos 3D…) que usan las demos.

## Audio

Aplicaciones de audio: dispositivos, reproducción, grabación, síntesis, MIDI y MIDI 2.0.

| Ejemplo | Tipo | Descripción | Módulos principales |
|---|---|---|---|
| [`AudioAppDemo`](https://github.com/jalexiscv/higgs-mythos/blob/main/examples/Audio/AudioAppDemo.h) | Aplicación | Aplicación de audio sencilla. | `higgs_audio_basics`, `higgs_audio_devices`, `higgs_audio_formats`, `higgs_audio_processors` … |
| [`AudioLatencyDemo`](https://github.com/jalexiscv/higgs-mythos/blob/main/examples/Audio/AudioLatencyDemo.h) | Aplicación | Prueba la latencia de audio de un dispositivo. | `higgs_audio_basics`, `higgs_audio_devices`, `higgs_audio_formats`, `higgs_audio_processors` … |
| [`AudioPlaybackDemo`](https://github.com/jalexiscv/higgs-mythos/blob/main/examples/Audio/AudioPlaybackDemo.h) | Aplicación | Reproduce un archivo de audio. | `higgs_audio_basics`, `higgs_audio_devices`, `higgs_audio_formats`, `higgs_audio_processors` … |
| [`AudioRecordingDemo`](https://github.com/jalexiscv/higgs-mythos/blob/main/examples/Audio/AudioRecordingDemo.h) | Aplicación | Graba audio en un archivo. | `higgs_audio_basics`, `higgs_audio_devices`, `higgs_audio_formats`, `higgs_audio_processors` … |
| [`AudioSettingsDemo`](https://github.com/jalexiscv/higgs-mythos/blob/main/examples/Audio/AudioSettingsDemo.h) | Aplicación | Muestra información sobre los dispositivos de audio. | `higgs_audio_basics`, `higgs_audio_devices`, `higgs_audio_formats`, `higgs_audio_processors` … |
| [`AudioSynthesiserDemo`](https://github.com/jalexiscv/higgs-mythos/blob/main/examples/Audio/AudioSynthesiserDemo.h) | Aplicación | Aplicación de sintetizador sencilla. | `higgs_audio_basics`, `higgs_audio_devices`, `higgs_audio_formats`, `higgs_audio_processors` … |
| [`AudioWorkgroupDemo`](https://github.com/jalexiscv/higgs-mythos/blob/main/examples/Audio/AudioWorkgroupDemo.h) | Aplicación | Aplicación de demostración sencilla de grupos de trabajo de audio (audio workgroups). | `higgs_audio_basics`, `higgs_audio_devices`, `higgs_audio_formats`, `higgs_audio_processors` … |
| [`CapabilityInquiryDemo`](https://github.com/jalexiscv/higgs-mythos/blob/main/examples/Audio/CapabilityInquiryDemo.h) | Aplicación | Realiza transacciones de MIDI Capability Inquiry. | `higgs_audio_basics`, `higgs_audio_devices`, `higgs_midi_ci` |
| [`MPEDemo`](https://github.com/jalexiscv/higgs-mythos/blob/main/examples/Audio/MPEDemo.h) | Aplicación | Aplicación de sintetizador MPE sencilla. | `higgs_audio_basics`, `higgs_audio_devices`, `higgs_audio_formats`, `higgs_audio_processors` … |
| [`MidiDemo`](https://github.com/jalexiscv/higgs-mythos/blob/main/examples/Audio/MidiDemo.h) | Aplicación | Gestiona mensajes MIDI entrantes y salientes en formato de flujo de bytes MIDI 1.0. Para ver un ejemplo de cómo gestionar mensajes MIDI 2.0 en formato Universal MIDI Packet, consulte `UMPDemo`. | `higgs_audio_basics`, `higgs_audio_devices`, `higgs_audio_formats`, `higgs_audio_processors` … |
| [`PluckedStringsDemo`](https://github.com/jalexiscv/higgs-mythos/blob/main/examples/Audio/PluckedStringsDemo.h) | Aplicación | Simulación del sonido de una cuerda pulsada. | `higgs_audio_basics`, `higgs_audio_devices`, `higgs_audio_formats`, `higgs_audio_processors` … |
| [`SimpleFFTDemo`](https://github.com/jalexiscv/higgs-mythos/blob/main/examples/Audio/SimpleFFTDemo.h) | Aplicación | Aplicación de FFT sencilla. | `higgs_audio_basics`, `higgs_audio_devices`, `higgs_audio_formats`, `higgs_audio_processors` … |
| [`UmpDemo`](https://github.com/jalexiscv/higgs-mythos/blob/main/examples/Audio/UmpDemo.h) | Aplicación | Demuestra técnicas para enviar y recibir mensajes MIDI en formato Universal MIDI Packet. | `higgs_audio_basics`, `higgs_audio_devices`, `higgs_audio_formats`, `higgs_audio_processors` … |

## DSP

Demostraciones del módulo `higgs_dsp`.

| Ejemplo | Tipo | Descripción | Módulos principales |
|---|---|---|---|
| [`ConvolutionDemo`](https://github.com/jalexiscv/higgs-mythos/blob/main/examples/DSP/ConvolutionDemo.h) | Aplicación | Demostración de convolución usando el módulo DSP. | `higgs_audio_basics`, `higgs_audio_devices`, `higgs_audio_formats`, `higgs_audio_processors` … |
| [`FIRFilterDemo`](https://github.com/jalexiscv/higgs-mythos/blob/main/examples/DSP/FIRFilterDemo.h) | Aplicación | Demostración de filtro FIR usando el módulo DSP. | `higgs_audio_basics`, `higgs_audio_devices`, `higgs_audio_formats`, `higgs_audio_processors` … |
| [`GainDemo`](https://github.com/jalexiscv/higgs-mythos/blob/main/examples/DSP/GainDemo.h) | Aplicación | Demostración de ganancia usando el módulo DSP. | `higgs_audio_basics`, `higgs_audio_devices`, `higgs_audio_formats`, `higgs_audio_processors` … |
| [`IIRFilterDemo`](https://github.com/jalexiscv/higgs-mythos/blob/main/examples/DSP/IIRFilterDemo.h) | Aplicación | Demostración de filtro IIR usando el módulo DSP. | `higgs_audio_basics`, `higgs_audio_devices`, `higgs_audio_formats`, `higgs_audio_processors` … |
| [`OscillatorDemo`](https://github.com/jalexiscv/higgs-mythos/blob/main/examples/DSP/OscillatorDemo.h) | Aplicación | Demostración de oscilador usando el módulo DSP. | `higgs_audio_basics`, `higgs_audio_devices`, `higgs_audio_formats`, `higgs_audio_processors` … |
| [`OverdriveDemo`](https://github.com/jalexiscv/higgs-mythos/blob/main/examples/DSP/OverdriveDemo.h) | Aplicación | Demostración de overdrive usando el módulo DSP. | `higgs_audio_basics`, `higgs_audio_devices`, `higgs_audio_formats`, `higgs_audio_processors` … |
| [`SIMDRegisterDemo`](https://github.com/jalexiscv/higgs-mythos/blob/main/examples/DSP/SIMDRegisterDemo.h) | Aplicación | Demostración de registros SIMD usando el módulo DSP. | `higgs_audio_basics`, `higgs_audio_devices`, `higgs_audio_formats`, `higgs_audio_processors` … |
| [`StateVariableFilterDemo`](https://github.com/jalexiscv/higgs-mythos/blob/main/examples/DSP/StateVariableFilterDemo.h) | Aplicación | Demostración de filtro de variables de estado usando el módulo DSP. | `higgs_audio_basics`, `higgs_audio_devices`, `higgs_audio_formats`, `higgs_audio_processors` … |
| [`WaveShaperTanhDemo`](https://github.com/jalexiscv/higgs-mythos/blob/main/examples/DSP/WaveShaperTanhDemo.h) | Aplicación | Demostración de modelador de onda (waveshaper) con tanh usando el módulo DSP. | `higgs_audio_basics`, `higgs_audio_devices`, `higgs_audio_formats`, `higgs_audio_processors` … |

## Interfaz gráfica

Componentes, *layouts*, gráficos, fuentes, animación, OpenGL, vídeo y web.

| Ejemplo | Tipo | Descripción | Módulos principales |
|---|---|---|---|
| [`AccessibilityDemo`](https://github.com/jalexiscv/higgs-mythos/blob/main/examples/GUI/AccessibilityDemo.h) | Aplicación | Interfaz de usuario sencilla que demuestra las funciones de accesibilidad en las plataformas compatibles. | `higgs_gui_extra` |
| [`AnimationAppDemo`](https://github.com/jalexiscv/higgs-mythos/blob/main/examples/GUI/AnimationAppDemo.h) | Aplicación | Aplicación de animación sencilla. | `higgs_gui_extra` |
| [`AnimationEasingDemo`](https://github.com/jalexiscv/higgs-mythos/blob/main/examples/GUI/AnimationEasingDemo.h) | Aplicación | Aplicación para comparar curvas de suavizado (easings) de animación. | `higgs_animation` |
| [`AnimatorsDemo`](https://github.com/jalexiscv/higgs-mythos/blob/main/examples/GUI/AnimatorsDemo.h) | Aplicación | Aplicación que demuestra cómo pueden usarse las clases `Animator` que proporciona HIGGS para crear animaciones dinámicas, componibles y fluidas. | `higgs_animation` |
| [`BouncingBallWavetableDemo`](https://github.com/jalexiscv/higgs-mythos/blob/main/examples/GUI/BouncingBallWavetableDemo.h) | Aplicación | Síntesis por tabla de ondas con una pelota que rebota. | `higgs_audio_basics`, `higgs_audio_devices`, `higgs_audio_formats`, `higgs_audio_processors` … |
| [`CameraDemo`](https://github.com/jalexiscv/higgs-mythos/blob/main/examples/GUI/CameraDemo.h) | Aplicación | Presenta las funciones de cámara. | `higgs_audio_basics`, `higgs_audio_devices`, `higgs_cryptography`, `higgs_gui_extra` … |
| [`CodeEditorDemo`](https://github.com/jalexiscv/higgs-mythos/blob/main/examples/GUI/CodeEditorDemo.h) | Aplicación | Muestra un editor de código. | `higgs_gui_extra` |
| [`ComponentDemo`](https://github.com/jalexiscv/higgs-mythos/blob/main/examples/GUI/ComponentDemo.h) | Aplicación | Muestra una cuadrícula de luces. | `higgs_gui_basics` |
| [`ComponentDiagnosticsDemo`](https://github.com/jalexiscv/higgs-mythos/blob/main/examples/GUI/ComponentDiagnosticsDemo.h) | Aplicación | Muestra varios tipos de ventanas. | `higgs_gui_basics` |
| [`ComponentTransformsDemo`](https://github.com/jalexiscv/higgs-mythos/blob/main/examples/GUI/ComponentTransformsDemo.h) | Aplicación | Aplica transformaciones a componentes. | `higgs_gui_extra` |
| [`DialogsDemo`](https://github.com/jalexiscv/higgs-mythos/blob/main/examples/GUI/DialogsDemo.h) | Aplicación | Muestra distintos tipos de ventanas de diálogo. | `higgs_gui_extra` |
| [`FlexBoxDemo`](https://github.com/jalexiscv/higgs-mythos/blob/main/examples/GUI/FlexBoxDemo.h) | Aplicación | Diseños adaptables con `FlexBox`. | `higgs_gui_basics` |
| [`FontFeaturesDemo`](https://github.com/jalexiscv/higgs-mythos/blob/main/examples/GUI/FontFeaturesDemo.h) | Aplicación | Muestra distintas características de fuentes. | `higgs_gui_basics` |
| [`FontVariablesDemo`](https://github.com/jalexiscv/higgs-mythos/blob/main/examples/GUI/FontVariablesDemo.h) | Aplicación | Presenta la compatibilidad con fuentes variables OpenType. | `higgs_gui_basics` |
| [`FontsDemo`](https://github.com/jalexiscv/higgs-mythos/blob/main/examples/GUI/FontsDemo.h) | Aplicación | Muestra distintos estilos y tipos de fuentes. | `higgs_gui_basics` |
| [`GraphicsDemo`](https://github.com/jalexiscv/higgs-mythos/blob/main/examples/GUI/GraphicsDemo.h) | Aplicación | Presenta diversas funciones gráficas. | `higgs_gui_basics` |
| [`GridDemo`](https://github.com/jalexiscv/higgs-mythos/blob/main/examples/GUI/GridDemo.h) | Aplicación | Diseños adaptables con `Grid`. | `higgs_gui_basics` |
| [`HelloWorldDemo`](https://github.com/jalexiscv/higgs-mythos/blob/main/examples/GUI/HelloWorldDemo.h) | Aplicación | Aplicación HelloWorld sencilla. | `higgs_gui_basics` |
| [`ImagesDemo`](https://github.com/jalexiscv/higgs-mythos/blob/main/examples/GUI/ImagesDemo.h) | Aplicación | Muestra archivos de imagen. | `higgs_gui_basics` |
| [`KeyMappingsDemo`](https://github.com/jalexiscv/higgs-mythos/blob/main/examples/GUI/KeyMappingsDemo.h) | Aplicación | Presenta las funciones de asignación de teclas. | `higgs_gui_extra` |
| [`LineSpacingDemo`](https://github.com/jalexiscv/higgs-mythos/blob/main/examples/GUI/LineSpacingDemo.h) | Aplicación | Demuestra las opciones de interlineado de `GlyphArrangement`. | `higgs_gui_basics` |
| [`LookAndFeelDemo`](https://github.com/jalexiscv/higgs-mythos/blob/main/examples/GUI/LookAndFeelDemo.h) | Aplicación | Presenta componentes con look and feel personalizado. | `higgs_gui_basics` |
| [`MDIDemo`](https://github.com/jalexiscv/higgs-mythos/blob/main/examples/GUI/MDIDemo.h) | Aplicación | Muestra y edita archivos MDI. | `higgs_gui_extra` |
| [`MenusDemo`](https://github.com/jalexiscv/higgs-mythos/blob/main/examples/GUI/MenusDemo.h) | Aplicación | Presenta las funciones de menús. | `higgs_gui_extra` |
| [`MultiTouchDemo`](https://github.com/jalexiscv/higgs-mythos/blob/main/examples/GUI/MultiTouchDemo.h) | Aplicación | Presenta las funciones multitáctiles. | `higgs_gui_basics` |
| [`OpenGLAppDemo`](https://github.com/jalexiscv/higgs-mythos/blob/main/examples/GUI/OpenGLAppDemo.h) | Aplicación | Aplicación OpenGL sencilla. | `higgs_gui_extra`, `higgs_opengl` |
| [`OpenGLDemo`](https://github.com/jalexiscv/higgs-mythos/blob/main/examples/GUI/OpenGLDemo.h) | Aplicación | Aplicación OpenGL 3D sencilla. | `higgs_gui_extra`, `higgs_opengl` |
| [`OpenGLDemo2D`](https://github.com/jalexiscv/higgs-mythos/blob/main/examples/GUI/OpenGLDemo2D.h) | Aplicación | Aplicación OpenGL 2D sencilla. | `higgs_gui_extra`, `higgs_opengl` |
| [`PropertiesDemo`](https://github.com/jalexiscv/higgs-mythos/blob/main/examples/GUI/PropertiesDemo.h) | Aplicación | Muestra diversos componentes de propiedades. | `higgs_gui_basics` |
| [`VideoDemo`](https://github.com/jalexiscv/higgs-mythos/blob/main/examples/GUI/VideoDemo.h) | Aplicación | Reproduce archivos de video. | `higgs_gui_extra`, `higgs_video` |
| [`WebBrowserDemo`](https://github.com/jalexiscv/higgs-mythos/blob/main/examples/GUI/WebBrowserDemo.h) | Aplicación | Muestra un navegador web. | `higgs_gui_extra` |
| [`WidgetsDemo`](https://github.com/jalexiscv/higgs-mythos/blob/main/examples/GUI/WidgetsDemo.h) | Aplicación | Presenta diversos widgets. | `higgs_gui_extra` |
| [`WindowsDemo`](https://github.com/jalexiscv/higgs-mythos/blob/main/examples/GUI/WindowsDemo.h) | Aplicación | Muestra varios tipos de ventanas. | `higgs_gui_extra` |

## Plug-ins

Plug-ins de audio de ejemplo, que se compilan en todos los formatos disponibles.

| Ejemplo | Tipo | Descripción | Módulos principales |
|---|---|---|---|
| [`ARAPluginDemo`](https://github.com/jalexiscv/higgs-mythos/blob/main/examples/Plugins/ARAPluginDemo.h) | Plug-in | Plug-in de audio que usa la API de ARA. | `higgs_audio_basics`, `higgs_audio_devices`, `higgs_audio_formats`, `higgs_audio_plugin_client` … |
| [`AUv3SynthPlugin`](https://github.com/jalexiscv/higgs-mythos/blob/main/examples/Plugins/AUv3SynthPluginDemo.h) | Plug-in | Plug-in de audio de sintetizador AUv3. | `higgs_audio_basics`, `higgs_audio_devices`, `higgs_audio_formats`, `higgs_audio_plugin_client` … |
| [`ArpeggiatorPlugin`](https://github.com/jalexiscv/higgs-mythos/blob/main/examples/Plugins/ArpeggiatorPluginDemo.h) | Plug-in | Plug-in de audio de arpegiador. | `higgs_audio_basics`, `higgs_audio_devices`, `higgs_audio_formats`, `higgs_audio_plugin_client` … |
| [`AudioPluginDemo`](https://github.com/jalexiscv/higgs-mythos/blob/main/examples/Plugins/AudioPluginDemo.h) | Plug-in | Plug-in de audio de sintetizador. | `higgs_audio_basics`, `higgs_audio_devices`, `higgs_audio_formats`, `higgs_audio_plugin_client` … |
| [`DSPModulePluginDemo`](https://github.com/jalexiscv/higgs-mythos/blob/main/examples/Plugins/DSPModulePluginDemo.h) | Plug-in | Un plug-in de audio que usa el módulo DSP. | `higgs_audio_basics`, `higgs_audio_devices`, `higgs_audio_formats`, `higgs_audio_plugin_client` … |
| [`GainPlugin`](https://github.com/jalexiscv/higgs-mythos/blob/main/examples/Plugins/GainPluginDemo.h) | Plug-in | Plug-in de audio de ganancia. | `higgs_audio_basics`, `higgs_audio_devices`, `higgs_audio_formats`, `higgs_audio_plugin_client` … |
| [`HostPluginDemo`](https://github.com/jalexiscv/higgs-mythos/blob/main/examples/Plugins/HostPluginDemo.h) | Plug-in | Plug-in que puede alojar otros plug-ins. | `higgs_audio_basics`, `higgs_audio_devices`, `higgs_audio_formats`, `higgs_audio_plugin_client` … |
| [`MIDILogger`](https://github.com/jalexiscv/higgs-mythos/blob/main/examples/Plugins/MidiLoggerPluginDemo.h) | Plug-in | Registra los mensajes MIDI entrantes. | `higgs_audio_basics`, `higgs_audio_devices`, `higgs_audio_formats`, `higgs_audio_plugin_client` … |
| [`MultiOutSynthPlugin`](https://github.com/jalexiscv/higgs-mythos/blob/main/examples/Plugins/MultiOutSynthPluginDemo.h) | Plug-in | Plug-in de audio de sintetizador con múltiples salidas. | `higgs_audio_basics`, `higgs_audio_devices`, `higgs_audio_formats`, `higgs_audio_plugin_client` … |
| [`NoiseGatePlugin`](https://github.com/jalexiscv/higgs-mythos/blob/main/examples/Plugins/NoiseGatePluginDemo.h) | Plug-in | Plug-in de audio de puerta de ruido. | `higgs_audio_basics`, `higgs_audio_devices`, `higgs_audio_formats`, `higgs_audio_plugin_client` … |
| [`ReaperEmbeddedViewDemo`](https://github.com/jalexiscv/higgs-mythos/blob/main/examples/Plugins/ReaperEmbeddedViewPluginDemo.h) | Plug-in | Un plug-in de audio que incrusta una vista secundaria en los formatos VST2 y VST3 en REAPER. | `higgs_audio_basics`, `higgs_audio_devices`, `higgs_audio_formats`, `higgs_audio_plugin_client` … |
| [`SamplerPlugin`](https://github.com/jalexiscv/higgs-mythos/blob/main/examples/Plugins/SamplerPluginDemo.h) | Plug-in | Plug-in de audio de sampler. | `higgs_audio_basics`, `higgs_audio_devices`, `higgs_audio_formats`, `higgs_audio_plugin_client` … |
| [`SurroundPlugin`](https://github.com/jalexiscv/higgs-mythos/blob/main/examples/Plugins/SurroundPluginDemo.h) | Plug-in | Plug-in de audio envolvente (surround). | `higgs_audio_basics`, `higgs_audio_devices`, `higgs_audio_formats`, `higgs_audio_plugin_client` … |
| [`WebViewPluginDemo`](https://github.com/jalexiscv/higgs-mythos/blob/main/examples/Plugins/WebViewPluginDemo.h) | Plug-in | Plug-in de audio de filtrado con una interfaz de usuario HTML/JS. | `higgs_audio_basics`, `higgs_audio_devices`, `higgs_audio_formats`, `higgs_audio_plugin_client` … |

## Utilidades

Red, hilos, datos, criptografía, JavaScript, OSC y otras utilidades.

| Ejemplo | Tipo | Descripción | Módulos principales |
|---|---|---|---|
| [`AnalyticsCollectionDemo`](https://github.com/jalexiscv/higgs-mythos/blob/main/examples/Utilities/AnalyticsCollectionDemo.h) | Aplicación | Recopila datos analíticos. | `higgs_analytics` |
| [`Box2DDemo`](https://github.com/jalexiscv/higgs-mythos/blob/main/examples/Utilities/Box2DDemo.h) | Aplicación | Presenta funciones de gráficos 2D. | `higgs_box2d` |
| [`ChildProcessDemo`](https://github.com/jalexiscv/higgs-mythos/blob/main/examples/Utilities/ChildProcessDemo.h) | Consola | Inicia aplicaciones como procesos secundarios. | `higgs_gui_basics` |
| [`CryptographyDemo`](https://github.com/jalexiscv/higgs-mythos/blob/main/examples/Utilities/CryptographyDemo.h) | Aplicación | Cifra y descifra datos. | `higgs_cryptography` |
| [`InAppPurchasesDemo`](https://github.com/jalexiscv/higgs-mythos/blob/main/examples/Utilities/InAppPurchasesDemo.h) | Aplicación | Presenta las funciones de compras dentro de la aplicación. Para ejecutar esta demostración, debe habilitar la opción `In-App Purchases Capability` en el exportador de Mythos Studio. | `higgs_audio_basics`, `higgs_audio_devices`, `higgs_audio_formats`, `higgs_audio_processors` … |
| [`JavaScriptDemo`](https://github.com/jalexiscv/higgs-mythos/blob/main/examples/Utilities/JavaScriptDemo.h) | Aplicación | Presenta funciones de JavaScript. | `higgs_gui_extra`, `higgs_javascript` |
| [`LiveConstantDemo`](https://github.com/jalexiscv/higgs-mythos/blob/main/examples/Utilities/LiveConstantDemo.h) | Aplicación | Demuestra la macro de constantes en vivo. | `higgs_gui_extra` |
| [`MultithreadingDemo`](https://github.com/jalexiscv/higgs-mythos/blob/main/examples/Utilities/MultithreadingDemo.h) | Aplicación | Demuestra la ejecución con múltiples hilos. | `higgs_gui_basics` |
| [`NetworkingDemo`](https://github.com/jalexiscv/higgs-mythos/blob/main/examples/Utilities/NetworkingDemo.h) | Aplicación | Presenta funciones de red. | `higgs_gui_extra` |
| [`OSCDemo`](https://github.com/jalexiscv/higgs-mythos/blob/main/examples/Utilities/OSCDemo.h) | Aplicación | Aplicación que usa el protocolo OSC. | `higgs_osc` |
| [`PushNotificationsDemo`](https://github.com/jalexiscv/higgs-mythos/blob/main/examples/Utilities/PushNotificationsDemo.h) | Aplicación | Presenta las funciones de notificaciones push. Para ejecutar esta demostración, debe habilitar la opción `Push Notifications Capability` en el exportador de Mythos Studio. | `higgs_audio_basics`, `higgs_audio_devices`, `higgs_audio_formats`, `higgs_audio_processors` … |
| [`SystemInfoDemo`](https://github.com/jalexiscv/higgs-mythos/blob/main/examples/Utilities/SystemInfoDemo.h) | Aplicación | Muestra información del sistema. | `higgs_gui_basics` |
| [`TimersAndEventsDemo`](https://github.com/jalexiscv/higgs-mythos/blob/main/examples/Utilities/TimersAndEventsDemo.h) | Aplicación | Aplicación que usa temporizadores y eventos. | `higgs_gui_basics` |
| [`UnitTestsDemo`](https://github.com/jalexiscv/higgs-mythos/blob/main/examples/Utilities/UnitTestsDemo.h) | Aplicación | Ejecuta pruebas unitarias. | `higgs_analytics`, `higgs_audio_basics`, `higgs_audio_devices`, `higgs_audio_formats` … |
| [`ValueTreesDemo`](https://github.com/jalexiscv/higgs-mythos/blob/main/examples/Utilities/ValueTreesDemo.h) | Aplicación | Presenta las funciones de los árboles de valores. | `higgs_gui_basics` |
| [`XMLandJSONDemo`](https://github.com/jalexiscv/higgs-mythos/blob/main/examples/Utilities/XMLandJSONDemo.h) | Aplicación | Lee archivos XML y JSON. | `higgs_gui_extra` |
