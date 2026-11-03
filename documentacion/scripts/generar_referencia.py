#!/usr/bin/env python3
"""Genera las páginas de referencia de la documentación a partir del código fuente.

Uso:
    python3 documentacion/scripts/generar_referencia.py [--comprobar]

Lee directamente del repositorio:

  * el bloque BEGIN_HIGGS_MODULE_DECLARATION de cada módulo,
  * las opciones de configuración (comentarios "Config:") de sus cabeceras principales,
  * las clases públicas declaradas en las cabeceras que incluye cada módulo, con la primera
    frase de su comentario Doxygen,
  * los metadatos BEGIN_HIGGS_PIP_METADATA de los ejemplos,
  * el manifiesto de dependencias de terceros (extras/SBOM/dependencies.json),

y escribe:

  * contenido/modulos/index.md y contenido/modulos/<módulo>.md
  * contenido/compilacion/opciones.md
  * contenido/ejemplos/index.md

Las descripciones en español se toman de datos/traducciones.json y datos/modulos.json. Cuando el
código contiene un texto que aún no tiene traducción, se usa el original en inglés y el script lo
indica al terminar, para que se pueda completar el archivo de traducciones.

Con --comprobar no escribe nada: termina con error si alguna página generada no coincide con la
que hay en disco (útil en integración continua).
"""

import argparse
import json
import os
import re
import sys
from pathlib import Path

RAIZ_DOCS = Path(__file__).resolve().parent.parent
RAIZ_REPO = RAIZ_DOCS.parent
CONTENIDO = RAIZ_DOCS / "contenido"
DATOS = RAIZ_DOCS / "datos"
MODULOS = RAIZ_REPO / "modules"
EJEMPLOS = RAIZ_REPO / "examples"
URL_FUENTE = "https://github.com/jalexiscv/higgs-mythos/blob/main/"

# Valores por defecto que dependen de la plataforma o de otras opciones y que no se pueden leer
# de una única línea "#define".
DEFECTOS_ESPECIALES = {
    "HIGGS_USE_ANDROID_OPENSLES": "1 si `HIGGS_USE_ANDROID_OBOE` es 0; si no, 0",
    "HIGGS_FORCE_DEBUG": "sin definir",
    "HIGGS_LOG_ASSERTIONS": "1 en Android; 0 en el resto",
    "HIGGS_CHECK_MEMORY_LEAKS": "1 en compilaciones de depuración",
    "HIGGS_ENABLE_LIVE_CONSTANT_EDITOR": "1 en depuración (solo escritorio)",
    "HIGGS_STANDALONE_FILTER_WINDOW_USE_KIOSK_MODE": "1 en iOS y Android; 0 en el resto",
}

ORDEN_MODULOS = [
    "higgs_core", "higgs_events", "higgs_data_structures", "higgs_cryptography",
    "higgs_graphics", "higgs_gui_basics", "higgs_gui_extra", "higgs_animation", "higgs_opengl",
    "higgs_video", "higgs_audio_basics", "higgs_audio_devices", "higgs_audio_formats",
    "higgs_audio_processors_headless", "higgs_audio_processors", "higgs_audio_plugin_client",
    "higgs_audio_utils", "higgs_dsp", "higgs_midi_ci", "higgs_osc", "higgs_javascript",
    "higgs_box2d", "higgs_analytics", "higgs_product_unlocking",
]

GRUPOS = [
    ("Núcleo", ["higgs_core", "higgs_events", "higgs_data_structures", "higgs_cryptography"]),
    ("Gráficos e interfaz", ["higgs_graphics", "higgs_gui_basics", "higgs_gui_extra",
                             "higgs_animation", "higgs_opengl", "higgs_video"]),
    ("Audio", ["higgs_audio_basics", "higgs_audio_devices", "higgs_audio_formats",
               "higgs_audio_utils", "higgs_dsp", "higgs_midi_ci"]),
    ("Plug-ins", ["higgs_audio_processors_headless", "higgs_audio_processors",
                  "higgs_audio_plugin_client"]),
    ("Otros", ["higgs_osc", "higgs_javascript", "higgs_box2d", "higgs_analytics",
               "higgs_product_unlocking"]),
]

CATEGORIAS_EJEMPLOS = [
    ("Audio", "Audio", "Aplicaciones de audio: dispositivos, reproducción, grabación, síntesis, MIDI y MIDI 2.0."),
    ("DSP", "DSP", "Demostraciones del módulo `higgs_dsp`."),
    ("GUI", "Interfaz gráfica", "Componentes, *layouts*, gráficos, fuentes, animación, OpenGL, vídeo y web."),
    ("Plugins", "Plug-ins", "Plug-ins de audio de ejemplo, que se compilan en todos los formatos disponibles."),
    ("Utilities", "Utilidades", "Red, hilos, datos, criptografía, JavaScript, OSC y otras utilidades."),
]

TIPOS_PIP = {"Component": "Aplicación", "AudioProcessor": "Plug-in", "Console": "Consola"}

pendientes = set()


# ------------------------------------------------------------------------------------------------
# Lectura del código

def leer(ruta):
    return Path(ruta).read_text(encoding="utf-8", errors="replace")


def declaracion_modulo(nombre):
    texto = leer(MODULOS / nombre / f"{nombre}.h")
    bloque = texto[texto.find("BEGIN_HIGGS_MODULE_DECLARATION"):texto.find("END_HIGGS_MODULE_DECLARATION")]
    valores = {}
    for linea in bloque.splitlines()[1:]:
        m = re.match(r"\s*(\w+):\s*(.*?)\s*$", linea)
        if m:
            valores[m.group(1)] = m.group(2)
    return valores


def lista(valor):
    return [x for x in re.split(r"[\s,]+", valor or "") if x]


def opciones_modulo(nombre):
    texto = leer(MODULOS / nombre / f"{nombre}.h")
    patron = re.compile(r"/\*\*\s*Config:\s*(\w+)(.*?)\*/\s*(#ifndef\s+\w+\s*\n\s*#\s*define\s+\w+[ \t]*([^\n]*))?", re.S)
    resultado = []
    for m in patron.finditer(texto):
        nombre_opcion = m.group(1)
        defecto = DEFECTOS_ESPECIALES.get(nombre_opcion, (m.group(4) or "").strip()) or "—"
        original = re.sub(r"\s+", " ", m.group(2)).strip()
        resultado.append({"nombre": nombre_opcion, "defecto": defecto, "original": original})
    return resultado


def _quitar_comentarios_y_cadenas(src):
    salida = list(src)
    i, n = 0, len(src)

    def borrar(a, b):
        for k in range(a, min(b, n)):
            if salida[k] != "\n":
                salida[k] = " "

    while i < n:
        if src.startswith("//", i):
            j = src.find("\n", i)
            j = n if j < 0 else j
            borrar(i, j)
            i = j
        elif src.startswith("/*", i):
            j = src.find("*/", i + 2)
            j = n if j < 0 else j + 2
            borrar(i, j)
            i = j
        elif src[i] in "\"'":
            q, j = src[i], i + 1
            while j < n and src[j] != q and src[j] != "\n":
                j += 2 if src[j] == "\\" else 1
            borrar(i, j + 1)
            i = j + 1
        elif src[i] == "#" and (i == 0 or src[:i].rstrip(" \t").endswith("\n")):
            j = i
            while True:
                e = src.find("\n", j)
                e = n if e < 0 else e
                if e > 0 and src[e - 1] == "\\":
                    j = e + 1
                    continue
                break
            borrar(i, e)
            i = e
        else:
            i += 1
    return "".join(salida)


_DECL = re.compile(r"\b(class|struct)\s+(?:HIGGS_API\s+)?(?:\[\[[^\]]*\]\]\s+)?(?:HIGGS_API\s+)?"
                   r"([A-Za-z_]\w*)(?:\s+final)?\s*(?::[^{};]*)?\s*$")


def _breve(doc):
    lineas = []
    for linea in doc.split("\n"):
        linea = linea.strip().lstrip("*").strip()
        if linea.startswith("@"):
            break
        lineas.append(linea)
    texto = re.sub(r"\s+", " ", " ".join(x for x in lineas if x)).strip()
    m = re.match(r"(.+?[.!?])(\s|$)", texto)
    return (m.group(1) if m else texto)[:400]


def clases_de_cabecera(src):
    """Clases y structs declarados en el ámbito de espacio de nombres, con su comentario Doxygen."""
    limpio = _quitar_comentarios_y_cadenas(src)
    pila, anterior, encontradas = [], 0, []
    for evento in re.finditer(r"[{};]", limpio):
        caracter, pos = evento.group(0), evento.start()
        segmento = limpio[anterior:pos]
        if caracter == "{":
            es_espacio = re.search(r"\b(namespace|extern)\b[^;{}]*$", segmento) is not None
            if not es_espacio and all(p == "ns" for p in pila):
                ultima = None
                for m in _DECL.finditer(segmento):
                    ultima = m
                if ultima and re.fullmatch(r"\s*(template\s*<.*>\s*)?", segmento[:ultima.start()], re.S):
                    encontradas.append((ultima.group(1), ultima.group(2), anterior + ultima.start()))
            pila.append("ns" if es_espacio else "x")
        elif caracter == "}" and pila:
            pila.pop()
        anterior = pos + 1

    resultado = []
    for tipo, nombre, pos in encontradas:
        cabeza = re.sub(r"template\s*<[^;{}]*>\s*$", "", src[:pos], flags=re.S).rstrip()
        if not cabeza.endswith("*/"):
            continue
        inicio = cabeza.rfind("/**")
        if inicio < 0 or cabeza.rfind("*/", 0, len(cabeza) - 2) > inicio:
            continue
        breve = _breve(cabeza[inicio + 3:-2])
        if breve and not nombre.lower().startswith("detail"):
            resultado.append({"nombre": nombre, "tipo": tipo, "breve": breve})
    return resultado


def catalogo_modulo(nombre):
    principal = leer(MODULOS / nombre / f"{nombre}.h")
    cabeceras = []
    for inc in re.findall(r'^\s*#\s*include\s+[<"]([^>"]+)[>"]', principal, re.M):
        if inc.startswith(nombre + "/"):
            rel = inc[len(nombre) + 1:]
        elif not inc.startswith("higgs_"):
            rel = inc
        else:
            continue
        ruta = MODULOS / nombre / rel
        if (ruta.is_file() and rel.endswith(".h") and "/native/" not in rel
                and not rel.startswith(("native/", "detail/")) and rel not in cabeceras):
            cabeceras.append(rel)
    catalogo = []
    for rel in cabeceras:
        clases = clases_de_cabecera(leer(MODULOS / nombre / rel))
        carpeta = rel.rsplit("/", 1)[0] if "/" in rel else "(raíz)"
        catalogo.append({"cabecera": rel, "carpeta": carpeta, "clases": clases})
    return catalogo


def ejemplos():
    resultado = []
    for carpeta, _, _ in CATEGORIAS_EJEMPLOS:
        for archivo in sorted((EJEMPLOS / carpeta).glob("*.h")):
            m = re.search(r"BEGIN_HIGGS_PIP_METADATA(.*?)END_HIGGS_PIP_METADATA", leer(archivo), re.S)
            if not m:
                continue
            datos, clave = {}, None
            for linea in m.group(1).splitlines():
                mm = re.match(r"\s*(\w+):\s*(.*)$", linea)
                if mm:
                    clave = mm.group(1)
                    datos[clave] = mm.group(2).strip()
                elif clave and linea.strip():
                    datos[clave] += " " + linea.strip()
            datos["archivo"] = archivo.name
            datos["carpeta"] = carpeta
            resultado.append(datos)
    return resultado


def dependencias_terceros():
    manifiesto = json.loads(leer(RAIZ_REPO / "extras" / "SBOM" / "dependencies.json"))
    por_modulo = {}
    for dep in manifiesto["dependencies"]:
        por_modulo.setdefault(dep["module"], []).append(dep)
    return por_modulo


def dependencias_transitivas(nombre, declaraciones):
    vistas, pendientes_dep = [], list(lista(declaraciones[nombre].get("dependencies")))
    while pendientes_dep:
        dep = pendientes_dep.pop(0)
        if dep not in vistas and dep in declaraciones:
            vistas.append(dep)
            pendientes_dep.extend(lista(declaraciones[dep].get("dependencies")))
    return [m for m in ORDEN_MODULOS if m in vistas]


# ------------------------------------------------------------------------------------------------
# Traducciones

def traducir(tabla, clave, original):
    if clave in tabla:
        return tabla[clave]
    if original:
        pendientes.add(f"{clave[:60]}…" if len(clave) > 60 else clave)
    return original


def celda(texto):
    return texto.replace("|", "\\|").replace("\n", " ")


def codigo_en_texto(texto):
    """Pone entre comillas invertidas los identificadores evidentes de un texto original en inglés."""
    return re.sub(r"(?<![`\w])(HIGGS_\w+|\w+::\w+(?:::\w+)*(?:\(\))?)(?![`\w])", r"`\1`", texto)


# ------------------------------------------------------------------------------------------------
# Generación de páginas

def enlace_modulo(nombre, desde_modulos=True):
    return f"[`{nombre}`]({nombre}.md)" if desde_modulos else f"[`{nombre}`](../modulos/{nombre}.md)"


def pagina_modulo(nombre, decl, opciones, catalogo, resumenes, trad, terceros, declaraciones):
    info = resumenes[nombre]
    lineas = [f"# {nombre}", "", info["resumen"], ""]

    directas = lista(decl.get("dependencies"))
    transitivas = [d for d in dependencias_transitivas(nombre, declaraciones) if d not in directas]
    usuarios = [m for m in ORDEN_MODULOS if nombre in lista(declaraciones[m].get("dependencies"))]

    lineas += ["## Ficha", "", "| Campo | Valor |", "|---|---|",
               f"| Identificador | `{decl.get('ID', nombre)}` |",
               f"| Versión | {decl.get('version', '—')} |",
               f"| Nombre declarado | {decl.get('name', '—')} |",
               f"| Descripción declarada | {celda(decl.get('description', '—'))} |",
               f"| Estándar mínimo de C++ | C++{decl.get('minimumCppStandard', '17')} |",
               f"| Licencia | {decl.get('license', '—')} |",
               f"| Dependencias directas | {', '.join(enlace_modulo(d) for d in directas) or 'ninguna'} |"]
    if transitivas:
        lineas.append(f"| Dependencias indirectas | {', '.join(enlace_modulo(d) for d in transitivas)} |")
    if usuarios:
        lineas.append(f"| Módulos que dependen de él | {', '.join(enlace_modulo(d) for d in usuarios)} |")
    plataforma = [("Frameworks de macOS", "OSXFrameworks"), ("Frameworks débiles de macOS", "WeakOSXFrameworks"),
                  ("Frameworks débiles de macOS", "WeakMacOSFrameworks"), ("Frameworks de iOS", "iOSFrameworks"),
                  ("Frameworks débiles de iOS", "WeakiOSFrameworks"), ("Paquetes pkg-config (Linux)", "linuxPackages"),
                  ("Bibliotecas de Linux", "linuxLibs"), ("Bibliotecas de Windows", "windowsLibs")]
    for etiqueta, clave in plataforma:
        if decl.get(clave):
            lineas.append(f"| {etiqueta} | {', '.join(f'`{x}`' for x in lista(decl[clave]))} |")
    lineas += [f"| Código fuente | [`modules/{nombre}`]({URL_FUENTE}modules/{nombre}) |", ""]

    lineas += ["## Uso", "", "```cmake", f"target_link_libraries(MiTarget PRIVATE higgs::{nombre})", "```", "",
               "```cpp", f"#include <{nombre}/{nombre}.h>", "```", ""]
    if nombre == "higgs_audio_plugin_client":
        lineas += ["!!! note", "    Este módulo no se enlaza a mano: `higgs_add_plugin` lo añade automáticamente a cada",
                   "    target de formato. Tu código solo tiene que definir `createPluginFilter()`.", ""]
    if info.get("guias"):
        lineas += ["Guías relacionadas: " + ", ".join(f"[{t}]({r})" for t, r in info["guias"]) + ".", ""]

    lineas += ["## Opciones de configuración", ""]
    if opciones:
        lineas += ["Se cambian con `target_compile_definitions(MiTarget PUBLIC OPCION=valor)` o en la sección "
                   "*Modules* de Mythos Studio.", "", "| Opción | Por defecto | Descripción |", "|---|---|---|"]
        for op in opciones:
            descripcion = traducir(trad["opciones"], op["nombre"], codigo_en_texto(op["original"]))
            lineas.append(f"| `{op['nombre']}` | `{celda(op['defecto'])}` | {celda(descripcion)} |"
                          if op["defecto"] != "—" and " " not in op["defecto"].strip("()")
                          else f"| `{op['nombre']}` | {celda(op['defecto'])} | {celda(descripcion)} |")
    else:
        lineas.append("Este módulo no declara opciones de configuración.")
    lineas.append("")

    if nombre in terceros:
        lineas += ["## Código de terceros incluido", "", "| Componente | Licencia | Ubicación |", "|---|---|---|"]
        for dep in terceros[nombre]:
            lineas.append(f"| [{dep['name']}]({dep['homepage']}) | `{dep['license']}` | `{dep['path']}` |"
                          if dep.get("homepage") else f"| {dep['name']} | `{dep['license']}` | `{dep['path']}` |")
        lineas.append("")

    if nombre == "higgs_audio_plugin_client":
        lineas += ["## Wrappers por formato", "", "| Formato | Archivos | Carpeta |", "|---|---|---|"]
        archivos = sorted(p.name for p in (MODULOS / nombre).iterdir() if p.is_file() and p.suffix in (".cpp", ".mm"))
        formatos = [("AAX", "AAX"), ("ARA", None), ("AU (v2)", "AU"), ("AUv3", None), ("LV2", "LV2"),
                    ("Standalone", "Standalone"), ("Unity", "Unity"), ("VST2", None), ("VST3", "VST3")]
        for formato, carpeta in formatos:
            clave = formato.split(" ")[0]
            propios = [a for a in archivos if re.search(rf"_{clave}(_\w+)?\.(cpp|mm)$", a)]
            lineas.append(f"| {formato} | {', '.join(f'`{a}`' for a in propios)} | "
                          f"{f'`{carpeta}/`' if carpeta else '—'} |")
        lineas.append("")

    total = sum(len(c["clases"]) for c in catalogo)
    lineas += ["## Catálogo de clases", ""]
    if total == 0:
        lineas += ["Este módulo no expone clases públicas propias en su cabecera principal.", ""]
    else:
        lineas += [f"{total} clases y estructuras públicas declaradas en las cabeceras que incluye "
                   f"`{nombre}.h`, agrupadas por carpeta. La descripción es la primera frase de su "
                   "comentario de documentación en el código.", ""]
        carpetas = []
        for entrada in catalogo:
            if entrada["clases"] and entrada["carpeta"] not in carpetas:
                carpetas.append(entrada["carpeta"])
        for carpeta in carpetas:
            lineas += [f"### `{carpeta}`", "", "| Clase | Descripción | Cabecera |", "|---|---|---|"]
            for entrada in catalogo:
                if entrada["carpeta"] != carpeta:
                    continue
                url = f"{URL_FUENTE}modules/{nombre}/{entrada['cabecera']}"
                archivo = entrada["cabecera"].rsplit("/", 1)[-1]
                for c in entrada["clases"]:
                    descripcion = traducir(trad["clases"], c["breve"], c["breve"])
                    lineas.append(f"| `{c['nombre']}` | {celda(descripcion)} | [`{archivo}`]({url}) |")
            lineas.append("")
    return "\n".join(lineas).rstrip() + "\n"


def pagina_indice_modulos(declaraciones, catalogos, opciones, resumenes):
    lineas = ["# Módulos", "",
              "Higgs Mythos está formado por 24 módulos. Cada ficha incluye sus dependencias, los frameworks "
              "y bibliotecas del sistema que necesita, sus opciones de configuración, el código de terceros "
              "que incluye y el catálogo completo de sus clases públicas.", "",
              "El diagrama de dependencias está en [Arquitectura del framework](../conceptos/arquitectura.md). "
              "Para la referencia de cada método, consulta los comentarios de las cabeceras o genera la "
              "referencia Doxygen (véase [Depuración y diagnóstico](../conceptos/depuracion.md#referencia-del-api-generada-con-doxygen)).", ""]
    for grupo, mods in GRUPOS:
        lineas += [f"## {grupo}", "", "| Módulo | Descripción | Depende de | Clases | Opciones |", "|---|---|---|---|---|"]
        for m in mods:
            deps = ", ".join(f"`{d}`" for d in lista(declaraciones[m].get("dependencies"))) or "—"
            resumen = resumenes[m]["resumen"]
            if ": " in resumen[:90]:
                resumen = resumen.split(": ")[0] + "."
            resumen = re.split(r"(?<=\.)\s", resumen)[0]
            clases = sum(len(c["clases"]) for c in catalogos[m])
            lineas.append(f"| [`{m}`]({m}.md) | {celda(resumen)} | {deps} | {clases} | {len(opciones[m])} |")
        lineas.append("")
    return "\n".join(lineas).rstrip() + "\n"


def pagina_opciones(opciones, trad):
    lineas = ["# Opciones de configuración", "",
              "Las opciones de configuración son macros de preprocesador `HIGGS_*` que cada módulo declara "
              "en su cabecera principal y que activan, desactivan o ajustan funciones. Todas tienen un valor "
              "por defecto; para cambiarlo:", "",
              "=== \"CMake\"", "", "    ```cmake", "    target_compile_definitions(MiTarget PUBLIC HIGGS_JACK=1 HIGGS_USE_CURL=0)",
              "    ```", "",
              "    En plug-ins usa visibilidad `PUBLIC` sobre el target de código compartido, para que la "
              "definición llegue a todos los *wrappers*.", "",
              "=== \"Mythos Studio\"", "",
              "    Selecciona el módulo en la sección *Modules* del proyecto y cambia la opción en su panel.", "",
              "!!! warning \"Opciones de plug-in que afectan a la compatibilidad\"",
              "    Algunas opciones de `higgs_audio_plugin_client` cambian cómo se identifican los parámetros o "
              "cómo se guarda el estado. Cambiarlas en un plug-in ya publicado puede romper proyectos "
              "guardados por tus usuarios.", ""]
    total = sum(len(v) for v in opciones.values())
    lineas += [f"En total hay **{total} opciones** repartidas en {sum(1 for v in opciones.values() if v)} módulos.", ""]
    for m in ORDEN_MODULOS:
        if not opciones[m]:
            continue
        lineas += [f"## {m}", "", f"Módulo [`{m}`](../modulos/{m}.md).", "",
                   "| Opción | Por defecto | Descripción |", "|---|---|---|"]
        for op in opciones[m]:
            descripcion = traducir(trad["opciones"], op["nombre"], codigo_en_texto(op["original"]))
            defecto = op["defecto"]
            defecto = f"`{celda(defecto)}`" if defecto != "—" and " " not in defecto.strip("()") else celda(defecto)
            lineas.append(f"| `{op['nombre']}` | {defecto} | {celda(descripcion)} |")
        lineas.append("")
    lineas += ["## Opciones de CMake", "",
               "Las opciones del propio sistema de compilación (`HIGGS_BUILD_EXAMPLES`, `HIGGS_BUILD_EXTRAS`, "
               "`HIGGS_MODULES_ONLY`…) y los argumentos de `higgs_add_*` (`NEEDS_CURL`, `NEEDS_WEB_BROWSER`, "
               "`NEEDS_OPENGL_ES`…) se describen en el [API de CMake](cmake-api.md).", ""]
    return "\n".join(lineas).rstrip() + "\n"


def pagina_ejemplos(lista_ejemplos, trad):
    lineas = ["# Catálogo de ejemplos", "",
              f"El repositorio incluye **{len(lista_ejemplos)} ejemplos** en `examples/`, bajo licencia ISC. Cada uno es "
              "un [proyecto PIP](../compilacion/pip.md) de un solo archivo. Se pueden compilar por separado o todos "
              "juntos en el **DemoRunner** (véase [Compilar los ejemplos](../primeros-pasos/compilar-ejemplos.md)).", "",
              "```bash", "cmake . -B cmake-build -DHIGGS_BUILD_EXAMPLES=ON", "cmake --build cmake-build --target DemoRunner",
              "cmake --build cmake-build --target <nombre del ejemplo>", "```", "",
              "Además, `examples/CMake` contiene las plantillas `GuiApp`, `ConsoleApp` y `AudioPlugin`, y "
              "`examples/Assets` los recursos compartidos (audio, imágenes, modelos 3D…) que usan las demos.", ""]
    for carpeta, titulo, descripcion in CATEGORIAS_EJEMPLOS:
        del_grupo = [e for e in lista_ejemplos if e["carpeta"] == carpeta]
        lineas += [f"## {titulo}", "", descripcion, "", "| Ejemplo | Tipo | Descripción | Módulos principales |", "|---|---|---|---|"]
        for e in del_grupo:
            texto = traducir(trad["ejemplos"], e["archivo"], e.get("description", ""))
            mods = [d for d in lista(e.get("dependencies")) if d not in (
                "higgs_core", "higgs_data_structures", "higgs_events", "higgs_graphics", "higgs_gui_basics")]
            mods_txt = ", ".join(f"`{d}`" for d in mods[:4]) + (" …" if len(mods) > 4 else "") or "`higgs_gui_basics`"
            url = f"{URL_FUENTE}examples/{carpeta}/{e['archivo']}"
            lineas.append(f"| [`{e.get('name', e['archivo'])}`]({url}) | {TIPOS_PIP.get(e.get('type'), e.get('type', ''))} "
                          f"| {celda(texto)} | {mods_txt} |")
        lineas.append("")
    return "\n".join(lineas).rstrip() + "\n"


# ------------------------------------------------------------------------------------------------

def main():
    analizador = argparse.ArgumentParser(description=__doc__.splitlines()[0])
    analizador.add_argument("--comprobar", action="store_true",
                            help="no escribe; falla si las páginas generadas están desactualizadas")
    args = analizador.parse_args()

    trad = json.loads(leer(DATOS / "traducciones.json"))
    resumenes = json.loads(leer(DATOS / "modulos.json"))
    declaraciones = {m: declaracion_modulo(m) for m in ORDEN_MODULOS}
    opciones = {m: opciones_modulo(m) for m in ORDEN_MODULOS}
    catalogos = {m: catalogo_modulo(m) for m in ORDEN_MODULOS}
    terceros = dependencias_terceros()

    paginas = {CONTENIDO / "modulos" / "index.md": pagina_indice_modulos(declaraciones, catalogos, opciones, resumenes),
               CONTENIDO / "compilacion" / "opciones.md": pagina_opciones(opciones, trad),
               CONTENIDO / "ejemplos" / "index.md": pagina_ejemplos(ejemplos(), trad)}
    for m in ORDEN_MODULOS:
        paginas[CONTENIDO / "modulos" / f"{m}.md"] = pagina_modulo(
            m, declaraciones[m], opciones[m], catalogos[m], resumenes, trad, terceros, declaraciones)

    desactualizadas = []
    for ruta, texto in paginas.items():
        if args.comprobar:
            if not ruta.exists() or ruta.read_text(encoding="utf-8") != texto:
                desactualizadas.append(ruta.relative_to(RAIZ_DOCS))
        else:
            ruta.parent.mkdir(parents=True, exist_ok=True)
            ruta.write_text(texto, encoding="utf-8")

    if pendientes:
        print(f"Aviso: {len(pendientes)} textos sin traducción en datos/traducciones.json (se usó el original).",
              file=sys.stderr)
    if args.comprobar:
        if desactualizadas:
            print("Páginas desactualizadas:\n  " + "\n  ".join(map(str, desactualizadas)), file=sys.stderr)
            return 1
        print("Las páginas de referencia están al día.")
    else:
        print(f"Generadas {len(paginas)} páginas.")
    return 0


if __name__ == "__main__":
    sys.exit(main())
