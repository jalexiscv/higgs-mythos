# higgs_cryptography

Funciones criptográficas básicas: claves RSA, cifrado Blowfish, números primos y los resúmenes MD5, SHA-256 y Whirlpool.

## Ficha

| Campo | Valor |
|---|---|
| Identificador | `higgs_cryptography` |
| Versión | 9.0.5 |
| Nombre declarado | Higgs Mythos cryptography classes |
| Descripción declarada | Classes for various basic cryptography functions, including RSA, Blowfish, MD5, SHA, etc. |
| Estándar mínimo de C++ | C++17 |
| Licencia | AGPLv3 |
| Dependencias directas | [`higgs_core`](higgs_core.md) |
| Módulos que dependen de él | [`higgs_product_unlocking`](higgs_product_unlocking.md) |
| Código fuente | [`modules/higgs_cryptography`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_cryptography) |

## Uso

```cmake
target_link_libraries(MiTarget PRIVATE higgs::higgs_cryptography)
```

```cpp
#include <higgs_cryptography/higgs_cryptography.h>
```

Guías relacionadas: [Otros módulos](../guias/otros-modulos.md#criptografia).

## Opciones de configuración

Este módulo no declara opciones de configuración.

## Catálogo de clases

6 clases y estructuras públicas declaradas en las cabeceras que incluye `higgs_cryptography.h`, agrupadas por carpeta. La descripción es la primera frase de su comentario de documentación en el código.

### `encryption`

| Clase | Descripción | Cabecera |
|---|---|---|
| `BlowFish` | Clase de cifrado BlowFish. | [`higgs_BlowFish.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_cryptography/encryption/higgs_BlowFish.h) |
| `Primes` | Clase para la generación de números primos. | [`higgs_Primes.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_cryptography/encryption/higgs_Primes.h) |
| `RSAKey` | Clase de cifrado RSA con par de claves pública/privada. | [`higgs_RSAKey.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_cryptography/encryption/higgs_RSAKey.h) |

### `hashing`

| Clase | Descripción | Cabecera |
|---|---|---|
| `MD5` | Clase de suma de verificación MD5. | [`higgs_MD5.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_cryptography/hashing/higgs_MD5.h) |
| `SHA256` | Generador de hash seguro SHA-256. | [`higgs_SHA256.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_cryptography/hashing/higgs_SHA256.h) |
| `Whirlpool` | Clase de hash Whirlpool. | [`higgs_Whirlpool.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_cryptography/hashing/higgs_Whirlpool.h) |
