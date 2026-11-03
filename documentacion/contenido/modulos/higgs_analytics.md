# higgs_analytics

Recopilación de analítica de uso y envío por lotes a destinos configurables desde un hilo de fondo.

## Ficha

| Campo | Valor |
|---|---|
| Identificador | `higgs_analytics` |
| Versión | 9.0.5 |
| Nombre declarado | Higgs Mythos analytics classes |
| Descripción declarada | Classes to collect analytics and send to destinations |
| Estándar mínimo de C++ | C++17 |
| Licencia | AGPLv3 |
| Dependencias directas | [`higgs_gui_basics`](higgs_gui_basics.md) |
| Dependencias indirectas | [`higgs_core`](higgs_core.md), [`higgs_events`](higgs_events.md), [`higgs_data_structures`](higgs_data_structures.md), [`higgs_graphics`](higgs_graphics.md) |
| Código fuente | [`modules/higgs_analytics`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_analytics) |

## Uso

```cmake
target_link_libraries(MiTarget PRIVATE higgs::higgs_analytics)
```

```cpp
#include <higgs_analytics/higgs_analytics.h>
```

Guías relacionadas: [Otros módulos](../guias/otros-modulos.md#analitica).

## Opciones de configuración

Este módulo no declara opciones de configuración.

## Catálogo de clases

4 clases y estructuras públicas declaradas en las cabeceras que incluye `higgs_analytics.h`, agrupadas por carpeta. La descripción es la primera frase de su comentario de documentación en el código.

### `destinations`

| Clase | Descripción | Cabecera |
|---|---|---|
| `AnalyticsDestination` | Una interfaz para gestionar los eventos de analítica recopilados por un objeto Analytics. | [`higgs_AnalyticsDestination.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_analytics/destinations/higgs_AnalyticsDestination.h) |
| `ThreadedAnalyticsDestination` | Clase base para despachar eventos de analítica en un hilo dedicado. | [`higgs_ThreadedAnalyticsDestination.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_analytics/destinations/higgs_ThreadedAnalyticsDestination.h) |

### `analytics`

| Clase | Descripción | Cabecera |
|---|---|---|
| `Analytics` | Una clase singleton para administrar datos de analítica. | [`higgs_Analytics.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_analytics/analytics/higgs_Analytics.h) |
| `ButtonTracker` | Una clase que envía automáticamente eventos de analítica al singleton Analytics cuando se hace clic en un botón. | [`higgs_ButtonTracker.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_analytics/analytics/higgs_ButtonTracker.h) |
