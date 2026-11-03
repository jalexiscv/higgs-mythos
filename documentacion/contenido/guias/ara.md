# ARA (*Audio Random Access*)

Higgs Mythos permite desarrollar hosts y plug-ins compatibles con **ARA**, la extensión que da a
los plug-ins acceso aleatorio al audio completo de las pistas del host (en lugar de solo bloque a
bloque) y les permite intercambiar información musical: notas, tempo, compases, tonalidad,
acordes… ARA es una extensión de **VST3** y **AU**: un plug-in ARA necesita al menos uno de esos
formatos.

## Dependencia externa

El SDK de ARA **no se incluye** en el repositorio. Se necesita la versión **2.3.0**:

```bash
git clone --recursive --branch releases/2.3.0 https://github.com/Celemony/ARA_SDK
```

## Activar ARA

=== "CMake"

    ```cmake
    higgs_set_ara_sdk_path(/ruta/a/ARA_SDK)     # antes de crear los targets
    ```

    Si compilas los ejemplos y extras del propio repositorio, también puedes pasar
    `-DHIGGS_GLOBAL_ARA_SDK_PATH=/ruta/a/ARA_SDK` al configurar CMake.

=== "Mythos Studio"

    Añade la ruta del SDK en los ajustes de rutas globales: menú **MythosStudio → Global Paths...**
    en macOS o **File → Global Paths...** en Windows y Linux.

## Plug-ins con ARA

=== "CMake"

    Añade `IS_ARA_EFFECT TRUE` a tu llamada a `higgs_add_plugin`. Esto activa rutas de código
    adicionales en los *wrappers* de VST3 y AU y define `HiggsPlugin_Enable_ARA=1` (no la definas
    a mano). Opcionalmente: `ARA_FACTORY_ID`, `ARA_DOCUMENT_ARCHIVE_ID`, `ARA_ANALYSIS_TYPES` y
    `ARA_TRANSFORMATION_FLAGS` (véase el [API de CMake](../compilacion/cmake-api.md#higgs_add_target)).

=== "Mythos Studio"

    Marca la opción **Enable ARA** en los ajustes de formatos de plug-in, junto con VST3 y/o AU.

En el código, además de `createPluginFilter()` debes implementar `createARAFactory()`. La forma más
sencilla es heredar de `higgs::ARADocumentControllerSpecialisation` y usar su función auxiliar;
la documentación de esa clase incluye un ejemplo, y el `ARAPluginDemo` muestra un plug-in completo.

Clases principales del lado del plug-in: `ARADocumentControllerSpecialisation`,
`ARADocument`, `ARAMusicalContext`, `ARARegionSequence`, `ARAAudioSource`,
`ARAAudioModification`, `ARAPlaybackRegion`, `ARAPlaybackRenderer`, `ARAEditorRenderer`,
`ARAEditorView` y `AudioProcessorARAExtension` / `AudioProcessorEditorARAExtension`.

## Hosts con ARA: el AudioPluginHost

El AudioPluginHost tiene funciones básicas de host ARA, pero hay que activarlas en su
configuración:

=== "CMake"

    Define `HIGGS_PLUGINHOST_ARA=1` en `extras/AudioPluginHost/CMakeLists.txt`.

=== "Mythos Studio"

    Abre `AudioPluginHost.mythos`, ve a *Modules* → *higgs_audio_processors* y activa
    `HIGGS_PLUGINHOST_ARA`.

Con ello, los plug-ins compatibles con ARA aparecen dos veces en el menú de creación; la entrada
marcada como **(ARA)** activa las funciones adicionales. Con el botón derecho sobre el plug-in en el
grafo, la opción *Show ARA host control* permite asignar un archivo de audio que el plug-in puede
leer a través de las interfaces ARA.

## Para aprender más

ARA tiene un API amplio que permite intercambiar información con el host de formas totalmente
nuevas. La documentación oficial incluida en el SDK, con secciones introductorias, está en
`ARA_SDK/ARA_Library/html_docs/index.html`.
