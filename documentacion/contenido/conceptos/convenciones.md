# Convenciones de nombres

Higgs Mythos tiene una identidad de nombres coherente en todas sus superficies. Todo código nuevo
—del framework o de un módulo propio que quiera integrarse con naturalidad— debe seguirla.

## Nombre del producto

- Nombre del producto: **Higgs Mythos**.
- Abreviatura en el código y en la documentación técnica: **HIGGS**, siempre en mayúsculas cuando
  forma parte de un identificador de macro.

## Tabla de convenciones

| Superficie | Convención | Ejemplo |
|---|---|---|
| Espacio de nombres C++ | `higgs` | `higgs::Component` |
| Macros de configuración | prefijo `HIGGS_` | `HIGGS_MAJOR_VERSION`, `HIGGS_UNIT_TESTS` |
| Módulos | `modules/higgs_<área>` | `modules/higgs_gui_basics` |
| Cabecera pública del módulo | `higgs_<área>/higgs_<área>.h` | `#include <higgs_core/higgs_core.h>` |
| Archivos de implementación | `higgs_<Nombre>.cpp` / `.h` | `higgs_String.cpp` |
| Funciones de CMake | prefijo `higgs_` | `higgs_add_gui_app`, `higgs_add_plugin` |
| Targets de CMake | `higgs::higgs_<área>` | `higgs::higgs_core` |
| Herramienta de proyectos | **Mythos Studio** (`extras/MythosStudio`) | `MythosStudio.exe` |
| Archivos de proyecto | extensión `.mythos` | `MiApp.mythos` |
| CLI de compilación | `higgsaide` | `higgsaide header …` |
| Icono | monograma **HM** | `higgs_icon.png` |
| Símbolos exportados | macro `HIGGS_API` | `class HIGGS_API String` |
| Macros de plug-in generadas | prefijo `HiggsPlugin_` | `HiggsPlugin_Name`, `HiggsPlugin_IsSynth` |

## Versión del framework

Las constantes de versión están en `modules/higgs_core/system/higgs_StandardHeader.h`:

```cpp
#define HIGGS_MAJOR_VERSION      9
#define HIGGS_MINOR_VERSION      0
#define HIGGS_BUILDNUMBER        5
```

Puedes comprobarla en tiempo de compilación con estas macros, o en tiempo de ejecución con
`higgs::SystemStats::getHIGGSVersion()`.

## Macros de plataforma

`higgs_core/system/higgs_TargetPlatform.h` define una macro por plataforma, con valor 1 en la
plataforma actual:

| Macro | Plataforma |
|---|---|
| `HIGGS_MAC` | macOS |
| `HIGGS_IOS` | iOS |
| `HIGGS_WINDOWS` | Windows |
| `HIGGS_LINUX` | Linux |
| `HIGGS_BSD` | FreeBSD y derivados |
| `HIGGS_ANDROID` | Android |
| `HIGGS_WASM` | WebAssembly |

Y otras de utilidad como `HIGGS_DEBUG` (compilación de depuración), `HIGGS_64BIT`, `HIGGS_ARM`,
`HIGGS_INTEL`, `HIGGS_GCC`, `HIGGS_CLANG` o `HIGGS_MSVC`.

## Convenciones de código

El estilo del código del framework, que verás en todos los módulos y ejemplos:

- Clases en `PascalCase`, funciones y variables en `camelCase`, constantes y macros en
  `MAYÚSCULAS`.
- Un espacio antes del paréntesis en llamadas y declaraciones: `setSize (600, 400);`.
- Clases no copiables con `HIGGS_DECLARE_NON_COPYABLE` o, mejor,
  `HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MiClase)` al final de la declaración.
- Separadores de sección con líneas `//==============================================================================`.
- Comentarios Doxygen (`/** … */`) sobre cada clase y método público; de ahí se genera la
  referencia del API.
- Código, comentarios y mensajes de commit **en inglés** dentro del framework (véase
  [Contribuir](../proyecto/contribuir.md)).

## Licencia y atribución

- Licencia del framework: **AGPL-3.0-or-later** (`LICENSE.md`).
- Los avisos de copyright originales se conservan en la cabecera de cada archivo; no se eliminan
  ni se alteran.
- Los ejemplos están bajo licencia ISC.
- El inventario de dependencias de terceros está en `HiggsMythos.spdx.json`.

Más detalles en [Licencia](../proyecto/licencia.md).
