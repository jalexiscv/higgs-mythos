# higgs_events

La infraestructura del bucle de mensajes de la aplicación: `MessageManager`, temporizadores, mensajes asíncronos, difusores y oyentes, la clase base de aplicación, comunicación entre procesos y descubrimiento de servicios en red.

## Ficha

| Campo | Valor |
|---|---|
| Identificador | `higgs_events` |
| Versión | 9.0.5 |
| Nombre declarado | Higgs Mythos message and event handling classes |
| Descripción declarada | Classes for running an application's main event loop and sending/receiving messages, timers, etc. |
| Estándar mínimo de C++ | C++17 |
| Licencia | AGPLv3 |
| Dependencias directas | [`higgs_core`](higgs_core.md) |
| Módulos que dependen de él | [`higgs_data_structures`](higgs_data_structures.md), [`higgs_graphics`](higgs_graphics.md), [`higgs_audio_devices`](higgs_audio_devices.md), [`higgs_audio_processors_headless`](higgs_audio_processors_headless.md), [`higgs_osc`](higgs_osc.md) |
| Código fuente | [`modules/higgs_events`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_events) |

## Uso

```cmake
target_link_libraries(MiTarget PRIVATE higgs::higgs_events)
```

```cpp
#include <higgs_events/higgs_events.h>
```

Guías relacionadas: [Bucle de mensajes y eventos](../conceptos/eventos.md).

## Opciones de configuración

Se cambian con `target_compile_definitions(MiTarget PUBLIC OPCION=valor)` o en la sección *Modules* de Mythos Studio.

| Opción | Por defecto | Descripción |
|---|---|---|
| `HIGGS_EXECUTE_APP_SUSPEND_ON_BACKGROUND_TASK` | `0` | Ejecutará el método de suspensión de su aplicación en una tarea en segundo plano de iOS, lo que le da tiempo adicional para guardar el estado de su aplicación. |

## Catálogo de clases

24 clases y estructuras públicas declaradas en las cabeceras que incluye `higgs_events.h`, agrupadas por carpeta. La descripción es la primera frase de su comentario de documentación en el código.

### `messages`

| Clase | Descripción | Cabecera |
|---|---|---|
| `MessageManager` | Esta clase se encarga del bucle de despacho de eventos de la aplicación. | [`higgs_MessageManager.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_events/messages/higgs_MessageManager.h) |
| `MessageManagerLock` | Se usa para garantizar que el hilo que llama tenga acceso exclusivo al bucle de mensajes. | [`higgs_MessageManager.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_events/messages/higgs_MessageManager.h) |
| `Message` | La clase base para objetos que pueden enviarse a un MessageListener. | [`higgs_Message.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_events/messages/higgs_Message.h) |
| `MessageListener` | Las subclases de MessageListener pueden publicar y recibir objetos Message. | [`higgs_MessageListener.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_events/messages/higgs_MessageListener.h) |
| `CallbackMessage` | Un mensaje que invoca un método de callback cuando se entrega. | [`higgs_CallbackMessage.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_events/messages/higgs_CallbackMessage.h) |
| `DeletedAtShutdown` | Las clases derivadas de esta se eliminarán automáticamente cuando la aplicación finalice. | [`higgs_DeletedAtShutdown.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_events/messages/higgs_DeletedAtShutdown.h) |
| `HIGGSApplicationBase` | Clase base abstracta para clases de aplicación. | [`higgs_ApplicationBase.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_events/messages/higgs_ApplicationBase.h) |
| `ScopedHiggsInitialiser_GUI` | Un objeto utilitario que ayuda a inicializar y finalizar HIGGS correctamente usando un patrón RAII. | [`higgs_Initialisation.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_events/messages/higgs_Initialisation.h) |
| `MountedVolumeListChangeDetector` | Una instancia de esta clase proporcionará callbacks cuando se monten o desmonten unidades en el sistema. | [`higgs_MountedVolumeListChangeDetector.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_events/messages/higgs_MountedVolumeListChangeDetector.h) |

### `broadcasters`

| Clase | Descripción | Cabecera |
|---|---|---|
| `ActionBroadcaster` | Administra una lista de ActionListeners y puede enviarles mensajes. | [`higgs_ActionBroadcaster.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_events/broadcasters/higgs_ActionBroadcaster.h) |
| `ActionListener` | Clase de interfaz para la entrega de eventos enviados por un ActionBroadcaster. | [`higgs_ActionListener.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_events/broadcasters/higgs_ActionListener.h) |
| `AsyncUpdater` | Tiene un método de callback que se activa de forma asíncrona. | [`higgs_AsyncUpdater.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_events/broadcasters/higgs_AsyncUpdater.h) |
| `LockingAsyncUpdater` | Similar a un AsyncUpdater, pero garantiza que, una vez que cancelPendingUpdate() retorne, la función asíncrona nunca se llamará hasta que se vuelva a llamar a triggerAsyncUpdate(). | [`higgs_LockingAsyncUpdater.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_events/broadcasters/higgs_LockingAsyncUpdater.h) |
| `ChangeListener` | Recibe callbacks de eventos de cambio enviados por un ChangeBroadcaster. | [`higgs_ChangeListener.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_events/broadcasters/higgs_ChangeListener.h) |
| `ChangeBroadcaster` | Contiene una lista de ChangeListeners y les envía mensajes cuando se le indica. | [`higgs_ChangeBroadcaster.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_events/broadcasters/higgs_ChangeBroadcaster.h) |

### `timers`

| Clase | Descripción | Cabecera |
|---|---|---|
| `Timer` | Realiza callbacks repetidos a un método virtual en un intervalo de tiempo especificado. | [`higgs_Timer.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_events/timers/higgs_Timer.h) |
| `TimedCallback` | Clase utilitaria que envuelve un único callback no nulo invocado por un Timer. | [`higgs_TimedCallback.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_events/timers/higgs_TimedCallback.h) |
| `MultiTimer` | Tipo de clase de temporizador que puede ejecutar varios temporizadores con distintas frecuencias, todos los cuales comparten un único callback. | [`higgs_MultiTimer.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_events/timers/higgs_MultiTimer.h) |

### `interprocess`

| Clase | Descripción | Cabecera |
|---|---|---|
| `ChildProcessManager` | Gestiona un conjunto de ChildProcesses y comprueba periódicamente su valor de retorno. | [`higgs_ChildProcessManager.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_events/interprocess/higgs_ChildProcessManager.h) |
| `InterprocessConnection` | Administra una conexión de mensajería bidireccional simple con otro proceso, usando un socket o una tubería con nombre como medio de transporte. | [`higgs_InterprocessConnection.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_events/interprocess/higgs_InterprocessConnection.h) |
| `InterprocessConnectionServer` | Un objeto que espera a que los sockets cliente se conecten a un puerto de este host y crea objetos InterprocessConnection para cada uno. | [`higgs_InterprocessConnectionServer.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_events/interprocess/higgs_InterprocessConnectionServer.h) |
| `ChildProcessWorker` | Actúa como el extremo trabajador de un par de procesos conectados coordinador/trabajador. | [`higgs_ConnectedChildProcess.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_events/interprocess/higgs_ConnectedChildProcess.h) |
| `ChildProcessCoordinator` | Actúa como coordinador en un par coordinador/trabajador de procesos conectados. | [`higgs_ConnectedChildProcess.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_events/interprocess/higgs_ConnectedChildProcess.h) |
| `NetworkServiceDiscovery` | Contiene clases que implementan un protocolo sencillo para difundir la disponibilidad y la ubicación de un servicio detectable en la red local, y para mantener una lista de servicios conocidos. | [`higgs_NetworkServiceDiscovery.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_events/interprocess/higgs_NetworkServiceDiscovery.h) |
