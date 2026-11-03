# higgs_core

El conjunto esencial de clases básicas que necesitan todos los demás módulos: texto (`String`, `StringArray`), contenedores (`Array`, `OwnedArray`, `HashMap`), memoria y punteros inteligentes, hilos y sincronización, archivos y flujos, red (`URL`, sockets), XML, JSON, ZIP, tiempo, matemáticas, información del sistema, registro y pruebas unitarias.

## Ficha

| Campo | Valor |
|---|---|
| Identificador | `higgs_core` |
| Versión | 9.0.5 |
| Nombre declarado | Higgs Mythos core classes |
| Descripción declarada | The essential set of basic Higgs Mythos classes, as required by all the other Higgs Mythos modules. Includes text, container, memory, threading and i/o functionality. |
| Estándar mínimo de C++ | C++17 |
| Licencia | AGPLv3 |
| Dependencias directas | ninguna |
| Módulos que dependen de él | [`higgs_events`](higgs_events.md), [`higgs_cryptography`](higgs_cryptography.md), [`higgs_audio_basics`](higgs_audio_basics.md), [`higgs_javascript`](higgs_javascript.md) |
| Frameworks de macOS | `Cocoa`, `Foundation`, `IOKit`, `Security` |
| Frameworks de iOS | `Foundation` |
| Bibliotecas de Linux | `rt`, `dl`, `pthread` |
| Código fuente | [`modules/higgs_core`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core) |

## Uso

```cmake
target_link_libraries(MiTarget PRIVATE higgs::higgs_core)
```

```cpp
#include <higgs_core/higgs_core.h>
```

Guías relacionadas: [Memoria, cadenas y contenedores](../conceptos/nucleo.md), [Hilos y concurrencia](../conceptos/hilos.md), [Archivos, red y serialización](../guias/archivos-red.md), [Pruebas unitarias](../guias/pruebas.md), [Localización](../guias/localizacion.md).

## Opciones de configuración

Se cambian con `target_compile_definitions(MiTarget PUBLIC OPCION=valor)` o en la sección *Modules* de Mythos Studio.

| Opción | Por defecto | Descripción |
|---|---|---|
| `HIGGS_FORCE_DEBUG` | sin definir | Normalmente, `HIGGS_DEBUG` se establece en `1` o `0` según la configuración del compilador y del proyecto, pero si define este valor, puede sobrescribirlo para forzar que sea verdadero o falso. |
| `HIGGS_LOG_ASSERTIONS` | 1 en Android; 0 en el resto | Si este indicador está habilitado, las macros `jassert` y `jassertfalse` siempre usarán `Logger::writeToLog()` para escribir un mensaje cuando se produzca una aserción. Habilitarlo también hará que esto permanezca activado en las compilaciones de release. En cambio, cuando está deshabilitado, las macros `jassert` y `jassertfalse` no se compilarán en una compilación de release. Véase también: `jassert`, `jassertfalse`, `Logger`. |
| `HIGGS_CHECK_MEMORY_LEAKS` | 1 en compilaciones de depuración | Habilita una comprobación de fugas de memoria para ciertos objetos cuando la aplicación finaliza. Consulte la clase `LeakedObjectDetector` y la macro `HIGGS_LEAK_DETECTOR` para obtener más detalles sobre cómo habilitar la comprobación de fugas para clases específicas. |
| `HIGGS_DONT_AUTOLINK_TO_WIN32_LIBRARIES` | `0` | En una compilación para Windows, esta opción puede usarse para evitar que las bibliotecas del sistema necesarias se agreguen automáticamente a la etapa de enlazado. |
| `HIGGS_INCLUDE_ZLIB_CODE` | `1` | Esta opción permite deshabilitar el código de terceros de `zlib` integrado en HIGGS. Es posible que deba ajustarla si enlaza su aplicación con una biblioteca `zlib` externa, pero en aplicaciones normales esta opción debe dejarse como está. Si la deshabilita, quizá también le convenga establecer un valor para `HIGGS_ZLIB_INCLUDE_PATH`, para especificar la ruta donde se encuentran sus encabezados de `zlib`. |
| `HIGGS_USE_CURL` | `1` | Habilita la compatibilidad con http/https mediante `libcurl` (solo Linux). Habilitar esta opción agregará una dependencia dinámica adicional en tiempo de ejecución con `libcurl`. Si la deshabilita, la compatibilidad con https/ssl no estará disponible en Linux. |
| `HIGGS_LOAD_CURL_SYMBOLS_LAZILY` | `0` | Si se habilita, HIGGS cargará `libcurl` de forma diferida cuando sea necesario (por ejemplo, cuando se use `WebInputStream`). Habilitar este indicador también puede ayudar con los errores de dependencias de bibliotecas, ya que enlazar `libcurl` en tiempo de compilación puede indicarle al enlazador que dependa estrictamente de una versión específica de `libcurl`. También resulta útil si desea limitar la cantidad de dependencias de HIGGS y no usa `WebInputStream` ni las clases `URL`. |
| `HIGGS_CATCH_UNHANDLED_EXCEPTIONS` | `0` | Si se habilita, se agregará código de captura de excepciones para reenviar las excepciones no controladas a su callback `HIGGSApplicationBase::unhandledException()`. |
| `HIGGS_ALLOW_STATIC_NULL_VARIABLES` | `0` | Si se deshabilita, se desactivarán variables globales estáticas peligrosas como `String::empty`, `var::null`, etc., que pueden causar graves problemas de orden de inicialización si se hace referencia a ellas durante el código de constructores estáticos. |
| `HIGGS_STRICT_REFCOUNTEDPOINTER` | `0` | Si se habilita, la clase `ReferenceCountedObjectPtr` será más estricta a la hora de permitir que se convierta directamente en un puntero crudo. De forma predeterminada, esta opción está deshabilitada por compatibilidad con código antiguo, pero, si es posible, siempre debería habilitarla para mejorar la seguridad del código. |
| `HIGGS_ENABLE_ALLOCATION_HOOKS` | `0` | Si se habilita, se agregarán funciones globales de asignación de memoria con aserciones integradas, lo que puede ayudar a depurar las asignaciones en las pruebas unitarias. |

## Código de terceros incluido

| Componente | Licencia | Ubicación |
|---|---|---|
| [zlib](https://zlib.net) | `Zlib` | `modules/higgs_core/zip/zlib` |

## Catálogo de clases

162 clases y estructuras públicas declaradas en las cabeceras que incluye `higgs_core.h`, agrupadas por carpeta. La descripción es la primera frase de su comentario de documentación en el código.

### `misc`

| Clase | Descripción | Cabecera |
|---|---|---|
| `OrderedContainerHelpers` | Una estructura auxiliar que proporciona funciones para gestionar contenedores ordenados. | [`higgs_OrderedContainerHelpers.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/misc/higgs_OrderedContainerHelpers.h) |
| `NullCheckedInvocation` | Algunos métodos auxiliares para verificar un objeto invocable antes de invocarlo con los argumentos especificados. | [`higgs_Functional.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/misc/higgs_Functional.h) |
| `ScopeGuard` | Forma sencilla de garantizar que se llame a una función al final del ámbito actual. | [`higgs_ScopeGuard.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/misc/higgs_ScopeGuard.h) |
| `ErasedScopeGuard` | Un ScopeGuard que usa internamente un std::function para permitir el borrado de tipos. | [`higgs_ScopeGuard.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/misc/higgs_ScopeGuard.h) |
| `Result` | Representa el 'éxito' o el 'fracaso' de una operación y contiene un mensaje de error asociado para describir el error cuando se produce un fallo. | [`higgs_Result.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/misc/higgs_Result.h) |
| `Uuid` | Un identificador único universal de 128 bits. | [`higgs_Uuid.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/misc/higgs_Uuid.h) |
| `ArgumentList` | Contiene una lista de argumentos de línea de comandos y proporciona métodos útiles para buscarlos y operar sobre ellos. | [`higgs_ConsoleApplication.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/misc/higgs_ConsoleApplication.h) |
| `ConsoleApplication` | Representa el conjunto de comandos que puede ejecutar una aplicación de consola y proporciona funciones auxiliares para ejecutarlos. | [`higgs_ConsoleApplication.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/misc/higgs_ConsoleApplication.h) |
| `RuntimePermissions` | Clase para gestionar los permisos en tiempo de ejecución de la aplicación para ciertas funcionalidades en algunas plataformas. | [`higgs_RuntimePermissions.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/misc/higgs_RuntimePermissions.h) |
| `WindowsRegistry` | Contiene algunas funciones auxiliares estáticas para manipular el registro de MS Windows (¡solo disponible en Windows, por supuesto!). | [`higgs_WindowsRegistry.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/misc/higgs_WindowsRegistry.h) |
| `OptionsBuilder` | Una clase base para construir Options. | [`higgs_OptionsHelpers.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/misc/higgs_OptionsHelpers.h) |

### `memory`

| Clase | Descripción | Cabecera |
|---|---|---|
| `ScopedAutoReleasePool` | Un práctico envoltorio de C++ que crea y elimina un objeto NSAutoreleasePool mediante RAII. | [`higgs_Memory.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/memory/higgs_Memory.h) |
| `ByteOrder` | Contiene métodos estáticos para convertir el orden de bytes entre distintos tipos de endianness. | [`higgs_ByteOrder.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/memory/higgs_ByteOrder.h) |
| `Atomic` | Un contenedor simple de std::atomic. | [`higgs_Atomic.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/memory/higgs_Atomic.h) |
| `LeakedObjectDetector` | Incrustar una instancia de esta clase dentro de otra clase sirve como una forma de baja sobrecarga para detectar instancias con fugas. | [`higgs_LeakedObjectDetector.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/memory/higgs_LeakedObjectDetector.h) |
| `ContainerDeletePolicy` | Utilizada por las clases contenedoras como forma indirecta de eliminar un objeto de un tipo concreto. | [`higgs_ContainerDeletePolicy.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/memory/higgs_ContainerDeletePolicy.h) |
| `HeapBlock` | Clase contenedora muy sencilla para guardar un puntero a datos en el heap. | [`higgs_HeapBlock.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/memory/higgs_HeapBlock.h) |
| `MemoryBlock` | Clase para contener un bloque redimensionable de datos sin procesar. | [`higgs_MemoryBlock.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/memory/higgs_MemoryBlock.h) |
| `CopyableHeapBlock` | Envuelve un HeapBlock, pero además proporciona un constructor de copia y recuerda su tamaño. | [`higgs_CopyableHeapBlock.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/memory/higgs_CopyableHeapBlock.h) |
| `ReferenceCountedObject` | Clase base que proporciona métodos para el conteo de referencias. | [`higgs_ReferenceCountedObject.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/memory/higgs_ReferenceCountedObject.h) |
| `SingleThreadedReferenceCountedObject` | Agrega conteo de referencias a un objeto. | [`higgs_ReferenceCountedObject.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/memory/higgs_ReferenceCountedObject.h) |
| `ReferenceCountedObjectPtr` | Una clase de puntero inteligente que apunta a un objeto con conteo de referencias. | [`higgs_ReferenceCountedObject.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/memory/higgs_ReferenceCountedObject.h) |
| `ScopedPointer` | Esta clase está obsoleta. | [`higgs_ScopedPointer.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/memory/higgs_ScopedPointer.h) |
| `OptionalScopedPointer` | Contiene un puntero a un objeto que, opcionalmente, se puede eliminar cuando este puntero sale del ámbito. | [`higgs_OptionalScopedPointer.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/memory/higgs_OptionalScopedPointer.h) |
| `SingletonHolder` | Usado por las macros HIGGS_DECLARE_SINGLETON para administrar un puntero estático a una instancia singleton. | [`higgs_Singleton.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/memory/higgs_Singleton.h) |
| `WeakReference` | Esta clase actúa como un puntero que se vuelve nulo automáticamente si se elimina el objeto al que apunta. | [`higgs_WeakReference.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/memory/higgs_WeakReference.h) |
| `HeavyweightLeakedObjectDetector` | Esta clase es una forma útil de rastrear fugas de memoria difíciles de encontrar cuando el LeakedObjectDetector habitual no es suficiente. | [`higgs_HeavyweightLeakedObjectDetector.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/memory/higgs_HeavyweightLeakedObjectDetector.h) |
| `SharedResourcePointer` | Puntero inteligente que crea automáticamente y administra la vida útil de una instancia estática compartida de una clase. | [`higgs_SharedResourcePointer.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/memory/higgs_SharedResourcePointer.h) |
| `UnitTestAllocationChecker` | Verificador con ámbito que provoca un fallo en la prueba unitaria si se realiza alguna llamada a new/delete durante la vida útil del UnitTestAllocationChecker. | [`higgs_AllocationHooks.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/memory/higgs_AllocationHooks.h) |
| `Reservoir` | Funciones auxiliares para gestionar lectores con búfer. | [`higgs_Reservoir.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/memory/higgs_Reservoir.h) |

### `containers`

| Clase | Descripción | Cabecera |
|---|---|---|
| `Span` | Una vista no propietaria sobre objetos contiguos almacenados en un Array, un vector u otro contenedor similar. | [`higgs_Span.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/containers/higgs_Span.h) |
| `Enumerated` | Se devuelve al desreferenciar un EnumerateIterator. | [`higgs_Enumerate.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/containers/higgs_Enumerate.h) |
| `EnumerateIterator` | Un iterador que envuelve otro iterador y lleva el registro de su posición relativa según las llamadas a operadores aritméticos como operator++(), operator--(), operator+() y operator-(). | [`higgs_Enumerate.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/containers/higgs_Enumerate.h) |
| `IteratorPair` | Envuelve un par de iteradores y proporciona las funciones miembro begin() y end() que devuelven dichos iteradores. | [`higgs_Enumerate.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/containers/higgs_Enumerate.h) |
| `ScopedValueSetter` | Clase auxiliar que proporciona un mecanismo basado en RAII para establecer temporalmente un valor y luego restablecerlo. | [`higgs_ScopedValueSetter.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/containers/higgs_ScopedValueSetter.h) |
| `SortFunctionConverter` | Esta es una clase auxiliar interna que convierte una clase de estilo ElementComparator de higgs (que usa un método "compareElements") en una clase compatible con std::sort. | [`higgs_ElementComparator.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/containers/higgs_ElementComparator.h) |
| `DefaultElementComparator` | Una clase ElementComparator sencilla que se puede usar para ordenar un arreglo de objetos que admiten el operador '<'. | [`higgs_ElementComparator.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/containers/higgs_ElementComparator.h) |
| `ArrayAllocationBase` | Implementa algunas funciones básicas de asignación de almacenamiento para arreglos. | [`higgs_ArrayAllocationBase.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/containers/higgs_ArrayAllocationBase.h) |
| `ArrayBase` | Contenedor básico de objetos. | [`higgs_ArrayBase.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/containers/higgs_ArrayBase.h) |
| `Array` | Contiene un arreglo redimensionable de objetos primitivos o que se copian por valor. | [`higgs_Array.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/containers/higgs_Array.h) |
| `LinkedListPointer` | Ayuda a manipular listas simplemente enlazadas de objetos. | [`higgs_LinkedListPointer.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/containers/higgs_LinkedListPointer.h) |
| `ListenerList` | Contiene un conjunto de objetos y puede invocar un callback de función miembro en cada objeto del conjunto con una sola llamada. | [`higgs_ListenerList.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/containers/higgs_ListenerList.h) |
| `LightweightListenerList` | Una versión ligera de ListenerList que no ofrece garantías al modificar la lista desde un callback, pero permite que los callbacks se disparen de forma concurrente sin un mutex. | [`higgs_ListenerList.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/containers/higgs_ListenerList.h) |
| `OwnedArray` | Un arreglo diseñado para contener objetos. | [`higgs_OwnedArray.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/containers/higgs_OwnedArray.h) |
| `ReferenceCountedArray` | Contiene una lista de objetos derivados de ReferenceCountedObject o que implementan métodos básicos de manejo del conteo de referencias. | [`higgs_ReferenceCountedArray.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/containers/higgs_ReferenceCountedArray.h) |
| `SparseSet` | Contiene un conjunto de valores primitivos y los almacena como un conjunto de rangos. | [`higgs_SparseSet.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/containers/higgs_SparseSet.h) |
| `AbstractFifo` | Encapsula la lógica necesaria para implementar una FIFO sin bloqueos. | [`higgs_AbstractFifo.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/containers/higgs_AbstractFifo.h) |
| `SingleThreadedAbstractFifo` | Encapsula la lógica de una FIFO de un solo hilo. | [`higgs_SingleThreadedAbstractFifo.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/containers/higgs_SingleThreadedAbstractFifo.h) |
| `var` | Una clase variante que se puede usar para contener diversos valores primitivos. | [`higgs_Variant.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/containers/higgs_Variant.h) |
| `NamedValue` | Estructura para un objeto var con nombre, utilizada como elemento de un NamedValueSet. | [`higgs_NamedValueSet.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/containers/higgs_NamedValueSet.h) |
| `NamedValueSet` | Contiene un conjunto de objetos var con nombre. | [`higgs_NamedValueSet.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/containers/higgs_NamedValueSet.h) |
| `DynamicObject` | Representa un objeto implementado dinámicamente. | [`higgs_DynamicObject.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/containers/higgs_DynamicObject.h) |
| `DefaultHashFunctions` | Una clase sencilla para generar funciones hash para algunos tipos primitivos, pensada para usarse con la clase HashMap. | [`higgs_HashMap.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/containers/higgs_HashMap.h) |
| `HashMap` | Contiene un conjunto de asociaciones entre pares clave/valor. | [`higgs_HashMap.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/containers/higgs_HashMap.h) |
| `PropertySet` | Un conjunto de valores de propiedades con nombre, que pueden ser cadenas, enteros, de punto flotante, etc. | [`higgs_PropertySet.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/containers/higgs_PropertySet.h) |

### `maths`

| Clase | Descripción | Cabecera |
|---|---|---|
| `MathConstants` | Constantes matemáticas de uso común. | [`higgs_MathsFunctions.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/maths/higgs_MathsFunctions.h) |
| `Tolerance` | Una clase que encapsula tolerancias relativas y absolutas para usar en comparaciones de punto flotante. | [`higgs_MathsFunctions.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/maths/higgs_MathsFunctions.h) |
| `ParameterType` | La estructura ParameterType se usa para determinar el mejor tipo a utilizar al pasar algún tipo de objeto como parámetro. | [`higgs_MathsFunctions.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/maths/higgs_MathsFunctions.h) |
| `UnsignedTypeWithSize` | Estas plantillas están diseñadas para tomar un tipo entero y devolver una versión unsigned int del mismo tamaño. | [`higgs_MathsFunctions.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/maths/higgs_MathsFunctions.h) |
| `Range` | Un objeto de rango de propósito general que simplemente representa cualquier rango lineal con un punto inicial y uno final. | [`higgs_Range.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/maths/higgs_Range.h) |
| `NormalisableRange` | Representa un mapeo entre un rango arbitrario de valores y un rango normalizado 0->1. | [`higgs_NormalisableRange.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/maths/higgs_NormalisableRange.h) |
| `StatisticsAccumulator` | Clase que mide diversas estadísticas sobre una serie de valores de punto flotante que recibe. | [`higgs_StatisticsAccumulator.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/maths/higgs_StatisticsAccumulator.h) |
| `BigInteger` | Una clase de enteros de tamaño arbitrario. | [`higgs_BigInteger.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/maths/higgs_BigInteger.h) |
| `Expression` | Una clase para evaluar dinámicamente expresiones numéricas simples. | [`higgs_Expression.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/maths/higgs_Expression.h) |
| `Random` | Un generador de números aleatorios. | [`higgs_Random.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/maths/higgs_Random.h) |

### `text`

| Clase | Descripción | Cabecera |
|---|---|---|
| `CharacterFunctions` | Una colección de funciones para manipular caracteres y cadenas de caracteres. | [`higgs_CharacterFunctions.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/text/higgs_CharacterFunctions.h) |
| `CharPointer_UTF8` | Envuelve un puntero a una cadena de caracteres UTF-8 terminada en nulo y proporciona varios métodos para operar sobre los datos. | [`higgs_CharPointer_UTF8.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/text/higgs_CharPointer_UTF8.h) |
| `CharPointer_UTF16` | Envuelve un puntero a una cadena de caracteres UTF-16 terminada en nulo y proporciona varios métodos para operar sobre los datos. | [`higgs_CharPointer_UTF16.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/text/higgs_CharPointer_UTF16.h) |
| `CharPointer_UTF32` | Envuelve un puntero a una cadena de caracteres UTF-32 terminada en nulo y proporciona varios métodos para operar sobre los datos. | [`higgs_CharPointer_UTF32.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/text/higgs_CharPointer_UTF32.h) |
| `CharPointer_ASCII` | Envuelve un puntero a una cadena de caracteres ASCII terminada en nulo y proporciona diversos métodos para operar sobre los datos. | [`higgs_CharPointer_ASCII.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/text/higgs_CharPointer_ASCII.h) |
| `String` | ¡La clase String de HIGGS! | [`higgs_String.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/text/higgs_String.h) |
| `StringRef` | Clase sencilla para mantener referencias temporales a un literal de cadena o a un String. | [`higgs_StringRef.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/text/higgs_StringRef.h) |
| `NewLine` | Esta clase se usa para representar una secuencia de caracteres de nueva línea. | [`higgs_NewLine.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/text/higgs_NewLine.h) |
| `StringPool` | Un StringPool contiene un conjunto de cadenas compartidas, lo que reduce la sobrecarga de almacenamiento y mejora la velocidad de comparación al trabajar con muchas cadenas duplicadas. | [`higgs_StringPool.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/text/higgs_StringPool.h) |
| `Identifier` | Representa un identificador de cadena, diseñado para acceder a propiedades por nombre. | [`higgs_Identifier.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/text/higgs_Identifier.h) |
| `StringArray` | Un arreglo especial para contener una lista de cadenas. | [`higgs_StringArray.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/text/higgs_StringArray.h) |
| `StringPairArray` | Un contenedor para almacenar un conjunto de cadenas indexadas por otra cadena. | [`higgs_StringPairArray.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/text/higgs_StringPairArray.h) |
| `TextDiff` | Calcula y aplica una secuencia de cambios para convertir una cadena de texto en otra. | [`higgs_TextDiff.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/text/higgs_TextDiff.h) |
| `LocalisedStrings` | Se usa para convertir cadenas en versiones localizadas en otros idiomas. | [`higgs_LocalisedStrings.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/text/higgs_LocalisedStrings.h) |
| `Base64` | Contiene algunos métodos estáticos para convertir entre binario y el formato de codificación base-64 estándar. | [`higgs_Base64.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/text/higgs_Base64.h) |

### `logging`

| Clase | Descripción | Cabecera |
|---|---|---|
| `Logger` | Actúa como una clase de registro (logging) para toda la aplicación. | [`higgs_Logger.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/logging/higgs_Logger.h) |
| `FileLogger` | Una implementación sencilla de un Logger que escribe en un archivo. | [`higgs_FileLogger.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/logging/higgs_FileLogger.h) |

### `threads`

| Clase | Descripción | Cabecera |
|---|---|---|
| `GenericScopedLock` | Bloquea y desbloquea automáticamente un objeto mutex. | [`higgs_ScopedLock.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/threads/higgs_ScopedLock.h) |
| `GenericScopedUnlock` | Desbloquea y vuelve a bloquear automáticamente un objeto mutex. | [`higgs_ScopedLock.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/threads/higgs_ScopedLock.h) |
| `GenericScopedTryLock` | Bloquea y desbloquea automáticamente un objeto mutex. | [`higgs_ScopedLock.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/threads/higgs_ScopedLock.h) |
| `CriticalSection` | Un mutex reentrante. | [`higgs_CriticalSection.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/threads/higgs_CriticalSection.h) |
| `DummyCriticalSection` | Una clase que puede usarse en lugar de un objeto CriticalSection real, pero que no realiza ningún bloqueo. | [`higgs_CriticalSection.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/threads/higgs_CriticalSection.h) |
| `ChildProcess` | Inicia y supervisa un proceso hijo. | [`higgs_ChildProcess.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/threads/higgs_ChildProcess.h) |
| `DynamicLibrary` | Gestiona la apertura y el cierre de DLL. | [`higgs_DynamicLibrary.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/threads/higgs_DynamicLibrary.h) |
| `InterProcessLock` | Actúa como una sección crítica que los procesos pueden usar para bloquearse entre sí. | [`higgs_InterProcessLock.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/threads/higgs_InterProcessLock.h) |
| `Process` | Representa el proceso del ejecutable actual. | [`higgs_Process.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/threads/higgs_Process.h) |
| `SpinLock` | Clase de spin-lock sencilla que se puede usar como un mutex simple y de baja sobrecarga en situaciones sin contención. | [`higgs_SpinLock.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/threads/higgs_SpinLock.h) |
| `WaitableEvent` | Permite que los hilos esperen eventos disparados por otros hilos. | [`higgs_WaitableEvent.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/threads/higgs_WaitableEvent.h) |
| `Thread` | Encapsula un hilo. | [`higgs_Thread.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/threads/higgs_Thread.h) |
| `HighResolutionTimer` | Un temporizador periódico de alta resolución. | [`higgs_HighResolutionTimer.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/threads/higgs_HighResolutionTimer.h) |
| `ThreadLocalValue` | Proporciona soporte multiplataforma para objetos locales de hilo. | [`higgs_ThreadLocalValue.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/threads/higgs_ThreadLocalValue.h) |
| `ThreadPoolJob` | Una tarea que ejecuta un objeto ThreadPool. | [`higgs_ThreadPoolJob.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/threads/higgs_ThreadPoolJob.h) |
| `ThreadPoolOptions` | Un conjunto de hilos que ejecutarán una lista de trabajos. | [`higgs_ThreadPool.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/threads/higgs_ThreadPool.h) |
| `ThreadPool` | Un conjunto de hilos que ejecutarán una lista de trabajos. | [`higgs_ThreadPool.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/threads/higgs_ThreadPool.h) |
| `TimeSliceClient` | Usado por la clase TimeSliceThread. | [`higgs_TimeSliceThread.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/threads/higgs_TimeSliceThread.h) |
| `TimeSliceThread` | Un hilo que mantiene una lista de clientes y llama a cada uno por turno, dándoles a todos la oportunidad de ejecutar algún tipo de tarea breve. | [`higgs_TimeSliceThread.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/threads/higgs_TimeSliceThread.h) |
| `ReadWriteLock` | Una sección crítica que permite múltiples lectores simultáneos. | [`higgs_ReadWriteLock.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/threads/higgs_ReadWriteLock.h) |
| `ScopedReadLock` | Bloquea y desbloquea automáticamente un objeto ReadWriteLock. | [`higgs_ScopedReadLock.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/threads/higgs_ScopedReadLock.h) |
| `ScopedTryReadLock` | Bloquea y desbloquea automáticamente un objeto ReadWriteLock. | [`higgs_ScopedReadLock.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/threads/higgs_ScopedReadLock.h) |
| `ScopedWriteLock` | Bloquea y desbloquea automáticamente un objeto ReadWriteLock. | [`higgs_ScopedWriteLock.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/threads/higgs_ScopedWriteLock.h) |
| `ScopedTryWriteLock` | Bloquea y desbloquea automáticamente un objeto ReadWriteLock. | [`higgs_ScopedWriteLock.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/threads/higgs_ScopedWriteLock.h) |

### `system`

| Clase | Descripción | Cabecera |
|---|---|---|
| `SystemStats` | Contiene métodos para obtener información sobre la configuración actual del hardware y del sistema operativo. | [`higgs_SystemStats.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/system/higgs_SystemStats.h) |

### `json`

| Clase | Descripción | Cabecera |
|---|---|---|
| `JSON` | Contiene métodos estáticos para convertir texto con formato JSON a objetos var y viceversa. | [`higgs_JSON.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/json/higgs_JSON.h) |
| `JSONUtils` | Un pequeño espacio de nombres que contiene funciones utilitarias para trabajar con higgs::vars. | [`higgs_JSONUtils.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/json/higgs_JSONUtils.h) |
| `ToVarOptions` | Opciones que controlan la conversión de tipos arbitrarios a higgs::var. | [`higgs_JSONSerialisation.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/json/higgs_JSONSerialisation.h) |
| `ToVar` | Permite convertir un objeto de tipo arbitrario a var. | [`higgs_JSONSerialisation.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/json/higgs_JSONSerialisation.h) |
| `FromVar` | Permite convertir un var a un objeto de tipo arbitrario. | [`higgs_JSONSerialisation.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/json/higgs_JSONSerialisation.h) |
| `VariantConverter` | Esta clase con sobrecarga de plantillas puede usarse para convertir entre var y tipos personalizados. | [`higgs_JSONSerialisation.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/json/higgs_JSONSerialisation.h) |
| `StrictVariantConverter` | Tipo auxiliar que se puede usar para implementar especializaciones de VariantConverter que usan FromVar::convert y ToVar::convert internamente. | [`higgs_JSONSerialisation.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/json/higgs_JSONSerialisation.h) |

### `time`

| Clase | Descripción | Cabecera |
|---|---|---|
| `RelativeTime` | Una medida relativa de tiempo. | [`higgs_RelativeTime.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/time/higgs_RelativeTime.h) |
| `Time` | Contiene una fecha y hora absolutas. | [`higgs_Time.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/time/higgs_Time.h) |
| `PerformanceCounter` | Un temporizador para medir el rendimiento del código y volcar los resultados en un archivo. | [`higgs_PerformanceCounter.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/time/higgs_PerformanceCounter.h) |
| `ScopedTimeMeasurement` | Clase RAII sencilla para medir el tiempo transcurrido en un ámbito. | [`higgs_PerformanceCounter.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/time/higgs_PerformanceCounter.h) |
| `TimedDiagnostic` | Un objeto para almacenar y medir duraciones con fines de diagnóstico. | [`higgs_TimedDiagnostic.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/time/higgs_TimedDiagnostic.h) |

### `streams`

| Clase | Descripción | Cabecera |
|---|---|---|
| `InputStream` | La clase base para los flujos que leen datos. | [`higgs_InputStream.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/streams/higgs_InputStream.h) |
| `OutputStream` | La clase base para flujos que escriben datos en algún tipo de destino. | [`higgs_OutputStream.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/streams/higgs_OutputStream.h) |
| `BufferedInputStream` | Envuelve otro flujo de entrada y lee de él mediante un búfer intermedio; si se usa un flujo de entrada, como un flujo de entrada de archivo, y se realizan muchos accesos de lectura pequeños, probablemente convenga envolverlo en uno de estos para que el flujo de origen se acceda en bloques más grandes, lo que implica menos trabajo para el flujo subyacente. | [`higgs_BufferedInputStream.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/streams/higgs_BufferedInputStream.h) |
| `MemoryInputStream` | Permite acceder a un bloque de datos como un flujo. | [`higgs_MemoryInputStream.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/streams/higgs_MemoryInputStream.h) |
| `MemoryOutputStream` | Escribe datos en un búfer de memoria interno, que crece según sea necesario. | [`higgs_MemoryOutputStream.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/streams/higgs_MemoryOutputStream.h) |
| `SubregionStream` | Envuelve otro flujo de entrada y lee de una parte específica de él. | [`higgs_SubregionStream.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/streams/higgs_SubregionStream.h) |
| `InputSource` | Un objeto ligero que puede crear un flujo para leer algún tipo de recurso. | [`higgs_InputSource.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/streams/higgs_InputSource.h) |
| `FileInputSource` | Un tipo de InputSource que representa un archivo normal. | [`higgs_FileInputSource.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/streams/higgs_FileInputSource.h) |
| `URLInputSource` | Un tipo de InputSource que representa una URL. | [`higgs_URLInputSource.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/streams/higgs_URLInputSource.h) |
| `AndroidDocumentInputSource` | InputSource respaldado por un AndroidDocument. | [`higgs_AndroidDocumentInputSource.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/streams/higgs_AndroidDocumentInputSource.h) |

### `files`

| Clase | Descripción | Cabecera |
|---|---|---|
| `File` | Representa un archivo o directorio local. | [`higgs_File.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/files/higgs_File.h) |
| `DirectoryIterator` | Esta clase ahora está obsoleta en favor de RangedDirectoryIterator. | [`higgs_DirectoryIterator.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/files/higgs_DirectoryIterator.h) |
| `RangedDirectoryIterator` | Permite iterar sobre archivos y carpetas usando la sintaxis range-for de C++11. | [`higgs_RangedDirectoryIterator.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/files/higgs_RangedDirectoryIterator.h) |
| `FileInputStream` | Un flujo de entrada que lee de un archivo local. | [`higgs_FileInputStream.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/files/higgs_FileInputStream.h) |
| `FileOutputStream` | Un flujo de salida que escribe en un archivo local. | [`higgs_FileOutputStream.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/files/higgs_FileOutputStream.h) |
| `FileSearchPath` | Representa un conjunto de carpetas que conforman una ruta de búsqueda. | [`higgs_FileSearchPath.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/files/higgs_FileSearchPath.h) |
| `MemoryMappedFile` | Mapea un archivo en la memoria virtual para facilitar su lectura o escritura. | [`higgs_MemoryMappedFile.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/files/higgs_MemoryMappedFile.h) |
| `TemporaryFile` | Gestiona un archivo temporal, que se eliminará cuando se elimine este objeto. | [`higgs_TemporaryFile.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/files/higgs_TemporaryFile.h) |
| `FileFilter` | Interfaz para decidir qué archivos son adecuados para algo. | [`higgs_FileFilter.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/files/higgs_FileFilter.h) |
| `WildcardFileFilter` | Un tipo de FileFilter que funciona mediante coincidencia de patrones con comodines. | [`higgs_WildcardFileFilter.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/files/higgs_WildcardFileFilter.h) |
| `AndroidDocumentInfo` | Información sobre un documento. | [`higgs_AndroidDocument.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/files/higgs_AndroidDocument.h) |
| `AndroidDocumentPermission` | Representa un permiso otorgado a una aplicación para leer y/o escribir en un documento o árbol específico. | [`higgs_AndroidDocument.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/files/higgs_AndroidDocument.h) |
| `AndroidDocument` | Proporciona acceso a un documento en dispositivos Android. | [`higgs_AndroidDocument.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/files/higgs_AndroidDocument.h) |
| `AndroidDocumentIterator` | Un iterador que recorre los documentos hijos de un directorio. | [`higgs_AndroidDocument.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/files/higgs_AndroidDocument.h) |

### `serialisation`

| Clase | Descripción | Cabecera |
|---|---|---|
| `SerialisationTraits` | Permite asociar funciones de serialización a un tipo específico sin tener que modificar la declaración de ese tipo. | [`higgs_Serialisation.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/serialisation/higgs_Serialisation.h) |
| `Named` | Combina un objeto con un nombre. | [`higgs_Serialisation.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/serialisation/higgs_Serialisation.h) |
| `SerialisationSize` | Contiene una referencia a algún tipo de valor de tamaño, utilizada para indicar que un objeto que se está serializando tiene tamaño variable. | [`higgs_Serialisation.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/serialisation/higgs_Serialisation.h) |

### `network`

| Clase | Descripción | Cabecera |
|---|---|---|
| `IPAddress` | Representa una dirección IP. | [`higgs_IPAddress.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/network/higgs_IPAddress.h) |
| `MACAddress` | Representa el ID de dirección MAC de un adaptador de tarjeta de red. | [`higgs_MACAddress.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/network/higgs_MACAddress.h) |
| `NamedPipe` | Una tubería entre procesos en la que se pueden escribir y leer datos. | [`higgs_NamedPipe.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/network/higgs_NamedPipe.h) |
| `SocketOptions` | Opciones utilizadas para configurar el socket del sistema subyacente en las clases StreamingSocket y DatagramSocket. | [`higgs_Socket.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/network/higgs_Socket.h) |
| `StreamingSocket` | Envoltorio para un socket de flujo (TCP). | [`higgs_Socket.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/network/higgs_Socket.h) |
| `DatagramSocket` | Un contenedor para un socket de datagramas (UDP). | [`higgs_Socket.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/network/higgs_Socket.h) |
| `URL` | Representa una URL y ofrece varias funciones útiles para manipularla. | [`higgs_URL.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/network/higgs_URL.h) |
| `WebInputStream` | InputStream que se puede usar para leer desde una URL dada. | [`higgs_WebInputStream.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/network/higgs_WebInputStream.h) |

### `unit_tests`

| Clase | Descripción | Cabecera |
|---|---|---|
| `UnitTest` | Esta es una clase base para las clases que realizan una prueba unitaria. | [`higgs_UnitTest.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/unit_tests/higgs_UnitTest.h) |
| `UnitTestRunner` | Ejecuta un conjunto de pruebas unitarias. | [`higgs_UnitTest.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/unit_tests/higgs_UnitTest.h) |

### `xml`

| Clase | Descripción | Cabecera |
|---|---|---|
| `XmlDocument` | Analiza un documento XML en formato de texto y crea a partir de él un objeto XmlElement. | [`higgs_XmlDocument.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/xml/higgs_XmlDocument.h) |
| `XmlAttribute` | Un par nombre-valor que representa un atributo de una etiqueta XML. | [`higgs_XmlElement.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/xml/higgs_XmlElement.h) |
| `XmlElement` | Se usa para construir un árbol de elementos que representa un documento XML. | [`higgs_XmlElement.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/xml/higgs_XmlElement.h) |

### `zip`

| Clase | Descripción | Cabecera |
|---|---|---|
| `GZIPCompressorOutputStream` | Flujo que usa zlib para comprimir los datos que se escriben en él. | [`higgs_GZIPCompressorOutputStream.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/zip/higgs_GZIPCompressorOutputStream.h) |
| `GZIPDecompressorInputStream` | Este flujo descomprime un flujo de origen usando zlib. | [`higgs_GZIPDecompressorInputStream.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/zip/higgs_GZIPDecompressorInputStream.h) |
| `ZipFile` | Decodifica un archivo ZIP desde un flujo. | [`higgs_ZipFile.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_core/zip/higgs_ZipFile.h) |
