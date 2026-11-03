# higgs_gui_extra

Clases de interfaz para tareas especializadas: editor de código con resaltado de sintaxis, `WebBrowserComponent` con integración nativa JavaScript/C++, selector de colores, editor de atajos, paneles de preferencias, icono en la bandeja del sistema, notificaciones push, editor de constantes en caliente y utilidades de documentos.

## Ficha

| Campo | Valor |
|---|---|
| Identificador | `higgs_gui_extra` |
| Versión | 9.0.5 |
| Nombre declarado | Higgs Mythos extended GUI classes |
| Descripción declarada | Miscellaneous GUI classes for specialised tasks. |
| Estándar mínimo de C++ | C++17 |
| Licencia | AGPLv3 |
| Dependencias directas | [`higgs_gui_basics`](higgs_gui_basics.md) |
| Dependencias indirectas | [`higgs_core`](higgs_core.md), [`higgs_events`](higgs_events.md), [`higgs_data_structures`](higgs_data_structures.md), [`higgs_graphics`](higgs_graphics.md) |
| Módulos que dependen de él | [`higgs_opengl`](higgs_opengl.md), [`higgs_video`](higgs_video.md), [`higgs_audio_processors`](higgs_audio_processors.md) |
| Frameworks de macOS | `WebKit` |
| Frameworks débiles de macOS | `UserNotifications` |
| Frameworks de iOS | `WebKit` |
| Frameworks débiles de iOS | `UserNotifications` |
| Código fuente | [`modules/higgs_gui_extra`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_extra) |

## Uso

```cmake
target_link_libraries(MiTarget PRIVATE higgs::higgs_gui_extra)
```

```cpp
#include <higgs_gui_extra/higgs_gui_extra.h>
```

Guías relacionadas: [Interfaces web (WebView)](../guias/web.md), [Interfaz gráfica](../guias/interfaz-grafica.md).

## Opciones de configuración

Se cambian con `target_compile_definitions(MiTarget PUBLIC OPCION=valor)` o en la sección *Modules* de Mythos Studio.

| Opción | Por defecto | Descripción |
|---|---|---|
| `HIGGS_WEB_BROWSER` | `1` | Esta opción permite deshabilitar la clase `WebBrowserComponent`. Si no usa páginas web incrustadas, desactivarla puede reducir el tamaño de su código. |
| `HIGGS_USE_WIN_WEBVIEW2_WITH_STATIC_LINKING` | `0` | Habilita el uso del navegador WebView2 de Microsoft Edge (Chromium) en Windows. Si usa Mythos Studio, el paquete `Microsoft.Web.WebView2` se agregará a la solución del proyecto al habilitar este indicador. Si compila con CMake, deberá agregar el paquete manualmente mediante el administrador de paquetes NuGet. Usar este indicador requiere enlazar estáticamente con `WebView2LoaderStatic.lib`, que por ahora solo está disponible a través del paquete NuGet y no se encuentra en VCPKG. Además de habilitar esta macro, deberá usar la opción `WebBrowserComponent::Options::Backend::webview2` al crear la instancia de `WebBrowserComponent`. |
| `HIGGS_USE_WIN_WEBVIEW2` | `0` | Habilita el uso del navegador WebView2 de Microsoft Edge (Chromium) en Windows. Si usa Mythos Studio, el paquete `Microsoft.Web.WebView2` se agregará a la solución del proyecto al habilitar este indicador. Si compila con CMake, deberá agregar el paquete manualmente mediante el administrador de paquetes de Visual Studio. Si también está establecida la variante `WITH_STATIC_LINKING` de este indicador, debe enlazar estáticamente con `WebView2LoaderStatic.lib`; de lo contrario, se usará la carga dinámica. Encontrará más información sobre el enlazado dinámico en la documentación de `WebBrowserComponent::Options::WinWebView2::withDLLLocation()`. Además de habilitar esta macro, deberá usar la opción `WebBrowserComponent::Options::Backend::webview2` al crear la instancia de `WebBrowserComponent`. |
| `HIGGS_ENABLE_LIVE_CONSTANT_EDITOR` | 1 en depuración (solo escritorio) | Permite activar la compatibilidad con `HIGGS_ENABLE_LIVE_CONSTANT_EDITOR` (solo en escritorio). De forma predeterminada, estará habilitada en las compilaciones de depuración y deshabilitada en las compilaciones de release. Consulte la documentación de esa macro para obtener más detalles. |

## Código de terceros incluido

| Componente | Licencia | Ubicación |
|---|---|---|
| [@jalexiscv/webview](https://github.com/jalexiscv/higgs-mythos) | `AGPL-3.0-or-later` | `modules/higgs_gui_extra/native/typescript/webview-interop` |

## Catálogo de clases

29 clases y estructuras públicas declaradas en las cabeceras que incluye `higgs_gui_extra.h`, agrupadas por carpeta. La descripción es la primera frase de su comentario de documentación en el código.

### `documents`

| Clase | Descripción | Cabecera |
|---|---|---|
| `FileBasedDocument` | Una clase que se encarga de la lógica relacionada con la carga y el guardado de algún tipo de documento. | [`higgs_FileBasedDocument.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_extra/documents/higgs_FileBasedDocument.h) |

### `code_editor`

| Clase | Descripción | Cabecera |
|---|---|---|
| `CodeDocument` | Una clase para almacenar y manipular un archivo de código fuente. | [`higgs_CodeDocument.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_extra/code_editor/higgs_CodeDocument.h) |
| `CodeEditorComponent` | Un componente de editor de texto diseñado específicamente para código fuente. | [`higgs_CodeEditorComponent.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_extra/code_editor/higgs_CodeEditorComponent.h) |
| `CodeTokeniser` | Una clase base para tokenizar código de modo que su sintaxis pueda mostrarse en un editor de código. | [`higgs_CodeTokeniser.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_extra/code_editor/higgs_CodeTokeniser.h) |
| `CPlusPlusCodeTokeniser` | Analizador léxico sencillo para el coloreado de sintaxis de código C++. | [`higgs_CPlusPlusCodeTokeniser.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_extra/code_editor/higgs_CPlusPlusCodeTokeniser.h) |
| `CppTokeniserFunctions` | Clase que contiene algunas funciones básicas para la tokenización simple de código C++. | [`higgs_CPlusPlusCodeTokeniserFunctions.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_extra/code_editor/higgs_CPlusPlusCodeTokeniserFunctions.h) |

### `embedding`

| Clase | Descripción | Cabecera |
|---|---|---|
| `ActiveXControlComponent` | Una clase específica de Windows que puede crear e incrustar un control ActiveX dentro de sí misma. | [`higgs_ActiveXControlComponent.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_extra/embedding/higgs_ActiveXControlComponent.h) |
| `AndroidViewComponent` | Clase específica de Android que puede crear e incrustar una View dentro de sí misma. | [`higgs_AndroidViewComponent.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_extra/embedding/higgs_AndroidViewComponent.h) |
| `NSViewComponent` | Una clase específica de Mac que puede crear e incrustar un NSView dentro de sí misma. | [`higgs_NSViewComponent.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_extra/embedding/higgs_NSViewComponent.h) |
| `UIViewComponent` | Una clase específica de iOS que puede crear e incrustar una UIView dentro de sí misma. | [`higgs_UIViewComponent.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_extra/embedding/higgs_UIViewComponent.h) |
| `XEmbedComponentOptions` | Opciones para construir un XEmbedComponent. | [`higgs_XEmbedComponent.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_extra/embedding/higgs_XEmbedComponent.h) |
| `XEmbedComponent` | Clase específica de Linux que puede incrustar un widget X11 externo. | [`higgs_XEmbedComponent.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_extra/embedding/higgs_XEmbedComponent.h) |
| `HWNDComponent` | Una clase específica de Windows que puede crear e incrustar un HWND dentro de sí misma. | [`higgs_HWNDComponent.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_extra/embedding/higgs_HWNDComponent.h) |

### `misc`

| Clase | Descripción | Cabecera |
|---|---|---|
| `AppleRemoteDevice` | Recibe eventos de un dispositivo de control remoto IR de Apple (¡solo disponible en OSX!). | [`higgs_AppleRemote.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_extra/misc/higgs_AppleRemote.h) |
| `BubbleMessageComponent` | Un componente de globo de diálogo que muestra un mensaje breve. | [`higgs_BubbleMessageComponent.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_extra/misc/higgs_BubbleMessageComponent.h) |
| `ColourSelector` | Componente que permite al usuario elegir un color. | [`higgs_ColourSelector.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_extra/misc/higgs_ColourSelector.h) |
| `KeyMappingEditorComponent` | Un componente que permite editar los mapas de teclas almacenados por un objeto KeyPressMappingSet. | [`higgs_KeyMappingEditorComponent.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_extra/misc/higgs_KeyMappingEditorComponent.h) |
| `PreferencesPanel` | Un componente con un conjunto de botones en la parte superior para cambiar entre páginas de preferencias. | [`higgs_PreferencesPanel.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_extra/misc/higgs_PreferencesPanel.h) |
| `PushNotifications` | Clase singleton responsable de la funcionalidad de notificaciones push. | [`higgs_PushNotifications.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_extra/misc/higgs_PushNotifications.h) |
| `RecentlyOpenedFilesList` | Administra un conjunto de archivos para usarlos como lista de documentos abiertos recientemente. | [`higgs_RecentlyOpenedFilesList.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_extra/misc/higgs_RecentlyOpenedFilesList.h) |
| `SplashScreen` | Un componente para mostrar una pantalla de presentación mientras se inicia la aplicación. | [`higgs_SplashScreen.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_extra/misc/higgs_SplashScreen.h) |
| `SystemTrayIconComponent` | Este componente se ubica en la bandeja de la barra de tareas como un pequeño ícono. | [`higgs_SystemTrayIconComponent.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_extra/misc/higgs_SystemTrayIconComponent.h) |
| `WebViewLifetimeListener` | Tipo para un listener registrado mediante WebBrowserComponent::Options::withWebViewLifetimeListener. | [`higgs_WebBrowserComponent.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_extra/misc/higgs_WebBrowserComponent.h) |
| `WebBrowserComponent` | Componente que muestra un navegador web incrustado. | [`higgs_WebBrowserComponent.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_extra/misc/higgs_WebBrowserComponent.h) |
| `AnimatedAppComponent` | Una clase base para escribir aplicaciones gráficas simples de una sola página. | [`higgs_AnimatedAppComponent.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_extra/misc/higgs_AnimatedAppComponent.h) |
| `WebSliderRelay` | Clase auxiliar que transmite información de parámetros de audio a un objeto dentro de un WebBrowserComponent. | [`higgs_WebControlRelays.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_extra/misc/higgs_WebControlRelays.h) |
| `WebToggleButtonRelay` | Clase auxiliar que transmite información de parámetros de audio a un objeto dentro de un WebBrowserComponent. | [`higgs_WebControlRelays.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_extra/misc/higgs_WebControlRelays.h) |
| `WebComboBoxRelay` | Clase auxiliar que transmite información de parámetros de audio a un objeto dentro de un WebBrowserComponent. | [`higgs_WebControlRelays.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_extra/misc/higgs_WebControlRelays.h) |
| `WebControlParameterIndexReceiver` | Esta es una clase auxiliar para implementar AudioProcessorEditor::getControlParameterIndex con interfaces gráficas que usan un WebBrowserComponent. | [`higgs_WebControlParameterIndexReceiver.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_extra/misc/higgs_WebControlParameterIndexReceiver.h) |
