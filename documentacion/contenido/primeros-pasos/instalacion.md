# Instalación

Higgs Mythos se distribuye como código fuente. No hay binarios que instalar: el framework se
compila junto con tu proyecto. Hay dos formas de conectarlo a un proyecto CMake.

## 1. Obtener el código

```bash
git clone https://github.com/jalexiscv/higgs-mythos.git
```

Si tu proyecto ya usa Git, lo más cómodo es añadirlo como **submódulo**, de forma que cada
proyecto fije la versión exacta del framework que utiliza:

```bash
cd mi-proyecto
git submodule add https://github.com/jalexiscv/higgs-mythos.git HIGGS
git submodule update --init
```

## 2a. Usar `add_subdirectory` (recomendado)

La forma más sencilla es tener Higgs Mythos como subdirectorio de tu proyecto e incluirlo desde
tu `CMakeLists.txt`. Así quedan disponibles los targets de los módulos (`higgs::higgs_core`,
`higgs::higgs_gui_basics`, …) y las funciones auxiliares (`higgs_add_gui_app`,
`higgs_add_plugin`, …):

```cmake title="CMakeLists.txt"
cmake_minimum_required(VERSION 3.22)
project(MI_APP VERSION 0.1.0 LANGUAGES C CXX)

add_subdirectory(HIGGS)          # carpeta donde está Higgs Mythos

higgs_add_gui_app(MiApp PRODUCT_NAME "Mi App")
target_sources(MiApp PRIVATE Main.cpp)
target_link_libraries(MiApp PRIVATE higgs::higgs_gui_extra)
```

!!! warning "El proyecto necesita el lenguaje C"
    El `CMakeLists.txt` raíz de Higgs Mythos comprueba que el lenguaje **C** esté habilitado:
    algunas dependencias incluidas (zlib, libjpeg, libpng, FLAC, Ogg/Opus…) se compilan en modo C.
    Si tu llamada a `project()` solo declara `CXX`, CMake se detendrá con el error
    *"A C compiler is required to build targets that depend on HIGGS"*. Declara
    `LANGUAGES C CXX`, u omite `LANGUAGES` (por defecto CMake habilita C y C++).

## 2b. Instalar el framework y usar `find_package`

También puedes instalar Higgs Mythos en una ubicación del sistema y consumirlo desde varios
proyectos. Primero configura e instala solo los componentes de biblioteca:

```bash
# Desde el directorio donde clonaste Higgs Mythos
cmake -B cmake-build-install -DCMAKE_INSTALL_PREFIX=/ruta/a/HIGGS/install
cmake --build cmake-build-install --target install
```

La instalación copia los módulos en `include/HIGGS-9.0.5/modules`, los scripts de CMake en
`lib/cmake/HIGGS-9.0.5` y la herramienta auxiliar `higgsaide` (véase
[higgsaide](../herramientas/higgsaide.md)).

En el proyecto que consume el framework, sustituye `add_subdirectory` por:

```cmake
find_package(HIGGS CONFIG REQUIRED)
```

y pasa la ruta de instalación al configurar:

```bash
cd /ruta/a/mi/proyecto
cmake -B cmake-build -DCMAKE_PREFIX_PATH=/ruta/a/HIGGS/install
cmake --build cmake-build
```

!!! note "Compatibilidad de versión exacta"
    El archivo `HIGGSConfigVersion.cmake` se genera con `COMPATIBILITY ExactVersion`: si pides una
    versión concreta en `find_package(HIGGS 9.0.5 CONFIG REQUIRED)`, debe coincidir exactamente
    con la instalada.

## 3. Configurar y compilar

Con el proyecto preparado, genera el árbol de compilación de la forma habitual:

```bash
cmake -B build                       # generador por defecto
cmake -B build -G Ninja              # o elige uno con -G
cmake -B build -G Xcode              # proyecto de Xcode
cmake -B build -G "Visual Studio 17 2022"
```

`cmake -G` sin argumentos muestra la lista de generadores disponibles en tu plataforma. El
generador por defecto es Makefiles en macOS y Linux, y la versión más reciente de Visual Studio
en Windows.

Para compilar:

```bash
cmake --build build --target MiApp --config Release
```

Si generaste Makefiles o archivos de Ninja también puedes ejecutar `make` o `ninja` dentro de
`build`; si generaste un proyecto de IDE, ábrelo con Xcode o Visual Studio.

!!! tip "Windows"
    Antes de usar Ninja o MSBuild desde la línea de comandos, carga el entorno de Visual Studio
    (por ejemplo ejecutando `vcvars64.bat` o abriendo la *Developer Command Prompt*).

## Opciones del proyecto raíz

Al incluir Higgs Mythos puedes activar o desactivar estas opciones con `-DOPCION=ON/OFF` o con
`set(OPCION ON)` antes de `add_subdirectory`/`find_package`:

| Opción | Por defecto | Efecto |
|---|---|---|
| `HIGGS_BUILD_EXTRAS` | `OFF` | Añade los targets de `extras` (Mythos Studio, AudioPluginHost…) |
| `HIGGS_BUILD_EXAMPLES` | `OFF` | Añade los targets de `examples` (DemoRunner y los PIP) |
| `HIGGS_ENABLE_MODULE_SOURCE_GROUPS` | `OFF` | Muestra las fuentes de los módulos en los proyectos de IDE |
| `HIGGS_COPY_PLUGIN_AFTER_BUILD` | `OFF` | Instala los plug-ins en el sistema tras compilarlos |
| `HIGGS_MODULES_ONLY` | `OFF` | Solo añade los módulos y `higgs_add_module*` |
| `HIGGS_WEBVIEW2_PACKAGE_LOCATION` | — | Ruta alternativa para buscar el paquete WebView2 en Windows |
| `HIGGS_GLOBAL_VST2_SDK_PATH`, `HIGGS_GLOBAL_AAX_SDK_PATH`, `HIGGS_GLOBAL_ARA_SDK_PATH` | — | Rutas a SDK externos al compilar los ejemplos y extras del propio repositorio |

La descripción completa de cada opción está en el [API de CMake](../compilacion/cmake-api.md#opciones).

## Siguiente paso

Continúa con [Tu primera aplicación gráfica](primera-aplicacion.md).
