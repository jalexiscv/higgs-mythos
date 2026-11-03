# Primeros pasos

Esta sección te lleva desde cero hasta tener compilado tu primer proyecto con Higgs Mythos.

1. **[Requisitos del sistema](requisitos.md)**: compiladores, versiones de CMake y sistemas
   operativos soportados, tanto para compilar como para desplegar.
2. **[Instalación](instalacion.md)**: cómo obtener el framework y conectarlo a tu proyecto con
   `add_subdirectory` o con `find_package`.
3. **[Tu primera aplicación gráfica](primera-aplicacion.md)**: una ventana con un componente que
   dibuja texto, explicada línea por línea.
4. **[Aplicación de consola](aplicacion-consola.md)**: un programa sin interfaz que usa
   `higgs_core`.
5. **[Tu primer plug-in de audio](primer-plugin.md)**: un procesador de ganancia con parámetro
   automatizable, editor y guardado de estado, compilado como VST3, AU y Standalone.
6. **[Compilar los ejemplos](compilar-ejemplos.md)**: cómo compilar el DemoRunner y las
   herramientas incluidas en el repositorio.

!!! tip "Si tienes prisa"
    Copia una de las carpetas de `examples/CMake` (`GuiApp`, `ConsoleApp` o `AudioPlugin`) fuera
    del repositorio, añade Higgs Mythos como subdirectorio o submódulo de Git, descomenta la línea
    `add_subdirectory(...)` de su `CMakeLists.txt` y compila. Es exactamente lo que explican las
    páginas siguientes, paso a paso.
