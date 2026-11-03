# Crear un módulo propio

Empaquetar tu código como módulo te permite reutilizarlo entre proyectos y añadirlo igual que los
módulos del framework, tanto con CMake como con Mythos Studio. Sigue el
[formato de módulos](formato-modulos.md).

## 1. Estructura

```text
mis_modulos/
└── miempresa_efectos/
    ├── miempresa_efectos.h        # cabecera principal con la declaración
    ├── miempresa_efectos.cpp      # unidad de compilación única
    └── efectos/
        ├── miempresa_Distorsion.h
        └── miempresa_Distorsion.cpp
```

## 2. Cabecera principal

```cpp title="miempresa_efectos/miempresa_efectos.h"
/*******************************************************************************
 BEGIN_HIGGS_MODULE_DECLARATION

  ID:                 miempresa_efectos
  vendor:             miempresa
  version:            1.0.0
  name:               Efectos de Mi Empresa
  description:        Procesadores de audio reutilizables.
  website:            https://miempresa.example
  license:            Propietaria
  minimumCppStandard: 17

  dependencies:       higgs_dsp

 END_HIGGS_MODULE_DECLARATION
*******************************************************************************/

#pragma once
#define MIEMPRESA_EFECTOS_H_INCLUDED

#include <higgs_dsp/higgs_dsp.h>

/** Config: MIEMPRESA_EFECTOS_SOBREMUESTREO
    Activa el sobremuestreo interno de la distorsión.
*/
#ifndef MIEMPRESA_EFECTOS_SOBREMUESTREO
 #define MIEMPRESA_EFECTOS_SOBREMUESTREO 1
#endif

#include "efectos/miempresa_Distorsion.h"
```

## 3. Unidad de compilación

```cpp title="miempresa_efectos/miempresa_efectos.cpp"
#include "miempresa_efectos.h"

#include "efectos/miempresa_Distorsion.cpp"
```

Recuerda: dentro del módulo, todas las inclusiones de sus propios archivos usan **rutas relativas**.

## 4. Añadirlo con CMake

```cmake
higgs_add_module(${CMAKE_CURRENT_SOURCE_DIR}/mis_modulos/miempresa_efectos
                 ALIAS_NAMESPACE miempresa)

target_link_libraries(MiPlugin PRIVATE miempresa::miempresa_efectos)

target_compile_definitions(MiPlugin PUBLIC MIEMPRESA_EFECTOS_SOBREMUESTREO=0)
```

- Enlaza **siempre** con visibilidad `PRIVATE` (véase
  [higgs_add_module](cmake-api.md#higgs_add_module)).
- Si activas `HIGGS_ENABLE_MODULE_SOURCE_GROUPS`, llama a `higgs_add_module` antes de crear los
  targets que lo usan.
- Las dependencias declaradas (`higgs_dsp` en el ejemplo) se enlazan automáticamente.

## 5. Añadirlo con Mythos Studio

En el menú contextual de la sección *Modules* del proyecto, elige **Add a module from a specified
folder...** y selecciona la carpeta del módulo. Mythos Studio lee la declaración, resuelve las dependencias y muestra
las opciones `Config:` para configurarlas desde la interfaz. Puedes registrar una ruta global de
módulos de usuario con `--set-global-search-path … defaultUserModulePath …` (véase
[Mythos Studio](../herramientas/mythos-studio.md#linea-de-comandos)).

## 6. Distribuirlo

`MythosStudio --buildmodule carpeta_destino carpeta_modulo` empaqueta un módulo en un ZIP, y
`--buildallmodules` empaqueta todos los módulos de una carpeta generando un índice.

## Buenas prácticas

- Usa un prefijo de proveedor propio en el nombre del módulo y de los archivos para evitar
  colisiones.
- Declara tu código en tu propio espacio de nombres.
- Mantén el número de unidades de compilación al mínimo.
- Documenta cada opción `Config:`: es lo que verá el usuario en Mythos Studio.
- Si incluyes código de terceros, documenta su licencia y su procedencia.
