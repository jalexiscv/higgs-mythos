# README ampliado en español con traducciones a siete idiomas

**Fecha:** 2026-11-03
**Versión:** 9.0.5
**Área:** docs > README
**Tiempo laborado:** 12 horas
**Autor:** Jose Alexis Correa Valencia <jalexiscv@gmail.com>
**Asistencia:** Agente de IA (Claude)

## Descripción

El `README.md` raíz estaba en inglés y sus secciones eran párrafos breves que
no explicaban qué es un módulo, cómo se relacionan entre sí, qué herramientas
acompañan al framework ni por qué rigen los requisitos de sistema. Se
reescribió en español como fuente única, con cada explicación ampliada a un
mínimo de 150 palabras, y se añadieron secciones nuevas: una tabla de los
veinticuatro módulos con su propósito, una sección de herramientas y ejemplos
(`AudioPluginHost`, `UnitTestRunner`, `BinaryBuilder`, `AudioPerformanceTest`,
`NetworkGraphicsDemo`, `SBOM`, PIP y `examples/Plugins`), la descripción del
sitio de documentación y sus disparadores de despliegue, y prosa que explica
las dos listas de requisitos (equipo de desarrollo frente a plataforma de
ejecución). El resto de la información se conserva: versión en
`higgs_StandardHeader.h`, API de CMake, Mythos Studio y sus ejecutables
precompilados, contribución, licencia y firma de plug-ins AAX.

El mismo contenido se ofrece en siete traducciones completas (inglés,
portugués, francés, alemán, italiano, chino simplificado y japonés), cada una
con un selector de idioma en la cabecera que enlaza a las demás. Las
traducciones reproducen la estructura y los datos del original; los
identificadores de código, las rutas y los comandos no se traducen.

Se descartó la mención a una «Projucer Instant Project» como expansión de PIP
porque esa marca se retiró del proyecto; el README describe el PIP por lo que
es (un proyecto completo en un archivo de cabecera con bloque de metadatos),
igual que `documentacion/contenido/compilacion/pip.md`.

### Decisión de diseño

Las traducciones viven en la raíz como `README.<idioma>.md`, la convención que
GitHub y los lectores esperan, en lugar de en `docs/`, que está reservado a la
documentación técnica. El protocolo de documentación se actualiza para
registrar esa excepción y la regla de que todo cambio del `README.md` se
replica en las ocho versiones.

## Tipo de Cambio

- `Cambiado`
- `Agregado`

## Archivos Afectados

### [MODIFICADO] `README.md`
- Reescrito en español con explicaciones de 150 palabras o más por sección,
  secciones nuevas de módulos y de herramientas y ejemplos, y selector de
  idioma.

### [NUEVO] `README.en.md`, `README.pt.md`, `README.fr.md`, `README.de.md`, `README.it.md`, `README.zh-CN.md`, `README.ja.md`
- Traducciones completas del README en español, con el mismo contenido y el
  selector de idioma apuntando a las demás versiones.

### [MODIFICADO] `docs/protocols/Documentations.md`
- La lista de archivos permitidos en la raíz y el árbol de directorios incluyen
  `README.<idioma>.md`; nueva regla «README multilingüe».

### [NUEVO] `changelogs/2026-11-03_readme_multilingue.md`
- Este registro.

### [MODIFICADO] `changelogs/CHANGELOG.md`
- Fila 2.

## Verificación

- Recuento por sección con un script del scratchpad: en los seis archivos en
  alfabeto latino cada sección y subsección de prosa (sin contar tablas,
  listas ni bloques de código) supera las 150 palabras; en chino y japonés
  cada una supera los 300 caracteres. Ninguna sección quedó marcada como
  corta tras la ampliación de «Módulos», «Requisitos mínimos» y sus dos
  subsecciones.
- Enlaces relativos de los ocho archivos comprobados contra el árbol del
  repositorio con el mismo script: cero enlaces rotos.
- Datos contrastados con el código y la documentación: lista de módulos en
  `modules/`, funciones `higgs_*` y opciones en `docs/CMake API.md`,
  exportadores y modos de línea de comandos de Mythos Studio en
  `documentacion/contenido/herramientas/`, nombres de los flujos de GitHub
  Actions y sus disparadores en `.github/workflows/`, plug-ins de ejemplo en
  `examples/Plugins/`, `minimumCppStandard: 17` en `higgs_core.h`.
- Búsqueda de la marca retirada en los ocho archivos: cero resultados.
- No hay cambios de código, por lo que no se compiló ningún objetivo ni se
  ejecutó `UnitTestRunner`.

## Impacto

- Solo documentación: sin efecto en el código, la API ni la compatibilidad
  binaria. `CLAUDE.md` importa `@README.md`, de modo que los agentes de IA
  reciben ahora el README en español y más extenso en cada sesión.
- No hay entradas en `BREAKING_CHANGES.md` ni `CHANGE_LIST.md`.
