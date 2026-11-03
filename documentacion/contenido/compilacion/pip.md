# Proyectos PIP

Un **PIP** es un proyecto completo contenido en **un único archivo de cabecera**. El archivo empieza
con un bloque de metadatos que describe el proyecto —nombre, dependencias, tipo y clase principal—
y a continuación contiene el código. Todos los ejemplos de `examples/` son PIP.

## El bloque de metadatos

```cpp
/*******************************************************************************
 BEGIN_HIGGS_PIP_METADATA

 name:             HelloWorldDemo
 version:          1.0.0
 vendor:           HIGGS
 website:          http://higgsmythos.com
 description:      Simple HelloWorld application.

 dependencies:     higgs_core, higgs_data_structures, higgs_events, higgs_graphics,
                   higgs_gui_basics
 exporters:        xcode_mac, vs2022, vs2026, linux_make, xcode_iphone

 moduleFlags:      HIGGS_STRICT_REFCOUNTEDPOINTER=1

 type:             Component
 mainClass:        HelloWorldDemo

 useLocalCopy:     1

 END_HIGGS_PIP_METADATA
*******************************************************************************/

#pragma once

class HelloWorldDemo final : public Component
{
    // ...
};
```

| Clave | Descripción |
|---|---|
| `name` | Nombre del proyecto y del target que se genera. |
| `version`, `vendor`, `website`, `description` | Datos informativos del proyecto. |
| `dependencies` | Módulos necesarios, separados por comas o espacios (pueden ocupar varias líneas). |
| `exporters` | Exportadores de Mythos Studio para los que se genera proyecto: `xcode_mac`, `xcode_iphone`, `vs2022`, `vs2026`, `linux_make`, `androidstudio`… |
| `moduleFlags` | Opciones de configuración de módulos (`CLAVE=valor`, separadas por comas). |
| `defines` | Definiciones de preprocesador adicionales. |
| `type` | `Component` (aplicación gráfica cuyo contenido es la clase principal), `AudioProcessor` (plug-in) o `Console` (aplicación de consola). |
| `mainClass` | Clase principal: el componente, el procesador de audio o la clase con el código de consola. |
| `documentControllerClass` | Para plug-ins ARA: la clase del controlador de documento. |
| `pluginCharacteristics` | Para plug-ins: características como `pluginIsSynth`, `pluginWantsMidiIn`, `pluginProducesMidiOut`, `pluginIsMidiEffectPlugin`, `pluginEditorRequiresKeys`. |
| `extraPluginFormats` | Formatos adicionales para plug-ins (por ejemplo `AUv3`). |
| `useLocalCopy` | Si Mythos Studio copia el archivo PIP dentro del proyecto generado. |

## Compilar un PIP

=== "CMake"

    ```cmake
    higgs_add_pip(${CMAKE_CURRENT_SOURCE_DIR}/MiPrototipo.h)
    ```

    `higgs_add_pip` crea un target de aplicación gráfica, de consola o de plug-in (con todos los
    formatos posibles) según el campo `type`. Para compilar también VST2, llama antes a
    `higgs_set_vst2_sdk_path`.

=== "Mythos Studio"

    Abre el archivo `.h` con Mythos Studio (o arrástralo a su ventana) y se generará un proyecto
    completo. Desde la línea de comandos:

    ```bash
    MythosStudio --create-project-from-pip ruta/al/PIP.h ruta/de/salida [ruta/a/modulos] [ruta/a/modulos/usuario]
    ```

!!! tip "Cuándo usar un PIP"
    Los PIP son ideales para demos, prototipos rápidos e informes de errores reproducibles. Para un
    producto real usa `higgs_add_gui_app`, `higgs_add_plugin` o `higgs_add_console_app`, que dan un
    control mucho más fino.
