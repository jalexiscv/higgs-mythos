# Depuración y diagnóstico

## Aserciones

| Macro | Comportamiento |
|---|---|
| `jassert (condición)` | En depuración, si la condición es falsa, detiene el programa en el depurador. En *release* no genera código (salvo con `HIGGS_LOG_ASSERTIONS`). |
| `jassertfalse` | Equivale a `jassert (false)`: marca código que no debería alcanzarse. |
| `HIGGS_ASSERT_MESSAGE_THREAD` | Aserción de que se está en el hilo de mensajes. |
| `HIGGS_ASSERT_MESSAGE_MANAGER_IS_LOCKED` | Aserción de que el hilo actual tiene bloqueado el `MessageManager`. |
| `HIGGS_BREAK_IN_DEBUGGER` | Detiene la ejecución en el depurador. |

Muchas clases del framework usan `jassert` para avisarte de usos incorrectos del API. Si una
aserción salta dentro del framework, lee el comentario junto a ella: casi siempre explica qué se
hizo mal.

## Mensajes de depuración

```cpp
DBG ("Frecuencia de muestreo: " << sampleRate);   // solo en compilaciones de depuración
```

`DBG` escribe en la salida de depuración (la consola de Xcode o Visual Studio, `stderr` en Linux)
y desaparece en *release*. Para registros persistentes usa `Logger`:

```cpp
std::unique_ptr<higgs::FileLogger> log (higgs::FileLogger::createDefaultAppLogger ("MiApp",
                                                                                  "registro.txt",
                                                                                  "Inicio de MiApp"));
higgs::Logger::setCurrentLogger (log.get());
higgs::Logger::writeToLog ("Dispositivo de audio abierto");
```

Recuerda llamar a `Logger::setCurrentLogger (nullptr)` antes de destruir el registro.

## Detección de fugas de memoria

Añade `HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MiClase)` (o `HIGGS_LEAK_DETECTOR (MiClase)`)
a tus clases. En depuración, al cerrar la aplicación se lanza una aserción por cada clase con
instancias vivas, indicando su nombre. La opción `HIGGS_CHECK_MEMORY_LEAKS` controla esta
comprobación.

## Opciones útiles durante el desarrollo

| Opción | Efecto |
|---|---|
| `HIGGS_FORCE_DEBUG` | Fuerza `HIGGS_DEBUG` a 1 o 0 independientemente de la configuración. |
| `HIGGS_LOG_ASSERTIONS` | Registra cada aserción con `Logger::writeToLog()`, también en *release*. |
| `HIGGS_CHECK_MEMORY_LEAKS` | Activa la detección de fugas al terminar. |
| `HIGGS_CATCH_UNHANDLED_EXCEPTIONS` | Reenvía las excepciones no capturadas a `unhandledException()` de tu aplicación. |
| `HIGGS_ENABLE_ALLOCATION_HOOKS` | Añade funciones globales de reserva con aserciones, útiles en pruebas unitarias. |
| `HIGGS_ENABLE_REPAINT_DEBUGGING` | Cada zona repintada parpadea con un color aleatorio. |
| `HIGGS_ENABLE_LIVE_CONSTANT_EDITOR` | Permite ajustar constantes en caliente con `HIGGS_LIVE_CONSTANT` (solo escritorio). |
| `HIGGS_STRICT_REFCOUNTEDPOINTER` | Prohíbe la conversión implícita de `ReferenceCountedObjectPtr` a puntero crudo. |

Todas las opciones de configuración están en [Opciones de configuración](../compilacion/opciones.md).

## Ajustar constantes en caliente

Con `HIGGS_ENABLE_LIVE_CONSTANT_EDITOR` activado (por defecto en depuración), envolver un literal
en `HIGGS_LIVE_CONSTANT` abre una ventana con un control para modificarlo mientras la aplicación
se ejecuta:

```cpp
g.setColour (HIGGS_LIVE_CONSTANT (higgs::Colour (0xff4a90e2)));
g.fillRoundedRectangle (area, HIGGS_LIVE_CONSTANT (6.0f));
```

Es ideal para afinar colores y medidas de la interfaz. Véase el ejemplo `LiveConstantDemo`.

## Medir rendimiento

- `PerformanceCounter` y `ScopedTimeMeasurement` miden el tiempo de secciones de código.
- `AudioProcessLoadMeasurer` mide la carga del *callback* de audio.
- La aplicación `AudioPerformanceTest` de `extras` sirve para comparar rendimiento entre
  plataformas.

## Referencia del API generada con Doxygen

Los comentarios de las cabeceras pueden convertirse en una referencia HTML navegable (en inglés):

```bash
cd docs/doxygen
doxygen Doxyfile      # genera la referencia en docs/doxygen/doc
# o bien
python3 build.py      # ejecuta Doxygen y añade índices adicionales por módulo
```
