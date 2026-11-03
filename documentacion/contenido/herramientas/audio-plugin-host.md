# AudioPluginHost

**AudioPluginHost** (`extras/AudioPluginHost`) es una aplicación de escritorio para cargar
plug-ins, conectarlos en un grafo y escucharlos a través del dispositivo de audio. Es la forma más
cómoda de probar tus plug-ins durante el desarrollo sin abrir un DAW, y su código es a la vez un
ejemplo completo de las clases de [hosting](../guias/hosting.md).

## Compilarlo

```bash
cmake . -B cmake-build -DHIGGS_BUILD_EXTRAS=ON
cmake --build cmake-build --target AudioPluginHost
```

También hay proyectos de Mythos Studio en `extras/AudioPluginHost/Builds` y el proyecto
`AudioPluginHost.mythos`. Formatos que puede alojar: VST3, AU (macOS), LV2, LADSPA (Linux) y, si se
configura el SDK correspondiente, VST2 y extensiones ARA.

## Funciones

- **Grafo de plug-ins**: nodos de entrada y salida de audio y MIDI, plug-ins conectados con cables
  que se arrastran entre sus puertos. Con el botón derecho sobre un nodo se accede a su editor, a
  su interfaz genérica de parámetros, a la configuración de buses y a opciones de ARA.
- **Plug-ins internos**: generador de onda senoidal y reverberación, útiles para pruebas rápidas.
- **Lista de plug-ins disponibles** (*Edit the List of Available Plug-ins...*): escaneo de las
  carpetas del sistema, en el propio proceso (*In-process*) o en un proceso separado
  (*Out-of-process*), de modo que un plug-in defectuoso no cierre la aplicación. Los plug-ins que
  fallaron recientemente se recuerdan.
- **Menú de creación** ordenable por categoría, fabricante, estructura de carpetas u orden
  alfabético.
- **Archivos de grafo** (`.filtergraph`): *New*, *Open...*, *Save*, *Save As...* y archivos
  recientes; el estado se guarda automáticamente entre sesiones.
- **Ajustes de audio** (*Change the Audio Device Settings*), renderizado en **doble precisión** y
  escalado automático de las ventanas de plug-in.

## Activar ARA

Véase [ARA](../guias/ara.md#hosts-con-ara-el-audiopluginhost): define `HIGGS_PLUGINHOST_ARA=1` en
`extras/AudioPluginHost/CMakeLists.txt` y configura el SDK de ARA. Los plug-ins compatibles
aparecerán dos veces en el menú de creación, una marcada como **(ARA)**.
