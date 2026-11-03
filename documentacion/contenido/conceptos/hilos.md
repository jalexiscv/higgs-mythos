# Hilos y concurrencia

`higgs_core` ofrece primitivas de hilos portables. Las aplicaciones de audio suelen tener al
menos tres tipos de hilo: el **hilo de mensajes** (interfaz), el **hilo de audio** (tiempo real)
y uno o varios **hilos de trabajo** (carga de archivos, red, análisis).

## `Thread`

```cpp
class Cargador final : public higgs::Thread
{
public:
    Cargador() : Thread ("Cargador de muestras") {}
    ~Cargador() override { stopThread (2000); }      // espera hasta 2 s a que termine

    void run() override
    {
        while (! threadShouldExit())
        {
            procesarSiguienteArchivo();
            wait (100);                               // duerme o despierta con notify()
        }
    }
};

Cargador cargador;
cargador.startThread();
```

Puntos importantes:

- `run()` debe comprobar periódicamente `threadShouldExit()` y salir cuanto antes cuando devuelva
  `true`.
- `stopThread (ms)` pide la salida con `signalThreadShouldExit()` y espera; si el hilo no termina
  a tiempo, lo fuerza (algo que conviene evitar).
- `Thread::launch (función)` lanza un hilo efímero que ejecuta una lambda.
- `startRealtimeThread (RealtimeOptions)` crea hilos con prioridad de tiempo real, útiles para
  procesamiento de audio fuera del *callback* del dispositivo.

## Grupos de hilos y tareas

```cpp
higgs::ThreadPool pool;   // por defecto, un hilo por núcleo

pool.addJob ([]
{
    analizarArchivo();
});
```

`ThreadPool` ejecuta tareas (`ThreadPoolJob` o lambdas) con un número fijo de hilos.
`TimeSliceThread` comparte un único hilo entre varios clientes que reciben porciones de tiempo,
como hace `BufferingAudioSource` para leer audio de disco.

## Sincronización

| Clase | Descripción |
|---|---|
| `CriticalSection` | Mutex reentrante. Se usa con `const ScopedLock sl (cs);` |
| `SpinLock` | Bloqueo activo muy ligero para secciones extremadamente cortas |
| `ReadWriteLock` | Varios lectores simultáneos o un único escritor |
| `WaitableEvent` | Un hilo espera hasta que otro lo señaliza |
| `InterProcessLock` | Exclusión mutua entre procesos |
| `ScopedLock`, `ScopedUnlock`, `ScopedTryLock` | Guardas RAII para los anteriores |
| `Atomic<T>` | Envoltorio sencillo sobre `std::atomic` |

## Colas sin bloqueo

`AbstractFifo` implementa la lógica de un FIFO de un productor y un consumidor sin bloqueos, ideal
para pasar datos entre el hilo de audio y la interfaz:

```cpp
class ColaDeNiveles
{
public:
    void push (float valor)                 // hilo de audio
    {
        const auto scope = fifo.write (1);
        if (scope.blockSize1 > 0) datos[(size_t) scope.startIndex1] = valor;
    }

    bool pop (float& valor)                 // hilo de interfaz
    {
        const auto scope = fifo.read (1);
        if (scope.blockSize1 == 0) return false;
        valor = datos[(size_t) scope.startIndex1];
        return true;
    }

private:
    higgs::AbstractFifo fifo { 1024 };
    std::array<float, 1024> datos {};
};
```

## Reglas del hilo de audio

El *callback* de audio (`AudioIODeviceCallback`, `AudioSource::getNextAudioBlock()`,
`AudioProcessor::processBlock()`) se ejecuta con plazos de unos pocos milisegundos. Dentro de él:

- [x] Usa variables atómicas, colas sin bloqueos y memoria preasignada en `prepareToPlay()`.
- [x] Usa `ScopedNoDenormals` para evitar números desnormalizados.
- [ ] No reserves ni liberes memoria (`new`, `std::vector::push_back` que crezca, `String` nuevos…).
- [ ] No bloquees mutex que pueda tener otro hilo, ni llames a `MessageManagerLock`.
- [ ] No hagas E/S de disco ni de red, ni escribas en el registro.
- [ ] No llames a métodos de componentes de la interfaz.

`AudioProcessLoadMeasurer` (`higgs_audio_basics`) mide qué proporción del tiempo disponible
consume tu procesamiento. En Apple, `AudioWorkgroup` permite unir tus hilos de trabajo al grupo
de trabajo de audio del sistema (véase el ejemplo `AudioWorkgroupDemo`).
