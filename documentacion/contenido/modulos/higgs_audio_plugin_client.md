# higgs_audio_plugin_client

Los *wrappers* que convierten un `AudioProcessor` en un plug-in de cada formato: VST2, VST3, AU, AUv3, AAX, LV2, Unity y aplicación Standalone. Normalmente no se usa directamente: `higgs_add_plugin` lo enlaza en cada target de formato.

## Ficha

| Campo | Valor |
|---|---|
| Identificador | `higgs_audio_plugin_client` |
| Versión | 9.0.5 |
| Nombre declarado | Higgs Mythos audio plugin wrapper classes |
| Descripción declarada | Classes for building VST, VST3, AU, AUv3, LV2 and AAX plugins. |
| Estándar mínimo de C++ | C++17 |
| Licencia | AGPLv3 |
| Dependencias directas | [`higgs_audio_processors`](higgs_audio_processors.md) |
| Dependencias indirectas | [`higgs_core`](higgs_core.md), [`higgs_events`](higgs_events.md), [`higgs_data_structures`](higgs_data_structures.md), [`higgs_graphics`](higgs_graphics.md), [`higgs_gui_basics`](higgs_gui_basics.md), [`higgs_gui_extra`](higgs_gui_extra.md), [`higgs_audio_basics`](higgs_audio_basics.md), [`higgs_audio_processors_headless`](higgs_audio_processors_headless.md) |
| Código fuente | [`modules/higgs_audio_plugin_client`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_plugin_client) |

## Uso

```cmake
target_link_libraries(MiTarget PRIVATE higgs::higgs_audio_plugin_client)
```

```cpp
#include <higgs_audio_plugin_client/higgs_audio_plugin_client.h>
```

!!! note
    Este módulo no se enlaza a mano: `higgs_add_plugin` lo añade automáticamente a cada
    target de formato. Tu código solo tiene que definir `createPluginFilter()`.

Guías relacionadas: [Plug-ins de audio](../guias/plugins.md), [Tu primer plug-in](../primeros-pasos/primer-plugin.md).

## Opciones de configuración

Se cambian con `target_compile_definitions(MiTarget PUBLIC OPCION=valor)` o en la sección *Modules* de Mythos Studio.

| Opción | Por defecto | Descripción |
|---|---|---|
| `HIGGS_VST3_CAN_REPLACE_VST2` | `1` | Habilite esta opción si desea que su plug-in VST3 cargue y guarde un estado compatible con VST2. Esto permite que los hosts reemplacen plug-ins VST2 por plug-ins VST3. Si cambia esta opción, su plug-in VST3 será, de forma predeterminada, incompatible con las versiones anteriores. Si ya publicó un VST2 y un VST3 con este indicador establecido en `0`, aún puede habilitar la migración de VST2 a VST3 en hosts más recientes definiendo el símbolo de preprocesador `HIGGS_VST3_COMPATIBLE_CLASSES` e implementando el método `VST3ClientExtensions::getCompatibleParameterIds()`. |
| `HIGGS_FORCE_USE_LEGACY_PARAM_IDS` | `0` | Habilite esta opción si desea obligar a HIGGS a usar un índice de parámetro continuo para identificar un parámetro en un DAW (este era el comportamiento predeterminado en versiones antiguas de HIGGS y siempre es el predeterminado para los plug-ins VST2). El DAW suele usar este índice para guardar los datos de automatización. Cambiar esta configuración puede estropear los proyectos de DAW de los usuarios; consulte `VST3ClientExtensions::getCompatibleParameterIds()` para conocer una forma de resolver este problema en hosts VST3 más recientes. |
| `HIGGS_FORCE_LEGACY_PARAMETER_AUTOMATION_TYPE` | `0` | Habilite esta opción si desea obligar a HIGGS a usar un esquema heredado para identificar los parámetros de los plug-ins como continuos o discretos. Los proyectos de DAW con datos de automatización escritos por un plug-in AudioUnit, VST3 o AAX compilado con HIGGS versión 5.1.1 o anterior pueden cargarse incorrectamente al abrirlos con un plug-in AudioUnit, VST3 o AAX compilado con HIGGS versión 5.2.0 o posterior. |
| `HIGGS_USE_STUDIO_ONE_COMPATIBLE_PARAMETERS` | `1` | Habilite esta opción si desea que HIGGS use identificadores de parámetro compatibles con Studio One, ya que Studio One ignora los identificadores de parámetro negativos. Al habilitar esta opción, HIGGS generará solo identificadores de parámetro positivos. Tenga en cuenta que, si ya publicó un plug-in antes de HIGGS 4.3.0, habilitar esta opción cambiará sus identificadores de parámetro, lo que hará que su plug-in sea incompatible con los datos de automatización antiguos. |
| `HIGGS_AU_WRAPPERS_SAVE_PROGRAM_STATES` | `0` | Habilite esta opción si desea recibir llamadas a `get/setProgramStateInformation`, en lugar de llamadas a `get/setStateInformation`, desde los wrappers de plug-in AU y AUv3. En HIGGS versión 5.4.5 y anteriores este era el comportamiento predeterminado, por lo que, si modificó las implementaciones predeterminadas de `get/setProgramStateInformation` (cuyas implementaciones predeterminadas simplemente llaman a `get/setStateInformation`), es posible que deba habilitar esta opción de configuración para mantener la compatibilidad con versiones anteriores del estado guardado previamente. |
| `HIGGS_STANDALONE_FILTER_WINDOW_USE_KIOSK_MODE` | 1 en iOS y Android; 0 en el resto | Habilite esta opción si desea que la ventana de su plug-in independiente (standalone) use el modo quiosco. De forma predeterminada, el modo quiosco está habilitado en iOS y Android. |
| `HIGGS_IGNORE_VST3_MISMATCHED_PARAMETER_ID_WARNING` | `0` | Habilite esta opción para ignorar una advertencia que se produce al habilitar `HIGGS_VST3_CAN_REPLACE_VST2` sin habilitar `HIGGS_FORCE_USE_LEGACY_PARAM_IDS`. |

## Código de terceros incluido

| Componente | Licencia | Ubicación |
|---|---|---|
| [AAX SDK](https://developer.avid.com/aax) | `LicenseRef-Avid-AAX OR GPL-3.0-only` | `modules/higgs_audio_plugin_client/AAX/SDK` |
| [AudioUnitSDK](https://github.com/apple/AudioUnitSDK) | `Apache-2.0` | `modules/higgs_audio_plugin_client/AU/AudioUnitSDK` |

## Wrappers por formato

| Formato | Archivos | Carpeta |
|---|---|---|
| AAX | `higgs_audio_plugin_client_AAX.cpp`, `higgs_audio_plugin_client_AAX.mm`, `higgs_audio_plugin_client_AAX_utils.cpp` | `AAX/` |
| ARA | `higgs_audio_plugin_client_ARA.cpp` | — |
| AU (v2) | `higgs_audio_plugin_client_AU_1.mm`, `higgs_audio_plugin_client_AU_2.mm` | `AU/` |
| AUv3 | `higgs_audio_plugin_client_AUv3.mm` | — |
| LV2 | `higgs_audio_plugin_client_LV2.cpp`, `higgs_audio_plugin_client_LV2.mm` | `LV2/` |
| Standalone | `higgs_audio_plugin_client_Standalone.cpp` | `Standalone/` |
| Unity | `higgs_audio_plugin_client_Unity.cpp` | `Unity/` |
| VST2 | `higgs_audio_plugin_client_VST2.cpp`, `higgs_audio_plugin_client_VST2.mm` | — |
| VST3 | `higgs_audio_plugin_client_VST3.cpp`, `higgs_audio_plugin_client_VST3.mm` | `VST3/` |

## Catálogo de clases

Este módulo no expone clases públicas propias en su cabecera principal.
