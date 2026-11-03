# Requisitos del sistema

## Para compilar proyectos con Higgs Mythos

| Elemento | Requisito mínimo |
|---|---|
| Estándar de C++ | **C++17** (C++20 si se activa `HIGGS_USE_WINDOWS_MIDI_SERVICES`) |
| CMake | **3.22** o superior (para cualquier tipo de proyecto) |
| macOS / iOS | **Xcode 12.4** (macOS Intel 10.15.4; Apple Silicon macOS 11.0) |
| Windows | **Visual Studio 2019** (Windows 10 versión 1607) |
| Linux | **g++ 7.0** o **Clang 6.0**, más los paquetes de desarrollo indicados en [Dependencias en Linux](../compilacion/linux.md) |
| Android | **Android Studio Narwhal 3 (2025.1.3)** con Android Gradle plugin 8.13.2 y NDK 28, en Windows, macOS o Linux |

Todos los módulos declaran `minimumCppStandard: 17` en su bloque de declaración.

!!! note "CMake y Android"
    Con el API de CMake **no se pueden generar actualmente proyectos para Android**. Para Android
    utiliza el exportador de Android Studio de [Mythos Studio](../herramientas/mythos-studio.md).

!!! tip "Usa un CMake reciente"
    La mayoría de gestores de paquetes ofrecen CMake, pero conviene usar la versión más reciente
    publicada en <https://cmake.org/download>. CMake debe ser siempre más nuevo que tu cadena de
    herramientas, para que sepa identificar los compiladores y cómo invocarlos.

Además de CMake necesitas la cadena de compilación de tu plataforma: Xcode en macOS, MSVC (Visual
Studio) en Windows, o GCC/Clang en Linux.

## Plataformas de despliegue

Las aplicaciones y plug-ins compilados con Higgs Mythos pueden ejecutarse en:

| Plataforma | Versión mínima | Arquitecturas |
|---|---|---|
| macOS | macOS 10.11 | x86_64, Arm64 |
| Windows | Windows 10 versión 1607 | x86_64, x86, Arm64, Arm64EC |
| Linux | Distribuciones de uso general | x86_64, Arm64/aarch64 |
| iOS | iOS 12 | Arm64, Arm64e, simulador x86_64 |
| Android | Android 7 (Nougat, API 24) | arm64-v8a, armeabi-v7a, x86_64, x86 |

## SDK de terceros opcionales

Algunas funciones dependen de SDK externos que debes obtener aparte y registrar con las funciones
`higgs_set_<tipo>_sdk_path` (véase el [API de CMake](../compilacion/cmake-api.md#higgs_set_kind_sdk_path)):

| SDK | Para qué se usa | Función CMake |
|---|---|---|
| VST2 (heredado) | Compilar plug-ins VST2 o alojarlos (`HIGGS_PLUGINHOST_VST`) | `higgs_set_vst2_sdk_path` |
| VST3 | Incluido en el repositorio; solo si quieres usar una copia propia (`HIGGS_CUSTOM_VST3_SDK`) | `higgs_set_vst3_sdk_path` |
| AAX | Plug-ins para Pro Tools | `higgs_set_aax_sdk_path` |
| ARA 2.3.0 | Plug-ins y hosts con extensiones ARA | `higgs_set_ara_sdk_path` |

!!! warning "Firmas AAX"
    Los plug-ins AAX deben firmarse digitalmente con las herramientas de PACE Anti-Piracy antes de
    poder ejecutarse en las versiones comerciales de Pro Tools. Consulta la sección
    [AAX en la guía de plug-ins](../guias/plugins.md#formato-aax-y-firma-digital).
