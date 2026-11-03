# Integración continua

El repositorio define sus automatizaciones con GitHub Actions en `.github/workflows`. Varias de ellas
solo se activan en repositorios concretos (condiciones `if: github.repository == ...`), de modo que
en un *fork* no se ejecutan.

| Flujo | Disparador | Qué hace |
|---|---|---|
| `higgs_docs_pages.yml` | *Push* a `main` que cambie `documentacion/`, o manual | Construye esta documentación con MkDocs y la publica en GitHub Pages. |
| `higgs_private_build.yml` | Manual (`workflow_dispatch`) o desde los disparadores | Marca el estado `CI` del commit y lanza la compilación y pruebas, compilaciones nocturnas, despliegue, envío del SBOM, documentación y notificaciones mediante flujos reutilizables del repositorio `jalexiscv/HIGGS-utils`. Admite compilar con C++20 o C++23 (`cpp-std`). |
| `higgs_private_push_trigger.yml` | *Push* a `master`, `develop`, `bugfix/**`, `feature/**`, `wayland` | Solo en `jalexiscv/HIGGS-dev`: dispara la compilación de CI mediante la API de GitHub. |
| `higgs_private_nightly_trigger.yml` | Diario a las 03:00 UTC, o manual | Solo en `jalexiscv/HIGGS-dev`: lanza las compilaciones nocturnas configuradas en la variable `NIGHTLY_BUILD_TARGETS` (incluida una con C++20). |
| `higgs_private_github_publish.yml` | Manual, con una etiqueta de versión | Publica una *release* en GitHub mediante `jalexiscv/HIGGS-utils`. |
| `higgs_submit_sbom.yml` | Manual o llamado por otro flujo | Comprueba que `HiggsMythos.spdx.json` está al día (`generate_sbom.py --check`) y lo envía al grafo de dependencias de GitHub. |
| `check-cla.yml` | `pull_request_target` | Solo en `jalexiscv/HIGGS`: comprueba que los autores de un PR han firmado el acuerdo de contribución. |

Los archivos auxiliares `trigger_workflow.py`, `github_api_request.py` y `configure_logger.py`, y
las acciones compuestas de `.github/actions` (`job_wrapper`, `upload_artifact`,
`download_artifacts`), los usan estos flujos.

## Ejecutar las comprobaciones en local

```bash
# Pruebas unitarias
cmake . -B cmake-build -DHIGGS_BUILD_EXTRAS=ON
cmake --build cmake-build --target UnitTestRunner
# ejecuta el binario generado en cmake-build/extras/UnitTestRunner/UnitTestRunner_artefacts/

# Inventario de dependencias
python3 extras/SBOM/generate_sbom.py . --check

# Documentación
pip install -r documentacion/requirements.txt
python3 documentacion/scripts/generar_referencia.py --comprobar
mkdocs build --strict -f documentacion/mkdocs.yml
```
