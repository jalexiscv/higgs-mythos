# Aplicación de consola

Las aplicaciones de consola no tienen ventana ni bucle de mensajes gráfico; son útiles para
herramientas de línea de comandos, procesamiento por lotes de audio, servidores o pruebas. La
plantilla está en `examples/CMake/ConsoleApp`.

## `CMakeLists.txt`

```cmake title="CMakeLists.txt"
cmake_minimum_required(VERSION 3.22)

project(CONSOLE_APP_EXAMPLE VERSION 0.0.1)

add_subdirectory(HIGGS)

higgs_add_console_app(ConsoleAppExample
    PRODUCT_NAME "Console App Example"
    NEEDS_WEB_BROWSER FALSE
    NEEDS_CURL FALSE)

target_sources(ConsoleAppExample PRIVATE Main.cpp)

target_compile_definitions(ConsoleAppExample
    PRIVATE
        HIGGS_WEB_BROWSER=0
        HIGGS_USE_CURL=0)

target_link_libraries(ConsoleAppExample
    PRIVATE
        higgs::higgs_core
    PUBLIC
        higgs::higgs_recommended_config_flags
        higgs::higgs_recommended_warning_flags)
```

## `Main.cpp`

En una aplicación de consola escribes tu propio `main`:

```cpp title="Main.cpp"
#include <higgs_core/higgs_core.h>

int main (int argc, char* argv[])
{
    higgs::ArgumentList args (argc, argv);

    if (args.containsOption ("--help|-h"))
    {
        std::cout << "Uso: " << args.executableName << " [--saludo=texto]" << std::endl;
        return 0;
    }

    const auto saludo = args.containsOption ("--saludo")
                            ? args.getValueForOption ("--saludo")
                            : higgs::String ("Hola desde Higgs Mythos");

    std::cout << saludo << std::endl;

    const auto escritorio = higgs::File::getSpecialLocation (higgs::File::userDesktopDirectory);
    std::cout << "Tu escritorio está en: " << escritorio.getFullPathName() << std::endl;

    return 0;
}
```

`ArgumentList` analiza la línea de comandos; `ConsoleApplication` (también en `higgs_core`) permite
además registrar subcomandos y gestionar errores con `ConsoleApplication::fail()`, que es como
está escrita la herramienta [`higgsaide`](../herramientas/higgsaide.md).

## Inicializar el sistema de mensajes

Si tu programa de consola necesita clases que dependen del bucle de mensajes (`Timer`,
`MessageManager::callAsync`, `ChangeBroadcaster` asíncrono…), crea al principio de `main` un
objeto `higgs::ScopedHiggsInitialiser_GUI` (módulo `higgs_events`). Mientras exista, el
`MessageManager` y el resto de la infraestructura de mensajes estarán inicializados:

```cpp
#include <higgs_events/higgs_events.h>

int main()
{
    higgs::ScopedHiggsInitialiser_GUI inicializador;   // arranca el MessageManager
    // ...
}
```

Para tareas puramente de cálculo, ficheros o red basta con `higgs_core`.
