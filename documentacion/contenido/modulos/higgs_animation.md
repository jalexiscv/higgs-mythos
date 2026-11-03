# higgs_animation

Sistema declarativo de animación: constructores inmutables de animaciones (`ValueAnimatorBuilder`, `AnimatorSetBuilder`), funciones de suavizado (`Easings`) y actualizadores sincronizados con el refresco de pantalla.

## Ficha

| Campo | Valor |
|---|---|
| Identificador | `higgs_animation` |
| Versión | 9.0.5 |
| Nombre declarado | Higgs Mythos Animation classes |
| Descripción declarada | Classes for defining and handling animations. |
| Estándar mínimo de C++ | C++17 |
| Licencia | AGPLv3 |
| Dependencias directas | [`higgs_gui_basics`](higgs_gui_basics.md) |
| Dependencias indirectas | [`higgs_core`](higgs_core.md), [`higgs_events`](higgs_events.md), [`higgs_data_structures`](higgs_data_structures.md), [`higgs_graphics`](higgs_graphics.md) |
| Código fuente | [`modules/higgs_animation`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_animation) |

## Uso

```cmake
target_link_libraries(MiTarget PRIVATE higgs::higgs_animation)
```

```cpp
#include <higgs_animation/higgs_animation.h>
```

Guías relacionadas: [Animación](../guias/animacion.md).

## Opciones de configuración

Este módulo no declara opciones de configuración.

## Código de terceros incluido

| Componente | Licencia | Ubicación |
|---|---|---|
| [Chromium cubic_bezier](https://chromium.googlesource.com/chromium/src) | `BSD-3-Clause` | `modules/higgs_animation/detail/chromium` |

## Catálogo de clases

8 clases y estructuras públicas declaradas en las cabeceras que incluye `higgs_animation.h`, agrupadas por carpeta. La descripción es la primera frase de su comentario de documentación en el código.

### `animation`

| Clase | Descripción | Cabecera |
|---|---|---|
| `Animator` | Clase contenedora para administrar el ciclo de vida de todos los distintos tipos de animadores creados mediante las clases constructoras. | [`higgs_Animator.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_animation/animation/higgs_Animator.h) |
| `AnimatorSetBuilder` | Clase constructora que se puede usar para construir un Animator que envuelve una implementación de AnimatorSet. | [`higgs_AnimatorSetBuilder.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_animation/animation/higgs_AnimatorSetBuilder.h) |
| `AnimatorUpdater` | Clase auxiliar para actualizar varios animadores a la vez, sin poseerlos ni extender de otro modo su tiempo de vida. | [`higgs_AnimatorUpdater.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_animation/animation/higgs_AnimatorUpdater.h) |
| `SpringEasingOptions` | Un conjunto de opciones disponibles para personalizar una función de easing de tipo resorte. | [`higgs_Easings.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_animation/animation/higgs_Easings.h) |
| `Easings` | Contiene varias funciones de suavizado (easing) que se pueden pasar a ValueAnimatorBuilder::withEasing para transformar la progresión lineal de las animaciones. | [`higgs_Easings.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_animation/animation/higgs_Easings.h) |
| `StaticAnimationLimits` | Clase auxiliar para usar interpolación lineal entre un valor inicial y un valor final. | [`higgs_StaticAnimationLimits.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_animation/animation/higgs_StaticAnimationLimits.h) |
| `ValueAnimatorBuilder` | Una clase constructora que puede usarse para construir un Animator que envuelve una implementación de ValueAnimator. | [`higgs_ValueAnimatorBuilder.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_animation/animation/higgs_ValueAnimatorBuilder.h) |
| `VBlankAnimatorUpdater` | Similar a AnimatorUpdater, pero llama automáticamente a update() cada vez que se actualiza la pantalla. | [`higgs_VBlankAnimatorUpdater.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_animation/animation/higgs_VBlankAnimatorUpdater.h) |
