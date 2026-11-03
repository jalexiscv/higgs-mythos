# higgs_data_structures

Estructuras de datos para el modelo de la aplicación: `ValueTree`, `Value`, `CachedValue`, el sistema de deshacer/rehacer (`UndoManager`) y la gestión de preferencias (`ApplicationProperties`, `PropertiesFile`).

## Ficha

| Campo | Valor |
|---|---|
| Identificador | `higgs_data_structures` |
| Versión | 9.0.5 |
| Nombre declarado | Higgs Mythos data model helper classes |
| Descripción declarada | Classes for undo/redo management, and smart data structures. |
| Estándar mínimo de C++ | C++17 |
| Licencia | AGPLv3 |
| Dependencias directas | [`higgs_events`](higgs_events.md) |
| Dependencias indirectas | [`higgs_core`](higgs_core.md) |
| Módulos que dependen de él | [`higgs_gui_basics`](higgs_gui_basics.md) |
| Código fuente | [`modules/higgs_data_structures`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_data_structures) |

## Uso

```cmake
target_link_libraries(MiTarget PRIVATE higgs::higgs_data_structures)
```

```cpp
#include <higgs_data_structures/higgs_data_structures.h>
```

Guías relacionadas: [Modelo de datos y deshacer](../guias/datos.md).

## Opciones de configuración

Este módulo no declara opciones de configuración.

## Catálogo de clases

9 clases y estructuras públicas declaradas en las cabeceras que incluye `higgs_data_structures.h`, agrupadas por carpeta. La descripción es la primera frase de su comentario de documentación en el código.

### `undomanager`

| Clase | Descripción | Cabecera |
|---|---|---|
| `UndoableAction` | Utilizada por la clase UndoManager para almacenar una acción que puede hacerse y deshacerse. | [`higgs_UndoableAction.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_data_structures/undomanager/higgs_UndoableAction.h) |
| `UndoManager` | Administra una lista de comandos de deshacer/rehacer. | [`higgs_UndoManager.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_data_structures/undomanager/higgs_UndoManager.h) |

### `values`

| Clase | Descripción | Cabecera |
|---|---|---|
| `Value` | Representa un valor variante compartido. | [`higgs_Value.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_data_structures/values/higgs_Value.h) |
| `ValueTree` | Una potente estructura de árbol que se puede usar para contener datos de forma libre y que puede gestionar su propio comportamiento de deshacer y rehacer. | [`higgs_ValueTree.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_data_structures/values/higgs_ValueTree.h) |
| `ValueTreeSynchroniser` | Esta clase se puede usar para observar todos los cambios en el estado de un ValueTree y convertirlos a una codificación binaria transmisible. | [`higgs_ValueTreeSynchroniser.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_data_structures/values/higgs_ValueTreeSynchroniser.h) |
| `CachedValue` | Esta clase actúa como un contenedor tipado de una propiedad dentro de un ValueTree. | [`higgs_CachedValue.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_data_structures/values/higgs_CachedValue.h) |
| `ValueTreePropertyWithDefault` | Esta clase actúa como un envoltorio de una propiedad dentro de un ValueTree. | [`higgs_ValueTreePropertyWithDefault.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_data_structures/values/higgs_ValueTreePropertyWithDefault.h) |

### `app_properties`

| Clase | Descripción | Cabecera |
|---|---|---|
| `PropertiesFile` | Contenedor de un archivo que almacena una lista de pares de datos clave/valor. | [`higgs_PropertiesFile.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_data_structures/app_properties/higgs_PropertiesFile.h) |
| `ApplicationProperties` | Gestiona una colección de propiedades. | [`higgs_ApplicationProperties.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_data_structures/app_properties/higgs_ApplicationProperties.h) |
