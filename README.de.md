# Higgs Mythos

**Sprache:** [Español](README.md) ·
[English](README.en.md) ·
[Português](README.pt.md) ·
[Français](README.fr.md) ·
Deutsch ·
[Italiano](README.it.md) ·
[中文](README.zh-CN.md) ·
[日本語](README.ja.md)

<p align="center"><img src="documentacion/contenido/assets/higgs_mythos_banner.svg" alt="Higgs Mythos" width="100%"></p>

*Das Maskottchen von Higgs Mythos. Textversion: [higgs_mythos_mascot.txt](documentacion/contenido/assets/higgs_mythos_mascot.txt).*

Higgs Mythos (im Code und in den Modulnamen als **HIGGS** abgekürzt) ist ein
quelloffenes, plattformübergreifendes C++-Anwendungsframework zum Erstellen
von Desktop- und Mobilanwendungen, einschließlich Audio-Plug-ins in den
Formaten VST, VST3, AU, AUv3, AAX und LV2 sowie der Hosts, die sie laden.
Derselbe Quellcode wird für macOS, Windows, Linux, iOS und Android kompiliert,
ohne die API zu wechseln: Das Framework kapselt Fenstersystem, 2D-Zeichnung,
Audio und MIDI jedes Betriebssystems hinter gemeinsamen Klassen, und das
Projekt beschreibt nur, was es bauen muss.

Das Framework lässt sich über CMake in bestehende Projekte integrieren oder
über **Mythos Studio** verwenden, sein Werkzeug zur Projektkonfiguration, das
Projekte für Xcode (macOS und iOS), Visual Studio, Android Studio und
Linux-Makefiles exportiert und einen Quellcode-Editor enthält. Es ist sowohl
für allgemeine Anwendungen mit grafischer Oberfläche gedacht, etwa
Produktivitätswerkzeuge oder Systemdienstprogramme, als auch für
professionelle Audiosoftware, die Echtzeitverarbeitung, strikte
Thread-Verwaltung und Kompatibilität mit den Plug-in-Formaten der digitalen
Audio-Workstations verlangt.

Higgs Mythos wird unter der Lizenz **AGPL-3.0-or-later** verbreitet. Die
Copyright-Hinweise der ursprünglichen Autoren bleiben in jeder Quelldatei
erhalten, und die Beispiele erscheinen unter der ISC-Lizenz, damit sie ohne
zusätzliche Einschränkungen als Ausgangspunkt eines eigenen Projekts kopiert
werden können.

## Erste Schritte

Das Framework liegt im Verzeichnis `modules`. Jedes Modul heißt
`higgs_<bereich>`, sein öffentlicher Header ist `<modul>/<modul>.h`, und die
gesamte C++-API wird im Namensraum `higgs` deklariert. Ein Modul ist eine in
sich geschlossene Einheit: Es deklariert in seinem Haupt-Header, von welchen
anderen Modulen es abhängt, welche öffentlichen Header es bereitstellt und
welche Systembibliotheken es auf jeder Plattform benötigt, sodass das
Build-System die minimale Menge an Quellen ermitteln kann, die für ein
bestimmtes Projekt kompiliert werden müssen.

Zum Einstieg genügt es, den Header des benötigten Moduls einzubinden und von
der entsprechenden Klasse abzuleiten. Das folgende Beispiel erzeugt eine
GUI-Komponente mit einer Anfangsgröße von 800 mal 600 Pixeln; `Component` ist
die Basisklasse jedes sichtbaren Elements im Framework, vom vollständigen
Fenster bis zur Schaltfläche:

```cpp
#include <higgs_core/higgs_core.h>
#include <higgs_gui_basics/higgs_gui_basics.h>

class MainComponent final : public higgs::Component
{
public:
    MainComponent() { setSize (800, 600); }
};
```

Die Konfigurationsmakros behalten das Präfix `HIGGS_` (zum Beispiel
`HIGGS_MAJOR_VERSION` oder `HIGGS_MODULE_AVAILABLE_higgs_core`), und die
Versionsnummern sind in
[higgs_StandardHeader.h](/modules/higgs_core/system/higgs_StandardHeader.h)
definiert. Die Makros `HIGGS_WINDOWS`, `HIGGS_MAC`, `HIGGS_LINUX`, `HIGGS_IOS`
und `HIGGS_ANDROID` kennzeichnen die Zielplattform, wenn ein Projekt
systemspezifischen Code benötigt. Die übrigen Namenskonventionen (Namensraum,
CMake-Funktionen, Projekterweiterung `.mythos`) sind in
[docs/higgs-mythos.md](/docs/higgs-mythos.md) zusammengefasst.

## Module

Higgs Mythos ist in vierundzwanzig unabhängige Module gegliedert, die je nach
Bedarf des Projekts kombiniert werden. Eine Konsolenanwendung kann nur
`higgs_core` verwenden; eine grafische Anwendung fügt `higgs_events`,
`higgs_graphics` und `higgs_gui_basics` hinzu; ein Audio-Plug-in ergänzt die
Audiomodule und `higgs_audio_plugin_client`. Jedes Modul wird nur kompiliert,
wenn es gelinkt wird, sodass die fertige Binärdatei keinen Code aus Bereichen
mitschleppt, die sie nie benutzt. Die folgende Tabelle fasst den Zweck jedes
Moduls zusammen; die vollständige Dokumentation jedes Moduls steht auf der
Dokumentationsseite und in den Doxygen-Headern.

Die Module sind in Schichten gegliedert. Die Basis ist `higgs_core`, von dem
alle anderen abhängen; darauf setzen `higgs_events` und
`higgs_data_structures` auf, die keine grafische Oberfläche benötigen und
deshalb auch Konsolenwerkzeugen und Hintergrundprozessen dienen. Die
Grafikschicht (`higgs_graphics`, `higgs_gui_basics`, `higgs_gui_extra`,
`higgs_animation`, `higgs_opengl`) baut die Oberfläche, und die Audioschicht
reicht von den Daten (`higgs_audio_basics`) über die Geräte, die Dateiformate
und die Prozessoren bis zu den Plug-in-Formaten. Jedes Modul deklariert seine
Abhängigkeiten in seinem Haupt-Header, und Mythos Studio oder CMake lösen sie
automatisch auf, wenn das Modul einem Projekt hinzugefügt wird.

| Modul | Zweck |
|---|---|
| `higgs_core` | Grundklassen: Zeichenketten, Container, Speicher, Threads, Dateien, Netzwerk, XML, JSON, ZIP, Zeit und Unit-Tests. |
| `higgs_events` | Nachrichtenschleife, Timer, asynchrone Nachrichten, Broadcaster und Listener, Anwendungsbasisklasse und IPC. |
| `higgs_data_structures` | `ValueTree`, `Value`, `UndoManager` und Verwaltung von Einstellungen. |
| `higgs_cryptography` | RSA, Blowfish, Primzahlen und die Hashes MD5, SHA-256 und Whirlpool. |
| `higgs_graphics` | 2D-Engine: `Graphics`, Geometrie, Farben, Schriften, Bilder, SVG und Effekte. |
| `higgs_gui_basics` | `Component`, Fenster, Widgets, Menüs, Layouts, Look-and-Feel und Barrierefreiheit. |
| `higgs_gui_extra` | Code-Editor, `WebBrowserComponent`, Farbwähler, Infobereich und Benachrichtigungen. |
| `higgs_animation` | Deklarative Animation mit Easing-Funktionen, synchron zur Bildwiederholung. |
| `higgs_opengl` | OpenGL- und OpenGL-ES-Rendering innerhalb von Komponenten. |
| `higgs_video` | Videowiedergabe und Kameraaufnahme. |
| `higgs_audio_basics` | `AudioBuffer`, Audioquellen, Synthesizer, MIDI 1.0, MPE und MIDI 2.0 (UMP). |
| `higgs_audio_devices` | Audio- und MIDI-Ein-/Ausgabe mit CoreAudio, WASAPI, ASIO, ALSA, JACK, Oboe und OpenSL ES. |
| `higgs_audio_formats` | Lesen und Schreiben von WAV, AIFF, FLAC, Ogg Vorbis, Opus, MP3 und mehr. |
| `higgs_audio_processors_headless` | `AudioProcessor`, Parameter, `AudioProcessorGraph` und Plug-in-Hosting ohne Oberfläche. |
| `higgs_audio_processors` | Prozessor-Editoren, `AudioProcessorValueTreeState` und Plug-in-Scan. |
| `higgs_audio_plugin_client` | Wrapper für VST2, VST3, AU, AUv3, AAX, LV2, Unity und Standalone. |
| `higgs_audio_utils` | `AudioAppComponent`, Geräteauswahl, Audio-Thumbnails und MIDI-Keyboards. |
| `higgs_dsp` | Filter, Oszillatoren, Dynamik, Hall, Faltung, FFT und SIMD. |
| `higgs_midi_ci` | MIDI Capability Inquiry: Erkennung, Profile und Eigenschaften. |
| `higgs_osc` | Open Sound Control über UDP. |
| `higgs_javascript` | JavaScript-Interpreter auf Basis von QuickJS. |
| `higgs_box2d` | Die 2D-Physik-Engine Box2D mit integriertem Renderer. |
| `higgs_analytics` | Erfassung und gebündelter Versand von Nutzungsstatistiken. |
| `higgs_product_unlocking` | In-App-Käufe und Lizenzen mit Online-Freischaltung. |

## CMake

Erforderlich ist CMake 3.22 oder höher. Der direkteste Weg, die
Einsatzbereitschaft der Umgebung zu prüfen, ist das Kompilieren der im
Repository enthaltenen Werkzeuge und Beispiele: Der erste Befehl konfiguriert
den Build-Baum im Verzeichnis `cmake-build` und aktiviert Beispiele und
Extras, der zweite kompiliert `DemoRunner`, die Anwendung, die alle
Demonstrationen des Frameworks vereint:

```bash
cmake . -B cmake-build -DHIGGS_BUILD_EXAMPLES=ON -DHIGGS_BUILD_EXTRAS=ON
cmake --build cmake-build --target DemoRunner
```

Unter Windows muss zuvor die Visual-Studio-Umgebung (`vcvars64.bat`) geladen
werden, damit CMake den MSVC-Compiler findet, sowohl mit Ninja als auch mit
MSBuild. Unter Linux müssen vor der Konfiguration die in
[docs/Linux Dependencies.md](/docs/Linux%20Dependencies.md) aufgeführten
Entwicklungspakete installiert werden.

Die CMake-API verwendet das Präfix `higgs_`: `higgs_add_gui_app` erzeugt eine
grafische Anwendung, `higgs_add_console_app` eine Konsolenanwendung,
`higgs_add_plugin` ein Plug-in mit einem Ziel pro Format, `higgs_add_module`
registriert ein eigenes Modul und `higgs_generate_higgs_header` erzeugt den
aggregierten Header des Projekts. Außerdem gibt es `higgs_add_binary_data` zum
Einbetten von Ressourcen und `higgs_set_<kind>_sdk_path` zur Angabe des Pfads
der VST2-, AAX- oder ARA-SDKs. Die importierten Ziele heißen
`higgs::higgs_core`, `higgs::higgs_gui_basics` und so weiter und werden mit
`target_link_libraries` wie jede andere Bibliothek gelinkt. Um das Framework in
ein bestehendes Projekt zu integrieren, genügt `add_subdirectory` auf eine
Kopie des Repositorys oder `find_package(HIGGS)` auf eine Installation. Die
vollständige Referenz steht in [docs/CMake API.md](/docs/CMake%20API.md), und
[examples/CMake](/examples/CMake) enthält drei minimale Projekte (`GuiApp`,
`ConsoleApp` und `AudioPlugin`), die als Ausgangspunkt kopiert werden können.

## Mythos Studio

Mythos Studio ist der Projektgenerator, der in `extras/MythosStudio`
ausgeliefert wird. Er speichert die Konfiguration eines Projekts in einer
`.mythos`-Datei und exportiert daraus native Projekte für jede IDE und
Plattform: Xcode für macOS und iOS, Visual Studio 2019, 2022 und 2026,
Linux-Makefiles und Android Studio mit Gradle. Eine einzige `.mythos`-Datei
kann mehrere Exporter gleichzeitig haben, jeder mit eigenen Debug- und
Release-Konfigurationen, SDK-Pfaden sowie Compiler- und Linker-Optionen, sodass
das Projekt an einem einzigen Ort gepflegt wird und die IDE-Projekte neu
erzeugt werden, sobald sich Module oder Quelldateien ändern.

Das Werkzeug enthält eine Startseite mit Vorlagen für Anwendungen und
Plug-ins, eine Modulverwaltung, die Abhängigkeiten auflöst, einen Code-Editor
mit Syntaxhervorhebung und eine Kommandozeilenschnittstelle (`--resave`,
`--set-version`, `--encode-binary`, `--create-project-from-pip` und weitere),
um Aufgaben zu automatisieren, ohne das Fenster zu öffnen. Sein Begleiter
`higgsaide` erzeugt im Terminal die Binärdaten und sonstigen Ressourcen, die
Projekte einbetten. Android wird nur über Mythos Studio unterstützt, da die
CMake-API keine Projekte für diese Plattform erzeugt.

Mythos Studio wird aus `extras/MythosStudio/Builds` (Visual-Studio-, Xcode-
oder Linux-Makefile-Projekte) oder über CMake mit `-DHIGGS_BUILD_EXTRAS=ON`
und dem Ziel `MythosStudio` kompiliert. Es gibt außerdem vorkompilierte,
unsignierte ausführbare Dateien für Windows x64, macOS universal und Linux x64,
die der GitHub-Actions-Workflow `HIGGS Mythos Studio Build` erzeugt und die als
Artefakte jedes Laufs heruntergeladen werden können.

## Werkzeuge und Beispiele

Neben Mythos Studio versammelt das Verzeichnis `extras` die Werkzeuge, die das
Framework begleiten und mit `-DHIGGS_BUILD_EXTRAS=ON` kompiliert werden.
`AudioPluginHost` ist ein vollständiger Plug-in-Host, der die im System
installierten Formate scannt, sie in einem Prozessorgraphen verbindet und das
Testen eines eigenen Plug-ins ermöglicht, ohne auf eine kommerzielle
Workstation angewiesen zu sein. `UnitTestRunner` führt die Unit-Tests aus, die
jedes Modul mit der Klasse `UnitTest` deklariert; er kann nach Kategorie oder
Name filtern und ist die Verifikationsreferenz des Projekts. `BinaryBuilder`
wandelt Dateien in eingebettete Daten um, `AudioPerformanceTest` misst die
Leistung der Echtzeit-Audioverarbeitung, `NetworkGraphicsDemo` demonstriert
verteiltes Zeichnen über mehrere Rechner und `SBOM` erzeugt und prüft das
Inventar der Drittanbieter-Abhängigkeiten.

Das Verzeichnis `examples` enthält vollständige, kompilierbare Demonstrationen
der meisten Funktionen, nach Bereichen geordnet: `GUI`, `Audio`, `DSP`,
`Plugins`, `Utilities` und `Assets`. Alle sind PIPs: vollständige Projekte in
einer einzigen Header-Datei, die mit einem Metadatenblock (Name,
Abhängigkeiten, Typ und Hauptklasse) beginnt und die `DemoRunner` lädt und
neben ihrem Quellcode anzeigt, sodass man die Implementierung jeder Demo lesen
kann, während sie läuft. `examples/CMake` enthält die drei bereits erwähnten
Minimalprojekte, und `examples/Plugins` umfasst vollständige Plug-ins
(Synthesizer, einen Sampler, einen Arpeggiator, ein Noise Gate, einen Host
innerhalb eines Plug-ins und ein ARA-Beispiel), die als Referenz für den
vollständigen Lebenszyklus eines `AudioProcessor` dienen.

## Dokumentation

Das vollständige Benutzerhandbuch auf Spanisch wird unter
<https://jalexiscv.github.io/higgs-mythos/> veröffentlicht; seine Quellen
liegen in [documentacion](/documentacion). Es behandelt die ersten Schritte
(Installation, erste Anwendung und erstes Plug-in), die Konzepte des
Frameworks (Architektur, Nachrichtenschleife, Threads, Speicher und
Konventionen), einen Leitfaden pro Bereich (grafische Oberfläche, Grafik,
Audio, MIDI, DSP, Plug-ins, Hosting, OpenGL, Video, Web, Barrierefreiheit,
Lokalisierung), eine Referenzseite pro Modul, die CMake-API, die Werkzeuge und
die Projektnotizen (inkompatible Änderungen, Lizenz, Sicherheit und Glossar).
Die Seite wird mit MkDocs gebaut und automatisch auf GitHub Pages
bereitgestellt, sobald sich Dokumentation, Module oder Beispiele im Zweig
`main` ändern; die Referenzseiten werden bei jeder Bereitstellung aus dem
Quellbaum neu erzeugt.

Die API-Dokumentation wird direkt aus den Doxygen-Kommentaren der
Modul-Header erzeugt, und der Quellcode ist die vollständigste und aktuellste
Dokumentation, die es gibt. Der Haupt-Header jedes Moduls,
`higgs_<modul>/higgs_<modul>.h`, listet seine öffentlichen Header und seine
Abhängigkeiten auf; das Modulformat ist in
[docs/Higgs Mythos Module Format.md](/docs/Higgs%20Mythos%20Module%20Format.md)
beschrieben. Um eine Klasse zu finden, genügt es, ihre Deklaration in den
Headern zu suchen und den Kommentarblock darüber zu lesen:

```bash
grep -rlE "^\s*(class|struct)\s+(HIGGS_API\s+)?AudioProcessorValueTreeState\b" modules
```

[BREAKING_CHANGES.md](BREAKING_CHANGES.md) beschreibt Version für Version, was
sich geändert hat und wie der Code zu aktualisieren ist;
[CHANGE_LIST.md](CHANGE_LIST.md) fasst die Schlagzeilen jeder Version
zusammen, und [docs/README.md](/docs/README.md) ist der Index der technischen
Dokumentation des Repositorys (Modulformat, CMake-API, Identität des
Frameworks und Arbeitsprotokolle).

## Mindestsystemanforderungen

Higgs Mythos verlangt C++17 als Mindeststandard, weil seine öffentlichen
Header Funktionen dieses Standards verwenden (jedes Modul deklariert ihn im
Feld `minimumCppStandard` seines Haupt-Headers); ist
`HIGGS_USE_WINDOWS_MIDI_SERVICES` aktiviert, muss das Projekt mit C++20
kompiliert werden, da das Windows-MIDI-Services-SDK dies verlangt. Die unten
genannten Compiler-Versionen sind die ältesten, die getestet werden; neuere
Versionen von Xcode, Visual Studio, GCC und Clang funktionieren ohne
Änderungen. Unter Linux werden neben dem Compiler die Entwicklungsbibliotheken
für X11, FreeType, ALSA und weitere benötigt, die in
[docs/Linux Dependencies.md](/docs/Linux%20Dependencies.md) aufgeführt sind;
unter Android wird das Projekt mit Gradle und dem NDK aus Android Studio
gebaut, über die Projekte, die Mythos Studio exportiert. Die beiden folgenden
Listen unterscheiden den Entwicklungsrechner vom Ausführungsrechner: Die erste
nennt, womit kompiliert wird, die zweite, wo das Ergebnis läuft. Ein
Windows-Arm64-Plug-in wird zum Beispiel aus Visual Studio auf jedem
kompatiblen Windows-Rechner gebaut, und iOS- und Android-Anwendungen werden
von macOS beziehungsweise von einem Desktop mit Android Studio aus gebaut.

### HIGGS-Projekte kompilieren

Diese Liste nennt je Entwicklungsplattform das minimale Werkzeug, mit dem das
Framework kompiliert wird: die IDE oder den Compiler und die
Betriebssystemversion des Entwicklungsrechners. Unter macOS und iOS kommt Xcode
zum Einsatz, das Clang und die Apple-SDKs enthält; die minimale Systemversion
hängt davon ab, ob der Rechner Intel oder Apple Silicon ist, weil Xcode 12.4
auf jeder Architektur ein anderes System voraussetzt. Unter Windows wird Visual
Studio mit der Arbeitslast „Desktopentwicklung mit C++“ und dem Windows 10 SDK
verwendet, sowohl aus der IDE als auch von der Kommandozeile nach dem Laden
von `vcvars64.bat`. Unter Linux genügt jeder moderne GCC oder Clang zusammen
mit den Entwicklungsbibliotheken des Systems, und die von Mythos Studio
exportierten Makefiles oder die CMake-Projekte werden im Terminal gebaut.
Unter Android sind Android Studio, das Gradle-Plugin und das NDK
erforderlich, mit denen die von Mythos Studio exportierten Projekte gebaut
werden; der Host-Rechner kann Windows, macOS oder Linux sein. Die genannten
Versionen sind getestete Minima, keine Empfehlungen: Die aktuellen Versionen
jedes Werkzeugs funktionieren ohne Änderungen.

- __C++-Standard__: 17 (20 beim Kompilieren mit aktiviertem `HIGGS_USE_WINDOWS_MIDI_SERVICES`)
- __macOS/iOS__: Xcode 12.4 (macOS 10.15.4 auf Intel, macOS 11.0 auf Apple Silicon)
- __Windows__: Visual Studio 2019 (Windows 10 Version 1607)
- __Linux__: g++ 7.0 oder Clang 6.0 (siehe [docs/Linux Dependencies.md](/docs/Linux%20Dependencies.md))
- __Android__: Android Studio Narwhal 3 (2025.1.3) mit Android Gradle Plugin 8.13.2
  und NDK 28 unter Windows, macOS oder Linux

### Zielplattformen

Die folgenden Werte sind die minimalen Betriebssystemversionen, auf denen die
mit dem Framework kompilierten Anwendungen und Plug-ins laufen, sowie die
Prozessorarchitekturen, für die Binärdateien erzeugt werden. Unter macOS und
Windows ist es üblich, universelle Binärdateien oder mehrere Architekturen aus
demselben Projekt zu erzeugen; unter Android erzeugt Mythos Studio die
ausgewählten ABIs in einem einzigen Paket. Eine höhere Mindestversion für ein
bestimmtes Projekt zu wählen (zum Beispiel macOS 11 statt 10.11) ist über die
Exporter-Einstellungen in Mythos Studio oder mit den üblichen CMake-Variablen
möglich und erlaubt die Nutzung neuerer System-APIs; unter die Werte der Liste
zu gehen wird nicht unterstützt, weil das Framework APIs verwendet, die in
früheren Versionen nicht existieren. Unter iOS erlaubt der x86_64-Simulator
das Testen auf Intel-Macs, und physische Geräte verwenden Arm64 oder Arm64e.
Unter Android empfiehlt es sich, mindestens arm64-v8a aufzunehmen, die
Architektur aller aktuellen Geräte, und die übrigen nur hinzuzufügen, wenn die
Zielgruppe es rechtfertigt, weil jede zusätzliche ABI die Paketgröße erhöht.

- __macOS__: macOS 10.11 (x86_64, Arm64)
- __Windows__: Windows 10 Version 1607 (x86_64, x86, Arm64, Arm64EC)
- __Linux__: gängige Distributionen (x86_64, Arm64/aarch64)
- __iOS__: iOS 12 (Arm64, Arm64e, x86_64-Simulator)
- __Android__: Android 7 - Nougat (API Level 24) (arm64-v8a, armeabi-v7a, x86_64, x86)

## Mitwirken

Beiträge werden über GitHub entgegengenommen. Funktionswünsche und allgemeine
Fragen gehören in den Bereich *Discussions* des Repositorys; reproduzierbare
Fehler werden als Issues gemeldet, mit Angabe der Higgs-Mythos-Version, der
betroffenen Module, der Plattform und einer minimalen Reproduktion, nach
Möglichkeit mit den Vorlagen aus `.github/ISSUE_TEMPLATE`. Sicherheitslücken
werden niemals in einem Issue veröffentlicht, sondern vertraulich über den in
[SECURITY.md](SECURITY.md) beschriebenen Kanal gemeldet.

Jeder Pull Request muss sich auf eine einzige Änderung konzentrieren und
beschreiben, was er behebt oder hinzufügt. Vor dem Öffnen müssen die
betroffenen Ziele kompiliert und die Unit-Tests mit `UnitTestRunner`
ausgeführt werden. Code, Kommentare und Commit-Nachrichten werden auf Englisch
im Format `area: summary` verfasst. Mit dem Einreichen eines Beitrags erklärt
man sich einverstanden, ihn unter denselben Bedingungen wie das Projekt
(AGPL-3.0-or-later) zu lizenzieren, und bestätigt, den Code selbst geschrieben
zu haben oder zur Einreichung berechtigt zu sein; Code, der aus proprietären
Binärdateien oder aus Quellen mit AGPL-inkompatibler Lizenz kopiert wurde, wird
nicht angenommen. Der vollständige Leitfaden steht in
[.github/contributing.md](.github/contributing.md), und
[CODE_OF_CONDUCT.md](CODE_OF_CONDUCT.md) legt die Verhaltensregeln der
Gemeinschaft fest.

## Lizenz

Higgs Mythos ist freie Software: Es darf unter den Bedingungen der GNU Affero
General Public License, wie von der Free Software Foundation veröffentlicht,
in Version 3 oder nach Wahl des Nutzers jeder späteren Version
(**AGPL-3.0-or-later**) weiterverbreitet und verändert werden. Der vollständige
Text und die Hinweise zu seiner Anwendung stehen in [LICENSE.md](LICENSE.md).
Die AGPL ist eine *Copyleft*-Lizenz: Jedes abgeleitete Werk, das verbreitet
oder Nutzern über ein Netzwerk angeboten wird, muss unter derselben Lizenz und
mit verfügbarem Quellcode veröffentlicht werden. Das schließt die Anwendungen
und Plug-ins ein, die das Framework linken.

Higgs Mythos stammt von einer früheren Open-Source-Codebasis ab, deren Marke
vollständig aus Namen, Modulen, Makros und Ressourcen entfernt wurde. Die
Copyright-Hinweise der ursprünglichen Autoren bleiben, wie die Lizenz es
verlangt, im Kopf jeder Quelldatei unverändert erhalten und dürfen weder
bearbeitet noch entfernt werden. Die Beispiele in `examples` erscheinen unter
der weit freizügigeren ISC-Lizenz, damit sie als Grundlage eines neuen
Projekts kopiert werden können. Das Inventar der Drittanbieter-Abhängigkeiten
und ihrer Lizenzen liegt im SPDX-Format in
[HiggsMythos.spdx.json](HiggsMythos.spdx.json), und die Plug-in-SDKs (VST3,
AAX, ARA, LV2) werden unter ihren eigenen Lizenzen und Vereinbarungen genutzt,
ohne in das Repository aufgenommen zu werden, wenn diese das nicht erlauben.

## AAX-Plug-ins

AAX-Plug-ins müssen mit den Signaturwerkzeugen von PACE Anti-Piracy digital
signiert werden, bevor sie in den kommerziellen Versionen von Pro Tools laufen.
Avid stellt diese Werkzeuge kostenlos bereit. Bevor man sie erhält, muss man
eine spezielle Ausgabe von Pro Tools, genannt Pro Tools Developer, verwenden,
um die unsignierten Plug-ins zu testen. Die Schritte, um Pro Tools Developer zu
erhalten, sind:

1. Als AAX-Entwickler unter <https://developer.avid.com/aax/> registrieren.
2. Einen Aktivierungscode für das Pro Tools Developer Bundle per E-Mail an
   <devauth@avid.com> anfordern.
3. Die neueste Pro-Tools-Developer-Version aus dem Avid-Entwicklerkonto
   herunterladen.

Wenn die Plug-ins in Pro Tools Developer getestet und debuggt wurden und zum
Signieren bereit sind, schreibt man an <audiosdk@avid.com> mit dem Betreff
„PACE Eden Signing Tools Request“. Die Nachricht muss eine Beschreibung jedes
Plug-ins sowie eine Bildschirmaufnahme enthalten, die das Plug-in in Pro Tools
Developer zeigt, nach Möglichkeit mit Ton, und außerdem die folgenden Angaben:

- Firmenname
- Vollständiger Name des Administrators
- Telefonnummer

Nach dem Absenden der Anfrage meldet sich PACE Anti-Piracy direkt mit
Informationen zur Signierung der Plug-ins. Das AAX-Ziel von
`higgs_add_plugin` erzeugt das signierfertige Paket `.aaxplugin`; der Pfad zum
AAX-SDK wird vor der Deklaration des Plug-ins mit `higgs_set_aax_sdk_path`
angegeben.
