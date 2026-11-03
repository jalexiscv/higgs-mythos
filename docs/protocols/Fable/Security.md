# Protocolo de Seguridad y Manejo de Datos

Este documento define las reglas de seguridad que todo cambio de código debe
cumplir en **Higgs Mythos**, y la disciplina para operar sin destruir el trabajo
del usuario. Higgs Mythos es un framework: el código que escribimos corre dentro
de aplicaciones y plug-ins de terceros, en escritorio, móvil y dentro de hosts de
audio. Su superficie de ataque son los **datos que decodifica** (ZIP, XML, JSON,
`ValueTree`, imágenes PNG/GIF/JPEG, audio WAV/AIFF/FLAC/OGG/MP3, MIDI y MIDI-CI,
OSC, fuentes, JavaScript), los **binarios que carga** (plug-ins en un host), lo que
recibe por **red** (`URL`, `StreamingSocket`, `WebBrowserComponent`) y lo que
**escribe en disco**. No es una guía de auditoría externa: es la checklist que el
propio agente aplica **en cada cambio** que toque la lectura o escritura de datos,
la memoria, los hilos o el origen del código. Las vulnerabilidades encontradas se
reportan por el canal privado de [SECURITY.md](../../../SECURITY.md), nunca en un
issue público ni en un changelog antes de corregirse.

---

## 1. Principio Rector

**Todo dato que entra es hostil hasta que se valida; ningún archivo, mensaje ni
plug-in ajeno puede agotar la memoria, colgar el proceso ni escribir fuera de su
sitio; un fallo del framework nunca tumba la aplicación o el host que lo aloja.**
La seguridad no es una fase: es una propiedad de cada línea que cruza una frontera
(bytes → objeto, objeto → bytes, red → memoria, hilo → hilo, código ajeno →
repositorio).

---

## 2. Decodificación de Datos

1. **Límites antes de procesar.** Todo lector respeta los límites existentes y
   añade los suyos cuando procesa algo nuevo: profundidad máxima de anidamiento
   en `ValueTree`, `var`/JSON y XML (256 niveles, ver `BREAKING_CHANGES.md`
   9.0.5), tamaño y píxeles máximos de imagen, número de canales, frecuencia de
   muestreo y longitud declarada de los archivos de audio, tamaño de los
   mensajes OSC y de los `SysEx`. Un valor numérico leído del archivo que
   controla un bucle, una reserva de memoria (`MemoryBlock`, `Array`,
   `AudioBuffer`) o un índice se acota **antes** de usarse.
2. **XML sin entidades externas ni DOCTYPE peligrosos.** `XmlDocument` no
   resuelve entidades externas; ningún lector nuevo introduce un analizador
   propio que las resuelva.
3. **Rutas dentro de paquetes.** Los nombres de las entradas de un `ZipFile` y
   las rutas que vienen de un archivo o de la red son datos hostiles: nunca se
   usan para construir una ruta del disco sin comprobar que el resultado queda
   dentro del directorio destino (`File::isAChildOf`), y nunca se siguen `..`
   ni rutas absolutas.
4. **Nada se ejecuta ni se descarga a ciegas.** El framework no abre una `URL`
   ni lanza un `ChildProcess` con datos que vengan de un archivo o de un mensaje
   sin que la aplicación lo haya pedido explícitamente. `JavascriptEngine`
   ejecuta solo lo que la aplicación le entrega, con límites de tiempo y de
   recursión.
5. **Formatos por contenido.** El tipo de una imagen o de un archivo de audio se
   decide por sus bytes (`ImageFileFormat::findImageFormatForStream`,
   `AudioFormatManager::createReaderFor`), no por la extensión ni por el tipo
   declarado.
6. **Lo que no se entiende se rechaza o se ignora con criterio,** nunca se
   interpreta a ciegas: un chunk desconocido de WAV o AIFF se salta por su
   longitud declarada y acotada; un mensaje MIDI mal formado se descarta; un
   `var` de tipo inesperado se trata como vacío. El fallo se comunica con
   `Result`, un puntero nulo documentado o un callback de error, y la aplicación
   sigue viva.

---

## 3. Memoria, Hilos y Plug-ins

1. **Sin `new`/`delete` manuales;** dueños claros con `std::unique_ptr`,
   `OwnedArray`, `ReferenceCountedObjectPtr` o contenedores. `HIGGS_LEAK_DETECTOR`
   en las clases con tiempo de vida no trivial, como sus vecinas.
2. **Índices validados.** Todo índice que viene de fuera (canal, muestra, hijo
   de `ValueTree`, entrada de ZIP, posición en un `MidiBuffer`) se comprueba
   contra el tamaño real antes de acceder; `jassert` documenta el invariante,
   pero no sustituye la comprobación en Release.
3. **Texto íntegro.** `String` es UTF-8 interno; las conversiones pasan por los
   `CharPointer_*`, que toleran secuencias inválidas. Ningún cálculo de longitud
   mezcla bytes con puntos de código.
4. **Recursión acotada.** Las estructuras anidadas (`ValueTree`, `var`, XML,
   árboles de `Component`, grupos de formas en SVG) se recorren con profundidad
   limitada o de forma iterativa.
5. **Hilo de audio.** Dentro de `processBlock` y de los callbacks de
   `AudioIODeviceCallback` no se reserva memoria, no se bloquea
   (`CriticalSection`, E/S, `MessageManagerLock`) ni se llama a nada que pueda
   hacerlo. El estado se comparte con `std::atomic`, `AbstractFifo` o copias
   preparadas fuera del callback.
6. **Plug-ins ajenos en un host.** Un plug-in cargado por `AudioPluginFormatManager`
   es código no confiable: el escaneo se hace fuera de proceso cuando el host lo
   soporta (`PluginDirectoryScanner` con `KnownPluginList`), los fallos de
   carga se reportan sin tumbar el host, y lo que el plug-in devuelve (nombres,
   tamaños de editor, número de buses) se valida como cualquier otro dato
   externo.
7. **Expresiones y entradas del usuario final.** Lo que el usuario escribe en un
   `TextEditor`, en un `FileChooser` o en la línea de órdenes de `higgsaide` se
   valida antes de usarse como ruta, número o expresión; una entrada inválida
   produce un mensaje, no un cierre.

---

## 4. Escritura de Archivos y Datos del Usuario

1. **Escritura atómica.** Los archivos que el framework escribe por cuenta de la
   aplicación (`PropertiesFile`, proyectos `.mythos`, `ValueTree` serializados)
   pasan por `TemporaryFile` y `overwriteTargetFileWithTemporary`; un fallo a
   mitad de la escritura nunca deja el archivo del usuario truncado.
2. **Nunca perder trabajo.** Los proyectos de Mythos Studio y las preferencias
   de las herramientas se guardan con el ciclo completo probado (guardar,
   cerrar, recargar) cuando un cambio los toca.
3. **Ida y vuelta sin pérdidas.** Un cambio en un escritor no puede descartar
   información que el lector importó (metadatos de audio, propiedades
   desconocidas de un `ValueTree`, ajustes de un proyecto). Lo que un formato no
   puede representar se declara en la documentación.
4. **Operaciones destructivas en el repositorio o en el disco** (borrar árboles,
   `git reset --hard`, reescribir historia, sobrescribir archivos que no creó el
   agente): requieren mirar antes lo que se destruye, respaldo o confirmación
   explícita del usuario, y quedan registradas en el changelog.
5. **Los archivos temporales de las pruebas** se crean con `TemporaryFile` o en
   `File::getSpecialLocation (File::tempDirectory)` y se borran al terminar; los
   del agente, en el scratchpad del entorno, nunca en el árbol del proyecto.

---

## 5. Origen del Código y Licencia

1. **Regla de origen** ([contributing.md](../../../.github/contributing.md)): todo
   el código se escribe desde cero o se toma de fuentes públicas con licencia
   compatible con la AGPL-3.0-or-later. **Está prohibido** incorporar código,
   constantes, tablas o estructuras copiados o traducidos de binarios
   propietarios (hosts de audio, sistemas operativos, SDKs cerrados, otros
   frameworks sin licencia compatible), aunque el análisis lo haga una
   herramienta automática. Observar el comportamiento visible de un host o de
   un sistema para comparar sí está permitido.
2. **SDKs de plug-ins.** Los SDKs de VST3, AU, AAX, ARA y LV2 se usan según sus
   propias licencias y acuerdos. Lo que no es redistribuible (AAX, ARA) no se
   copia al repositorio: se referencia por ruta en CMake como ya hace el
   proyecto, y la documentación dice de dónde obtenerlo.
3. **Licencia compatible.** Higgs Mythos se distribuye bajo AGPL-3.0-or-later
   (`LICENSE.md`), así que todo código o dato de terceros que entre en
   `modules/` tiene una licencia compatible (MIT, BSD, ISC, Apache-2.0, zlib,
   BSL-1.0, LGPL, GPL-3.0; OFL para fuentes; CC0 o CC BY para datos), se cita
   su procedencia en el commit y se añade al inventario `HiggsMythos.spdx.json`.
   Los ejemplos de `examples/` se publican bajo ISC y no incorporan código
   copyleft.
4. **Cabeceras de licencia.** Cada archivo nuevo de `modules/` y `extras/` lleva
   el bloque de licencia AGPL del archivo vecino. Los avisos de copyright de los
   archivos existentes **se conservan intactos**: ni se borran, ni se cambian de
   titular, ni se les añade un autor
   ([higgs-mythos.md](../../higgs-mythos.md) §"Licencia y atribución",
   [git-authorship.md](../../../.claude/rules/git-authorship.md)).
5. **Marcas.** Los nombres de terceros (Steinberg, Apple, Avid, Microsoft, los
   hosts) se mencionan solo para describir compatibilidad, como hace el
   `README.md`. La marca retirada de la base original no reaparece en ninguna
   forma.

---

## 6. Secretos y Datos Personales

1. **Nunca se codifican en el código fuente** credenciales, tokens, certificados
   de firma (Apple, Windows, PACE) ni claves privadas de
   `higgs_product_unlocking` o de `higgs_cryptography` (`RSAKey`): viven en la
   configuración de la integración continua o en el llavero del desarrollador.
   Las claves públicas que el código legítimamente embebe se marcan como tales.
2. **Nunca se escriben secretos** en el código, en los registros, en el
   changelog, en la documentación ni en los reportes del agente. Si un secreto
   aparece accidentalmente en cualquiera de esos lugares, se reporta de inmediato
   para rotarlo: borrarlo no lo des-expone.
3. **Los archivos del usuario son datos personales.** Audio, imágenes, proyectos
   y preferencias reales no se copian a pruebas, ejemplos ni documentación; las
   pruebas usan datos sintéticos generados en la propia prueba o los recursos
   de `examples/Assets`, y los archivos reales de diagnóstico se quedan fuera
   del repositorio.
4. **Telemetría y red.** `higgs_analytics` y cualquier envío a la red se activan
   solo por decisión explícita de la aplicación; el framework no envía nada por
   defecto y nunca incluye identificadores personales sin que la aplicación los
   aporte a sabiendas.
5. **Los mensajes de error al usuario no revelan internals** innecesarios: dicen
   qué no se pudo hacer y por qué en términos del usuario; el detalle técnico va
   al `Logger` o a la terminal.

---

## 7. Checklist de Seguridad por Cambio

Al terminar un cambio, y como parte de la autorrevisión
([Review.md](Review.md) §2.6), se responde:

1. ¿Todo dato nuevo leído de un archivo, un mensaje o un plug-in se valida y se
   acota antes de controlar un bucle, una reserva de memoria o un índice?
2. ¿Ningún lector nuevo recorre estructuras anidadas sin límite de profundidad?
3. ¿Ningún nombre de entrada, ruta o URL de origen externo se convierte en ruta
   del disco sin comprobar que queda dentro del destino?
4. ¿Nada externo se ejecuta, se descarga ni abre una URL sin que la aplicación
   lo haya pedido?
5. ¿El código nuevo del hilo de audio no reserva memoria ni bloquea, y el estado
   compartido entre hilos está protegido?
6. ¿Toda escritura de archivos del usuario sigue siendo atómica y la ida y vuelta
   no pierde datos?
7. ¿Todo el código nuevo tiene origen público y licencia compatible con la AGPL,
   está en el SBOM si es de terceros, y las cabeceras de copyright quedaron
   intactas?
8. ¿Ningún secreto, certificado ni archivo real del usuario quedó en el código,
   las pruebas, el changelog o el reporte?

Un "no" en cualquier punto bloquea la entrega igual que una prueba fallida.

---

## 8. Anti-patrones Prohibidos

| Anti-patrón                                                       | Conducta correcta                                             |
|--------------------------------------------------------------------|----------------------------------------------------------------|
| Confiar en un tamaño, un conteo o un índice leído de fuera        | Validar y acotar antes de usar (§2.1, §3.2)                    |
| Recorrer `ValueTree`, `var` o XML sin límite de profundidad       | Profundidad acotada o recorrido iterativo (§3.4)               |
| Construir una ruta del disco con un nombre de un ZIP o de la red  | Comprobar que queda dentro del destino (§2.3)                  |
| Decidir el formato de un archivo por su extensión                 | Probar los bytes (§2.5)                                        |
| Reservar memoria o tomar un `CriticalSection` en `processBlock`   | Estado preparado fuera del callback; `std::atomic`/`AbstractFifo` (§3.5) |
| Escribir el archivo del usuario directamente sobre el destino     | `TemporaryFile` y reemplazo atómico (§4.1)                     |
| Copiar código o constantes de un binario o SDK sin licencia       | Solo fuentes públicas y compatibles con la AGPL (§5.1–5.3)     |
| Añadir un tercero sin entrada en `HiggsMythos.spdx.json`          | SBOM actualizado en el mismo cambio (§5.3)                     |
| Alterar el aviso de copyright de un archivo existente             | Conservarlo intacto (§5.4)                                     |
| Usar audio o proyectos reales del usuario como dato de prueba     | Datos sintéticos o `examples/Assets` (§6.3)                    |
| Credenciales o certificados en el código o en los registros       | Configuración de la CI; reporte inmediato si se expone (§6)    |
| Publicar una vulnerabilidad en un issue o un changelog antes de corregirla | Canal privado de `SECURITY.md`                        |

---

## 9. Relación con Otros Protocolos

* El canal de reporte de vulnerabilidades: [SECURITY.md](../../../SECURITY.md).
* La regla de origen y el idioma de los commits:
  [contributing.md](../../../.github/contributing.md).
* La autoría y las cabeceras: [git-authorship.md](../../../.claude/rules/git-authorship.md)
  y [docs/higgs-mythos.md](../../higgs-mythos.md).
* La autorrevisión donde esta checklist se ejecuta: [Review.md](Review.md).
* Las operaciones destructivas y su registro: [Execution.md](Execution.md) §10 y
  [Changelogs.md](../Changelogs.md).

*Este documento es la referencia de seguridad por cambio; la regla que lo resume
es: desconfía de lo que decodificas, acota lo que procesas, no bloquees el hilo de
audio, no pierdas lo que guardas y escribe solo código de origen limpio.*
