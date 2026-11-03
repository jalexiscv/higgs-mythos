# Cambios incompatibles 9.x

Esta página recoge, traducidos, todos los cambios de la serie 9.x que pueden obligarte a modificar
tu código o que alteran el comportamiento existente. Cada entrada describe el cambio, los
**posibles problemas**, la **solución alternativa** y el **motivo**. El registro completo, con todas
las versiones anteriores, está en
[`BREAKING_CHANGES.md`](https://github.com/jalexiscv/higgs-mythos/blob/main/BREAKING_CHANGES.md)
(en inglés).

!!! tip "Al actualizar"
    Lee las entradas de todas las versiones entre la tuya y la nueva, de la más antigua a la más
    reciente. La versión 9.0.4 no introdujo cambios incompatibles.

## Versión 9.0.5

### `ValueTree::readFromStream()` limita el anidamiento a 256 niveles

`ValueTree::readFromStream()` y las funciones relacionadas `readFromData()` y
`readFromGZIPData()` ahora devuelven un `ValueTree` no válido para los datos en
los que los árboles están anidados a más de 256 niveles de profundidad.
`var::readFromStream()` ahora lee como valores `void` los arreglos anidados a
más de 256 niveles de profundidad.

**Posibles problemas**

Esos datos, que solo pueden haber sido producidos por un árbol o un arreglo que
a su vez estaba anidado a más de 256 niveles de profundidad, ya no se podrán
volver a leer.

**Solución alternativa**

Reestructure el árbol o el arreglo para que tenga menos niveles de anidamiento.

**Motivo**

Ambos lectores se invocan recursivamente una vez por cada nivel de anidamiento,
por lo que un bloque de datos malicioso podría usar un anidamiento profundo para
desbordar la pila y hacer fallar una aplicación. El límite coincide con el que
se aplica a los documentos JSON y XML. Además, los lectores ya no reservan
memoria por adelantado a partir de los recuentos y tamaños leídos de los datos,
algo que antes podía aprovecharse para solicitar una asignación de memoria muy
grande.

### `JSON::parse()` rechaza anidamientos de más de 256 niveles

`JSON::parse()` y `JSON::fromString()` ahora rechazan los documentos en los que
los objetos o arreglos están anidados a más de 256 niveles de profundidad.

**Posibles problemas**

Esos documentos no se podrán analizar. El `Result` que devuelve `JSON::parse()`
informará `"Objects or arrays are nested too deeply"`, y las demás funciones de
análisis devolverán un `var` vacío (`void`).

**Solución alternativa**

Reestructure el documento para que tenga menos niveles de anidamiento.

**Motivo**

El analizador se invoca recursivamente una vez por cada nivel de anidamiento,
por lo que un documento malicioso podría usar un anidamiento profundo para
desbordar la pila y hacer fallar una aplicación. El límite coincide con el de
`XmlDocument` y se encuentra dentro del rango que usan otras bibliotecas JSON,
muy por encima de lo que necesita cualquier documento real.

### `File::isAbsolutePath()` acepta barras diagonales iniciales en Windows

En Windows, `File::isAbsolutePath()` ahora devuelve `true` para las rutas que
comienzan con una barra diagonal (`/`), y `File::getChildFile()` resuelve una
ruta que comienza con un separador, pero que no indica una unidad ni un recurso
compartido, con respecto a la unidad o al recurso compartido del archivo sobre
el que se invoca, en lugar de hacerlo con respecto a la unidad del directorio de
trabajo actual.

**Posibles problemas**

El código que dependía de que `File::isAbsolutePath()` devolviera `false` para
una ruta como `"/foo"` en Windows observará un comportamiento diferente. Por
ejemplo, en Windows `ZipFile` ahora se negará a extraer las entradas cuyos
nombres comiencen con una barra diagonal, tal como ya lo hacía con las entradas
que comenzaban con una barra invertida.

El código que llame a `getChildFile()` con una ruta como `"\\foo.txt"` sobre un
`File` ubicado en una unidad distinta de la del directorio de trabajo actual
ahora recibirá un `File` en la misma unidad que el `File` sobre el que se llamó
a `getChildFile()`.

**Solución alternativa**

Si necesita el comportamiento anterior, resuelva explícitamente esas rutas con
respecto a `File::getCurrentWorkingDirectory()`.

**Motivo**

Una ruta que comienza con un separador pero sin unidad no es relativa, aunque
tampoco está completamente especificada. Antes, pasar una ruta así a
`getChildFile()` provocaba una aserción, y el resultado dependía del directorio
de trabajo actual en lugar del `File` que se usaba como contexto. Las barras
diagonales se aceptan en todas las demás partes de `File` en Windows, por lo que
`isAbsolutePath()` ahora también las acepta.

### Destinos de implementación mínimos: macOS 12.0 e iOS 15.0

Los destinos de implementación (deployment targets) predeterminados de macOS e
iOS que establece Mythos Studio se elevaron a macOS 12.0 e iOS 15.0,
respectivamente.

**Posibles problemas**

En los proyectos que usan el destino de implementación mínimo predeterminado de
Mythos Studio, ese destino de implementación mínimo se elevará.

**Solución alternativa**

Si necesita un destino de implementación mínimo inferior, debe establecerlo en
la configuración de compilación de Xcode de Mythos Studio.

**Motivo**

Xcode 27 ya no admite destinos de implementación inferiores a macOS 12.0 e
iOS 15.0.

### Cuadros de mensaje nativos sin cancelación predeterminada

En Windows, los cuadros de mensaje nativos con tres botones, creados con el
método de construcción (builder) `MessageBoxOptions::withButton()`, ya no tienen
de forma predeterminada un comportamiento de cancelación (por el cual el cuadro
puede descartarse con Alt+F4, Escape, etc.).

En Android, los cuadros de mensaje nativos de dos y tres botones creados con
`MessageBoxOptions::withButton()` ya no tienen de forma predeterminada un
comportamiento de cancelación (por el cual el cuadro puede descartarse tocando
fuera de él).

**Posibles problemas**

Es posible que los cuadros de diálogo que muestran los programas ya no respondan
a eventos que antes los habrían descartado.

**Solución alternativa**

Use `MessageBoxButtonOptions::withRole` para asignar explícitamente el rol
"cancel" al botón que corresponde a la acción de cancelación del cuadro de
diálogo.

**Motivo**

Antes, el comportamiento de cancelación estaba mal especificado, y el código de
resultado de un cuadro cancelado era difícil de predecir. El nuevo rol "cancel"
permite especificar explícitamente el código de resultado de la acción de
cancelación, y permite especificar el comportamiento de cancelación para
cuadros de diálogo con dos o tres botones.

### `XmlDocument` limita el anidamiento y la expansión de entidades

`XmlDocument` ahora rechaza los documentos en los que los elementos están
anidados a más de 256 niveles de profundidad, en los que las referencias a
entidades están anidadas a más de 40 niveles de profundidad, o que generan más
de 8 MB de texto al expandir entidades cuando eso, además, supera 100 veces el
tamaño del documento.

**Posibles problemas**

Esos documentos no se podrán analizar, y `getLastParseError()` informará
`"elements nested too deeply"`, `"entities nested too deeply"` o
`"entity expansion too large"`. Los documentos que contienen entidades que
hacen referencia a sí mismas, que antes hacían fallar al analizador, ahora
fallan con `"recursive entity reference"`.

**Solución alternativa**

Reestructure el documento para que se mantenga dentro de estos límites.

**Motivo**

El analizador se invoca recursivamente una vez por cada nivel de anidamiento y,
antes, expandía las entidades sin límite. Un documento malicioso podría
aprovechar cualquiera de las dos cosas para hacer fallar una aplicación o para
agotar su memoria. Estos límites coinciden con los que usan libxml2 y expat, y
están muy por encima de lo que necesita cualquier documento real.

### `XmlDocument` ya no carga DTD ni entidades externas

`XmlDocument` ya no carga subconjuntos DTD externos ni entidades de parámetro
externas, a menos que se haya proporcionado un `InputSource` con
`setInputSource()`.

**Posibles problemas**

En los documentos creados a partir de un `File` que dependen de definiciones de
entidades contenidas en un archivo independiente, referenciado mediante un
identificador `SYSTEM` en su declaración `DOCTYPE`, esas entidades quedarán sin
expandir, y `getLastParseError()` informará una entidad desconocida. Los
documentos analizados a partir de una cadena ya recibían este tratamiento.

**Solución alternativa**

Para documentos que provienen de una fuente de confianza, proporcione
explícitamente una fuente de entrada:

```cpp
XmlDocument document (file);
document.setInputSource (new FileInputSource (file));
```

De lo contrario, mueva las definiciones de entidades al subconjunto DTD interno
del documento, o sustituya las entidades antes del análisis.

**Motivo**

Un documento podía indicar cualquier archivo como su subconjunto externo, y el
analizador lo leía a través del `FileInputSource` creado para el `File` del
documento, que acepta rutas absolutas y referencias al directorio superior. Un
documento malicioso podía aprovechar esto para leer archivos arbitrarios de la
máquina del usuario, por lo que ahora estas referencias solo se siguen a través
de una fuente de entrada que quien hace la llamada haya decidido proporcionar.

### `ZipFile` restringe enlaces simbólicos y tamaños de entradas

`ZipFile::uncompressTo` y `ZipFile::uncompressEntry` ahora se niegan a crear un
enlace simbólico cuyo destino no permanezca dentro del directorio de destino, y
hacen fallar una entrada que contenga más datos que el tamaño registrado en el
directorio central del archivo comprimido.

**Posibles problemas**

Los archivos comprimidos que contengan enlaces simbólicos a rutas absolutas, a
ubicaciones por encima del directorio de destino o a rutas que contengan `..`
después de un componente de ruta con nombre ya no se extraerán por completo. Lo
mismo se aplica a los archivos comprimidos cuyo directorio central declare un
tamaño de entrada inferior al real.

**Solución alternativa**

Extraiga las entradas afectadas con `createStreamForEntry()` y gestiónelas en el
código de la aplicación.

**Motivo**

Un enlace simbólico creado a partir de un archivo comprimido no confiable podía
apuntar a cualquier lugar de la máquina del usuario, y cualquier cosa que
posteriormente abriera o ejecutara una ruta dentro del árbol extraído podía ser
redirigida fuera de él. Sin una comprobación contra el tamaño registrado, un
archivo comprimido pequeño también podía llenar el disco, y quienes hacían la
llamada no tenían forma de acotar el espacio que usaría la extracción.

### `NativeMessageBox::show` y `showAsync` cambian de nombre

`NativeMessageBox::show` y `NativeMessageBox::showAsync` se renombraron a
`showForOptions` y `showForOptionsAsync`, respectivamente.

**Posibles problemas**

Las aplicaciones que llamen a estas funciones no compilarán.

**Solución alternativa**

Adopte los nuevos nombres de función. Al hacerlo, tenga en cuenta que los
códigos de resultado del cuadro de mensaje nativo coincidirán con los códigos de
resultado especificados en `MessageBoxOptions`. Revise los códigos de resultado
especificados en las opciones que pasa a estas funciones. Luego, actualice el
código que gestiona el código de resultado del cuadro de mensaje para asegurarse
de que se comporte correctamente. En lugar de actualizar el código del gestor,
también puede optar por actualizar el propio objeto de opciones con códigos de
resultado diferentes.

**Motivo**

Antes, los resultados de `NativeMessageBox::show` y
`NativeMessageBox::showAsync` indicaban el índice del botón activado. Esto
difería del comportamiento de otras funciones que mostraban cuadros de mensaje y
ventanas de alerta, como las funciones estáticas de `AlertWindow`. El
comportamiento de todas estas funciones se unificó: ahora siempre devolverán los
códigos de resultado especificados en `MessageBoxOptions`. Los nombres de las
funciones se cambiaron para indicar a los usuarios que deben revisar su código a
medida que adoptan el nuevo comportamiento.

### `withButton()` exige un código de resultado; `createAlertWindow()` eliminado

`MessageBoxOptions::withButton()` ahora requiere un argumento entero de código de
resultado. `AlertWindow::LookAndFeelMethods::createAlertWindow()` se eliminó y se
reemplazó por `createAlertWindowForOptions`, que acepta un único argumento
`MessageBoxOptions`.

**Posibles problemas**

Las aplicaciones que llamen a estas funciones o las sobrescriban no compilarán.

**Solución alternativa**

Adáptese a la nueva firma de `withButton()` pasando un código de resultado
explícito asociado a cada botón.

Adapte las implementaciones de `createAlertWindow()` para que obtengan el texto
de los botones y los códigos de resultado asociados a partir del argumento
`MessageBoxOptions`.

Al migrar código existente, revise con cuidado las funciones de devolución de
llamada (callbacks) que gestionan los códigos de resultado, y asigne códigos de
resultado adecuados a cada botón. La convención anterior para
`AlertWindow::showAsync`, `AlertWindow::showOkCancelBox` y
`AlertWindow::showYesNoCancelBox` era que el código de resultado del botón fuera
igual a `(x + 1) % N`, donde `x` es el índice del botón y `N` es el número total
de botones. La convención anterior para `NativeMessageBox::showAsync` era
devolver directamente el índice del botón. Esta discrepancia fue la principal
motivación de este cambio.

**Motivo**

Este cambio hace explícitos los códigos de resultado de las ventanas de alerta,
lo que a su vez debería facilitar la escritura de funciones de devolución de
llamada que gestionen correctamente cada botón.

### `AlertWindow` se asocia a su componente con `setAssociatedComponent()`

Una `AlertWindow` creada con un componente asociado ahora se asocia a ese
componente mediante `TopLevelWindow::setAssociatedComponent()`. Esto incluye las
ventanas de alerta que muestran `AlertWindow::showAsync()` y las demás funciones
estáticas de `AlertWindow`.

**Posibles problemas**

La ventana de alerta ahora se mantiene por encima de la ventana que contiene el
componente, y se minimiza y se restaura junto con ella. También se mueve con esa
ventana, en los casos en que una ventana puede posicionarse por sí misma. En
macOS adopta el nivel de esa ventana, por lo que puede quedar cubierta por una
ventana siempre visible (always-on-top) que antes habría quedado debajo de ella.

**Solución alternativa**

No pase un componente asociado, o llame a `setAssociatedComponent (nullptr)` en
una `AlertWindow` que construya usted mismo.

**Motivo**

Una alerta debe permanecer con la ventana a la que pertenece. Esta es la única
forma de mantenerla delante de esa ventana en Wayland, donde una ventana no
puede elevarse, y coincide con el comportamiento de las alertas nativas en otras
plataformas.

### `Component::createNewPeer()` ahora recibe `DesktopWindowOptions`

`Component::createNewPeer()` ahora recibe un `DesktopWindowOptions` en lugar de
indicadores de estilo y argumentos de ventana nativa por separado.
`Component::addToDesktop()` ya no es virtual.

**Posibles problemas**

El código que sobrescriba `createNewPeer()` o `addToDesktop()` no compilará.

**Solución alternativa**

Actualice la sobrescritura de `createNewPeer()` para que reciba un
`const DesktopWindowOptions&`, y lea de él los indicadores de estilo y el padre
nativo con `getStyleFlags()` y con el método de acceso correspondiente al tipo
de padre de la plataforma, como `getParentHWND()`. Reemplace una sobrescritura
de `addToDesktop()` por una sobrescritura del nuevo método protegido
`Component::createDesktopWindow()`, al que llaman ambas sobrecargas de
`addToDesktop()`.

**Motivo**

Ahora se puede crear una ventana de escritorio con el componente desde el que se
abrió, de modo que las ventanas temporales, como los menús y las descripciones
emergentes (tooltips), puedan tener como padre la ventana correcta. Describir la
ventana con un `DesktopWindowOptions` requería una segunda sobrecarga de
`addToDesktop()`, y un único punto de personalización con su propio nombre evita
tener dos sobrecargas virtuales que puedan ocultarse o llamarse entre sí.

## Versión 9.0.3

### `isOperatingSystem64Bit()` informa sobre el sistema operativo, no el proceso

`SystemStats::isOperatingSystem64Bit()` ahora indica si el sistema operativo es
de 64 bits, en lugar de si lo es el proceso actual, tal como indica la
documentación. Puede devolver `true` desde una compilación de 32 bits que se
ejecuta en un sistema Linux o Android de 64 bits, o en Windows en ARM.

**Posibles problemas**

El código que usaba esta función para determinar la arquitectura del proceso
actual puede comportarse de forma diferente al compilarse para iOS, para
destinos Linux o Android de 32 bits, o para destinos Windows de 32 bits que se
ejecutan en ARM64.

**Solución alternativa**

Use las macros `HIGGS_64BIT` y `HIGGS_32BIT` para determinar cómo se compiló el
proceso actual.

**Motivo**

La función está documentada como descriptora del sistema operativo, y la
implementación de macOS siempre se comportó así, pero las implementaciones de
iOS, Linux y Android no estaban alineadas con ella, y la implementación de
Windows no detectaba Windows de 64 bits en ARM desde un proceso de 32 bits.

## Versión 9.0.2

### `ThreadPool::addJob`: el invocable debe devolver `void` o `JobStatus`

`ThreadPool::addJob` ya no ofrece sobrecargas `std::function` separadas para los
tipos de retorno `void` y `ThreadPoolJob::JobStatus`. Ahora acepta un único
parámetro de plantilla invocable que debe devolver exactamente `void` o
`ThreadPoolJob::JobStatus`.

**Posibles problemas**

El código que antes pasaba un invocable que devolvía un tipo distinto de `void`
o de `ThreadPoolJob::JobStatus` (por ejemplo, `int` o `bool`) no compilará.
Antes, el valor de retorno se descartaba mediante la conversión a
`std::function<void()>`.

**Solución alternativa**

Modifique el invocable para que devuelva `void`, o para que devuelva un
`ThreadPoolJob::JobStatus` que controle si el trabajo debe volver a ejecutarse.

**Motivo**

Las sobrecargas anteriores eran ambiguas para las lambdas que devolvían
`ThreadPoolJob::JobStatus`, porque `std::function<void()>` puede construirse a
partir de invocables con tipos de retorno distintos de `void`. Restringir los
tipos de retorno aceptados elimina esa ambigüedad y rechaza en tiempo de
compilación los usos indebidos accidentales.

### Se eliminó `AudioDeviceSelectorComponent::getMidiInputSelectorListBox`

Se eliminó la función `AudioDeviceSelectorComponent::getMidiInputSelectorListBox`.

**Posibles problemas**

El código que use la función eliminada no compilará.

**Solución alternativa**

No hay ninguna solución alternativa.

**Motivo**

Antes de este cambio, `MidiInputSelectorListBox` no era accesible al usar
software lector de pantalla. Aunque hubiéramos rediseñado el componente y
conservado la clase base `ListBox`, esta habría encajado mal conceptualmente con
`MidiInputSelector`, y habría complicado innecesariamente la interacción con los
lectores de pantalla. Por eso se eliminó la clase base `ListBox`, y ya no es
posible devolver un puntero a `ListBox` para el nuevo objeto
`MidiInputSelector`.

## Versión 9.0.1

### `OpenGLImageType::create()` respeta las imágenes `Image::SingleChannel`

`OpenGLImageType::create()` ahora respeta una solicitud de imagen
`Image::SingleChannel`. Antes, el formato solicitado se ignoraba, y la imagen
devuelta era siempre una imagen `Image::ARGB` respaldada por un búfer de
fotogramas (framebuffer) de OpenGL.

**Posibles problemas**

`OpenGLImageType::getFrameBufferFrom()` devuelve `nullptr` para las imágenes de
un solo canal, cuando antes devolvía un búfer de fotogramas válido. Esas
imágenes también informan `Image::SingleChannel` en lugar de `Image::ARGB` desde
`Image::getFormat()`.

**Solución alternativa**

Solicite explícitamente una imagen `Image::ARGB` si necesita una imagen
respaldada por un búfer de fotogramas.

**Motivo**

Antes se ignoraba el formato solicitado al crear una imagen, por lo que una
imagen podía informar un formato distinto del que se había solicitado. Esto
causaba problemas de renderizado en los lugares donde se necesitaba una imagen
real de un solo canal, y `OpenGLImageType` era el único tipo de imagen de HIGGS
que no producía un formato real de un solo canal.

### `OpenGLContext::setImageCacheSize()` interpreta su argumento en bytes

`OpenGLContext::setImageCacheSize()` ahora interpreta su argumento como un
número de bytes, tal como está documentado. Antes, el valor se comparaba con un
recuento de píxeles, por lo que la caché efectiva era cuatro veces mayor que la
solicitada. El tamaño predeterminado de la caché se elevó de 8 MB a 32 MB para
que el comportamiento predeterminado no cambie.

**Posibles problemas**

El código que llame a `setImageCacheSize()` con un valor explícito terminará con
una caché de imágenes cuatro veces más pequeña que antes.

**Solución alternativa**

Multiplique por cuatro el valor que pasaba anteriormente.

**Motivo**

El contrato documentado estaba expresado en bytes, pero la implementación
aplicaba el valor como un número de píxeles.

### zlib, libjpeg, libpng y libflac ahora se compilan como C

zlib, libjpeg, libpng y libflac ahora se compilan en modo de lenguaje C, en
lugar de como C++.

**Posibles problemas**

Los símbolos de estas bibliotecas ya no están envueltos en espacios de nombres
de C++. Si HIGGS se enlaza en un binario que incluye o enlaza por separado zlib,
libjpeg, libpng y/o libflac, los símbolos internos de HIGGS pueden entrar en
conflicto con los símbolos de la copia externa de la biblioteca. Esto provocará
infracciones de la ODR (regla de una sola definición) y, posiblemente, errores
del enlazador.

**Solución alternativa**

Si su proyecto ya incluye copias externas de estas bibliotecas, establezca en
`0` las definiciones de preprocesador `HIGGS_INCLUDE_ZLIB_CODE`,
`HIGGS_INCLUDE_JPEGLIB_CODE`, `HIGGS_INCLUDE_PNGLIB_CODE` y/o
`HIGGS_INCLUDE_FLAC_CODE` para deshabilitar las copias incluidas en HIGGS.
También puede establecer las definiciones `HIGGS_ZLIB_INCLUDE_PATH`,
`HIGGS_JPEGLIB_INCLUDE_PATH`, `HIGGS_PNGLIB_INCLUDE_PATH` y/o
`HIGGS_FLAC_INCLUDE_PATH` para indicar rutas de inclusión personalizadas para
los encabezados de las bibliotecas.

**Motivo**

Compilar estas bibliotecas como código C significa que las copias integradas
(vendored) en HIGGS requieren menos cambios invasivos. Históricamente,
compilarlas como C++ requería cambios internos debido, por ejemplo, a las reglas
de comprobación de tipos más estrictas de C++. Compilar como C debería permitir
en el futuro actualizaciones más rápidas y fluidas de las dependencias
integradas. Otra consideración es que el lenguaje C tiene una semántica
diferente de la de C++ en algunos aspectos, por lo que compilar como C tiene más
probabilidades de producir un binario con el comportamiento previsto por los
autores.

### Nueva ubicación del paquete de integraciones nativas de `WebBrowserComponent`

La ubicación del paquete de integraciones nativas de `WebBrowserComponent`
cambió de `modules/higgs_gui_extra/native/javascript` a
`modules/higgs_gui_extra/native/typescript/webview-interop`.

**Posibles problemas**

Las aplicaciones que enlacen directamente a la ubicación original del paquete
dentro del código fuente no podrán cargar la biblioteca.

**Solución alternativa**

Debe actualizarse la ubicación del paquete para que refleje la nueva ubicación
dentro del código fuente. Como alternativa, los proyectos que usan un gestor de
paquetes pueden usar el paquete `@jalexiscv/webview` de npm.

Los proyectos de JavaScript que dependen del archivo `index.js` original pueden
importar directamente `webview-interop/dist/index.js` en su lugar.

**Motivo**

El paquete de integraciones nativas de `WebBrowserComponent` se migró a
TypeScript y se preparó para publicarse como paquete público de npm. Esto
proporciona información de tipos a los proyectos que consumen este paquete,
pero impone nuevos requisitos sobre la estructura de directorios.

## Versión 9.0.0

### `libxi-dev` es ahora una dependencia obligatoria en Linux

`libxi-dev` ahora es una dependencia obligatoria para compilar HIGGS en Linux, a
menos que se establezca la definición de preprocesador `HIGGS_USE_XINPUT=0`.

**Posibles problemas**

Las aplicaciones que no deshabiliten XInput no compilarán a menos que se
satisfaga la dependencia.

**Solución alternativa**

Instale `libxi-dev`.

**Motivo**

Para agregar compatibilidad con la entrada multitáctil en Linux, se requiere la
extensión XInput2. El paquete `libxi-dev` proporciona los encabezados y las
bibliotecas necesarios para esta extensión.

### Entrada multitáctil deshabilitada de forma predeterminada en Windows

La entrada multitáctil ahora está deshabilitada de forma predeterminada en
Windows.

**Posibles problemas**

Las aplicaciones que dependen de la entrada multitáctil en Windows dejarán de
recibir eventos multitáctiles.

**Solución alternativa**

Para habilitar explícitamente la compatibilidad multitáctil en Windows, llame a
`TopLevelWindow::setUsingWindowsMultiTouch (true)` en las aplicaciones de
escritorio, o sobrescriba `PluginEditor::usesWindowsMultiTouch()` en los
plug-ins y devuelva `true`.

**Motivo**

Habilitar la entrada multitáctil en Windows implica que la aplicación no puede
responder a los gestos integrados, como pellizcar para hacer zoom. Es probable
que este sea un comportamiento predeterminado no deseado. La razón es que, si se
llama a `registerTouchWindow()` para un `HWND`, su `DefWindowProc` dejará de
emitir eventos de gestos. Esto significa que no se envían mensajes `WM_GESTURE`
y que no se invocará ninguna función de devolución de llamada como, por ejemplo,
`Component::mouseMagnify()`.

### Se eliminó `Drawable::createFromSVG (const XmlElement&)`

Se eliminó la función `Drawable::createFromSVG (const XmlElement& svgDocument)`.

**Posibles problemas**

El código que llame a la función no compilará.

**Solución alternativa**

Use en su lugar las funciones `createFromSVGFile()` o `createFromSVGString()`.

**Motivo**

Las funciones de análisis de SVG de HIGGS se rediseñaron a fondo y ahora
dependen de la biblioteca lunasvg. Lunasvg realiza su propio análisis de XML y
no es compatible con el tipo `higgs::XmlElement`.

### Nuevos tipos de retorno y de parámetro en `DrawableShape`

Los tipos de retorno de `DrawableShape::getStrokeType()` y
`DrawableShape::getDashLengths()` cambiaron de `const PathStrokeType&` a
`PathStrokeType`, y de `const Array<float>&` a `Span<const float>`,
respectivamente. El tipo del parámetro de
`DrawableShape::setDashLengths (const Array<float>&)` se cambió a
`Span<const float>`.

**Posibles problemas**

Es posible que el código que llame a estas funciones no compile.

**Solución alternativa**

Debería ser sencillo ajustar el código que hace las llamadas para que maneje los
nuevos tipos de retorno y de parámetro.

**Motivo**

Las opciones de trazo se ampliaron y se generalizaron al renderizado de texto.
Los nuevos tipos se adaptan mejor al diseño ajustado.

### `Drawable` ya no hereda de `Component`

La clase `Drawable` ya no hereda de `Component`.

**Posibles problemas**

El código que dependía de que los objetos `Drawable` heredaran de `Component` no
compilará.

**Solución alternativa**

Si es necesario, el código afectado puede usar la nueva clase
`DrawableComponent` para envolver objetos `Drawable` en un `Component`. Hay
muchos ejemplos en el código base de HIGGS en los que ya se realizó esta
transición.

**Motivo**

Este cambio prepara el terreno para trasladar las clases `Drawable`, junto con el
analizador de SVG, al módulo `higgs_graphics`. Esto nos permite evitar una
dependencia de la pesada clase `Component` y del módulo `higgs_gui_basics`, y
hace posible usar las clases `Drawable` y el analizador de SVG en casos de uso
sin interfaz gráfica (headless).

### HIGGS usa EGL en lugar de GLX en Linux

HIGGS ahora usa EGL, en lugar de GLX, para crear un contexto de OpenGL en Linux.

**Posibles problemas**

EGL es una nueva dependencia, y es posible que deba agregarse a su sistema
operativo.

**Solución alternativa**

Instale el paquete `libegl-dev` (Debian) o su equivalente para su sistema
operativo.

**Motivo**

Usar EGL, en lugar de GLX, amplía la gama de hardware en la que podemos
implementar HIGGS.
