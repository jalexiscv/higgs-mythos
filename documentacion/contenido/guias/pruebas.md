# Pruebas unitarias

`higgs_core` incluye un pequeño framework de pruebas unitarias (`UnitTest`, `UnitTestRunner`),
usado por el propio framework para probar sus módulos y disponible para tu código.

## Escribir una prueba

```cpp
class PruebasConversion final : public higgs::UnitTest
{
public:
    PruebasConversion() : UnitTest ("Conversión de decibelios", "Audio") {}   // nombre, categoría

    void runTest() override
    {
        beginTest ("0 dB es ganancia unidad");
        expectWithinAbsoluteError (higgs::Decibels::decibelsToGain (0.0f), 1.0f, 1.0e-6f);

        beginTest ("-inf dB es silencio");
        expectEquals (higgs::Decibels::decibelsToGain (-100.0f), 0.0f);

        beginTest ("Ida y vuelta");
        auto r = getRandom();
        for (int i = 0; i < 100; ++i)
        {
            const auto db = r.nextFloat() * 60.0f - 50.0f;
            expectWithinAbsoluteError (higgs::Decibels::gainToDecibels (higgs::Decibels::decibelsToGain (db)),
                                       db, 1.0e-3f);
        }
    }
};

static PruebasConversion pruebasConversion;   // se registra automáticamente
```

Comprobaciones disponibles: `expect`, `expectEquals`, `expectNotEquals`, `expectGreaterThan`,
`expectLessThan`, `expectGreaterOrEqual`, `expectLessOrEqual`, `expectWithinAbsoluteError`,
`expectDoesNotThrow`, `expectThrows`, `expectThrowsType`. `logMessage()` escribe en la salida, y
`getRandom()` devuelve un generador con semilla controlada por el ejecutor.

## Ejecutar las pruebas

```cpp
higgs::UnitTestRunner ejecutor;
ejecutor.setAssertOnFailure (false);
ejecutor.runAllTests();                      // o runTestsInCategory ("Audio")

for (int i = 0; i < ejecutor.getNumResults(); ++i)
    if (auto* r = ejecutor.getResult (i); r->failures > 0)
        DBG (r->unitTestName << " / " << r->subcategoryName << ": " << r->failures << " fallos");
```

Las pruebas internas de los módulos solo se compilan cuando `HIGGS_UNIT_TESTS=1`. Las categorías
estándar están en `UnitTestCategories` (`audio`, `containers`, `cryptography`, `dsp`, `files`,
`gui`, `json`, `midi`, `networking`, `xml`, etc.).

## El ejecutor de consola

La aplicación [UnitTestRunner](../herramientas/unit-test-runner.md) de `extras` compila todos los
módulos con `HIGGS_UNIT_TESTS=1` y permite ejecutar las pruebas desde la línea de comandos,
filtrando por categoría o nombre y fijando la semilla aleatoria; es la que usa la integración
continua. El ejemplo `UnitTestsDemo` ejecuta las mismas pruebas desde una interfaz gráfica.
