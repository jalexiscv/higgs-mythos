# Mythos Studio

**Mythos Studio** es la herramienta de configuración de proyectos de Higgs Mythos. Guarda la
configuración de un proyecto en un archivo `.mythos` y a partir de él **exporta** proyectos nativos
para cada IDE y plataforma. Incluye un editor de código, gestión de módulos y una interfaz de línea
de comandos para automatizar tareas.

## Compilarlo

=== "CMake"

    ```bash
    cmake . -B cmake-build -DHIGGS_BUILD_EXTRAS=ON
    cmake --build cmake-build --target MythosStudio
    ```

=== "Proyectos incluidos"

    Abre el proyecto correspondiente de `extras/MythosStudio/Builds`: `MacOSX` (Xcode),
    `VisualStudio2019`, `VisualStudio2022`, `VisualStudio2026` o `LinuxMakefile`.

## Ejecutables precompilados

El flujo de GitHub Actions `HIGGS Mythos Studio Build` (`.github/workflows/higgs_mythos_studio.yml`)
compila Mythos Studio en modo *Release* para **Windows x64**, **macOS universal** (arm64 + x86_64)
y **Linux x64** cada vez que cambia `extras/MythosStudio` en `main`, o a mano desde la pestaña
*Actions* (*Run workflow*). Los ejecutables se descargan como *artifacts* de la ejecución:
`MythosStudio-Windows-x64` (`MythosStudio.exe`), `MythosStudio-macOS-universal`
(`MythosStudio.app.zip`) y `MythosStudio-Linux-x64` (`MythosStudio`).

!!! note "Binarios sin firmar"
    Los ejecutables no están firmados digitalmente: Windows SmartScreen puede mostrar un aviso
    (*Más información → Ejecutar de todas formas*) y en macOS hay que abrir la aplicación con
    clic derecho → *Abrir* la primera vez.

## Exportadores

| Exportador | Genera |
|---|---|
| Xcode (macOS) | Proyecto de Xcode para macOS |
| Xcode (iOS) | Proyecto de Xcode para iOS |
| Visual Studio 2019 / 2022 / 2026 | Solución de Visual Studio |
| Linux Makefile | `Makefile` para GCC/Clang |
| Android | Proyecto de Android Studio (Gradle) |

Un mismo `.mythos` puede tener varios exportadores; cada uno con sus configuraciones (Debug,
Release…), rutas de SDK, opciones de compilador y enlazador.

!!! tip "Android solo con Mythos Studio"
    El API de CMake no admite Android, así que Mythos Studio es la vía para compilar para Android.
    En 9.0.5 se actualizaron las versiones por defecto de Android Gradle y Java del exportador.

## Crear un proyecto

La página de inicio ofrece plantillas agrupadas por categoría:

| Categoría | Plantilla | Descripción |
|---|---|---|
| Aplicación | **Blank** | Aplicación gráfica vacía |
| Aplicación | **GUI** | Aplicación gráfica con una ventana y un componente |
| Aplicación | **Audio** | Aplicación gráfica con un componente y funciones de entrada/salida de audio y MIDI |
| Aplicación | **Console** | Aplicación de línea de comandos sin interfaz gráfica |
| Aplicación | **Animated** | Aplicación gráfica que dibuja una animación |
| Aplicación | **OpenGL** | Aplicación con un componente que renderiza con OpenGL |
| Plug-in | **Basic** | Plug-in de audio con editor y funciones de audio/MIDI |
| Plug-in | **ARA** | Plug-in de audio con funciones ARA |
| Biblioteca | **Static Library** | Biblioteca estática |
| Biblioteca | **Dynamic Library** | Biblioteca dinámica |

Al crear el proyecto eliges los módulos, los exportadores y qué archivos iniciales se generan
(ninguno, un archivo principal, procesador y editor, archivos ARA…). También puedes abrir un
[PIP](../compilacion/pip.md) para convertirlo en proyecto.

## La ventana del proyecto

- **Archivos**: grupos de archivos fuente, recursos binarios y opciones por archivo (compilar,
  añadir a recursos binarios, etc.).
- **Módulos**: módulos activados, su ruta, si se copian dentro del proyecto y sus **opciones de
  configuración** (las macros `Config:`). Menú contextual con *Add a module* y *Add a module from a
  specified folder...*.
- **Exportadores**: ajustes por IDE y configuración.
- **Ajustes del proyecto**: nombre, versión, empresa, tipo de proyecto, formatos de plug-in (VST3,
  AU, AUv3, AAX, LV2, Unity, Standalone, ARA), características del plug-in, definiciones de
  preprocesador, estándar de C++…
- **Editor de código** integrado con resaltado de sintaxis.

Mythos Studio genera en la carpeta `HiggsLibraryCode/` del proyecto los archivos que incluyen los
módulos (`include_higgs_*.cpp`, `HiggsHeader.h`, datos binarios…). **No edites esa carpeta**: se
borra y se reescribe cada vez que se guarda el proyecto.

### Rutas globales

En **MythosStudio → Global Paths...** (macOS) o **File → Global Paths...** (Windows/Linux) se
configuran, por sistema operativo, la ruta de los módulos de Higgs Mythos, la de los módulos de
usuario y las de los SDK (VST2 heredado, AAX, ARA, Android SDK…).

## Línea de comandos

El ejecutable de Mythos Studio también funciona como herramienta de línea de comandos. Las órdenes
que reescriben archivos usan saltos de línea CR-LF salvo que se añada `--lf`.

| Orden | Descripción |
|---|---|
| `--help`, `-h` | Muestra la ayuda. |
| `--resave archivo_proyecto` | Vuelve a guardar todos los archivos y recursos del proyecto (regenera los proyectos de los exportadores). Con `--fix-missing-dependencies` añade automáticamente los módulos dependientes que falten. |
| `--resave-resources archivo_proyecto` | Vuelve a guardar solo los recursos binarios del proyecto. |
| `--get-version archivo_proyecto` | Muestra el número de versión del proyecto. |
| `--set-version versión archivo_proyecto` | Cambia el número de versión del proyecto. |
| `--bump-version archivo_proyecto` | Incrementa en 1 la versión menor del proyecto. |
| `--git-tag-version archivo_proyecto` | Ejecuta `git tag` para etiquetar el repositorio actual con la versión del proyecto. |
| `--status archivo_proyecto` | Muestra información del proyecto (archivo, nombre, UID, módulos). |
| `--buildmodule carpeta_destino carpeta_modulo` | Empaqueta un módulo en un archivo ZIP descargable. |
| `--buildallmodules carpeta_destino carpeta_modulos` | Empaqueta todos los módulos de una carpeta y crea un índice. |
| `--trim-whitespace carpeta` | Recorre (recursivamente) los archivos C/C++, elimina los espacios finales de línea y normaliza los saltos de línea a CR-LF. |
| `--remove-tabs carpeta` | Sustituye los tabuladores por 4 espacios en los archivos C/C++. |
| `--tidy-divider-comments carpeta` | Normaliza las líneas separadoras de comentarios (`//=====`, `//-----`, `/////`). |
| `--fix-broken-include-paths carpeta` | Corrige los `#include` de archivos de la propia carpeta para usar la ruta relativa óptima; útil tras reorganizar archivos. |
| `--obfuscated-string-code cadena` | Genera una función C++ que devuelve la cadena de forma ofuscada. |
| `--encode-binary archivo_binario archivo_cpp_destino` | Convierte un binario en un archivo C++ con su contenido. Si el destino es `.h` genera un único archivo; si es `.cpp`, un par `.h`/`.cpp`. |
| `--trans carpetas...` | Busca macros `NEEDS_TRANS` en las carpetas y genera un archivo de traducción (véase [Localización](../guias/localizacion.md)). |
| `--trans-finish pretraducido traducido [existente]` | Crea el archivo final de correspondencias para inicializar un `LocalisedStrings`. |
| `--set-global-search-path so identificador ruta` | Cambia una ruta global. `so`: `osx`, `windows` o `linux`; identificador: `defaultHiggsModulePath`, `defaultUserModulePath`, `vstLegacyPath`, `aaxPath` (no en Linux) o `androidSDKPath`. |
| `--create-project-from-pip ruta/PIP ruta/salida [módulos] [módulos_usuario]` | Genera un proyecto completo a partir de un PIP. |

Ejemplos:

```bash
# Regenerar los proyectos de IDE tras cambiar el .mythos (por ejemplo, en CI)
MythosStudio --resave examples/DemoRunner/DemoRunner.mythos

# Fijar la versión antes de publicar
MythosStudio --set-version 1.2.0 MiApp.mythos
MythosStudio --git-tag-version MiApp.mythos

# Incrustar un archivo binario como código
MythosStudio --encode-binary logo.png Source/Logo.cpp
```

## Formato `.mythos`

Un archivo `.mythos` es XML. Su elemento raíz es `MYTHOSPROJECT`, con atributos como `name`,
`projectType` (`guiapp`, `consoleapp`, `audioplug`, `library`, `dll`…), `version`,
`companyName` o `defines`, y contiene los grupos de archivos (`MAINGROUP`), los módulos
(`MODULES`), sus opciones (`HIGGSOPTIONS`) y los exportadores con sus configuraciones
(`EXPORTFORMATS`). Normalmente no necesitas editarlo a mano.
