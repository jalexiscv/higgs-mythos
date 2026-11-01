# Higgs Mythos — identidad del framework

Este documento fija la identidad y las convenciones de nombres de Higgs Mythos.
Cualquier código nuevo debe seguirlas.

## Nombre

- Nombre del producto: **Higgs Mythos**.
- Abreviatura usada en código y documentación técnica: **HIGGS** (siempre en
  mayúsculas cuando forma parte de un identificador).

## Convenciones de nombres

| Superficie | Convención | Ejemplo |
|---|---|---|
| Namespace C++ | `higgs` | `higgs::Component` |
| Macros de configuración | `HIGGS_` | `HIGGS_MAJOR_VERSION`, `HIGGS_UNIT_TESTS` |
| Módulos | `modules/higgs_<área>` | `modules/higgs_gui_basics` |
| Cabecera pública del módulo | `higgs_<área>/higgs_<área>.h` | `#include <higgs_core/higgs_core.h>` |
| Ficheros de implementación | `higgs_<nombre>.cpp` | `higgs_String.cpp` |
| Funciones CMake | `higgs_` | `higgs_add_gui_app`, `higgs_add_plugin` |
| Targets CMake | `higgs::higgs_<área>` | `higgs::higgs_core` |
| Herramienta de proyectos | **Mythos Studio** (`extras/MythosStudio`) | `MythosStudio.exe` |
| Ficheros de proyecto | extensión `.mythos` | `MiApp.mythos` |
| CLI de construcción | `higgsaide` | `higgsaide --resgen` |
| Icono | monograma **HM** | `higgs_icon.png`, `higgsicon.svg` |

El prefijo `HIGGS_API` marca los símbolos exportados de cada módulo. Las
constantes de versión viven en
`modules/higgs_core/system/higgs_StandardHeader.h`.

## Licencia y atribución

- Licencia del framework: **AGPL-3.0-or-later** (ver `LICENSE.md`).
- Los avisos de copyright originales se conservan en la cabecera de cada
  fichero; no se eliminan ni se alteran.
- Los ejemplos están bajo licencia ISC.
- El inventario de dependencias de terceros está en
  `HiggsMythos.spdx.json`.

## Procedencia

Higgs Mythos deriva de una base de código abierto anterior. Por decisión del
autor (2026-11-02) la marca anterior se retiró por completo: nombre del
producto, módulos, macros, namespace, herramienta de proyectos, extensión de
proyecto, recursos y arte. La atribución legal —los titulares de copyright— se
conserva en cada fichero, y la licencia del conjunto pasa a ser AGPL-3.0-or-later.

La sustitución se verificó con una búsqueda insensible a mayúsculas del nombre
retirado sobre el contenido y las rutas del árbol completo, con cero resultados.

## Compilación

```bash
cmake . -B cmake-build -DHIGGS_BUILD_EXAMPLES=ON -DHIGGS_BUILD_EXTRAS=ON
cmake --build cmake-build --target DemoRunner
```

En Windows hay que cargar el entorno de Visual Studio (`vcvars64.bat`) antes de
usar Ninja o MSBuild. Los proyectos de Visual Studio, Xcode y Linux Makefile de
Mythos Studio están en `extras/MythosStudio/Builds`.
