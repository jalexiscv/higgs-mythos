# higgs_javascript

Un intérprete de JavaScript (basado en QuickJS a través de CHOC) para ejecutar scripts y exponerles objetos nativos.

## Ficha

| Campo | Valor |
|---|---|
| Identificador | `higgs_javascript` |
| Versión | 9.0.5 |
| Nombre declarado | Higgs Mythos Javascript interpreter |
| Descripción declarada | Javascript interpreter. |
| Estándar mínimo de C++ | C++17 |
| Licencia | AGPLv3 |
| Dependencias directas | [`higgs_core`](higgs_core.md) |
| Código fuente | [`modules/higgs_javascript`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_javascript) |

## Uso

```cmake
target_link_libraries(MiTarget PRIVATE higgs::higgs_javascript)
```

```cpp
#include <higgs_javascript/higgs_javascript.h>
```

Guías relacionadas: [Otros módulos](../guias/otros-modulos.md#javascript).

## Opciones de configuración

Este módulo no declara opciones de configuración.

## Código de terceros incluido

| Componente | Licencia | Ubicación |
|---|---|---|
| [CHOC](https://github.com/Tracktion/choc) | `ISC` | `modules/higgs_javascript/choc` |
| [QuickJS](https://bellard.org/quickjs) | `MIT` | `modules/higgs_javascript/choc/javascript/choc_javascript_QuickJS.h` |

## Catálogo de clases

3 clases y estructuras públicas declaradas en las cabeceras que incluye `higgs_javascript.h`, agrupadas por carpeta. La descripción es la primera frase de su comentario de documentación en el código.

### `javascript`

| Clase | Descripción | Cabecera |
|---|---|---|
| `JSObject` | Un JSObject representa una referencia propietaria al objeto JS subyacente, lo que significa que seguirá siendo válido aunque una ejecución posterior de un script elimine otros handles que apunten a él. | [`higgs_JSObject.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_javascript/javascript/higgs_JSObject.h) |
| `JSCursor` | Un contenedor de alto nivel en torno a un JSObject raíz propietario y una ruta jerárquica relativa a él. | [`higgs_JSCursor.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_javascript/javascript/higgs_JSCursor.h) |
| `JavascriptEngine` | Esta clase es un envoltorio de QuickJS, un motor de JavaScript incrustable compatible con ES2023. | [`higgs_JavascriptEngine.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_javascript/javascript/higgs_JavascriptEngine.h) |
