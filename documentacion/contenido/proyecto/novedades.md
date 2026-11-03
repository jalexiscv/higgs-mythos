# Novedades por versión

Funciones y cambios destacados de cada versión, traducidos de `CHANGE_LIST.md`. Para el detalle de
cada corrección consulta el historial de Git, y para los cambios que pueden obligarte a modificar
tu código, [Cambios incompatibles 9.x](cambios-incompatibles.md).

## Serie 9.x

### 9.0.5

- Corregidos algunos problemas de diseño en el iPhone Duo.
- Añadido soporte para ventanas hijas nativas.
- Añadidos roles de botón y acciones principales explícitas a `NativeMessageBox`.
- Añadida protección contra entradas ZIP, XML, JSON, `ValueTree` y OSC malformadas.
- Añadida protección contra archivos PNG, GIF, WAV y AIFF malformados.
- HarfBuzz actualizado a 15.0.0.
- Mejorada la compatibilidad con Xcode 27 e iOS 26.
- Corregidos varios problemas de renderizado de Direct2D.
- Corregidos varios cierres inesperados de `JavascriptEngine`.
- Corregidos algunos problemas de escalado al alojar plug-ins VST3.
- Corregidos algunos problemas de dispositivos MIDI WinMM.
- Corregidos algunos problemas de ventanas en X11.
- Actualizadas las versiones por defecto de Android Gradle y Java en Mythos Studio.

### 9.0.4

- Añadida un API para escuchar la desconexión de dispositivos MIDI.
- Corregidos varios problemas de desconexión de dispositivos MIDI en Windows.
- Corregidos varios problemas de `MessageManagerLock`.
- Corregida la colocación de ventanas en Windows cuando cambia la escala de la pantalla.
- Corregidos varios problemas de ventanas y de la bandeja del sistema en Linux.
- Corregidos algunos problemas al alojar plug-ins LV2.
- Corregido el tratamiento de espacios en blanco de `XmlElement`.
- Añadida la opción de CMake `NEEDS_OPENGL_ES` para Linux.
- Añadida protección contra entradas `String` malformadas.

### 9.0.3

- Añadido soporte para leer y escribir archivos Opus.
- Añadido soporte para imágenes WebP.
- Soporte de Windows MIDI Services actualizado al API de vista previa 7 incluido en el sistema.
- Corregidos varios problemas de frecuencia de muestreo y canales en ALSA (Linux).
- Corregidos problemas de colocación y foco de ventanas en Windows.
- Corregida la cancelación de peticiones de red bloqueantes en macOS.
- Corregidos algunos problemas de `OpenGLFrameBuffer`.
- Permitido el escaneo recursivo de plug-ins LV2.

### 9.0.2

- `MP3AudioFormat` activado por defecto.
- Corregida la lectura de archivos MP3 VBR con relleno tras la cabecera ID3v2.
- Corregido el análisis de archivos WAV a los que les falta el byte de relleno final.
- Mejorado el rendimiento del renderizado OpenGL.
- Corregida la selección por defecto de frecuencia de muestreo y tamaño de búfer en CoreAudio.
- Corregida la gestión de dispositivos Multi-Output de CoreAudio.
- Corregidos algunos problemas al alojar plug-ins VST3.
- Corregido un problema de marcas de tiempo de sysex7 en UMP.
- Corregido el enlace débil de frameworks de Mythos Studio en proyectos de Xcode con varios targets.
- Añadidos un `SECURITY.md` y un inventario de software (SBOM).

### 9.0.1

- Añadido un nuevo paquete npm de TypeScript para la integración con WebView.
- Añadida protección contra entradas `AudioFormat` malformadas.
- Corregidas las compilaciones de iOS con Xcode 12.4.
- Corregida la compilación de FreeType en distribuciones Linux antiguas.
- Corregidos iconos ausentes de macOS para los documentos asociados.
- Corregidas interfaces de Linux que dejaban de responder.
- Corregido un problema de compilación de CoreAudio en macOS.

### 9.0.0

- Añadido un nuevo analizador de SVG.
- Añadido soporte para fuentes variables.
- Añadida una nueva implementación de CoreAudio para macOS.
- Mejorado el rendimiento del renderizador por software.
- Añadido soporte de OpenGL ES en Linux.
- Mejorado el soporte multitáctil en Linux y Windows.
- Mejorado el sistema de CMake para entornos sin interfaz gráfica.

## Serie 8.x

### 8.0.15

- Corregido el comportamiento de `loadFromHexString`.
- Corregidos varios problemas de escalado de la interfaz en Windows.
- Oboe actualizado a 1.10.0.
- Mejorada la gestión de texturas de OpenGL.
- Corregido un problema de `DropShadow` con radios muy pequeños.

### 8.0.14

- Reducida la profundidad del árbol de compilación para evitar los límites de Windows.
- Corregidos algunos problemas de MIDI.
- Corregidos algunos problemas de firma de código de Mythos Studio en macOS.
- Corregido un problema en la comprobación de frecuencias de muestreo en iOS.

### 8.0.13

- Mejorado el tiempo de compilación del módulo `higgs_gui_basics`.
- Añadido soporte para Apple Icon Composer.
- Mejorado el rendimiento del pintado de componentes.
- Añadida la nueva demo `ComponentDiagnosticsDemo`.
- Reducido el tamaño en pila de `Component` y `ListenerList`.
- Mejorado el mosaico de imágenes en macOS.
- Versión mínima de Windows elevada a 1607.
- Mejorado el rendimiento de renderizado en Windows.
- Mejorado el comportamiento al redimensionar en Windows.
- Mejorado el soporte de entrada en iOS (táctil, ratón y lápiz).
- Añadido soporte de la papelera FreeDesktop.org en Linux.

### 8.0.12

- Visual Studio 2026 pasa a ser el exportador por defecto de Mythos Studio.
- Corregido un error de compilación en las compras integradas de Android.
- Corregidos algunos nombres de dispositivos MIDI.

### 8.0.11

- Añadida una nueva demo de paquetes universales MIDI 2.0 (UMP).
- Añadido un exportador de Visual Studio 2026.
- Mejorado el soporte de macOS/iOS 26.
- SDK de VST3 actualizado a 3.8.0 (licencia MIT).
- SDK de AAX actualizado a 2.9.0.
- Incluidos directamente los archivos fuente del SDK de ASIO (licencia GPLv3).
- SheenBidi actualizado a 2.9.0.
- Mejorado el rendimiento de renderizado con una caché de fuentes y glifos.
- Añadido el nuevo módulo `higgs_audio_processors_headless`.

### 8.0.10

- Evitado el reinicio de las *Activities* de Android al cambiar el tema del sistema.
- Corregido un problema de visibilidad de elementos de `PopupMenu`.
- Corregida la detección del tamaño de pantalla de iOS en plug-ins.
- Corregidas algunas advertencias del compilador LLVM 21.
- Corregidos algunos problemas de dibujo de Direct2D.
- Corregido un problema de `XEmbedComponent`.

### 8.0.9

- Añadido soporte para características tipográficas configurables (ligaduras, *kerning*…).
- Varias mejoras en las ventanas de Android.
- Varias mejoras en la conformación de texto.
- Corregido el borrado de búferes de audio al iniciar/detener dispositivos WASAPI.
- Corregido el alojamiento AU de plug-ins con parámetros mal implementados.
- Añadido soporte de macOS/iOS 26.
- Añadido soporte del ciclo de vida UIScene en iOS 13+.
- Añadido soporte para archivos WAV de enteros de 32 bits.
- Varias mejoras de WebView en Linux.
- Corregido un problema con plug-ins AAX de efectos MIDI en cualquier disposición de canales.
- La navegación de accesibilidad se activa ahora por defecto para los componentes desactivados.

### 8.0.8

- Mejorado el comportamiento de diseño de `TextEditor`.
- Añadidas nuevas opciones para controlar el interlineado del texto.
- Más correcciones y mejoras de rendimiento de Direct2D.
- Corregido un problema de tamaño de búfer en el simulador de iOS.
- La demo de MIDI Capability Inquiry pasa al DemoRunner.
- Actualizada la versión por defecto de la cadena de herramientas de Android.

### 8.0.7

- Mejorados el tratamiento de Unicode y el rendimiento de `TextEditor`.
- Corregida la gestión de la frecuencia de muestreo de dispositivos externos en iOS 18.
- Muchas correcciones y mejoras de rendimiento de Direct2D.
- Añadido `MessageManager::callSync`, complemento de `callAsync`.
- Corregido un cierre inesperado en Ableton al cerrar la ventana de un plug-in.
- Mejorada la compatibilidad con sccache.
- Corregidos algunos errores de `PopupMenu`.
- zlib actualizado.

### 8.0.6

- Un error de la cadena de herramientas de Visual Studio pasa a ser una advertencia.

### 8.0.5

- Añadido soporte para Windows Arm.
- Añadido soporte para notificaciones locales.
- Añadidas opciones de compilador de paso directo a `higgsaide`.
- Añadido soporte para migraciones de parámetros VST3.
- Corregidos algunos eventos de ratón y el arrastre de ventanas en Windows.
- Añadida funcionalidad de rangos (*Ranges*).
- Añadidos nombres de notas MIDI para VST2 y VST3.

### 8.0.4

- Simplificada la creación de *singletons*.
- Corregidos algunos problemas de interoperabilidad entre JavaScript y C++.
- Añadido el paso exacto de las marcas de tiempo de los MIDI CC.
- Las propiedades MIDI de los plug-ins se obtienen ahora en tiempo de ejecución.
- Mejorado el soporte de CMake para Windows Arm.
- Mejorado `ShapedText`.
- Corregidos algunos problemas con las compilaciones como DLL de Windows.
- Añadidas marcas de tiempo del sistema a `VBlankAttachment` y a las animaciones.
- Corregidas algunas advertencias de obsolescencia en iOS.
- Actualizada la versión incluida de CHOC.
- Actualizada la versión incluida de Oboe.
- La implementación de JavaScript pasa a un módulo propio.

### 8.0.3

- SDK de AAX actualizado a 2.8.0.
- Corregidos varios problemas de dibujo de Direct2D.
- Corregida la selección de tamaño de búfer y frecuencia de muestreo en iOS 18.

### 8.0.2

- Corregidos algunos problemas con imágenes grandes en Direct2D.
- Activadas las esquinas redondeadas de ventanas en Windows 11.
- Corregidas algunas advertencias del compilador en Xcode 16.
- Mejorado el rendimiento de renderizado de la interfaz en macOS y Android.
- Añadido soporte para C++20 y C++23.
- Corregido un problema de respuesta del ratón en Windows.
- SDK de VST3 actualizado a 3.7.12.

### 8.0.1

- Corregidos algunos problemas de disposición de texto.
- Eliminado el código fuente de plataformas no soportadas.
- Corregidos algunos problemas de Direct2D.
- Actualizada la versión incluida de HarfBuzz.
- Añadidos más formatos envolventes.

### 8.0.0

- Añadido un nuevo renderizador Direct2D.
- Añadido soporte para interfaces basadas en WebView.
- Soporte de Unicode coherente en todas las plataformas.
- Añadido un nuevo módulo de animación.
- Incluido el SDK de AAX.

## Versiones anteriores

El historial de la serie 7.x y anteriores está en
[`CHANGE_LIST.md`](https://github.com/jalexiscv/higgs-mythos/blob/main/CHANGE_LIST.md) (en inglés).
