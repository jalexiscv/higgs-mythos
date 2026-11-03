# Interfaces web (WebView)

`WebBrowserComponent` (módulo `higgs_gui_extra`) incrusta la vista web nativa de cada sistema en un
componente. Además de mostrar páginas, ofrece una **integración bidireccional** entre el código
JavaScript de la página y tu aplicación C++, de modo que puedes construir la interfaz de una
aplicación o de un plug-in con HTML, CSS y JavaScript/TypeScript, empaquetada dentro del binario
y sin servidor web externo.

## Motores por plataforma

| Plataforma | Motor | Requisitos |
|---|---|---|
| macOS / iOS | WKWebView | Ninguno |
| Windows | WebView2 (Edge/Chromium) o Internet Explorer heredado | WebView2: paquete NuGet `Microsoft.Web.WebView2`; `NEEDS_WEBVIEW2 TRUE` en CMake o `HIGGS_USE_WIN_WEBVIEW2(_WITH_STATIC_LINKING)` |
| Linux | WebKitGTK | `libwebkit2gtk-4.1-dev` (o 4.0); `NEEDS_WEB_BROWSER TRUE` en CMake |
| Android | WebView del sistema | Ninguno |

La opción `HIGGS_WEB_BROWSER` (activada por defecto) permite eliminar por completo
`WebBrowserComponent` si no lo usas. En plug-ins de Linux, el navegador se aloja en un pequeño
subproceso incrustado (véase `higgs_link_with_embedded_linux_subprocess` en el
[API de CMake](../compilacion/cmake-api.md#higgs_link_with_embedded_linux_subprocess)).

## Uso básico

```cpp
higgs::WebBrowserComponent navegador;
addAndMakeVisible (navegador);
navegador.goToURL ("https://github.com/jalexiscv/higgs-mythos");
```

## Integración nativa

Las opciones se configuran con `WebBrowserComponent::Options`, un constructor inmutable:

```cpp
higgs::WebBrowserComponent web { higgs::WebBrowserComponent::Options{}
    .withBackend (higgs::WebBrowserComponent::Options::Backend::webview2)       // solo afecta a Windows
    .withNativeIntegrationEnabled()
    .withNativeFunction ("saludar", [] (const higgs::Array<higgs::var>& args, auto completar)
    {
        completar ("Hola, " + args[0].toString());
    })
    .withEventListener ("botonPulsado", [] (const higgs::var& datos)
    {
        DBG ("La página envió: " << higgs::JSON::toString (datos));
    })
    .withResourceProvider ([this] (const higgs::String& ruta) { return obtenerRecurso (ruta); }) };

// Cargar la interfaz servida por el proveedor de recursos:
web.goToURL (higgs::WebBrowserComponent::getResourceProviderRoot());

// Enviar un evento a la página:
web.emitEventIfBrowserIsVisible ("nivel", higgs::var (0.75));
```

Con la integración nativa activada, la página puede:

- registrar oyentes de eventos emitidos por el *backend* C++,
- emitir eventos que el *backend* observa (`withEventListener`),
- llamar a funciones nativas registradas (`withNativeFunction`), que devuelven una *Promise*,
- descargar recursos servidos por el *backend* (`withResourceProvider`).

El proveedor de recursos recibe la ruta pedida y devuelve un
`std::optional<WebBrowserComponent::Resource>` con los bytes y el tipo MIME. Así puedes incrustar
toda la interfaz web (por ejemplo, un ZIP generado con `higgs_add_binary_data`) dentro del
ejecutable. Para desarrollo, el segundo argumento de `withResourceProvider` permite autorizar un
origen externo (por ejemplo, un servidor de desarrollo en `http://localhost:5173`).

## El paquete npm `@jalexiscv/webview`

Las vinculaciones TypeScript para el lado de la página están en
`modules/higgs_gui_extra/native/typescript/webview-interop` y se publican como paquete npm:

```bash
npm install @jalexiscv/webview
```

```ts
import { getNativeFunction, getSliderState, getBackendResourceAddress } from "@jalexiscv/webview";

const saludar = getNativeFunction("saludar");
saludar("mundo").then((respuesta) => console.log(respuesta));

const corte = getSliderState("cutoffSlider");
corte.valueChangedEvent.addListener(() => actualizar(corte.getNormalisedValue()));
```

Exporta `getNativeFunction`, `getSliderState`, `getToggleState`, `getComboBoxState`,
`getBackendResourceAddress` y `ControlParameterIndexUpdater`. La versión del *frontend* debe
coincidir con la que espera el *backend*: si no coinciden, verás una aserción en C++ y una
advertencia en la consola de JavaScript. El paquete incluye objetos simulados para probar la
página en un navegador normal (sin eventos ni respuestas reales).

## Parámetros de plug-in en la interfaz web

Para sincronizar controles HTML con parámetros del plug-in se usan **relays** y sus
**attachments**:

| Control web | Relay (C++) | Attachment | Estado en TypeScript |
|---|---|---|---|
| Deslizador | `WebSliderRelay` | `WebSliderParameterAttachment` | `getSliderState()` |
| Interruptor | `WebToggleButtonRelay` | `WebToggleButtonParameterAttachment` | `getToggleState()` |
| Lista desplegable | `WebComboBoxRelay` | `WebComboBoxParameterAttachment` | `getComboBoxState()` |

```cpp
class EditorWeb final : public higgs::AudioProcessorEditor
{
public:
    explicit EditorWeb (MiProcesador& p)
        : AudioProcessorEditor (&p),
          adjuntoCorte (*p.parametros.getParameter ("corte"), releCorte, nullptr)
    {
        addAndMakeVisible (web);
        web.goToURL (higgs::WebBrowserComponent::getResourceProviderRoot());
        setSize (600, 400);
    }

    void resized() override { web.setBounds (getLocalBounds()); }

private:
    higgs::WebSliderRelay releCorte { "cutoffSlider" };

    higgs::WebBrowserComponent web { higgs::WebBrowserComponent::Options{}
                                         .withNativeIntegrationEnabled()
                                         .withOptionsFrom (releCorte)
                                         .withResourceProvider ([] (const auto& ruta) { return obtenerRecurso (ruta); }) };

    higgs::WebSliderParameterAttachment adjuntoCorte;
};
```

`WebControlParameterIndexReceiver` junto con `ControlParameterIndexUpdater` (TypeScript) informa
al host de qué parámetro está bajo el ratón, para funciones como «automatizar el control
señalado».

!!! warning "Orden de declaración"
    Declara los *relays* **antes** que el `WebBrowserComponent` (que los recibe en sus opciones) y
    los *attachments* **después**, como en el ejemplo.

## Demos relacionadas

`WebBrowserDemo` y `WebViewPluginDemo` (un filtro con interfaz HTML/JS que incluye análisis
espectral enviado desde C++).
