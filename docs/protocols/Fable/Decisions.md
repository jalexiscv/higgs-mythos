# Protocolo de Decisiones de Diseño

Este documento define cómo debe tomar decisiones de diseño un agente de IA (o un
desarrollador) cuando una orden admite **más de una implementación razonable** en
**Higgs Mythos**. Su objetivo es eliminar la principal fuente de resultados
mediocres en este terreno: adoptar la primera idea que aparece sin haber
considerado alternativas, o adoptar la idea "más elegante" en abstracto en lugar
de la más coherente con el código real del framework y con los proyectos que ya
dependen de él.

---

## 1. Cuándo Aplica

Aplica a toda decisión **no trivial**: aquella en la que dos implementadores
competentes podrían razonablemente elegir caminos distintos (en qué módulo vive la
lógica, si una función nueva es un método de la clase o una función libre, si se
extiende una clase o se crea otra, si una opción nueva es un parámetro, una macro
`HIGGS_*` de configuración o una opción de CMake, cómo se nombra una API nueva,
si un cambio se hace compatible o rompiente). Las decisiones triviales (las que el
ejemplar de referencia del código ya resuelve) no requieren este procedimiento: se
sigue el ejemplar y punto.

---

## 2. Jerarquía de Criterios

Cuando las opciones entran en conflicto, se resuelve por este orden estricto. Un
criterio inferior **nunca** vence a uno superior.

1. **Corrección y seguridad.** Una opción que deja un caso límite roto, que puede
   corromper el estado de la aplicación del usuario o que abre una superficie
   insegura ante datos hostiles ([Security.md](Security.md)) queda descartada,
   por elegante que sea. La regla de origen de
   [contributing.md](../../../.github/contributing.md) pertenece a este nivel:
   ninguna opción puede apoyarse en código copiado de un binario propietario o
   de una fuente con licencia incompatible con la AGPL.
2. **Compatibilidad con los proyectos que usan el framework.** Higgs Mythos es
   una biblioteca: su API pública (clases, firmas, macros `HIGGS_*`, funciones
   `higgs_*` de CMake, formato de módulo, formato `.mythos`) es un contrato con
   código ajeno. Entre dos opciones correctas gana la que no obliga a los
   usuarios a cambiar su código. Romper el contrato solo se hace por decisión
   del usuario (§4.1), con entrada en `BREAKING_CHANGES.md`, y preferiblemente
   con un periodo `[[deprecated]]` antes de retirar.
3. **Consistencia con el código existente.** El patrón que el framework ya usa
   gana sobre el patrón "teóricamente mejor": `ListenerList` para observadores,
   `ValueTree` para estado estructurado, `Result` para fallos recuperables,
   `std::unique_ptr` para propiedad única, `ReferenceCountedObjectPtr` para
   compartida, un peer nativo por plataforma detrás de una interfaz común. Un
   código con dos maneras de hacer lo mismo es más caro de mantener que uno con
   una manera imperfecta y uniforme. Cambiar el patrón establecido es una
   decisión de alcance que se propone al usuario, no se toma en silencio. Las
   dependencias declaradas de los módulos
   ([Higgs Mythos Module Format.md](../../Higgs%20Mythos%20Module%20Format.md))
   no se violan nunca.
4. **Paridad entre plataformas.** Una función nueva de plataforma se diseña para
   que las cinco plataformas puedan implementarla con el mismo contrato público;
   lo que una plataforma no soporta se documenta en Doxygen y devuelve un valor
   neutro, no se oculta detrás de un `#if` en la cabecera pública.
5. **Diff mínimo completo y reversibilidad.** Entre opciones correctas,
   compatibles y consistentes, gana la que toca menos superficie y es más fácil
   de deshacer. Un parámetro nuevo con valor por defecto, o una sobrecarga
   nueva, es preferible a cambiar el significado de una firma existente.
6. **Simplicidad presente (YAGNI).** No se diseña para futuros hipotéticos: ni
   parámetros "por si acaso", ni capas de abstracción para un único caso de uso,
   ni configurabilidad que nadie pidió. La necesidad futura, cuando llegue,
   traerá su propia información de diseño.
7. **Rendimiento, con evidencia.** El framework corre en hilos de audio con
   plazos duros y en bucles de dibujo a 60 Hz; aun así, solo se sacrifica
   claridad por rendimiento cuando hay evidencia (medición con
   `AudioPerformanceTest`, un perfil, una prueba que lo demuestra) de que
   importa en este caso concreto. La única regla sin medición es la del hilo de
   audio: nada que bloquee, reserve memoria ni tome un `CriticalSection` dentro
   de `processBlock`.

---

## 3. Procedimiento

1. **Enumera al menos dos opciones reales** antes de decidir. Si solo se te
   ocurre una, todavía no exploraste lo suficiente: busca cómo resuelven el
   problema las clases hermanas del módulo y los peers nativos de las otras
   plataformas.
2. **Evalúa contra la jerarquía del §2**, por escrito si la decisión es de peso
   (dos líneas por opción bastan: qué gana, qué cuesta).
3. **Decide y ejecuta.** El análisis es para decidir, no para aplazar: elegida la
   opción, se implementa completa sin volver a abrir el debate a mitad de camino.
4. **Registra la justificación.** Toda decisión de peso queda documentada en el
   changelog del cambio ([Changelogs.md](../Changelogs.md)) y en el reporte final:
   qué se eligió, qué alternativas se consideraron y por qué se descartaron, en
   un párrafo. Esto evita que la siguiente sesión (u otro agente) re-litigue la
   decisión sin la información que la motivó.

---

## 4. Decisiones que No le Pertenecen al Agente

Estas decisiones se **presentan al usuario con una recomendación concreta** en
lugar de tomarse en silencio:

1. **Cambios rompientes de contratos externos:** la API pública de cualquier
   módulo, las macros `HIGGS_*` de configuración, las funciones y opciones de
   CMake de [CMake API.md](../../CMake%20API.md), el formato de módulo, el
   formato de proyecto `.mythos` y la interfaz de `higgsaide`, los formatos de
   plug-in generados (VST3, AU, AAX, LV2) y lo que los hosts esperan de ellos.
   Detectarlos es obligación del agente (búsqueda de usuarios del contrato);
   decidirlos es del usuario.
2. **La versión del framework** (`HIGGS_MAJOR_VERSION`, `HIGGS_MINOR_VERSION`,
   `HIGGS_BUILDNUMBER` en `higgs_StandardHeader.h`) y cuándo se publica una
   release: nunca se suben por iniciativa del agente.
3. **Criterios de producto** que el código ni la documentación revelan (¿cómo
   debe verse un control nuevo con el `LookAndFeel` por defecto?, ¿qué hace
   Mythos Studio con un proyecto antiguo?).
4. **Dependencias nuevas** (bibliotecas de terceros incrustadas en un módulo,
   SDKs, descargas de `FetchContent`, paquetes del sistema en Linux): solo con
   necesidad demostrada, licencia compatible con la AGPL-3.0-or-later, entrada
   en `HiggsMythos.spdx.json` y aprobación explícita.
5. **Eliminación de funcionalidad existente** que la orden no pidió eliminar,
   incluido el soporte de una plataforma, un compilador o una versión mínima de
   sistema de las que declara el `README.md`.
6. **Cambios de los requisitos mínimos** (estándar de C++, versión de CMake,
   Xcode, Visual Studio, NDK, API de Android, versiones mínimas de despliegue).

Todo lo demás —lo reversible, lo interno a un módulo, lo derivable del código y de
la documentación— se decide de forma autónoma, se ejecuta y se declara en el
reporte ([Execution.md](Execution.md) §3.4).

---

## 5. Reglas de Diseño Permanentes

1. **Aburrido gana a ingenioso.** El mejor código es el que un desarrollador del
   proyecto entiende en la primera lectura porque se parece al resto del
   framework. La astucia (metaprogramación de plantillas, indirección extra,
   trucos del lenguaje) requiere una justificación que casi nunca existe.
2. **Regla de tres para abstracciones.** No se crea una abstracción (clase base,
   helper genérico, plantilla) hasta que existan al menos tres usos reales que la
   demanden. Dos casos parecidos se toleran duplicados; al tercero se abstrae con
   conocimiento real de la variación.
3. **Los nombres son diseño.** Un concepto nuevo recibe un nombre coherente con
   la tabla de [higgs-mythos.md](../../higgs-mythos.md) y con las clases
   vecinas (`getX`/`setX`, `isX`, `addListener`/`removeListener`,
   `XListener::xChanged`), y se usa idéntico en la cabecera, los peers nativos,
   la documentación y las pruebas. Dos nombres para el mismo concepto es un bug
   de diseño.
4. **Deuda técnica solo deliberada.** Aceptar una solución imperfecta es legítimo
   cuando se declara: qué se sacrificó, por qué y qué haría falta para saldarla.
   Queda registrada en el changelog. La deuda silenciosa está prohibida.
5. **El módulo manda.** Antes de diseñar lógica, entiende el módulo donde vive:
   qué depende de él y de qué depende, si corre en el hilo de mensajes o en el de
   audio, si tiene peers nativos. La lógica que contradice la forma del módulo
   (GUI en `higgs_core`, bloqueos en el hilo de audio, tipos del sistema en una
   cabecera pública) se paga para siempre.
6. **Retirar con aviso.** Una API que deja de recomendarse se marca
   `[[deprecated]]` con el reemplazo en el mensaje, se documenta en
   `BREAKING_CHANGES.md` y sigue funcionando al menos una versión antes de
   desaparecer.

---

## 6. Anti-patrones Prohibidos

| Anti-patrón                                                        | Conducta correcta                                                  |
|--------------------------------------------------------------------|---------------------------------------------------------------------|
| Implementar la primera idea sin considerar alternativas            | Al menos dos opciones reales evaluadas (§3.1)                       |
| Cambiar una firma pública porque "queda mejor"                     | Compatibilidad primero; romper es decisión del usuario (§2.2, §4.1) |
| Introducir un patrón "mejor" que contradice el del framework       | Consistencia primero; cambiar el patrón es decisión del usuario (§2.3) |
| Incluir cabeceras de GUI en `higgs_core` o saltarse `dependencies` | Respetar las dependencias declaradas de los módulos (§2.3)          |
| Resolver una función de plataforma solo para la plataforma actual  | Contrato común y paridad entre plataformas (§2.4)                   |
| Reservar memoria o bloquear en `processBlock`                      | Hilo de audio sin bloqueos ni reservas (§2.7)                       |
| Abstracción especulativa para un solo caso de uso                  | Regla de tres (§5.2)                                                |
| Parámetros y opciones "por si acaso"                               | YAGNI: diseñar para el presente (§2.6)                              |
| Optimizar sin evidencia de que el rendimiento importa aquí         | Claridad por defecto; rendimiento con medición (§2.7)               |
| Subir la versión o publicar por cuenta propia                      | Decisión del usuario (§4.2)                                         |
| Añadir una dependencia sin aprobación ni entrada en el SBOM        | Necesidad, licencia compatible, SBOM y aprobación (§4.4)            |
| Decisión de peso sin justificación registrada                      | Párrafo de justificación en changelog y reporte (§3.4)              |
| Reabrir a mitad de la implementación una decisión ya tomada        | Decidir, ejecutar completo, y solo reevaluar con información nueva (§3.3) |

---

## 7. Relación con Otros Protocolos

* El ciclo de trabajo donde estas decisiones ocurren (fase de planificación):
  [Execution.md](Execution.md).
* Los nombres y la identidad que el criterio de consistencia protege:
  [docs/higgs-mythos.md](../../higgs-mythos.md).
* Las dependencias de los módulos:
  [docs/Higgs Mythos Module Format.md](../../Higgs%20Mythos%20Module%20Format.md).
* Los contratos de CMake: [docs/CMake API.md](../../CMake%20API.md).
* El registro de la justificación: [Changelogs.md](../Changelogs.md).

*Este documento es la referencia sobre cómo decidir; la regla que lo resume es:
correcto, compatible, consistente, uniforme entre plataformas, mínimo y simple — en
ese orden.*
