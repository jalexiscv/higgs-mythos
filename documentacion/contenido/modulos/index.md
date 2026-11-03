# Módulos

Higgs Mythos está formado por 24 módulos. Cada ficha incluye sus dependencias, los frameworks y bibliotecas del sistema que necesita, sus opciones de configuración, el código de terceros que incluye y el catálogo completo de sus clases públicas.

El diagrama de dependencias está en [Arquitectura del framework](../conceptos/arquitectura.md). Para la referencia de cada método, consulta los comentarios de las cabeceras o genera la referencia Doxygen (véase [Depuración y diagnóstico](../conceptos/depuracion.md#referencia-del-api-generada-con-doxygen)).

## Núcleo

| Módulo | Descripción | Depende de | Clases | Opciones |
|---|---|---|---|---|
| [`higgs_core`](higgs_core.md) | El conjunto esencial de clases básicas que necesitan todos los demás módulos. | — | 162 | 11 |
| [`higgs_events`](higgs_events.md) | La infraestructura del bucle de mensajes de la aplicación. | `higgs_core` | 24 | 1 |
| [`higgs_data_structures`](higgs_data_structures.md) | Estructuras de datos para el modelo de la aplicación. | `higgs_events` | 9 | 0 |
| [`higgs_cryptography`](higgs_cryptography.md) | Funciones criptográficas básicas. | `higgs_core` | 6 | 0 |

## Gráficos e interfaz

| Módulo | Descripción | Depende de | Clases | Opciones |
|---|---|---|---|---|
| [`higgs_graphics`](higgs_graphics.md) | El motor de gráficos 2D. | `higgs_events` | 64 | 6 |
| [`higgs_gui_basics`](higgs_gui_basics.md) | Las clases fundamentales de la interfaz de usuario. | `higgs_graphics`, `higgs_data_structures` | 167 | 8 |
| [`higgs_gui_extra`](higgs_gui_extra.md) | Clases de interfaz para tareas especializadas. | `higgs_gui_basics` | 29 | 4 |
| [`higgs_animation`](higgs_animation.md) | Sistema declarativo de animación. | `higgs_gui_basics` | 8 | 0 |
| [`higgs_opengl`](higgs_opengl.md) | Renderizado OpenGL dentro de componentes. | `higgs_gui_extra` | 15 | 1 |
| [`higgs_video`](higgs_video.md) | Reproducción de vídeo (`VideoComponent`) y captura de cámara (`CameraDevice`) en macOS, iOS, Windows y Android. | `higgs_gui_extra` | 2 | 2 |

## Audio

| Módulo | Descripción | Depende de | Clases | Opciones |
|---|---|---|---|---|
| [`higgs_audio_basics`](higgs_audio_basics.md) | Las clases de datos de audio y MIDI. | `higgs_core` | 62 | 0 |
| [`higgs_audio_devices`](higgs_audio_devices.md) | Entrada y salida de audio y MIDI. | `higgs_audio_basics`, `higgs_events` | 28 | 12 |
| [`higgs_audio_formats`](higgs_audio_formats.md) | Lectura y escritura de archivos de audio. | `higgs_audio_basics` | 22 | 8 |
| [`higgs_audio_utils`](higgs_audio_utils.md) | Utilidades de audio con interfaz y misceláneas. | `higgs_audio_processors`, `higgs_audio_formats`, `higgs_audio_devices` | 13 | 2 |
| [`higgs_dsp`](higgs_dsp.md) | Procesamiento digital de señal. | `higgs_audio_formats` | 54 | 5 |
| [`higgs_midi_ci`](higgs_midi_ci.md) | Implementación de MIDI Capability Inquiry (MIDI-CI). | `higgs_audio_basics` | 81 | 0 |

## Plug-ins

| Módulo | Descripción | Depende de | Clases | Opciones |
|---|---|---|---|---|
| [`higgs_audio_processors_headless`](higgs_audio_processors_headless.md) | La parte sin interfaz del sistema de procesadores. | `higgs_audio_basics`, `higgs_events` | 45 | 7 |
| [`higgs_audio_processors`](higgs_audio_processors.md) | Amplía `higgs_audio_processors_headless` con la parte de interfaz. | `higgs_gui_extra`, `higgs_audio_processors_headless` | 22 | 0 |
| [`higgs_audio_plugin_client`](higgs_audio_plugin_client.md) | Los *wrappers* que convierten un `AudioProcessor` en un plug-in de cada formato. | `higgs_audio_processors` | 0 | 7 |

## Otros

| Módulo | Descripción | Depende de | Clases | Opciones |
|---|---|---|---|---|
| [`higgs_osc`](higgs_osc.md) | Implementación de Open Sound Control. | `higgs_events` | 13 | 0 |
| [`higgs_javascript`](higgs_javascript.md) | Un intérprete de JavaScript (basado en QuickJS a través de CHOC) para ejecutar scripts y exponerles objetos nativos. | `higgs_core` | 3 | 0 |
| [`higgs_box2d`](higgs_box2d.md) | El motor de física 2D Box2D y un renderizador que dibuja sus mundos con `Graphics`. | `higgs_graphics` | 1 | 0 |
| [`higgs_analytics`](higgs_analytics.md) | Recopilación de analítica de uso y envío por lotes a destinos configurables desde un hilo de fondo. | `higgs_gui_basics` | 4 | 0 |
| [`higgs_product_unlocking`](higgs_product_unlocking.md) | Compras integradas en las tiendas de Apple y Google, y un sistema de licencias con desbloqueo en línea y archivos de clave firmados. | `higgs_cryptography` | 5 | 0 |
