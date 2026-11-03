# Protocolo de Autorrevisión Adversarial

Este documento define la revisión que todo agente de IA (o desarrollador) debe
aplicar a su **propio trabajo** antes de entregarlo en **Higgs Mythos**. Entre "el
cambio está hecho y verificado" y "el cambio se entrega" hay una etapa
obligatoria: releer el diff completo **como un revisor hostil cuyo único trabajo es
encontrar el defecto**. La verificación (fase 5 de `Execution.md`) demuestra que lo
pedido funciona; esta revisión busca lo que **nadie pidió pero se rompió**, y los
casos que la prueba feliz no ejercitó. En una biblioteca, además, busca lo que
rompe a los proyectos ajenos que la usan.

---

## 1. Procedimiento

1. **Momento:** después de la verificación funcional y antes de documentar y
   reportar. Con el diff completo a la vista (`git diff`), no de memoria.
2. **Actitud:** se revisa asumiendo que **hay al menos un defecto** y que el
   trabajo es encontrarlo. "Se ve bien" no es una conclusión de revisión; la
   conclusión válida es "busqué las clases de defecto del §2 y encontré X" (donde
   X puede ser "ninguno", solo después de haber buscado de verdad).
3. **Cobertura:** archivo por archivo, fragmento por fragmento. Para cada
   fragmento del diff, la pregunta es siempre la misma: **¿qué entrada, estado,
   hilo, plataforma o secuencia de llamadas rompe esto?**
4. **Resultado:** cada defecto encontrado se corrige antes de entregar (volviendo
   a compilar y a probar lo corregido); cada duda que no se pueda resolver con
   evidencia se declara en el reporte final como riesgo, no se silencia.

---

## 2. Catálogo de Clases de Defecto

La revisión no es una relectura pasiva: es una **búsqueda dirigida** de estas
clases de defecto, una por una. Las clases marcadas con ★ son las que con más
frecuencia sobreviven a una prueba feliz.

### 2.1. Casos límite ★

* Contenedores **vacíos**: `Array` sin elementos, `String` vacío, `AudioBuffer`
  con cero canales o cero muestras, `Rectangle` vacío, `ValueTree` inválido,
  `Component` sin padre ni peer, `MidiBuffer` vacío.
* **Ausente** donde se esperaba valor: `std::optional` sin valor, `var` vacío,
  puntero nulo devuelto por `getParentComponent`, `getPeer` o
  `getActiveEditor`, `File` que no existe, `AudioFormatReader` nulo.
* Valores frontera: `0`, negativos, el primer y el último canal, el último
  bloque de un archivo de audio más corto que el tamaño de bloque, el máximo
  permitido (profundidades de anidamiento de `ValueTree`, `var` y XML, tamaños
  máximos de imagen), `int` que desborda con muestras de 64 bits.
* Texto: UTF-8 con caracteres multibyte y emoji (`String` es UTF-8 interno con
  `CharPointer_UTF8`), tildes y eñes en nombres de archivo y en rutas, `String`
  con saltos de línea.
* Off-by-one en rangos de muestras, en índices de canales, en bucles de
  `getNumChildren` y en recortes de `Rectangle`.

### 2.2. Ruta de error ★

* ¿Qué pasa cuando la operación **falla**? (archivo inexistente o sin permiso,
  dispositivo de audio que desaparece, plug-in que no carga, formato de imagen
  o audio no soportado, red caída). Cada operación que puede fallar devuelve
  `Result`, un puntero nulo documentado o lanza el callback de error; nunca
  termina el programa ni deja un objeto a medias.
* Los `jassert` son para invariantes del programador, no para datos del
  usuario: una entrada mal formada produce un fallo controlado, no una
  aserción.

### 2.3. Hilos y tiempo de vida ★

* ¿Se toca la GUI desde otro hilo sin `MessageManagerLock` o
  `callAsync`? ¿Se comparte estado entre `processBlock` y el hilo de mensajes
  sin `std::atomic`, `AbstractFifo` o una copia?
* ¿Hay reservas de memoria, bloqueos (`CriticalSection`, `ScopedLock`), E/S o
  `MessageManager` dentro del hilo de audio?
* ¿Todo `Listener` añadido se quita en el destructor? ¿Todo `Timer`,
  `AsyncUpdater` o `Thread` se detiene antes de destruir lo que usa? ¿Los
  callbacks que capturan `this` usan `Component::SafePointer` o
  `WeakReference`?
* ¿El orden de declaración de los miembros garantiza que se destruyen después
  de quien los usa?

### 2.4. Contratos y simetría ★

* Si cambió una API pública: ¿la usan **todos** sus clientes dentro del repo
  (otros módulos, `examples/`, `extras/`, plantillas de CMake y de Mythos
  Studio)? Búsqueda, no memoria. ¿Está en `BREAKING_CHANGES.md` si cambia lo
  que un proyecto ajeno podía esperar?
* **Paridad de plataformas:** una función nueva de plataforma tiene su
  implementación (o su valor neutro documentado) en los cinco peers nativos.
  Un `#if HIGGS_WINDOWS` nuevo sin su `#else` pensado es un defecto.
* **Ida y vuelta:** lo que un escritor produce, su lector lo lee de vuelta
  (`ValueTree` ↔ XML, `var` ↔ JSON, `AudioFormatWriter` ↔ `AudioFormatReader`,
  proyecto `.mythos` guardado y recargado).
* **Documentación simétrica:** el comentario Doxygen dice exactamente lo que la
  función hace ahora, incluidos los valores devueltos en el fallo y el hilo
  desde el que puede llamarse.
* En lotes sobre N elementos hermanos: ¿los N quedaron realmente idénticos en lo
  que debía ser idéntico? (regla §7.6 de `Execution.md`).

### 2.5. Estado y cachés

* Imágenes, fuentes, glifos y rutas en caché: ¿el cambio invalida lo que debe
  invalidar? ¿Puede la pantalla mostrar un estado anterior al cambio?
* `repaint` y `resized`: ¿se repinta lo que cambió, y solo eso? ¿Se llama a
  `resized` cuando cambian los límites y no antes de que existan hijos?
* Orden de iteración no garantizado (`std::unordered_map`, `HashMap`) que se
  filtra a una salida que debería ser estable (archivos generados, proyectos
  exportados, pruebas).

### 2.6. Seguridad ante entradas hostiles

* ¿Un archivo (ZIP, XML, JSON, PNG, GIF, JPEG, WAV, AIFF, FLAC, OGG, MIDI, fuente),
  un mensaje (MIDI, OSC, red) o un plug-in malicioso puede agotar la memoria o
  la pila, escribir fuera de su sitio o colgar el programa? (checklist completa
  en [Security.md](Security.md)).

### 2.7. Plataformas y compiladores

* ¿Compila sin avisos en MSVC, GCC y Clang, y con el estándar mínimo (C++17)?
  Lo que MSVC acepta y GCC no (`#include` que falta, conversiones implícitas,
  plantillas dependientes), lo que Clang avisa y MSVC no (`-Wshadow`,
  `-Wconversion`).
* ¿El código de Objective-C++ (`.mm`) respeta ARC y los `@autoreleasepool` como
  sus vecinos? ¿El de Android pasa por JNI con las macros del módulo?
* Rutas: siempre por `File`, nunca concatenando separadores; nombres con tildes
  y espacios en Windows.
* Escala de pantalla, DPI y múltiples monitores en lo que toque posicionar
  ventanas o convertir coordenadas.

### 2.8. Cara al usuario del framework

* Nombres coherentes con las clases vecinas y con la tabla de
  [higgs-mythos.md](../../higgs-mythos.md); ninguna aparición de la marca
  retirada.
* Doxygen en inglés, completo, con `@param`, `@returns` y `@see` donde los
  vecinos los tienen; textos visibles de Mythos Studio y de las demos con la
  ortografía y el tono del resto.
* Un cambio de comportamiento observable sin entrada en `BREAKING_CHANGES.md`
  es un defecto de documentación.

### 2.9. Residuos e intenciones

* Código muerto, instrumentación de depuración (`DBG()` temporal,
  `Logger::writeToLog`), includes sin uso, archivos temporales o directorios de
  compilación dentro del árbol.
* Cabecera de licencia intacta en los archivos existentes y presente en los
  nuevos; comentarios en inglés.
* Ninguna línea de atribución a la IA en el código, los comentarios ni el
  mensaje de commit preparado
  ([git-authorship.md](../../../.claude/rules/git-authorship.md)).
* Cambios que el diff contiene y la orden **no pidió**: cada línea del diff debe
  poder justificarse con la orden.

---

## 3. Profundidad Según Riesgo

La revisión es siempre obligatoria; su profundidad se calibra:

| Tipo de cambio                                              | Profundidad mínima                                              |
|-------------------------------------------------------------|-----------------------------------------------------------------|
| Documentación, Doxygen, textos de Mythos Studio o demos     | §2.8 y §2.9                                                     |
| Demos y ejemplos (`examples/`)                              | §2.1, §2.2, §2.3, §2.8, §2.9                                    |
| GUI y gráficos (`higgs_gui_*`, `higgs_graphics`, `higgs_opengl`) | Catálogo completo, con énfasis en §2.3, §2.5 y §2.7        |
| Audio y DSP (`higgs_audio_*`, `higgs_dsp`)                  | Catálogo completo, con énfasis en §2.1 y §2.3 (hilo de audio)   |
| Peers nativos (`native/`)                                   | Catálogo completo, con énfasis en §2.4 (paridad) y §2.7         |
| API pública de cualquier módulo, macros `HIGGS_*`, CMake    | Catálogo completo, con énfasis en §2.4 (clientes y `BREAKING_CHANGES.md`) |
| Lectores de formatos y protocolos (`higgs_audio_formats`, `higgs_graphics` imágenes, XML/JSON/ZIP de `higgs_core`, `higgs_osc`, MIDI) | Catálogo completo, más §2.6 con doble pasada |

---

## 4. Anti-patrones Prohibidos

| Anti-patrón                                                   | Conducta correcta                                                |
|----------------------------------------------------------------|-------------------------------------------------------------------|
| Concluir "se ve bien" tras una relectura pasiva               | Búsqueda dirigida por clases de defecto (§1.2)                    |
| Revisar solo el último archivo tocado                          | Diff completo, archivo por archivo (§1.3)                         |
| Saltarse la revisión "porque el cambio es pequeño"            | Calibrar profundidad (§3), nunca omitir                           |
| Probar solo la entrada feliz                                   | Casos límite y ruta de error primero (§2.1–§2.2)                  |
| Revisar la lógica e ignorar el hilo desde el que se llama      | Hilos y tiempo de vida en cada fragmento (§2.3)                   |
| Cambiar el peer de una plataforma sin mirar los otros cuatro   | Paridad de plataformas (§2.4)                                     |
| Silenciar una duda no resuelta                                 | Declararla como riesgo en el reporte (§1.4)                       |
| Corregir el defecto hallado sin volver a compilar y probar     | Toda corrección vuelve a pasar por verificación (§1.4)            |

---

## 5. Relación con Otros Protocolos

* La verificación funcional que precede a esta revisión:
  [Execution.md](Execution.md) (fase 5).
* La checklist específica de seguridad: [Security.md](Security.md).
* El método a seguir cuando la revisión encuentra un defecto cuya causa no es
  obvia: [Debugging.md](Debugging.md).
* La compatibilidad que §2.4 protege: [Decisions.md](Decisions.md) §2.2.

*Este documento es la referencia de la última línea de defensa antes de entregar;
la regla que lo resume es: asume que hay un defecto y encuéntralo tú, antes de que
lo encuentre el usuario.*
