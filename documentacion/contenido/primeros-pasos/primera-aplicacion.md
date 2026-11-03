# Tu primera aplicación gráfica

Vamos a construir la aplicación de la plantilla `examples/CMake/GuiApp`: una ventana de escritorio
que muestra el texto *Hello World!*. Al terminar sabrás cómo se estructura cualquier aplicación
con interfaz gráfica en Higgs Mythos.

## Estructura del proyecto

```text
MiApp/
├── CMakeLists.txt
├── HIGGS/              # Higgs Mythos (submódulo o copia)
├── Main.cpp            # Clase de aplicación y ventana principal
├── MainComponent.h     # Componente con el contenido
└── MainComponent.cpp
```

## `CMakeLists.txt`

```cmake title="CMakeLists.txt"
cmake_minimum_required(VERSION 3.22)

project(GUI_APP_EXAMPLE VERSION 0.0.1)

add_subdirectory(HIGGS)                    # o bien: find_package(HIGGS CONFIG REQUIRED)

higgs_add_gui_app(GuiAppExample
    PRODUCT_NAME "Gui App Example"         # nombre del ejecutable final
    NEEDS_WEB_BROWSER FALSE                # TRUE si vas a usar vistas web
    NEEDS_CURL FALSE)                      # TRUE si vas a usar curl (Linux)

target_sources(GuiAppExample
    PRIVATE
        Main.cpp
        MainComponent.cpp)

target_compile_definitions(GuiAppExample
    PRIVATE
        HIGGS_WEB_BROWSER=0
        HIGGS_USE_CURL=0
        HIGGS_APPLICATION_NAME_STRING="$<TARGET_PROPERTY:GuiAppExample,HIGGS_PRODUCT_NAME>"
        HIGGS_APPLICATION_VERSION_STRING="$<TARGET_PROPERTY:GuiAppExample,HIGGS_VERSION>")

target_link_libraries(GuiAppExample
    PRIVATE
        higgs::higgs_gui_extra
    PUBLIC
        higgs::higgs_recommended_config_flags
        higgs::higgs_recommended_lto_flags
        higgs::higgs_recommended_warning_flags)
```

Puntos clave:

- **`higgs_add_gui_app`** crea un target ejecutable con muchas propiedades ya configuradas
  (bundle de macOS, `Info.plist`, iconos…). Acepta decenas de argumentos opcionales, descritos en
  el [API de CMake](../compilacion/cmake-api.md#higgs_add_target).
- **`HIGGS_WEB_BROWSER` y `HIGGS_USE_CURL`** están activados por defecto en los módulos. Si no los
  necesitas, desactívalos como aquí; si los necesitas, quita esas definiciones y pon
  `NEEDS_WEB_BROWSER TRUE` / `NEEDS_CURL TRUE`.
- **Las dependencias entre módulos se resuelven solas**: al enlazar `higgs::higgs_gui_extra` se
  enlazan también `higgs_gui_basics`, `higgs_graphics`, `higgs_events`, `higgs_core`, etc.
- Los targets **`higgs_recommended_*_flags`** activan las advertencias, optimizaciones y LTO
  recomendadas.

## `Main.cpp`: la clase de aplicación

Toda aplicación gráfica deriva de `higgs::HIGGSApplication` y se registra con la macro
`START_HIGGS_APPLICATION`, que genera la función `main` adecuada para cada plataforma.

```cpp title="Main.cpp"
#include "MainComponent.h"

class GuiAppApplication final : public higgs::HIGGSApplication
{
public:
    GuiAppApplication() {}

    const higgs::String getApplicationName() override    { return HIGGS_APPLICATION_NAME_STRING; }
    const higgs::String getApplicationVersion() override { return HIGGS_APPLICATION_VERSION_STRING; }
    bool moreThanOneInstanceAllowed() override          { return true; }

    void initialise (const higgs::String& commandLine) override
    {
        higgs::ignoreUnused (commandLine);
        mainWindow.reset (new MainWindow (getApplicationName()));   // (1)!
    }

    void shutdown() override
    {
        mainWindow = nullptr;                                         // (2)!
    }

    void systemRequestedQuit() override
    {
        quit();                                                       // (3)!
    }

    void anotherInstanceStarted (const higgs::String& commandLine) override
    {
        higgs::ignoreUnused (commandLine);
    }

    class MainWindow final : public higgs::DocumentWindow
    {
    public:
        explicit MainWindow (higgs::String name)
            : DocumentWindow (name,
                              higgs::Desktop::getInstance().getDefaultLookAndFeel()
                                                           .findColour (backgroundColourId),
                              allButtons)
        {
            setUsingNativeTitleBar (true);
            setContentOwned (new MainComponent(), true);              // (4)!

           #if HIGGS_IOS || HIGGS_ANDROID
            setFullScreen (true);
           #else
            setResizable (true, true);
            centreWithSize (getWidth(), getHeight());
           #endif

            setVisible (true);
        }

        void closeButtonPressed() override
        {
            getInstance()->systemRequestedQuit();
        }

    private:
        HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainWindow)
    };

private:
    std::unique_ptr<MainWindow> mainWindow;
};

START_HIGGS_APPLICATION (GuiAppApplication)                           // (5)!
```

1. `initialise()` se llama cuando el bucle de mensajes ya está en marcha: es el lugar para crear
   ventanas y recursos.
2. `shutdown()` se llama justo antes de salir; aquí se destruye la ventana.
3. El sistema operativo pide cerrar la aplicación (por ejemplo, con ++cmd+q++ en macOS).
4. La ventana toma posesión del componente de contenido y lo destruirá con ella.
5. Genera el punto de entrada (`main`, `WinMain`, etc.) para la plataforma actual.

!!! tip "No sobrescribas métodos de `DocumentWindow` sin necesidad"
    La clase base usa muchos de sus métodos internamente. Pon tu lógica en el componente de
    contenido; si de verdad necesitas sobrescribir un método de la ventana, llama también al de la
    clase base.

## `MainComponent`: el contenido

Un `higgs::Component` es la unidad básica de la interfaz. Los dos métodos que casi siempre vas a
sobrescribir son `paint()` (dibujar) y `resized()` (colocar los hijos).

```cpp title="MainComponent.h"
#pragma once

#include <higgs_gui_extra/higgs_gui_extra.h>

class MainComponent final : public higgs::Component
{
public:
    MainComponent();

    void paint (higgs::Graphics&) override;
    void resized() override;

private:
    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MainComponent)
};
```

```cpp title="MainComponent.cpp"
#include "MainComponent.h"

MainComponent::MainComponent()
{
    setSize (600, 400);
}

void MainComponent::paint (higgs::Graphics& g)
{
    g.fillAll (getLookAndFeel().findColour (higgs::ResizableWindow::backgroundColourId));

    g.setFont (higgs::FontOptions (16.0f));
    g.setColour (higgs::Colours::white);
    g.drawText ("Hello World!", getLocalBounds(), higgs::Justification::centred, true);
}

void MainComponent::resized()
{
    // Aquí se colocan los componentes hijos con setBounds() o con FlexBox/Grid.
}
```

`HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR` hace la clase no copiable y le añade un detector
de fugas de memoria que avisa en las compilaciones de depuración si quedan instancias vivas al
cerrar la aplicación.

## Compilar y ejecutar

```bash
cmake -B build
cmake --build build --target GuiAppExample
```

El ejecutable se genera dentro de `build/GuiAppExample_artefacts/<Configuración>/`. En macOS es un
bundle `.app`.

## Añadir un botón

Para comprobar cómo se combinan componentes, añade un `TextButton` como miembro, hazlo visible y
colócalo en `resized()`:

```cpp
class MainComponent final : public higgs::Component
{
public:
    MainComponent()
    {
        addAndMakeVisible (boton);
        boton.onClick = [this] { ++pulsaciones; repaint(); };
        setSize (600, 400);
    }

    void paint (higgs::Graphics& g) override
    {
        g.fillAll (higgs::Colours::darkslategrey);
        g.setColour (higgs::Colours::white);
        g.drawText ("Pulsaciones: " + higgs::String (pulsaciones),
                    getLocalBounds().removeFromTop (getHeight() / 2),
                    higgs::Justification::centred);
    }

    void resized() override
    {
        boton.setBounds (getLocalBounds().withSizeKeepingCentre (160, 40));
    }

private:
    higgs::TextButton boton { "Púlsame" };
    int pulsaciones = 0;
};
```

Continúa con la [guía de interfaz gráfica](../guias/interfaz-grafica.md) para profundizar en
componentes, *layouts* y *look-and-feel*.
