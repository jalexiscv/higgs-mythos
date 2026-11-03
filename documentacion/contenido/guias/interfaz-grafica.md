# Interfaz gráfica

La interfaz de usuario se construye con **componentes**: objetos derivados de `higgs::Component`
organizados en un árbol de padres e hijos. Los módulos implicados son `higgs_gui_basics`
(componentes, ventanas, *widgets*, menús, *layouts*, accesibilidad) y `higgs_gui_extra` (editor de
código, navegador web, utilidades de documentos y más).

## El componente

| Método | Cuándo se llama | Para qué |
|---|---|---|
| `paint (Graphics&)` | Cuando hay que redibujar | Dibujar el contenido propio |
| `paintOverChildren (Graphics&)` | Tras pintar los hijos | Dibujar encima de ellos |
| `resized()` | Cuando cambia el tamaño | Colocar los componentes hijos |
| `mouseDown`, `mouseDrag`, `mouseUp`, `mouseMove`, `mouseWheelMove`… | Eventos de ratón/táctiles | Interacción |
| `keyPressed (const KeyPress&)` | Pulsación de tecla con el foco | Atajos y edición |
| `visibilityChanged()`, `parentHierarchyChanged()`, `lookAndFeelChanged()` | Cambios de estado | Reaccionar al contexto |

Operaciones fundamentales:

```cpp
addAndMakeVisible (hijo);           // añade un hijo y lo hace visible
hijo.setBounds (10, 10, 100, 30);   // posición y tamaño relativos al padre
repaint();                          // pide un redibujado (asíncrono)
setOpaque (true);                   // optimización si pintas todo tu área
setInterceptsMouseClicks (false, true);
```

!!! warning "Solo desde el hilo de mensajes"
    Crear, modificar, repintar o destruir componentes debe hacerse siempre en el hilo de
    mensajes. Desde otros hilos usa `MessageManager::callAsync` (véase
    [Bucle de mensajes](../conceptos/eventos.md)).

Un componente **no es propietario** de sus hijos: lo habitual es declararlos como miembros por
valor de la clase padre, con lo que se destruyen con ella.

## Widgets incluidos

| Categoría | Clases |
|---|---|
| Botones | `TextButton`, `ToggleButton`, `ImageButton`, `DrawableButton`, `ArrowButton`, `HyperlinkButton`, `ShapeButton` |
| Texto | `Label`, `TextEditor` |
| Valores | `Slider`, `ComboBox`, `ProgressBar` |
| Listas y tablas | `ListBox`, `TableListBox`, `TreeView` |
| Contenedores | `Viewport`, `TabbedComponent`, `ConcertinaPanel`, `GroupComponent`, `ResizableWindow`, `DocumentWindow`, `DialogWindow` |
| Propiedades | `PropertyPanel` con `SliderPropertyComponent`, `BooleanPropertyComponent`, `ChoicePropertyComponent`, `TextPropertyComponent`… |
| Menús | `PopupMenu`, `MenuBarComponent`, `MenuBarModel`, `BurgerMenuComponent` |
| Archivos | `FileChooser`, `FileBrowserComponent`, `FilenameComponent`, `FileTreeComponent` |
| Avisos | `AlertWindow`, `NativeMessageBox`, `BubbleMessageComponent`, `TooltipWindow`, `CallOutBox` |
| Extra (`higgs_gui_extra`) | `CodeEditorComponent`, `WebBrowserComponent`, `ColourSelector`, `KeyMappingEditorComponent`, `PreferencesPanel`, `SplashScreen`, `SystemTrayIconComponent`, `AnimatedAppComponent` |

Los widgets exponen *callbacks* como `std::function` además de las clases `Listener` clásicas:

```cpp
slider.setRange (20.0, 20000.0);
slider.setSkewFactorFromMidPoint (1000.0);
slider.onValueChange = [this] { frecuencia = slider.getValue(); };

combo.addItemList ({ "Seno", "Cuadrada", "Sierra" }, 1);
combo.onChange = [this] { forma = combo.getSelectedId(); };

boton.onClick = [this] { reiniciar(); };
```

## Diseño (*layout*)

### Con `Rectangle`

La forma más simple y muy legible: ir recortando el área disponible.

```cpp
void resized() override
{
    auto area = getLocalBounds().reduced (10);

    cabecera.setBounds (area.removeFromTop (40));
    area.removeFromTop (10);                         // separación
    panelLateral.setBounds (area.removeFromLeft (200));
    contenido.setBounds (area);
}
```

### Con `FlexBox`

```cpp
void resized() override
{
    higgs::FlexBox fb;
    fb.flexDirection  = higgs::FlexBox::Direction::row;
    fb.flexWrap       = higgs::FlexBox::Wrap::wrap;
    fb.justifyContent = higgs::FlexBox::JustifyContent::spaceBetween;

    for (auto* b : botones)
        fb.items.add (higgs::FlexItem (*b).withMinWidth (80.0f).withHeight (32.0f).withFlex (1.0f));

    fb.performLayout (getLocalBounds().reduced (8));
}
```

### Con `Grid`

```cpp
void resized() override
{
    using Track = higgs::Grid::TrackInfo;
    using Fr    = higgs::Grid::Fr;

    higgs::Grid grid;
    grid.templateRows    = { Track (Fr (1)), Track (Fr (3)) };
    grid.templateColumns = { Track (Fr (1)), Track (Fr (2)) };
    grid.columnGap = grid.rowGap = higgs::Grid::Px (6);

    grid.items = { higgs::GridItem (titulo).withArea (1, 1, 2, 3),
                   higgs::GridItem (lista),
                   higgs::GridItem (detalle) };

    grid.performLayout (getLocalBounds());
}
```

Los ejemplos `FlexBoxDemo` y `GridDemo` muestran todas las opciones. Para restringir el tamaño de
una ventana, usa `ComponentBoundsConstrainer` o `ResizableWindow::setResizeLimits()`.

## Look-and-feel

El aspecto de todos los widgets lo decide un objeto `LookAndFeel`. La implementación por defecto
es `LookAndFeel_V4`, con esquemas de color oscuro, intermedio, gris y claro. Puedes:

1. **Cambiar colores** con identificadores de color:

    ```cpp
    slider.setColour (higgs::Slider::thumbColourId, higgs::Colours::orange);
    getLookAndFeel().setColour (higgs::TextButton::buttonColourId, higgs::Colour (0xff2d2d30));
    ```

2. **Sustituir el dibujo** de un widget derivando de `LookAndFeel_V4`:

    ```cpp
    class MiAspecto final : public higgs::LookAndFeel_V4
    {
    public:
        void drawRotarySlider (higgs::Graphics& g, int x, int y, int w, int h,
                               float posicion, float anguloInicio, float anguloFin,
                               higgs::Slider&) override
        {
            auto area   = higgs::Rectangle<int> (x, y, w, h).toFloat().reduced (4.0f);
            auto angulo = anguloInicio + posicion * (anguloFin - anguloInicio);

            g.setColour (higgs::Colours::darkgrey);
            g.fillEllipse (area);

            higgs::Path aguja;
            aguja.addRectangle (-2.0f, -area.getHeight() * 0.5f, 4.0f, area.getHeight() * 0.3f);
            g.setColour (higgs::Colours::orange);
            g.fillPath (aguja, higgs::AffineTransform::rotation (angulo)
                                   .translated (area.getCentreX(), area.getCentreY()));
        }
    };
    ```

3. **Aplicarlo** a un componente (y sus hijos) con `setLookAndFeel (&miAspecto)` o globalmente con
   `LookAndFeel::setDefaultLookAndFeel (&miAspecto)`. Recuerda llamar a
   `setLookAndFeel (nullptr)` antes de destruir el objeto de aspecto.

Véase el ejemplo `LookAndFeelDemo`.

## Ratón, teclado y táctil

```cpp
void mouseDown (const higgs::MouseEvent& e) override
{
    if (e.mods.isPopupMenu())
        mostrarMenu();
    else
        inicioArrastre = e.position;
}

void mouseDrag (const higgs::MouseEvent& e) override
{
    desplazamiento = e.position - inicioArrastre;
    repaint();
}

bool keyPressed (const higgs::KeyPress& k) override
{
    if (k == higgs::KeyPress::spaceKey) { alternarReproduccion(); return true; }
    return false;   // deja que el padre lo gestione
}
```

`MouseEvent::source` identifica el dedo o puntero en entornos multitáctiles (véase
`MultiTouchDemo`). Para arrastrar y soltar entre componentes o desde el sistema de archivos, usa
`DragAndDropContainer`, `DragAndDropTarget` y `FileDragAndDropTarget`.

## Comandos y menús

`ApplicationCommandManager` centraliza las acciones de la aplicación, sus atajos de teclado y su
estado (activado, marcado), para usarlas desde menús, botones y teclado de forma coherente:

```cpp
class Editor final : public higgs::Component,
                     public higgs::ApplicationCommandTarget
{
public:
    enum Comandos { guardar = 1, deshacer };

    void getAllCommands (higgs::Array<higgs::CommandID>& c) override { c.addArray ({ guardar, deshacer }); }

    void getCommandInfo (higgs::CommandID id, higgs::ApplicationCommandInfo& info) override
    {
        if (id == guardar)
        {
            info.setInfo ("Guardar", "Guarda el documento", "Archivo", 0);
            info.addDefaultKeypress ('s', higgs::ModifierKeys::commandModifier);
        }
        // ...
    }

    bool perform (const InvocationInfo& info) override
    {
        if (info.commandID == guardar) { guardarDocumento(); return true; }
        return false;
    }

    higgs::ApplicationCommandTarget* getNextCommandTarget() override { return nullptr; }
};
```

`PopupMenu::addCommandItem()` crea entradas de menú enlazadas a estos comandos, y
`KeyMappingEditorComponent` permite que el usuario redefina los atajos (véase `KeyMappingsDemo` y
`MenusDemo`).

## Diálogos

```cpp
auto opciones = higgs::MessageBoxOptions::makeOptionsOkCancel (higgs::MessageBoxIconType::QuestionIcon,
                                                              "¿Guardar cambios?",
                                                              "El documento tiene cambios sin guardar.");
higgs::AlertWindow::showAsync (opciones, [] (int resultado)
{
    if (resultado == 1)
        guardar();
});
```

Para elegir archivos usa `FileChooser` de forma asíncrona:

```cpp
selector = std::make_unique<higgs::FileChooser> ("Abrir audio", higgs::File(), "*.wav;*.flac;*.mp3");
selector->launchAsync (higgs::FileBrowserComponent::openMode | higgs::FileBrowserComponent::canSelectFiles,
                       [this] (const higgs::FileChooser& fc)
                       {
                           if (auto f = fc.getResult(); f.existsAsFile())
                               cargar (f);
                       });
```

!!! note "Cambios en 9.0.5"
    En 9.0.5, `NativeMessageBox::show` y `showAsync` se renombraron y `MessageBoxOptions::withButton()`
    exige ahora un código de resultado entero. Consulta
    [Cambios incompatibles 9.x](../proyecto/cambios-incompatibles.md) si actualizas código antiguo.

## Ventanas

- `DocumentWindow`: ventana de nivel superior con barra de título y botones.
- `DialogWindow`: ventanas de diálogo modales o no modales (`DialogWindow::LaunchOptions`).
- `Desktop::getInstance().getDisplays()`: información de pantallas, áreas útiles y escala.
- Ventanas nativas hijas (*native child windows*), añadidas en 9.0.5.
- `TopLevelWindow`, `ThreadWithProgressWindow`, `CallOutBox` y `TooltipWindow` para otros casos.

La escala de interfaz (HiDPI) se gestiona automáticamente; `Desktop::setGlobalScaleFactor()`
permite aplicar una escala adicional a toda la aplicación.

## Demos relacionadas

`HelloWorldDemo`, `ComponentDemo`, `WidgetsDemo`, `LookAndFeelDemo`, `FlexBoxDemo`, `GridDemo`,
`MenusDemo`, `DialogsDemo`, `WindowsDemo`, `PropertiesDemo`, `KeyMappingsDemo`,
`ComponentTransformsDemo`, `MultiTouchDemo`, `CodeEditorDemo`, `MDIDemo`. Véase el
[catálogo de ejemplos](../ejemplos/index.md).
