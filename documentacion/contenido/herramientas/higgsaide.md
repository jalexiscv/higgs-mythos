# higgsaide

`higgsaide` es una pequeña aplicación de consola (`extras/Build/higgsaide`) que el sistema de CMake
de Higgs Mythos compila y ejecuta **durante la compilación** de tus targets. Genera los archivos
auxiliares que en un proyecto de Mythos Studio produciría la propia herramienta: la cabecera
`HiggsHeader.h`, el código de los datos binarios, los `Info.plist`, los iconos, los
*entitlements*, los recursos de Windows, etc.

Normalmente **no necesitas invocarla**: las funciones `higgs_add_*`, `higgs_add_binary_data` y
`higgs_generate_higgs_header` la llaman por ti. Se instala junto con el framework (en el directorio
de herramientas indicado por la configuración de `find_package`) y no se compila cuando se activa
`HIGGS_MODULES_ONLY`.

## Uso

```text
higgsaide <modo> [argumentos...]
```

| Modo | Argumentos | Qué genera |
|---|---|---|
| `version` | — | Imprime la versión de Higgs Mythos con la que se compiló la herramienta. |
| `header` | `entrada salida` | La cabecera `HiggsHeader.h` del target, con los `#include` de sus módulos, el `using namespace higgs` opcional y el bloque `ProjectInfo`. |
| `binarydata` | `espacio_de_nombres nombre_cabecera carpeta_salida lista_de_archivos [--windows]` | El código C++ (`BinaryData.h` y `BinaryDataN.cpp`) que incrusta los archivos enumerados en `lista_de_archivos`, uno por línea. `--windows` usa saltos de línea CR-LF. |
| `plist` | `tipo entrada salida` | El `Info.plist` de una aplicación o plug-in de Apple. |
| `entitlements` | `tipo entrada salida` | El archivo de *entitlements* (*hardened runtime*, *sandbox*, iCloud, grupos de aplicaciones, notificaciones push…). |
| `pkginfo` | `tipo salida` | El archivo `PkgInfo` de un bundle de Apple. |
| `rcfile` | `entrada salida` | El archivo de recursos `.rc` de Windows (versión, icono, datos de la empresa). |
| `auplugindefines` | `entrada salida` | Las definiciones que necesitan los *wrappers* de Audio Unit. |
| `macicon` | `salida nombre_icono [icono_pequeño] [icono_grande]` | El icono `.icns` de macOS. |
| `iosassets` | `salida nombre_icono [icono_pequeño] [icono_grande]` | La carpeta `.xcassets` con los iconos de iOS. |
| `winicon` | `salida [icono_pequeño] [icono_grande]` | El icono `.ico` de Windows. |

Los archivos de `entrada` son volcados de las propiedades `HIGGS_*` del target que escribe el propio
script de CMake; por eso estos modos no están pensados para usarse a mano. Los modos de iconos y de
datos binarios sí pueden ser útiles en scripts propios.

!!! note
    En `docs/higgs-mythos.md` aparece `higgsaide --resgen` como ejemplo de invocación, pero la
    herramienta no tiene un modo `--resgen`: los modos disponibles son los de la tabla anterior.
