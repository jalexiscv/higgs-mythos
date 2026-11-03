# higgs_graphics

El motor de gráficos 2D: contexto `Graphics`, geometría (`Path`, `Rectangle`, `AffineTransform`), colores y degradados, fuentes y conformación de texto (HarfBuzz, SheenBidi), imágenes (PNG, JPEG, GIF, WebP), gráficos vectoriales y SVG (`Drawable`) y efectos.

## Ficha

| Campo | Valor |
|---|---|
| Identificador | `higgs_graphics` |
| Versión | 9.0.5 |
| Nombre declarado | Higgs Mythos graphics classes |
| Descripción declarada | Classes for 2D vector graphics, image loading/saving, font handling, etc. |
| Estándar mínimo de C++ | C++17 |
| Licencia | AGPLv3 |
| Dependencias directas | [`higgs_events`](higgs_events.md) |
| Dependencias indirectas | [`higgs_core`](higgs_core.md) |
| Módulos que dependen de él | [`higgs_gui_basics`](higgs_gui_basics.md), [`higgs_box2d`](higgs_box2d.md) |
| Frameworks de macOS | `Cocoa`, `QuartzCore` |
| Frameworks de iOS | `CoreGraphics`, `CoreImage`, `CoreText`, `QuartzCore` |
| Paquetes pkg-config (Linux) | `freetype2`, `fontconfig` |
| Código fuente | [`modules/higgs_graphics`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_graphics) |

## Uso

```cmake
target_link_libraries(MiTarget PRIVATE higgs::higgs_graphics)
```

```cpp
#include <higgs_graphics/higgs_graphics.h>
```

Guías relacionadas: [Gráficos 2D, fuentes e imágenes](../guias/graficos.md).

## Opciones de configuración

Se cambian con `target_compile_definitions(MiTarget PUBLIC OPCION=valor)` o en la sección *Modules* de Mythos Studio.

| Opción | Por defecto | Descripción |
|---|---|---|
| `HIGGS_USE_COREIMAGE_LOADER` | `1` | En OSX, habilitar este indicador hace que se usen los códecs de CoreImage para cargar archivos PNG/JPEG/GIF. Está habilitado de forma predeterminada, pero quizá le convenga deshabilitarlo si prefiere usar `libpng`, `libjpeg`, etc. |
| `HIGGS_DISABLE_COREGRAPHICS_FONT_SMOOTHING` | `0` | Establecer este indicador desactivará el suavizado de fuentes de CoreGraphics en macOS, que a algunas personas les parece que hace que el texto se vea demasiado "grueso" para su gusto. |
| `HIGGS_INCLUDE_PNGLIB_CODE` | `1` | Esta opción permite deshabilitar el código de terceros de `pnglib` integrado en HIGGS. Es posible que deba ajustarla si enlaza su aplicación con una biblioteca `pnglib` externa, pero en aplicaciones normales esta opción debe dejarse como está. Si la deshabilita, quizá también le convenga establecer un valor para `HIGGS_PNGLIB_INCLUDE_PATH`, para especificar la ruta donde se encuentran sus encabezados de `pnglib`. |
| `HIGGS_INCLUDE_JPEGLIB_CODE` | `1` | Esta opción permite deshabilitar el código de terceros de `jpeglib` integrado en HIGGS. Es posible que deba ajustarla si enlaza su aplicación con una biblioteca `jpeglib` externa, pero en aplicaciones normales esta opción debe dejarse como está. Si la deshabilita, quizá también le convenga establecer un valor para `HIGGS_JPEGLIB_INCLUDE_PATH`, para especificar la ruta donde se encuentran sus encabezados de `jpeglib`. |
| `HIGGS_USE_WEBP` | `1` | Habilita la clase del formato de imagen WebP. Si su aplicación no necesita leer ni escribir imágenes WebP, quizá le convenga deshabilitar esta opción para reducir el tamaño de sus binarios y el tiempo de compilación. |
| `HIGGS_INCLUDE_WEBPLIB_CODE` | `1` | Esta opción permite deshabilitar el código de terceros de `libwebp` integrado en HIGGS. Es posible que deba ajustarla si enlaza su aplicación con una biblioteca `libwebp` externa, pero en aplicaciones normales esta opción debe dejarse como está. Si la deshabilita, quizá también le convenga establecer valores para `HIGGS_WEBPLIB_DECODE_INCLUDE_PATH` y `HIGGS_WEBPLIB_ENCODE_INCLUDE_PATH`, para especificar las rutas donde se encuentran sus encabezados de `libwebp`. Esta opción de configuración solo tiene efecto cuando `HIGGS_USE_WEBP` está habilitado. |

## Código de terceros incluido

| Componente | Licencia | Ubicación |
|---|---|---|
| [HarfBuzz](https://harfbuzz.github.io) | `MIT-Modern-Variant` | `modules/higgs_graphics/fonts/harfbuzz` |
| [SheenBidi](https://github.com/Tehreer/SheenBidi) | `Apache-2.0` | `modules/higgs_graphics/unicode/sheenbidi` |
| [LunaSVG](https://github.com/sammycage/lunasvg) | `MIT` | `modules/higgs_graphics/drawables/lunasvg` |
| [PlutoVG](https://github.com/sammycage/plutovg) | `MIT` | `modules/higgs_graphics/drawables/lunasvg/plutovg` |
| [libpng](http://www.libpng.org/pub/png/libpng.html) | `libpng-2.0` | `modules/higgs_graphics/image_formats/pnglib` |
| [jpeglib](https://ijg.org) | `IJG` | `modules/higgs_graphics/image_formats/jpglib` |
| [libwebp](https://developers.google.com/speed/webp) | `BSD-3-Clause` | `modules/higgs_graphics/image_formats/libwebp` |

## Catálogo de clases

64 clases y estructuras públicas declaradas en las cabeceras que incluye `higgs_graphics.h`, agrupadas por carpeta. La descripción es la primera frase de su comentario de documentación en el código.

### `geometry`

| Clase | Descripción | Cabecera |
|---|---|---|
| `AffineTransform` | Representa una matriz de transformación afín 2D. | [`higgs_AffineTransform.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_graphics/geometry/higgs_AffineTransform.h) |
| `Point` | Par de coordenadas (x, y). | [`higgs_Point.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_graphics/geometry/higgs_Point.h) |
| `Line` | Representa una línea. | [`higgs_Line.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_graphics/geometry/higgs_Line.h) |
| `Rectangle` | Gestiona un rectángulo y permite realizar operaciones geométricas sobre él. | [`higgs_Rectangle.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_graphics/geometry/higgs_Rectangle.h) |
| `Parallelogram` | Representa un paralelogramo definido por 3 puntos. | [`higgs_Parallelogram.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_graphics/geometry/higgs_Parallelogram.h) |
| `Path` | Un trazado es una secuencia de líneas y curvas que puede formar una figura cerrada o quedar abierta. | [`higgs_Path.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_graphics/geometry/higgs_Path.h) |
| `RectangleList` | Mantiene un conjunto de rectángulos como una región compleja. | [`higgs_RectangleList.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_graphics/geometry/higgs_RectangleList.h) |
| `BorderSize` | Especifica un conjunto de márgenes que se dejan alrededor de los lados de un rectángulo. | [`higgs_BorderSize.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_graphics/geometry/higgs_BorderSize.h) |
| `EdgeTable` | Tabla de segmentos de líneas de escaneo horizontales, usada para rasterizar Paths. | [`higgs_EdgeTable.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_graphics/geometry/higgs_EdgeTable.h) |
| `PathFlatteningIterator` | Aplana un objeto Path en una serie de segmentos rectos. | [`higgs_PathIterator.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_graphics/geometry/higgs_PathIterator.h) |
| `PathStrokeType` | Describe un tipo de trazo usado para renderizar un contorno sólido a lo largo de una ruta. | [`higgs_PathStrokeType.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_graphics/geometry/higgs_PathStrokeType.h) |

### `placement`

| Clase | Descripción | Cabecera |
|---|---|---|
| `Justification` | Representa un tipo de justificación que se usa al posicionar elementos gráficos. | [`higgs_Justification.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_graphics/placement/higgs_Justification.h) |
| `RectanglePlacement` | Define el método utilizado para posicionar algún tipo de objeto rectangular dentro de un área de visualización rectangular. | [`higgs_RectanglePlacement.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_graphics/placement/higgs_RectanglePlacement.h) |

### `colour`

| Clase | Descripción | Cabecera |
|---|---|---|
| `PixelARGB` | Representa un píxel INTERNAL de 32 bits con alfa premultiplicado y puede realizar operaciones de composición con él. | [`higgs_PixelFormats.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_graphics/colour/higgs_PixelFormats.h) |
| `PixelRGB` | Representa un píxel RGB de 24 bits y puede realizar operaciones de composición sobre él. | [`higgs_PixelFormats.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_graphics/colour/higgs_PixelFormats.h) |
| `PixelAlpha` | Representa un píxel de 8 bits y un solo canal, y puede realizar operaciones de composición sobre él. | [`higgs_PixelFormats.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_graphics/colour/higgs_PixelFormats.h) |
| `Colour` | Representa un color, que también incluye un valor de transparencia. | [`higgs_Colour.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_graphics/colour/higgs_Colour.h) |
| `ColourGradient` | Describe la disposición y los colores que deben usarse para pintar un degradado de color. | [`higgs_ColourGradient.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_graphics/colour/higgs_ColourGradient.h) |
| `FillType` | Representa un color o patrón de relleno para usar al renderizar rutas. | [`higgs_FillType.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_graphics/colour/higgs_FillType.h) |

### `images`

| Clase | Descripción | Cabecera |
|---|---|---|
| `ImageCache` | Una caché global de imágenes que se han cargado desde archivos o memoria. | [`higgs_ImageCache.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_graphics/images/higgs_ImageCache.h) |
| `ImageConvolutionKernel` | Representa un kernel de filtro para usar en la convolución de una imagen. | [`higgs_ImageConvolutionKernel.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_graphics/images/higgs_ImageConvolutionKernel.h) |
| `ImageFileFormat` | Clase base para códecs que pueden leer y escribir formatos de archivo de imagen como PNG, JPEG, etc. | [`higgs_ImageFileFormat.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_graphics/images/higgs_ImageFileFormat.h) |
| `PNGImageFormat` | Subclase de ImageFileFormat para leer y escribir archivos PNG. | [`higgs_ImageFileFormat.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_graphics/images/higgs_ImageFileFormat.h) |
| `JPEGImageFormat` | Una subclase de ImageFileFormat para leer y escribir archivos JPEG. | [`higgs_ImageFileFormat.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_graphics/images/higgs_ImageFileFormat.h) |
| `GIFImageFormat` | Una subclase de ImageFileFormat para leer archivos GIF. | [`higgs_ImageFileFormat.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_graphics/images/higgs_ImageFileFormat.h) |
| `WebPImageFormat` | Subclase de ImageFileFormat para leer y escribir archivos WebP. | [`higgs_ImageFileFormat.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_graphics/images/higgs_ImageFileFormat.h) |
| `Image` | Contiene un mapa de bits de tamaño fijo. | [`higgs_Image.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_graphics/images/higgs_Image.h) |
| `ImagePixelDataBackupExtensions` | Los métodos de esta interfaz permiten a los clientes de ImagePixelData consultar y controlar el proceso de respaldo automático de la memoria gráfica a la memoria principal, si este mecanismo es pertinente y está soportado. | [`higgs_Image.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_graphics/images/higgs_Image.h) |
| `ImagePixelData` | Clase base para contener datos de imagen de formas específicas de cada implementación. | [`higgs_Image.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_graphics/images/higgs_Image.h) |
| `ImageType` | Esta clase base es para los manejadores que controlan un tipo de formato de manipulación de imágenes. | [`higgs_Image.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_graphics/images/higgs_Image.h) |
| `SoftwareImageType` | Un tipo de almacenamiento de imagen que guarda los píxeles en memoria como un bloque simple de valores. | [`higgs_Image.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_graphics/images/higgs_Image.h) |
| `NativeImageType` | Tipo de almacenamiento de imágenes que guarda los píxeles usando el formato de almacenamiento predeterminado de la plataforma actual. | [`higgs_Image.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_graphics/images/higgs_Image.h) |
| `ScaledImage` | Imagen que se remuestreará antes de dibujarse. | [`higgs_ScaledImage.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_graphics/images/higgs_ScaledImage.h) |

### `fonts`

| Clase | Descripción | Cabecera |
|---|---|---|
| `GlyphArrangementOptions` | Opciones que se pueden usar para modificar la disposición producida por GlyphArrangement::addFittedText. | [`higgs_GlyphArrangementOptions.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_graphics/fonts/higgs_GlyphArrangementOptions.h) |
| `FontFeatureTag` | Representa una única característica de fuente OpenType. | [`higgs_FontFeatures.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_graphics/fonts/higgs_FontFeatures.h) |
| `FontFeatureSetting` | Representa una única configuración de característica de fuente OpenType. | [`higgs_FontFeatures.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_graphics/fonts/higgs_FontFeatures.h) |
| `FontVariableSetting` | Representa una única configuración de fuente variable OpenType. | [`higgs_FontFeatures.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_graphics/fonts/higgs_FontFeatures.h) |
| `ColourLayer` | Una única capa basada en trazados de un glifo de color. | [`higgs_Typeface.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_graphics/fonts/higgs_Typeface.h) |
| `ImageLayer` | Mapa de bits que representa (parte de) un glifo, usado con mayor frecuencia para representar glifos de emoji a color. | [`higgs_Typeface.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_graphics/fonts/higgs_Typeface.h) |
| `GlyphLayer` | Una capa individual que forma parte de una imagen de glifo. | [`higgs_Typeface.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_graphics/fonts/higgs_Typeface.h) |
| `TypefaceMetrics` | Métricas de fuente que usan las convenciones de HIGGS. | [`higgs_Typeface.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_graphics/fonts/higgs_Typeface.h) |
| `Typeface` | Una tipografía (typeface) representa una fuente independiente del tamaño. | [`higgs_Typeface.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_graphics/fonts/higgs_Typeface.h) |
| `FontOptions` | Opciones que describen una fuente en particular. | [`higgs_FontOptions.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_graphics/fonts/higgs_FontOptions.h) |
| `Font` | Representa una fuente concreta, incluidos su tamaño, estilo, etc. | [`higgs_Font.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_graphics/fonts/higgs_Font.h) |
| `AttributedString` | Una cadena de texto con un conjunto de configuraciones de color/fuente asociadas a subrangos del texto. | [`higgs_AttributedString.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_graphics/fonts/higgs_AttributedString.h) |
| `PositionedGlyph` | Glifo de una fuente concreta, con un tamaño, un estilo, un tipo de letra y una posición determinados. | [`higgs_GlyphArrangement.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_graphics/fonts/higgs_GlyphArrangement.h) |
| `GlyphArrangement` | Un conjunto de glifos, cada uno con una posición. | [`higgs_GlyphArrangement.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_graphics/fonts/higgs_GlyphArrangement.h) |
| `TextLayout` | Un fragmento de texto preformateado, que puede contener múltiples fuentes y colores. | [`higgs_TextLayout.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_graphics/fonts/higgs_TextLayout.h) |

### `contexts`

| Clase | Descripción | Cabecera |
|---|---|---|
| `Graphics` | Un contexto gráfico, usado para dibujar un componente o una imagen. | [`higgs_GraphicsContext.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_graphics/contexts/higgs_GraphicsContext.h) |
| `LowLevelGraphicsContext` | Clase de interfaz para objetos de contexto gráfico, usada internamente por la clase Graphics. | [`higgs_LowLevelGraphicsContext.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_graphics/contexts/higgs_LowLevelGraphicsContext.h) |
| `ScopedBlendContext` | Usa RAII para crear una imagen temporal que se mezcla con el contexto padre cuando este objeto sale del ámbito. | [`higgs_ScopedBlendContext.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_graphics/contexts/higgs_ScopedBlendContext.h) |
| `LowLevelGraphicsSoftwareRenderer` | Una implementación de mínimo común denominador de LowLevelGraphicsContext que realiza todo su renderizado en memoria. | [`higgs_LowLevelGraphicsSoftwareRenderer.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_graphics/contexts/higgs_LowLevelGraphicsSoftwareRenderer.h) |

### `effects`

| Clase | Descripción | Cabecera |
|---|---|---|
| `ImageEffectFilter` | Un filtro de efecto gráfico que se puede aplicar a los componentes. | [`higgs_ImageEffectFilter.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_graphics/effects/higgs_ImageEffectFilter.h) |
| `DropShadow` | Define un efecto de sombra paralela. | [`higgs_DropShadowEffect.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_graphics/effects/higgs_DropShadowEffect.h) |
| `DropShadowEffect` | Filtro de efecto que agrega una sombra paralela detrás del contenido de la imagen. | [`higgs_DropShadowEffect.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_graphics/effects/higgs_DropShadowEffect.h) |
| `GlowEffect` | Un efecto de componente que agrega un desenfoque de color alrededor del contenido del componente. | [`higgs_GlowEffect.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_graphics/effects/higgs_GlowEffect.h) |

### `drawables`

| Clase | Descripción | Cabecera |
|---|---|---|
| `Drawable` | La clase base para objetos que pueden dibujarse a sí mismos. | [`higgs_Drawable.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_graphics/drawables/higgs_Drawable.h) |
| `StrokeOptions` | El conjunto de atributos de trazo SVG. | [`higgs_StrokeOptions.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_graphics/drawables/higgs_StrokeOptions.h) |
| `DrawableShape` | Clase base que implementa funcionalidad común para las clases Drawable que consisten en algún tipo de contorno relleno y trazado. | [`higgs_DrawableShape.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_graphics/drawables/higgs_DrawableShape.h) |
| `DrawableComposite` | Un objeto dibujable que actúa como contenedor de un conjunto de otros Drawables. | [`higgs_DrawableComposite.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_graphics/drawables/higgs_DrawableComposite.h) |
| `DrawableImage` | Un objeto dibujable que consiste en una imagen de mapa de bits. | [`higgs_DrawableImage.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_graphics/drawables/higgs_DrawableImage.h) |
| `DrawablePath` | Un objeto dibujable que renderiza una forma rellena o con contorno. | [`higgs_DrawablePath.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_graphics/drawables/higgs_DrawablePath.h) |
| `DrawableRectangle` | Un objeto Drawable que dibuja un rectángulo. | [`higgs_DrawableRectangle.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_graphics/drawables/higgs_DrawableRectangle.h) |
| `DrawableText` | Un objeto dibujable que renderiza una línea de texto. | [`higgs_DrawableText.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_graphics/drawables/higgs_DrawableText.h) |
