# UnitTestRunner

**UnitTestRunner** (`extras/UnitTestRunner`) es una aplicación de consola que compila todos los
módulos con `HIGGS_UNIT_TESTS=1` y ejecuta sus pruebas unitarias. La integración continua la usa
para validar cada cambio, y puedes usarla para comprobar que el framework funciona correctamente en
tu plataforma.

## Compilarla

```bash
cmake . -B cmake-build -DHIGGS_BUILD_EXTRAS=ON
cmake --build cmake-build --target UnitTestRunner
```

## Uso

```text
UnitTestRunner [--help|-h] [--list-categories|-l] [--category|-c=categoría] [--seed|-s=semilla] [--name|-n=nombre]
```

| Opción | Descripción |
|---|---|
| `--help`, `-h` | Muestra la ayuda. |
| `--list-categories`, `-l` | Lista las categorías de pruebas disponibles. |
| `--category`, `-c` | Ejecuta solo las pruebas de una categoría (se puede repetir). |
| `--name`, `-n` | Ejecuta solo la prueba con ese nombre (se puede repetir). |
| `--seed`, `-s` | Fija la semilla aleatoria, para reproducir exactamente una ejecución. |

Ejemplos:

```bash
UnitTestRunner --list-categories
UnitTestRunner --category=Audio --category=MIDI
UnitTestRunner --name="ValueTree" --seed=12345
```

El programa devuelve un código de salida distinto de cero si alguna prueba falla. Para escribir tus
propias pruebas, véase [Pruebas unitarias](../guias/pruebas.md).
