# higgs_osc

Implementación de Open Sound Control: emisor y receptor UDP, mensajes, paquetes, argumentos, patrones de dirección y etiquetas de tiempo.

## Ficha

| Campo | Valor |
|---|---|
| Identificador | `higgs_osc` |
| Versión | 9.0.5 |
| Nombre declarado | Higgs Mythos OSC classes |
| Descripción declarada | Open Sound Control implementation. |
| Estándar mínimo de C++ | C++17 |
| Licencia | AGPLv3 |
| Dependencias directas | [`higgs_events`](higgs_events.md) |
| Dependencias indirectas | [`higgs_core`](higgs_core.md) |
| Código fuente | [`modules/higgs_osc`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_osc) |

## Uso

```cmake
target_link_libraries(MiTarget PRIVATE higgs::higgs_osc)
```

```cpp
#include <higgs_osc/higgs_osc.h>
```

Guías relacionadas: [Otros módulos](../guias/otros-modulos.md#osc).

## Opciones de configuración

Este módulo no declara opciones de configuración.

## Catálogo de clases

13 clases y estructuras públicas declaradas en las cabeceras que incluye `higgs_osc.h`, agrupadas por carpeta. La descripción es la primera frase de su comentario de documentación en el código.

### `osc`

| Clase | Descripción | Cabecera |
|---|---|---|
| `OSCTypes` | Definiciones de los tipos OSC admitidos y sus etiquetas de tipo OSC asociadas, según la especificación OpenSoundControl 1.0. | [`higgs_OSCTypes.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_osc/osc/higgs_OSCTypes.h) |
| `OSCColour` | Contiene un color RGBA de 32 bits para pasarlo a un OSCArgument y obtenerlo de él. | [`higgs_OSCTypes.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_osc/osc/higgs_OSCTypes.h) |
| `OSCException` | Clase base para las excepciones que pueden lanzar los métodos del módulo OSC. | [`higgs_OSCTypes.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_osc/osc/higgs_OSCTypes.h) |
| `OSCFormatError` | Tipo de excepción que se lanza cuando el módulo OSC no puede analizar algo debido a un formato de datos incompatible con la especificación OpenSoundControl 1.0. | [`higgs_OSCTypes.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_osc/osc/higgs_OSCTypes.h) |
| `OSCInternalError` | Tipo de excepción que se lanza en caso de errores inesperados en el módulo OSC. | [`higgs_OSCTypes.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_osc/osc/higgs_OSCTypes.h) |
| `OSCTimeTag` | Una etiqueta de tiempo OSC. | [`higgs_OSCTimeTag.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_osc/osc/higgs_OSCTimeTag.h) |
| `OSCArgument` | Un argumento OSC. | [`higgs_OSCArgument.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_osc/osc/higgs_OSCArgument.h) |
| `OSCAddress` | Dirección OSC. | [`higgs_OSCAddress.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_osc/osc/higgs_OSCAddress.h) |
| `OSCAddressPattern` | Un patrón de dirección OSC. | [`higgs_OSCAddress.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_osc/osc/higgs_OSCAddress.h) |
| `OSCMessage` | Mensaje OSC. | [`higgs_OSCMessage.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_osc/osc/higgs_OSCMessage.h) |
| `OSCBundle` | Un bundle OSC. | [`higgs_OSCBundle.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_osc/osc/higgs_OSCBundle.h) |
| `OSCReceiver` | Una clase para recibir datos OSC. | [`higgs_OSCReceiver.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_osc/osc/higgs_OSCReceiver.h) |
| `OSCSender` | Emisor de mensajes OSC. | [`higgs_OSCSender.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_osc/osc/higgs_OSCSender.h) |
