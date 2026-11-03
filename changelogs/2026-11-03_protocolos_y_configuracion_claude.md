# Protocolos de trabajo adaptados a Higgs Mythos y configuración `.claude`

**Fecha:** 2026-11-03
**Versión:** 9.0.5
**Área:** docs > protocols
**Tiempo laborado:** 6 horas
**Autor:** Jose Alexis Correa Valencia <jalexiscv@gmail.com>
**Asistencia:** Agente de IA (Claude)

## Descripción

Los ocho protocolos de `docs/protocols/` provenían de otro proyecto (un procesador
de textos de escritorio) y describían su arquitectura, sus herramientas y sus
reglas de licencia: capas `core`/`model`/`layout`, pruebas Catch2, `skriba-cli`,
especificación de una versión web, sala limpia frente a Word, licencia
propietaria, releases con `release.ps1`, hoja de ruta `plan.md` y documentos de
`docs/` que aquí no existen. Un agente que los siguiera al pie de la letra
trabajaría contra referencias inexistentes.

Se reescribieron los ocho documentos para Higgs Mythos conservando su estructura
(principio rector, procedimiento, anti-patrones, checklist, relación entre
protocolos) y sustituyendo cada regla específica por la equivalente del
framework: módulos `higgs_<área>` y sus dependencias declaradas, peers nativos
por plataforma, pruebas `UnitTest` ejecutadas por `UnitTestRunner`, verificación
funcional con `DemoRunner`, `AudioPluginHost` y Mythos Studio, compilación con
CMake (`cmake-build`), registros públicos `BREAKING_CHANGES.md` y
`CHANGE_LIST.md`, licencia AGPL-3.0-or-later con cabeceras de copyright
intactas, SBOM `HiggsMythos.spdx.json`, canal privado de `SECURITY.md`, y la
versión del framework tomada de `higgs_StandardHeader.h` porque no existe un
protocolo de releases. Se añadieron reglas propias de una biblioteca: la
compatibilidad con los proyectos que la usan como criterio de diseño, la paridad
entre las cinco plataformas, el hilo de audio sin bloqueos ni reservas y la
revisión de los clientes internos de toda API pública.

En la misma sesión se creó el directorio `.claude/` con la configuración que
impide que el agente se atribuya autoría en Git (sin trailers `Co-Authored-By` ni
menciones a la IA en commits, código o documentación) y con la regla que obliga a
seguir los protocolos, y se enlazó todo desde `CLAUDE.md` y desde el índice
`docs/README.md`.

### Decisión de diseño

Se consideró dejar los protocolos sin tocar y limitarse a enlazarlos, o borrar
los que no tenían equivalente aquí (releases, publicación, tipografías). Se
descartó lo primero porque un protocolo con referencias falsas es peor que
ninguno: obliga al agente a ignorarlo o a inventar. Se descartó borrar
`Changelogs.md` y `Documentations.md` porque el usuario los quiere vigentes; en
su lugar se eliminaron solo las referencias a los documentos ausentes
(`Releases.md`, `Publishing.md`, `Tipografias.md`, `plan.md`,
`docs/arquitectura.md`, `docs/estilo-de-codigo.md`, `docs/especificacion-web/`) y
se apuntó a los archivos que sí existen (`.github/contributing.md`,
`docs/higgs-mythos.md`, `docs/CMake API.md`, `docs/Higgs Mythos Module Format.md`).

Sobre el idioma: la política pide documentación en español, pero la referencia
heredada de `docs/` (`CMake API.md`, formato de módulo, dependencias de Linux,
`BREAKING_CHANGES.md`, `CHANGE_LIST.md`) está en inglés y la consumen usuarios del
framework. Se decidió que los documentos nuevos, los protocolos y los changelogs
van en español, y que los heredados y los registros públicos se mantienen en
inglés, en lugar de traducirlos a medias.

## Tipo de Cambio

- `Cambiado` (protocolos)
- `Agregado` (configuración `.claude`, índice de changelogs)

## Archivos Afectados

### [MODIFICADO] `docs/protocols/Documentations.md`
- Estructura de directorios, idiomas, índice maestro (`docs/README.md`) y las
  tres capas de registro (docs, changelogs, `BREAKING_CHANGES.md`) de este repo.

### [MODIFICADO] `docs/protocols/Changelogs.md`
- Áreas tomadas de los nombres de módulo usados en los commits; versión desde
  `higgs_StandardHeader.h`; verificación con `UnitTestRunner`; relación con
  `BREAKING_CHANGES.md` y `CHANGE_LIST.md`; autoría sin IA enlazada a
  `.claude/rules/git-authorship.md`; sin sección de notas de release.

### [MODIFICADO] `docs/protocols/Fable/Execution.md`
- Ciclo de siete fases con compilación CMake, pruebas `UnitTest`, verificación
  funcional con las herramientas de `extras/`, estilo del framework, cabeceras
  de licencia, regla de origen compatible con AGPL y declaración de plataformas
  no compiladas.

### [MODIFICADO] `docs/protocols/Fable/Decisions.md`
- Jerarquía con compatibilidad de API y paridad de plataformas; decisiones
  reservadas al usuario (API pública, versión, dependencias y SBOM, requisitos
  mínimos); retirada con `[[deprecated]]`.

### [MODIFICADO] `docs/protocols/Fable/Debugging.md`
- Familias de hipótesis con hilos y plataforma; bisección por el flujo del
  framework; reproducción con `UnitTest`, `DemoRunner` y `AudioPluginHost`;
  fallos de plataformas no ejecutables declarados como no verificados.

### [MODIFICADO] `docs/protocols/Fable/Review.md`
- Catálogo de defectos con hilos y tiempo de vida, paridad de peers, clientes
  internos de la API y profundidad por tipo de módulo.

### [MODIFICADO] `docs/protocols/Fable/Security.md`
- Superficie de ataque del framework (decodificadores, plug-ins, red), hilo de
  audio, SDKs de plug-ins, licencias compatibles con AGPL, SBOM, secretos de
  firma y de `higgs_product_unlocking`, canal de `SECURITY.md`.

### [MODIFICADO] `docs/protocols/Fable/Continuity.md`
- Referencias a `changelogs/`, series de commits numeradas, `cmake-build` como
  artefacto que no se confirma, peers a medias declarados al pausar.

### [NUEVO] `.claude/settings.json`
- `attribution.commit` y `attribution.pr` vacíos e `includeCoAuthoredBy: false`.

### [NUEVO] `.claude/rules/git-authorship.md`
- Prohibición de trailers de coautoría, menciones a la IA y cambios de identidad
  de Git, con comando de verificación.

### [NUEVO] `.claude/rules/protocols.md`
- Tabla de qué protocolo leer en cada situación y reglas de aplicación.

### [MODIFICADO] `CLAUDE.md`
- Importa las dos reglas de `.claude/rules/`, `Execution.md` y `Changelogs.md`.

### [MODIFICADO] `docs/README.md`
- Entrada de `protocols/` y `changelogs/` en el índice maestro.

### [NUEVO] `changelogs/CHANGELOG.md`
- Índice creado con esta primera fila.

## Verificación

- Todos los identificadores del framework citados en los protocolos
  (`UnitTest`, `UnitTestRunner`, `HIGGS_UNIT_TESTS`, `jassert`, `DBG`,
  `HIGGS_LEAK_DETECTOR`, `HIGGS_DECLARE_NON_COPYABLE`, `TemporaryFile`,
  `overwriteTargetFileWithTemporary`, `AccessibilityHandler`,
  `ReferenceCountedObjectPtr`, `WeakReference`, `Component::SafePointer`,
  `CriticalSection`, `AbstractFifo`, `ZipFile`, `XmlDocument`, `URL`,
  `ChildProcess`, `JavascriptEngine`, `PluginDirectoryScanner`,
  `KnownPluginList`, `AudioPluginFormatManager`, `OnlineUnlockStatus`, `RSAKey`,
  `PropertiesFile`, `StreamingSocket`, `AudioIODeviceCallback`,
  `File::isAChildOf`, `ImageFileFormat::findImageFormatForStream`, los macros
  `HIGGS_WINDOWS`/`HIGGS_MAC`/`HIGGS_ANDROID` y el patrón de nombres
  `native/higgs_<Nombre>_<plataforma>`) se comprobaron con búsquedas en
  `modules/`.
- Todos los enlaces relativos de los ocho protocolos, de `CLAUDE.md` y de
  `docs/README.md` se comprobaron contra el árbol de archivos.
- Se comprobó con búsqueda que no quedan referencias al proyecto de origen ni a
  sus archivos (`Skriba`, `skriba-cli`, Catch2, `Releases.md`, `plan.md`,
  `arquitectura.md`, `estilo-de-codigo.md`, `especificacion-web`) en
  `docs/protocols/`.
- No hay cambios de código: no aplica compilación ni `UnitTestRunner`.

## Impacto

- Documental y de proceso. Ningún módulo, API, archivo de CMake ni requisito
  cambia; no hay entrada en `BREAKING_CHANGES.md` ni en `CHANGE_LIST.md`.
- A partir de este cambio, todo trabajo de un agente en el repositorio debe
  terminar con un changelog en `changelogs/` y sin atribución a la IA en Git.
