# Otros módulos

## OSC

`higgs_osc` implementa **Open Sound Control** sobre UDP: mensajes, paquetes (*bundles*),
patrones de dirección con comodines y etiquetas de tiempo.

```cpp
// Enviar
higgs::OSCSender emisor;
if (emisor.connect ("127.0.0.1", 9001))
    emisor.send ("/mezclador/canal/1/volumen", 0.8f);

// Recibir (en el hilo de mensajes)
class Receptor final : private higgs::OSCReceiver,
                       private higgs::OSCReceiver::Listener<higgs::OSCReceiver::MessageLoopCallback>
{
public:
    Receptor()
    {
        connect (9001);
        addListener (this, "/mezclador/canal/1/volumen");
    }

private:
    void oscMessageReceived (const higgs::OSCMessage& m) override
    {
        if (m.size() == 1 && m[0].isFloat32())
            volumen = m[0].getFloat32();
    }

    float volumen = 0.0f;
};
```

Usa `OSCReceiver::RealtimeCallback` como argumento de plantilla si prefieres recibir los mensajes
directamente en el hilo de red. Clases: `OSCSender`, `OSCReceiver`, `OSCMessage`, `OSCBundle`,
`OSCArgument`, `OSCAddress`, `OSCAddressPattern`, `OSCTimeTag`, `OSCTypes`. Véase `OSCDemo`.

## JavaScript

`higgs_javascript` contiene un intérprete de JavaScript basado en **QuickJS** (a través de CHOC):

```cpp
higgs::JavascriptEngine motor;
motor.maximumExecutionTime = higgs::RelativeTime::seconds (2);

auto api = new higgs::DynamicObject();
api->setMethod ("log", [] (const higgs::var::NativeFunctionArgs& args)
{
    DBG (args.arguments[0].toString());
    return higgs::var();
});
motor.registerNativeObject ("app", api);

higgs::Result resultado = higgs::Result::ok();
auto valor = motor.evaluate ("app.log('hola'); 6 * 7", &resultado);   // 42
```

`execute()` ejecuta código sin devolver valor, `callFunction()` llama a una función definida en el
script y `stop()` interrumpe la ejecución desde otro hilo. `JSObject` y `JSCursor` permiten navegar
por los objetos del intérprete. Véase `JavaScriptDemo`.

## Criptografía

`higgs_cryptography` ofrece funciones básicas: `RSAKey` (generación de pares de claves,
cifrado/firma), `BlowFish`, `Primes`, y los resúmenes `MD5`, `SHA256` y `Whirlpool`.

```cpp
auto resumen = higgs::SHA256 (higgs::File ("/ruta/archivo.bin")).toHexString();

higgs::RSAKey publica, privada;
higgs::RSAKey::createKeyPair (publica, privada, 1024);
```

!!! warning
    Estas clases cubren usos sencillos (licencias, integridad, ofuscación). Para seguridad de
    nivel profesional usa una biblioteca criptográfica especializada.

Véase `CryptographyDemo`.

## Box2D

`higgs_box2d` incluye el motor de física **Box2D 2.2.1** y `Box2DRenderer`, que dibuja un `b2World`
con un objeto `Graphics`. Véase `Box2DDemo`.

## Analítica

`higgs_analytics` recopila eventos de uso y los envía a destinos configurables:

```cpp
auto& analitica = *higgs::Analytics::getInstance();
analitica.setUserId ("usuario-anonimo-123");
analitica.addDestination (new MiDestino());          // deriva de ThreadedAnalyticsDestination
higgs::StringPairArray parametros;
parametros.set ("formato", "wav");
analitica.logEvent ("archivo_abierto", parametros);
```

`ThreadedAnalyticsDestination` envía los eventos por lotes desde un hilo de fondo y los guarda si
no hay conexión; `ButtonTracker` registra automáticamente las pulsaciones de un botón. Véase
`AnalyticsCollectionDemo`.

## Compras integradas y desbloqueo de productos

`higgs_product_unlocking` incluye:

- `InAppPurchases`: compras integradas en las tiendas de Apple y Google (consultar productos,
  comprar, restaurar, descargar contenido alojado). Requiere `NEEDS_STORE_KIT TRUE` en macOS.
  Véase `InAppPurchasesDemo`.
- `OnlineUnlockStatus` y `OnlineUnlockForm`: sistema de licencias con autenticación en línea y
  archivos de clave firmados con RSA. `KeyGeneration` genera esos archivos de clave
  (`generateKeyFile`, `generateExpiringKeyFile`), y `TracktionMarketplaceStatus` es una
  implementación lista para un servidor de *marketplace* concreto.

## Notificaciones push

`PushNotifications` (en `higgs_gui_extra`) gestiona las notificaciones locales y remotas en iOS,
macOS y Android. Requiere activar `PUSH_NOTIFICATIONS_ENABLED TRUE` en el target. Véase
`PushNotificationsDemo`.
