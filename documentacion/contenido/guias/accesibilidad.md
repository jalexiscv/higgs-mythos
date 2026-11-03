# Accesibilidad

Higgs Mythos es compatible con los lectores de pantalla **Narrator** (Windows), **VoiceOver**
(macOS e iOS) y **TalkBack** (Android). Su API de accesibilidad expone a estos clientes:

- el título, la descripción y el texto de ayuda de los elementos de la interfaz;
- acceso programático a los elementos y a su texto;
- interacción con los elementos;
- navegación completa de la interfaz con el teclado;
- envío de notificaciones a los clientes que escuchan.

## Comportamiento por defecto

Todo `Component` visible y habilitado es accesible por defecto y expone información básica: su
título, descripción, texto de ayuda y su posición en la jerarquía de elementos. Los *widgets*
estándar (botones, deslizadores, editores de texto, listas, tablas, árboles, menús…) ya
proporcionan su rol, su valor y las acciones adecuadas.

## Personalizar textos y navegación

```cpp
botonGrabar.setTitle ("Grabar");
botonGrabar.setDescription ("Empieza a grabar desde la entrada seleccionada");
botonGrabar.setHelpText ("Atajo: R");

// Orden de navegación explícito y agrupación
panelTransporte.setFocusContainerType (higgs::Component::FocusContainerType::keyboardFocusContainer);
botonReproducir.setExplicitFocusOrder (1);
botonGrabar.setExplicitFocusOrder (2);
```

- `setTitle()`, `setDescription()` y `setHelpText()` controlan lo que leen los lectores de
  pantalla.
- `setExplicitFocusOrder()`, `setFocusContainerType()` y `createFocusTraverser()` controlan las
  relaciones padre/hijo y el orden de navegación entre elementos.
- `setAccessible (false)` oculta a los lectores de pantalla un componente puramente decorativo.

## Componentes personalizados

Para un control propio, crea un `AccessibilityHandler` con el rol adecuado y devuélvelo desde
`Component::createAccessibilityHandler()`:

```cpp
class Potenciometro final : public higgs::Component
{
public:
    std::unique_ptr<higgs::AccessibilityHandler> createAccessibilityHandler() override
    {
        struct Valor final : public higgs::AccessibilityRangedNumericValueInterface
        {
            explicit Valor (Potenciometro& p) : pot (p) {}

            bool isReadOnly() const override               { return false; }
            double getCurrentValue() const override        { return pot.valor; }
            void setValue (double v) override              { pot.setValor (v); }
            AccessibleValueRange getRange() const override { return { { 0.0, 1.0 }, 0.01 }; }

            Potenciometro& pot;
        };

        return std::make_unique<higgs::AccessibilityHandler> (
            *this,
            higgs::AccessibilityRole::slider,
            higgs::AccessibilityActions{},
            higgs::AccessibilityHandler::Interfaces { std::make_unique<Valor> (*this) });
    }

    void setValor (double v) { valor = higgs::jlimit (0.0, 1.0, v); repaint(); }

    double valor = 0.5;
};
```

`AccessibilityHandler` envuelve el componente con un rol (`AccessibilityRole`), una lista de
acciones (`AccessibilityActions`: pulsar, alternar, enfocar, mostrar menú…) e interfaces opcionales
de valor, texto, tabla o celda (`AccessibilityValueInterface`, `AccessibilityTextInterface`,
`AccessibilityTableInterface`, `AccessibilityCellInterface`). Su estado (`getCurrentState()`)
transmite más información a los clientes. Para anunciar algo al usuario usa
`AccessibilityHandler::postAnnouncement()`.

## Para saber más

- El ejemplo `AccessibilityDemo` muestra todas estas técnicas.
- [Protocolo NSAccessibility](https://developer.apple.com/documentation/appkit/nsaccessibility?language=objc) (Apple).
- [UI Automation para aplicaciones Win32](https://docs.microsoft.com/en-us/windows/win32/winauto/entry-uiauto-win32) (Microsoft).
- Charla de introducción a esta funcionalidad (ADC 2020): <https://youtu.be/BqrEv4ApH3U>.
