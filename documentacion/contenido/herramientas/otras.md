# Otras utilidades

## AudioPerformanceTest

`extras/AudioPerformanceTest`: aplicación de audio que mide el rendimiento del procesamiento en
tiempo real del dispositivo y del sistema. Útil para comparar plataformas y detectar regresiones de
rendimiento.

## BinaryBuilder

`extras/BinaryBuilder`: utilidad de consola que convierte un conjunto de archivos binarios en un par
`.h`/`.cpp` con su contenido como datos, para incrustarlos directamente en un ejecutable. En CMake
normalmente usarás [`higgs_add_binary_data`](../compilacion/cmake-api.md#higgs_add_binary_data), y en
Mythos Studio los recursos binarios del proyecto o `--encode-binary`.

## NetworkGraphicsDemo

`extras/NetworkGraphicsDemo`: demostración de dibujo distribuido. Varios dispositivos heterogéneos
de una red local (macOS, Linux, Windows, iOS, Android) dibujan conjuntamente partes de una gran
imagen vectorial animada que genera y difunde uno de ellos. Si se colocan las pantallas una junto a
otra, forman una única pantalla grande.

- Ejecuta una instancia con el argumento `master` para que actúe como generador; el resto actúan
  como clientes.
- El maestro abre una ventana donde se colocan los dispositivos (arrastrando y con la rueda del
  ratón) y se elige la demo con las flechas izquierda/derecha.
- Los toques y clics en los clientes se envían al maestro, que puede reaccionar a ellos.
- Su pieza más interesante es un `LowLevelGraphicsContext` que, en lugar de pintar en pantalla,
  serializa todo lo dibujado en paquetes binarios que se difunden por UDP. El contenido se edita en
  `Demos.h`.

## WindowsDLL

`extras/WindowsDLL`: proyecto de Mythos Studio (`projectType="dll"`, definición
`HIGGS_DLL_BUILD=1`) para compilar el framework como biblioteca dinámica de Windows (`higgs_dll`).

## SBOM

`extras/SBOM`: herramientas del inventario de software (*Software Bill of Materials*):

- `dependencies.json`: manifiesto del código de terceros incluido (nombre, módulo, ruta, licencia,
  proveedor, versión…).
- `generate_sbom.py`: genera `HiggsMythos.spdx.json` a partir del manifiesto, o comprueba con
  `--check` que el archivo del repositorio está al día.
- `submit_dependency_snapshot.py`: envía el inventario a la API de dependencias de GitHub.

```bash
python3 extras/SBOM/generate_sbom.py .            # regenera HiggsMythos.spdx.json
python3 extras/SBOM/generate_sbom.py . --check    # falla si está desactualizado
```

Para añadir o actualizar una dependencia: incorpora el código, añade un archivo
`HIGGS_UPSTREAM.txt` en su directorio (con al menos la línea `Upstream:` y, si la versión no se
puede deducir del código, `Version:` o `Commit:`), añade o actualiza su entrada en el manifiesto,
incrementa `lastUpdated` y ejecuta el script. Véase [Dependencias de terceros](../proyecto/dependencias.md).

## Soporte de CMake

`extras/Build/CMake` contiene los scripts del sistema de compilación (`HIGGSUtils.cmake`,
`HIGGSModuleSupport.cmake`, `HIGGSHelperTargets.cmake`, `HIGGSCheckAtomic.cmake`, módulos
`Find*.cmake` para WebView2, CppWinRT y Windows MIDI Services) y las plantillas que usa para generar
código (`PIPComponent.cpp.in`, `PIPAudioProcessor.cpp.in`, `PIPConsole.cpp.in`,
`UnityPluginGUIScript.cs.in`, `LaunchScreen.storyboard`…). `extras/Build/higgs_build_tools` es la
biblioteca compartida por Mythos Studio y `higgsaide` para generar *plists*, iconos y recursos.
