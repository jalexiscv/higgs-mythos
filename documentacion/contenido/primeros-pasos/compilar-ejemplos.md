# Compilar los ejemplos y herramientas

El repositorio incluye más de 80 ejemplos (en `examples/`) y varias aplicaciones de apoyo (en
`extras/`). Ninguno se compila por defecto; se activan con dos opciones de CMake.

## DemoRunner: todas las demos en una aplicación

```bash
cmake . -B cmake-build -DHIGGS_BUILD_EXAMPLES=ON -DHIGGS_BUILD_EXTRAS=ON
cmake --build cmake-build --target DemoRunner
```

**DemoRunner** reúne las demos de audio, DSP, interfaz gráfica y utilidades en una sola aplicación
con un navegador lateral. Es la mejor forma de explorar lo que puede hacer el framework y de leer
el código de cada demo junto a su resultado. También tiene proyectos ya generados en
`examples/DemoRunner/Builds` para Android, iOS, macOS, Visual Studio 2019/2022/2026 y Linux
Makefile.

## Cada ejemplo como proyecto independiente

Con `-DHIGGS_BUILD_EXAMPLES=ON`, cada archivo `.h` de `examples/Audio`, `DSP`, `GUI`, `Plugins` y
`Utilities` es un **PIP**: un proyecto completo contenido en un solo archivo de cabecera, con un
bloque de metadatos que describe sus dependencias y su tipo (véase [Proyectos PIP](../compilacion/pip.md)).
`higgs_add_pip` convierte cada uno en su propio target, cuyo nombre coincide con el campo `name`
de sus metadatos:

```bash
cmake --build cmake-build --target HelloWorldDemo
cmake --build cmake-build --target GainPlugin_VST3
cmake --build cmake-build --target AudioPluginDemo_Standalone
```

Algunos ejemplos se excluyen automáticamente según la plataforma y los SDK disponibles:

| Ejemplo | Se omite cuando… |
|---|---|
| `CameraDemo`, `VideoDemo` | se compila en Linux o BSD |
| `PushNotificationsDemo` | se compila en Linux, BSD o Windows |
| `ARAPluginDemo` | no se ha configurado el SDK de ARA (`higgs_set_ara_sdk_path`) |
| `ReaperEmbeddedViewPluginDemo` | no se ha configurado el SDK de VST2 |
| `WebViewPluginDemo` | en Windows, si no se encuentra el paquete WebView2 |

El [catálogo de ejemplos](../ejemplos/index.md) describe todos ellos.

## Plantillas mínimas de `examples/CMake`

`examples/CMake` contiene tres proyectos para copiar y empezar: `GuiApp`, `ConsoleApp` y
`AudioPlugin`. Con `HIGGS_BUILD_EXAMPLES=ON` también se compilan dentro del repositorio, lo que
sirve para comprobar que tu entorno funciona.

## Herramientas de `extras`

Con `-DHIGGS_BUILD_EXTRAS=ON` se añaden estos targets:

| Target | Descripción |
|---|---|
| `MythosStudio` | La herramienta de configuración y exportación de proyectos. Véase [Mythos Studio](../herramientas/mythos-studio.md). |
| `AudioPluginHost` | Host de plug-ins con grafo de procesamiento, para probar tus plug-ins. Véase [AudioPluginHost](../herramientas/audio-plugin-host.md). |
| `UnitTestRunner` | Ejecuta las pruebas unitarias de los módulos desde la consola. Véase [UnitTestRunner](../herramientas/unit-test-runner.md). |
| `AudioPerformanceTest` | Aplicación para medir el rendimiento del procesamiento de audio. |
| `BinaryBuilder` | Convierte archivos binarios en código C++ (`.h`/`.cpp`). |
| `NetworkGraphicsDemo` | Demo de dibujo distribuido entre varios dispositivos de una red local. |

## Compilar con SDK externos

Si quieres compilar los ejemplos que dependen de SDK que no se incluyen (VST2, ARA) o usar una
copia propia del SDK de AAX, pásalos al configurar:

```bash
cmake . -B cmake-build -DHIGGS_BUILD_EXAMPLES=ON -DHIGGS_BUILD_EXTRAS=ON \
    -DHIGGS_GLOBAL_ARA_SDK_PATH=/ruta/a/ARA_SDK \
    -DHIGGS_GLOBAL_VST2_SDK_PATH=/ruta/a/VST2_SDK \
    -DHIGGS_GLOBAL_AAX_SDK_PATH=/ruta/a/AAX_SDK
```

!!! tip "Tiempo de configuración"
    Hay muchos ejemplos, y generar los proyectos con estas opciones activadas tarda bastante más.
    En tu día a día, déjalas desactivadas y actívalas solo cuando quieras explorar los ejemplos.
