# Opciones de configuración

Las opciones de configuración son macros de preprocesador `HIGGS_*` que cada módulo declara en su cabecera principal y que activan, desactivan o ajustan funciones. Todas tienen un valor por defecto; para cambiarlo:

=== "CMake"

    ```cmake
    target_compile_definitions(MiTarget PUBLIC HIGGS_JACK=1 HIGGS_USE_CURL=0)
    ```

    En plug-ins usa visibilidad `PUBLIC` sobre el target de código compartido, para que la definición llegue a todos los *wrappers*.

=== "Mythos Studio"

    Selecciona el módulo en la sección *Modules* del proyecto y cambia la opción en su panel.

!!! warning "Opciones de plug-in que afectan a la compatibilidad"
    Algunas opciones de `higgs_audio_plugin_client` cambian cómo se identifican los parámetros o cómo se guarda el estado. Cambiarlas en un plug-in ya publicado puede romper proyectos guardados por tus usuarios.

En total hay **74 opciones** repartidas en 13 módulos.

## higgs_core

Módulo [`higgs_core`](../modulos/higgs_core.md).

| Opción | Por defecto | Descripción |
|---|---|---|
| `HIGGS_FORCE_DEBUG` | sin definir | Normalmente, `HIGGS_DEBUG` se establece en `1` o `0` según la configuración del compilador y del proyecto, pero si define este valor, puede sobrescribirlo para forzar que sea verdadero o falso. |
| `HIGGS_LOG_ASSERTIONS` | 1 en Android; 0 en el resto | Si este indicador está habilitado, las macros `jassert` y `jassertfalse` siempre usarán `Logger::writeToLog()` para escribir un mensaje cuando se produzca una aserción. Habilitarlo también hará que esto permanezca activado en las compilaciones de release. En cambio, cuando está deshabilitado, las macros `jassert` y `jassertfalse` no se compilarán en una compilación de release. Véase también: `jassert`, `jassertfalse`, `Logger`. |
| `HIGGS_CHECK_MEMORY_LEAKS` | 1 en compilaciones de depuración | Habilita una comprobación de fugas de memoria para ciertos objetos cuando la aplicación finaliza. Consulte la clase `LeakedObjectDetector` y la macro `HIGGS_LEAK_DETECTOR` para obtener más detalles sobre cómo habilitar la comprobación de fugas para clases específicas. |
| `HIGGS_DONT_AUTOLINK_TO_WIN32_LIBRARIES` | `0` | En una compilación para Windows, esta opción puede usarse para evitar que las bibliotecas del sistema necesarias se agreguen automáticamente a la etapa de enlazado. |
| `HIGGS_INCLUDE_ZLIB_CODE` | `1` | Esta opción permite deshabilitar el código de terceros de `zlib` integrado en HIGGS. Es posible que deba ajustarla si enlaza su aplicación con una biblioteca `zlib` externa, pero en aplicaciones normales esta opción debe dejarse como está. Si la deshabilita, quizá también le convenga establecer un valor para `HIGGS_ZLIB_INCLUDE_PATH`, para especificar la ruta donde se encuentran sus encabezados de `zlib`. |
| `HIGGS_USE_CURL` | `1` | Habilita la compatibilidad con http/https mediante `libcurl` (solo Linux). Habilitar esta opción agregará una dependencia dinámica adicional en tiempo de ejecución con `libcurl`. Si la deshabilita, la compatibilidad con https/ssl no estará disponible en Linux. |
| `HIGGS_LOAD_CURL_SYMBOLS_LAZILY` | `0` | Si se habilita, HIGGS cargará `libcurl` de forma diferida cuando sea necesario (por ejemplo, cuando se use `WebInputStream`). Habilitar este indicador también puede ayudar con los errores de dependencias de bibliotecas, ya que enlazar `libcurl` en tiempo de compilación puede indicarle al enlazador que dependa estrictamente de una versión específica de `libcurl`. También resulta útil si desea limitar la cantidad de dependencias de HIGGS y no usa `WebInputStream` ni las clases `URL`. |
| `HIGGS_CATCH_UNHANDLED_EXCEPTIONS` | `0` | Si se habilita, se agregará código de captura de excepciones para reenviar las excepciones no controladas a su callback `HIGGSApplicationBase::unhandledException()`. |
| `HIGGS_ALLOW_STATIC_NULL_VARIABLES` | `0` | Si se deshabilita, se desactivarán variables globales estáticas peligrosas como `String::empty`, `var::null`, etc., que pueden causar graves problemas de orden de inicialización si se hace referencia a ellas durante el código de constructores estáticos. |
| `HIGGS_STRICT_REFCOUNTEDPOINTER` | `0` | Si se habilita, la clase `ReferenceCountedObjectPtr` será más estricta a la hora de permitir que se convierta directamente en un puntero crudo. De forma predeterminada, esta opción está deshabilitada por compatibilidad con código antiguo, pero, si es posible, siempre debería habilitarla para mejorar la seguridad del código. |
| `HIGGS_ENABLE_ALLOCATION_HOOKS` | `0` | Si se habilita, se agregarán funciones globales de asignación de memoria con aserciones integradas, lo que puede ayudar a depurar las asignaciones en las pruebas unitarias. |

## higgs_events

Módulo [`higgs_events`](../modulos/higgs_events.md).

| Opción | Por defecto | Descripción |
|---|---|---|
| `HIGGS_EXECUTE_APP_SUSPEND_ON_BACKGROUND_TASK` | `0` | Ejecutará el método de suspensión de su aplicación en una tarea en segundo plano de iOS, lo que le da tiempo adicional para guardar el estado de su aplicación. |

## higgs_graphics

Módulo [`higgs_graphics`](../modulos/higgs_graphics.md).

| Opción | Por defecto | Descripción |
|---|---|---|
| `HIGGS_USE_COREIMAGE_LOADER` | `1` | En OSX, habilitar este indicador hace que se usen los códecs de CoreImage para cargar archivos PNG/JPEG/GIF. Está habilitado de forma predeterminada, pero quizá le convenga deshabilitarlo si prefiere usar `libpng`, `libjpeg`, etc. |
| `HIGGS_DISABLE_COREGRAPHICS_FONT_SMOOTHING` | `0` | Establecer este indicador desactivará el suavizado de fuentes de CoreGraphics en macOS, que a algunas personas les parece que hace que el texto se vea demasiado "grueso" para su gusto. |
| `HIGGS_INCLUDE_PNGLIB_CODE` | `1` | Esta opción permite deshabilitar el código de terceros de `pnglib` integrado en HIGGS. Es posible que deba ajustarla si enlaza su aplicación con una biblioteca `pnglib` externa, pero en aplicaciones normales esta opción debe dejarse como está. Si la deshabilita, quizá también le convenga establecer un valor para `HIGGS_PNGLIB_INCLUDE_PATH`, para especificar la ruta donde se encuentran sus encabezados de `pnglib`. |
| `HIGGS_INCLUDE_JPEGLIB_CODE` | `1` | Esta opción permite deshabilitar el código de terceros de `jpeglib` integrado en HIGGS. Es posible que deba ajustarla si enlaza su aplicación con una biblioteca `jpeglib` externa, pero en aplicaciones normales esta opción debe dejarse como está. Si la deshabilita, quizá también le convenga establecer un valor para `HIGGS_JPEGLIB_INCLUDE_PATH`, para especificar la ruta donde se encuentran sus encabezados de `jpeglib`. |
| `HIGGS_USE_WEBP` | `1` | Habilita la clase del formato de imagen WebP. Si su aplicación no necesita leer ni escribir imágenes WebP, quizá le convenga deshabilitar esta opción para reducir el tamaño de sus binarios y el tiempo de compilación. |
| `HIGGS_INCLUDE_WEBPLIB_CODE` | `1` | Esta opción permite deshabilitar el código de terceros de `libwebp` integrado en HIGGS. Es posible que deba ajustarla si enlaza su aplicación con una biblioteca `libwebp` externa, pero en aplicaciones normales esta opción debe dejarse como está. Si la deshabilita, quizá también le convenga establecer valores para `HIGGS_WEBPLIB_DECODE_INCLUDE_PATH` y `HIGGS_WEBPLIB_ENCODE_INCLUDE_PATH`, para especificar las rutas donde se encuentran sus encabezados de `libwebp`. Esta opción de configuración solo tiene efecto cuando `HIGGS_USE_WEBP` está habilitado. |

## higgs_gui_basics

Módulo [`higgs_gui_basics`](../modulos/higgs_gui_basics.md).

| Opción | Por defecto | Descripción |
|---|---|---|
| `HIGGS_ENABLE_REPAINT_DEBUGGING` | `0` | Si esta opción está activada, cada área de la pantalla que se vuelva a pintar parpadeará con un color aleatorio, para que pueda ver exactamente qué partes de sus componentes se están dibujando. |
| `HIGGS_USE_XRANDR` | `1` | Habilita la compatibilidad con varios monitores mediante Xrandr (solo Linux). A menos que desee deshabilitarla específicamente, es mejor dejar esta opción activada. Tenga en cuenta que sus usuarios no necesitan tener Xrandr instalado para que su aplicación HIGGS se ejecute, ya que la disponibilidad de Xrandr se consulta en tiempo de ejecución. |
| `HIGGS_USE_XINERAMA` | `1` | Habilita la compatibilidad con varios monitores mediante Xinerama (solo Linux). A menos que desee deshabilitarla específicamente, es mejor dejar esta opción activada. Se usará como alternativa si `HIGGS_USE_XRANDR` no está establecido o si no se encuentra `libxrandr`. Tenga en cuenta que sus usuarios no necesitan tener Xinerama instalado para que su aplicación HIGGS se ejecute, ya que la disponibilidad de Xinerama se consulta en tiempo de ejecución. |
| `HIGGS_USE_XSHM` | `1` | Habilita la memoria compartida de X para un renderizado más rápido en Linux. Es mejor dejar esta opción activada a menos que tenga una buena razón para deshabilitarla. |
| `HIGGS_USE_XRENDER` | `0` | Habilita XRender para permitir ventanas semitransparentes en Linux. |
| `HIGGS_USE_XCURSOR` | `1` | Usa XCursor para permitir cursores ARGB en Linux. Es mejor dejar esta opción activada a menos que tenga una buena razón para deshabilitarla. |
| `HIGGS_USE_XINPUT` | `1` | Usa XInput (v2.2+) para permitir la entrada multitáctil en Linux. Es mejor dejar esta opción activada a menos que tenga una buena razón para deshabilitarla. La disponibilidad de XInput v2.2+ se consulta en tiempo de ejecución, por lo que sus usuarios no necesitan tener XInput instalado para que su aplicación HIGGS se ejecute. |
| `HIGGS_WIN_PER_MONITOR_DPI_AWARE` | `1` | Habilita el reconocimiento de DPI por monitor en Windows 8.1 y posteriores. |

## higgs_gui_extra

Módulo [`higgs_gui_extra`](../modulos/higgs_gui_extra.md).

| Opción | Por defecto | Descripción |
|---|---|---|
| `HIGGS_WEB_BROWSER` | `1` | Esta opción permite deshabilitar la clase `WebBrowserComponent`. Si no usa páginas web incrustadas, desactivarla puede reducir el tamaño de su código. |
| `HIGGS_USE_WIN_WEBVIEW2_WITH_STATIC_LINKING` | `0` | Habilita el uso del navegador WebView2 de Microsoft Edge (Chromium) en Windows. Si usa Mythos Studio, el paquete `Microsoft.Web.WebView2` se agregará a la solución del proyecto al habilitar este indicador. Si compila con CMake, deberá agregar el paquete manualmente mediante el administrador de paquetes NuGet. Usar este indicador requiere enlazar estáticamente con `WebView2LoaderStatic.lib`, que por ahora solo está disponible a través del paquete NuGet y no se encuentra en VCPKG. Además de habilitar esta macro, deberá usar la opción `WebBrowserComponent::Options::Backend::webview2` al crear la instancia de `WebBrowserComponent`. |
| `HIGGS_USE_WIN_WEBVIEW2` | `0` | Habilita el uso del navegador WebView2 de Microsoft Edge (Chromium) en Windows. Si usa Mythos Studio, el paquete `Microsoft.Web.WebView2` se agregará a la solución del proyecto al habilitar este indicador. Si compila con CMake, deberá agregar el paquete manualmente mediante el administrador de paquetes de Visual Studio. Si también está establecida la variante `WITH_STATIC_LINKING` de este indicador, debe enlazar estáticamente con `WebView2LoaderStatic.lib`; de lo contrario, se usará la carga dinámica. Encontrará más información sobre el enlazado dinámico en la documentación de `WebBrowserComponent::Options::WinWebView2::withDLLLocation()`. Además de habilitar esta macro, deberá usar la opción `WebBrowserComponent::Options::Backend::webview2` al crear la instancia de `WebBrowserComponent`. |
| `HIGGS_ENABLE_LIVE_CONSTANT_EDITOR` | 1 en depuración (solo escritorio) | Permite activar la compatibilidad con `HIGGS_ENABLE_LIVE_CONSTANT_EDITOR` (solo en escritorio). De forma predeterminada, estará habilitada en las compilaciones de depuración y deshabilitada en las compilaciones de release. Consulte la documentación de esa macro para obtener más detalles. |

## higgs_opengl

Módulo [`higgs_opengl`](../modulos/higgs_opengl.md).

| Opción | Por defecto | Descripción |
|---|---|---|
| `HIGGS_OPENGL_ES` | `0` | Compila `higgs_opengl` con OpenGL ES en lugar de OpenGL de escritorio. Solo se puede elegir en Linux y BSD, para sistemas cuyos controladores gráficos solo proporcionan OpenGL ES, como muchas placas Arm embebidas. Siempre está habilitado en iOS y Android, y no puede habilitarse en macOS ni en Windows. Compilar con OpenGL ES requiere enlazar con `libGLESv2` en lugar de `libGL`. Con CMake, habilítelo con la opción de destino `NEEDS_OPENGL_ES` en lugar de definir esta macro, ya que esa opción también enlaza las bibliotecas correctas. En Mythos Studio, habilite el indicador del módulo, o agregue `HIGGS_OPENGL_ES=1` a las definiciones de preprocesador de un exportador de Linux cuando el proyecto también tenga como destino otras plataformas. |

## higgs_video

Módulo [`higgs_video`](../modulos/higgs_video.md).

| Opción | Por defecto | Descripción |
|---|---|---|
| `HIGGS_USE_CAMERA` | `0` | Habilita la compatibilidad con cámaras mediante la clase `CameraDevice` (Mac, Windows, iOS, Android). |
| `HIGGS_SYNC_VIDEO_VOLUME_WITH_OS_MEDIA_VOLUME` | `1` | Habilita la sincronización entre el volumen de reproducción de video y el volumen multimedia del sistema operativo. Actualmente solo es compatible con Android. |

## higgs_audio_devices

Módulo [`higgs_audio_devices`](../modulos/higgs_audio_devices.md).

| Opción | Por defecto | Descripción |
|---|---|---|
| `HIGGS_USE_WINRT_MIDI` | `0` | Habilita el uso de la API de Windows Runtime para MIDI, lo que permite conectarse a dispositivos Bluetooth Low Energy en Windows 10 versión 1809 (October 2018 Update) y posteriores. Si habilita este indicador, las versiones anteriores de Windows recurrirán automáticamente a la API MIDI de Win32 habitual. Para compilar necesitará la versión 10.0.14393.0 del Windows Standalone SDK, y es posible que deba agregar la ruta a los encabezados de WinRT. La ruta a los encabezados será similar a `C:\Program Files (x86)\Windows Kits\10\Include\10.0.14393.0\winrt`. |
| `HIGGS_USE_WINDOWS_MIDI_SERVICES` | `0` | Habilita el uso de Windows MIDI Services, lo que a su vez habilita la compatibilidad con MIDI 2.0 en algunas versiones de Windows. Al momento de escribir esto, solo funciona en la compilación Canary del programa Insider de Windows 11, pero con el tiempo también funcionará en la versión final de Windows 10. Para habilitar esta funcionalidad, las versiones de destino y mínima del Windows SDK deben establecerse como mínimo en 10.0.20348.0. Esta funcionalidad depende, en tiempo de compilación, de la herramienta CppWinRT y del paquete NuGet de Windows MIDI Services. Si usa Mythos Studio, esto se configurará automáticamente al habilitar este indicador. Si usa CMake, deberá pasar `NEEDS_WINDOWS_MIDI_SERVICES TRUE` a su llamada a `higgs_add_<target>`. Actualmente, el paquete NuGet de Windows MIDI Services no está alojado en el repositorio oficial. Para que NuGet y/o Visual Studio puedan encontrar el paquete, deberá configurar un repositorio local que apunte a una carpeta local que contenga el paquete. Puede hacerlo desde la interfaz de administración de paquetes de Visual Studio. Dado que esta API se implementa con código generado por la herramienta CppWinRT, solo pueden compilarla los compiladores que entienden el código generado. Lo más probable es que MSVC funcione; algunas versiones de Clang podrían funcionar, pero no está garantizado. Incluso una vez que Windows MIDI Services sea estable y esté habilitado en Windows, el servicio no podrá usarse hasta que se instale el paquete SDK del servicio. Por lo tanto, deberá asegurarse de que el instalador de su software también instale este paquete SDK, o de que informe al usuario cómo instalar la dependencia. Si el SDK no está disponible, HIGGS recurrirá a la API WinRT anterior (si está habilitada) o, en caso contrario, a la API Win32. Estas API no admiten MIDI 2.0. |
| `HIGGS_ASIO` | `0` | Habilita los dispositivos de audio ASIO (solo MS Windows). IMPORTANTE: activar esta opción hará que su programa dependa de archivos fuente del ASIO SDK de Steinberg, por lo que el programa resultante deberá cumplir los términos de licencia de esos archivos. De forma predeterminada, esta opción usará los encabezados ASIO integrados que se distribuyen junto con HIGGS. Consulte la documentación de `HIGGS_ASIO_USE_EXTERNAL_SDK` para saber cómo usar una versión diferente de esos encabezados. |
| `HIGGS_ASIO_USE_EXTERNAL_SDK` | `0` | El comportamiento predeterminado del indicador `HIGGS_ASIO` es incluir un encabezado integrado del ASIO SDK. Si necesita proporcionar su propia versión de este encabezado en lugar de usar la integrada, puede habilitar este indicador. En ese caso, también debe asegurarse de que el archivo `iasiodrv.h` del SDK pueda localizarse mediante las rutas de inclusión de encabezados de su proyecto. La opción solo tiene efecto cuando `HIGGS_ASIO` también está habilitado. |
| `HIGGS_WASAPI` | `1` | Habilita los dispositivos de audio WASAPI (Windows Vista y posteriores). |
| `HIGGS_DIRECTSOUND` | `1` | Habilita el audio DirectSound (solo MS Windows). |
| `HIGGS_ALSA` | `1` | Habilita los dispositivos de audio ALSA (solo Linux). |
| `HIGGS_JACK` | `0` | Habilita los dispositivos de audio JACK. |
| `HIGGS_USE_ANDROID_OBOE` | `1` | Habilita los dispositivos Oboe (solo Android). |
| `HIGGS_USE_OBOE_STABILIZED_CALLBACK` | `0` | Si `HIGGS_USE_ANDROID_OBOE` está habilitado, habilitar esta opción envolverá los flujos de audio de salida en la clase `oboe::StabilizedCallback`. Esta clase intenta mantener la CPU ocupada para evitar que se reduzca su frecuencia en ciertos dispositivos. (Solo Android). |
| `HIGGS_USE_ANDROID_OPENSLES` | 1 si `HIGGS_USE_ANDROID_OBOE` es 0; si no, 0 | Habilita los dispositivos OpenSLES (solo Android). |
| `HIGGS_DISABLE_AUDIO_MIXING_WITH_OTHER_APPS` | `0` | Activar esta opción otorga a su aplicación acceso exclusivo al audio del sistema en las plataformas que lo admiten (actualmente, solo iOS). |

## higgs_audio_formats

Módulo [`higgs_audio_formats`](../modulos/higgs_audio_formats.md).

| Opción | Por defecto | Descripción |
|---|---|---|
| `HIGGS_USE_FLAC` | `1` | Habilita las clases del códec de audio FLAC (disponibles en todas las plataformas). Si su aplicación no necesita leer archivos FLAC, quizá le convenga deshabilitar esta opción para reducir el tamaño de su código base y el tiempo de compilación. |
| `HIGGS_INCLUDE_FLAC_CODE` | `1` | Esta opción permite deshabilitar el código de terceros de `flaclib` integrado en HIGGS. Es posible que deba ajustarla si enlaza su aplicación con una biblioteca `flaclib` externa, pero en aplicaciones normales esta opción debe dejarse como está. Si la deshabilita, quizá también le convenga establecer un valor para `HIGGS_FLAC_INCLUDE_PATH`, para especificar la ruta donde se encuentran sus encabezados de `flaclib`. Esta opción de configuración solo tiene efecto cuando `HIGGS_USE_FLAC` está habilitado. |
| `HIGGS_USE_OGGVORBIS` | `1` | Habilita las clases del códec de audio Ogg-Vorbis (disponibles en todas las plataformas). Si su aplicación no necesita leer archivos Ogg-Vorbis, quizá le convenga deshabilitar esta opción para reducir el tamaño de su código base y el tiempo de compilación. |
| `HIGGS_USE_OPUS` | `1` | Habilita las clases del códec de audio Opus (disponibles en todas las plataformas). Si su aplicación no necesita leer ni escribir archivos Opus, quizá le convenga deshabilitar esta opción para reducir el tamaño de su código base y el tiempo de compilación. |
| `HIGGS_INCLUDE_OPUS_CODE` | `1` | Esta opción permite deshabilitar el código de terceros de `opus`, `opusfile` y `libopusenc` integrado en HIGGS. Es posible que deba ajustarla si enlaza su aplicación con copias externas de estas bibliotecas, pero en aplicaciones normales esta opción debe dejarse como está. La `libogg` integrada se compila cuando esta opción o `HIGGS_INCLUDE_OGGVORBIS_CODE` está habilitada. Si solo una de ellas lo está, puede entrar en conflicto con la `libogg` de la que depende la biblioteca externa. Esta opción de configuración solo tiene efecto cuando `HIGGS_USE_OPUS` está habilitado. |
| `HIGGS_USE_MP3AUDIOFORMAT` | `1` | Habilita la clase `MP3AudioFormat`, basada en software. Si su aplicación no necesita leer archivos MP3, quizá le convenga deshabilitar esta opción para reducir el tamaño de su código base y el tiempo de compilación. |
| `HIGGS_USE_LAME_AUDIO_FORMAT` | `0` | Habilita la clase `LameEncoderAudioFormat`. |
| `HIGGS_USE_WINDOWS_MEDIA_FORMAT` | `1` | Habilita los códecs del Windows Media SDK. |

## higgs_audio_processors_headless

Módulo [`higgs_audio_processors_headless`](../modulos/higgs_audio_processors_headless.md).

| Opción | Por defecto | Descripción |
|---|---|---|
| `HIGGS_PLUGINHOST_VST` | `0` | Habilita las clases de alojamiento de plug-ins de audio VST. Necesitará tener los archivos del VST2 SDK en sus rutas de búsqueda de encabezados. Puede obtener los archivos del VST2 SDK de una versión anterior del VST3 SDK. Véase también: `VSTPluginFormat`, `VST3PluginFormat`, `AudioPluginFormat`, `AudioPluginFormatManager`, `HIGGS_PLUGINHOST_AU`, `HIGGS_PLUGINHOST_VST3`, `HIGGS_PLUGINHOST_LADSPA`. |
| `HIGGS_PLUGINHOST_VST3` | `0` | Habilita las clases de alojamiento de plug-ins de audio VST3. Véase también: `VSTPluginFormat`, `VST3PluginFormat`, `AudioPluginFormat`, `AudioPluginFormatManager`, `HIGGS_PLUGINHOST_VST`, `HIGGS_PLUGINHOST_AU`, `HIGGS_PLUGINHOST_LADSPA`. |
| `HIGGS_PLUGINHOST_AU` | `0` | Habilita las clases de alojamiento de plug-ins AudioUnit. Obviamente, solo está disponible en Mac. Véase también: `AudioUnitPluginFormat`, `AudioPluginFormat`, `AudioPluginFormatManager`, `HIGGS_PLUGINHOST_VST`, `HIGGS_PLUGINHOST_VST3`, `HIGGS_PLUGINHOST_LADSPA`. |
| `HIGGS_PLUGINHOST_LADSPA` | `0` | Habilita las clases de alojamiento de plug-ins LADSPA. Obviamente, solo está disponible en Linux. Véase también: `LADSPAPluginFormat`, `AudioPluginFormat`, `AudioPluginFormatManager`, `HIGGS_PLUGINHOST_VST`, `HIGGS_PLUGINHOST_VST3`, `HIGGS_PLUGINHOST_AU`. |
| `HIGGS_PLUGINHOST_LV2` | `0` | Habilita las clases de alojamiento de plug-ins LV2. |
| `HIGGS_PLUGINHOST_ARA` | `0` | Habilita las clases de alojamiento de la extensión de plug-ins ARA. Deberá descargar el ARA SDK y especificar su ruta, ya sea en Mythos Studio o mediante `higgs_set_ara_sdk_path()` en el archivo de su proyecto CMake. El directorio puede obtenerse clonando recursivamente https://github.com/Celemony/ARA_SDK y haciendo checkout de la etiqueta `releases/2.1.0`. |
| `HIGGS_CUSTOM_VST3_SDK` | `0` | Si se habilita, el VST3 SDK integrado en HIGGS no se agregará al proyecto y, en su lugar, deberá agregar la ruta a su VST3 SDK personalizado a las rutas de búsqueda de encabezados del proyecto. La mayoría de los usuarios no debería necesitar habilitar esta opción y simplemente debería usar la versión del SDK incluida con HIGGS. |

## higgs_audio_plugin_client

Módulo [`higgs_audio_plugin_client`](../modulos/higgs_audio_plugin_client.md).

| Opción | Por defecto | Descripción |
|---|---|---|
| `HIGGS_VST3_CAN_REPLACE_VST2` | `1` | Habilite esta opción si desea que su plug-in VST3 cargue y guarde un estado compatible con VST2. Esto permite que los hosts reemplacen plug-ins VST2 por plug-ins VST3. Si cambia esta opción, su plug-in VST3 será, de forma predeterminada, incompatible con las versiones anteriores. Si ya publicó un VST2 y un VST3 con este indicador establecido en `0`, aún puede habilitar la migración de VST2 a VST3 en hosts más recientes definiendo el símbolo de preprocesador `HIGGS_VST3_COMPATIBLE_CLASSES` e implementando el método `VST3ClientExtensions::getCompatibleParameterIds()`. |
| `HIGGS_FORCE_USE_LEGACY_PARAM_IDS` | `0` | Habilite esta opción si desea obligar a HIGGS a usar un índice de parámetro continuo para identificar un parámetro en un DAW (este era el comportamiento predeterminado en versiones antiguas de HIGGS y siempre es el predeterminado para los plug-ins VST2). El DAW suele usar este índice para guardar los datos de automatización. Cambiar esta configuración puede estropear los proyectos de DAW de los usuarios; consulte `VST3ClientExtensions::getCompatibleParameterIds()` para conocer una forma de resolver este problema en hosts VST3 más recientes. |
| `HIGGS_FORCE_LEGACY_PARAMETER_AUTOMATION_TYPE` | `0` | Habilite esta opción si desea obligar a HIGGS a usar un esquema heredado para identificar los parámetros de los plug-ins como continuos o discretos. Los proyectos de DAW con datos de automatización escritos por un plug-in AudioUnit, VST3 o AAX compilado con HIGGS versión 5.1.1 o anterior pueden cargarse incorrectamente al abrirlos con un plug-in AudioUnit, VST3 o AAX compilado con HIGGS versión 5.2.0 o posterior. |
| `HIGGS_USE_STUDIO_ONE_COMPATIBLE_PARAMETERS` | `1` | Habilite esta opción si desea que HIGGS use identificadores de parámetro compatibles con Studio One, ya que Studio One ignora los identificadores de parámetro negativos. Al habilitar esta opción, HIGGS generará solo identificadores de parámetro positivos. Tenga en cuenta que, si ya publicó un plug-in antes de HIGGS 4.3.0, habilitar esta opción cambiará sus identificadores de parámetro, lo que hará que su plug-in sea incompatible con los datos de automatización antiguos. |
| `HIGGS_AU_WRAPPERS_SAVE_PROGRAM_STATES` | `0` | Habilite esta opción si desea recibir llamadas a `get/setProgramStateInformation`, en lugar de llamadas a `get/setStateInformation`, desde los wrappers de plug-in AU y AUv3. En HIGGS versión 5.4.5 y anteriores este era el comportamiento predeterminado, por lo que, si modificó las implementaciones predeterminadas de `get/setProgramStateInformation` (cuyas implementaciones predeterminadas simplemente llaman a `get/setStateInformation`), es posible que deba habilitar esta opción de configuración para mantener la compatibilidad con versiones anteriores del estado guardado previamente. |
| `HIGGS_STANDALONE_FILTER_WINDOW_USE_KIOSK_MODE` | 1 en iOS y Android; 0 en el resto | Habilite esta opción si desea que la ventana de su plug-in independiente (standalone) use el modo quiosco. De forma predeterminada, el modo quiosco está habilitado en iOS y Android. |
| `HIGGS_IGNORE_VST3_MISMATCHED_PARAMETER_ID_WARNING` | `0` | Habilite esta opción para ignorar una advertencia que se produce al habilitar `HIGGS_VST3_CAN_REPLACE_VST2` sin habilitar `HIGGS_FORCE_USE_LEGACY_PARAM_IDS`. |

## higgs_audio_utils

Módulo [`higgs_audio_utils`](../modulos/higgs_audio_utils.md).

| Opción | Por defecto | Descripción |
|---|---|---|
| `HIGGS_USE_CDREADER` | `0` | Habilita la clase `AudioCDReader` (en las plataformas compatibles). |
| `HIGGS_USE_CDBURNER` | `0` | Habilita la clase `AudioCDBurner` (en las plataformas compatibles). |

## higgs_dsp

Módulo [`higgs_dsp`](../modulos/higgs_dsp.md).

| Opción | Por defecto | Descripción |
|---|---|---|
| `HIGGS_ASSERTION_FIRFILTER` | `1` | Cuando este indicador está habilitado, se generará una aserción durante la ejecución de las configuraciones DEBUG si usa una clase `FIRFilter` para procesar `FIRCoefficients` con un tamaño superior a 128, para indicarle que sería más eficiente usar la clase `Convolution` en su lugar. Está habilitado de forma predeterminada, pero quizá le convenga deshabilitarlo si realmente desea procesar un filtro de ese tipo en el dominio del tiempo. |
| `HIGGS_DSP_USE_INTEL_MKL` | `0` | Si este indicador está establecido, HIGGS usará la MKL de Intel para sus clases de FFT y convolución. Si usa el exportador de Visual Studio de Mythos Studio, también debe establecer la opción `Use MKL Library (oneAPI)` en `Sequential` o `Parallel` en la configuración del exportador. Si no usa el exportador de Visual Studio, la carpeta que contiene el encabezado `mkl_dfti.h` debe estar en sus rutas de búsqueda de encabezados, y debe enlazar con todas las bibliotecas MKL necesarias. |
| `HIGGS_DSP_USE_SHARED_FFTW` | `0` | Si este indicador está establecido, HIGGS buscará las bibliotecas compartidas de `fftw` en tiempo de ejecución y usará la biblioteca para sus clases de FFT y convolución. Si no se encuentra la biblioteca, se usarán las rutinas de FFT alternativas de HIGGS. Esto es especialmente útil en Linux, ya que `fftw` suele venir preinstalada en las distribuciones de Linux más populares. Debe respetar la licencia de FFTW al habilitar esta opción. |
| `HIGGS_DSP_USE_STATIC_FFTW` | `0` | Si este indicador está establecido, HIGGS usará las bibliotecas de `fftw` enlazadas estáticamente para sus clases de FFT y convolución. Debe agregar la carpeta de encabezados/bibliotecas de `fftw` a las rutas de búsqueda adicionales de encabezados/bibliotecas de su proyecto HIGGS. También debe agregar la propia biblioteca `fftw` a las bibliotecas adicionales que se proporcionan a su proyecto HIGGS durante el enlazado. Debe respetar la licencia de FFTW al habilitar esta opción. |
| `HIGGS_DSP_ENABLE_SNAP_TO_ZERO` | `1` | Habilita código en el módulo `dsp` para evitar los números desnormalizados de punto flotante durante el procesamiento de algunos de los filtros del módulo `dsp`. Habilitarlo añadirá una ligera sobrecarga de rendimiento a los filtros y algoritmos del módulo DSP. Si su aplicación de audio ya deshabilita por completo los números desnormalizados (por ejemplo, mediante la clase `ScopedNoDenormals` o el método `FloatVectorOperations::disableDenormalisedNumberSupport`), puede deshabilitar este indicador con seguridad para ahorrar algunos ciclos de CPU en los filtros y algoritmos del módulo DSP. |

## Opciones de CMake

Las opciones del propio sistema de compilación (`HIGGS_BUILD_EXAMPLES`, `HIGGS_BUILD_EXTRAS`, `HIGGS_MODULES_ONLY`…) y los argumentos de `higgs_add_*` (`NEEDS_CURL`, `NEEDS_WEB_BROWSER`, `NEEDS_OPENGL_ES`…) se describen en el [API de CMake](cmake-api.md).
