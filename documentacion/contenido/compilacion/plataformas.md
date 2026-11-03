# Plataformas

Higgs Mythos genera binarios nativos para cada plataforma. Esta página reúne lo específico de cada
una; los requisitos mínimos están en [Requisitos del sistema](../primeros-pasos/requisitos.md).

## macOS

- Generadores: **Xcode** (recomendado para bundles y firma), Ninja o Makefiles.
- **Binarios universales**: `-DCMAKE_OSX_ARCHITECTURES="arm64;x86_64"`.
- **Objetivo de despliegue**: `-DCMAKE_OSX_DEPLOYMENT_TARGET=10.11` (o superior). En 9.0.5 cambiaron
  los objetivos por defecto que establece Mythos Studio; véase
  [Cambios incompatibles](../proyecto/cambios-incompatibles.md).
- **Notarización**: activa `HARDENED_RUNTIME_ENABLED TRUE` y añade en
  `HARDENED_RUNTIME_OPTIONS` los *entitlements* necesarios (por ejemplo
  `com.apple.security.device.audio-input` si grabas audio).
- **Sandbox** (App Store): `APP_SANDBOX_ENABLED TRUE` y las opciones `APP_SANDBOX_*`.
- **Iconos**: `ICON_BIG`/`ICON_SMALL` o, con Xcode 26 o posterior, `ICON_COMPOSER_BUNDLE`.
- Audio: CoreAudio; MIDI: CoreMIDI; plug-ins: AU, AUv3, VST3, AAX, LV2.
- Gráficos: CoreGraphics; `HIGGS_USE_COREIMAGE_LOADER` usa CoreImage para cargar PNG/JPEG/GIF;
  `HIGGS_DISABLE_COREGRAPHICS_FONT_SMOOTHING` desactiva el suavizado de fuentes.

## iOS

- Usa el generador **Xcode** con `-DCMAKE_SYSTEM_NAME=iOS -DCMAKE_OSX_DEPLOYMENT_TARGET=12.0`
  (detalles en el [API de CMake](cmake-api.md#compilar-para-ios)).
- Para dispositivos reales, indica `CMAKE_XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY` y
  `CMAKE_XCODE_ATTRIBUTE_DEVELOPMENT_TEAM`.
- Plug-ins: **AUv3** (empaquetado en una aplicación contenedora) y Standalone.
- Permisos: `MICROPHONE_PERMISSION_*`, `CAMERA_PERMISSION_*`, `BLUETOOTH_PERMISSION_*`…; audio en
  segundo plano con `BACKGROUND_AUDIO_ENABLED`.
- `HIGGS_DISABLE_AUDIO_MIXING_WITH_OTHER_APPS` da a tu aplicación acceso exclusivo al audio.
- `HIGGS_EXECUTE_APP_SUSPEND_ON_BACKGROUND_TASK` ejecuta `suspended()` en una tarea de segundo
  plano, dando más tiempo para guardar el estado.

## Windows

- Visual Studio 2019 o posterior (Mythos Studio exporta para VS2019, VS2022 y VS2026).
- Arquitecturas: x86_64, x86, Arm64 y Arm64EC.
- Clang: `-T ClangCL` con el generador de Visual Studio, o `clang`/`clang++` estilo GNU (véase el
  [API de CMake](cmake-api.md#compilar-con-clang-en-windows)).
- Antes de usar Ninja o MSBuild desde la consola, carga el entorno de Visual Studio
  (`vcvars64.bat`).
- Audio: WASAPI, DirectSound y ASIO (`HIGGS_ASIO`); MIDI: Win32, WinRT (`HIGGS_USE_WINRT_MIDI`) o
  Windows MIDI Services (`NEEDS_WINDOWS_MIDI_SERVICES`, requiere C++20).
- Gráficos: Direct2D; `HIGGS_WIN_PER_MONITOR_DPI_AWARE` (activado) habilita la escala por monitor.
- WebView2: `NEEDS_WEBVIEW2 TRUE` y el paquete NuGet `Microsoft.Web.WebView2`
  (`HIGGS_WEBVIEW2_PACKAGE_LOCATION` cambia dónde se busca).
- `HIGGS_DONT_AUTOLINK_TO_WIN32_LIBRARIES` evita el enlace automático de las bibliotecas del
  sistema.
- Desde 9.0.0 el multitáctil está **desactivado por defecto** en Windows.
- La carpeta `extras/WindowsDLL` contiene un proyecto para compilar el framework como DLL.

## Linux y BSD

- Compiladores: g++ 7.0+ o Clang 6.0+.
- Instala los paquetes de desarrollo de [Dependencias en Linux](linux.md).
- Audio: ALSA (`HIGGS_ALSA`) y JACK (`HIGGS_JACK`); plug-ins alojables: VST3, LV2, LADSPA.
- Gráficos: X11 con el renderizador por software; extensiones opcionales Xrandr, Xinerama, XShm,
  XRender, XCursor y XInput (multitáctil) controladas por `HIGGS_USE_X*`.
- OpenGL mediante **EGL** (desde 9.0.0) u **OpenGL ES** (`NEEDS_OPENGL_ES TRUE`, desde 9.0.4).
- HTTPS mediante libcurl (`NEEDS_CURL TRUE`); navegador mediante WebKitGTK (`NEEDS_WEB_BROWSER TRUE`).
- Desde 9.0.0, `libxi-dev` es obligatorio salvo que se desactive `HIGGS_USE_XINPUT`.
- Los módulos de vídeo y cámara no están disponibles.

## Android

- **CMake no admite Android**: usa el exportador de **Android Studio** de
  [Mythos Studio](../herramientas/mythos-studio.md).
- Requisitos: Android Studio Narwhal 3 (2025.1.3), Android Gradle plugin 8.13.2 y NDK 28.
- Despliegue: Android 7 (API 24) o superior; ABI arm64-v8a, armeabi-v7a, x86_64 y x86.
- Audio: Oboe (`HIGGS_USE_ANDROID_OBOE`, por defecto) u OpenSL ES (`HIGGS_USE_ANDROID_OPENSLES`).
- En Android, abre dispositivos de cámara y vídeo siempre con los métodos asíncronos.
- El DemoRunner incluye un proyecto de Android ya generado en `examples/DemoRunner/Builds/Android`.

## WebAssembly

El núcleo detecta la plataforma WebAssembly (`HIGGS_WASM`), pero no hay exportadores ni funciones
de CMake específicos para ella.
