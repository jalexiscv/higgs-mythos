# Mantener esta documentación

Este sitio se construye con [MkDocs](https://www.mkdocs.org/) y el tema
[Material for MkDocs](https://squidfunk.github.io/mkdocs-material/), y se publica en GitHub Pages
mediante GitHub Actions.

## Estructura

```text
documentacion/
├── mkdocs.yml                    # configuración del sitio y menú de navegación
├── requirements.txt              # versiones fijadas de MkDocs y Material
├── contenido/                    # páginas en Markdown
│   ├── index.md
│   ├── primeros-pasos/  conceptos/  guias/  compilacion/
│   ├── herramientas/  proyecto/
│   ├── modulos/                  # GENERADO por scripts/generar_referencia.py
│   ├── ejemplos/index.md         # GENERADO
│   ├── compilacion/opciones.md   # GENERADO
│   ├── assets/  stylesheets/
├── datos/
│   ├── modulos.json              # resumen en español de cada módulo y guías relacionadas
│   └── traducciones.json         # traducciones de descripciones de clases, opciones y ejemplos
└── scripts/
    └── generar_referencia.py     # genera las páginas de referencia desde el código
```

## Previsualizar en local

```bash
pip install -r documentacion/requirements.txt
mkdocs serve -f documentacion/mkdocs.yml
```

Abre <http://127.0.0.1:8000/higgs-mythos/> en el navegador; la página se recarga sola al guardar
los cambios.

## Páginas generadas

Las fichas de módulos, el catálogo de ejemplos y la tabla de opciones de configuración se generan
leyendo el código del repositorio (bloques de declaración de módulos, comentarios `Config:`, clases
públicas con sus comentarios Doxygen y metadatos de los PIP). Tras cambiar el código o las
traducciones, regenéralas:

```bash
python3 documentacion/scripts/generar_referencia.py
```

Si el código contiene textos nuevos sin traducción, el script usa el original en inglés y avisa al
terminar; añade la traducción en `datos/traducciones.json` y vuelve a ejecutarlo. Con
`--comprobar` no escribe nada y falla si las páginas no están al día, lo que permite validarlo en
CI. **No edites a mano** las páginas generadas: se sobrescriben.

## Publicación

El flujo `.github/workflows/higgs_docs_pages.yml` se ejecuta en cada *push* a `main` que modifique
`documentacion/` (o manualmente desde la pestaña *Actions*). Construye el sitio con
`mkdocs build --strict` —que falla ante enlaces rotos— y lo despliega en GitHub Pages con las
acciones oficiales `upload-pages-artifact` y `deploy-pages`.

Para que funcione, el repositorio debe tener GitHub Pages configurado con **Source: GitHub Actions**
(*Settings → Pages → Build and deployment*). El sitio queda en
<https://jalexiscv.github.io/higgs-mythos/>.

## Estilo

- Español neutro, tuteo y terminología técnica habitual (plug-in, *callback*, búfer, hilo).
- Identificadores de código, macros, rutas y opciones siempre entre comillas invertidas.
- Los ejemplos de código usan el prefijo `higgs::` explícito.
- No menciones marcas anteriores del código: el producto es **Higgs Mythos** y su herramienta de
  proyectos es **Mythos Studio**.
