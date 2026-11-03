# Gráficos 2D, fuentes e imágenes

El módulo `higgs_graphics` proporciona el motor de dibujo vectorial 2D, la gestión de fuentes y
texto, y la carga y escritura de imágenes. Todo el dibujo se hace a través de un objeto
`higgs::Graphics`, que recibes en `Component::paint()` o que puedes crear sobre una `Image`.

## El contexto `Graphics`

```cpp
void paint (higgs::Graphics& g) override
{
    auto area = getLocalBounds().toFloat().reduced (10.0f);

    g.fillAll (higgs::Colours::black);

    // Relleno con degradado
    g.setGradientFill (higgs::ColourGradient (higgs::Colours::purple, area.getTopLeft(),
                                              higgs::Colours::orange, area.getBottomRight(), false));
    g.fillRoundedRectangle (area, 12.0f);

    // Contorno
    g.setColour (higgs::Colours::white.withAlpha (0.6f));
    g.drawRoundedRectangle (area, 12.0f, 2.0f);

    // Texto
    g.setFont (higgs::FontOptions (24.0f, higgs::Font::bold));
    g.drawText ("Higgs Mythos", area, higgs::Justification::centred);
}
```

Operaciones principales: `fillRect`, `fillEllipse`, `drawLine`, `fillPath`, `strokePath`,
`drawImage`, `drawText`, `drawFittedText`, `setOpacity`, `addTransform`, `reduceClipRegion`,
`saveState`/`restoreState` (o `Graphics::ScopedSaveState`).

## Geometría

| Clase | Descripción |
|---|---|
| `Point<T>`, `Line<T>`, `Rectangle<T>` | Primitivas geométricas con operaciones (unión, intersección, recortes `removeFromTop`…) |
| `RectangleList<T>` | Región formada por varios rectángulos |
| `Parallelogram<T>` | Paralelogramo, útil con transformaciones |
| `Path` | Trazado vectorial arbitrario: líneas, curvas cuadráticas y cúbicas, arcos, figuras |
| `PathStrokeType` | Grosor, uniones y terminaciones de un trazo; puede generar trazos discontinuos |
| `AffineTransform` | Traslación, rotación, escala e inclinación |
| `BorderSize<T>`, `Justification`, `RectanglePlacement` | Márgenes y alineación |

```cpp
higgs::Path onda;
onda.startNewSubPath (0.0f, alto * 0.5f);

for (int x = 1; x < ancho; ++x)
    onda.lineTo ((float) x, alto * 0.5f + std::sin ((float) x * 0.05f) * alto * 0.4f);

g.setColour (higgs::Colours::cyan);
g.strokePath (onda, higgs::PathStrokeType (2.0f, higgs::PathStrokeType::curved,
                                                 higgs::PathStrokeType::rounded));
```

## Colores

`Colour` representa un color ARGB de 32 bits (`Colour (0xff336699)`, `Colour::fromRGB`,
`Colour::fromHSV`, `withAlpha`, `brighter`, `darker`, `interpolatedWith`). `Colours` contiene
constantes con nombre (`Colours::red`, `Colours::darkslategrey`…). `ColourGradient` define
degradados lineales o radiales con varios puntos.

## Texto y fuentes

| Clase | Uso |
|---|---|
| `FontOptions` | Descripción de una fuente (familia, tamaño, estilo, características OpenType) para construir un `Font` |
| `Font` | Fuente concreta lista para dibujar |
| `Typeface` | Tipografía cargada (del sistema o desde memoria) |
| `GlyphArrangement` | Disposición de glifos para texto con control fino |
| `AttributedString` + `TextLayout` | Texto con varios estilos, colores y saltos de línea |

```cpp
// Cargar una fuente incrustada en BinaryData
auto tipografia = higgs::Typeface::createSystemTypefaceFor (BinaryData::MiFuente_ttf,
                                                            BinaryData::MiFuente_ttfSize);
g.setFont (higgs::FontOptions (tipografia).withHeight (18.0f));
```

Higgs Mythos usa **HarfBuzz** para la conformación de texto y **SheenBidi** para el texto
bidireccional, lo que permite escribir correctamente árabe, hebreo, scripts índicos, ligaduras y
emoji. Desde la versión 9.0.0 se admiten **fuentes variables** OpenType (véase
`FontVariablesDemo`) y características tipográficas (véase `FontFeaturesDemo`).

## Imágenes

```cpp
// Crear y dibujar sobre una imagen fuera de pantalla
higgs::Image img (higgs::Image::ARGB, 256, 256, true);
{
    higgs::Graphics gi (img);
    gi.setColour (higgs::Colours::green);
    gi.fillEllipse (32.0f, 32.0f, 192.0f, 192.0f);
}

// Cargar desde archivo o memoria
auto foto  = higgs::ImageFileFormat::loadFrom (higgs::File ("/ruta/foto.jpg"));
auto icono = higgs::ImageCache::getFromMemory (BinaryData::icono_png, BinaryData::icono_pngSize);

// Guardar como PNG
higgs::PNGImageFormat png;
if (auto out = higgs::File ("/ruta/salida.png").createOutputStream())
    png.writeImageToStream (img, *out);
```

Formatos soportados: **PNG**, **JPEG**, **GIF** (lectura) y **WebP** (lectura y escritura, desde
9.0.3, con `HIGGS_USE_WEBP`). `ImageCache` mantiene en memoria las imágenes ya cargadas. Para
efectos, `ImageConvolutionKernel`, `DropShadow`, `GlowEffect` y `DropShadowEffect`.

## Gráficos vectoriales (SVG) y `Drawable`

`Drawable` y sus subclases (`DrawablePath`, `DrawableImage`, `DrawableText`, `DrawableComposite`,
`DrawableRectangle`) representan gráficos vectoriales que se pueden dibujar a cualquier escala.

```cpp
auto svg = higgs::Drawable::createFromSVGFile (higgs::File ("/ruta/logo.svg"));
// o desde memoria:
auto svg2 = higgs::Drawable::createFromImageData (BinaryData::logo_svg, BinaryData::logo_svgSize);

svg->drawWithin (g, getLocalBounds().toFloat(), higgs::RectanglePlacement::centred, 1.0f);
```

!!! note "Cambios en 9.0.0"
    La versión 9.0.0 incorporó un nuevo analizador SVG (basado en LunaSVG/PlutoVG), y `Drawable`
    **ya no hereda de `Component`**: para mostrarlo como componente, usa `DrawableComponent` o
    dibújalo en tu `paint()`. Detalles en [Cambios incompatibles 9.x](../proyecto/cambios-incompatibles.md).

## Motores de renderizado

| Plataforma | Renderizador |
|---|---|
| macOS / iOS | CoreGraphics |
| Windows | Direct2D (por defecto) o software |
| Linux, Android | Renderizador por software optimizado (más rápido desde 9.0.0) |
| Cualquiera | OpenGL, con `OpenGLContext` adjunto a un componente (véase [OpenGL](opengl.md)) |

## Demos relacionadas

`GraphicsDemo`, `FontsDemo`, `FontFeaturesDemo`, `FontVariablesDemo`, `LineSpacingDemo`,
`ImagesDemo`, `ComponentTransformsDemo`.
