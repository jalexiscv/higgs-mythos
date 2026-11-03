# Dependencias de terceros

Higgs Mythos incluye en el repositorio el código de terceros que necesitan sus módulos, ejemplos y
herramientas, de modo que no hay que descargar nada aparte para compilarlo (salvo los SDK opcionales
de VST2 y ARA). El inventario completo y formal es el documento SPDX
[`HiggsMythos.spdx.json`](https://github.com/jalexiscv/higgs-mythos/blob/main/HiggsMythos.spdx.json),
que se genera desde el manifiesto `extras/SBOM/dependencies.json` (véase [SBOM](../herramientas/otras.md#sbom)).

!!! warning "Revisa las licencias de lo que usas"
    Tu aplicación debe cumplir la licencia de cada componente que incluya. Algunos solo se compilan
    si activas la opción correspondiente: por ejemplo, el SDK de ASIO (`HIGGS_ASIO`) o el SDK de AAX
    (formato `AAX`) tienen licencias propias de Steinberg y Avid, con alternativa GPL-3.0.

| Componente | Versión | Licencia (SPDX) | Usado por | Ubicación |
|---|---|---|---|---|
| [zlib](https://zlib.net) | 1.3.2 | `Zlib` | [`higgs_core`](../modulos/higgs_core.md) | `modules/higgs_core/zip/zlib` |
| [HarfBuzz](https://harfbuzz.github.io) | 15.0.0 | `MIT-Modern-Variant` | [`higgs_graphics`](../modulos/higgs_graphics.md) | `modules/higgs_graphics/fonts/harfbuzz` |
| [SheenBidi](https://github.com/Tehreer/SheenBidi) | 2.9.0 | `Apache-2.0` | [`higgs_graphics`](../modulos/higgs_graphics.md) | `modules/higgs_graphics/unicode/sheenbidi` |
| [LunaSVG](https://github.com/sammycage/lunasvg) | 3.5.0 | `MIT` | [`higgs_graphics`](../modulos/higgs_graphics.md) | `modules/higgs_graphics/drawables/lunasvg` |
| [PlutoVG](https://github.com/sammycage/plutovg) | 1.3.2 | `MIT` | [`higgs_graphics`](../modulos/higgs_graphics.md) | `modules/higgs_graphics/drawables/lunasvg/plutovg` |
| [libpng](http://www.libpng.org/pub/png/libpng.html) | 1.6.58 | `libpng-2.0` | [`higgs_graphics`](../modulos/higgs_graphics.md) | `modules/higgs_graphics/image_formats/pnglib` |
| [jpeglib](https://ijg.org) | 10.0 | `IJG` | [`higgs_graphics`](../modulos/higgs_graphics.md) | `modules/higgs_graphics/image_formats/jpglib` |
| [libwebp](https://developers.google.com/speed/webp) | 1.6.0 | `BSD-3-Clause` | [`higgs_graphics`](../modulos/higgs_graphics.md) | `modules/higgs_graphics/image_formats/libwebp` |
| [Chromium cubic_bezier](https://chromium.googlesource.com/chromium/src) | 3e1a26c44c024d97dc9a4c09bbc6a2365398ca2c | `BSD-3-Clause` | [`higgs_animation`](../modulos/higgs_animation.md) | `modules/higgs_animation/detail/chromium` |
| [FLAC](https://xiph.org/flac) | 1.5.0 | `BSD-3-Clause` | [`higgs_audio_formats`](../modulos/higgs_audio_formats.md) | `modules/higgs_audio_formats/codecs/flac` |
| [libogg](https://xiph.org/ogg) | 1.3.6 | `BSD-3-Clause` | [`higgs_audio_formats`](../modulos/higgs_audio_formats.md) | `modules/higgs_audio_formats/codecs/ogg` |
| [libvorbis](https://xiph.org/vorbis) | 1.3.7 | `BSD-3-Clause` | [`higgs_audio_formats`](../modulos/higgs_audio_formats.md) | `modules/higgs_audio_formats/codecs/vorbis` |
| [Opus](https://opus-codec.org) | 1.6.1 | `BSD-3-Clause` | [`higgs_audio_formats`](../modulos/higgs_audio_formats.md) | `modules/higgs_audio_formats/codecs/opus/opus` |
| [opusfile](https://opus-codec.org) | 0.12-59-g6dfd29e | `BSD-3-Clause` | [`higgs_audio_formats`](../modulos/higgs_audio_formats.md) | `modules/higgs_audio_formats/codecs/opus/opusfile` |
| [libopusenc](https://opus-codec.org) | 0.3 | `BSD-3-Clause` | [`higgs_audio_formats`](../modulos/higgs_audio_formats.md) | `modules/higgs_audio_formats/codecs/opus/libopusenc` |
| [Oboe](https://github.com/google/oboe) | 1.10.0 | `Apache-2.0` | [`higgs_audio_devices`](../modulos/higgs_audio_devices.md) | `modules/higgs_audio_devices/native/oboe` |
| [ASIO SDK](https://www.steinberg.net/developers) | 2.3 | `LicenseRef-Steinberg-ASIO OR GPL-3.0-only` | [`higgs_audio_devices`](../modulos/higgs_audio_devices.md) | `modules/higgs_audio_devices/native/asio` |
| [VST3 SDK](https://github.com/steinbergmedia/vst3sdk) | 3.8.0 | `MIT` | [`higgs_audio_processors_headless`](../modulos/higgs_audio_processors_headless.md) | `modules/higgs_audio_processors_headless/format_types/VST3_SDK` |
| [LV2](https://lv2plug.in) | 1.18.0 | `ISC` | [`higgs_audio_processors_headless`](../modulos/higgs_audio_processors_headless.md) | `modules/higgs_audio_processors_headless/format_types/LV2_SDK/lv2` |
| [lilv](https://drobilla.net/software/lilv.html) | 0.24.12 | `ISC` | [`higgs_audio_processors_headless`](../modulos/higgs_audio_processors_headless.md) | `modules/higgs_audio_processors_headless/format_types/LV2_SDK/lilv` |
| [serd](https://drobilla.net/software/serd.html) | 0.30.10 | `ISC` | [`higgs_audio_processors_headless`](../modulos/higgs_audio_processors_headless.md) | `modules/higgs_audio_processors_headless/format_types/LV2_SDK/serd` |
| [sord](https://drobilla.net/software/sord.html) | 0.16.9 | `ISC` | [`higgs_audio_processors_headless`](../modulos/higgs_audio_processors_headless.md) | `modules/higgs_audio_processors_headless/format_types/LV2_SDK/sord` |
| [sratom](https://drobilla.net/software/sratom.html) | 0.6.8 | `ISC` | [`higgs_audio_processors_headless`](../modulos/higgs_audio_processors_headless.md) | `modules/higgs_audio_processors_headless/format_types/LV2_SDK/sratom` |
| pslextensions | — | `LicenseRef-Public-Domain` | [`higgs_audio_processors_headless`](../modulos/higgs_audio_processors_headless.md) | `modules/higgs_audio_processors_headless/format_types/pslextensions` |
| [AAX SDK](https://developer.avid.com/aax) | 2.9 | `LicenseRef-Avid-AAX OR GPL-3.0-only` | [`higgs_audio_plugin_client`](../modulos/higgs_audio_plugin_client.md) | `modules/higgs_audio_plugin_client/AAX/SDK` |
| [AudioUnitSDK](https://github.com/apple/AudioUnitSDK) | 1.1.0 | `Apache-2.0` | [`higgs_audio_plugin_client`](../modulos/higgs_audio_plugin_client.md) | `modules/higgs_audio_plugin_client/AU/AudioUnitSDK` |
| [Box2D](https://box2d.org) | 2.2.1 | `Zlib` | [`higgs_box2d`](../modulos/higgs_box2d.md) | `modules/higgs_box2d/box2d` |
| [CHOC](https://github.com/Tracktion/choc) | 123d322c7ed4de8f0db2c30edea07b248dfe14c4 | `ISC` | [`higgs_javascript`](../modulos/higgs_javascript.md) | `modules/higgs_javascript/choc` |
| [QuickJS](https://bellard.org/quickjs) | 2021-03-27 | `MIT` | [`higgs_javascript`](../modulos/higgs_javascript.md) | `modules/higgs_javascript/choc/javascript/choc_javascript_QuickJS.h` |
| [OpenGL registry headers](https://registry.khronos.org/OpenGL) | 2024-01-16 | `BSD-3-Clause AND MIT` | [`higgs_opengl`](../modulos/higgs_opengl.md) | `modules/higgs_opengl/opengl/higgs_gl.h` |
| [@jalexiscv/webview](https://github.com/jalexiscv/higgs-mythos) | 1.0.0 | `AGPL-3.0-or-later` | [`higgs_gui_extra`](../modulos/higgs_gui_extra.md) | `modules/higgs_gui_extra/native/typescript/webview-interop` |
| [reaper-sdk](https://github.com/justinfrankel/reaper-sdk) | ec0c1dc1655eb6a192fa7a22c2258a38a5ebbe9c | `Zlib` | `examples` | `examples/Plugins/extern` |
| [Box2D testbed](https://box2d.org) | 2.2.1 | `Zlib` | `examples` | `examples/Assets/Box2DTests` |
| [MythosStudio icons](https://raphaeljs.com/icons) | — | `MIT` | `extras` | `extras/MythosStudio/Source/Utility/UI/mythos_Icons.cpp` |
| [Gradle wrapper](https://gradle.org) | 9.5.1 | `Apache-2.0` | `build-system` | `examples/DemoRunner/Builds/Android/gradle/wrapper` |

## SDK externos no incluidos

| SDK | Uso | Cómo configurarlo |
|---|---|---|
| VST2 (heredado) | Compilar plug-ins VST2 y alojarlos | `higgs_set_vst2_sdk_path`, `HIGGS_GLOBAL_VST2_SDK_PATH` |
| ARA 2.3.0 | Plug-ins y hosts con ARA | `higgs_set_ara_sdk_path`, `HIGGS_GLOBAL_ARA_SDK_PATH` (véase [ARA](../guias/ara.md)) |
| Windows MIDI Services | MIDI 2.0 en Windows | `NEEDS_WINDOWS_MIDI_SERVICES TRUE` (CMake indica cómo instalar los paquetes) |
| WebView2 | Navegador Chromium en Windows | Paquete NuGet `Microsoft.Web.WebView2`; `NEEDS_WEBVIEW2 TRUE` |
| Intel MKL / FFTW | FFT y convolución aceleradas | `HIGGS_DSP_USE_INTEL_MKL`, `HIGGS_DSP_USE_SHARED_FFTW`, `HIGGS_DSP_USE_STATIC_FFTW` |
