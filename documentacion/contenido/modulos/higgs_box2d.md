# higgs_box2d

El motor de física 2D Box2D y un renderizador que dibuja sus mundos con `Graphics`.

## Ficha

| Campo | Valor |
|---|---|
| Identificador | `higgs_box2d` |
| Versión | 9.0.5 |
| Nombre declarado | Higgs Mythos wrapper for the Box2D physics engine |
| Descripción declarada | The Box2D physics engine and some utility classes. |
| Estándar mínimo de C++ | C++17 |
| Licencia | AGPLv3 |
| Dependencias directas | [`higgs_graphics`](higgs_graphics.md) |
| Dependencias indirectas | [`higgs_core`](higgs_core.md), [`higgs_events`](higgs_events.md) |
| Código fuente | [`modules/higgs_box2d`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_box2d) |

## Uso

```cmake
target_link_libraries(MiTarget PRIVATE higgs::higgs_box2d)
```

```cpp
#include <higgs_box2d/higgs_box2d.h>
```

Guías relacionadas: [Otros módulos](../guias/otros-modulos.md#box2d).

## Opciones de configuración

Este módulo no declara opciones de configuración.

## Código de terceros incluido

| Componente | Licencia | Ubicación |
|---|---|---|
| [Box2D](https://box2d.org) | `Zlib` | `modules/higgs_box2d/box2d` |

## Catálogo de clases

1 clases y estructuras públicas declaradas en las cabeceras que incluye `higgs_box2d.h`, agrupadas por carpeta. La descripción es la primera frase de su comentario de documentación en el código.

### `utils`

| Clase | Descripción | Cabecera |
|---|---|---|
| `Box2DRenderer` | Una implementación simple de la clase b2Draw, usada para dibujar un mundo de Box2D. | [`higgs_Box2DRenderer.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_box2d/utils/higgs_Box2DRenderer.h) |
