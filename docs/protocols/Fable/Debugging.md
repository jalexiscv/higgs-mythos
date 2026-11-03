# Protocolo de Diagnóstico y Depuración

Este documento define el método obligatorio para diagnosticar y corregir errores
en **Higgs Mythos**: los módulos del framework (núcleo, eventos, gráficos, GUI,
audio, DSP, formatos, plug-ins), sus peers nativos por plataforma, las
herramientas de `extras/` y las demos de `examples/`. Su objetivo es que toda
corrección ataque la **causa raíz** con evidencia, en lugar de parchar síntomas: un
parche de síntoma hace desaparecer el error de la vista mientras el defecto sigue
vivo y reaparece en otra forma (en otro host, en otra plataforma, con otro
dispositivo de audio).

---

## 1. Principio Rector

**No se corrige lo que no se entiende.** Antes de aplicar cualquier arreglo, debe
poderse formular en una frase la cadena causal completa: *"el síntoma X ocurre
porque el componente Y hace Z cuando se cumple la condición W"*. Por ejemplo: *"la
ventana aparece en el monitor equivocado porque el peer de Windows convierte la
posición lógica a física con la escala del monitor principal cuando el monitor
destino tiene otra escala"*. Si esa frase no puede escribirse con evidencia que la
respalde, el diagnóstico no ha terminado y está prohibido "arreglar".

---

## 2. Ciclo de Diagnóstico

Las seis etapas se recorren en orden. Saltarse una etapa es la causa habitual de
los arreglos que no arreglan.

### Etapa 1 — Caracterizar el síntoma

1. Registra el síntoma **exacto**: mensaje de error **textual** (nunca
   parafraseado), `jassert` disparado con su archivo y línea, salida de
   `UnitTestRunner` con la prueba y la expectativa fallida, entrada que lo
   provoca (archivo, dispositivo, plug-in, host), condiciones (sistema operativo
   y versión, compilador, configuración Debug o Release, escala de pantalla,
   backend gráfico, dispositivo y tasa de muestreo).
2. **Reproduce el error** antes de intentar corregirlo, de la forma más barata
   posible: una prueba `UnitTest` del módulo, la demo correspondiente de
   `DemoRunner`, `AudioPluginHost` con el plug-in implicado, un PIP mínimo de
   `examples/`. Si no es reproducible, documenta con precisión bajo qué
   condiciones se observó y trátalo como intermitente (§4.4). Un error que no se
   puede reproducir tampoco se puede declarar corregido.
3. Establece desde cuándo ocurre: ¿funcionaba antes? ¿Qué cambió entre el estado
   que funcionaba y el actual? (`git log`, changelog, `BREAKING_CHANGES.md`). Un
   error que apareció con un cambio casi siempre vive en ese cambio.
4. **Fija la referencia correcta** antes de hablar de "error": el comportamiento
   esperado es el que documenta el comentario Doxygen de la API y, para los
   formatos y protocolos, su especificación pública (VST3 SDK, AU, LV2, MIDI 2.0
   y MIDI-CI, OSC, los formatos de archivo de audio e imagen). Lo que hace otra
   plataforma del mismo framework es la segunda referencia: un peer que se
   desvía de los demás sin razón documentada es sospechoso.

### Etapa 2 — Leer la evidencia completa

1. **Lee el mensaje de error entero**: del compilador, el **primer** error de la
   cascada (los siguientes suelen ser consecuencia); de `UnitTestRunner`, la
   prueba, la subprueba (`beginTest`) y el mensaje de la expectativa fallida; de
   un `jassert`, la pila de llamadas completa.
2. **Mira los datos reales, no los supuestos.** Para un archivo que no carga,
   inspecciona sus bytes o su XML en el scratchpad; para un problema de dibujo,
   captura la pantalla o guarda la `Image` a PNG; para un problema de audio,
   registra el buffer o el número de muestras por bloque; para un plug-in,
   mira lo que el host realmente envía (`AudioPluginHost` lo muestra).
3. Separa **compilación de lógica**: un error de compilación o de enlace se
   resuelve antes de sospechar de la lógica; un cambio que "no hace nada" puede ser
   un ejecutable viejo de `cmake-build` que no se volvió a compilar, o un módulo
   que se compila con otras macros `HIGGS_*` que las que crees.

### Etapa 3 — Formular hipótesis explícitas

1. Escribe la **lista de hipótesis** plausibles, ordenadas de más a menos
   probable, considerando siempre las cinco familias:
   * **Código**: lógica incorrecta, firma equivocada, contrato roto (un
     `Listener` que no se quita en el destructor, un `Component` que dibuja
     fuera de `paint`, un `ValueTree` modificado sin `UndoManager`).
   * **Hilos y tiempo de vida**: acceso a la GUI fuera del hilo de mensajes sin
     `MessageManagerLock`, estado compartido entre el hilo de audio y el de
     mensajes sin sincronización, objeto destruido con un `Timer` o un
     `AsyncUpdater` vivo, `Component::SafePointer` que no se usó.
   * **Plataforma**: el peer nativo de una plataforma se comporta distinto
     (escala de pantalla, orden de eventos, API del sistema obsoleta o con
     permisos), diferencias entre backends gráficos (Direct2D, CoreGraphics,
     OpenGL, software) o entre dispositivos de audio (WASAPI, ASIO, CoreAudio,
     ALSA, Oboe).
   * **Datos de entrada**: el archivo, el mensaje MIDI/OSC o el plug-in usan una
     construcción que el lector no contempla o está mal formada
     ([Security.md](Security.md)).
   * **Entorno/estado**: ejecutable viejo, macros de configuración distintas,
     caché de fuentes o de imágenes, configuración Debug frente a Release, el
     `jassert` que solo existe en Debug.
2. Para cada hipótesis define **la comprobación más barata que la discrimina**
   (una lectura de código, un grep en los peers de las otras plataformas, una
   prueba de una línea, un `DBG` temporal). El orden de comprobación es por
   costo y probabilidad, no por intuición del momento.

### Etapa 4 — Aislar por bisección

1. Reduce el espacio del problema **partiéndolo por la mitad** siguiendo el flujo
   del framework: ¿el evento llega al peer nativo? → el problema está después;
   ¿el `Component` recibe `mouseDown`? → el problema está en el componente;
   ¿`processBlock` recibe las muestras correctas? → el problema está en el DSP;
   ¿el archivo se decodifica bien en el `AudioFormatReader`? → el problema está
   en quien lo consume. Cada comprobación debe descartar la mitad del territorio
   restante.
2. **Un solo cambio por experimento.** Si modificas dos cosas y el síntoma
   desaparece, no sabes cuál actuó y no has aprendido nada. Revierte la que no
   fue.
3. Construye la **reproducción mínima** cuando el caso sea enredado: el PIP más
   pequeño en `examples/`, el `UnitTest` más corto que dispara el error, el
   archivo de entrada reducido al fragmento que importa.
4. Si el error apareció con el tiempo, bisecta el historial (`git bisect` o
   `git log` sobre los archivos implicados; changelog) para hallar el cambio que
   lo introdujo.

### Etapa 5 — Confirmar la causa

1. La causa candidata debe **explicar todos los síntomas observados**, no solo
   algunos. Un síntoma sin explicar significa causa incompleta u otra causa
   adicional.
2. Verifica la causa **provocándola y desprovocándola**: si al restaurar la
   condición W el síntoma vuelve y al retirarla desaparece, la cadena causal está
   demostrada.

### Etapa 6 — Corregir y cerrar

1. **Corrige donde se rompe el invariante, no donde explota.** Si un `Component`
   recibe un rectángulo vacío, la corrección va en quien calculó mal el
   `Rectangle`; comprobar el vacío en `paint` es defensa válida, pero como
   complemento, no como única medida.
2. Tras corregir, verifica que: (a) el síntoma original desapareció con la misma
   reproducción de la etapa 1; (b) no se introdujo una regresión (batería
   completa de `UnitTestRunner`); (c) los criterios de la fase 5 de
   [Execution.md](Execution.md) se cumplen.
3. **Toda corrección lleva su prueba de regresión** que falla sin el arreglo y
   pasa con él, en una clase `UnitTest` del módulo. Lo que no puede probarse sin
   hardware o sin host (un dispositivo de audio, una ventana real) se verifica a
   mano con la demo o el host y se declara así en el changelog.
4. **Busca el mismo defecto en el resto del código.** Si el patrón defectuoso se
   repite (el mismo cálculo en los peers de Windows y de Linux, el mismo bucle
   en el lector WAV y en el AIFF, la misma conversión en dos backends gráficos),
   un grep del patrón encuentra a los demás afectados; corregir uno de dos es
   dejar un bug conocido sin tocar.
5. Registra el cambio según [Changelogs.md](../Changelogs.md), incluyendo la causa
   raíz en la descripción (no solo "se corrigió X"), y en `BREAKING_CHANGES.md`
   si la corrección cambia un comportamiento del que alguien podía depender.

---

## 3. Distinguir Bug Nuevo de Bug Preexistente

Antes de atribuir un fallo al cambio en curso, compara contra el estado previo:
ejecuta la misma prueba o la misma demo sobre el código sin el cambio (por
ejemplo con `git stash`, o con un ejecutable compilado antes). Si el fallo ya
existía, se reporta como hallazgo separado y **no** se mezcla su corrección con la
orden en curso sin decirlo (regla de alcance de [Execution.md](Execution.md) §6.4);
si no existía, es una regresión del cambio y bloquea la entrega.

---

## 4. Reglas Transversales

1. **Prohibido el arreglo por lotería:** cambiar cosas al azar hasta que el
   síntoma desaparezca no es depurar. Un síntoma que desaparece sin explicación
   volverá.
2. **La instrumentación temporal se retira:** todo `DBG()`, `Logger::writeToLog`
   o salida a consola que se agregue para diagnosticar se elimina antes de
   entregar (o, si aporta a la prueba, se deja con intención como `expect` con
   mensaje). Los volcados de archivos van al scratchpad, nunca al árbol del
   proyecto.
3. **Registra lo descartado.** Las hipótesis comprobadas y descartadas se anotan
   (y se incluyen en el reporte si el diagnóstico fue largo): evita repetir
   comprobaciones y le sirve al siguiente que depure la zona.
4. **Errores intermitentes o que dependen de la máquina:** sospecha en este orden
   de: carrera entre hilos (audio frente a mensajes, `Timer` frente a
   destructor), memoria sin inicializar o comportamiento indefinido (un valor
   que cambia entre Debug y Release, entre MSVC y Clang), orden de iteración no
   garantizado (`std::unordered_map`, `HashMap`), diferencias de plataforma
   (escala de pantalla, fuentes instaladas, tamaño de bloque del dispositivo de
   audio), y ejecutable o módulo viejos.
5. **Dos fallos consecutivos del mismo enfoque obligan a cambiar de enfoque**, no
   a insistir con más fuerza (regla §6.6 de `Execution.md`).
6. **Las suposiciones de la propia prueba también se verifican.** Una prueba que
   falla puede estar mal planteada (por ejemplo, suponer una tasa de muestreo o
   un tamaño de bloque que el `AudioBuffer` de la prueba no tiene); se confirma
   leyendo la prueba antes de cambiar el código de producción.
7. **Lo que no se puede reproducir aquí se declara.** Un fallo reportado en
   macOS, iOS, Android o Linux que no se puede ejecutar en esta máquina se
   diagnostica por lectura del peer y comparación con los demás, y la
   corrección se entrega como **no verificada en esa plataforma**, nunca como
   corregida.

---

## 5. Anti-patrones Prohibidos

| Anti-patrón                                                      | Conducta correcta                                                     |
|------------------------------------------------------------------|------------------------------------------------------------------------|
| Aplicar un arreglo sin poder enunciar la causa en una frase      | Completar el ciclo de diagnóstico antes de tocar código (§1)           |
| Parafrasear el error en lugar de citarlo                         | Mensaje textual del compilador, del `jassert` o de `UnitTestRunner` (Etapa 1) |
| Leer solo el último error de la cascada del compilador           | Buscar el primer error de la cadena (Etapa 2.1)                        |
| Teorizar sobre un archivo o un mensaje sin mirar sus bytes       | Inspeccionar los datos reales (Etapa 2.2)                              |
| Cambiar varias cosas a la vez "a ver si funciona"                | Un cambio por experimento (Etapa 4.2)                                  |
| Blindar el punto de la explosión y dar por cerrado               | Corregir donde se rompe el invariante (Etapa 6.1)                      |
| Declarar corregido sin re-ejecutar la reproducción original      | Verificar contra la misma reproducción de la Etapa 1 (Etapa 6.2)       |
| Corregir sin prueba de regresión                                 | `UnitTest` que falla sin el arreglo y pasa con él (Etapa 6.3)          |
| Corregir el peer de Windows e ignorar el de Linux con el mismo defecto | Grep del patrón defectuoso en todos los peers (Etapa 6.4)        |
| Declarar corregido un fallo de una plataforma que no se ejecutó  | Entregarlo como no verificado en esa plataforma (§4.7)                 |
| Dejar `DBG()` o volcados de depuración entregados                | Retirar toda instrumentación temporal (§4.2)                           |

---

## 6. Relación con Otros Protocolos

* El ciclo general de trabajo y su verificación: [Execution.md](Execution.md).
* El registro del cambio con su causa raíz: [Changelogs.md](../Changelogs.md).
* La estructura de los módulos y sus peers nativos:
  [docs/Higgs Mythos Module Format.md](../../Higgs%20Mythos%20Module%20Format.md).
* Las entradas hostiles que suelen estar detrás de un fallo: [Security.md](Security.md).
* La autorrevisión que detecta defectos antes de que sean bugs:
  [Review.md](Review.md).

*Este documento es la referencia sobre cómo diagnosticar y corregir errores; la
regla que lo resume es: sin cadena causal demostrada no hay corrección.*
