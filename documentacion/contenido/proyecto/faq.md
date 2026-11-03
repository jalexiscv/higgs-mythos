# Preguntas frecuentes

??? question "¿Puedo usar Higgs Mythos en un producto comercial?"
    El framework se distribuye bajo **AGPL-3.0-or-later**. Puedes usarlo en cualquier proyecto que
    cumpla esa licencia, lo que en general implica publicar el código fuente completo de tu
    aplicación bajo la misma licencia cuando la distribuyas o la ofrezcas por red. Revisa también
    las licencias de los componentes de terceros que uses. Véase [Licencia](licencia.md).

??? question "¿CMake o Mythos Studio?"
    Para proyectos nuevos de escritorio y plug-ins, **CMake** es la opción más flexible e
    integrable con otras herramientas. **Mythos Studio** es imprescindible para Android (CMake no lo
    admite) y cómodo si prefieres una interfaz gráfica para configurar módulos y exportadores.

??? question "CMake dice «A C compiler is required to build targets that depend on HIGGS»."
    Tu `project()` solo habilita C++. Declara `LANGUAGES C CXX` (o no indiques `LANGUAGES`): algunas
    dependencias incluidas se compilan como C.

??? question "Tengo errores de enlace con curl, WebKit o WebView2."
    Esas dependencias no se enlazan por defecto. Añade `NEEDS_CURL TRUE`, `NEEDS_WEB_BROWSER TRUE`
    o `NEEDS_WEBVIEW2 TRUE` a tu `higgs_add_*`, o desactiva la función con `HIGGS_USE_CURL=0` /
    `HIGGS_WEB_BROWSER=0` si no la necesitas.

??? question "Al enlazar un módulo con `PUBLIC` obtengo símbolos duplicados."
    Los módulos son bibliotecas `INTERFACE` cuyas fuentes se compilan en cada target que las
    enlaza: deben enlazarse siempre con visibilidad **`PRIVATE`**. Los targets de opciones
    (`higgs_recommended_*_flags`) sí pueden ir como `PUBLIC`.

??? question "Mi plug-in no aparece en el DAW."
    Comprueba que el formato está en `FORMATS`, que el bundle se ha copiado a la carpeta correcta
    (`COPY_PLUGIN_AFTER_BUILD TRUE` o copia manual), que `PLUGIN_MANUFACTURER_CODE` y `PLUGIN_CODE`
    cumplen las reglas de mayúsculas y son fijos, y vuelve a escanear en el host. En macOS, los AU
    pueden requerir reiniciar el host o ejecutar `killall -9 AudioComponentRegistrar`. Prueba
    primero en el [AudioPluginHost](../herramientas/audio-plugin-host.md).

??? question "¿Por qué el host pierde la automatización tras actualizar mi plug-in?"
    Probablemente cambiaste el identificador (`ParameterID`) de algún parámetro, su orden en un
    formato que usa índices, o alguna opción de compatibilidad (`HIGGS_FORCE_USE_LEGACY_PARAM_IDS`,
    `HIGGS_VST3_CAN_REPLACE_VST2`…). Los identificadores publicados no deben cambiar nunca.

??? question "¿Puedo llamar a métodos de la interfaz desde `processBlock()`?"
    No. `processBlock()` se ejecuta en el hilo de audio. Comunica datos a la interfaz mediante
    variables atómicas o colas sin bloqueos y léelos desde un `Timer` en el editor. Véase
    [Hilos y concurrencia](../conceptos/hilos.md#reglas-del-hilo-de-audio).

??? question "¿Hay referencia del API método a método?"
    Las [fichas de módulos](../modulos/index.md) incluyen el catálogo de clases con enlaces a sus
    cabeceras, donde cada método está documentado. También puedes generar la referencia Doxygen
    completa en HTML desde `docs/doxygen` (véase
    [Depuración y diagnóstico](../conceptos/depuracion.md#referencia-del-api-generada-con-doxygen)).

??? question "¿Qué versión de C++ necesito?"
    C++17 como mínimo; C++20 si activas Windows MIDI Services. El framework también compila con
    C++20 y C++23.

??? question "¿Dónde pregunto o informo de un error?"
    Preguntas y propuestas en [Discussions](https://github.com/jalexiscv/higgs-mythos/discussions);
    errores en [Issues](https://github.com/jalexiscv/higgs-mythos/issues); vulnerabilidades por el
    canal privado descrito en [Seguridad](seguridad.md).
