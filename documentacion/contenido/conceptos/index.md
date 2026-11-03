# Conceptos

Antes de profundizar en las guías conviene entender las ideas sobre las que se apoya todo el
framework:

- **[Arquitectura del framework](arquitectura.md)**: qué es un módulo, cómo se organizan en capas y
  cómo se resuelven sus dependencias.
- **[Convenciones de nombres](convenciones.md)**: el espacio de nombres `higgs`, los prefijos
  `HIGGS_` y `higgs_`, y el resto de reglas de identidad del proyecto.
- **[Bucle de mensajes y eventos](eventos.md)**: el hilo de mensajes, los temporizadores, los
  mensajes asíncronos y los patrones de difusión (*broadcasters*).
- **[Hilos y concurrencia](hilos.md)**: `Thread`, `ThreadPool`, bloqueos, colas sin bloqueo y las
  reglas del hilo de audio.
- **[Memoria, cadenas y contenedores](nucleo.md)**: `String`, `Array`, `OwnedArray`, `var`,
  punteros con recuento de referencias y otras piezas de `higgs_core`.
- **[Depuración y diagnóstico](depuracion.md)**: aserciones, registro, detección de fugas y
  opciones de depuración.
