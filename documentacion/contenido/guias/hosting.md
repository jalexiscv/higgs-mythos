# Alojar plug-ins (hosting)

Además de crear plug-ins, Higgs Mythos permite **cargarlos**: escanear los instalados en el sistema,
instanciarlos, mostrar sus editores y conectarlos en un grafo de procesamiento. El
[AudioPluginHost](../herramientas/audio-plugin-host.md) de `extras` es una aplicación completa
construida con estas clases.

## Activar los formatos que quieres alojar

Los formatos se activan con opciones del módulo `higgs_audio_processors_headless` (todas `0` por
defecto):

| Opción | Formato | Requisitos |
|---|---|---|
| `HIGGS_PLUGINHOST_VST3` | VST3 | SDK incluido |
| `HIGGS_PLUGINHOST_AU` | Audio Unit | Solo macOS/iOS; en CMake usa `PLUGINHOST_AU TRUE` en el target |
| `HIGGS_PLUGINHOST_LV2` | LV2 | Bibliotecas LV2 incluidas |
| `HIGGS_PLUGINHOST_LADSPA` | LADSPA | Solo Linux; paquete `ladspa-sdk` |
| `HIGGS_PLUGINHOST_VST` | VST2 heredado | Cabeceras del SDK de VST2 en la ruta de búsqueda |
| `HIGGS_PLUGINHOST_ARA` | Extensiones ARA | SDK de ARA (`higgs_set_ara_sdk_path`) |

```cmake
higgs_add_gui_app(MiHost PRODUCT_NAME "Mi Host" PLUGINHOST_AU TRUE)

target_compile_definitions(MiHost PRIVATE
    HIGGS_PLUGINHOST_VST3=1
    HIGGS_PLUGINHOST_LV2=1)

target_link_libraries(MiHost PRIVATE higgs::higgs_audio_utils)
```

## Gestor de formatos

```cpp
higgs::AudioPluginFormatManager formatos;
higgs::addDefaultFormatsToManager (formatos);           // con soporte de interfaz
// o, en herramientas de consola sin interfaz:
// higgs::addHeadlessDefaultFormatsToManager (formatos);
```

!!! note "`addDefaultFormats()` ya no existe"
    El antiguo método `AudioPluginFormatManager::addDefaultFormats()` está eliminado; usa las
    funciones libres `addDefaultFormatsToManager()` (módulo `higgs_audio_processors`) o
    `addHeadlessDefaultFormatsToManager()` (módulo `higgs_audio_processors_headless`).

## Escanear plug-ins

`KnownPluginList` mantiene la lista de plug-ins conocidos (`PluginDescription`) y se puede guardar
y restaurar como XML. Para escanear:

- `PluginDirectoryScanner` recorre carpetas de búsqueda y añade cada plug-in válido a la lista.
- `PluginListComponent` es un componente listo para mostrar la lista y lanzar el escaneo.
- Para hacer el escaneo en un proceso aparte —de modo que un plug-in defectuoso no cierre tu
  aplicación— implementa `KnownPluginList::CustomScanner` (el AudioPluginHost lo hace).

```cpp
higgs::KnownPluginList lista;
higgs::PluginListComponent panel (formatos, lista,
                                  higgs::File(),   // archivo de «plug-ins muertos» (opcional)
                                  nullptr);       // PropertiesFile para recordar rutas (opcional)
```

## Crear una instancia

```cpp
formatos.createPluginInstanceAsync (descripcion, 48000.0, 512,
    [this] (std::unique_ptr<higgs::AudioPluginInstance> instancia, const higgs::String& error)
    {
        if (instancia == nullptr)
        {
            DBG ("No se pudo cargar: " << error);
            return;
        }

        plugin = std::move (instancia);
        plugin->prepareToPlay (48000.0, 512);

        if (plugin->hasEditor())
            ventanaEditor.setContentOwned (plugin->createEditorIfNeeded(), true);
    });
```

Las instancias son `AudioProcessor` normales: puedes llamar a `processBlock()`, leer sus
parámetros (`getParameters()`), guardar su estado (`getStateInformation()`) y mostrar su editor.
Usa preferentemente la versión asíncrona: algunos formatos (AUv3, por ejemplo) solo admiten
creación asíncrona.

## El grafo de procesamiento

`AudioProcessorGraph` es a su vez un `AudioProcessor` que contiene nodos (procesadores) y
conexiones de audio y MIDI entre ellos:

```cpp
using Grafo = higgs::AudioProcessorGraph;
using IO    = Grafo::AudioGraphIOProcessor;

auto grafo = std::make_unique<Grafo>();

auto entrada = grafo->addNode (std::make_unique<IO> (IO::audioInputNode));
auto salida  = grafo->addNode (std::make_unique<IO> (IO::audioOutputNode));
auto efecto  = grafo->addNode (std::move (plugin));

for (int canal = 0; canal < 2; ++canal)
{
    grafo->addConnection ({ { entrada->nodeID, canal }, { efecto->nodeID, canal } });
    grafo->addConnection ({ { efecto->nodeID, canal },  { salida->nodeID, canal } });
}

// Reproducirlo a través del dispositivo de audio:
reproductor.setProcessor (grafo.get());          // higgs::AudioProcessorPlayer
gestorDispositivos.addAudioCallback (&reproductor);
```

Para conexiones MIDI usa el índice especial `Grafo::midiChannelIndex`. También existen nodos de
entrada/salida MIDI (`IO::midiInputNode`, `IO::midiOutputNode`).

## Demos relacionadas

`HostPluginDemo` (un plug-in que aloja otros plug-ins) y la aplicación
[AudioPluginHost](../herramientas/audio-plugin-host.md).
