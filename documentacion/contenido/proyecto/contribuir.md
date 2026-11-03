# Contribuir

## Solicitudes de funciones y preguntas

Las solicitudes de funciones y las preguntas generales van en la sección
[Discussions](https://github.com/jalexiscv/higgs-mythos/discussions) del repositorio.

## Informes de errores

Abre un *issue* en GitHub con un error **reproducible**, indicando:

- la versión de Higgs Mythos,
- el módulo o módulos afectados,
- la plataforma (sistema operativo, versión y arquitectura),
- una reproducción mínima (idealmente usando código existente, como los ejemplos o el plug-in de
  demostración),
- el comportamiento esperado y, si lo hay, el *stack trace*,
- en plug-ins, los formatos y los hosts (DAW) donde ocurre.

Usa la plantilla de `.github/ISSUE_TEMPLATE` siempre que puedas.

!!! warning "Vulnerabilidades"
    Los problemas de seguridad **no** se informan con un *issue* público: sigue la
    [política de seguridad](seguridad.md).

## Pull requests

- Mantén cada cambio acotado y describe qué corrige o añade.
- Compila los targets afectados y ejecuta las pruebas unitarias antes de abrir el PR (véase
  [UnitTestRunner](../herramientas/unit-test-runner.md)).
- El código, los comentarios y los mensajes de commit del framework se escriben **en inglés**.
- Al enviar un *pull request* aceptas licenciar tu contribución en los mismos términos que el
  proyecto (AGPL-3.0-or-later) y confirmas que escribiste el código o que tienes derecho a
  enviarlo.
- No incluyas código copiado de binarios propietarios ni de fuentes cuya licencia sea incompatible
  con la AGPL.
- Respeta las [convenciones de nombres](../conceptos/convenciones.md) y el estilo del código
  existente; conserva los avisos de copyright de las cabeceras.
- Si añades o actualizas código de terceros, actualiza el manifiesto del SBOM (véase
  [SBOM](../herramientas/otras.md#sbom)).

## Contribuir a esta documentación

La documentación en español vive en `documentacion/` y es una excepción a la regla del inglés.
Véase [Mantener esta documentación](documentacion.md). Cada página tiene un botón de edición que
abre el archivo correspondiente en GitHub.

## Código de conducta

El proyecto se rige por el código de conducta de `CODE_OF_CONDUCT.md`. Al participar en *issues*,
discusiones o *pull requests* aceptas respetarlo.
