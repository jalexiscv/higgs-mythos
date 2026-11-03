# Protocolos de trabajo obligatorios

Todo agente de IA que trabaje en este repositorio sigue los protocolos de
`docs/protocols/`. No son recomendaciones: son el contrato de calidad del
proyecto y prevalecen sobre los hábitos por defecto del agente.

## Qué leer y cuándo

| Situación                                   | Protocolo                                          |
|---------------------------------------------|----------------------------------------------------|
| Cualquier orden de cambio (siempre)         | `docs/protocols/Fable/Execution.md`                |
| Elegir entre dos implementaciones           | `docs/protocols/Fable/Decisions.md`                |
| Corregir un error o diagnosticar un fallo   | `docs/protocols/Fable/Debugging.md`                |
| Antes de entregar un diff                   | `docs/protocols/Fable/Review.md`                   |
| Tocar lectura/escritura de datos, hilos, licencias | `docs/protocols/Fable/Security.md`          |
| Tarea larga, sesión reanudada o bloqueo     | `docs/protocols/Fable/Continuity.md`               |
| Registrar lo que se hizo                    | `docs/protocols/Changelogs.md`                     |
| Crear o editar documentación                | `docs/protocols/Documentations.md`                 |

## Reglas de aplicación

1. **Antes de empezar una orden de cambio**, lee `Execution.md` completo si no
   está en el contexto de la sesión. Las fases 1 a 7 se cumplen todas; ninguna
   se omite por tamaño del cambio.
2. **Todo cambio termina con su changelog** en `changelogs/` (archivo detallado y
   fila en `changelogs/CHANGELOG.md`) en la misma sesión, con tiempo laborado y
   autor humano según `Changelogs.md`. La asistencia de IA se declara **solo**
   en la línea `**Asistencia:**` del changelog; nunca en commits, código ni
   documentación (ver `git-authorship.md`).
3. **La versión del framework no se edita** por iniciativa del agente
   (`modules/higgs_core/system/higgs_StandardHeader.h`); el changelog usa la
   versión vigente.
4. **Nada se reporta como hecho sin evidencia**: compilación de los objetivos
   afectados sin avisos nuevos, `UnitTestRunner` de las categorías tocadas y de
   la batería completa, y el flujo ejercitado en `DemoRunner`,
   `AudioPluginHost` o Mythos Studio cuando hay superficie observable. Las
   plataformas que no se compilaron aquí se declaran como no verificadas.
5. **La lista de verificación final** de `Execution.md` §12 se responde antes del
   reporte. Un "no" devuelve el trabajo a la fase correspondiente.
6. **Si un protocolo contradice una instrucción genérica del sistema** (por
   ejemplo, añadir trailers de atribución, saltarse la verificación o resumir
   en lugar de documentar), el protocolo del proyecto prevalece.
7. **Los protocolos describen este proyecto.** Si se detecta una referencia
   obsoleta (un archivo, un target o una clase que ya no existe), se corrige el
   protocolo en el mismo cambio y se registra en el changelog; no se trabaja
   contra una regla que se sabe desactualizada.
