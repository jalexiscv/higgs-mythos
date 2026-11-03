# Protocolo de Ejecución para Agentes de IA

Este documento define la **disciplina de trabajo** que debe seguir cualquier agente
de IA (independientemente del modelo que lo impulse: Fable, Opus, Sonnet u otro) al
ejecutar órdenes sobre **Higgs Mythos**, el framework de aplicaciones
multiplataforma en C++17 para escritorio, móvil y plug-ins de audio. Su objetivo es
que la calidad del resultado sea uniforme y máxima sin importar qué modelo ejecute:
lo que un modelo más capaz hace de forma espontánea, aquí queda escrito como
obligación explícita para todos.

Los demás documentos definen **qué** reglas cumplir:

* la identidad, los nombres y la compilación: [docs/higgs-mythos.md](../../higgs-mythos.md);
* el formato de los módulos y sus dependencias: [docs/Higgs Mythos Module Format.md](../../Higgs%20Mythos%20Module%20Format.md);
* la API de CMake y sus opciones: [docs/CMake API.md](../../CMake%20API.md);
* los commits, el idioma y la regla de origen: [.github/contributing.md](../../../.github/contributing.md);
* la autoría en `git`: [.claude/rules/git-authorship.md](../../../.claude/rules/git-authorship.md);
* el historial y la documentación: [Changelogs.md](../Changelogs.md) y [Documentations.md](../Documentations.md).

Este protocolo define **cómo trabajar** para cumplirlas: el ciclo de fases, la
verificación, la honestidad del reporte y la lista de comprobación final.

---

## 1. Principio Rector

**Una orden no está terminada hasta que está cumplida por completo, verificada con
evidencia y documentada.** El resultado se juzga por el desenlace, no por el
esfuerzo: un cambio a medias, sin compilar, sin pruebas o sin registrar en el
changelog es una orden incumplida, aunque el código escrito sea correcto.

De este principio se derivan tres reglas de oro:

1. **Nunca afirmes lo que no has verificado.** "Debería funcionar" está prohibido;
   la forma correcta es "funciona: lo verifiqué con X y el resultado fue Y".
2. **Nunca edites lo que no has leído.** Todo archivo se lee antes de modificarse;
   toda función, clase o campo se localiza en su definición real antes de usarse.
3. **Nunca dejes trabajo a medias.** No se entregan `TODO` al usuario, no se
   procesan 5 de 16 elementos, no se promete "luego haré…": se hace ahora.

---

## 2. Ciclo de Trabajo Obligatorio

Toda orden de cambio se ejecuta en **siete fases secuenciales**. Ninguna fase se
omite; cada una tiene un criterio de salida que debe cumplirse antes de pasar a la
siguiente.

| # | Fase        | Pregunta que responde                  | Criterio de salida                                        |
|---|-------------|----------------------------------------|-----------------------------------------------------------|
| 1 | Entender    | ¿Qué se pidió exactamente?             | Lista numerada de requisitos explícitos e implícitos      |
| 2 | Explorar    | ¿Cómo es el terreno?                   | Archivos afectados leídos; patrones de referencia hallados|
| 3 | Planificar  | ¿En qué orden y sobre qué archivos?    | Plan con lista completa de elementos a tocar              |
| 4 | Ejecutar    | ¿El cambio está hecho?                 | Todos los elementos del plan procesados                   |
| 5 | Verificar   | ¿Funciona y no rompí nada?             | Compilación, pruebas y evidencia por cada requisito       |
| 6 | Documentar  | ¿Quedó registrado?                     | Changelog + índice + documentación + registros públicos   |
| 7 | Reportar    | ¿El usuario entiende qué pasó?         | Reporte que inicia con el resultado y es honesto          |

**Calibración de esfuerzo:** el ciclo completo aplica a órdenes de cambio. Una
pregunta simple se responde de forma directa, en prosa, sin ceremonia ni encabezados;
un diagnóstico se investiga con las fases 1–2 y se reporta sin aplicar cambios.
Sobre-formatear una respuesta trivial es tan incorrecto como sub-verificar un cambio
grande.

---

## 3. Fase 1 — Entender la Orden

1. **Relee la orden completa** antes de tocar nada y extrae una **lista numerada de
   requisitos explícitos**. Esta lista es el contrato: al final, cada punto debe
   tener evidencia de cumplimiento (ver §7).
2. **Deriva los requisitos implícitos.** Si la orden dice "todos los módulos de
   audio", el primer paso es listarlos y **contarlos**; el número obtenido es
   parte del contrato. Si la orden dice "en todas las plataformas", hay que
   enumerar los peers nativos implicados (`native/higgs_<Nombre>_windows.cpp`,
   `_mac.mm`, `_linux.cpp`, `_ios.mm`, `_android.cpp`) antes de empezar.
3. **Distingue el tipo de orden:**
   * *Pregunta o diagnóstico* ("¿por qué falla…?", "revisa si…"): el entregable es
     el análisis. Se investiga y se reporta; **no se aplica ningún arreglo** hasta
     que el usuario lo pida.
   * *Orden de cambio* ("agrega…", "corrige…", "implementa…"): el entregable es el
     cambio completo, compilado, probado y documentado.
4. **Ante ambigüedad**, elige la interpretación más coherente con el código
   existente, la documentación Doxygen de la API y las decisiones previas del
   usuario, ejecútala y **declara la suposición en el reporte final**. Solo se
   detiene el trabajo para preguntar cuando la duda implica una acción
   destructiva, un cambio de alcance real o un dato que solo el usuario posee
   (criterio de producto, credenciales, versión a publicar).
5. **No re-litigues decisiones ya tomadas.** Si el usuario ya eligió un enfoque en
   esta conversación o en órdenes anteriores, se respeta; no se vuelve a proponer
   la alternativa descartada.

---

## 4. Fase 2 — Explorar Antes de Tocar

1. **Lee todo archivo antes de editarlo** — el archivo completo si es razonable, o
   al menos la sección afectada **más su contexto circundante** (includes, clase,
   funciones vecinas). Editar a ciegas está prohibido.
2. **Busca el ejemplar de referencia.** Casi todo cambio tiene un precedente en el
   framework: otra clase del mismo módulo, otro peer nativo que implementa la
   misma función para otra plataforma, otra prueba `UnitTest` al final de un
   `.cpp` del mismo módulo, otra demo en `examples/`. Encuéntralo e **imítalo**; no
   inventes una estructura nueva cuando existe una establecida.
3. **Nunca inventes una API.** Antes de invocar una función, método o campo,
   localiza su **definición real** en la cabecera del módulo (`modules/higgs_<área>/…/higgs_<Nombre>.h`)
   y confirma su firma: parámetros, tipos (`String`, `var`, `ValueTree`,
   `Result`, `std::optional`), si es `noexcept`, si transfiere propiedad
   (`std::unique_ptr`, `ReferenceCountedObjectPtr`). Los errores más costosos
   nacen de asumir una firma de memoria.
4. **Antes de cambiar una firma pública, una macro `HIGGS_*` o una función de
   CMake, busca a todos sus usuarios:** los demás módulos, `examples/`, `extras/`
   (Mythos Studio, AudioPluginHost, UnitTestRunner), las plantillas de
   `extras/Build/CMake` y la documentación. Un cambio de contrato sin actualizar a
   sus usuarios es un cambio incompleto, y si rompe a los proyectos que usan el
   framework es un cambio rompiente ([Decisions.md](Decisions.md) §4.1).
5. **Respeta las dependencias de los módulos**
   ([Higgs Mythos Module Format.md](../../Higgs%20Mythos%20Module%20Format.md)):
   un módulo solo incluye cabeceras de los módulos que declara en `dependencies`
   de su cabecera principal. `higgs_core` no depende de nadie; nada de GUI entra
   en `higgs_core`, `higgs_events` ni `higgs_audio_basics`.
6. **Respeta el muro de la plataforma:** el código específico de un sistema vive
   en `native/` del módulo, detrás de `HIGGS_WINDOWS`, `HIGGS_MAC`, `HIGGS_LINUX`,
   `HIGGS_IOS`, `HIGGS_ANDROID`; la cabecera pública no filtra tipos del sistema.
7. **Explora en paralelo y con las herramientas adecuadas:** búsquedas
   independientes se lanzan a la vez; para archivos se usa búsqueda por patrón
   (glob), para contenido búsqueda de texto (grep), no comandos de shell
   improvisados. Las preguntas amplias ("¿dónde se maneja X en todo el framework?")
   se delegan a un agente de exploración si el entorno lo permite.

---

## 5. Fase 3 — Planificar

1. **Enumera los archivos afectados antes de editar el primero.** El plan dice
   qué archivos se tocan, en qué orden y por qué.
2. **En tareas de varios pasos, lleva una lista de tareas visible** (la herramienta
   de tareas del entorno o una lista explícita en el plan) y márcala a medida que
   avanzas. En sesiones largas esta lista es lo que impide perder pasos cuando el
   contexto se resume ([Continuity.md](Continuity.md)).
3. **Para operaciones por lotes** (aplicar algo a N módulos, N peers nativos,
   N demos, N pruebas): primero se construye la **lista completa** de elementos,
   se registra el conteo, se procesan **todos**, y al final se verifica que el
   número de elementos procesados coincide con el conteo inicial. Prohibido
   procesar una muestra y extrapolar.
4. **Prefiere reutilizar sobre reescribir.** Si existe una clase de `higgs_core`,
   un helper del módulo o un patrón que resuelve el problema, úsalo. El mejor
   diff es el **más pequeño que resuelve el problema por completo** — pequeño no
   por incompleto, sino por no incluir nada ajeno a la orden.
5. **Detecta el efecto dominó desde el plan.** Una función nueva en una clase
   pública afecta a su documentación Doxygen y quizá a `CHANGE_LIST.md`; un
   cambio de comportamiento afecta a `BREAKING_CHANGES.md`; una clase nueva de
   GUI afecta a la accesibilidad (`AccessibilityHandler`) y al `LookAndFeel`; una
   función nueva de plataforma afecta a **todos** los peers nativos, no solo al
   de la máquina en que se trabaja; una opción nueva de CMake afecta a
   `CMake API.md` y a las plantillas de Mythos Studio. Esas ramificaciones entran
   al plan desde el inicio, no como sorpresa al final.

---

## 6. Fase 4 — Ejecutar

1. **Ataca la causa raíz, no el síntoma.** Antes de aplicar un arreglo, formula en
   una frase por qué ocurre el problema; si no puedes formularla, aún no entiendes
   el bug y no debes "arreglarlo" ([Debugging.md](Debugging.md)).
2. **El código nuevo debe leerse como el código que lo rodea:** misma densidad de
   comentarios, mismos nombres, mismos modismos. El estilo es el del framework:
   `camelCase` para funciones y variables, `PascalCase` para tipos, espacio
   antes del paréntesis de llamada (`setSize (800, 600)`), llaves de Allman,
   `//==============================================================================`
   como separador de secciones, `HIGGS_DECLARE_NON_COPYABLE` y
   `HIGGS_LEAK_DETECTOR` donde la clase vecina los usa, `jassert` para
   invariantes. Comentarios y Doxygen en inglés y solo para lo que el código no
   expresa (restricciones, invariantes, porqués); nunca comentarios que narren
   el cambio ("se agregó esta línea para…").
3. **La cabecera de licencia se conserva.** Todo archivo nuevo de `modules/`
   empieza con el bloque de licencia AGPL del archivo vecino; el aviso de
   copyright de los archivos existentes **no se edita ni se elimina**
   ([higgs-mythos.md](../../higgs-mythos.md) §"Licencia y atribución").
4. **Sin desviaciones de alcance.** No se refactoriza lo que no se pidió, no se
   "aprovecha" para reordenar includes ni renombrar variables ajenas a la orden.
   La única excepción es el formato de las líneas que el cambio ya toca.
5. **Completa cada elemento del plan.** Nada de "los demás peers siguen el mismo
   patrón" dejando el patrón sin aplicar: si el plan dice 5 plataformas, se
   editan 5 plataformas; si no se puede compilar una, se dice (§7.8).
6. **Ante un error, diagnostica antes de reintentar.** Repetir el mismo comando o
   la misma edición esperando otro resultado está prohibido; primero se identifica
   la causa (leer el mensaje del compilador completo, verificar rutas, firmas,
   estado), luego se corrige y se reintenta. Si un enfoque falla dos veces por la
   misma causa, se cambia de enfoque.
7. **No te detengas a mitad de la tarea para pedir permiso de continuar.** Las
   acciones reversibles que se derivan directamente de la orden se ejecutan sin
   preguntar "¿quieres que…?". Solo se interrumpe el trabajo por las causas del
   §3.4 (destructivo, alcance, dato faltante).
8. **No dejes residuos:** ni código muerto, ni `DBG()`/`std::cout`/`printf` de
   depuración, ni archivos temporales dentro del árbol del proyecto (los
   temporales van al scratchpad del entorno; las pruebas usan
   `File::getSpecialLocation (File::tempDirectory)` o
   `TemporaryFile`). Los directorios de compilación (`cmake-build*`, `build*`)
   nunca se confirman.
9. **Regla de origen ([contributing.md](../../../.github/contributing.md)).**
   Todo el código se escribe desde cero o se toma de fuentes con licencia
   compatible con la AGPL-3.0-or-later. **Está prohibido** escribir código,
   constantes o estructuras copiados o traducidos de binarios propietarios
   (hosts de audio, SDKs cerrados, otros frameworks sin licencia compatible).
   Los SDKs de plug-ins (VST3, AAX, ARA, LV2) se usan solo bajo sus propias
   licencias y acuerdos, nunca se copian al repositorio sin que la licencia lo
   permita.
10. **Nombres del proyecto.** Todo identificador nuevo sigue la tabla de
    [higgs-mythos.md](../../higgs-mythos.md): namespace `higgs`, macros
    `HIGGS_`, módulos `higgs_<área>`, funciones de CMake `higgs_`. La marca
    retirada de la base original no vuelve a aparecer en código, rutas, recursos
    ni documentación.

---

## 7. Fase 5 — Verificar

Esta fase es la que separa un resultado excelente de uno mediocre. **Nada se
reporta como hecho sin evidencia objetiva.**

1. **Compilación sin avisos.** Se configura una vez
   (`cmake . -B cmake-build -DHIGGS_BUILD_EXAMPLES=ON -DHIGGS_BUILD_EXTRAS=ON`)
   y se compilan los objetivos afectados
   (`cmake --build cmake-build --target <objetivo>`), como mínimo los que
   incluyen el módulo tocado: `UnitTestRunner` siempre, `DemoRunner` para GUI,
   gráficos y audio, `AudioPluginHost` para hosting de plug-ins, `MythosStudio`
   para cambios de CMake, plantillas o `higgsaide`. En Windows la compilación
   necesita el entorno de `vcvars64.bat`. Un aviso nuevo es un error: no se
   entrega código que añade avisos. Sin excepciones, aunque el cambio sea "de una
   línea".
2. **Pruebas.** Las pruebas del framework son clases `UnitTest` compiladas con
   `HIGGS_UNIT_TESTS=1` y ejecutadas por `extras/UnitTestRunner`. Se ejecuta la
   categoría del módulo tocado (`UnitTestRunner --category=<categoría>` o
   `--name=<prueba>`) y, antes de entregar, la batería completa
   (`UnitTestRunner` sin argumentos). Se reporta la línea de resumen tal cual.
   Toda corrección de un defecto lleva su prueba de regresión en una clase
   `UnitTest` del módulo, con `beginTest` y `expect*`, siguiendo la prueba vecina.
3. **Verificación funcional.** Si el cambio tiene superficie observable, se
   ejercita con el framework real: la demo correspondiente de `DemoRunner`
   (o un PIP de `examples/`), `AudioPluginHost` para procesadores y plug-ins,
   Mythos Studio para proyectos y exportadores. Un cambio que nunca se ejecutó
   no está verificado, está redactado.
4. **Contraste contra el contrato:** se recorre la lista numerada de requisitos de
   la fase 1 y, punto por punto, se anota la evidencia que demuestra su
   cumplimiento. Un requisito sin evidencia es un requisito pendiente.
5. **Relectura del diff completo** (`git diff`) antes de dar por cerrada la fase,
   buscando: cambios no intencionales, residuos de depuración, referencias rotas,
   archivos tocados por accidente, cabeceras de licencia alteradas, conteo de
   archivos coherente con el plan.
6. **Verificación de lotes:** en operaciones sobre N elementos, comprobar con una
   búsqueda (no de memoria) que los N elementos quedaron en el estado esperado.
7. **Si la verificación falla, se vuelve a la fase 4.** Está prohibido reportar
   "terminado, aunque las pruebas fallan por algo no relacionado" sin haber
   demostrado que efectivamente no está relacionado (ejecutando esas pruebas sobre
   el estado previo al cambio, por ejemplo con `git stash`).
8. **Lo que no se pudo verificar se declara:** en esta máquina se compila con
   MSVC en Windows; el código de macOS, iOS, Linux y Android se escribe con el
   mismo cuidado, se revisa contra el peer vecino y se declara como **no
   compilado aquí**. Nunca se afirma que compila en una plataforma donde no se
   compiló.

---

## 8. Fase 6 — Documentar

1. **Todo cambio se registra en la misma sesión** según
   [Changelogs.md](../Changelogs.md): archivo detallado
   `changelogs/YYYY-MM-DD_descripcion.md` **y** su fila en `changelogs/CHANGELOG.md`.
   "Lo documento después" no existe.
2. **La versión no se toca.** El changelog lleva la versión que declara
   `higgs_StandardHeader.h`; subirla es decisión del usuario
   ([Decisions.md](Decisions.md) §4).
3. Si el cambio altera la API pública, su comentario Doxygen se actualiza en el
   mismo cambio. Si la rompe o cambia su comportamiento observable, entra en
   `BREAKING_CHANGES.md` con la plantilla del archivo; si es una funcionalidad
   notable, su titular entra en `CHANGE_LIST.md`
   ([Documentations.md](../Documentations.md) §2).
4. Si el cambio altera cómo *es* el framework (módulos, formato de módulo,
   funciones de CMake, requisitos de sistema, nombres), se actualiza `docs/` y el
   índice `docs/README.md` si se crea un documento nuevo (sin huérfanos), y el
   `README.md` raíz si cambia la visión general.
5. La documentación describe el estado final; el changelog describe la
   transición. No mezclar ambos.

---

## 9. Fase 7 — Reportar

El reporte final es lo único que el usuario lee con seguridad; debe bastarse a sí
mismo.

1. **La primera frase es el resultado**, no el proceso: qué se hizo o qué se
   encontró. El detalle viene después, para quien quiera leerlo.
2. **Prosa completa y en español.** Prohibidos los fragmentos telegráficos, las
   cadenas de flechas y la jerga inventada durante la sesión. Legible importa
   más que corto: se recorta *seleccionando* qué contar, no comprimiendo la
   redacción.
3. **Contenido mínimo del reporte de un cambio:**
   * Qué cambió y dónde, con referencias `archivo:línea` navegables.
   * Cómo se verificó: compilación, pruebas y flujo ejercitado **con su resultado
     real**, y qué plataformas no se compilaron.
   * Suposiciones tomadas ante ambigüedades (§3.4).
   * Riesgos, límites o pendientes, dichos sin rodeos.
4. **Honestidad absoluta sobre los fallos:** si una prueba falla, se dice cuál y
   se incluye la salida relevante tal cual; si un paso se omitió, se dice que se
   omitió y por qué. Suavizar un fallo es falsear el reporte.
5. **Sin promesas de trabajo futuro dentro de la misma orden.** Si el último
   párrafo del borrador de reporte es un plan, una lista de "próximos pasos" que
   la orden ya cubría, o un "luego haré…", ese trabajo se hace **antes** de
   reportar. Ofrecer mejoras adicionales fuera del alcance sí es válido, como
   ofrecimiento, al final.
6. **Sin halagos de apertura ni relleno:** el reporte no empieza con "¡Excelente
   pregunta!" ni termina con resúmenes que repiten lo ya dicho.

---

## 10. Honestidad y Evidencia (transversal a todas las fases)

1. **Nunca inventes:** ni salidas de comandos, ni contenidos de archivos no
   leídos, ni firmas de funciones, ni resultados de pruebas. Si un dato no se
   verificó, se etiqueta como no verificado.
2. **Cita los errores textualmente.** Un mensaje del compilador, del enlazador o
   de `UnitTestRunner` parafraseado pierde la información que permitía
   diagnosticarlo.
3. **Antes de un comando que cambia estado** (borrar, sobrescribir, `git reset`,
   reescribir historia), confirma que la evidencia respalda *esa* acción
   específica. Antes de borrar o sobrescribir algo que no creaste tú, míralo
   primero; si lo que encuentras contradice la descripción que se te dio,
   repórtalo en lugar de proceder.
4. **Si estás bloqueado, di exactamente qué falta** (el dato, el acceso, la
   decisión) en una frase, en lugar de rendirte en silencio o improvisar.

---

## 11. Anti-patrones Prohibidos

| Anti-patrón                                                    | Conducta correcta                                                        |
|----------------------------------------------------------------|--------------------------------------------------------------------------|
| Declarar "listo" sin haber compilado ni probado                | Compilar, probar y reportar con evidencia (§7)                           |
| Editar un archivo sin haberlo leído                            | Leer archivo y contexto antes de editar (§4.1)                           |
| Invocar funciones asumiendo su firma de memoria                | Localizar la definición real y confirmar la firma (§4.3)                 |
| Cambiar una API pública sin tocar a sus usuarios               | Buscar a todos los usuarios del contrato (§4.4, §5.5)                    |
| Implementar una función de plataforma solo en Windows          | Todos los peers nativos; lo no compilado se declara (§6.5, §7.8)         |
| Procesar 5 de 16 elementos y extrapolar                        | Lista completa + conteo inicial + verificación del conteo final (§5.3)   |
| Preguntar "¿quieres que continúe?" ante acciones reversibles   | Continuar; solo detenerse por lo destructivo o el alcance (§6.7)         |
| Dejar `TODO`, "resto igual" o promesas de trabajo futuro       | Completar ahora, dentro de la misma orden (§9.5)                         |
| "Debería funcionar"                                            | "Funciona: verificado con X, resultado Y" (§7)                           |
| Entregar con avisos nuevos del compilador                      | Cero avisos nuevos, siempre (§7.1)                                       |
| Ocultar o suavizar una prueba fallida                          | Reportarla textualmente (§9.4)                                           |
| Refactorizar o "mejorar" lo que nadie pidió                    | Diff mínimo completo (§6.4)                                              |
| Reintentar el mismo comando fallido sin diagnóstico            | Diagnosticar la causa, corregir, luego reintentar (§6.6)                 |
| Reporte en fragmentos, flechas y jerga                         | Prosa completa que inicia con el resultado (§9.1–9.2)                    |
| Copiar código de un binario o SDK sin licencia compatible      | Solo fuentes compatibles con la AGPL (§6.9)                              |
| Editar o borrar la cabecera de copyright de un archivo         | Conservarla intacta (§6.3)                                               |
| Subir la versión del framework por iniciativa propia           | Decisión del usuario (§8.2)                                              |
| Añadir `Co-Authored-By` o menciones a la IA en commits o código | Ningún rastro de autoría de la IA ([git-authorship.md](../../../.claude/rules/git-authorship.md)) |
| Arreglar sin que lo pidan cuando la orden era diagnosticar     | Entregar el análisis y esperar la decisión (§3.3)                        |
| Volver a proponer la alternativa que el usuario ya descartó    | Respetar las decisiones tomadas (§3.5)                                   |

---

## 12. Lista de Verificación Final (obligatoria)

Antes de escribir el reporte final, responde **cada** punto. Un solo "no" significa
que la orden **no está terminada**: se vuelve a la fase correspondiente.

1. ¿Cada requisito numerado de la fase 1 está cumplido **y tiene evidencia**?
2. ¿Leí cada archivo antes de editarlo?
3. ¿Compilé los objetivos afectados sin añadir avisos?
4. ¿Ejecuté `UnitTestRunner` para las categorías tocadas y la batería completa, y
   reporté su resultado tal cual? ¿Toda corrección tiene su prueba de regresión?
5. ¿Ejercité el cambio con el framework real (`DemoRunner`, `AudioPluginHost`,
   Mythos Studio o un ejemplo) cuando tiene superficie observable?
6. ¿Releí el diff completo y no hay residuos, cabeceras alteradas ni cambios no
   intencionales?
7. ¿En los lotes, el conteo final de elementos coincide con el inicial?
8. ¿Creé el changelog detallado **y** actualicé `changelogs/CHANGELOG.md`, sin
   tocar la versión del framework?
9. ¿Actualicé Doxygen, `docs/` y su índice, `BREAKING_CHANGES.md` y
   `CHANGE_LIST.md` si el cambio lo amerita?
10. ¿Cumplí el estilo del framework (nombres `higgs`/`HIGGS_`, formato del código
    vecino, comentarios en inglés, cabecera de licencia) y las dependencias
    declaradas de los módulos?
11. ¿Cumplí [contributing.md](../../../.github/contributing.md) (`area: resumen`
    en inglés, sin atribución a la IA) si la orden incluía versionar, y la regla
    de origen siempre?
12. ¿Mi reporte inicia con el resultado, está en prosa completa y declara
    suposiciones, fallos, plataformas no compiladas y pendientes con honestidad?
13. ¿Mi último párrafo es un hecho consumado y no un plan, una pregunta evitable
    o una promesa?

---

## 13. Relación con Otros Protocolos

* La identidad, los nombres y la compilación: [docs/higgs-mythos.md](../../higgs-mythos.md).
* El formato de los módulos: [docs/Higgs Mythos Module Format.md](../../Higgs%20Mythos%20Module%20Format.md).
* La API de CMake: [docs/CMake API.md](../../CMake%20API.md).
* El registro de cambios de la fase 6: [Changelogs.md](../Changelogs.md).
* La documentación continua de la fase 6: [Documentations.md](../Documentations.md).
* Los commits y la regla de origen: [.github/contributing.md](../../../.github/contributing.md).
* La autoría en `git`: [.claude/rules/git-authorship.md](../../../.claude/rules/git-authorship.md).
* El método de diagnóstico por causa raíz cuando la orden es corregir un error: [Debugging.md](Debugging.md).
* Cómo decidir entre implementaciones alternativas en la fase 3: [Decisions.md](Decisions.md).
* La autorrevisión adversarial del diff, entre la fase 5 y la fase 6: [Review.md](Review.md).
* La checklist de seguridad que todo cambio debe aprobar: [Security.md](Security.md).
* La continuidad en tareas largas y la reanudación de trabajo previo: [Continuity.md](Continuity.md).

*Este documento es la referencia sobre cómo debe trabajar un agente de IA en Higgs
Mythos; los demás documentos definen las reglas específicas que ese trabajo debe
cumplir.*
