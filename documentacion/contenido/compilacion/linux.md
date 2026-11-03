# Dependencias en Linux

Lista de las dependencias necesarias para compilar proyectos de Higgs Mythos en Ubuntu, por
módulo. Cuando una dependencia es opcional se indica la macro que la desactiva.

Se ha probado en Ubuntu 16.04 LTS (Xenial Xerus), 18.04 LTS (Bionic Beaver) y 20.04 LTS (Focal
Fossa). En otras distribuciones los paquetes pueden tener otro nombre o no estar disponibles.

## Compilador

Se necesita un compilador de C++. Higgs Mythos se ha probado a fondo con Clang y GCC:

```bash
sudo apt update
sudo apt install clang
# o bien
sudo apt install g++
```

## Paquetes por módulo

| Módulo | Paquete | Opcional (macro que lo desactiva) |
|---|---|---|
| `higgs_audio_devices` | `libasound2-dev` | — |
| `higgs_audio_devices` | `libjack-jackd2-dev` | `HIGGS_JACK=0` |
| `higgs_audio_processors` | `ladspa-sdk` | `HIGGS_PLUGINHOST_LADSPA=0` |
| `higgs_core` | `libcurl4-openssl-dev` | `HIGGS_USE_CURL=0` |
| `higgs_graphics` | `libfontconfig1-dev` | `HIGGS_USE_FONTCONFIG=0` |
| `higgs_graphics` | `libfreetype-dev` | `HIGGS_USE_FREETYPE=0` |
| `higgs_gui_basics` | `libx11-dev` | — |
| `higgs_gui_basics` | `libxcomposite-dev` | — |
| `higgs_gui_basics` | `libxcursor-dev` | `HIGGS_USE_XCURSOR=0` |
| `higgs_gui_basics` | `libxext-dev` | — |
| `higgs_gui_basics` | `libxinerama-dev` | `HIGGS_USE_XINERAMA=0` |
| `higgs_gui_basics` | `libxrandr-dev` | `HIGGS_USE_XRANDR=0` |
| `higgs_gui_basics` | `libxrender-dev` | `HIGGS_USE_XRENDER=0` |
| `higgs_gui_basics` | `libxi-dev` | `HIGGS_USE_XINPUT=0` |
| `higgs_gui_extra` | `libwebkit2gtk-4.1-dev` (o `libwebkit2gtk-4.0-dev` en sistemas antiguos) | `HIGGS_WEB_BROWSER=0` |
| `higgs_opengl` | `libegl-dev` | — |
| `higgs_opengl` | `libglu1-mesa-dev` y `mesa-common-dev` | `HIGGS_OPENGL_ES=1` |
| `higgs_opengl` | `libgles-dev` | solo si `HIGGS_OPENGL_ES=1` |

Notas:

- `libfreetype-dev` y `libfontconfig1-dev` están disponibles en Ubuntu 22 y 24. Si
  `libfreetype-dev` no existe en tu sistema, prueba con `libfreetype6-dev`.
- Las aplicaciones compiladas cargan dinámicamente la versión de WebKitGTK (4.1 o 4.0) que haya
  disponible en tiempo de ejecución.
- Xrandr, Xinerama y XInput se consultan en tiempo de ejecución: tus usuarios no necesitan tenerlos
  instalados para que la aplicación funcione.

## Instalación completa

```bash
sudo apt update
sudo apt install libasound2-dev libjack-jackd2-dev \
    ladspa-sdk \
    libcurl4-openssl-dev \
    libfreetype-dev libfontconfig1-dev \
    libx11-dev libxcomposite-dev libxcursor-dev libxext-dev libxinerama-dev libxrandr-dev libxrender-dev libxi-dev \
    libwebkit2gtk-4.1-dev \
    libegl-dev \
    libglu1-mesa-dev mesa-common-dev
```

## Paquetes pkg-config y bibliotecas declaradas por los módulos

Los bloques de declaración de los módulos indican además estas dependencias de enlace en Linux:

| Módulo | `linuxPackages` / `linuxLibs` |
|---|---|
| `higgs_core` | `rt`, `dl`, `pthread` |
| `higgs_audio_devices` | `alsa` |
| `higgs_graphics` | `freetype2`, `fontconfig` |
| `higgs_opengl` | `egl`, `gl` |
