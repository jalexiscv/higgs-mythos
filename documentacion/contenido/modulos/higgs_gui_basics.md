# higgs_gui_basics

Las clases fundamentales de la interfaz de usuario: `Component`, ventanas, ratón, teclado y táctil, *widgets* (botones, deslizadores, etiquetas, editores de texto, listas, tablas, árboles), menús, *layouts* (`FlexBox`, `Grid`), *look-and-feel*, comandos, selectores de archivos, diálogos y accesibilidad.

## Ficha

| Campo | Valor |
|---|---|
| Identificador | `higgs_gui_basics` |
| Versión | 9.0.5 |
| Nombre declarado | Higgs Mythos GUI core classes |
| Descripción declarada | Basic user-interface components and related classes. |
| Estándar mínimo de C++ | C++17 |
| Licencia | AGPLv3 |
| Dependencias directas | [`higgs_graphics`](higgs_graphics.md), [`higgs_data_structures`](higgs_data_structures.md) |
| Dependencias indirectas | [`higgs_core`](higgs_core.md), [`higgs_events`](higgs_events.md) |
| Módulos que dependen de él | [`higgs_gui_extra`](higgs_gui_extra.md), [`higgs_animation`](higgs_animation.md), [`higgs_analytics`](higgs_analytics.md) |
| Frameworks de macOS | `Cocoa`, `QuartzCore` |
| Frameworks débiles de macOS | `Metal`, `MetalKit` |
| Frameworks de iOS | `CoreServices`, `UIKit` |
| Frameworks débiles de iOS | `Metal`, `MetalKit`, `UniformTypeIdentifiers`, `UserNotifications` |
| Código fuente | [`modules/higgs_gui_basics`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics) |

## Uso

```cmake
target_link_libraries(MiTarget PRIVATE higgs::higgs_gui_basics)
```

```cpp
#include <higgs_gui_basics/higgs_gui_basics.h>
```

Guías relacionadas: [Interfaz gráfica](../guias/interfaz-grafica.md), [Accesibilidad](../guias/accesibilidad.md).

## Opciones de configuración

Se cambian con `target_compile_definitions(MiTarget PUBLIC OPCION=valor)` o en la sección *Modules* de Mythos Studio.

| Opción | Por defecto | Descripción |
|---|---|---|
| `HIGGS_ENABLE_REPAINT_DEBUGGING` | `0` | Si esta opción está activada, cada área de la pantalla que se vuelva a pintar parpadeará con un color aleatorio, para que pueda ver exactamente qué partes de sus componentes se están dibujando. |
| `HIGGS_USE_XRANDR` | `1` | Habilita la compatibilidad con varios monitores mediante Xrandr (solo Linux). A menos que desee deshabilitarla específicamente, es mejor dejar esta opción activada. Tenga en cuenta que sus usuarios no necesitan tener Xrandr instalado para que su aplicación HIGGS se ejecute, ya que la disponibilidad de Xrandr se consulta en tiempo de ejecución. |
| `HIGGS_USE_XINERAMA` | `1` | Habilita la compatibilidad con varios monitores mediante Xinerama (solo Linux). A menos que desee deshabilitarla específicamente, es mejor dejar esta opción activada. Se usará como alternativa si `HIGGS_USE_XRANDR` no está establecido o si no se encuentra `libxrandr`. Tenga en cuenta que sus usuarios no necesitan tener Xinerama instalado para que su aplicación HIGGS se ejecute, ya que la disponibilidad de Xinerama se consulta en tiempo de ejecución. |
| `HIGGS_USE_XSHM` | `1` | Habilita la memoria compartida de X para un renderizado más rápido en Linux. Es mejor dejar esta opción activada a menos que tenga una buena razón para deshabilitarla. |
| `HIGGS_USE_XRENDER` | `0` | Habilita XRender para permitir ventanas semitransparentes en Linux. |
| `HIGGS_USE_XCURSOR` | `1` | Usa XCursor para permitir cursores ARGB en Linux. Es mejor dejar esta opción activada a menos que tenga una buena razón para deshabilitarla. |
| `HIGGS_USE_XINPUT` | `1` | Usa XInput (v2.2+) para permitir la entrada multitáctil en Linux. Es mejor dejar esta opción activada a menos que tenga una buena razón para deshabilitarla. La disponibilidad de XInput v2.2+ se consulta en tiempo de ejecución, por lo que sus usuarios no necesitan tener XInput instalado para que su aplicación HIGGS se ejecute. |
| `HIGGS_WIN_PER_MONITOR_DPI_AWARE` | `1` | Habilita el reconocimiento de DPI por monitor en Windows 8.1 y posteriores. |

## Catálogo de clases

167 clases y estructuras públicas declaradas en las cabeceras que incluye `higgs_gui_basics.h`, agrupadas por carpeta. La descripción es la primera frase de su comentario de documentación en el código.

### `mouse`

| Clase | Descripción | Cabecera |
|---|---|---|
| `MouseCursor` | Representa una imagen de cursor del mouse. | [`higgs_MouseCursor.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/mouse/higgs_MouseCursor.h) |
| `MouseListener` | Un MouseListener se puede registrar en un componente para recibir callbacks sobre los eventos del mouse que ocurren en ese componente. | [`higgs_MouseListener.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/mouse/higgs_MouseListener.h) |
| `MouseInputSource` | Representa una fuente lineal de eventos de mouse proveniente de un dispositivo de mouse o de un dedo individual en un entorno multitáctil. | [`higgs_MouseInputSource.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/mouse/higgs_MouseInputSource.h) |
| `MouseEvent` | Contiene información de posición y estado sobre un evento del mouse. | [`higgs_MouseEvent.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/mouse/higgs_MouseEvent.h) |
| `MouseWheelDetails` | Contiene información de estado sobre un evento de la rueda del mouse. | [`higgs_MouseEvent.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/mouse/higgs_MouseEvent.h) |
| `PenDetails` | Contiene información de estado sobre un evento de lápiz. | [`higgs_MouseEvent.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/mouse/higgs_MouseEvent.h) |
| `ComponentDragger` | Un objeto que se encarga de la lógica para arrastrar componentes con el mouse. | [`higgs_ComponentDragger.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/mouse/higgs_ComponentDragger.h) |
| `DragAndDropTarget` | Los componentes derivados de esta clase pueden recibir elementos soltados sobre ellos por un DragAndDropContainer. | [`higgs_DragAndDropTarget.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/mouse/higgs_DragAndDropTarget.h) |
| `DragAndDropContainer` | Habilita el comportamiento de arrastrar y soltar para un componente y todos sus subcomponentes. | [`higgs_DragAndDropContainer.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/mouse/higgs_DragAndDropContainer.h) |
| `FileDragAndDropTarget` | Los componentes derivados de esta clase pueden recibir archivos soltados sobre ellos desde una aplicación externa. | [`higgs_FileDragAndDropTarget.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/mouse/higgs_FileDragAndDropTarget.h) |
| `SelectedItemSet` | Administra una lista de elementos seleccionables. | [`higgs_SelectedItemSet.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/mouse/higgs_SelectedItemSet.h) |
| `MouseInactivityDetector` | Este objeto vigila los eventos del mouse que ocurren dentro de un componente y, si el mouse permanece quieto el tiempo suficiente, dispara un evento para indicar que se ha quedado inactivo. | [`higgs_MouseInactivityDetector.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/mouse/higgs_MouseInactivityDetector.h) |
| `TextDragAndDropTarget` | Los componentes derivados de esta clase pueden recibir texto soltado sobre ellos desde una aplicación externa. | [`higgs_TextDragAndDropTarget.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/mouse/higgs_TextDragAndDropTarget.h) |
| `TooltipClient` | Los componentes que quieran usar tooltips emergentes deben implementar esta interfaz. | [`higgs_TooltipClient.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/mouse/higgs_TooltipClient.h) |
| `SettableTooltipClient` | Una implementación de TooltipClient que almacena la cadena del tooltip y un método para cambiarla. | [`higgs_TooltipClient.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/mouse/higgs_TooltipClient.h) |
| `LassoSource` | Una clase que usa el LassoComponent para gestionar los elementos que selecciona. | [`higgs_LassoComponent.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/mouse/higgs_LassoComponent.h) |
| `LassoComponent` | Un componente que actúa como una región de selección rectangular, que se arrastra con el mouse para seleccionar grupos de objetos (junto con un SelectedItemSet). | [`higgs_LassoComponent.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/mouse/higgs_LassoComponent.h) |

### `keyboard`

| Clase | Descripción | Cabecera |
|---|---|---|
| `ModifierKeys` | Representa el estado de los botones del mouse y de las teclas modificadoras. | [`higgs_ModifierKeys.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/keyboard/higgs_ModifierKeys.h) |
| `KeyPress` | Representa una pulsación de tecla, incluidas las teclas modificadoras necesarias. | [`higgs_KeyPress.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/keyboard/higgs_KeyPress.h) |
| `KeyListener` | Recibe callbacks cuando se presionan teclas. | [`higgs_KeyListener.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/keyboard/higgs_KeyListener.h) |
| `KeyboardFocusTraverser` | Controla el orden en que el foco del teclado se mueve entre componentes. | [`higgs_KeyboardFocusTraverser.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/keyboard/higgs_KeyboardFocusTraverser.h) |
| `SystemClipboard` | Gestiona la lectura y escritura en el portapapeles del sistema. | [`higgs_SystemClipboard.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/keyboard/higgs_SystemClipboard.h) |
| `TextEditorKeyMapper` | Esta clase se usa para invocar un conjunto de métodos de navegación de editor de texto sobre un objeto, a partir de un evento de pulsación de tecla. | [`higgs_TextEditorKeyMapper.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/keyboard/higgs_TextEditorKeyMapper.h) |
| `TextInputTarget` | Una clase base abstracta que pueden implementar los componentes que funcionan como editores de texto. | [`higgs_TextInputTarget.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/keyboard/higgs_TextInputTarget.h) |

### `components`

| Clase | Descripción | Cabecera |
|---|---|---|
| `ComponentTraverser` | Clase base para recorrer componentes. | [`higgs_ComponentTraverser.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/components/higgs_ComponentTraverser.h) |
| `FocusTraverser` | Controla el orden en que el foco se mueve entre los componentes. | [`higgs_FocusTraverser.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/components/higgs_FocusTraverser.h) |
| `ModalComponentManager` | Administra la pila de componentes modales del sistema. | [`higgs_ModalComponentManager.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/components/higgs_ModalComponentManager.h) |
| `ModalCallbackFunction` | Esta clase proporciona algunos métodos utilitarios prácticos para crear objetos ModalComponentManager::Callback que invocarán una función estática con ciertos parámetros cuando se cierre un componente modal. | [`higgs_ModalComponentManager.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/components/higgs_ModalComponentManager.h) |
| `ComponentPaintDiagnostics` | Contiene información de tiempos y uso de caché de la operación de pintado de un Component. | [`higgs_ComponentListener.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/components/higgs_ComponentListener.h) |
| `ComponentListener` | Recibe notificaciones sobre los cambios en la jerarquía o la posición de un componente. | [`higgs_ComponentListener.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/components/higgs_ComponentListener.h) |
| `CachedComponentImage` | Clase base usada internamente para estructuras que pueden almacenar imágenes en caché del estado de un componente. | [`higgs_CachedComponentImage.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/components/higgs_CachedComponentImage.h) |
| `Component` | La clase base de todos los objetos de interfaz de usuario de HIGGS. | [`higgs_Component.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/components/higgs_Component.h) |

### `desktop`

| Clase | Descripción | Cabecera |
|---|---|---|
| `DesktopWindowOptions` | Describe la ventana de escritorio que Component::addToDesktop() debe crear para un componente. | [`higgs_DesktopWindowOptions.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/desktop/higgs_DesktopWindowOptions.h) |
| `FocusChangeListener` | Las clases pueden implementar esta interfaz y registrarse en la clase Desktop para recibir callbacks cuando cambia el componente que tiene el foco. | [`higgs_Desktop.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/desktop/higgs_Desktop.h) |
| `DarkModeSettingListener` | Las clases pueden implementar esta interfaz y registrarse en la clase Desktop para recibir callbacks cuando cambia la configuración de modo oscuro del sistema operativo. | [`higgs_Desktop.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/desktop/higgs_Desktop.h) |
| `Desktop` | Describe y controla aspectos del escritorio de la computadora. | [`higgs_Desktop.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/desktop/higgs_Desktop.h) |
| `Displays` | Administra los detalles de los dispositivos de pantalla conectados. | [`higgs_Displays.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/desktop/higgs_Displays.h) |

### `layout`

| Clase | Descripción | Cabecera |
|---|---|---|
| `ComponentAnimator` | Esta clase ha sido reemplazada; ahora se recomienda usar la clase Animator del módulo higgs_animation. | [`higgs_ComponentAnimator.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/layout/higgs_ComponentAnimator.h) |
| `ComponentBoundsConstrainer` | Una clase que impone restricciones al tamaño o la posición de un Component. | [`higgs_ComponentBoundsConstrainer.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/layout/higgs_ComponentBoundsConstrainer.h) |
| `BorderedComponentBoundsConstrainer` | ComponentBoundsConstrainer que se puede usar para agregar un borde constante a otro ComponentBoundsConstrainer. | [`higgs_BorderedComponentBoundsConstrainer.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/layout/higgs_BorderedComponentBoundsConstrainer.h) |
| `AnimatedPosition` | Modela una posición unidimensional que el usuario puede arrastrar y que luego sigue moviéndose con un comportamiento físico personalizable al soltarla. | [`higgs_AnimatedPosition.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/layout/higgs_AnimatedPosition.h) |
| `ContinuousWithMomentum` | Un comportamiento sin ajuste (snapping) que permite deslizar libremente el contenido en cualquier dirección, con una inercia basada en la velocidad con la que se soltó y una fricción variable que lo hace detenerse. | [`higgs_AnimatedPositionBehaviours.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/layout/higgs_AnimatedPositionBehaviours.h) |
| `SnapToPageBoundaries` | Un comportamiento que hace que un objeto AnimatedPosition gravite hacia la posición entera más cercana al soltarlo. | [`higgs_AnimatedPositionBehaviours.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/layout/higgs_AnimatedPositionBehaviours.h) |
| `ComponentBuilder` | Carga y mantiene un árbol de Components a partir de un ValueTree que los representa. | [`higgs_ComponentBuilder.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/layout/higgs_ComponentBuilder.h) |
| `ComponentMovementWatcher` | Un objeto que detecta cualquier movimiento de un componente o de cualquiera de sus componentes padre. | [`higgs_ComponentMovementWatcher.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/layout/higgs_ComponentMovementWatcher.h) |
| `ConcertinaPanel` | Panel que contiene una pila vertical de componentes que se pueden expandir y contraer. | [`higgs_ConcertinaPanel.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/layout/higgs_ConcertinaPanel.h) |
| `GroupComponent` | Un componente que dibuja un contorno a su alrededor y tiene un título opcional en la parte superior, para enmarcar un grupo de controles. | [`higgs_GroupComponent.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/layout/higgs_GroupComponent.h) |
| `ResizableBorderComponent` | Un componente que redimensiona su componente padre al ser arrastrado. | [`higgs_ResizableBorderComponent.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/layout/higgs_ResizableBorderComponent.h) |
| `ResizableCornerComponent` | Un componente que redimensiona un componente padre al arrastrarlo. | [`higgs_ResizableCornerComponent.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/layout/higgs_ResizableCornerComponent.h) |
| `ResizableEdgeComponent` | Un componente que redimensiona su componente padre al ser arrastrado. | [`higgs_ResizableEdgeComponent.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/layout/higgs_ResizableEdgeComponent.h) |
| `ScrollBar` | Componente de barra de desplazamiento. | [`higgs_ScrollBar.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/layout/higgs_ScrollBar.h) |
| `StretchableLayoutManager` | Para distribuir un conjunto de componentes que tienen tamaños preferidos y límites de tamaño, pero que pueden estirarse para ocupar el espacio disponible. | [`higgs_StretchableLayoutManager.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/layout/higgs_StretchableLayoutManager.h) |
| `StretchableLayoutResizerBar` | Un componente que funciona como una de las barras verticales u horizontales que se usan para redimensionar paneles en una ventana. | [`higgs_StretchableLayoutResizerBar.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/layout/higgs_StretchableLayoutResizerBar.h) |
| `StretchableObjectResizer` | Una clase utilitaria para ajustar en un espacio un conjunto de objetos cuyos tamaños pueden variar entre un mínimo y un máximo. | [`higgs_StretchableObjectResizer.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/layout/higgs_StretchableObjectResizer.h) |
| `TabBarButton` | En un TabbedButtonBar, este componente se usa para cada uno de los botones. | [`higgs_TabbedButtonBar.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/layout/higgs_TabbedButtonBar.h) |
| `TabbedButtonBar` | Una barra vertical u horizontal con pestañas que se pueden seleccionar. | [`higgs_TabbedButtonBar.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/layout/higgs_TabbedButtonBar.h) |
| `TabbedComponent` | Un componente con un TabbedButtonBar en uno de sus lados. | [`higgs_TabbedComponent.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/layout/higgs_TabbedComponent.h) |
| `Viewport` | Un Viewport se utiliza para contener un componente hijo más grande y permite desplazar automáticamente dicho hijo. | [`higgs_Viewport.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/layout/higgs_Viewport.h) |
| `MultiDocumentPanelWindow` | Derivado de DocumentWindow que se usa dentro de un componente MultiDocumentPanel. | [`higgs_MultiDocumentPanel.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/layout/higgs_MultiDocumentPanel.h) |
| `MultiDocumentPanel` | Un componente que contiene un conjunto de otros componentes, ya sea en ventanas flotantes o en pestañas. | [`higgs_MultiDocumentPanel.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/layout/higgs_MultiDocumentPanel.h) |
| `SidePanel` | Un componente que se ubica en el lado izquierdo o derecho de su padre y que contiene un encabezado y algo de contenido. | [`higgs_SidePanel.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/layout/higgs_SidePanel.h) |
| `FlexItem` | Describe las propiedades de un elemento dentro de un contenedor FlexBox. | [`higgs_FlexItem.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/layout/higgs_FlexItem.h) |
| `FlexBox` | Representa un contenedor FlexBox, que contiene y administra el diseño de un conjunto de objetos FlexItem. | [`higgs_FlexBox.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/layout/higgs_FlexBox.h) |
| `GridItem` | Define un elemento de un Grid. | [`higgs_GridItem.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/layout/higgs_GridItem.h) |
| `Grid` | Contenedor que gestiona la geometría de los diseños de cuadrícula (columnas y filas fijas) mediante un conjunto de reglas declarativas. | [`higgs_Grid.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/layout/higgs_Grid.h) |

### `commands`

| Clase | Descripción | Cabecera |
|---|---|---|
| `ApplicationCommandInfo` | Contiene información que describe un comando de la aplicación. | [`higgs_ApplicationCommandInfo.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/commands/higgs_ApplicationCommandInfo.h) |
| `ApplicationCommandTarget` | Un destino de comandos publica una lista de los ID de comando que puede ejecutar. | [`higgs_ApplicationCommandTarget.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/commands/higgs_ApplicationCommandTarget.h) |
| `ApplicationCommandManager` | Cada uno de estos objetos contiene una lista de todos los comandos que la aplicación puede ejecutar y los despacha cuando es necesario. | [`higgs_ApplicationCommandManager.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/commands/higgs_ApplicationCommandManager.h) |
| `ApplicationCommandManagerListener` | Un listener que recibe callbacks de un ApplicationCommandManager cuando se invocan comandos o cambia la lista de comandos. | [`higgs_ApplicationCommandManager.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/commands/higgs_ApplicationCommandManager.h) |
| `KeyPressMappingSet` | Administra y edita una lista de pulsaciones de teclas, que usa para invocar el comando correspondiente en un ApplicationCommandManager. | [`higgs_KeyPressMappingSet.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/commands/higgs_KeyPressMappingSet.h) |

### `buttons`

| Clase | Descripción | Cabecera |
|---|---|---|
| `Button` | Clase base para botones. | [`higgs_Button.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/buttons/higgs_Button.h) |
| `ArrowButton` | Botón con una flecha. | [`higgs_ArrowButton.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/buttons/higgs_ArrowButton.h) |
| `DrawableButton` | Un botón que muestra un Drawable. | [`higgs_DrawableButton.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/buttons/higgs_DrawableButton.h) |
| `HyperlinkButton` | Un botón que muestra un enlace web subrayado y que abre el enlace al hacer clic en él. | [`higgs_HyperlinkButton.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/buttons/higgs_HyperlinkButton.h) |
| `ImageButton` | Como su nombre lo indica, este es un botón que contiene una imagen. | [`higgs_ImageButton.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/buttons/higgs_ImageButton.h) |
| `ShapeButton` | Un botón que contiene una forma rellena. | [`higgs_ShapeButton.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/buttons/higgs_ShapeButton.h) |
| `TextButton` | Botón que usa el fondo estándar con forma de pastilla con una línea de texto encima. | [`higgs_TextButton.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/buttons/higgs_TextButton.h) |
| `ToggleButton` | Un botón que puede alternarse entre activado y desactivado. | [`higgs_ToggleButton.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/buttons/higgs_ToggleButton.h) |
| `ToolbarButton` | Un tipo de botón diseñado para colocarse en una barra de herramientas. | [`higgs_ToolbarButton.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/buttons/higgs_ToolbarButton.h) |

### `misc`

| Clase | Descripción | Cabecera |
|---|---|---|
| `OwningDrawableComponent` | Clase auxiliar que posee un objeto Drawable y lo expone como un DrawableComponent. | [`higgs_OwningDrawableComponent.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/misc/higgs_OwningDrawableComponent.h) |
| `DropShadower` | Agrega una sombra paralela a un componente. | [`higgs_DropShadower.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/misc/higgs_DropShadower.h) |
| `FocusOutline` | Agrega un contorno de foco a un componente. | [`higgs_FocusOutline.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/misc/higgs_FocusOutline.h) |
| `BubbleComponent` | Un componente para mostrar un mensaje u otros gráficos dentro de un contorno con forma de globo de diálogo, que apunta a una ubicación en la pantalla. | [`higgs_BubbleComponent.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/misc/higgs_BubbleComponent.h) |

### `accessibility/interfaces`

| Clase | Descripción | Cabecera |
|---|---|---|
| `AccessibilityCellInterface` | Una interfaz abstracta que representa un elemento de UI que admite una interfaz de celda. | [`higgs_AccessibilityCellInterface.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/accessibility/interfaces/higgs_AccessibilityCellInterface.h) |
| `AccessibilityTableInterface` | Interfaz abstracta que representa un elemento de la interfaz de usuario que admite una interfaz de tabla. | [`higgs_AccessibilityTableInterface.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/accessibility/interfaces/higgs_AccessibilityTableInterface.h) |
| `AccessibilityTextInterface` | Una interfaz abstracta que representa un elemento de UI que admite una interfaz de texto. | [`higgs_AccessibilityTextInterface.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/accessibility/interfaces/higgs_AccessibilityTextInterface.h) |
| `AccessibilityValueInterface` | Una interfaz abstracta que representa el valor de un elemento de accesibilidad. | [`higgs_AccessibilityValueInterface.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/accessibility/interfaces/higgs_AccessibilityValueInterface.h) |
| `AccessibilityTextValueInterface` | Una interfaz de valor que representa un valor de texto. | [`higgs_AccessibilityValueInterface.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/accessibility/interfaces/higgs_AccessibilityValueInterface.h) |
| `AccessibilityNumericValueInterface` | Interfaz de valor que representa un valor numérico sin rango. | [`higgs_AccessibilityValueInterface.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/accessibility/interfaces/higgs_AccessibilityValueInterface.h) |
| `AccessibilityRangedNumericValueInterface` | Una interfaz de valor que representa un valor numérico dentro de un rango. | [`higgs_AccessibilityValueInterface.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/accessibility/interfaces/higgs_AccessibilityValueInterface.h) |

### `accessibility/enums`

| Clase | Descripción | Cabecera |
|---|---|---|
| `AccessibilityActions` | Un envoltorio sencillo para construir una colección de acciones de accesibilidad admitidas y sus callbacks correspondientes para un elemento de la interfaz de usuario. | [`higgs_AccessibilityActions.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/accessibility/enums/higgs_AccessibilityActions.h) |

### `accessibility`

| Clase | Descripción | Cabecera |
|---|---|---|
| `AccessibleState` | Representa el estado de un elemento de UI accesible. | [`higgs_AccessibilityState.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/accessibility/higgs_AccessibilityState.h) |
| `AccessibilityHandler` | Clase base para Components accesibles. | [`higgs_AccessibilityHandler.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/accessibility/higgs_AccessibilityHandler.h) |

### `menus`

| Clase | Descripción | Cabecera |
|---|---|---|
| `PopupMenu` | Crea y muestra un menú emergente. | [`higgs_PopupMenu.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/menus/higgs_PopupMenu.h) |
| `MenuBarModel` | Clase para controlar componentes MenuBar. | [`higgs_MenuBarModel.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/menus/higgs_MenuBarModel.h) |
| `MenuBarComponent` | Un componente de barra de menús. | [`higgs_MenuBarComponent.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/menus/higgs_MenuBarComponent.h) |
| `BurgerMenuComponent` | Un componente que lista todos los elementos de menú y los agrupa en categorías según sus respectivos menús padre. | [`higgs_BurgerMenuComponent.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/menus/higgs_BurgerMenuComponent.h) |

### `positioning`

| Clase | Descripción | Cabecera |
|---|---|---|
| `RelativeCoordinate` | Expresa una coordenada como una expresión evaluada dinámicamente. | [`higgs_RelativeCoordinate.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/positioning/higgs_RelativeCoordinate.h) |
| `MarkerList` | Contiene un conjunto de puntos de marcador con nombre a lo largo de un eje unidimensional. | [`higgs_MarkerList.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/positioning/higgs_MarkerList.h) |
| `RelativePoint` | Una posición X-Y almacenada como un par de valores RelativeCoordinate. | [`higgs_RelativePoint.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/positioning/higgs_RelativePoint.h) |
| `RelativeRectangle` | Un rectángulo almacenado como un conjunto de valores RelativeCoordinate. | [`higgs_RelativeRectangle.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/positioning/higgs_RelativeRectangle.h) |
| `RelativeCoordinatePositionerBase` | Clase base para los Component::Positioners basados en coordenadas relativas. | [`higgs_RelativeCoordinatePositioner.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/positioning/higgs_RelativeCoordinatePositioner.h) |
| `RelativeParallelogram` | Paralelogramo definido por tres posiciones RelativePoint. | [`higgs_RelativeParallelogram.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/positioning/higgs_RelativeParallelogram.h) |
| `RelativePointPath` | Un objeto de trazado formado por coordenadas RelativePoint en lugar de las coordenadas fijas habituales. | [`higgs_RelativePointPath.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/positioning/higgs_RelativePointPath.h) |

### `widgets`

| Clase | Descripción | Cabecera |
|---|---|---|
| `TextEditor` | Un cuadro de texto editable. | [`higgs_TextEditor.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/widgets/higgs_TextEditor.h) |
| `Label` | Componente que muestra una cadena de texto y que, opcionalmente, puede convertirse en un editor de texto al hacer clic en él. | [`higgs_Label.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/widgets/higgs_Label.h) |
| `ComboBox` | Un componente que permite al usuario elegir entre las opciones de una lista desplegable. | [`higgs_ComboBox.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/widgets/higgs_ComboBox.h) |
| `ImageComponent` | Un componente que simplemente muestra una imagen. | [`higgs_ImageComponent.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/widgets/higgs_ImageComponent.h) |
| `ListBoxModel` | Una subclase de esta clase se usa para controlar un ListBox. | [`higgs_ListBox.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/widgets/higgs_ListBox.h) |
| `ListBox` | Una lista de elementos que se puede desplazar verticalmente. | [`higgs_ListBox.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/widgets/higgs_ListBox.h) |
| `ProgressBar` | Un componente de barra de progreso. | [`higgs_ProgressBar.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/widgets/higgs_ProgressBar.h) |
| `SliderListener` | Una clase para recibir callbacks de un Slider o de un WebSliderRelay. | [`higgs_Slider.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/widgets/higgs_Slider.h) |
| `Slider` | Control deslizante para cambiar un valor. | [`higgs_Slider.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/widgets/higgs_Slider.h) |
| `TableHeaderComponent` | Un componente que muestra una franja de encabezados de columna para una tabla y permite redimensionarlos, arrastrarlos, etc. | [`higgs_TableHeaderComponent.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/widgets/higgs_TableHeaderComponent.h) |
| `TableListBoxModel` | Un TableListBox utiliza uno de estos como modelo de datos para el contenido de la tabla. | [`higgs_TableListBox.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/widgets/higgs_TableListBox.h) |
| `TableListBox` | Tabla de celdas que usa un TableHeaderComponent como encabezado. | [`higgs_TableListBox.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/widgets/higgs_TableListBox.h) |
| `Toolbar` | Un componente de barra de herramientas. | [`higgs_Toolbar.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/widgets/higgs_Toolbar.h) |
| `ToolbarItemComponent` | Un componente que se puede usar como uno de los elementos de un Toolbar. | [`higgs_ToolbarItemComponent.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/widgets/higgs_ToolbarItemComponent.h) |
| `ToolbarItemFactory` | Un objeto fábrica que puede crear objetos ToolbarItemComponent. | [`higgs_ToolbarItemFactory.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/widgets/higgs_ToolbarItemFactory.h) |
| `ToolbarItemPalette` | Componente que contiene una lista de elementos de barra de herramientas que el usuario puede arrastrar a una barra de herramientas para agregarlos. | [`higgs_ToolbarItemPalette.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/widgets/higgs_ToolbarItemPalette.h) |
| `TreeViewItem` | Un elemento de un TreeView. | [`higgs_TreeView.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/widgets/higgs_TreeView.h) |
| `TreeView` | Componente de vista de árbol. | [`higgs_TreeView.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/widgets/higgs_TreeView.h) |

### `windows`

| Clase | Descripción | Cabecera |
|---|---|---|
| `TopLevelWindow` | Una clase base para ventanas de nivel superior. | [`higgs_TopLevelWindow.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/windows/higgs_TopLevelWindow.h) |
| `MessageBoxButtonOptions` | Clase que contiene información de configuración sobre los botones de acción que se muestran en un cuadro de mensaje. | [`higgs_MessageBoxOptions.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/windows/higgs_MessageBoxOptions.h) |
| `MessageBoxOptions` | Clase usada para crear un conjunto de opciones que se pasan a los métodos de AlertWindow y NativeMessageBox para mostrar cuadros de diálogo. | [`higgs_MessageBoxOptions.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/windows/higgs_MessageBoxOptions.h) |
| `ScopedMessageBox` | Los objetos de este tipo se pueden usar para cerrar cuadros de mensaje mediante programación. | [`higgs_ScopedMessageBox.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/windows/higgs_ScopedMessageBox.h) |
| `AlertWindow` | Una ventana que muestra un mensaje y tiene botones para que el usuario responda. | [`higgs_AlertWindow.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/windows/higgs_AlertWindow.h) |
| `CallOutBox` | Un cuadro con una pequeña flecha que puede usarse como ventana emergente temporal para mostrar controles adicionales al hacer clic en un botón u otro componente. | [`higgs_CallOutBox.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/windows/higgs_CallOutBox.h) |
| `ComponentPeer` | La clase Component usa internamente un ComponentPeer para crear y gestionar una ventana real del sistema operativo. | [`higgs_ComponentPeer.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/windows/higgs_ComponentPeer.h) |
| `ResizableWindow` | Una clase base para ventanas de nivel superior que pueden arrastrarse y redimensionarse. | [`higgs_ResizableWindow.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/windows/higgs_ResizableWindow.h) |
| `DocumentWindow` | Ventana redimensionable con barra de título y botones de maximizar, minimizar y cerrar. | [`higgs_DocumentWindow.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/windows/higgs_DocumentWindow.h) |
| `DialogWindow` | Una ventana de estilo cuadro de diálogo. | [`higgs_DialogWindow.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/windows/higgs_DialogWindow.h) |
| `NativeMessageBox` | Esta clase contiene algunos métodos estáticos para mostrar ventanas de alerta nativas. | [`higgs_NativeMessageBox.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/windows/higgs_NativeMessageBox.h) |
| `ThreadWithProgressWindow` | Un hilo que muestra automáticamente un cuadro de diálogo modal con una barra de progreso y un botón de cancelar mientras está ocupado en ejecución. | [`higgs_ThreadWithProgressWindow.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/windows/higgs_ThreadWithProgressWindow.h) |
| `TooltipWindow` | Una ventana que muestra un tooltip emergente cuando el mouse se posa sobre otro componente. | [`higgs_TooltipWindow.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/windows/higgs_TooltipWindow.h) |
| `VBlankAttachment` | Clase auxiliar para sincronizar las actualizaciones de un Component con el evento de borrado vertical (vertical blank) de la pantalla en la que se muestra ese Component. | [`higgs_VBlankAttachment.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/windows/higgs_VBlankAttachment.h) |
| `WindowUtils` | Funciones auxiliares de ventanas. | [`higgs_WindowUtils.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/windows/higgs_WindowUtils.h) |
| `NativeScaleFactorNotifier` | Llama a una función cada vez que cambia el factor de escala nativo del peer de un componente. | [`higgs_NativeScaleFactorNotifier.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/windows/higgs_NativeScaleFactorNotifier.h) |

### `filebrowser`

| Clase | Descripción | Cabecera |
|---|---|---|
| `FileBrowserListener` | Un listener para eventos de selección del usuario en un explorador de archivos. | [`higgs_FileBrowserListener.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/filebrowser/higgs_FileBrowserListener.h) |
| `DirectoryContentsList` | Clase para explorar de forma asíncrona los detalles de los archivos de un directorio. | [`higgs_DirectoryContentsList.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/filebrowser/higgs_DirectoryContentsList.h) |
| `DirectoryContentsDisplayComponent` | Una clase base para componentes que muestran una lista de los archivos de un directorio. | [`higgs_DirectoryContentsDisplayComponent.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/filebrowser/higgs_DirectoryContentsDisplayComponent.h) |
| `FileBrowserComponent` | Un componente para explorar y seleccionar un archivo o directorio para abrir o guardar. | [`higgs_FileBrowserComponent.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/filebrowser/higgs_FileBrowserComponent.h) |
| `FileChooser` | Crea un cuadro de diálogo para elegir un archivo o directorio que se va a cargar o guardar. | [`higgs_FileChooser.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/filebrowser/higgs_FileChooser.h) |
| `FileChooserDialogBox` | Un cuadro de diálogo para abrir o guardar archivos. | [`higgs_FileChooserDialogBox.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/filebrowser/higgs_FileChooserDialogBox.h) |
| `FileListComponent` | Un componente que muestra los archivos de un directorio como un cuadro de lista. | [`higgs_FileListComponent.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/filebrowser/higgs_FileListComponent.h) |
| `FilenameComponentListener` | Escucha los eventos que ocurren en un FilenameComponent. | [`higgs_FilenameComponent.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/filebrowser/higgs_FilenameComponent.h) |
| `FilenameComponent` | Muestra un nombre de archivo como un cuadro de texto editable, con un botón 'examinar' y una lista desplegable de archivos seleccionados recientemente. | [`higgs_FilenameComponent.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/filebrowser/higgs_FilenameComponent.h) |
| `FilePreviewComponent` | Clase base para componentes que se ubican dentro de un cuadro de diálogo de selección de archivos y muestran vistas previas de los archivos seleccionados. | [`higgs_FilePreviewComponent.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/filebrowser/higgs_FilePreviewComponent.h) |
| `FileSearchPathListComponent` | Muestra un conjunto de rutas de archivo en una lista y permite agregarlas, eliminarlas o reordenarlas. | [`higgs_FileSearchPathListComponent.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/filebrowser/higgs_FileSearchPathListComponent.h) |
| `FileTreeComponent` | Un componente que muestra los archivos de un directorio como una vista de árbol. | [`higgs_FileTreeComponent.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/filebrowser/higgs_FileTreeComponent.h) |
| `ImagePreviewComponent` | Un componente de vista previa simple que muestra miniaturas de archivos de imagen. | [`higgs_ImagePreviewComponent.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/filebrowser/higgs_ImagePreviewComponent.h) |
| `ContentSharer` | Funciones que permiten compartir contenido entre aplicaciones y dispositivos. | [`higgs_ContentSharer.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/filebrowser/higgs_ContentSharer.h) |

### `properties`

| Clase | Descripción | Cabecera |
|---|---|---|
| `PropertyComponent` | Una clase base para un componente que se coloca en un PropertyPanel y muestra una de las propiedades de un elemento. | [`higgs_PropertyComponent.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/properties/higgs_PropertyComponent.h) |
| `BooleanPropertyComponent` | PropertyComponent que contiene un botón de alternancia de encendido/apagado. | [`higgs_BooleanPropertyComponent.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/properties/higgs_BooleanPropertyComponent.h) |
| `ButtonPropertyComponent` | Un PropertyComponent que contiene un botón. | [`higgs_ButtonPropertyComponent.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/properties/higgs_ButtonPropertyComponent.h) |
| `ChoicePropertyComponent` | Un PropertyComponent que muestra su valor como un cuadro combinado. | [`higgs_ChoicePropertyComponent.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/properties/higgs_ChoicePropertyComponent.h) |
| `PropertyPanel` | Panel que contiene una lista de objetos PropertyComponent. | [`higgs_PropertyPanel.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/properties/higgs_PropertyPanel.h) |
| `SliderPropertyComponent` | Un PropertyComponent que muestra su valor como un control deslizante. | [`higgs_SliderPropertyComponent.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/properties/higgs_SliderPropertyComponent.h) |
| `TextPropertyComponent` | Un PropertyComponent que muestra su valor como texto editable. | [`higgs_TextPropertyComponent.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/properties/higgs_TextPropertyComponent.h) |
| `MultiChoicePropertyComponent` | Un PropertyComponent que muestra su valor como una lista expandible de ToggleButtons. | [`higgs_MultiChoicePropertyComponent.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/properties/higgs_MultiChoicePropertyComponent.h) |

### `application`

| Clase | Descripción | Cabecera |
|---|---|---|
| `HIGGSApplication` | Una instancia de esta clase se usa para especificar el código de inicialización y cierre de la aplicación. | [`higgs_Application.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/application/higgs_Application.h) |

### `lookandfeel`

| Clase | Descripción | Cabecera |
|---|---|---|
| `ExtraLookAndFeelBaseClasses` | Esta clase se usa para contener algunas clases base de look-and-feel asociadas a clases que podrían no estar presentes porque pertenecen a módulos distintos de higgs_gui_basics. | [`higgs_LookAndFeel.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/lookandfeel/higgs_LookAndFeel.h) |
| `LookAndFeel` | Los objetos LookAndFeel definen la apariencia de todos los widgets de HIGGS, y sus subclases pueden usarse para aplicar distintas 'skins' a la aplicación. | [`higgs_LookAndFeel.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/lookandfeel/higgs_LookAndFeel.h) |
| `LookAndFeel_V2` | Esta subclase de LookAndFeel implementa el estilo de higgs de alrededor de 2008-12. | [`higgs_LookAndFeel_V2.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/lookandfeel/higgs_LookAndFeel_V2.h) |
| `LookAndFeel_V1` | El look-and-feel original de HIGGS, utilizado aproximadamente desde 2002 hasta 2007. | [`higgs_LookAndFeel_V1.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/lookandfeel/higgs_LookAndFeel_V1.h) |
| `LookAndFeel_V3` | El estilo de look-and-feel más reciente de HIGGS, introducido en 2013. | [`higgs_LookAndFeel_V3.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/lookandfeel/higgs_LookAndFeel_V3.h) |
| `LookAndFeel_V4` | El estilo de look-and-feel más reciente de HIGGS, introducido en 2017. | [`higgs_LookAndFeel_V4.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_gui_basics/lookandfeel/higgs_LookAndFeel_V4.h) |
