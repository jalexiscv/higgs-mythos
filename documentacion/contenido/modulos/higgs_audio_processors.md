# higgs_audio_processors

Amplía `higgs_audio_processors_headless` con la parte de interfaz: `AudioProcessorEditor`, `GenericAudioProcessorEditor`, `AudioProcessorValueTreeState` y sus *attachments*, *attachments* de parámetros (incluidos los de interfaces web), escaneo de plug-ins y componentes de lista de plug-ins.

## Ficha

| Campo | Valor |
|---|---|
| Identificador | `higgs_audio_processors` |
| Versión | 9.0.5 |
| Nombre declarado | Higgs Mythos audio processor classes |
| Descripción declarada | Classes for loading and playing VST, AU, LADSPA, or internally-generated audio processors. |
| Estándar mínimo de C++ | C++17 |
| Licencia | AGPLv3 |
| Dependencias directas | [`higgs_gui_extra`](higgs_gui_extra.md), [`higgs_audio_processors_headless`](higgs_audio_processors_headless.md) |
| Dependencias indirectas | [`higgs_core`](higgs_core.md), [`higgs_events`](higgs_events.md), [`higgs_data_structures`](higgs_data_structures.md), [`higgs_graphics`](higgs_graphics.md), [`higgs_gui_basics`](higgs_gui_basics.md), [`higgs_audio_basics`](higgs_audio_basics.md) |
| Módulos que dependen de él | [`higgs_audio_plugin_client`](higgs_audio_plugin_client.md), [`higgs_audio_utils`](higgs_audio_utils.md) |
| Código fuente | [`modules/higgs_audio_processors`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_processors) |

## Uso

```cmake
target_link_libraries(MiTarget PRIVATE higgs::higgs_audio_processors)
```

```cpp
#include <higgs_audio_processors/higgs_audio_processors.h>
```

Guías relacionadas: [Plug-ins de audio](../guias/plugins.md), [Alojar plug-ins (hosting)](../guias/hosting.md).

## Opciones de configuración

Este módulo no declara opciones de configuración.

## Catálogo de clases

22 clases y estructuras públicas declaradas en las cabeceras que incluye `higgs_audio_processors.h`, agrupadas por carpeta. La descripción es la primera frase de su comentario de documentación en el código.

### `processors`

| Clase | Descripción | Cabecera |
|---|---|---|
| `HostProvidedContextMenu` | Envuelve un menú contextual para un parámetro específico, tal como lo proporciona el host. | [`higgs_AudioProcessorEditorHostContext.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_processors/processors/higgs_AudioProcessorEditorHostContext.h) |
| `AudioProcessorEditorHostContext` | Llamar a AudioProcessorEditor::getHostContext() puede devolver un puntero a una instancia de esta clase. | [`higgs_AudioProcessorEditorHostContext.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_processors/processors/higgs_AudioProcessorEditorHostContext.h) |
| `AudioProcessorEditor` | Clase base para el componente que actúa como interfaz gráfica de un AudioProcessor. | [`higgs_AudioProcessorEditor.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_processors/processors/higgs_AudioProcessorEditor.h) |
| `GenericAudioProcessorEditor` | Un tipo de componente de interfaz de usuario que muestra los parámetros de un AudioProcessor como una lista sencilla de controles deslizantes, cuadros combinados e interruptores. | [`higgs_GenericAudioProcessorEditor.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_processors/processors/higgs_GenericAudioProcessorEditor.h) |

### `scanning`

| Clase | Descripción | Cabecera |
|---|---|---|
| `KnownPluginList` | Administra una lista de tipos de plug-in. | [`higgs_KnownPluginList.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_processors/scanning/higgs_KnownPluginList.h) |
| `PluginDirectoryScanner` | Examina un directorio en busca de plug-ins y los agrega a una KnownPluginList. | [`higgs_PluginDirectoryScanner.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_processors/scanning/higgs_PluginDirectoryScanner.h) |
| `PluginListComponent` | Un componente que muestra una lista de plug-ins, con opciones para escanearlos, agregarlos, eliminarlos y ordenarlos. | [`higgs_PluginListComponent.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_processors/scanning/higgs_PluginListComponent.h) |

### `format_types`

| Clase | Descripción | Cabecera |
|---|---|---|
| `AudioUnitPluginFormat` | Implementa un administrador de formatos de plug-in para AudioUnits. | [`higgs_AudioUnitPluginFormat.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_processors/format_types/higgs_AudioUnitPluginFormat.h) |
| `LADSPAPluginFormat` | Se proporciona por compatibilidad con versiones anteriores; los plug-ins LADSPA siempre carecen de interfaz gráfica. | [`higgs_LADSPAPluginFormat.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_processors/format_types/higgs_LADSPAPluginFormat.h) |
| `LV2PluginFormat` | Implementa un formato de plug-in para plug-ins LV2. | [`higgs_LV2PluginFormat.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_processors/format_types/higgs_LV2PluginFormat.h) |
| `VST3PluginFormat` | Implementa un formato de plug-in para VST3. | [`higgs_VST3PluginFormat.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_processors/format_types/higgs_VST3PluginFormat.h) |
| `VSTPluginFormat` | Implementa un administrador de formatos de plug-in para VST. | [`higgs_VSTPluginFormat.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_processors/format_types/higgs_VSTPluginFormat.h) |

### `utilities`

| Clase | Descripción | Cabecera |
|---|---|---|
| `ParameterAttachment` | Se usa para implementar 'attachments' o 'controladores' que vinculan un parámetro de plug-in con un elemento de la interfaz de usuario. | [`higgs_ParameterAttachments.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_processors/utilities/higgs_ParameterAttachments.h) |
| `SliderParameterAttachment` | Un objeto de esta clase mantiene una conexión entre un Slider y un parámetro de plug-in. | [`higgs_ParameterAttachments.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_processors/utilities/higgs_ParameterAttachments.h) |
| `ComboBoxParameterAttachment` | Un objeto de esta clase mantiene una conexión entre un ComboBox y un parámetro de plug-in. | [`higgs_ParameterAttachments.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_processors/utilities/higgs_ParameterAttachments.h) |
| `ButtonParameterAttachment` | Un objeto de esta clase mantiene una conexión entre un Button y un parámetro de plug-in. | [`higgs_ParameterAttachments.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_processors/utilities/higgs_ParameterAttachments.h) |
| `WebSliderParameterAttachment` | Un objeto de esta clase mantiene una conexión entre un WebSliderRelay y un parámetro de plug-in. | [`higgs_ParameterAttachments.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_processors/utilities/higgs_ParameterAttachments.h) |
| `WebToggleButtonParameterAttachment` | Un objeto de esta clase mantiene una conexión entre un WebToggleButtonRelay y un parámetro de plug-in. | [`higgs_ParameterAttachments.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_processors/utilities/higgs_ParameterAttachments.h) |
| `WebComboBoxParameterAttachment` | Un objeto de esta clase mantiene una conexión entre un WebComboBoxRelay y un parámetro de plug-in. | [`higgs_ParameterAttachments.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_processors/utilities/higgs_ParameterAttachments.h) |
| `AudioProcessorValueTreeStateParameterAttributes` | Propiedades avanzadas de un AudioProcessorValueTreeState::Parameter. | [`higgs_AudioProcessorValueTreeState.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_processors/utilities/higgs_AudioProcessorValueTreeState.h) |
| `AudioProcessorValueTreeState` | Esta clase contiene un ValueTree que se usa para administrar todo el estado de un AudioProcessor. | [`higgs_AudioProcessorValueTreeState.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_processors/utilities/higgs_AudioProcessorValueTreeState.h) |
| `PluginHostType` | Una clase utilitaria práctica para determinar el host o DAW en el que está cargado el plug-in. | [`higgs_PluginHostType.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_processors/utilities/higgs_PluginHostType.h) |
