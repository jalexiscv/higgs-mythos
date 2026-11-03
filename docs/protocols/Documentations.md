# Estrategia de Documentación

Este documento define las políticas y directrices para la documentación continua de
**Higgs Mythos**. Su propósito es guiar a desarrolladores y agentes de Inteligencia
Artificial (IA) para mantener la consistencia, integridad y utilidad de la
documentación técnica del framework.

## 1. Principios Fundamentales

*   **Modularidad**: La documentación no debe ser un monolito. Se divide en
    documentos por tema (un archivo por tema).
*   **Veracidad**: La documentación debe reflejar fielmente el código. Si el código
    cambia, la documentación se actualiza en el mismo cambio.
*   **La fuente es el código**: la referencia de la API vive en los comentarios
    Doxygen de las cabeceras de `modules/`. Los documentos de `docs/` explican lo
    que las cabeceras no pueden: convenciones, formato de módulos, API de CMake,
    dependencias de plataforma, identidad del proyecto y protocolos de trabajo.
*   **Centralización**: Toda la documentación técnica reside en `docs/` (incluidos
    estos protocolos, en `docs/protocols/`). La raíz del repositorio solo tiene los
    archivos que las herramientas y las personas esperan encontrar allí:
    `README.md` y sus traducciones `README.<idioma>.md`, `LICENSE.md`,
    `SECURITY.md`, `CODE_OF_CONDUCT.md`, `BREAKING_CHANGES.md`,
    `CHANGE_LIST.md`, `HiggsMythos.spdx.json`, `AGENTS.md` y `CLAUDE.md`. La
    guía de contribución está en `.github/contributing.md`.
*   **README multilingüe**: `README.md` se escribe en español y es la fuente;
    `README.en.md`, `README.pt.md`, `README.fr.md`, `README.de.md`,
    `README.it.md`, `README.zh-CN.md` y `README.ja.md` son sus traducciones.
    Todo cambio del `README.md` se replica en las ocho versiones en la misma
    sesión, y cada archivo conserva el selector de idioma de su cabecera.
*   **Trazabilidad**: La documentación describe *cómo es* el framework; el
    historial de *qué cambió* vive en `changelogs/` (ver
    [Changelogs.md](Changelogs.md)). No mezclar ambos.
*   **Idioma**: la documentación nueva de `docs/` y los changelogs se escriben en
    español; el código, los comentarios Doxygen, los commits, `BREAKING_CHANGES.md`
    y `CHANGE_LIST.md` se escriben en inglés
    ([contributing.md](../../.github/contributing.md)). Los documentos heredados
    en inglés (`CMake API.md`, `Higgs Mythos Module Format.md`,
    `Linux Dependencies.md`, `Accessibility.md`, `ARA.md`) se mantienen en inglés:
    se editan en su idioma, no se traducen a medias.

## 2. Estructura de Directorios

```
Higgs Mythos/
├── README.md                       <-- Punto de entrada en español (visión general, requisitos, licencia)
├── README.<idioma>.md              <-- Traducciones del README (en, pt, fr, de, it, zh-CN, ja)
├── AGENTS.md                       <-- Puntero para agentes: leer README.md
├── CLAUDE.md                       <-- Reglas para la IA (importa README y protocolos)
├── BREAKING_CHANGES.md             <-- Cambios rompientes por versión (inglés)
├── CHANGE_LIST.md                  <-- Titulares de cada versión (inglés)
├── SECURITY.md                     <-- Cómo reportar vulnerabilidades
├── HiggsMythos.spdx.json           <-- Inventario SBOM de terceros
├── .github/contributing.md         <-- Cómo contribuir: idioma, origen del código, PRs
├── .claude/                        <-- Configuración y reglas del agente (autoría en git)
├── modules/higgs_<área>/           <-- El framework; la API se documenta con Doxygen aquí
├── docs/                           <-- Directorio PRINCIPAL de documentación
│   ├── README.md                   <-- Índice maestro de docs/
│   ├── higgs-mythos.md             <-- Identidad: nombres, licencia, procedencia, compilación
│   ├── CMake API.md                <-- Funciones higgs_add_*, targets y opciones
│   ├── Higgs Mythos Module Format.md <-- Declaración y cabecera de un módulo
│   ├── Linux Dependencies.md       <-- Paquetes necesarios en Linux
│   ├── Accessibility.md, ARA.md    <-- Notas de integración por plataforma y plug-in
│   ├── doxygen/                    <-- Configuración para generar la referencia HTML
│   ├── <tema>.md                   <-- Documentos nuevos por tema
│   └── protocols/
│       ├── Documentations.md       <-- Este archivo (política general)
│       ├── Changelogs.md           <-- Protocolo del historial de cambios
│       └── Fable/                  <-- Protocolos de trabajo de los agentes de IA
└── changelogs/                     <-- Historial de cambios (índice + detalle por cambio)
```

> El índice maestro de la documentación es la lista de **`docs/README.md`**. Un
> archivo de `docs/` que no esté enlazado desde ese índice (o desde un documento
> que el índice enlaza) es un error de documentación (archivo huérfano). Los
> protocolos de `docs/protocols/` se enlazan desde `docs/README.md` y se importan
> desde `CLAUDE.md`.

### Las tres capas de registro

| Pregunta                                 | Dónde se responde                                          |
|------------------------------------------|------------------------------------------------------------|
| ¿Cómo es el framework hoy?               | `docs/` y los comentarios Doxygen de `modules/`            |
| ¿Qué cambió, por qué y cómo se verificó? | `changelogs/` ([Changelogs.md](Changelogs.md))             |
| ¿Qué rompe al actualizar y cómo migrar?  | `BREAKING_CHANGES.md`, con su titular en `CHANGE_LIST.md`  |

Un cambio rompiente de la API pública (firma, comportamiento, macro, función de
CMake) **siempre** entra en `BREAKING_CHANGES.md` con la plantilla que ese archivo
ya usa (`## Change`, `**Possible Issues**`, `**Workaround**`), además de su
changelog.

## 3. Flujo de Trabajo para Documentar (Instrucciones para IA)

Al documentar una nueva funcionalidad o componente, sigue estos pasos estrictos:

### Paso 1: Identificar el Documento
Determina dónde pertenece la documentación.
*   *API de una clase o función*: en el comentario Doxygen de su cabecera, en
    inglés, con el estilo del archivo (`/** ... */`, `@param`, `@returns`,
    `@see`). No se crea un documento en `docs/` para describir una clase.
*   *Convención, formato o herramienta*: edita el archivo correspondiente en
    `docs/` (nombres en `higgs-mythos.md`, funciones de CMake en `CMake API.md`,
    cabecera de módulo en `Higgs Mythos Module Format.md`).
*   *Tema nuevo*: crea `docs/<tema>.md`, con un nombre descriptivo en español, en
    minúsculas, con guiones y sin tildes (p. ej. `docs/pruebas-unitarias.md`).

### Paso 2: Estructura del Archivo
Cada documento nuevo debe seguir esta plantilla mínima:
1.  **Título**: Nombre del tema (p. ej. `# Pruebas unitarias`).
2.  **Descripción**: Breve explicación del tema en el contexto de Higgs Mythos.
3.  **Detalle técnico**: módulos y archivos implicados
    (`modules/higgs_<área>/...`), clases y funciones principales, macros
    `HIGGS_*`, opciones y funciones de CMake, targets (`UnitTestRunner`,
    `DemoRunner`, `AudioPluginHost`, `MythosStudio`).
4.  **Flujos**: procesos principales cuando apliquen (configurar, compilar,
    ejecutar, integrar en un proyecto propio).
5.  **Límites conocidos**: plataformas, compiladores o formatos no soportados o
    aproximados, dicho sin rodeos.

### Paso 3: Actualizar el Índice
**CRÍTICO**: Si creas un archivo nuevo en `docs/`, DEBES añadirlo a la lista de
`docs/README.md`. No dejar archivos huérfanos.

### Paso 4: Referencias Cruzadas
Si la documentación afecta la visión general del proyecto (módulos nuevos,
requisitos de sistema, herramientas), actualiza también las secciones relevantes
del `README.md` raíz. Enlaza a la fuente de verdad existente en vez de duplicar
información.

### Paso 5: Cambios de la API
Si el cambio altera o retira algo público, añade la entrada en
`BREAKING_CHANGES.md` bajo la versión en curso, y el titular en `CHANGE_LIST.md`
si es una funcionalidad notable.

## 4. Convenciones de Estilo

*   **Idioma**: Español (técnico y formal) en los documentos nuevos; inglés en los
    heredados y en todo lo que viaja con el código. Los nombres de código (clases,
    funciones, archivos, macros, targets) se escriben tal cual.
*   **Nombres del proyecto**: **Higgs Mythos** para el producto, **HIGGS** en
    identificadores, **Mythos Studio** para la herramienta de proyectos,
    `higgsaide` para su CLI. Nunca se usa la marca retirada de la base de la que
    deriva el framework ([higgs-mythos.md](../higgs-mythos.md) §"Procedencia").
*   **Formato**: Markdown estándar (GitHub Flavored).
*   **Código**: Usa bloques de código con resaltado de sintaxis (```cpp, ```cmake,
    ```sh, ```powershell, ```json).
*   **Enlaces**: Usa rutas relativas (p. ej. `[Ver tema](tema.md)` dentro de
    `docs/`, o `[docs/tema.md](docs/tema.md)` desde la raíz). Los nombres de
    archivo con espacios se enlazan con `%20`.

## 5. Mantenimiento
Antes de modificar la documentación, lee `README.md` y `docs/README.md` para
entender el contexto actual y el índice vigente. No dupliques información; enlaza
a la fuente de verdad. Mantén el historial de cambios en `changelogs/` según
[Changelogs.md](Changelogs.md).
