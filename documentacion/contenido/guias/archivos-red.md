# Archivos, red y serialización

Todo lo de esta guía está en `higgs_core`.

## Archivos y carpetas

```cpp
auto documentos = higgs::File::getSpecialLocation (higgs::File::userDocumentsDirectory);
auto carpeta    = documentos.getChildFile ("MiApp");
carpeta.createDirectory();

auto notas = carpeta.getChildFile ("notas.txt");
notas.replaceWithText ("Hola desde Higgs Mythos\n");
auto texto = notas.loadFileAsString();

for (const auto& entrada : higgs::RangedDirectoryIterator (carpeta, true, "*.wav"))
    DBG (entrada.getFile().getFileName() << " - " << entrada.getFileSize() << " bytes");
```

Ubicaciones especiales habituales de `File::getSpecialLocation`: `userHomeDirectory`,
`userDocumentsDirectory`, `userDesktopDirectory`, `userMusicDirectory`,
`userApplicationDataDirectory`, `commonApplicationDataDirectory`, `tempDirectory`,
`currentExecutableFile`, `currentApplicationFile`.

Otras clases útiles: `TemporaryFile` (escritura segura a través de un archivo temporal),
`FileSearchPath`, `WildcardFileFilter`, `MemoryMappedFile` y `DirectoryContentsList` (en
`higgs_gui_basics`, para mostrar carpetas en la interfaz).

!!! note "Rutas en Windows (9.0.5)"
    `File::isAbsolutePath()` devuelve ahora `true` para rutas que empiezan por `/`, y
    `File::getChildFile()` resuelve una ruta que empieza por separador sin unidad respecto a la
    unidad del propio archivo, no del directorio de trabajo.

## Flujos de datos

| Clase | Uso |
|---|---|
| `FileInputStream` / `FileOutputStream` | Leer y escribir archivos |
| `MemoryInputStream` / `MemoryOutputStream` | Leer y escribir en memoria |
| `BufferedInputStream` | Añade un búfer a otro flujo |
| `SubregionStream` | Expone una parte de otro flujo |
| `GZIPCompressorOutputStream` / `GZIPDecompressorInputStream` | Compresión zlib/gzip |
| `URL::createInputStream` / `WebInputStream` | Datos remotos |

```cpp
higgs::MemoryOutputStream salida;
salida.writeInt (42);
salida.writeString ("cabecera");
salida.writeFloat (0.75f);

higgs::MemoryInputStream entrada (salida.getData(), salida.getDataSize(), false);
auto n = entrada.readInt();
auto s = entrada.readString();
```

## XML

```cpp
auto xml = higgs::XmlDocument::parse (archivoXml);

if (xml != nullptr && xml->hasTagName ("CONFIG"))
    for (auto* elemento : xml->getChildIterator())
        DBG (elemento->getTagName() << ": " << elemento->getStringAttribute ("valor"));

higgs::XmlElement raiz ("CONFIG");
auto* hijo = raiz.createNewChildElement ("OPCION");
hijo->setAttribute ("valor", 3);
raiz.writeTo (higgs::File ("/ruta/config.xml"));
```

Desde 9.0.5, `XmlDocument` rechaza documentos con más de 256 niveles de anidamiento y ya no carga
subconjuntos DTD externos ni entidades de parámetro externas.

## JSON

```cpp
auto obj = std::make_unique<higgs::DynamicObject>();
obj->setProperty ("nombre", "Higgs Mythos");
obj->setProperty ("version", 9);

higgs::var datos (obj.release());
auto texto = higgs::JSON::toString (datos);

higgs::var leido;
auto resultado = higgs::JSON::parse (texto, leido);
if (resultado.wasOk())
    DBG (leido["nombre"].toString());
```

`JSON::parse()` rechaza documentos con objetos o arrays anidados más de 256 niveles (9.0.5).
`higgs_core` incluye también utilidades para (de)serializar tipos propios a `var`
(`ToVar`, `FromVar`, `SerialisationTraits`).

## Archivos ZIP

```cpp
higgs::ZipFile zip (higgs::File ("/ruta/paquete.zip"));
auto resultado = zip.uncompressTo (higgs::File ("/ruta/destino"));

higgs::ZipFile::Builder constructor;
constructor.addFile (higgs::File ("/ruta/a.txt"), 9);
higgs::FileOutputStream destino (higgs::File ("/ruta/nuevo.zip"));
constructor.writeToStream (destino, nullptr);
```

`ZipFile::uncompressTo()` y `uncompressEntry()` se niegan a crear archivos fuera de la carpeta de
destino (protección contra *path traversal*, 9.0.5).

## Red

### Peticiones HTTP

```cpp
higgs::URL url ("https://api.ejemplo.com/datos");

auto opciones = higgs::URL::InputStreamOptions (higgs::URL::ParameterHandling::inAddress)
                    .withConnectionTimeoutMs (5000)
                    .withExtraHeaders ("Accept: application/json");

if (auto flujo = url.createInputStream (opciones))
{
    auto respuesta = flujo->readEntireStreamAsString();
    auto json = higgs::JSON::parse (respuesta);
}
```

- `URL::withParameter()`, `withPOSTData()` y `withFileToUpload()` construyen peticiones.
- `URL::downloadToFile()` descarga en segundo plano con notificación de progreso
  (`URL::DownloadTask`).
- Las peticiones bloquean el hilo que las hace: ejecútalas fuera del hilo de mensajes.
- En Linux, HTTPS usa **libcurl** (`HIGGS_USE_CURL`, activado por defecto; en CMake añade
  `NEEDS_CURL TRUE`). `HIGGS_LOAD_CURL_SYMBOLS_LAZILY` carga libcurl solo cuando se necesita.

### Sockets

`StreamingSocket` (TCP), `DatagramSocket` (UDP), `IPAddress` y `MACAddress`. Para mensajería de
más alto nivel entre aplicaciones usa `InterprocessConnection` (véase
[Bucle de mensajes](../conceptos/eventos.md#comunicacion-entre-procesos)) u OSC
([Otros módulos](otros-modulos.md#osc)).

## Procesos y sistema

```cpp
higgs::ChildProcess proceso;
if (proceso.start ("git --version"))
    DBG (proceso.readAllProcessOutput());

DBG (higgs::SystemStats::getOperatingSystemName());
DBG (higgs::SystemStats::getNumCpus() << " CPU");
```

`SystemStats` informa del sistema operativo, CPU (y sus extensiones SIMD), memoria y usuario;
`Process` controla la prioridad del proceso y abre documentos o URL con la aplicación por defecto.

## Demos relacionadas

`XMLandJSONDemo`, `NetworkingDemo`, `ChildProcessDemo`, `SystemInfoDemo`, `MultithreadingDemo`.
