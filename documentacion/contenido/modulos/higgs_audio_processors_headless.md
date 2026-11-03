# higgs_audio_processors_headless

La parte sin interfaz del sistema de procesadores: `AudioProcessor`, parámetros y grupos, `AudioProcessorGraph`, `AudioPluginInstance`, formatos de alojamiento (VST3, VST2, AU, LV2, LADSPA, ARA) y extensiones de cliente por formato. Permite cargar y ejecutar plug-ins en herramientas sin interfaz gráfica.

## Ficha

| Campo | Valor |
|---|---|
| Identificador | `higgs_audio_processors_headless` |
| Versión | 9.0.5 |
| Nombre declarado | Higgs Mythos audio processor classes without UI |
| Descripción declarada | Classes for loading and playing VST, AU, LADSPA, or internally-generated audio processors without UI. |
| Estándar mínimo de C++ | C++17 |
| Licencia | AGPLv3 |
| Dependencias directas | [`higgs_audio_basics`](higgs_audio_basics.md), [`higgs_events`](higgs_events.md) |
| Dependencias indirectas | [`higgs_core`](higgs_core.md) |
| Módulos que dependen de él | [`higgs_audio_processors`](higgs_audio_processors.md) |
| Frameworks de macOS | `CoreAudio`, `CoreMIDI`, `AudioToolbox` |
| Frameworks de iOS | `AudioToolbox` |
| Código fuente | [`modules/higgs_audio_processors_headless`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_processors_headless) |

## Uso

```cmake
target_link_libraries(MiTarget PRIVATE higgs::higgs_audio_processors_headless)
```

```cpp
#include <higgs_audio_processors_headless/higgs_audio_processors_headless.h>
```

Guías relacionadas: [Plug-ins de audio](../guias/plugins.md), [Alojar plug-ins (hosting)](../guias/hosting.md).

## Opciones de configuración

Se cambian con `target_compile_definitions(MiTarget PUBLIC OPCION=valor)` o en la sección *Modules* de Mythos Studio.

| Opción | Por defecto | Descripción |
|---|---|---|
| `HIGGS_PLUGINHOST_VST` | `0` | Habilita las clases de alojamiento de plug-ins de audio VST. Necesitará tener los archivos del VST2 SDK en sus rutas de búsqueda de encabezados. Puede obtener los archivos del VST2 SDK de una versión anterior del VST3 SDK. Véase también: `VSTPluginFormat`, `VST3PluginFormat`, `AudioPluginFormat`, `AudioPluginFormatManager`, `HIGGS_PLUGINHOST_AU`, `HIGGS_PLUGINHOST_VST3`, `HIGGS_PLUGINHOST_LADSPA`. |
| `HIGGS_PLUGINHOST_VST3` | `0` | Habilita las clases de alojamiento de plug-ins de audio VST3. Véase también: `VSTPluginFormat`, `VST3PluginFormat`, `AudioPluginFormat`, `AudioPluginFormatManager`, `HIGGS_PLUGINHOST_VST`, `HIGGS_PLUGINHOST_AU`, `HIGGS_PLUGINHOST_LADSPA`. |
| `HIGGS_PLUGINHOST_AU` | `0` | Habilita las clases de alojamiento de plug-ins AudioUnit. Obviamente, solo está disponible en Mac. Véase también: `AudioUnitPluginFormat`, `AudioPluginFormat`, `AudioPluginFormatManager`, `HIGGS_PLUGINHOST_VST`, `HIGGS_PLUGINHOST_VST3`, `HIGGS_PLUGINHOST_LADSPA`. |
| `HIGGS_PLUGINHOST_LADSPA` | `0` | Habilita las clases de alojamiento de plug-ins LADSPA. Obviamente, solo está disponible en Linux. Véase también: `LADSPAPluginFormat`, `AudioPluginFormat`, `AudioPluginFormatManager`, `HIGGS_PLUGINHOST_VST`, `HIGGS_PLUGINHOST_VST3`, `HIGGS_PLUGINHOST_AU`. |
| `HIGGS_PLUGINHOST_LV2` | `0` | Habilita las clases de alojamiento de plug-ins LV2. |
| `HIGGS_PLUGINHOST_ARA` | `0` | Habilita las clases de alojamiento de la extensión de plug-ins ARA. Deberá descargar el ARA SDK y especificar su ruta, ya sea en Mythos Studio o mediante `higgs_set_ara_sdk_path()` en el archivo de su proyecto CMake. El directorio puede obtenerse clonando recursivamente https://github.com/Celemony/ARA_SDK y haciendo checkout de la etiqueta `releases/2.1.0`. |
| `HIGGS_CUSTOM_VST3_SDK` | `0` | Si se habilita, el VST3 SDK integrado en HIGGS no se agregará al proyecto y, en su lugar, deberá agregar la ruta a su VST3 SDK personalizado a las rutas de búsqueda de encabezados del proyecto. La mayoría de los usuarios no debería necesitar habilitar esta opción y simplemente debería usar la versión del SDK incluida con HIGGS. |

## Código de terceros incluido

| Componente | Licencia | Ubicación |
|---|---|---|
| [VST3 SDK](https://github.com/steinbergmedia/vst3sdk) | `MIT` | `modules/higgs_audio_processors_headless/format_types/VST3_SDK` |
| [LV2](https://lv2plug.in) | `ISC` | `modules/higgs_audio_processors_headless/format_types/LV2_SDK/lv2` |
| [lilv](https://drobilla.net/software/lilv.html) | `ISC` | `modules/higgs_audio_processors_headless/format_types/LV2_SDK/lilv` |
| [serd](https://drobilla.net/software/serd.html) | `ISC` | `modules/higgs_audio_processors_headless/format_types/LV2_SDK/serd` |
| [sord](https://drobilla.net/software/sord.html) | `ISC` | `modules/higgs_audio_processors_headless/format_types/LV2_SDK/sord` |
| [sratom](https://drobilla.net/software/sratom.html) | `ISC` | `modules/higgs_audio_processors_headless/format_types/LV2_SDK/sratom` |
| pslextensions | `LicenseRef-Public-Domain` | `modules/higgs_audio_processors_headless/format_types/pslextensions` |

## Catálogo de clases

45 clases y estructuras públicas declaradas en las cabeceras que incluye `higgs_audio_processors_headless.h`, agrupadas por carpeta. La descripción es la primera frase de su comentario de documentación en el código.

### `processors`

| Clase | Descripción | Cabecera |
|---|---|---|
| `AudioProcessorListener` | Clase base para listeners que desean enterarse de los cambios en un AudioProcessor. | [`higgs_AudioProcessorListener.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_processors_headless/processors/higgs_AudioProcessorListener.h) |
| `AudioProcessorParameter` | Una clase base abstracta para objetos de parámetro que pueden agregarse a un AudioProcessor. | [`higgs_AudioProcessorParameter.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_processors_headless/processors/higgs_AudioProcessorParameter.h) |
| `HostedAudioProcessorParameter` | Un parámetro con funciones útiles para los hosts de plug-ins. | [`higgs_HostedAudioProcessorParameter.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_processors_headless/processors/higgs_HostedAudioProcessorParameter.h) |
| `AudioProcessorParameterGroup` | Una clase que encapsula un grupo de AudioProcessorParameters y AudioProcessorParameterGroups anidados. | [`higgs_AudioProcessorParameterGroup.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_processors_headless/processors/higgs_AudioProcessorParameterGroup.h) |
| `AudioProcessor` | Clase base para clases de procesamiento de audio o plug-ins. | [`higgs_AudioProcessor.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_processors_headless/processors/higgs_AudioProcessor.h) |
| `PluginDescription` | Una clase pequeña para representar algunos datos sobre un tipo concreto de plug-in. | [`higgs_PluginDescription.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_processors_headless/processors/higgs_PluginDescription.h) |
| `AudioProcessorGraph` | Un tipo de AudioProcessor que reproduce un grafo de otros AudioProcessors. | [`higgs_AudioProcessorGraph.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_processors_headless/processors/higgs_AudioProcessorGraph.h) |

### `utilities`

| Clase | Descripción | Cabecera |
|---|---|---|
| `AAXClientExtensions` | Interfaz que permite a un AudioProcessor implementar funcionalidad extendida específica de AAX. | [`higgs_AAXClientExtensions.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_processors_headless/utilities/higgs_AAXClientExtensions.h) |
| `VST2ClientExtensions` | Una interfaz que permite a un AudioProcessor implementar funcionalidad extendida específica de VST2. | [`higgs_VST2ClientExtensions.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_processors_headless/utilities/higgs_VST2ClientExtensions.h) |
| `VST3Interface` | Funciones y clases útiles para definir identificadores de interfaz de VST3. | [`higgs_VST3Interface.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_processors_headless/utilities/higgs_VST3Interface.h) |
| `VST3ClientExtensions` | Una interfaz que permite a un AudioProcessor implementar funcionalidad extendida específica de VST3. | [`higgs_VST3ClientExtensions.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_processors_headless/utilities/higgs_VST3ClientExtensions.h) |
| `AudioPluginExtensions` | Una colección de interfaces para interactuar con plug-ins alojados de forma específica para cada formato de plug-in. | [`higgs_AudioPluginExtensions.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_processors_headless/utilities/higgs_AudioPluginExtensions.h) |
| `ParameterID` | Combina un ID de parámetro y una sugerencia de versión. | [`higgs_AudioProcessorParameterWithID.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_processors_headless/utilities/higgs_AudioProcessorParameterWithID.h) |
| `AudioProcessorParameterWithIDAttributes` | Se puede pasar una instancia de esta clase al constructor de un AudioProcessorParameterWithID para establecer características opcionales de ese parámetro. | [`higgs_AudioProcessorParameterWithID.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_processors_headless/utilities/higgs_AudioProcessorParameterWithID.h) |
| `AudioProcessorParameterWithID` | Esta clase base abstracta es utilizada por algunas clases auxiliares de AudioProcessorParameter. | [`higgs_AudioProcessorParameterWithID.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_processors_headless/utilities/higgs_AudioProcessorParameterWithID.h) |
| `RangedAudioParameter` | Esta clase base abstracta es utilizada por algunas clases auxiliares de AudioProcessorParameter. | [`higgs_RangedAudioParameter.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_processors_headless/utilities/higgs_RangedAudioParameter.h) |
| `AudioParameterFloatAttributes` | Propiedades de un AudioParameterFloat. | [`higgs_AudioParameterFloat.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_processors_headless/utilities/higgs_AudioParameterFloat.h) |
| `AudioParameterFloat` | Subclase de AudioProcessorParameter que ofrece una forma sencilla de crear un parámetro que se asigna a un NormalisableRange dado. | [`higgs_AudioParameterFloat.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_processors_headless/utilities/higgs_AudioParameterFloat.h) |
| `AudioParameterIntAttributes` | Propiedades de un AudioParameterInt. | [`higgs_AudioParameterInt.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_processors_headless/utilities/higgs_AudioParameterInt.h) |
| `AudioParameterInt` | Proporciona una clase de AudioProcessorParameter que puede usarse como un valor entero con un rango determinado. | [`higgs_AudioParameterInt.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_processors_headless/utilities/higgs_AudioParameterInt.h) |
| `AudioParameterBoolAttributes` | Propiedades de un AudioParameterBool. | [`higgs_AudioParameterBool.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_processors_headless/utilities/higgs_AudioParameterBool.h) |
| `AudioParameterBool` | Proporciona una clase de AudioProcessorParameter que se puede usar como valor booleano. | [`higgs_AudioParameterBool.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_processors_headless/utilities/higgs_AudioParameterBool.h) |
| `AudioParameterChoiceAttributes` | Propiedades de un AudioParameterChoice. | [`higgs_AudioParameterChoice.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_processors_headless/utilities/higgs_AudioParameterChoice.h) |
| `AudioParameterChoice` | Proporciona una clase de AudioProcessorParameter que se puede usar para seleccionar una opción indexada y con nombre de una lista. | [`higgs_AudioParameterChoice.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_processors_headless/utilities/higgs_AudioParameterChoice.h) |

### `format_types`

| Clase | Descripción | Cabecera |
|---|---|---|
| `ARAFactoryWrapper` | Encapsula un puntero a ARAFactory y garantiza que permanezca en un estado válido durante la vida útil del objeto ARAFactoryWrapper. | [`higgs_ARACommon.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_processors_headless/format_types/higgs_ARACommon.h) |
| `ARAFactoryResult` | Representa el resultado de AudioPluginFormatManager::createARAFactoryAsync(). | [`higgs_ARACommon.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_processors_headless/format_types/higgs_ARACommon.h) |
| `AudioUnitPluginFormatHeadless` | Implementa un administrador de formatos de plug-in para AudioUnits. | [`higgs_AudioUnitPluginFormatHeadless.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_processors_headless/format_types/higgs_AudioUnitPluginFormatHeadless.h) |
| `LADSPAPluginFormatHeadless` | Implementa un administrador de formatos de plug-in para plug-ins LADSPA. | [`higgs_LADSPAPluginFormatHeadless.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_processors_headless/format_types/higgs_LADSPAPluginFormatHeadless.h) |
| `LV2PluginFormatHeadless` | Implementa un formato de plug-in para plug-ins LV2. | [`higgs_LV2PluginFormatHeadless.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_processors_headless/format_types/higgs_LV2PluginFormatHeadless.h) |
| `VST3PluginFormatHeadless` | Implementa un formato de plug-in para VST3. | [`higgs_VST3PluginFormatHeadless.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_processors_headless/format_types/higgs_VST3PluginFormatHeadless.h) |
| `VSTPluginFormatHeadless` | Implementa un administrador de formatos de plug-in para VST. | [`higgs_VSTPluginFormatHeadless.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_processors_headless/format_types/higgs_VSTPluginFormatHeadless.h) |
| `ARAEditGuard` | Clase auxiliar de conteo de referencias para garantizar que el DocumentController esté en estado editable. | [`higgs_ARAHosting.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_processors_headless/format_types/higgs_ARAHosting.h) |
| `ConversionFunctions` | Permite convertir, sin advertencias, entre punteros de dos tipos no relacionados. | [`higgs_ARAHosting.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_processors_headless/format_types/higgs_ARAHosting.h) |
| `ManagedARAHandle` | Esta clase la usan las diversas clases auxiliares de objetos del modelo ARA, como MusicalContext, AudioSource, etc. | [`higgs_ARAHosting.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_processors_headless/format_types/higgs_ARAHosting.h) |
| `AudioSource` | Clase auxiliar para la implementación del lado del host del objeto de modelo %ARA %AudioSource. | [`higgs_ARAHosting.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_processors_headless/format_types/higgs_ARAHosting.h) |
| `AudioModification` | Clase auxiliar para la implementación del lado del host del objeto de modelo %ARA %AudioModification. | [`higgs_ARAHosting.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_processors_headless/format_types/higgs_ARAHosting.h) |
| `DeletionListener` | PlaybackRegionRegistry usa esta clase internamente para recibir notificaciones cuando se elimina un objeto PlaybackRegion. | [`higgs_ARAHosting.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_processors_headless/format_types/higgs_ARAHosting.h) |
| `PlaybackRegion` | Clase auxiliar para la implementación del lado del host del objeto de modelo %ARA %PlaybackRegion. | [`higgs_ARAHosting.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_processors_headless/format_types/higgs_ARAHosting.h) |
| `MusicalContext` | Clase auxiliar para la implementación del lado del host del objeto de modelo %ARA %MusicalContext. | [`higgs_ARAHosting.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_processors_headless/format_types/higgs_ARAHosting.h) |
| `RegionSequence` | Clase auxiliar para la implementación del lado del host del objeto de modelo %ARA %RegionSequence. | [`higgs_ARAHosting.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_processors_headless/format_types/higgs_ARAHosting.h) |
| `PlaybackRegionRegistry` | Clase base utilizada por las interfaces de extensión de plug-in ::PlaybackRendererInterface y ::EditorRendererInterface. | [`higgs_ARAHosting.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_processors_headless/format_types/higgs_ARAHosting.h) |
| `PlugInExtensionInstance` | Clase contenedora para `ARA::ARAPlugInExtensionInstance*`. | [`higgs_ARAHosting.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_processors_headless/format_types/higgs_ARAHosting.h) |
| `ARAHostDocumentController` | Clase envoltorio para `ARA::Host::DocumentController`. | [`higgs_ARAHosting.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_processors_headless/format_types/higgs_ARAHosting.h) |

### `format`

| Clase | Descripción | Cabecera |
|---|---|---|
| `AudioPluginFormat` | Clase base para un tipo de formato de plug-in, como VST, AudioUnit, LADSPA, etc. | [`higgs_AudioPluginFormat.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_processors_headless/format/higgs_AudioPluginFormat.h) |
| `AudioPluginFormatManager` | Mantiene una lista de AudioPluginFormats conocidos. | [`higgs_AudioPluginFormatManager.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_audio_processors_headless/format/higgs_AudioPluginFormatManager.h) |
