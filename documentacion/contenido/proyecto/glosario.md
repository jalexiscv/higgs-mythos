# Glosario

ARA
:   *Audio Random Access*. Extensión de VST3 y AU que da a los plug-ins acceso al audio completo de
    las pistas del host e intercambio de información musical. Véase [ARA](../guias/ara.md).

AU / AUv3
:   *Audio Unit*, el formato de plug-ins de Apple. AUv3 es la versión basada en extensiones de
    aplicación, disponible en macOS e iOS.

AAX
:   Formato de plug-ins de Avid para Pro Tools. Requiere firma digital con las herramientas de PACE.

*Attachment*
:   Objeto que sincroniza un control de la interfaz (deslizador, botón, lista) con un parámetro del
    plug-in, en ambos sentidos.

Bus
:   Grupo de canales de entrada o salida de un `AudioProcessor` (por ejemplo, la entrada principal
    estéreo y una entrada *sidechain*).

*Callback* de audio
:   Función que el sistema llama periódicamente en el hilo de audio para pedir o entregar un bloque
    de muestras.

Componente
:   Objeto derivado de `higgs::Component`; la unidad básica de la interfaz gráfica.

DAW
:   *Digital Audio Workstation*: aplicación de producción de audio que aloja plug-ins.

Exportador
:   En Mythos Studio, la configuración que genera el proyecto para un IDE concreto (Xcode, Visual
    Studio, Android Studio, Makefile).

Hilo de mensajes
:   El hilo principal que procesa los eventos y en el que deben manipularse los componentes.

Host
:   Aplicación que carga plug-ins (un DAW, el AudioPluginHost o tu propia aplicación).

*Look-and-feel*
:   Objeto que define cómo se dibujan los *widgets*; permite cambiar el aspecto de toda la interfaz.

LV2
:   Formato de plug-ins abierto, muy usado en Linux.

Módulo
:   Carpeta autocontenida `higgs_<área>` con una cabecera principal y un bloque de declaración.
    Véase [Formato de módulos](../compilacion/formato-modulos.md).

`.mythos`
:   Archivo de proyecto de Mythos Studio (formato XML).

MPE
:   *MIDI Polyphonic Expression*: convención MIDI que asigna un canal por nota para permitir
    expresión independiente por nota.

PIP
:   Proyecto completo en un único archivo de cabecera con un bloque de metadatos. Véase
    [Proyectos PIP](../compilacion/pip.md).

SBOM
:   *Software Bill of Materials*: inventario formal del software de terceros incluido
    (`HiggsMythos.spdx.json`).

Standalone
:   Formato que envuelve un plug-in en una aplicación independiente con su propia configuración de
    audio y MIDI.

UMP
:   *Universal MIDI Packet*: formato de paquetes de MIDI 2.0.

VST3
:   Formato de plug-ins de Steinberg, soportado en macOS, Windows y Linux. El SDK está incluido.

`ValueTree`
:   Árbol de datos con propiedades, notificaciones de cambio y soporte de deshacer.

*Wrapper*
:   Código que adapta un `AudioProcessor` a la interfaz de un formato de plug-in concreto.
