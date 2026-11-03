# Mascota en ASCII art y banner de cabecera en los README

**Fecha:** 2026-11-03
**Versión:** 9.0.5
**Área:** docs > README
**Tiempo laborado:** 4 horas
**Autor:** Jose Alexis Correa Valencia <jalexiscv@gmail.com>
**Asistencia:** Agente de IA (Claude)

## Descripción

Se incorpora la mascota de Higgs Mythos, un dibujo en ASCII art aportado por
el autor, en la cabecera de `README.md` y de sus siete traducciones, justo
después del selector de idioma y antes de la introducción.

El dibujo recibido tenía un margen izquierdo común de 40 columnas y su
primera línea había perdido la sangría al pegarse; se recortó el margen
común a cero y se centró la primera línea respecto a la segunda, de modo que
el resultado ocupa 39 líneas y 80 columnas como máximo. El contenido del
dibujo no se alteró. Esa versión en texto se guarda en
`documentacion/contenido/assets/higgs_mythos_mascot.txt`.

En una primera iteración el dibujo se insertó como bloque de código `text`.
El autor pidió después que la cabecera se viera como un banner de terminal:
título «HIGGS-MYTHOS» en letras de bloque, la mascota a la izquierda y un
panel de datos a la derecha, en naranja sobre fondo oscuro. Un bloque de
código de Markdown no admite colores ni columnas fiables en GitHub, así que
el banner se generó como imagen SVG
(`documentacion/contenido/assets/higgs_mythos_banner.svg`): la mascota y el
panel son texto monoespaciado con `xml:space="preserve"` en tres tonos de
gris medio sobre fondo transparente (la paleta naranja y el fondo oscuro de
la primera versión se cambiaron a petición del autor). Los grises son fijos
y no dependen del tema del visor: se eligieron tonos medios legibles tanto
sobre el blanco del tema claro de GitHub como sobre el fondo del tema
oscuro, porque una consulta `prefers-color-scheme` dentro de un SVG
incrustado sigue el tema del sistema y no el de GitHub, y daba texto claro
sobre fondo claro en la previsualización.
El título en letras de bloque de la primera versión del banner se retiró a
petición del autor, de modo que el banner mide 1040×520 píxeles y empieza
directamente por la mascota. El panel resume datos verificables
del framework: nombre y licencia (sin número de versión, para que el banner
no quede desactualizado al publicar una nueva), las cinco plataformas, los formatos
de plug-in, los veinticuatro módulos, las herramientas de `extras`, los dos
comandos de compilación y las direcciones del sitio de documentación y del
repositorio. Los ocho README muestran la imagen con un `<img>` a ancho
completo y un pie en el idioma de cada archivo que enlaza la versión en
texto.

### Decisión de diseño

Se descartó el bloque ```` ```ansi ```` con códigos de color porque solo lo
colorea GitHub y ensucia el README en cualquier otro visor (y en el contexto
que `CLAUDE.md` importa). Se descartó también un PNG porque pierde nitidez
al escalar y pesa más que el SVG (24 KB). El SVG se regenera con un script
a partir del archivo de texto de la mascota si el dibujo o el panel cambian.

## Tipo de Cambio

- `Agregado`

## Archivos Afectados

### [NUEVO] `documentacion/contenido/assets/higgs_mythos_banner.svg`
- Banner de cabecera: mascota y panel de datos, sin título.

### [NUEVO] `documentacion/contenido/assets/higgs_mythos_mascot.txt`
- La mascota en texto plano, 39 líneas y 80 columnas.

### [MODIFICADO] `README.md`, `README.en.md`, `README.pt.md`, `README.fr.md`, `README.de.md`, `README.it.md`, `README.zh-CN.md`, `README.ja.md`
- Imagen del banner y pie con enlace a la versión en texto entre el selector
  de idioma y la introducción (el bloque de código de la primera iteración se
  sustituyó por la imagen).

### [NUEVO] `changelogs/2026-11-03_mascota_ascii_readme.md`
- Este registro.

### [MODIFICADO] `changelogs/CHANGELOG.md`
- Fila 3.

## Verificación

- El SVG se previsualizó en Chrome a través de un servidor HTTP local
  temporal, en cada iteración (con título, sin título, en gris y con fondo
  transparente sobre un fondo blanco y otro oscuro): la mascota de 39 líneas
  y el panel se muestran completos, alineados y legibles en 1040×520
  píxeles, sin solapamientos; el servidor y la pestaña se cerraron después.
- Inserción y sustitución hechas con scripts del scratchpad; `git diff
  --stat` muestra el mismo número de líneas tocadas en cada uno de los ocho
  archivos.
- Recuento por sección y comprobación de enlaces con el script usado en el
  registro 2: ninguna sección queda por debajo del mínimo y cero enlaces
  rotos. Los dos archivos nuevos existen en la ruta que los README
  referencian.
- Sin cambios de código: no se compiló ningún objetivo ni se ejecutó
  `UnitTestRunner`.

## Impacto

- Solo documentación. `CLAUDE.md` importa `@README.md`, así que los agentes
  reciben la etiqueta de imagen y el pie en cada sesión; no afecta a la API
  ni a la compilación. El sitio MkDocs copia la carpeta `assets`, por lo que
  el banner queda disponible también para la documentación.
- Sin entradas en `BREAKING_CHANGES.md` ni `CHANGE_LIST.md`.
