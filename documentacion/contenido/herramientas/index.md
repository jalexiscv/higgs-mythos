# Herramientas

La carpeta `extras/` del repositorio contiene las aplicaciones y utilidades que acompañan al
framework. Se compilan con `-DHIGGS_BUILD_EXTRAS=ON` (salvo `higgsaide`, que se compila siempre que
no se use `HIGGS_MODULES_ONLY`), y casi todas incluyen también proyectos ya generados por Mythos
Studio en su carpeta `Builds/`.

| Herramienta | Carpeta | Para qué sirve |
|---|---|---|
| [Mythos Studio](mythos-studio.md) | `extras/MythosStudio` | Crear y configurar proyectos, exportarlos a Xcode, Visual Studio, Android Studio y Makefiles, editar código y automatizar tareas desde la línea de comandos |
| [higgsaide](higgsaide.md) | `extras/Build/higgsaide` | Herramienta auxiliar del sistema de CMake: genera `HiggsHeader.h`, datos binarios, *plists*, iconos, *entitlements*… |
| [AudioPluginHost](audio-plugin-host.md) | `extras/AudioPluginHost` | Host de plug-ins con grafo de procesamiento para probar tus plug-ins |
| [UnitTestRunner](unit-test-runner.md) | `extras/UnitTestRunner` | Ejecutor de las pruebas unitarias de los módulos desde la consola |
| [Otras utilidades](otras.md) | `extras/AudioPerformanceTest`, `BinaryBuilder`, `NetworkGraphicsDemo`, `WindowsDLL`, `SBOM` | Medición de rendimiento, conversión de binarios, demos de red, DLL de Windows e inventario de dependencias |
