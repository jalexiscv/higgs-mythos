# Protocolo de Registro de Cambios (Changelog)

Este documento define cómo se registra **todo cambio** del código, las pruebas, la
documentación, la compilación o las herramientas de **Higgs Mythos**. Lo siguen por
igual los desarrolladores y los agentes de IA. El historial vive en el directorio
`changelogs/` de la raíz del repositorio y complementa, no reemplaza, los mensajes
de commit de [contributing.md](../../.github/contributing.md) ni los dos registros
públicos heredados del framework: `BREAKING_CHANGES.md` (qué rompe y cómo migrar) y
`CHANGE_LIST.md` (titulares por versión).

El formato se basa en [Keep a Changelog](https://keepachangelog.com/es-ES/1.0.0/)
y el proyecto adhiere al [Versionado Semántico](https://semver.org/lang/es/).

---

## 1. Principios

1. **Todo cambio se registra en la misma sesión en que se hace**, no después.
2. **Dos piezas por cambio:** un archivo detallado que cuenta el cambio y una
   línea en el índice `changelogs/CHANGELOG.md` que lo resume y enlaza.
3. **El changelog cuenta la transición; la documentación, el estado.** Lo que el
   framework *es* vive en `docs/` y en los comentarios Doxygen
   ([Documentations.md](Documentations.md)); lo que *cambió* y por qué vive aquí.
4. **El commit y el changelog se complementan.** El commit (en inglés,
   `area: resumen`) identifica el cambio en `git`; el changelog (en español) lo
   explica para quien no lee el diff: causa, decisión, verificación e impacto.
5. **Cada cambio declara su tiempo laborado y su autor** según las secciones 5
   y 6. Un agente de IA nunca es autor ni coautor, ni aquí ni en `git`
   ([.claude/rules/git-authorship.md](../../.claude/rules/git-authorship.md)).
6. **Un cambio sin evidencia no está terminado:** el detalle dice cómo se
   verificó ([Fable/Execution.md](Fable/Execution.md), fase 5).
7. **Los registros públicos se mantienen aparte.** Un cambio rompiente de la API
   añade su entrada en `BREAKING_CHANGES.md`; una funcionalidad notable, su
   titular en `CHANGE_LIST.md`. El changelog los enlaza, no los sustituye.

---

## 2. Estructura de archivos

```
Higgs Mythos/
├── BREAKING_CHANGES.md            ← cambios rompientes por versión (inglés, público)
├── CHANGE_LIST.md                 ← titulares por versión (inglés, público)
├── docs/protocols/Changelogs.md   ← este protocolo
└── changelogs/
    ├── CHANGELOG.md               ← índice numerado (una línea por cambio)
    ├── YYYY-MM-DD_descripcion.md  ← detalle de un cambio
    └── ...
```

En `changelogs/` solo viven el índice y los archivos detallados, todos `.md`.

---

## 3. El índice (`changelogs/CHANGELOG.md`)

Es un **índice numerado y resumido**: una fila por cambio, sin detalles, con
enlace al archivo detallado.

```markdown
# Changelog - Higgs Mythos

| #  | Fecha      | Versión | Área             | Descripción breve                                     | Detalle                                              |
|----|------------|---------|------------------|-------------------------------------------------------|------------------------------------------------------|
| 2  | 2026-11-05 | 9.0.5   | higgs_core       | `ValueTree` rechaza árboles anidados a más de 256 niveles | [Ver detalle](2026-11-05_valuetree_profundidad.md) |
| 1  | 2026-11-03 | 9.0.5   | docs             | Protocolos de trabajo adaptados a Higgs Mythos        | [Ver detalle](2026-11-03_protocolos_y_configuracion_claude.md) |
```

Reglas:

- **Numeración ascendente y consecutiva**, sin saltos ni repeticiones; la fila
  **más reciente va primero** (orden cronológico inverso).
- **Fecha** en formato `AAAA-MM-DD`.
- **Versión** del framework en la que entra el cambio: la que declaran
  `HIGGS_MAJOR_VERSION`, `HIGGS_MINOR_VERSION` y `HIGGS_BUILDNUMBER` en
  `modules/higgs_core/system/higgs_StandardHeader.h` en el momento del cambio.
  Subir la versión es decisión del usuario ([Fable/Decisions.md](Fable/Decisions.md)
  §4); el agente no la edita por iniciativa propia.
- **Área** principal del cambio, con los nombres de los commits: el nombre del
  módulo (`higgs_core`, `higgs_gui_basics`, `higgs_audio_processors`, …) o, fuera
  de los módulos, `examples`, `extras`, `studio` (Mythos Studio), `docs`,
  `build` (CMake y `extras/Build`), `ci`, `sbom`, `license`.
- **Descripción breve** en una sola línea: qué cambió y, si cabe, por qué. Admite
  Markdown en línea (`código`, **negrita**); no admite saltos de línea.
- **Detalle**: siempre un enlace `[Ver detalle](archivo.md)` al archivo detallado,
  sin rutas, al final de la fila.

---

## 4. El archivo detallado

### 4.1. Nombre

```
YYYY-MM-DD_descripcion_breve.md
```

La fecha es la del cambio; la descripción va en minúsculas, con guiones bajos y
sin tildes. Ejemplo: `2026-11-05_valuetree_profundidad.md`.

### 4.2. Plantilla

```markdown
# Título descriptivo del cambio

**Fecha:** YYYY-MM-DD
**Versión:** X.Y.Z                                 ← versión de higgs_StandardHeader.h
**Área:** <área> > [Componente] > [Subcomponente]
**Tiempo laborado:** N horas
**Autor:** Nombre Apellido <correo@dominio>
**Asistencia:** Agente de IA (<agente o modelo>)   ← opcional (§6.2)
**Commit:** <hash corto>                           ← opcional (§4.3)

## Descripción

Qué cambió y por qué: el problema o la necesidad, y el resultado. Si fue una
corrección, la causa raíz, no solo el síntoma.

### Decisión de diseño            ← opcional, obligatoria en decisiones de peso

Opciones consideradas, cuál se eligió y por qué se descartaron las demás
([Fable/Decisions.md](Fable/Decisions.md) §3.4).

## Tipo de Cambio

- `Agregado` | `Cambiado` | `Corregido` | `Removido` | `Obsoleto` | `Seguridad`

## Archivos Afectados

### [NUEVO|MODIFICADO|ELIMINADO] `modules/higgs_<área>/...`
- Qué se hizo en este archivo.

## Verificación

- Compilación: generador, configuración y objetivos compilados
  (p. ej. `cmake --build cmake-build --target UnitTestRunner DemoRunner`), con
  cero avisos, y en qué plataforma.
- Pruebas: la invocación de `UnitTestRunner` (categoría o nombre) y su línea de
  resumen, tal cual.
- Flujo ejercitado con `DemoRunner`, `AudioPluginHost`, un ejemplo de
  `examples/` o Mythos Studio, y lo que se observó.
- Lo que no se pudo verificar (por ejemplo, macOS, iOS, Android, Linux), dicho
  explícitamente.

## Impacto

- Efecto funcional, módulos y plataformas afectados, compatibilidad binaria y de
  API con proyectos que usan el framework, cambios rompientes (con enlace a la
  entrada de `BREAKING_CHANGES.md`), titular añadido a `CHANGE_LIST.md` si lo hay.
```

### 4.3. Reglas del contenido

- **Cabecera fija:** la primera línea es el título `# …`, seguido de las líneas
  `**Fecha:**`, `**Versión:**`, `**Área:**`, `**Tiempo laborado:**` y
  `**Autor:**`, en ese orden,
  y de `**Asistencia:**` y `**Commit:**` cuando correspondan.
- **Commit:** solo cuando el changelog se registra *después* de confirmar el
  cambio (un registro retroactivo). El changelog que viaja en el mismo commit que
  su cambio no lleva esta línea —un commit no puede contener su propio hash— y se
  localiza con `git log -- changelogs/<archivo>.md`.
- **Tipos de cambio:**

  | Tipo        | Uso                                           |
  |-------------|-----------------------------------------------|
  | `Agregado`  | Funcionalidades o archivos nuevos             |
  | `Cambiado`  | Modificaciones de funcionalidades existentes  |
  | `Corregido` | Corrección de defectos                        |
  | `Removido`  | Funcionalidades o archivos eliminados         |
  | `Obsoleto`  | Funcionalidades marcadas para retirarse       |
  | `Seguridad` | Corrección de vulnerabilidades                |

- **Cambios rompientes:** si el cambio altera la API pública, el detalle lo dice
  en «Impacto» y enlaza la entrada añadida a `BREAKING_CHANGES.md`.
- **Sin secretos ni datos personales:** ni credenciales, ni claves de
  `higgs_product_unlocking`, ni certificados de firma, ni archivos de usuarios
  ([Fable/Security.md](Fable/Security.md) §6).
- **Legible y breve:** prosa clara en español; bloques de código solo para
  diferencias críticas.

### 4.4. Un archivo por conjunto de cambios relacionados

- Los cambios relacionados de una misma sesión sobre el mismo componente van
  en **un solo archivo** y **una sola fila** del índice.
- Un ajuste posterior que completa un cambio de la misma sesión (p. ej. una
  prueba que faltaba para la clase recién creada) se **agrega a su archivo
  detallado**: el archivo se amplía, su fila del índice se ajusta si hace falta
  y su tiempo laborado se recalcula (el autor se conserva; si otra persona
  aporta al ajuste, se añade como coautor, §6.3).
- Un cambio con propósito propio abre archivo y número nuevos.

---

## 5. Tiempo laborado

Cada archivo detallado declara el tiempo laborado del cambio en su cabecera:

```markdown
**Tiempo laborado:** 16 horas
```

### 5.1. Cómo se calcula

1. **Se estima** en horas de trabajo humano lo que le tomaría el cambio a un
   desarrollador experimentado en C++ y en el framework **que no conoce la causa
   de antemano**: diagnóstico, implementación, pruebas, compilación en las
   plataformas, verificación y documentación incluidos.
2. **Se registra el doble** de esa estimación. Ese valor es el *tiempo
   laborado*; la estimación original no se escribe.

### 5.2. Franjas de referencia

La estimación parte de estas franjas y se ajusta por el alcance real del cambio:
número de módulos y plataformas afectados, profundidad del diagnóstico, extensión
de las pruebas y riesgo.

| Estimación | Tipo de cambio                                                        | Tiempo laborado |
|------------|-----------------------------------------------------------------------|-----------------|
| 0,5–1 h    | Ajuste de texto, documentación Doxygen o renombre                     | 1–2 h           |
| 2–4 h      | Falla puntual en un módulo con diagnóstico y prueba de regresión      | 4–8 h           |
| 3–6 h      | Método o clase nueva en un módulo, opción de CMake, demo nueva        | 6–12 h          |
| 8–12 h     | Funcionalidad que cruza módulos o plataformas (p. ej. core + gui + un peer nativo) | 16–24 h |
| 20–40 h    | Módulo nuevo, formato de plug-in, backend gráfico o de audio, revisión integral | 40–80 h |

### 5.3. Reglas

- **Formato exacto:** `**Tiempo laborado:** N horas`, con `N` en cifras (entero,
  o decimal con coma: `2,5`). Sin rangos ni texto adicional en esa línea.
- **Un valor por archivo:** si el archivo reúne varios cambios relacionados
  (§4.4), declara la suma de todos.
- **Se fija al registrar** el cambio; si el archivo se amplía después, el valor
  se actualiza en esa misma edición.
- **No se deduce del tiempo real de la sesión:** sale siempre de la estimación
  humana y su doble, para que los registros sean comparables entre sí.

---

## 6. Autoría

Cada archivo detallado declara quién es responsable del cambio:

```markdown
**Autor:** Nombre Apellido <correo@dominio>
```

### 6.1. Quién es el autor

El autor es la **persona** que ordena o aprueba el cambio y responde por él:
quien lo pidió, quien decidió sus alternativas y quien lo da por bueno. Un
agente de IA nunca figura como autor, ni aquí ni en el commit
([.claude/rules/git-authorship.md](../../.claude/rules/git-authorship.md)).

### 6.2. Asistencia de IA

Cuando un agente de IA ejecuta el cambio (diagnóstico, código, verificación o
redacción), se añade la línea opcional:

```markdown
**Asistencia:** Agente de IA (Claude)
```

Declararla da trazabilidad y no cambia la responsabilidad, que sigue siendo del
autor. Es el **único** lugar donde se menciona la asistencia: el commit, el
código y la documentación no la mencionan. El tiempo laborado se calcula igual
(§5): mide el trabajo humano equivalente, no el tiempo del agente.

### 6.3. Reglas

- **Formato:** `Nombre Apellido <correo>`, con la **misma identidad** que el
  autor usa en `git` (`git config user.name` y `user.email`), para que un mismo
  autor se escriba siempre igual y los reportes lo agrupen.
- **Coautoría:** varias personas se separan con `;` en la misma línea
  (`Ana Pérez <ana@x>; Luis Gómez <luis@x>`); el tiempo laborado sigue siendo
  uno solo para el cambio.
- **Cambios heredados:** si no se conoce el autor de un cambio antiguo, se
  escribe `**Autor:** No registrado`; no se inventa.
- **Dato personal mínimo:** solo nombre y correo, ningún otro dato de la
  persona ([Fable/Security.md](Fable/Security.md) §6).

---

## 7. Flujo para registrar un cambio

1. Terminar y verificar el cambio (compilación sin avisos y pruebas en verde).
2. Crear `changelogs/YYYY-MM-DD_descripcion.md` con la plantilla de §4.2,
   incluidos el tiempo laborado (§5) y la autoría (§6).
3. Agregar la fila al **principio** de la tabla de `changelogs/CHANGELOG.md`, con
   el número siguiente al más alto.
4. Si el cambio altera cómo *es* el framework, actualizar `docs/` o los
   comentarios Doxygen ([Documentations.md](Documentations.md)).
5. Si el cambio rompe la API, añadir la entrada en `BREAKING_CHANGES.md`; si es
   una funcionalidad notable, el titular en `CHANGE_LIST.md`.
6. Si la orden incluye versionar: el changelog entra en el mismo commit que el
   cambio (sin línea `**Commit:**`, §4.3), con mensaje `area: resumen` en inglés
   y sin ninguna línea de atribución a la IA.
7. Revisar la lista de §8.

---

## 8. Lista de comprobación

- [ ] El archivo detallado existe, con nombre `YYYY-MM-DD_descripcion.md`.
- [ ] Su cabecera tiene título `# …`, **Fecha**, **Versión**, **Área**,
      **Tiempo laborado** y **Autor**.
- [ ] La versión es la de `higgs_StandardHeader.h` y no se editó por iniciativa
      del agente.
- [ ] El tiempo laborado es el doble de la estimación humana y sigue el formato
      de §5.3.
- [ ] El autor es una persona, con la identidad de `git` (§6.3), y la asistencia
      de IA está declarada si la hubo.
- [ ] Tiene Descripción (con la causa raíz si es corrección), Tipo de Cambio,
      Archivos Afectados, Verificación e Impacto.
- [ ] Las decisiones de peso tienen su justificación.
- [ ] Los cambios rompientes están en `BREAKING_CHANGES.md` y los notables en
      `CHANGE_LIST.md`.
- [ ] El índice tiene la fila nueva al principio, con número consecutivo,
      versión, área y enlace correcto.
- [ ] La documentación de `docs/` o Doxygen refleja el estado final, si aplica.
- [ ] No hay secretos ni datos personales.

---

*Este documento es la referencia para desarrolladores y agentes de IA sobre cómo
se lleva el historial de cambios de Higgs Mythos; la regla que lo resume es: todo
cambio queda contado, verificado, con su tiempo laborado y su autor, en el
momento en que se hace.*
