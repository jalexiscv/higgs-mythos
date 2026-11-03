# higgs_product_unlocking

Compras integradas en las tiendas de Apple y Google, y un sistema de licencias con desbloqueo en línea y archivos de clave firmados.

## Ficha

| Campo | Valor |
|---|---|
| Identificador | `higgs_product_unlocking` |
| Versión | 9.0.5 |
| Nombre declarado | Higgs Mythos Online marketplace support |
| Descripción declarada | Classes for online product authentication |
| Estándar mínimo de C++ | C++17 |
| Licencia | AGPLv3 |
| Dependencias directas | [`higgs_cryptography`](higgs_cryptography.md) |
| Dependencias indirectas | [`higgs_core`](higgs_core.md) |
| Código fuente | [`modules/higgs_product_unlocking`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_product_unlocking) |

## Uso

```cmake
target_link_libraries(MiTarget PRIVATE higgs::higgs_product_unlocking)
```

```cpp
#include <higgs_product_unlocking/higgs_product_unlocking.h>
```

Guías relacionadas: [Otros módulos](../guias/otros-modulos.md#compras-integradas-y-desbloqueo-de-productos).

## Opciones de configuración

Este módulo no declara opciones de configuración.

## Catálogo de clases

5 clases y estructuras públicas declaradas en las cabeceras que incluye `higgs_product_unlocking.h`, agrupadas por carpeta. La descripción es la primera frase de su comentario de documentación en el código.

### `in_app_purchases`

| Clase | Descripción | Cabecera |
|---|---|---|
| `InAppPurchases` | Proporciona funcionalidad de compras dentro de la aplicación. | [`higgs_InAppPurchases.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_product_unlocking/in_app_purchases/higgs_InAppPurchases.h) |

### `marketplace`

| Clase | Descripción | Cabecera |
|---|---|---|
| `OnlineUnlockStatus` | Clase base para sistemas de desbloqueo en línea. | [`higgs_OnlineUnlockStatus.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_product_unlocking/marketplace/higgs_OnlineUnlockStatus.h) |
| `TracktionMarketplaceStatus` | Una implementación de la clase OnlineUnlockStatus que se comunica con el servidor del marketplace. | [`higgs_TracktionMarketplaceStatus.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_product_unlocking/marketplace/higgs_TracktionMarketplaceStatus.h) |
| `KeyGeneration` | Contiene utilidades estáticas para generar archivos de clave que pueden desbloquearse mediante la clase OnlineUnlockStatus. | [`higgs_KeyFileGeneration.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_product_unlocking/marketplace/higgs_KeyFileGeneration.h) |
| `OnlineUnlockForm` | Actúa como una GUI que solicita al usuario sus datos y llama a los métodos correspondientes de su objeto OnlineUnlockStatus para intentar registrar la aplicación. | [`higgs_OnlineUnlockForm.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_product_unlocking/marketplace/higgs_OnlineUnlockForm.h) |
