# Protocolo de Continuidad: Sesiones Largas y Reanudación

Este documento define cómo debe trabajar un agente de IA en tareas largas y cómo
debe reanudar trabajo previo en **Higgs Mythos**. Las sesiones de IA tienen memoria
limitada: el contexto se resume, la sesión se corta o el trabajo lo retoma otro
agente. Sin disciplina, eso produce los peores resultados posibles: pasos que se
pierden, trabajo que se rehace, órdenes que se dan por cumplidas a medias. Este
protocolo convierte el **repositorio** —no la memoria conversacional— en la fuente
de verdad del estado del trabajo.

---

## 1. Principio Rector

**La memoria conversacional es un caché; el repositorio es la base de datos.**
Todo lo que importa para continuar el trabajo debe existir fuera de la
conversación: en el árbol de archivos, en `git`, en el changelog
(`changelogs/`), en `BREAKING_CHANGES.md` y `CHANGE_LIST.md`, o en la lista de
tareas. Ante cualquier discrepancia entre lo que el agente "recuerda" y lo que el
repositorio muestra, **gana el repositorio**, y la discrepancia misma es señal de
que hay que re-verificar antes de continuar.

---

## 2. Durante una Tarea Larga

1. **Lista de tareas viva.** Toda tarea de varios pasos mantiene una lista de
   tareas visible (la herramienta de tareas del entorno o la sección de plan del
   changelog en curso), actualizada en el momento en que un paso cambia de
   estado, no al final "de memoria".
2. **Completado significa verificado.** Un paso se marca completado únicamente
   cuando compila sin avisos nuevos y sus pruebas de `UnitTestRunner` pasan
   (fase 5 de [Execution.md](Execution.md)). Marcar por optimismo ("ya casi")
   corrompe la única brújula que sobrevive a un corte de contexto.
3. **El contrato de la orden se conserva por escrito.** La lista numerada de
   requisitos (fase 1 de `Execution.md`) debe ser recuperable: si la tarea es lo
   bastante larga para sufrir un resumen de contexto, el contrato vive en la
   lista de tareas, no solo en la memoria de la conversación.
4. **Puntos de consolidación.** En tareas grandes, al cerrar cada bloque coherente
   (por ejemplo, "la clase nueva de `higgs_core`, su prueba `UnitTest` y su
   Doxygen, compilados y en verde") se consolida: verificación del bloque, commit
   en la rama de la tarea si la orden incluye versionar, changelog parcial si
   corresponde y actualización de la lista. Así, un corte en cualquier punto deja
   como máximo un bloque en el aire, no toda la tarea.
5. **Las series largas se numeran.** Cuando un cambio grande se confirma en varios
   commits (como las series `(n/N)` del historial de `examples`), cada mensaje
   lleva su posición, y el último de la serie es el que cierra el changelog. Una
   serie incompleta se detecta con `git log` sin necesidad de recordar nada.
6. **Los apuntes intermedios van al scratchpad del entorno**, nunca al árbol del
   proyecto: el repositorio solo recibe entregables (código, pruebas,
   documentación, changelogs). Esto incluye los volcados de archivos de audio o
   imagen usados para diagnosticar, las capturas, los scripts auxiliares de
   compilación y los directorios `cmake-build*`.

---

## 3. Al Reanudar (nueva sesión, contexto resumido u otro agente)

Antes de escribir una sola línea, se reconstruye el estado real en este orden:

1. **Releer la orden original** (o su contrato numerado) — no la paráfrasis del
   resumen, que puede haber perdido requisitos.
2. **`git status`, `git diff` y `git log`**: qué está tocado realmente, qué está
   en staging, en qué rama se está y qué se confirmó. Esto es el estado objetivo
   del trabajo.
3. **Changelog** (`changelogs/CHANGELOG.md` y los detalles recientes): qué se dio
   ya por terminado y con qué alcance.
4. **Lista de tareas**: qué pasos figuran completados y cuáles pendientes.
5. **Verificación por muestreo de lo "ya hecho":** antes de construir encima,
   comprobar con evidencia (compilar el objetivo afectado, ejecutar
   `UnitTestRunner --category=<módulo>`, un grep, abrir un archivo clave) que lo
   marcado como completado lo está de verdad. Un directorio `cmake-build` puede
   tener ejecutables viejos: se recompila antes de fiarse de una prueba en verde.
   La confianza ciega en el estado heredado es la forma más rápida de construir
   sobre arena.

Solo entonces se continúa, empezando por el primer paso pendiente — sin rehacer
lo verificado (desperdicio) y sin saltarse la verificación por muestreo (riesgo).

---

## 4. Al Pausar o Quedar Bloqueado

Cuando una tarea no puede terminarse en el turno (bloqueo por dato del usuario,
por acceso o por decisión de alcance):

1. **El árbol queda consistente.** Nada a medio editar que rompa la compilación o
   las pruebas: el bloque en curso se termina y verifica, o se revierte
   limpiamente. Está prohibido dejar el repositorio en un estado en que
   `UnitTestRunner` o `DemoRunner` no compilan y nadie más puede trabajar. Un peer
   nativo a medias para una plataforma que no se puede compilar aquí se declara
   expresamente en la lista de tareas y en el reporte.
2. **El estado queda escrito:** la lista de tareas refleja exactamente qué está
   hecho-y-verificado, qué está a medias (y en qué archivo/paso) y qué falta.
3. **El reporte de pausa dice tres cosas** en prosa clara: qué se completó (con
   evidencia), qué está pendiente y **cuál es el único dato o decisión que se
   necesita para continuar**. Un bloqueo bien reportado se desbloquea con una
   respuesta de una línea del usuario.

---

## 5. Trabajo Nunca Perdido, Trabajo Nunca Duplicado

1. **Antes de rehacer algo "que parece faltar", buscarlo:** puede existir con
   otro nombre, en otro módulo o en otra rama (`git log --all`, grep). Duplicar
   trabajo existente crea la divergencia entre hermanos que
   [Review.md](Review.md) §2.4 persigue.
2. **Antes de descartar cambios locales (`git checkout --`, `git reset`,
   `git stash drop`), inventariarlos:** un `git diff` leído completo. Descartar es
   destructivo; lo que se descarta sin leer no se recupera.
3. **Los experimentos van en ramas o en el scratchpad,** nunca sobre el trabajo
   bueno sin respaldo. Si hay que probar un enfoque incierto que toca muchos
   archivos, el punto de retorno se asegura primero (commit del trabajo bueno en
   la rama de la tarea, con el formato de
   [contributing.md](../../../.github/contributing.md) y sin atribución a la IA).

---

## 6. Anti-patrones Prohibidos

| Anti-patrón                                                        | Conducta correcta                                                |
|---------------------------------------------------------------------|--------------------------------------------------------------------|
| Confiar en la memoria de la conversación sobre el estado del repo  | El repositorio es la fuente de verdad (§1)                         |
| Marcar pasos completados sin compilar ni probar                    | Completado = verificado (§2.2)                                     |
| Reanudar y seguir "donde creo que iba"                             | Reconstruir estado: orden → git → changelog → tareas (§3)          |
| Fiarse de una prueba en verde de un ejecutable sin recompilar      | Recompilar antes de muestrear (§3.5)                               |
| Dejar el proyecto sin compilar al pausar                           | Árbol consistente: terminar el bloque o revertirlo (§4.1)          |
| Bloqueo reportado como "no pude continuar"                         | Hecho + pendiente + el dato exacto que falta (§4.3)                |
| Rehacer trabajo que ya existe con otro nombre                      | Buscar antes de rehacer (§5.1)                                     |
| Descartar cambios locales sin leerlos                              | Inventariar el diff antes de toda operación destructiva (§5.2)     |
| Notas, volcados y directorios de compilación dentro del árbol      | Scratchpad del entorno para todo lo intermedio (§2.6)              |

---

## 7. Relación con Otros Protocolos

* El ciclo de trabajo cuyas fases este protocolo hace sobrevivir a los cortes:
  [Execution.md](Execution.md).
* El registro que sirve de memoria durable de lo terminado:
  [Changelogs.md](../Changelogs.md).
* El formato de los commits que protegen los puntos de retorno:
  [contributing.md](../../../.github/contributing.md) y
  [git-authorship.md](../../../.claude/rules/git-authorship.md).
* La revisión que detecta divergencias por duplicación: [Review.md](Review.md).

*Este documento es la referencia para que el trabajo sobreviva a los límites de
memoria de los agentes; la regla que lo resume es: escribe el estado donde no se
borra y desconfía de lo que solo recuerdas.*
