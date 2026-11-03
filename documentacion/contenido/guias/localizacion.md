# Localización

`higgs_core` incluye un sistema sencillo de traducción de cadenas basado en archivos de
correspondencias.

## Marcar las cadenas traducibles

```cpp
boton.setButtonText (TRANS ("Open file..."));
etiqueta.setText (TRANS ("Sample rate"), higgs::dontSendNotification);

// Cadenas que se traducen más tarde (por ejemplo, en una tabla estática):
static const char* const nombres[] = { NEEDS_TRANS ("Sine"), NEEDS_TRANS ("Square") };
combo.addItem (TRANS (nombres[0]), 1);
```

- `TRANS (texto)` devuelve la traducción con las correspondencias actuales (equivale a
  `higgs::translate (texto)`). Si no hay traducción, devuelve el texto original.
- `NEEDS_TRANS (texto)` no hace nada en tiempo de ejecución: solo marca la cadena para que las
  herramientas de extracción la encuentren.
- Las propias clases del framework usan `TRANS()` en los textos que muestran, así que tu archivo
  de traducción también puede traducir los mensajes internos (botones de diálogos, menús del
  selector de archivos, etc.).

## El archivo de traducción

Es un archivo de texto que empieza con el nombre del idioma y los códigos ISO de los países donde
es apropiado, seguido de líneas `"original" = "traducción"`. Las comillas dentro de las cadenas se
escriben como `\"`, y cualquier línea que no empiece por comillas se ignora (sirve para
comentarios):

```text
language: Español
countries: es mx co ar cl pe

"Open file..." = "Abrir archivo..."
"Sample rate" = "Frecuencia de muestreo"
"Sine" = "Seno"
"Square" = "Cuadrada"
```

Para cargarlo:

```cpp
higgs::LocalisedStrings::setCurrentMappings (new higgs::LocalisedStrings (archivoTraduccion, true));
// el segundo argumento indica si las claves no distinguen mayúsculas
```

`LocalisedStrings::getLanguageName()` y `getCountryCodes()` devuelven los datos de la cabecera, y
`SystemStats::getUserLanguage()` / `getUserRegion()` te ayudan a elegir el archivo adecuado al
arrancar.

## Generar los archivos con Mythos Studio

La herramienta de línea de comandos de [Mythos Studio](../herramientas/mythos-studio.md#linea-de-comandos)
automatiza el proceso:

```bash
# 1. Extrae todas las cadenas TRANS/NEEDS_TRANS de las carpetas indicadas
MythosStudio --trans Source/ > cadenas.txt

# 2. Tras traducir el archivo, genera el mapeo final (opcionalmente fusionando uno existente)
MythosStudio --trans-finish pretraducido.txt traducido.txt existente.txt
```

El resultado se puede cargar directamente con `LocalisedStrings`.
