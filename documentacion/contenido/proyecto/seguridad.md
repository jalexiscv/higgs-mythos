# Seguridad

## Informar de una vulnerabilidad

**No informes de vulnerabilidades de seguridad mediante *issues*, discusiones o foros públicos de
GitHub.**

Informa de forma privada a través de la página de avisos de seguridad del repositorio:

<https://github.com/jalexiscv/higgs-mythos/security/advisories/new>

Para que el problema se entienda y se resuelva rápido, incluye tanta información como puedas:

- El tipo de problema (por ejemplo, desbordamiento de búfer, uso tras liberación, *path
  traversal*, inyección).
- Las versiones y módulos de Higgs Mythos afectados (por ejemplo `higgs_graphics`,
  `higgs_audio_formats`), indicando si el problema está en un componente de terceros incluido.
- Las plataformas en las que lo observaste o crees que se aplica (macOS, Windows, iOS, Android,
  Linux).
- Las rutas completas de los archivos fuente afectados, si las conoces.
- Instrucciones paso a paso o un ejemplo de código mínimo que lo reproduzca.
- Código de prueba de concepto o de explotación, si lo tienes.
- El impacto que crees que tiene, incluida la forma en que un atacante podría explotarlo en una
  aplicación publicada.

## Versiones con soporte

Las correcciones de seguridad se aplican a la última versión de la rama `main` y, cuando
corresponde, a la serie estable actual.

## Endurecimiento reciente

Las versiones 9.0.1 a 9.0.5 añadieron protecciones frente a entradas malformadas o maliciosas en
`AudioFormat`, `String`, ZIP, XML, JSON, `ValueTree`, OSC, PNG, GIF, WAV y AIFF: límites de
anidamiento de 256 niveles, rechazo de DTD externas en XML, prevención de *path traversal* al
descomprimir y ausencia de reservas de memoria basadas en tamaños leídos de los datos. Los detalles
están en [Cambios incompatibles 9.x](cambios-incompatibles.md).
