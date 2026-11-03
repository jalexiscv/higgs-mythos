# Mascota en ASCII art en la cabecera de los README

**Fecha:** 2026-11-03
**Versión:** 9.0.5
**Área:** docs > README
**Tiempo laborado:** 1 hora
**Autor:** Jose Alexis Correa Valencia <jalexiscv@gmail.com>
**Asistencia:** Agente de IA (Claude)

## Descripción

Se incorpora la mascota de Higgs Mythos, un dibujo en ASCII art aportado por
el autor, en la cabecera de `README.md` y de sus siete traducciones, justo
después del selector de idioma y antes de la introducción. El dibujo se
coloca en un bloque de código `text` para que GitHub lo muestre en fuente
monoespaciada sin interpretar sus caracteres, y lleva debajo un pie en el
idioma de cada archivo («La mascota de Higgs Mythos.» y sus equivalentes).

El dibujo recibido tenía un margen izquierdo común de 40 columnas y su
primera línea había perdido la sangría al pegarse; se recortó el margen
común a cero y se centró la primera línea respecto a la segunda, de modo que
el resultado ocupa 39 líneas y 80 columnas como máximo. El contenido del
dibujo no se alteró.

## Tipo de Cambio

- `Agregado`

## Archivos Afectados

### [MODIFICADO] `README.md`, `README.en.md`, `README.pt.md`, `README.fr.md`, `README.de.md`, `README.it.md`, `README.zh-CN.md`, `README.ja.md`
- Bloque con la mascota y su pie entre el selector de idioma y la
  introducción (44 líneas añadidas en cada archivo, idénticas salvo el pie).

### [NUEVO] `changelogs/2026-11-03_mascota_ascii_readme.md`
- Este registro.

### [MODIFICADO] `changelogs/CHANGELOG.md`
- Fila 3.

## Verificación

- Inserción hecha con un script del scratchpad que localiza el selector de
  idioma y el primer salto en blanco posterior; `git diff --stat` muestra
  exactamente 44 líneas añadidas en cada uno de los ocho archivos y ninguna
  eliminada.
- Recuento por sección y comprobación de enlaces con el mismo script usado
  en el registro 2: ninguna sección queda por debajo del mínimo y cero
  enlaces rotos (el bloque de código no cuenta como prosa).
- Ancho máximo del dibujo comprobado en 80 columnas, para que no haya
  desplazamiento horizontal en la vista habitual de GitHub.
- Sin cambios de código: no se compiló ningún objetivo ni se ejecutó
  `UnitTestRunner`.

## Impacto

- Solo documentación. `CLAUDE.md` importa `@README.md`, así que los agentes
  reciben el dibujo en cada sesión; no afecta a la API ni a la compilación.
- Sin entradas en `BREAKING_CHANGES.md` ni `CHANGE_LIST.md`.
