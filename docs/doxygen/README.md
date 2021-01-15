# HIGGS's Doxygen documentation

HIGGS's doxygen setup aims to be pretty vanilla Doxygen and can be run like so:

```
doxygen Doxyfile
```

However, `build.py` can be run to generate additional documentation indexes for HIGGS modules:

```
python3 build.py
```

## Styles

`HTML_STYLESHEET` is left empty, meaning that Doxygen will generate its default styles.

HIGGS specific overrides then live in `css/doxygen-higgs-overrides.css`.

Code highlighting is done with [highlight.js](https://highlightjs.org).
