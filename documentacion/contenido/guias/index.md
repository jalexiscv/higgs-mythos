# Guías

Guías prácticas organizadas por área. Cada una explica las clases principales, muestra código de
ejemplo y remite a las demos del repositorio donde verlas en funcionamiento.

| Guía | Módulos implicados |
|---|---|
| [Interfaz gráfica](interfaz-grafica.md) | `higgs_gui_basics`, `higgs_gui_extra` |
| [Gráficos 2D, fuentes e imágenes](graficos.md) | `higgs_graphics` |
| [Audio y dispositivos](audio.md) | `higgs_audio_basics`, `higgs_audio_devices`, `higgs_audio_formats`, `higgs_audio_utils` |
| [MIDI, MPE y MIDI 2.0](midi.md) | `higgs_audio_basics`, `higgs_audio_devices`, `higgs_midi_ci` |
| [Procesamiento DSP](dsp.md) | `higgs_dsp` |
| [Plug-ins de audio](plugins.md) | `higgs_audio_processors*`, `higgs_audio_plugin_client` |
| [Alojar plug-ins (hosting)](hosting.md) | `higgs_audio_processors*` |
| [ARA](ara.md) | `higgs_audio_processors*`, `higgs_audio_plugin_client` |
| [Modelo de datos y deshacer](datos.md) | `higgs_data_structures` |
| [Archivos, red y serialización](archivos-red.md) | `higgs_core` |
| [Interfaces web (WebView)](web.md) | `higgs_gui_extra` |
| [OpenGL](opengl.md) | `higgs_opengl` |
| [Animación](animacion.md) | `higgs_animation` |
| [Vídeo y cámara](video.md) | `higgs_video` |
| [Accesibilidad](accesibilidad.md) | `higgs_gui_basics` |
| [Localización](localizacion.md) | `higgs_core` |
| [Pruebas unitarias](pruebas.md) | `higgs_core` |
| [Otros módulos](otros-modulos.md) | `higgs_osc`, `higgs_javascript`, `higgs_cryptography`, `higgs_box2d`, `higgs_analytics`, `higgs_product_unlocking` |

En los ejemplos de código de estas guías se escribe el prefijo `higgs::` de forma explícita. En
tus propios archivos puedes añadir `using namespace higgs;` si lo prefieres (la cabecera generada
`HiggsHeader.h` lo hace por defecto, salvo que definas `DONT_SET_USING_HIGGS_NAMESPACE`).
