# Formato de módulos

Un módulo de Higgs Mythos es un conjunto de archivos de cabecera y de código fuente que se puede
añadir a un proyecto para proporcionar un conjunto de clases y bibliotecas o una funcionalidad
relacionada. Su estructura está pensada para que añadirlo a un proyecto sea lo más sencillo
posible en cualquier plataforma, ya sea con herramientas automáticas o a mano.

Un módulo puede depender de otros módulos, pero por lo demás debe ser autocontenido. El original
en inglés está en `docs/Higgs Mythos Module Format.md`.

## Estructura de archivos

Cada módulo vive en una carpeta con el mismo nombre que el módulo. La convención de Higgs Mythos es
usar minúsculas con guiones bajos:

```text
higgs_core
higgs_events
higgs_graphics
```

Aunque cualquier nombre que sea un identificador C++ válido es aceptable.

En la raíz de la carpeta debe haber un conjunto de cabeceras públicas y archivos fuente que el
proyecto del usuario incluirá. El módulo puede tener tantos archivos internos como necesite, pero
**todos deben estar en subcarpetas**.

### Cabecera principal

En la carpeta raíz debe haber **una** cabecera principal que incluya todas las cabeceras
necesarias del módulo. Debe tener el mismo nombre que el módulo, con sufijo `.h`, `.hpp` o `.hxx`:

```text
higgs_core/higgs_core.h
```

!!! danger "Rutas relativas"
    Todo el código de un módulo que incluya otros archivos de sus propias subcarpetas debe hacerlo
    con rutas **relativas**. Un módulo debe poder reubicarse en disco por completo y no puede
    depender de que el proyecto del usuario tenga rutas de inclusión configuradas. Aunque el usuario
    no tenga ninguna ruta de inclusión e incluya la cabecera principal con una ruta absoluta, el
    módulo debe encontrar correctamente todos sus archivos internos.

La cabecera principal debe contener además un comentario con un bloque
`BEGIN_HIGGS_MODULE_DECLARATION` que define los requisitos del módulo (su sintaxis se describe más
abajo).

### Archivos `.cpp` del módulo

Un módulo consta de una cabecera y cero o más archivos `.cpp`. ¡Cuantos menos, mejor!

Lo ideal sería un módulo solo de cabeceras, que un proyecto pueda usar con solo incluir la
cabecera principal. Por diversas razones suele ser necesario o preferible tener una cabecera más
sencilla y algunos `.cpp` que el proyecto compile como unidades independientes. En ese caso,
idealmente debe haber un único `.cpp` en la raíz del módulo que incluya internamente todos los
demás `.cpp` de sus subcarpetas, de modo que baste con añadir un archivo al proyecto para compilar
el módulo completo.

En algunos casos (por ejemplo, si el módulo depende de código de terceros que no se puede combinar
fácilmente en una sola unidad de compilación) puede haber más de un archivo fuente, pero conviene
evitarlo porque complica la vida a quien añade los archivos a mano.

Los nombres de estos archivos deben empezar por el nombre del módulo, aunque pueden llevar un
número u otro sufijo si hay varios.

Para que un archivo solo se compile en una plataforma concreta, añade al nombre uno de estos
sufijos (sin distinguir mayúsculas):

| Sufijo | Se compila en |
|---|---|
| `_mac` u `_osx` | macOS |
| `_windows` | Windows |
| `_linux` | Linux y FreeBSD |
| `_android` | Android |
| `_ios` | iOS |

Por ejemplo:

```text
higgs_mimodulo/higgs_mimodulo_1.cpp         <- todas las plataformas
higgs_mimodulo/higgs_mimodulo_2.cpp         <- todas las plataformas
higgs_mimodulo/higgs_mimodulo_mac.cpp       <- solo macOS
higgs_mimodulo/higgs_mimodulo_windows.cpp   <- solo Windows
```

A menudo no hace falta, porque se puede comprobar la plataforma dentro de los archivos, pero
ayuda a no saturar los proyectos con archivos innecesarios.

Para simplificar el uso de Objective-C++ hay una regla especial: si la carpeta contiene un `.mm` y
un `.cpp` con el mismo nombre, en macOS/iOS se usa el `.mm` y se ignora el `.cpp` (y al revés en
las demás plataformas).

### Bibliotecas precompiladas

Se pueden incluir bibliotecas precompiladas en una subcarpeta `libs/`. Los siguientes directorios
se añaden automáticamente a las rutas de búsqueda de bibliotecas, y las bibliotecas que contienen
se enlazan con las claves `OSXLibs`, `iOSLibs`, `windowsLibs` y `linuxLibs` de la declaración del
módulo:

| Plataforma | Directorio |
|---|---|
| macOS | `libs/MacOSX`: para varias arquitecturas, coloca aquí bibliotecas universales. Por compatibilidad, Mythos Studio incluye también `libs/MacOSX/{arch}` (por ejemplo `x86_64`); con CMake solo se admiten bibliotecas universales y se ignoran esas subcarpetas. |
| Visual Studio | `libs/VisualStudio{año}/{arch}/{runtime}`, donde `{año}` es el año de la versión de Visual Studio, `{arch}` la arquitectura (`x64`, `Win32`…) y `{runtime}` el tipo de biblioteca de *runtime* (`MD`, `MDd`, `MT`, `MTd`). |
| Linux | `libs/Linux/{arch}`, donde `{arch}` es la arquitectura de destino (`x86_64`, `i386`, `armv6`…). |
| iOS | `libs/iOS`: bibliotecas universales. Mythos Studio incluye también `libs/iOS/{arch}` (`arm64`, `x86_64`); con CMake solo se admiten bibliotecas universales. |
| Android | `libs/Android/{arch}`, donde `{arch}` es la ABI que proporciona Android Studio en `${ANDROID_ABI}` (`x86`, `armeabi-v7a`…). |

## El bloque `BEGIN_HIGGS_MODULE_DECLARATION`

Este bloque de texto va dentro de la cabecera principal del módulo. Debe estar comentado (y
quizá dentro de un `#if 0`), aunque las herramientas simplemente buscan en todo el archivo la
cadena `BEGIN_HIGGS_MODULE_DECLARATION` sin importar su contexto sintáctico en C++.

El bloque termina con `END_HIGGS_MODULE_DECLARATION`. Ambas marcas deben estar en una línea propia.
Dentro, el analizador espera una lista de definiciones, una por línea, con la sintaxis:

```text
nombre_del_valor:   valor
```

El nombre distingue mayúsculas y debe ser uno de los de la tabla. Se ignoran los espacios. Algunos
valores son obligatorios; el orden no importa.

| Clave | Obligatoria | Descripción |
|---|---|---|
| `ID` | Sí | Debe coincidir con el nombre del archivo y de la carpeta, por ejemplo `higgs_core`. Sirve sobre todo como comprobación. |
| `vendor` | Sí | Identificador único del proveedor, por ejemplo `higgs`. Corto y sin espacios. |
| `version` | Sí | Número de versión del módulo. |
| `name` | Sí | Descripción corta del módulo. |
| `description` | Sí | Descripción más larga (¡pero en una sola línea!). |
| `dependencies` | No | Lista (separada por espacios o comas) de módulos que necesita. Las herramientas la usan para resolver dependencias automáticamente. |
| `website` | No | URL con información útil sobre el módulo. |
| `license` | No | Tipo de licencia que se aplica. |
| `minimumCppStandard` | No | Estándar mínimo de C++ requerido, solo el número (por ejemplo `17` para C++17). |
| `searchpaths` | No | Rutas de inclusión internas, separadas por espacios y relativas a la carpeta padre del módulo, que deben añadirse a las rutas de búsqueda de cabeceras del proyecto. |
| `OSXFrameworks` | No | Frameworks de macOS necesarios. |
| `WeakOSXFrameworks` | No | Frameworks de macOS enlazados de forma débil. |
| `iOSFrameworks` | No | Frameworks de iOS necesarios. |
| `WeakiOSFrameworks` | No | Frameworks de iOS enlazados de forma débil. |
| `linuxPackages` | No | Paquetes de pkg-config que aportan opciones de compilación (CFLAGS) y enlace (LDFLAGS). |
| `linuxLibs` | No | Bibliotecas estáticas o dinámicas que se enlazan en Linux (con `-l`). |
| `OSXLibs` | No | Bibliotecas que se enlazan en macOS (con `-l`). |
| `iOSLibs` | No | Bibliotecas que se enlazan en iOS (con `-l`). |
| `windowsLibs` | No | Bibliotecas que se enlazan con Visual Studio (sin el sufijo `.lib`). |

Ejemplo:

```text
BEGIN_HIGGS_MODULE_DECLARATION

 ID:               higgs_audio_devices
 vendor:           higgs
 version:          9.0.5
 name:             Higgs Mythos audio and MIDI I/O device classes
 description:      Classes to play and record from audio and MIDI I/O devices
 website:          https://github.com/jalexiscv/higgs-mythos
 license:          AGPLv3
 minimumCppStandard: 17

 dependencies:     higgs_audio_basics, higgs_events
 OSXFrameworks:    CoreAudio CoreMIDI AudioToolbox
 iOSFrameworks:    CoreAudio CoreMIDI AudioToolbox AVFoundation
 linuxPackages:    alsa

END_HIGGS_MODULE_DECLARATION
```

## Opciones de configuración del módulo

Las opciones que el usuario puede cambiar se declaran en la cabecera principal con un comentario
`Config:` seguido de un `#ifndef`, de modo que Mythos Studio las muestre en su interfaz y desde CMake
se puedan cambiar con `target_compile_definitions`:

```cpp
/** Config: HIGGS_MIMODULO_USAR_SIMD
    Activa las rutas de código vectorizadas del módulo.
*/
#ifndef HIGGS_MIMODULO_USAR_SIMD
 #define HIGGS_MIMODULO_USAR_SIMD 1
#endif
```
