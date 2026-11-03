# higgs_midi_ci

Implementación de MIDI Capability Inquiry (MIDI-CI): descubrimiento de dispositivos, configuración de perfiles, intercambio de propiedades y consultas de proceso, con codificación y análisis de los mensajes.

## Ficha

| Campo | Valor |
|---|---|
| Identificador | `higgs_midi_ci` |
| Versión | 9.0.5 |
| Nombre declarado | Higgs Mythos MIDI CI Classes |
| Descripción declarada | Classes facilitating communication via MIDI Capability Inquiry |
| Estándar mínimo de C++ | C++17 |
| Licencia | AGPLv3 |
| Dependencias directas | [`higgs_audio_basics`](higgs_audio_basics.md) |
| Dependencias indirectas | [`higgs_core`](higgs_core.md) |
| Código fuente | [`modules/higgs_midi_ci`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_midi_ci) |

## Uso

```cmake
target_link_libraries(MiTarget PRIVATE higgs::higgs_midi_ci)
```

```cpp
#include <higgs_midi_ci/higgs_midi_ci.h>
```

Guías relacionadas: [MIDI, MPE y MIDI 2.0](../guias/midi.md#midi-ci).

## Opciones de configuración

Este módulo no declara opciones de configuración.

## Catálogo de clases

81 clases y estructuras públicas declaradas en las cabeceras que incluye `higgs_midi_ci.h`, agrupadas por carpeta. La descripción es la primera frase de su comentario de documentación en el código.

### `ci`

| Clase | Descripción | Cabecera |
|---|---|---|
| `FunctionBlock` | Contiene información sobre un bloque de función MIDI 2.0. | [`higgs_CIFunctionBlock.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_midi_ci/ci/higgs_CIFunctionBlock.h) |
| `MUID` | Un ID de 28 bits que identifica de forma única a un dispositivo que participa en una serie de transacciones MIDI-CI. | [`higgs_CIMuid.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_midi_ci/ci/higgs_CIMuid.h) |
| `EncodingUtils` | Funciones utilitarias para trabajar con el enum Encoding. | [`higgs_CIEncoding.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_midi_ci/ci/higgs_CIEncoding.h) |
| `Encodings` | Funciones de utilidad para trabajar con formatos de datos usados por los mensajes de intercambio de propiedades. | [`higgs_CIEncodings.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_midi_ci/ci/higgs_CIEncodings.h) |
| `ChannelInGroupUtils` | Funciones utilitarias para trabajar con el enum ChannelInGroup. | [`higgs_CIMessages.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_midi_ci/ci/higgs_CIMessages.h) |
| `ComparableRange` | Envuelve un span y proporciona operadores de igualdad que comparan el contenido del span elemento por elemento. | [`higgs_CIMessages.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_midi_ci/ci/higgs_CIMessages.h) |
| `Header` | Contiene los campos que se encuentran al comienzo de cada mensaje MIDI CI. | [`higgs_CIMessages.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_midi_ci/ci/higgs_CIMessages.h) |
| `Generic` | Agrupa un encabezado de mensaje CI y cierta cantidad de bytes finales. | [`higgs_CIMessages.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_midi_ci/ci/higgs_CIMessages.h) |
| `DiscoveryResponse` | Consulte la especificación MIDI-CI. | [`higgs_CIMessages.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_midi_ci/ci/higgs_CIMessages.h) |
| `Discovery` | Consulte la especificación MIDI-CI. | [`higgs_CIMessages.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_midi_ci/ci/higgs_CIMessages.h) |
| `EndpointInquiryResponse` | Consulte la especificación MIDI-CI. | [`higgs_CIMessages.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_midi_ci/ci/higgs_CIMessages.h) |
| `EndpointInquiry` | Consulte la especificación MIDI-CI. | [`higgs_CIMessages.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_midi_ci/ci/higgs_CIMessages.h) |
| `InvalidateMUID` | Consulte la especificación MIDI-CI. | [`higgs_CIMessages.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_midi_ci/ci/higgs_CIMessages.h) |
| `ACK` | Consulte la especificación MIDI-CI. | [`higgs_CIMessages.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_midi_ci/ci/higgs_CIMessages.h) |
| `NAK` | Consulte la especificación MIDI-CI. | [`higgs_CIMessages.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_midi_ci/ci/higgs_CIMessages.h) |
| `ProfileInquiryResponse` | Consulte la especificación MIDI-CI. | [`higgs_CIMessages.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_midi_ci/ci/higgs_CIMessages.h) |
| `ProfileInquiry` | Consulte la especificación MIDI-CI. | [`higgs_CIMessages.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_midi_ci/ci/higgs_CIMessages.h) |
| `ProfileAdded` | Consulte la especificación MIDI-CI. | [`higgs_CIMessages.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_midi_ci/ci/higgs_CIMessages.h) |
| `ProfileRemoved` | Consulte la especificación MIDI-CI. | [`higgs_CIMessages.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_midi_ci/ci/higgs_CIMessages.h) |
| `ProfileDetailsResponse` | Consulte la especificación MIDI-CI. | [`higgs_CIMessages.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_midi_ci/ci/higgs_CIMessages.h) |
| `ProfileDetails` | Consulte la especificación MIDI-CI. | [`higgs_CIMessages.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_midi_ci/ci/higgs_CIMessages.h) |
| `ProfileOn` | Consulte la especificación MIDI-CI. | [`higgs_CIMessages.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_midi_ci/ci/higgs_CIMessages.h) |
| `ProfileOff` | Consulte la especificación MIDI-CI. | [`higgs_CIMessages.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_midi_ci/ci/higgs_CIMessages.h) |
| `ProfileEnabledReport` | Consulte la especificación MIDI-CI. | [`higgs_CIMessages.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_midi_ci/ci/higgs_CIMessages.h) |
| `ProfileDisabledReport` | Consulte la especificación MIDI-CI. | [`higgs_CIMessages.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_midi_ci/ci/higgs_CIMessages.h) |
| `ProfileSpecificData` | Consulte la especificación MIDI-CI. | [`higgs_CIMessages.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_midi_ci/ci/higgs_CIMessages.h) |
| `PropertyExchangeCapabilitiesResponse` | Consulte la especificación MIDI-CI. | [`higgs_CIMessages.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_midi_ci/ci/higgs_CIMessages.h) |
| `PropertyExchangeCapabilities` | Consulte la especificación MIDI-CI. | [`higgs_CIMessages.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_midi_ci/ci/higgs_CIMessages.h) |
| `StaticSizePropertyExchange` | Mensaje de intercambio de propiedades que no tiene carga útil y, por lo tanto, debe estar contenido en un único fragmento. | [`higgs_CIMessages.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_midi_ci/ci/higgs_CIMessages.h) |
| `DynamicSizePropertyExchange` | Un mensaje de intercambio de propiedades que puede formar parte de una secuencia de mensajes de varios fragmentos. | [`higgs_CIMessages.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_midi_ci/ci/higgs_CIMessages.h) |
| `PropertyGetDataResponse` | Consulte la especificación MIDI-CI. | [`higgs_CIMessages.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_midi_ci/ci/higgs_CIMessages.h) |
| `PropertyGetData` | Consulte la especificación MIDI-CI. | [`higgs_CIMessages.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_midi_ci/ci/higgs_CIMessages.h) |
| `PropertySetDataResponse` | Consulte la especificación MIDI-CI. | [`higgs_CIMessages.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_midi_ci/ci/higgs_CIMessages.h) |
| `PropertySetData` | Consulte la especificación MIDI-CI. | [`higgs_CIMessages.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_midi_ci/ci/higgs_CIMessages.h) |
| `PropertySubscribeResponse` | Consulte la especificación MIDI-CI. | [`higgs_CIMessages.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_midi_ci/ci/higgs_CIMessages.h) |
| `PropertySubscribe` | Consulte la especificación MIDI-CI. | [`higgs_CIMessages.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_midi_ci/ci/higgs_CIMessages.h) |
| `PropertyNotify` | Consulte la especificación MIDI-CI. | [`higgs_CIMessages.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_midi_ci/ci/higgs_CIMessages.h) |
| `ProcessInquiryResponse` | Consulte la especificación MIDI-CI. | [`higgs_CIMessages.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_midi_ci/ci/higgs_CIMessages.h) |
| `ProcessInquiry` | Consulte la especificación MIDI-CI. | [`higgs_CIMessages.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_midi_ci/ci/higgs_CIMessages.h) |
| `ProcessMidiMessageReportResponse` | Consulte la especificación MIDI-CI. | [`higgs_CIMessages.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_midi_ci/ci/higgs_CIMessages.h) |
| `ProcessMidiMessageReport` | Consulte la especificación MIDI-CI. | [`higgs_CIMessages.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_midi_ci/ci/higgs_CIMessages.h) |
| `ProcessEndMidiMessageReport` | Consulte la especificación MIDI-CI. | [`higgs_CIMessages.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_midi_ci/ci/higgs_CIMessages.h) |
| `Parsed` | Un mensaje con un encabezado y un cuerpo opcional. | [`higgs_CIMessages.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_midi_ci/ci/higgs_CIMessages.h) |
| `ChannelAddress` | Identifica un canal o un conjunto de canales en un endpoint MIDI de múltiples grupos. | [`higgs_CIChannelAddress.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_midi_ci/ci/higgs_CIChannelAddress.h) |
| `BufferOutput` | Representa un destino en el que pueden escribirse mensajes MIDI-CI. | [`higgs_CIResponderOutput.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_midi_ci/ci/higgs_CIResponderOutput.h) |
| `ResponderOutput` | Una salida de búfer que además proporciona información sobre un mensaje entrante, para poder construir una respuesta adecuada a ese mensaje. | [`higgs_CIResponderOutput.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_midi_ci/ci/higgs_CIResponderOutput.h) |
| `Parser` | Analiza mensajes CI. | [`higgs_CIParser.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_midi_ci/ci/higgs_CIParser.h) |
| `SupportedAndActive` | Contiene el número máximo de canales que se pueden activar para un perfil MIDI-CI, junto con el número de canales activos actualmente. | [`higgs_CISupportedAndActive.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_midi_ci/ci/higgs_CISupportedAndActive.h) |
| `ResponderDelegate` | Interfaz para tipos que implementan respuestas a ciertos tipos de mensajes. | [`higgs_CIResponderDelegate.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_midi_ci/ci/higgs_CIResponderDelegate.h) |
| `ProfileStateEntry` | Contiene un ID de perfil, junto con la cantidad de canales admitidos y activos correspondientes a ese perfil. | [`higgs_CIProfileStates.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_midi_ci/ci/higgs_CIProfileStates.h) |
| `ChannelProfileStates` | Contiene el número de canales admitidos y activados para todos los perfiles en una dirección de canal concreta. | [`higgs_CIProfileStates.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_midi_ci/ci/higgs_CIProfileStates.h) |
| `GroupProfileStates` | Contiene los estados de perfil de cada canal de un grupo, junto con el estado de los perfiles que se aplican al grupo en sí. | [`higgs_CIProfileStates.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_midi_ci/ci/higgs_CIProfileStates.h) |
| `BlockProfileStates` | Contiene los estados de perfil de cada grupo y canal de un bloque de funciones, junto con el estado de los perfiles que se aplican al propio bloque de funciones. | [`higgs_CIProfileStates.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_midi_ci/ci/higgs_CIProfileStates.h) |
| `ProfileAtAddress` | Contiene un ID de perfil y la dirección de un grupo/canal. | [`higgs_CIProfileAtAddress.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_midi_ci/ci/higgs_CIProfileAtAddress.h) |
| `ProfileDelegate` | Interfaz con métodos que se pueden sobrescribir para personalizar cómo responde a las consultas de perfiles un Device que implementa perfiles. | [`higgs_CIProfileDelegate.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_midi_ci/ci/higgs_CIProfileDelegate.h) |
| `ProfileHost` | Actuando como ResponderListener, las instancias de esta clase pueden formular respuestas adecuadas a las transacciones de perfil iniciadas por dispositivos remotos. | [`higgs_CIProfileHost.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_midi_ci/ci/higgs_CIProfileHost.h) |
| `Subscription` | Asocia un ID de suscripción con un nombre de recurso. | [`higgs_CISubscription.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_midi_ci/ci/higgs_CISubscription.h) |
| `PropertySubscriptionCommandUtils` | Funciones para usar con PropertySubscriptionCommand. | [`higgs_CIPropertyDelegate.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_midi_ci/ci/higgs_CIPropertyDelegate.h) |
| `PropertySubscriptionHeader` | Una estructura que contiene miembros de datos correspondientes a campos comunes de un encabezado de suscripción de propiedades. | [`higgs_CIPropertyDelegate.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_midi_ci/ci/higgs_CIPropertyDelegate.h) |
| `Pagination` | Contiene información sobre la paginación de una solicitud. | [`higgs_CIPropertyDelegate.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_midi_ci/ci/higgs_CIPropertyDelegate.h) |
| `PropertyRequestHeader` | Una estructura con miembros de datos que corresponden a los campos comunes de un encabezado de solicitud de propiedad. | [`higgs_CIPropertyDelegate.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_midi_ci/ci/higgs_CIPropertyDelegate.h) |
| `PropertyRequestData` | Agrupa un encabezado de solicitud de propiedad y una carga útil de datos. | [`higgs_CIPropertyDelegate.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_midi_ci/ci/higgs_CIPropertyDelegate.h) |
| `PropertyReplyHeader` | Estructura que contiene miembros de datos correspondientes a campos comunes en una respuesta a una solicitud de intercambio de propiedades. | [`higgs_CIPropertyDelegate.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_midi_ci/ci/higgs_CIPropertyDelegate.h) |
| `PropertyReplyData` | Agrupa un encabezado de respuesta de propiedad y una carga útil de datos. | [`higgs_CIPropertyDelegate.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_midi_ci/ci/higgs_CIPropertyDelegate.h) |
| `PropertyDelegate` | Una interfaz con métodos que se pueden sobrescribir para personalizar cómo responde a las consultas de propiedades un Device que implementa propiedades. | [`higgs_CIPropertyDelegate.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_midi_ci/ci/higgs_CIPropertyDelegate.h) |
| `PropertyExchangeResult` | Contiene los datos devueltos por un respondedor en respuesta a una solicitud. | [`higgs_CIPropertyExchangeResult.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_midi_ci/ci/higgs_CIPropertyExchangeResult.h) |
| `RequestID` | Un identificador fuertemente tipado para un ID de solicitud de 7 bits con un estado anulable. | [`higgs_CIPropertyExchangeCache.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_midi_ci/ci/higgs_CIPropertyExchangeCache.h) |
| `InitiatorPropertyExchangeCache` | Acumula fragmentos de mensajes enviados por otro dispositivo en respuesta a una transacción iniciada por un dispositivo local. | [`higgs_CIPropertyExchangeCache.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_midi_ci/ci/higgs_CIPropertyExchangeCache.h) |
| `ResponderPropertyExchangeCache` | Acumula los fragmentos de mensaje que forman una solicitud iniciada por un dispositivo remoto. | [`higgs_CIPropertyExchangeCache.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_midi_ci/ci/higgs_CIPropertyExchangeCache.h) |
| `CacheProvider` | Una interfaz para objetos que proporcionan recursos para transacciones de intercambio de propiedades. | [`higgs_CIPropertyExchangeCache.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_midi_ci/ci/higgs_CIPropertyExchangeCache.h) |
| `RequestKey` | Una clave utilizada para identificar de forma única las transacciones en curso iniciadas por un ci::Device. | [`higgs_CIPropertyHost.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_midi_ci/ci/higgs_CIPropertyHost.h) |
| `PropertyHost` | Al actuar como ResponderListener, las instancias de esta clase pueden formular respuestas adecuadas a las transacciones de propiedades iniciadas por dispositivos remotos. | [`higgs_CIPropertyHost.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_midi_ci/ci/higgs_CIPropertyHost.h) |
| `DeviceFeatures` | Indicadores que señalan las funciones admitidas por un dispositivo CI determinado. | [`higgs_CIDeviceFeatures.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_midi_ci/ci/higgs_CIDeviceFeatures.h) |
| `DeviceMessageHandler` | Una interfaz que recibirá un callback cada vez que un Device desee enviar un nuevo mensaje MIDI-CI. | [`higgs_CIDeviceMessageHandler.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_midi_ci/ci/higgs_CIDeviceMessageHandler.h) |
| `DeviceOptions` | Opciones de configuración de un Device. | [`higgs_CIDeviceOptions.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_midi_ci/ci/higgs_CIDeviceOptions.h) |
| `SubscriptionKey` | Una clave utilizada para identificar de forma única las suscripciones a propiedades en curso iniciadas por un ci::Device. | [`higgs_CISubscriptionManager.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_midi_ci/ci/higgs_CISubscriptionManager.h) |
| `SubscriptionManagerDelegate` | Funciones usadas por un SubscriptionManager para negociar suscripciones. | [`higgs_CISubscriptionManager.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_midi_ci/ci/higgs_CISubscriptionManager.h) |
| `SubscriptionManager` | Administra las suscripciones a propiedades en dispositivos remotos. | [`higgs_CISubscriptionManager.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_midi_ci/ci/higgs_CISubscriptionManager.h) |
| `PropertySubscriptionData` | Contiene información relacionada con una actualización de suscripción. | [`higgs_CIDeviceListener.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_midi_ci/ci/higgs_CIDeviceListener.h) |
| `DeviceListener` | Una interfaz que recibe callbacks cuando un Device recibe determinados mensajes. | [`higgs_CIDeviceListener.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_midi_ci/ci/higgs_CIDeviceListener.h) |
| `Device` | Las instancias de este tipo se encargan de analizar e interpretar los mensajes MIDI-CI entrantes y de enviar mensajes MIDI-CI a otros dispositivos. | [`higgs_CIDevice.h`](https://github.com/jalexiscv/higgs-mythos/blob/main/modules/higgs_midi_ci/ci/higgs_CIDevice.h) |
