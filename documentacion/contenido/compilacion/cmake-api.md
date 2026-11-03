# API de CMake

Referencia completa de las funciones, opciones y targets que Higgs Mythos añade a CMake. El
original en inglés está en `docs/CMake API.md`.

## Requisitos

- Todos los tipos de proyecto requieren **CMake 3.22** o superior.
- Actualmente **no se admiten targets de Android** con CMake (usa
  [Mythos Studio](../herramientas/mythos-studio.md)).

La mayoría de gestores de paquetes del sistema tienen CMake, pero se recomienda usar la versión
más reciente de <https://cmake.org/download>. Usa siempre un CMake más nuevo que tu cadena de
compilación, para que sepa identificar las herramientas y cómo invocarlas. Además de CMake
necesitarás la cadena de compilación de tu plataforma, como Xcode o MSVC.

## Primeros pasos

### Con `add_subdirectory`

La forma más sencilla de incluir Higgs Mythos es añadirlo como subdirectorio del proyecto e
incluir la línea `add_subdirectory(HIGGS)` en el `CMakeLists.txt`. Así quedan disponibles los
targets y funciones auxiliares de Higgs Mythos para tus propios targets.

### Con `find_package`

Para instalar Higgs Mythos en el sistema hay que indicar a CMake dónde colocar los archivos:

```bash
# Ir al directorio de Higgs Mythos
cd /ruta/al/clon/HIGGS
# Configurar solo los componentes de biblioteca
cmake -B cmake-build-install -DCMAKE_INSTALL_PREFIX=/ruta/a/HIGGS/install
# Ejecutar la instalación
cmake --build cmake-build-install --target install
```

En el proyecto que consume Higgs Mythos, asegúrate de que el `CMakeLists.txt` contiene la línea
`find_package(HIGGS CONFIG REQUIRED)`. Esto pone a disposición del resto de la compilación los
módulos y las funciones auxiliares. Después compila así:

```bash
# Ir al directorio del proyecto
cd /ruta/a/mi/proyecto
# Configurar pasando la ruta de instalación usada antes
cmake -B cmake-build -DCMAKE_PREFIX_PATH=/ruta/a/HIGGS/install
# Compilar
cmake --build cmake-build
```

### Proyectos de ejemplo

En `examples/CMake` hay proyectos de ejemplo de aplicación gráfica, aplicación de consola y
plug-in de audio. Copia una de esas carpetas fuera del repositorio, añade Higgs Mythos como
submódulo y descomenta la llamada a `add_subdirectory` donde se indica en el `CMakeLists.txt`. Si
instalaste Higgs Mythos con un gestor de paquetes o con el target `install`, descomenta en su lugar
la llamada a `find_package`.

Con el proyecto preparado, genera el árbol de compilación de la forma habitual, por ejemplo desde
el directorio del proyecto:

```bash
cmake -Bbuild (-GnombreDelGenerador) (-DHIGGS_BUILD_EXTRAS=ON) (-DHIGGS_BUILD_EXAMPLES=ON)
```

Esto crea un árbol de compilación en `build` con el `CMakeLists.txt` del directorio actual y el
generador por defecto (Makefiles en Mac/Linux y el Visual Studio más reciente en Windows). Puedes
elegir otro generador con `-G` (`cmake -G` muestra la lista de tu plataforma). Si incluiste
Higgs Mythos como subdirectorio, los dos últimos argumentos activan los targets de *extras* y de
ejemplos (desactivados por defecto). Hay muchos ejemplos y la generación tarda más con estas
opciones, así que normalmente no conviene activarlas.

Para compilar:

```bash
cmake --build build (--target nombreDelTarget) (--config Release/Debug/...)
```

Esto compila el target indicado en la configuración indicada con la herramienta adecuada. Si
generaste Makefiles o archivos de Ninja también puedes ejecutar `make` o `ninja` en el directorio
de compilación; si generaste un proyecto de IDE (Xcode, Visual Studio), ábrelo en el IDE.

### Compilar para iOS

Se recomienda mucho usar el generador de Xcode: otros generadores pueden no encontrar
automáticamente el SDK correcto del simulador de iPhone y fallar en pasos como la compilación de
iconos o el procesado del *plist*. Por defecto CMake compila para el mismo sistema en el que se
configuró el proyecto; para compilar de forma cruzada para iOS hay que pasar algunos parámetros
extra en la primera invocación:

```bash
cmake -Bbuild-ios -GXcode -DCMAKE_SYSTEM_NAME=iOS -DCMAKE_OSX_DEPLOYMENT_TARGET=12.0
```

`-DCMAKE_SYSTEM_NAME=iOS` activa la compilación cruzada para iOS y
`-DCMAKE_OSX_DEPLOYMENT_TARGET=12.0` fija el objetivo mínimo de despliegue (se aplica a iOS a
pesar del «OSX» del nombre).

Después puedes abrir el proyecto en Xcode (el archivo está en el directorio de compilación) o
compilar desde la línea de comandos:

```bash
cmake --build build-ios --target <nombreDelTarget> -- -sdk iphonesimulator
```

Todo lo que va tras `--` se pasa a la herramienta subyacente, en este caso `xcodebuild`. Compilar
para el simulador no requiere firma de código. Para un dispositivo real hay que pasar los datos de
firma en la configuración inicial:

```bash
cmake -Bbuild-ios -GXcode -DCMAKE_SYSTEM_NAME=iOS -DCMAKE_OSX_DEPLOYMENT_TARGET=12.0 \
    -DCMAKE_XCODE_ATTRIBUTE_CODE_SIGN_IDENTITY="iPhone Developer" \
    -DCMAKE_XCODE_ATTRIBUTE_DEVELOPMENT_TEAM=<id de 10 caracteres>
```

`CODE_SIGN_IDENTITY` es el tipo de certificado («iPhone Developer» para desarrollo) y
`DEVELOPMENT_TEAM` el identificador de 10 caracteres que aparece en el campo *Organizational
Unit* de tu certificado de desarrollo en la aplicación Acceso a Llaveros. Al compilar quizá
necesites permitir que Xcode actualice los perfiles de aprovisionamiento:

```bash
cmake --build build-ios --target <nombreDelTarget> -- -allowProvisioningUpdates
```

#### Archivar para iOS

El archivado de CMake no siempre funciona como se espera, sobre todo con targets que dependen de
bibliotecas estáticas propias: Xcode puede generarlas en `DerivedData` y luego omitir ese
directorio de las rutas de búsqueda. Si *Product → Archive* falla por bibliotecas estáticas
ausentes, establece `ARCHIVE_OUTPUT_DIRECTORY` explícitamente:

```cmake
set_target_properties(mi_biblioteca_estatica PROPERTIES ARCHIVE_OUTPUT_DIRECTORY "./")
```

La biblioteca estática que produce `higgs_add_binary_data` ya establece esta propiedad.

### Binarios universales para macOS

Para generar binarios que funcionen en arm64 y x86_64, configura con
`"-DCMAKE_OSX_ARCHITECTURES=arm64;x86_64"`.

### Compilar con Clang en Windows

Clang-cl (Clang con línea de comandos tipo MSVC) debería funcionar sin más. Si generas un proyecto
de Visual Studio y tienes instalado el paquete LLVM que se distribuye con Visual Studio, configura
con `-T ClangCL`.

Para usar Clang con línea de comandos tipo GNU, pasa `-DCMAKE_CXX_COMPILER=clang++` y
`-DCMAKE_C_COMPILER=clang` (deben estar en el `PATH`; solo las versiones recientes de CMake lo
admiten en Windows). En esta configuración CMake no parece enlazar automáticamente una biblioteca
de *runtime*; puedes solucionarlo con la propiedad `MSVC_RUNTIME_LIBRARY` (véase la
[documentación oficial](https://cmake.org/cmake/help/v3.22/prop_tgt/MSVC_RUNTIME_LIBRARY.html)).

### Nota sobre las definiciones de compilación

Las opciones de módulo y de plug-in que en Mythos Studio se configuran en la interfaz se pueden
establecer target a target con `target_compile_definitions`. Para encontrar las opciones de un
módulo, busca en su cabecera bloques con esta estructura:

```cpp
/** Config: NOMBRE_DE_LA_CLAVE
    Documentación...
*/
#ifndef NOMBRE_DE_LA_CLAVE
 #define NOMBRE_DE_LA_CLAVE ...
#endif
```

Para cambiar el valor por defecto:

```cmake
target_compile_definitions(mi_target PUBLIC NOMBRE_DE_LA_CLAVE=<valor>)
```

La lista completa, traducida, está en [Opciones de configuración](opciones.md).

La definición `HiggsPlugin_PreferredChannelConfigurations` es difícil de expresar de forma
portable por sus llaves, que pueden malinterpretarse en compilaciones Linux/Mac con Ninja o
Makefiles. Se recomienda evitarla y usar el API de buses para declarar entradas y salidas.

## Referencia

### Opciones

Estas opciones activan o desactivan partes de la compilación de Higgs Mythos. Normalmente se
configuran:

- pasando `-DNOMBRE_DE_LA_OPCION=ON/OFF` en la configuración inicial de CMake, o
- llamando a `set(NOMBRE_DE_LA_OPCION ON/OFF)` antes de incluir Higgs Mythos con
  `add_subdirectory` o `find_package`.

#### `HIGGS_BUILD_EXTRAS`

Controla si se añaden targets para los proyectos de `extras`, como Mythos Studio y
AudioPluginHost. Desactivada por defecto, porque normalmente no los necesitas si has incluido
Higgs Mythos en tu propio proyecto.

#### `HIGGS_BUILD_EXAMPLES`

Controla si se añaden targets para los proyectos de `examples`, como DemoRunner y los PIP.
Desactivada por defecto por la misma razón.

#### `HIGGS_ENABLE_MODULE_SOURCE_GROUPS`

Hace que los archivos fuente de los módulos se puedan explorar en los proyectos de IDE; no tiene
efecto en proyectos que no son de IDE. Desactivada por defecto porque aumenta el tamaño de los
proyectos generados y puede ralentizar la configuración. Si la activas, añade también
`set_property(GLOBAL PROPERTY USE_FOLDERS YES)` en tu `CMakeLists.txt` principal, necesario para
que funcione la agrupación.

La agrupación es sensible a la estructura del proyecto: asegúrate de que la llamada a
`higgs_add_module` que añade un módulo ocurre *antes* de llamar a `higgs_add_*` para crear los
targets que dependen de él. Los módulos aparecen en un grupo «HIGGS Modules» dentro del grupo de
cada target, junto a «Source Files» y «Header Files».

!!! warning
    Los grupos de fuentes solo funcionan cuando todos los targets que dependen de Higgs Mythos se
    crean con las funciones `higgs_add_*`. Con `add_executable` y `add_library` normales es
    probable que la compilación se rompa al activar esta opción.

#### `HIGGS_COPY_PLUGIN_AFTER_BUILD`

Controla si los targets de plug-in se instalan en el sistema tras compilarse. Las carpetas de
plug-ins pueden estar protegidas, así que puede que necesites permisos elevados o ajustar los
permisos de las carpetas de destino.

#### `HIGGS_MODULES_ONLY`

Solo añade los targets de los módulos incluidos y las funciones `higgs_add_module*`. Está pensada
para casos muy personalizados que no necesitan `higgs_add_gui_app` ni `higgs_add_plugin`. Sobre
todo, con esta opción **no se compila la herramienta auxiliar `higgsaide`**, lo que puede reducir
los tiempos de compilación de productos que gestionan por otros medios la estructura de los
bundles, iconos, *plists*, etc. Con esta opción, `HIGGS_ENABLE_MODULE_SOURCE_GROUPS` no tiene
efecto.

#### `HIGGS_WEBVIEW2_PACKAGE_LOCATION`

Puedes pedir que la biblioteca WebView2 se enlace estáticamente en Windows indicando la opción
`NEEDS_WEBVIEW2` al crear el target. En ese caso se buscará el paquete WebView2 en el sistema,
por defecto en `%userprofile%\AppData\Local\PackageManagement\NuGet\Packages`. Esta opción
cambia esa ubicación; la ruta indicada debe contener el directorio `*Microsoft.Web.WebView2*`.

#### `HIGGS_GLOBAL_AAX_SDK_PATH`, `HIGGS_GLOBAL_VST2_SDK_PATH`, `HIGGS_GLOBAL_ARA_SDK_PATH`

Solo en el `CMakeLists.txt` raíz del repositorio: si se definen, se llama a
`higgs_set_aax_sdk_path`, `higgs_set_vst2_sdk_path` o `higgs_set_ara_sdk_path` con esa ruta. Son
útiles para compilar los ejemplos y *extras* con esos SDK.

### Funciones

#### `higgs_add_<target>`

```cmake
higgs_add_gui_app(<target> [KEY value]...)
higgs_add_console_app(<target> [KEY value]...)
higgs_add_plugin(<target> [KEY value]...)
```

`higgs_add_gui_app` y `higgs_add_console_app` añaden un target ejecutable con el nombre
`<target>`. `higgs_add_plugin` añade un target de «código compartido» (biblioteca estática) con el
nombre `<target>` y un target adicional por cada formato de plug-in indicado. Las tres aceptan
argumentos opcionales con la forma `CLAVE` seguida de uno o más `valor`es, que establecen
atributos adicionales del target; si no se indican, se usan valores por defecto razonables.

Cada argumento añade al target una propiedad `HIGGS_nombreDelParametro`. Por ejemplo, tras
`higgs_add_gui_app(mi_target PRODUCT_NAME "Target")`, el target `mi_target` tiene una propiedad
`HIGGS_PRODUCT_NAME` con el valor `"Target"`. Las propiedades `HIGGS_` se pueden *consultar*
después, pero cambiar su valor puede no tener efecto (o romper cosas de formas inesperadas): pasa
siempre los atributos directamente a estas funciones de creación.

##### Identidad y versión

| Argumento | Descripción |
|---|---|
| `PRODUCT_NAME` | Nombre del binario generado, similar a la propiedad `OUTPUT_NAME` de CMake. Por defecto, el nombre del target. |
| `VERSION` | Versión con formato `mayor.menor.revisión`. Por defecto, la `VERSION` del proyecto. En Apple es la versión visible (`CFBundleShortVersionString`). |
| `BUILD_VERSION` | Versión `mayor.menor.revisión`; por defecto igual a `VERSION`. En Apple es la versión privada que distingue compilaciones de la App Store (`CFBundleVersion`). |
| `BUNDLE_ID` | Identificador con la forma `com.tuempresa.producto` que identifica el target de forma única (sobre todo en macOS). Por defecto se genera a partir de `COMPANY_NAME` y el nombre del target. |
| `COMPANY_NAME` | Autor del target. Se añade al `Info.plist` y puede formar parte del `BUNDLE_ID`. Se hereda de la propiedad `HIGGS_COMPANY_NAME`. |
| `COMPANY_COPYRIGHT` | Texto de copyright para el `Info.plist`. Se hereda de `HIGGS_COMPANY_COPYRIGHT`. |
| `COMPANY_WEBSITE` | Sitio web relacionado con el target. Se hereda de `HIGGS_COMPANY_WEBSITE`. |
| `COMPANY_EMAIL` | Correo del autor. Se hereda de `HIGGS_COMPANY_EMAIL`. |
| `DOCUMENT_EXTENSIONS` | Extensiones de archivo asociadas al target; por ejemplo Mythos Studio pasa `mythos` para abrir archivos `.mythos`. Admite varias: `DOCUMENT_EXTENSIONS wav mp3 aif`. |
| `PLIST_TO_MERGE` | Texto que se inserta en el `Info.plist` de la aplicación o plug-in. |

Para usar los mismos datos de empresa en varios targets, llama a
`set_directory_properties(PROPERTIES HIGGS_COMPANY_NAME ...)` (o `HIGGS_COMPANY_COPYRIGHT`,
`HIGGS_COMPANY_WEBSITE`, `HIGGS_COMPANY_EMAIL`) tras incluir Higgs Mythos y antes de añadir los
targets, y omite el argumento en cada uno.

##### Iconos

| Argumento | Descripción |
|---|---|
| `ICON_BIG`, `ICON_SMALL` | Rutas a imágenes con las que se generan los iconos. Si solo se indica una, se usa para todas las resoluciones; si se indican ambas, se elige la adecuada en cada resolución. |
| `ICON_COMPOSER_BUNDLE` | Bundle de Icon Composer para macOS e iOS. Con Xcode 26 o posterior tiene prioridad sobre `ICON_BIG`/`ICON_SMALL`; las versiones anteriores siguen usando estos últimos. |
| `CUSTOM_XCASSETS_FOLDER` | Carpeta `xcassets` con iconos y/o imágenes de arranque. Si se indica, `ICON_BIG` e `ICON_SMALL` no tienen efecto en iOS. Las *LaunchImages* están obsoletas desde iOS 13, pero si la carpeta contiene una y no se indica *storyboard* propio, se usa. |
| `LAUNCH_STORYBOARD_FILE` | *Storyboard* de arranque propio para iOS. Si no se indica se usa uno por defecto. Tiene prioridad sobre una *LaunchImage* de `CUSTOM_XCASSETS_FOLDER`. |

##### Permisos y capacidades (Apple)

| Argumento | Descripción |
|---|---|
| `MICROPHONE_PERMISSION_ENABLED` / `MICROPHONE_PERMISSION_TEXT` | `TRUE`/`FALSE`. Añade `NSMicrophoneUsageDescription` al `Info.plist`; el texto es el mensaje que verá el usuario. |
| `CAMERA_PERMISSION_ENABLED` / `CAMERA_PERMISSION_TEXT` | Añade `NSCameraUsageDescription`. |
| `BLUETOOTH_PERMISSION_ENABLED` / `BLUETOOTH_PERMISSION_TEXT` | Añade `NSBluetoothAlwaysUsageDescription`. |
| `LOCAL_NETWORK_PERMISSION_ENABLED` / `LOCAL_NETWORK_PERMISSION_TEXT` | Añade `NSLocalNetworkUsageDescription`. |
| `SEND_APPLE_EVENTS_PERMISSION_ENABLED` / `SEND_APPLE_EVENTS_PERMISSION_TEXT` | Permite que la aplicación envíe Apple Events. |
| `FILE_SHARING_ENABLED` | `TRUE`/`FALSE`. Entradas para compartir archivos en el `Info.plist` de iOS. |
| `DOCUMENT_BROWSER_ENABLED` | `TRUE`/`FALSE`. Entradas de navegador de documentos (iOS). |
| `STATUS_BAR_HIDDEN` | `TRUE`/`FALSE`. Oculta la barra de estado (iOS). |
| `REQUIRES_FULL_SCREEN` | `TRUE`/`FALSE`. Requiere pantalla completa (iOS). |
| `BACKGROUND_AUDIO_ENABLED` | `TRUE`/`FALSE`. Audio en segundo plano (iOS). |
| `BACKGROUND_BLE_ENABLED` | `TRUE`/`FALSE`. Bluetooth LE en segundo plano (iOS). |
| `APP_GROUPS_ENABLED` / `APP_GROUP_IDS` | Activa los grupos de aplicaciones de iOS y lista los grupos a los que pertenece la aplicación (se añaden a los *entitlements*). |
| `ICLOUD_PERMISSIONS_ENABLED` | `TRUE`/`FALSE`. Entradas de iCloud en los *entitlements* (iOS). |
| `PUSH_NOTIFICATIONS_ENABLED` | `TRUE`/`FALSE` (por defecto `FALSE`). *Entitlements* para notificaciones push. |
| `NETWORK_MULTICAST_ENABLED` | `TRUE`/`FALSE` (por defecto `FALSE`). Permite multidifusión o difusión IP en macOS/iOS. |
| `IPHONE_SCREEN_ORIENTATIONS`, `IPAD_SCREEN_ORIENTATIONS` | Una o varias de `UIInterfaceOrientationUnknown`, `UIInterfaceOrientationPortrait`, `UIInterfaceOrientationPortraitUpsideDown`, `UIInterfaceOrientationLandscapeLeft`, `UIInterfaceOrientationLandscapeRight`. Por defecto `UIInterfaceOrientationPortrait UIInterfaceOrientationLandscapeLeft UIInterfaceOrientationLandscapeRight`. |
| `TARGETED_DEVICE_FAMILY` | `1` (iPhone/iPod touch), `2` (iPad) o `1,2` (ambos, valor por defecto). |

##### *Hardened runtime* y *sandbox* (macOS)

| Argumento | Descripción |
|---|---|
| `HARDENED_RUNTIME_ENABLED` | `TRUE`/`FALSE` (por defecto `FALSE`). Activa el *hardened runtime* de macOS, necesario para la notarización. |
| `HARDENED_RUNTIME_OPTIONS` | Claves de *entitlements* separadas por espacios (forma `com.apple.security.*`) que se añaden si el *hardened runtime* está activado. |
| `APP_SANDBOX_ENABLED` | `TRUE`/`FALSE` (por defecto `FALSE`). Activa el *sandbox* de aplicaciones de macOS. |
| `APP_SANDBOX_INHERIT` | Permite que los procesos hijos hereden los *entitlements* estáticos del padre. Si es `TRUE`, no se establece ningún otro *entitlement* de *sandbox*. Por defecto `FALSE`. |
| `APP_SANDBOX_OPTIONS` | Claves de *entitlements* (`com.apple.security.*`) separadas por espacios que se añaden si el *sandbox* está activado. |
| `APP_SANDBOX_FILE_ACCESS_HOME_RO` / `_HOME_RW` | Rutas relativas a la carpeta personal con acceso de solo lectura / lectura y escritura. |
| `APP_SANDBOX_FILE_ACCESS_ABS_RO` / `_ABS_RW` | Rutas absolutas con acceso de solo lectura / lectura y escritura. |
| `APP_SANDBOX_EXCEPTION_IOKIT` | Subclases de `IOUserClient` que se pueden abrir o modificar (excepción temporal de IOKit). |

##### Dependencias opcionales del sistema

| Argumento | Descripción |
|---|---|
| `NEEDS_CURL` | En Linux, enlaza Curl. Ponlo a `TRUE` si ves errores de enlace o de inclusión relacionados con Curl. |
| `NEEDS_WEB_BROWSER` | En Linux, enlaza WebKit. Ponlo a `TRUE` si ves errores relacionados con WebKit. |
| `NEEDS_OPENGL_ES` | En Linux, compila `higgs_opengl` contra OpenGL ES en lugar de OpenGL de escritorio: define `HIGGS_OPENGL_ES=1` y enlaza los paquetes pkg-config `egl` y `glesv2`. Útil en sistemas cuyos controladores solo ofrecen OpenGL ES (muchas placas Arm embebidas). No tiene efecto en otras plataformas: iOS y Android usan siempre OpenGL ES, y macOS y Windows nunca. |
| `NEEDS_WEBVIEW2` | En Windows, enlaza WebView2. Ponlo a `TRUE` si ves errores relacionados con WebView2. |
| `NEEDS_STORE_KIT` | En macOS, enlaza StoreKit (compras integradas). |
| `NEEDS_WINDOWS_MIDI_SERVICES` | En Windows, usa Windows MIDI Services para MIDI 2.0. Requiere paquetes adicionales en compilación y ejecución; si faltan, CMake explica cómo instalarlos. Define automáticamente `HIGGS_USE_WINDOWS_MIDI_SERVICES=1`. |

##### Argumentos de plug-in

| Argumento | Descripción |
|---|---|
| `FORMATS` | Formatos que se compilan, separados por espacios: `Standalone Unity VST3 AU AUv3 AAX VST LV2`. `AU` y `AUv3` solo se activan en macOS, y `AUv3` solo con el generador de Xcode. Es un error pasar `VST` sin haber llamado antes a `higgs_set_vst2_sdk_path`. |
| `PLUGIN_NAME` | Nombre que ve el usuario en el DAW. Puede diferir del nombre del archivo (que fija `PRODUCT_NAME`). Por defecto, igual a `PRODUCT_NAME`. |
| `PLUGIN_MANUFACTURER_CODE` | Identificador de cuatro caracteres de tu empresa. Para AU debe contener al menos una mayúscula; GarageBand 10.3 exige la primera en mayúscula y el resto en minúscula. Por defecto `Manu`. |
| `PLUGIN_CODE` | Identificador de cuatro caracteres del plug-in. Para AU debe contener exactamente una mayúscula; GarageBand 10.3 exige la primera en mayúscula y el resto en minúscula. Por defecto, un código aleatorio que cambia en cada configuración. |
| `DESCRIPTION` | Descripción breve del plug-in. |
| `IS_SYNTH` | `TRUE`/`FALSE` (por defecto `FALSE`). Si es un sintetizador; se usa para elegir categorías razonables si no se indican. |
| `NEEDS_MIDI_INPUT` | `TRUE`/`FALSE` (por defecto `FALSE`). Si el plug-in tiene entrada MIDI. |
| `NEEDS_MIDI_OUTPUT` | `TRUE`/`FALSE` (por defecto `FALSE`). Si el plug-in tiene salida MIDI. |
| `IS_MIDI_EFFECT` | `TRUE`/`FALSE` (por defecto `FALSE`). Si es un efecto MIDI (algunos hosts tienen un lugar especial para ellos). |
| `EDITOR_WANTS_KEYBOARD_FOCUS` | `TRUE`/`FALSE` (por defecto `FALSE`). Si el editor necesita el foco del teclado o deja todo el teclado al host. |
| `DISABLE_AAX_BYPASS` | `TRUE`/`FALSE` (por defecto `FALSE`). Desactiva la función de bypass de AAX. |
| `DISABLE_AAX_MULTI_MONO` | `TRUE`/`FALSE` (por defecto `FALSE`). Desactiva la disposición de buses *multi-mono* de AAX. |
| `AAX_IDENTIFIER` | ID de bundle del target AAX. Por defecto, `BUNDLE_ID`. |
| `LV2URI` | Identificador único del plug-in LV2. **Debe cambiar** si haces cambios incompatibles (quitar o reordenar parámetros, cambiar el formato de presets…): los hosts LV2 consideran intercambiables los plug-ins con la misma URI. Por defecto se genera a partir de `COMPANY_WEBSITE` y `PLUGIN_NAME`; tendrás que indicarlo si el nombre contiene caracteres no válidos en una URI (como espacios), si `COMPANY_WEBSITE` no incluye el esquema (`http://`) o si no hay sitio web y quieres un identificador `urn:`. |
| `LV2_PLUGIN_CLASS` | Nombre de una de las clases de plug-in de <https://lv2plug.in/ns/lv2core#Plugin>. Por defecto `InstrumentPlugin` para sintetizadores y `Plugin` para el resto. |
| `VST_NUM_MIDI_INS` / `VST_NUM_MIDI_OUTS` | Número de entradas/salidas MIDI de los VST2 y VST3 que usan MIDI. Por defecto `16`. |
| `VST2_CATEGORY` | Una de `kPlugCategUnknown`, `kPlugCategEffect`, `kPlugCategSynth`, `kPlugCategAnalysis`, `kPlugCategMastering`, `kPlugCategSpacializer`, `kPlugCategRoomFx`, `kPlugSurroundFx`, `kPlugCategRestoration`, `kPlugCategOfflineProcess`, `kPlugCategShell`, `kPlugCategGenerator`. Por defecto `kPlugCategSynth` si `IS_SYNTH` es `TRUE` y `kPlugCategEffect` en otro caso. |
| `VST3_CATEGORIES` | Una o varias, separadas por espacios: `Fx`, `Instrument`, `Analyzer`, `Delay`, `Distortion`, `Drum`, `Dynamics`, `EQ`, `External`, `Filter`, `Generator`, `Mastering`, `Modulation`, `Mono`, `Network`, `NoOfflineProcess`, `OnlyOfflineProcess`, `OnlyRT`, `Pitch Shift`, `Restoration`, `Reverb`, `Sampler`, `Spatial`, `Stereo`, `Surround`, `Synth`, `Tools`, `Up-Downmix`. Por defecto `Instrument Synth` si `IS_SYNTH` es `TRUE` y `Fx` en otro caso. |
| `AU_MAIN_TYPE` | Uno de `kAudioUnitType_Effect`, `kAudioUnitType_FormatConverter`, `kAudioUnitType_Generator`, `kAudioUnitType_MIDIProcessor`, `kAudioUnitType_Mixer`, `kAudioUnitType_MusicDevice`, `kAudioUnitType_MusicEffect`, `kAudioUnitType_OfflineEffect`, `kAudioUnitType_Output`, `kAudioUnitType_Panner`. |
| `AU_EXPORT_PREFIX` | Prefijo de los nombres de las funciones de entrada que expone el componente; normalmente una versión del nombre del plug-in válida como identificador C++. Por defecto, el nombre del plug-in con el sufijo `AU`. |
| `AU_SANDBOX_SAFE` | `TRUE`/`FALSE` (por defecto `FALSE`). Añade las entradas correspondientes al `Info.plist` del AU. Si es `TRUE`, `SUPPRESS_AU_PLIST_RESOURCE_USAGE` no tiene efecto. |
| `SUPPRESS_AU_PLIST_RESOURCE_USAGE` | `TRUE`/`FALSE` (por defecto `FALSE`). Elimina la clave `resourceUsage` del *plist*. Útil para AU que deben acceder a recursos que no se pueden declarar ahí, como sockets de dominio UNIX; en particular, los AU protegidos con PACE pueden necesitarlo para cargar en GarageBand. Sin efecto si `AU_SANDBOX_SAFE` es `TRUE`. |
| `AAX_CATEGORY` | Una o varias de `None`, `EQ`, `Dynamics`, `PitchShift`, `Reverb`, `Delay`, `Modulation`, `Harmonic`, `NoiseReduction`, `Dither`, `SoundField`, `HWGenerators`, `SWGenerators`, `WrappedPlugin`, `Effect` y `MIDIEffect` (se admite el prefijo `AAX_ePlugInCategory_`). Por defecto `MIDIEffect` si `IS_MIDI_EFFECT` es `TRUE`, `SWGenerators` si `IS_SYNTH` es `TRUE` y `None` en otro caso. |
| `PLUGINHOST_AU` | `TRUE`/`FALSE` (por defecto `FALSE`). Si es `TRUE`, define `HIGGS_PLUGINHOST_AU=1` y enlaza los frameworks de macOS necesarios para alojar plug-ins AU. Es preferible a definir `HIGGS_PLUGINHOST_AU` a mano con `target_compile_definitions`. |
| `USE_LEGACY_COMPATIBILITY_PLUGIN_CODE` | `TRUE`/`FALSE` (por defecto `FALSE`). Si es `TRUE`, `HiggsPlugin_ManufacturerCode` toma el valor hexadecimal de `proj`. Existe por compatibilidad con una versión antigua y defectuosa del soporte de CMake que gestionaba mal el código de fabricante; la mayoría de proyectos deben dejar el valor por defecto. |
| `COPY_PLUGIN_AFTER_BUILD` | `TRUE`/`FALSE` (por defecto `FALSE`). Instala el plug-in en el sistema tras compilarlo. Para todos los plug-ins de un subdirectorio, establece la propiedad `HIGGS_COPY_PLUGIN_AFTER_BUILD` en el directorio antes de añadirlos. En Windows, las ubicaciones por defecto pueden no ser escribibles por usuarios normales. |
| `VST_COPY_DIR`, `VST3_COPY_DIR`, `AAX_COPY_DIR`, `AU_COPY_DIR`, `UNITY_COPY_DIR` | Destino de la copia tras compilar de cada formato. Para cambiarlo en todo un subdirectorio, establece la propiedad `HIGGS_<FORMATO>_COPY_DIR` en el directorio antes de añadir los targets. Las ubicaciones por defecto se listan en la [guía de plug-ins](../guias/plugins.md#instalacion-y-prueba); si usas el formato Unity con copia automática, conviene indicar `UNITY_COPY_DIR` explícitamente. |
| `IS_ARA_EFFECT` | `TRUE`/`FALSE` (por defecto `FALSE`). Activa en los *wrappers* de VST3 y AU las rutas de código que permiten a los hosts compatibles cargar el plug-in con funciones ARA, y define `HiggsPlugin_Enable_ARA=1` (no la añadas a mano). |
| `ARA_FACTORY_ID` | Identificador único global y versionado. Si no se indica, se genera a partir de `BUNDLE_ID` y `VERSION`. Debe actualizarse si cambian, por ejemplo, los identificadores de archivo de documento compatibles o las capacidades de análisis o transformación del plug-in. |
| `ARA_DOCUMENT_ARCHIVE_ID` | Identificador de los archivos de documento que crea el controlador de documento. Debe ser único y compartirse solo entre controladores que creen los mismos archivos y produzcan los mismos resultados con los mismos datos; hay que cambiarlo si el formato deja de ser compatible hacia atrás. Si no se indica, se crea uno independiente de la versión, válido mientras el formato no cambie. |
| `ARA_ANALYSIS_TYPES` | Por defecto ningún tipo analizable. Uno o varios de: `kARAContentTypeNotes`, `kARAContentTypeTempoEntries`, `kARAContentTypeBarSignatures`, `kARAContentTypeStaticTuning`, `kARAContentTypeKeySignatures`, `kARAContentTypeSheetChords`. |
| `ARA_TRANSFORMATION_FLAGS` | Por defecto `kARAPlaybackTransformationNoChanges`. Si el controlador puede aplicar los cambios correspondientes, uno o varios de: `kARAPlaybackTransformationTimestretch`, `kARAPlaybackTransformationTimestretchReflectingTempo`, `kARAPlaybackTransformationContentBasedFadeAtTail`, `kARAPlaybackTransformationContentBasedFadeAtHead`. |
| `VST3_AUTO_MANIFEST` | `TRUE`/`FALSE` (por defecto `TRUE`). Añade al target VST3 un paso `POST_BUILD` que genera `moduleinfo.json` en la carpeta `Resources` del bundle. Requiere que el plug-in pueda cargarse justo después de compilarse; si necesita procesado adicional antes (por ejemplo, firma propia), ponlo a `FALSE` y genera el manifiesto después con `higgs_enable_vst3_manifest_step`. Se recomienda encarecidamente generar el manifiesto: los hosts compatibles escanean el plug-in mucho más rápido. |

#### `higgs_add_binary_data`

```cmake
higgs_add_binary_data(<nombre>
    [HEADER_NAME ...]
    [NAMESPACE ...]
    SOURCES ...)
```

Crea una biblioteca estática que incrusta el contenido de los archivos indicados. Añade un target
`<nombre>` que se enlaza con `target_link_libraries`.

`HEADER_NAME` es opcional: si se indica, la cabecera generada tendrá ese nombre; si no, se llamará
`BinaryData.h`. En proyectos nuevos conviene dar un nombre único, para que los proyectos con más
de un target de datos binarios puedan incluir sus cabeceras sin ambigüedad.

`NAMESPACE` también es opcional; por defecto los archivos generados usan el espacio de nombres
`BinaryData`. Cada archivo de `SOURCES` se codifica y se incrusta en la biblioteca, que se enlaza
con `target_link_libraries(<otroTarget> PRIVATE <nombre>)`; la cabecera se incluye con
`#include <BinaryData.h>`.

```cmake
higgs_add_binary_data(MiAppDatos
    HEADER_NAME MiAppDatos.h
    NAMESPACE MiAppDatos
    SOURCES recursos/logo.svg recursos/fuente.ttf recursos/ir.wav)

target_link_libraries(MiApp PRIVATE MiAppDatos)
```

```cpp
#include <MiAppDatos.h>
auto logo = higgs::Drawable::createFromImageData (MiAppDatos::logo_svg, MiAppDatos::logo_svgSize);
```

#### `higgs_add_bundle_resources_directory`

```cmake
higgs_add_bundle_resources_directory(<target> <carpeta>)
```

Copia la carpeta `<carpeta>` completa en el directorio de recursos de un bundle de Apple: la
carpeta `Resources` en macOS y el nivel superior del bundle en iOS.

#### `higgs_generate_higgs_header`

```cmake
higgs_generate_higgs_header(<target>)
```

Examina los módulos enlazados a `<target>` y genera un `HiggsHeader.h` con un `#include` por cada
cabecera de módulo. Esta cabecera incluye también una instrucción opcional `using namespace higgs`
y un bloque opcional `ProjectInfo` (nombre del proyecto, empresa y versión), que se desactivan
definiendo `DONT_SET_USING_HIGGS_NAMESPACE` y `HIGGS_DONT_DECLARE_PROJECTINFO` respectivamente.
Se incluye con `#include <HiggsHeader.h>`. En proyectos CMake que no necesitan compatibilidad con
Mythos Studio su uso es opcional: puedes incluir directamente las cabeceras de módulo que
necesites.

#### `higgs_enable_copy_plugin_step`

```cmake
higgs_enable_copy_plugin_step(<target>)
```

Alternativa a la propiedad `HIGGS_COPY_PLUGIN_AFTER_BUILD`: activa manualmente la copia tras
compilar de un target creado con `higgs_add_plugin`.

`HIGGS_COPY_PLUGIN_AFTER_BUILD` instala los plug-ins inmediatamente tras compilarlos, lo que no
siempre es apropiado si hay que ejecutar pasos extra (firmar o modificar el bundle) antes. En esos
casos deja desactivada esa propiedad, añade tus pasos con
`add_custom_command(TARGET POST_BUILD)` y llama por último a `higgs_enable_copy_plugin_step`.

Si tus pasos necesitan la ubicación del artefacto, consulta la propiedad
`HIGGS_PLUGIN_ARTEFACT_FILE` del target de formato (¡*no* del target de código compartido!).

#### `higgs_enable_vst3_manifest_step`

```cmake
higgs_enable_vst3_manifest_step(<target>)
```

Activa manualmente la generación del manifiesto VST3 en un target creado con `higgs_add_plugin`.
`VST3_AUTO_MANIFEST TRUE` genera el manifiesto justo después de compilar, lo que no es apropiado si
el plug-in necesita pasos extra antes de poder cargarse. En ese caso usa `VST3_AUTO_MANIFEST
FALSE`, añade tus pasos con `add_custom_command(TARGET POST_BUILD)` y llama después a
`higgs_enable_vst3_manifest_step`.

#### `higgs_set_<kind>_sdk_path`

```cmake
higgs_set_aax_sdk_path(<ruta absoluta>)
higgs_set_vst2_sdk_path(<ruta absoluta>)
higgs_set_vst3_sdk_path(<ruta absoluta>)
higgs_set_ara_sdk_path(<ruta absoluta>)
```

Configuran tus SDK locales de AAX, VST2, VST3 y ARA. Deben llamarse *antes* de añadir cualquier
target que pueda depender de ellos (hosts de plug-ins, plug-ins AAX/VST2/VST3/ARA, etc.).

#### `higgs_add_module`

```cmake
higgs_add_module(<ruta al módulo>)
higgs_add_modules(<rutas de módulos>...)
```

`higgs_add_module` añade un target de biblioteca para el módulo situado en la ruta indicada, que
debe ser un directorio de módulo (por ejemplo `/Users/yo/HIGGS/modules/higgs_core`). Se crea una
biblioteca `INTERFACE` con el mismo nombre que el directorio, que se enlaza como cualquier otra con
`target_link_libraries`.

!!! danger "Enlaza los módulos siempre con visibilidad `PRIVATE`"
    Por cómo funcionan las bibliotecas `INTERFACE` en CMake, el enlace a un módulo añadido así
    *debe* hacerse con visibilidad `PRIVATE`. Con `PUBLIC`, las fuentes del módulo se añadirían
    tanto a `SOURCES` como a `INTERFACE_SOURCES` del target, lo que puede compilar muchas copias
    del módulo en un mismo target: en el mejor caso fallará la compilación y, en el peor, habrá
    violaciones silenciosas de la regla de una definición (ODR).

Argumentos opcionales: `INSTALL_PATH` es una ruta, relativa al prefijo de instalación, a la que se
copian las fuentes del módulo al instalarlo. `ALIAS_NAMESPACE` añade un alias del target con el
espacio de nombres indicado; por ejemplo, lo siguiente crea el target `mi_modulo` y el alias
`empresa::mi_modulo`:

```cmake
higgs_add_module(mi_modulo ALIAS_NAMESPACE empresa)
```

`higgs_add_modules` es una función de conveniencia para añadir varios módulos a la vez; acepta
muchas rutas de módulo. Véase el `CMakeLists.txt` del directorio `modules` como ejemplo.

#### `higgs_add_pip`

```cmake
higgs_add_pip(<cabecera>)
```

Analiza el bloque de metadatos PIP de la cabecera indicada y añade los targets correspondientes:
aplicación de consola, aplicación gráfica o plug-in de audio. Para plug-ins compila tantos
formatos como sea posible; para compilar también VST2, llama a `higgs_set_vst2_sdk_path` *antes*
de `higgs_add_pip`.

Está pensada sobre todo para compilar los ejemplos del repositorio y para prototipos rápidos con
la mínima configuración. Para cualquier caso más complejo, usa `higgs_add_gui_app`,
`higgs_add_plugin` o `higgs_add_console_app`, que dan un control más fino de las propiedades del
target. Véase [Proyectos PIP](pip.md).

#### `higgs_disable_default_flags`

```cmake
higgs_disable_default_flags()
```

Vacía `CMAKE_<LANG>_FLAGS_<MODO>` en el directorio actual y sus subdirectorios, para poder indicar
otras opciones de optimización/depuración sin que choquen con los valores por defecto de CMake.

#### `higgs_link_with_embedded_linux_subprocess`

```cmake
higgs_link_with_embedded_linux_subprocess(<target>)
```

Enlaza el target con una biblioteca `INTERFACE` que genera un ejecutable mínimo y lo incrusta como
recurso binario. Este recurso solo lo usa el módulo `higgs_gui_extra`, y solo con
`HIGGS_WEB_BROWSER` activado: cuando el código se ejecuta en un formato no independiente, el
ejecutable se despliega en un archivo temporal y se usa para alojar una vista WebKit. Es la técnica
que usan los plug-ins de audio en Linux.

Se llama automáticamente cuando hace falta en todos los targets creados con las funciones de
creación (`higgs_add_gui_app`, `higgs_add_console_app` y `higgs_add_plugin`), así que en esos casos
no necesitas llamarla.

### Targets

#### `higgs::higgs_recommended_warning_flags`

```cmake
target_link_libraries(miTarget PUBLIC higgs::higgs_recommended_warning_flags)
```

Activa las advertencias recomendadas al compilar el target. Solo establece opciones de compilador
y enlazador, sin bibliotecas ni directorios de inclusión. En plug-ins conviene enlazarlo al target
de código compartido con visibilidad `PUBLIC`, para que todos los *wrappers* hereden las mismas
opciones.

#### `higgs::higgs_recommended_config_flags`

```cmake
target_link_libraries(miTarget PUBLIC higgs::higgs_recommended_config_flags)
```

Activa las opciones de optimización y depuración recomendadas. Igual que el anterior, solo
establece opciones; en plug-ins, enlázalo al código compartido con visibilidad `PUBLIC`.

#### `higgs::higgs_recommended_lto_flags`

```cmake
target_link_libraries(miTarget PUBLIC higgs::higgs_recommended_lto_flags)
```

Activa la optimización en tiempo de enlace (LTO) recomendada. Igual que los anteriores, solo
establece opciones; en plug-ins, enlázalo al código compartido con visibilidad `PUBLIC`.

#### Targets de módulo

Cada módulo está disponible como `higgs::higgs_<área>` (por ejemplo `higgs::higgs_core`,
`higgs::higgs_dsp`). Las dependencias entre módulos se resuelven automáticamente: enlazar un módulo
enlaza también todos aquellos de los que depende.
