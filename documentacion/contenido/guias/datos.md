# Modelo de datos y deshacer

El módulo `higgs_data_structures` proporciona estructuras de datos «inteligentes» que notifican
sus cambios, se serializan con facilidad y se integran con el sistema de deshacer/rehacer.

## `ValueTree`

Un `ValueTree` es un árbol de nodos; cada nodo tiene un **tipo** (`Identifier`), un conjunto de
**propiedades** (`Identifier` → `var`) y una lista de **hijos**. Es la estructura ideal para el
modelo de documento de una aplicación o el estado de un plug-in.

```cpp
namespace IDs
{
    static const higgs::Identifier proyecto  { "PROYECTO" };
    static const higgs::Identifier pista     { "PISTA" };
    static const higgs::Identifier nombre    { "nombre" };
    static const higgs::Identifier volumen   { "volumen" };
}

higgs::ValueTree proyecto (IDs::proyecto);
proyecto.setProperty (IDs::nombre, "Mi canción", nullptr);

higgs::ValueTree pista (IDs::pista);
pista.setProperty (IDs::nombre, "Batería", nullptr)
     .setProperty (IDs::volumen, 0.8, nullptr);
proyecto.appendChild (pista, nullptr);

for (auto hijo : proyecto)
    DBG (hijo[IDs::nombre].toString());
```

Características clave:

- **Semántica de referencia**: copiar un `ValueTree` crea otra referencia al mismo nodo; usa
  `createCopy()` para una copia profunda.
- **Notificaciones**: `ValueTree::Listener` recibe `valueTreePropertyChanged`,
  `valueTreeChildAdded`, `valueTreeChildRemoved`, `valueTreeChildOrderChanged` y
  `valueTreeParentChanged`.
- **Serialización**: `createXml()` / `fromXml()`, `toXmlString()`, `writeToStream()` /
  `readFromStream()` (binario) y `readFromGZIPData()`.
- **Deshacer**: todos los métodos que modifican el árbol aceptan un `UndoManager*`; pasa
  `nullptr` si no quieres registrar la operación.
- `CachedValue<T>` mantiene en caché el valor tipado de una propiedad y lo actualiza al cambiar.
- `ValueTreeSynchroniser` replica un árbol en otro (por ejemplo, entre procesos).

!!! warning "Límites de anidamiento (9.0.5)"
    `ValueTree::readFromStream()`, `readFromData()` y `readFromGZIPData()` devuelven un árbol
    inválido si los datos tienen más de 256 niveles de anidamiento, como protección frente a datos
    maliciosos.

## `Value`

`Value` es un contenedor de un `var` compartido y observable. Muchos componentes pueden enlazarse
directamente a un `Value`:

```cpp
higgs::Value frecuencia (440.0);
slider.getValueObject().referTo (frecuencia);
etiqueta.getTextValue().referTo (frecuencia);   // ambos se actualizan mutuamente
```

`ValueTree::getPropertyAsValue()` devuelve un `Value` enlazado a una propiedad del árbol.

## Deshacer y rehacer

`UndoManager` guarda una pila de `UndoableAction` agrupadas en transacciones:

```cpp
higgs::UndoManager deshacer;

deshacer.beginNewTransaction ("Cambiar volumen");
pista.setProperty (IDs::volumen, 0.5, &deshacer);

deshacer.undo();   // volumen vuelve a 0.8
deshacer.redo();   // volumen pasa a 0.5
```

Puedes crear tus propias acciones derivando de `UndoableAction` e implementando `perform()` y
`undo()`. `canUndo()`, `getUndoDescription()` y la notificación de `ChangeBroadcaster` permiten
mantener actualizados los menús.

## Preferencias de la aplicación

`ApplicationProperties` gestiona archivos `PropertiesFile` de usuario y comunes en la ubicación
estándar de cada sistema operativo:

```cpp
higgs::ApplicationProperties preferencias;

higgs::PropertiesFile::Options opciones;
opciones.applicationName     = "MiApp";
opciones.filenameSuffix      = ".settings";
opciones.osxLibrarySubFolder = "Application Support";
preferencias.setStorageParameters (opciones);

auto* ajustes = preferencias.getUserSettings();
ajustes->setValue ("ultimoArchivo", archivo.getFullPathName());
auto tema = ajustes->getValue ("tema", "oscuro");
```

## Demos relacionadas

`ValueTreesDemo` (árbol editable con deshacer) y, en plug-ins, cualquier ejemplo que use
`AudioProcessorValueTreeState`.
