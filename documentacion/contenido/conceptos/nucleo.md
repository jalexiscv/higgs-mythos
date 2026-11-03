# Memoria, cadenas y contenedores

`higgs_core` es la base de todos los demás módulos. Aunque puedes usar la biblioteca estándar de
C++ en tu código, el API del framework se expresa con estos tipos, así que conviene conocerlos.

## Texto

| Clase | Uso |
|---|---|
| `String` | Cadena Unicode con recuento de referencias (copias baratas). Internamente UTF-8 por defecto. |
| `StringRef` | Referencia ligera a una cadena literal o a un `String`, para parámetros de funciones. |
| `StringArray` | Lista de cadenas con utilidades para dividir, unir, ordenar y buscar. |
| `StringPairArray` | Pares clave/valor de texto. |
| `Identifier` | Cadena interna (*interned*) de comparación muy rápida, usada como nombre de propiedad en `ValueTree`. |
| `CharPointer_UTF8`, `_UTF16`, `_UTF32`, `_ASCII` | Recorrido y conversión de texto en distintas codificaciones. |
| `LocalisedStrings` | Traducción de las cadenas de la interfaz (véase [Localización](../guias/localizacion.md)). |

```cpp
higgs::String s ("Frecuencia: ");
s << 440.0 << " Hz";

auto partes = higgs::StringArray::fromTokens ("a,b,c", ",", "");
auto mayus  = s.toUpperCase();
auto numero = higgs::String ("3.25").getFloatValue();
```

## Contenedores

| Clase | Equivalente aproximado | Notas |
|---|---|---|
| `Array<T>` | `std::vector<T>` | Para tipos copiables o movibles; parámetro opcional de sección crítica para uso concurrente. |
| `OwnedArray<T>` | `std::vector<std::unique_ptr<T>>` | Posee y destruye los objetos que contiene. |
| `ReferenceCountedArray<T>` | `std::vector<Ptr>` | Para objetos derivados de `ReferenceCountedObject`. |
| `SortedSet<T>` | `std::set<T>` sobre un vector | Valores únicos ordenados. |
| `SparseSet<T>` | — | Conjuntos de valores almacenados como rangos. |
| `HashMap<K, V>` | `std::unordered_map` | Tabla hash sencilla. |
| `NamedValueSet` | — | Mapa `Identifier` → `var`. |
| `Span<T>` | `std::span` | Vista no propietaria sobre memoria contigua. |
| `Optional<T>` | `std::optional` | Tipo opcional. |
| `AbstractFifo` | — | Lógica de FIFO sin bloqueos (véase [Hilos](hilos.md)). |

## El tipo dinámico `var`

`var` puede contener `void`, `bool`, `int`, `int64`, `double`, `String`, un array de `var`, un
`DynamicObject`, un bloque binario o un método. Es el tipo que usan `JSON`, el intérprete de
JavaScript y las propiedades de `ValueTree`:

```cpp
higgs::var datos = higgs::JSON::parse (R"({ "nombre": "Higgs", "version": 9 })");
auto nombre = datos["nombre"].toString();
auto version = (int) datos["version"];
```

## Gestión de memoria

| Clase / macro | Para qué sirve |
|---|---|
| `ReferenceCountedObject` + `ReferenceCountedObject::Ptr` | Objetos compartidos con recuento intrusivo de referencias. |
| `WeakReference<T>` + `HIGGS_DECLARE_WEAK_REFERENCEABLE` | Punteros que se vuelven nulos al destruirse el objeto. |
| `HeapBlock<T>` | Bloque de memoria en el *heap* sin copia, con alineación opcional. |
| `MemoryBlock` | Bloque binario redimensionable con utilidades (base64, hex…). |
| `SharedResourcePointer<T>` | Instancia compartida que vive mientras exista algún puntero a ella. |
| `HIGGS_DECLARE_SINGLETON` | Implementa el patrón *singleton* con control de destrucción. |
| `HIGGS_LEAK_DETECTOR (Clase)` | Detecta instancias no destruidas al cerrar (en depuración). |
| `ScopedValueSetter<T>` | Cambia temporalmente un valor y lo restaura al salir del ámbito. |

!!! tip "Prefiere la propiedad explícita"
    En código nuevo usa `std::unique_ptr` y miembros por valor siempre que sea posible.
    `ScopedPointer` está obsoleto y se conserva solo por compatibilidad.

## Archivos, flujos y formatos

`higgs_core` también incluye:

- **Archivos**: `File`, `DirectoryIterator`/`RangedDirectoryIterator`, `FileSearchPath`,
  `TemporaryFile`, `MemoryMappedFile`, `FileFilter`.
- **Flujos**: `InputStream`, `OutputStream`, `FileInputStream`, `FileOutputStream`,
  `MemoryInputStream`, `MemoryOutputStream`, `BufferedInputStream`, `SubregionStream`.
- **Compresión**: `GZIPCompressorOutputStream`, `GZIPDecompressorInputStream`, `ZipFile`.
- **XML y JSON**: `XmlDocument`, `XmlElement`, `JSON`.
- **Red**: `URL`, `WebInputStream`, `StreamingSocket`, `DatagramSocket`, `IPAddress`,
  `MACAddress`.
- **Tiempo**: `Time`, `RelativeTime`, `PerformanceCounter`.
- **Matemáticas**: `Range`, `NormalisableRange`, `Random`, `BigInteger`, `Expression`,
  `StatisticsAccumulator`, `MathConstants`.
- **Sistema**: `SystemStats`, `Process`, `ChildProcess`, `DynamicLibrary`, `Uuid`.

La guía [Archivos, red y serialización](../guias/archivos-red.md) muestra cómo usarlos.
