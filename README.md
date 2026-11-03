# Higgs Mythos

**Idioma:** Español ·
[English](README.en.md) ·
[Português](README.pt.md) ·
[Français](README.fr.md) ·
[Deutsch](README.de.md) ·
[Italiano](README.it.md) ·
[中文](README.zh-CN.md) ·
[日本語](README.ja.md)

<p align="center"><img src="documentacion/contenido/assets/higgs_mythos_banner.svg" alt="Higgs Mythos" width="100%"></p>

*La mascota de Higgs Mythos. Versión en texto: [higgs_mythos_mascot.txt](documentacion/contenido/assets/higgs_mythos_mascot.txt).*

Higgs Mythos (abreviado **HIGGS** en el código y en los nombres de los módulos)
es un framework de aplicaciones en C++ de código abierto y multiplataforma para
crear aplicaciones de escritorio y móviles, incluidos plug-ins de audio en los
formatos VST, VST3, AU, AUv3, AAX y LV2 y los *hosts* que los cargan. Un mismo
código fuente se compila para macOS, Windows, Linux, iOS y Android sin cambiar
de API: el framework encapsula el sistema de ventanas, el dibujo 2D, el audio y
el MIDI de cada sistema operativo detrás de clases comunes, y el proyecto solo
describe lo que necesita construir.

El framework se integra en proyectos existentes a través de CMake o se usa
desde **Mythos Studio**, su herramienta de configuración de proyectos, que
exporta proyectos para Xcode (macOS e iOS), Visual Studio, Android Studio y
Makefiles de Linux e incluye un editor de código. Está pensado tanto para
aplicaciones generales con interfaz gráfica, como herramientas de productividad
o utilidades de sistema, como para software de audio profesional que exige
procesamiento en tiempo real, una gestión de hilos estricta y compatibilidad
con los formatos de plug-in que usan las estaciones de trabajo de audio.

Higgs Mythos se distribuye bajo la licencia **AGPL-3.0-or-later**. Los avisos de
copyright de los autores originales se conservan en cada archivo fuente, y los
ejemplos se publican bajo la licencia ISC para que puedan copiarse como punto
de partida de un proyecto propio sin restricciones adicionales.

## Primeros pasos

El framework vive en el directorio `modules`. Cada módulo se llama
`higgs_<área>`, su cabecera pública es `<módulo>/<módulo>.h`, y toda la API de
C++ se declara en el espacio de nombres `higgs`. Un módulo es una unidad
autocontenida: declara en su cabecera principal de qué otros módulos depende,
qué cabeceras públicas expone y qué bibliotecas del sistema necesita en cada
plataforma, de modo que el sistema de compilación puede resolver el conjunto
mínimo de fuentes que hay que compilar para un proyecto dado.

Para empezar basta con incluir la cabecera del módulo que se necesita y derivar
de la clase correspondiente. El ejemplo siguiente crea un componente de
interfaz gráfica con un tamaño inicial de 800 por 600 píxeles; `Component` es la
clase base de todo elemento visual del framework, desde una ventana completa
hasta un botón:

```cpp
#include <higgs_core/higgs_core.h>
#include <higgs_gui_basics/higgs_gui_basics.h>

class MainComponent final : public higgs::Component
{
public:
    MainComponent() { setSize (800, 600); }
};
```

Las macros de configuración conservan el prefijo `HIGGS_` (por ejemplo
`HIGGS_MAJOR_VERSION` o `HIGGS_MODULE_AVAILABLE_higgs_core`), y los números de
versión se definen en
[higgs_StandardHeader.h](/modules/higgs_core/system/higgs_StandardHeader.h).
Las macros `HIGGS_WINDOWS`, `HIGGS_MAC`, `HIGGS_LINUX`, `HIGGS_IOS` y
`HIGGS_ANDROID` identifican la plataforma de destino cuando un proyecto necesita
código específico de un sistema. El resto de las convenciones de nombres
(espacio de nombres, funciones de CMake, extensión de proyecto `.mythos`) está
reunido en [docs/higgs-mythos.md](/docs/higgs-mythos.md).

## Módulos

Higgs Mythos se organiza en veinticuatro módulos independientes que se combinan
según lo que necesite cada proyecto. Una aplicación de consola puede usar solo
`higgs_core`; una aplicación gráfica añade `higgs_events`, `higgs_graphics` y
`higgs_gui_basics`; un plug-in de audio suma los módulos de audio y
`higgs_audio_plugin_client`. Cada módulo compila únicamente cuando se enlaza, de
modo que el binario final no arrastra código de áreas que no utiliza. La tabla
siguiente resume el propósito de cada uno; la documentación completa de cada
módulo está en el sitio de documentación y en las cabeceras Doxygen.

Los módulos se agrupan en capas. La base es `higgs_core`, del que dependen
todos los demás; sobre él se apoyan `higgs_events` y `higgs_data_structures`,
que no necesitan interfaz gráfica y por eso sirven también para herramientas
de consola y procesos en segundo plano. La capa gráfica (`higgs_graphics`,
`higgs_gui_basics`, `higgs_gui_extra`, `higgs_animation`, `higgs_opengl`)
construye la interfaz, y la capa de audio va desde los datos
(`higgs_audio_basics`) hasta los dispositivos, los formatos de archivo, los
procesadores y los formatos de plug-in. Cada módulo declara sus dependencias
en su cabecera principal, y Mythos Studio o CMake las resuelven
automáticamente al añadirlo a un proyecto.

| Módulo | Propósito |
|---|---|
| `higgs_core` | Clases básicas: cadenas, contenedores, memoria, hilos, archivos, red, XML, JSON, ZIP, tiempo y pruebas unitarias. |
| `higgs_events` | Bucle de mensajes, temporizadores, mensajes asíncronos, difusores y oyentes, clase base de aplicación e IPC. |
| `higgs_data_structures` | `ValueTree`, `Value`, `UndoManager` y gestión de preferencias. |
| `higgs_cryptography` | RSA, Blowfish, números primos y resúmenes MD5, SHA-256 y Whirlpool. |
| `higgs_graphics` | Motor 2D: `Graphics`, geometría, colores, fuentes, imágenes, SVG y efectos. |
| `higgs_gui_basics` | `Component`, ventanas, *widgets*, menús, *layouts*, *look-and-feel* y accesibilidad. |
| `higgs_gui_extra` | Editor de código, `WebBrowserComponent`, selector de colores, bandeja del sistema y notificaciones. |
| `higgs_animation` | Animación declarativa con funciones de suavizado sincronizadas con la pantalla. |
| `higgs_opengl` | Renderizado OpenGL y OpenGL ES dentro de componentes. |
| `higgs_video` | Reproducción de vídeo y captura de cámara. |
| `higgs_audio_basics` | `AudioBuffer`, fuentes de audio, sintetizador, MIDI 1.0, MPE y MIDI 2.0 (UMP). |
| `higgs_audio_devices` | Entrada y salida de audio y MIDI con CoreAudio, WASAPI, ASIO, ALSA, JACK, Oboe y OpenSL ES. |
| `higgs_audio_formats` | Lectura y escritura de WAV, AIFF, FLAC, Ogg Vorbis, Opus, MP3 y más. |
| `higgs_audio_processors_headless` | `AudioProcessor`, parámetros, `AudioProcessorGraph` y alojamiento de plug-ins sin interfaz. |
| `higgs_audio_processors` | Editores de procesador, `AudioProcessorValueTreeState` y escaneo de plug-ins. |
| `higgs_audio_plugin_client` | *Wrappers* VST2, VST3, AU, AUv3, AAX, LV2, Unity y Standalone. |
| `higgs_audio_utils` | `AudioAppComponent`, selectores de dispositivo, miniaturas de audio y teclados MIDI. |
| `higgs_dsp` | Filtros, osciladores, dinámica, reverberación, convolución, FFT y SIMD. |
| `higgs_midi_ci` | MIDI Capability Inquiry: descubrimiento, perfiles y propiedades. |
| `higgs_osc` | Open Sound Control sobre UDP. |
| `higgs_javascript` | Intérprete de JavaScript basado en QuickJS. |
| `higgs_box2d` | Motor de física 2D Box2D con renderizador integrado. |
| `higgs_analytics` | Recopilación y envío por lotes de analítica de uso. |
| `higgs_product_unlocking` | Compras integradas y licencias con desbloqueo en línea. |

## CMake

Se requiere CMake 3.22 o superior. La forma más directa de comprobar que el
entorno está listo es compilar las herramientas y los ejemplos incluidos en el
repositorio: la primera orden configura el árbol de compilación en el
directorio `cmake-build`, activando los ejemplos y los extras, y la segunda
compila `DemoRunner`, la aplicación que reúne todas las demostraciones del
framework:

```bash
cmake . -B cmake-build -DHIGGS_BUILD_EXAMPLES=ON -DHIGGS_BUILD_EXTRAS=ON
cmake --build cmake-build --target DemoRunner
```

En Windows hay que cargar antes el entorno de Visual Studio (`vcvars64.bat`)
para que CMake encuentre el compilador de MSVC, tanto con Ninja como con
MSBuild. En Linux es necesario instalar los paquetes de desarrollo que enumera
[docs/Linux Dependencies.md](/docs/Linux%20Dependencies.md) antes de
configurar.

La API de CMake usa el prefijo `higgs_`: `higgs_add_gui_app` crea una aplicación
gráfica, `higgs_add_console_app` una de consola, `higgs_add_plugin` un plug-in
con un objetivo por formato, `higgs_add_module` registra un módulo propio y
`higgs_generate_higgs_header` genera la cabecera agregada del proyecto. También
existen `higgs_add_binary_data` para incrustar recursos y
`higgs_set_<kind>_sdk_path` para indicar la ruta de los SDK de VST2, AAX o ARA.
Los objetivos importados se llaman `higgs::higgs_core`,
`higgs::higgs_gui_basics` y así sucesivamente, y se enlazan con
`target_link_libraries` como cualquier otra biblioteca. Para integrar el
framework en un proyecto existente basta con `add_subdirectory` sobre una copia
del repositorio o `find_package(HIGGS)` sobre una instalación. La referencia
completa está en [docs/CMake API.md](/docs/CMake%20API.md), y
[examples/CMake](/examples/CMake) contiene tres proyectos mínimos
(`GuiApp`, `ConsoleApp` y `AudioPlugin`) pensados para copiarse como punto de
partida.

## Mythos Studio

Mythos Studio es el generador de proyectos que se distribuye en
`extras/MythosStudio`. Guarda la configuración de un proyecto en un archivo
`.mythos` y, a partir de él, exporta proyectos nativos para cada IDE y
plataforma: Xcode para macOS e iOS, Visual Studio 2019, 2022 y 2026, Makefiles
de Linux y Android Studio con Gradle. Un mismo archivo `.mythos` puede tener
varios exportadores a la vez, cada uno con sus configuraciones de Debug y
Release, rutas de SDK y opciones de compilador y enlazador, de modo que el
proyecto se mantiene en un único lugar y los proyectos de IDE se regeneran
cuando cambian los módulos o los archivos fuente.

La herramienta incluye una página de inicio con plantillas de aplicación y de
plug-in, un gestor de módulos que resuelve las dependencias, un editor de
código con resaltado de sintaxis y una interfaz de línea de comandos
(`--resave`, `--set-version`, `--encode-binary`, `--create-project-from-pip`,
entre otras) para automatizar tareas sin abrir la ventana. Su compañera
`higgsaide` genera desde la terminal los datos binarios y demás recursos que los
proyectos incrustan. Android solo se admite a través de Mythos Studio, ya que la
API de CMake no genera proyectos para esa plataforma.

Mythos Studio se compila desde `extras/MythosStudio/Builds` (proyectos de Visual
Studio, Xcode o Makefile de Linux) o mediante CMake con
`-DHIGGS_BUILD_EXTRAS=ON` y el objetivo `MythosStudio`. También hay ejecutables
precompilados, sin firmar, para Windows x64, macOS universal y Linux x64,
generados por el flujo de GitHub Actions `HIGGS Mythos Studio Build` y
descargables como *artifacts* de cada ejecución.

## Herramientas y ejemplos

Además de Mythos Studio, el directorio `extras` reúne las herramientas que
acompañan al framework y que se compilan con `-DHIGGS_BUILD_EXTRAS=ON`.
`AudioPluginHost` es un *host* de plug-ins completo que escanea los formatos
instalados en el sistema, los conecta en un grafo de procesadores y permite
probar un plug-in propio sin depender de una estación de trabajo comercial.
`UnitTestRunner` ejecuta las pruebas unitarias que cada módulo declara con la
clase `UnitTest`; admite filtrar por categoría o por nombre y es la referencia
de verificación del proyecto. `BinaryBuilder` convierte archivos en datos
embebidos, `AudioPerformanceTest` mide el rendimiento del procesamiento de
audio en tiempo real, `NetworkGraphicsDemo` demuestra el dibujo distribuido
entre varias máquinas y `SBOM` genera y comprueba el inventario de dependencias
de terceros.

El directorio `examples` contiene demostraciones completas y compilables de la
mayoría de las funcionalidades, organizadas por área: `GUI`, `Audio`, `DSP`,
`Plugins`, `Utilities` y `Assets`. Todas son PIP: proyectos completos
contenidos en un único archivo de cabecera que empieza con un bloque de
metadatos (nombre, dependencias, tipo y clase principal), y que `DemoRunner`
carga y muestra junto a su código fuente, de modo que se puede leer la
implementación de cada demo mientras se ejecuta. `examples/CMake` contiene los
tres proyectos mínimos ya mencionados, y `examples/Plugins` incluye plug-ins
completos (sintetizadores, un sampler, un arpegiador, una puerta de ruido, un
*host* dentro de un plug-in y un ejemplo de ARA) que sirven como referencia
para el ciclo de vida completo de un `AudioProcessor`.

## Documentación

La guía de usuario completa en español se publica en
<https://jalexiscv.github.io/higgs-mythos/> y sus fuentes viven en
[documentacion](/documentacion). Cubre los primeros pasos (instalación,
primera aplicación y primer plug-in), los conceptos del framework
(arquitectura, bucle de mensajes, hilos, memoria y convenciones), una guía por
cada área (interfaz gráfica, gráficos, audio, MIDI, DSP, plug-ins, alojamiento,
OpenGL, vídeo, web, accesibilidad, localización), una página de referencia por
módulo, la API de CMake, las herramientas y las notas del proyecto (cambios
incompatibles, licencia, seguridad y glosario). El sitio se construye con
MkDocs y se despliega automáticamente en GitHub Pages cuando cambian la
documentación, los módulos o los ejemplos en la rama `main`; las páginas de
referencia se regeneran a partir del árbol de fuentes en cada despliegue.

La documentación de la API se genera directamente a partir de los comentarios
Doxygen de las cabeceras de los módulos, y el código fuente es la documentación
más completa y actualizada que existe. La cabecera principal de cada módulo,
`higgs_<módulo>/higgs_<módulo>.h`, enumera sus cabeceras públicas y sus
dependencias; el formato de módulo está descrito en
[docs/Higgs Mythos Module Format.md](/docs/Higgs%20Mythos%20Module%20Format.md).
Para localizar una clase, basta con buscar su declaración en las cabeceras y
leer el bloque de comentario situado encima:

```bash
grep -rlE "^\s*(class|struct)\s+(HIGGS_API\s+)?AudioProcessorValueTreeState\b" modules
```

[BREAKING_CHANGES.md](BREAKING_CHANGES.md) describe, versión por versión, qué
cambió y cómo actualizar el código; [CHANGE_LIST.md](CHANGE_LIST.md) resume los
titulares de cada versión, y [docs/README.md](/docs/README.md) es el índice de
la documentación técnica del repositorio (formato de módulos, API de CMake,
identidad del framework y protocolos de trabajo).

## Requisitos mínimos del sistema

Higgs Mythos exige C++17 como estándar mínimo porque sus cabeceras públicas
usan características de ese estándar (cada módulo lo declara en el campo
`minimumCppStandard` de su cabecera principal); cuando se activa
`HIGGS_USE_WINDOWS_MIDI_SERVICES` el proyecto debe compilarse con C++20, ya que
el SDK de Windows MIDI Services lo requiere. Las versiones de compilador que se
indican a continuación son las más antiguas que se prueban: versiones más
recientes de Xcode, Visual Studio, GCC y Clang funcionan sin cambios. En Linux,
además del compilador, hacen falta las bibliotecas de desarrollo de X11,
FreeType, ALSA y otras que se enumeran en
[docs/Linux Dependencies.md](/docs/Linux%20Dependencies.md); en Android el
proyecto se construye con Gradle y el NDK desde Android Studio, a través de los
proyectos que exporta Mythos Studio. Las dos listas siguientes distinguen el
equipo de desarrollo del equipo de ejecución: la primera indica con qué se
compila, la segunda dónde se ejecuta el resultado. Por ejemplo, un plug-in
para Windows en Arm64 se compila desde Visual Studio en cualquier máquina
Windows compatible, y las aplicaciones de iOS y Android se compilan desde
macOS o desde un escritorio con Android Studio, respectivamente.

### Compilar proyectos HIGGS

Esta lista reúne, por plataforma de desarrollo, la herramienta mínima con la
que el framework se compila: el IDE o el compilador y la versión del sistema
operativo del equipo de desarrollo. En macOS e iOS se usa Xcode, que incluye
Clang y los SDK de Apple; la versión mínima del sistema depende de si el
equipo es Intel o Apple Silicon porque Xcode 12.4 exige sistemas distintos en
cada arquitectura. En Windows se usa Visual Studio con la carga de trabajo de
escritorio de C++ y el SDK de Windows 10, tanto desde el IDE como desde la
línea de comandos tras cargar `vcvars64.bat`. En Linux sirve cualquier GCC o
Clang moderno junto con las bibliotecas de desarrollo del sistema, y los
Makefiles que exporta Mythos Studio o los proyectos de CMake se compilan desde
la terminal. En Android se requieren Android Studio, el plugin de Gradle y el
NDK, con los que se compilan los proyectos que exporta Mythos Studio; el
equipo anfitrión puede ser Windows, macOS o Linux. Las versiones indicadas son
mínimos probados, no recomendaciones: las versiones actuales de cada
herramienta funcionan sin cambios.

- __Estándar de C++__: 17 (20 cuando se compila con `HIGGS_USE_WINDOWS_MIDI_SERVICES` activado)
- __macOS/iOS__: Xcode 12.4 (macOS 10.15.4 en Intel, macOS 11.0 en Apple Silicon)
- __Windows__: Visual Studio 2019 (Windows 10 versión 1607)
- __Linux__: g++ 7.0 o Clang 6.0 (ver [docs/Linux Dependencies.md](/docs/Linux%20Dependencies.md))
- __Android__: Android Studio Narwhal 3 (2025.1.3) con Android Gradle plugin 8.13.2
  y NDK 28 en Windows, macOS o Linux

### Plataformas de destino

Los valores siguientes son las versiones mínimas del sistema operativo en las
que se ejecutan las aplicaciones y los plug-ins compilados con el framework, y
las arquitecturas de procesador para las que se generan binarios. En macOS y
Windows es habitual producir binarios universales o varias arquitecturas desde
el mismo proyecto; en Android, Mythos Studio genera los ABI seleccionados en un
único paquete. Elegir una versión mínima más alta para un proyecto concreto
(por ejemplo macOS 11 en lugar de 10.11) es posible desde los ajustes del
exportador en Mythos Studio o con las variables habituales de CMake, y permite
usar API más recientes del sistema; bajar de los valores de la lista no está
soportado, porque el framework usa API que no existen en versiones anteriores.
En iOS, el simulador en x86_64 permite probar en Macs con procesador Intel, y
los dispositivos físicos usan Arm64 o Arm64e. En Android conviene incluir al
menos arm64-v8a, la arquitectura de todos los dispositivos actuales, y añadir
las demás solo cuando el público objetivo lo justifique, porque cada ABI
adicional aumenta el tamaño del paquete.

- __macOS__: macOS 10.11 (x86_64, Arm64)
- __Windows__: Windows 10 versión 1607 (x86_64, x86, Arm64, Arm64EC)
- __Linux__: distribuciones mayoritarias (x86_64, Arm64/aarch64)
- __iOS__: iOS 12 (Arm64, Arm64e, simulador x86_64)
- __Android__: Android 7 - Nougat (API Level 24) (arm64-v8a, armeabi-v7a, x86_64, x86)

## Contribuir

Las contribuciones se reciben a través de GitHub. Las peticiones de
funcionalidades y las preguntas generales van en la sección de *Discussions*
del repositorio; los errores reproducibles se registran como *issues* indicando
la versión de Higgs Mythos, los módulos afectados, la plataforma y una
reproducción mínima, usando las plantillas de `.github/ISSUE_TEMPLATE` cuando
sea posible. Las vulnerabilidades de seguridad nunca se publican en un *issue*:
se comunican de forma privada por el canal que describe
[SECURITY.md](SECURITY.md).

Cada *pull request* debe centrarse en un solo cambio y describir qué corrige o
qué añade. Antes de abrirlo hay que compilar los objetivos afectados y ejecutar
las pruebas unitarias con `UnitTestRunner`. El código, los comentarios y los
mensajes de commit se escriben en inglés con el formato `area: resumen`. Al
enviar una contribución se acepta licenciarla bajo los mismos términos que el
proyecto (AGPL-3.0-or-later) y se confirma ser autor del código o tener derecho
a enviarlo; no se admite código copiado de binarios propietarios ni de fuentes
con licencia incompatible con la AGPL. La guía completa está en
[.github/contributing.md](.github/contributing.md), y
[CODE_OF_CONDUCT.md](CODE_OF_CONDUCT.md) establece las normas de convivencia de
la comunidad.

## Licencia

Higgs Mythos es software libre: se puede redistribuir y modificar bajo los
términos de la GNU Affero General Public License publicada por la Free Software
Foundation, en su versión 3 o, a elección del usuario, cualquier versión
posterior (**AGPL-3.0-or-later**). El texto completo y las notas sobre su
aplicación están en [LICENSE.md](LICENSE.md). La AGPL es una licencia
*copyleft*: cualquier obra derivada que se distribuya, o que se ofrezca a los
usuarios a través de una red, debe publicarse bajo la misma licencia y con su
código fuente disponible. Esto incluye las aplicaciones y los plug-ins que
enlazan el framework.

Higgs Mythos deriva de una base de código abierto anterior cuya marca se retiró
por completo de los nombres, los módulos, las macros y los recursos. Los
avisos de copyright de los autores originales se conservan intactos en la
cabecera de cada archivo fuente, tal como exige la licencia, y no deben
editarse ni eliminarse. Los ejemplos de `examples` se publican bajo la licencia
ISC, mucho más permisiva, para que puedan copiarse como base de un proyecto
nuevo. El inventario de dependencias de terceros y sus licencias está en
[HiggsMythos.spdx.json](HiggsMythos.spdx.json), en formato SPDX, y los SDK de
plug-ins (VST3, AAX, ARA, LV2) se utilizan bajo sus propias licencias y
acuerdos sin incluirse en el repositorio cuando estas no lo permiten.

## Plug-ins AAX

Los plug-ins AAX deben firmarse digitalmente con las herramientas de firma de
PACE Anti-Piracy antes de que puedan ejecutarse en las versiones comerciales de
Pro Tools. Avid proporciona estas herramientas de forma gratuita. Antes de
obtenerlas es necesario usar una compilación especial de Pro Tools, llamada Pro
Tools Developer, para probar los plug-ins sin firmar. Los pasos para obtener
Pro Tools Developer son:

1. Registrarse como desarrollador AAX en <https://developer.avid.com/aax/>.
2. Solicitar un código de activación del Pro Tools Developer Bundle escribiendo
   a <devauth@avid.com>.
3. Descargar la última compilación de Pro Tools Developer desde la cuenta de
   desarrollador de Avid.

Cuando los plug-ins se hayan probado y depurado en Pro Tools Developer y estén
listos para firmarse, hay que escribir a <audiosdk@avid.com> con el asunto
"PACE Eden Signing Tools Request". El mensaje debe incluir una descripción de
cada plug-in junto con una grabación de pantalla que muestre el plug-in
ejecutándose en Pro Tools Developer, con audio si es posible, además de la
siguiente información:

- Nombre de la empresa
- Nombre completo del administrador
- Número de teléfono

Una vez enviada la solicitud, PACE Anti-Piracy se pondrá en contacto
directamente con información sobre la firma de los plug-ins. El objetivo AAX de
`higgs_add_plugin` genera el paquete `.aaxplugin` listo para firmar; la ruta
del SDK de AAX se indica con `higgs_set_aax_sdk_path` antes de declarar el
plug-in.
