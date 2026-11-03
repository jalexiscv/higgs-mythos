# Higgs Mythos

**Lingua:** [Español](README.md) ·
[English](README.en.md) ·
[Português](README.pt.md) ·
[Français](README.fr.md) ·
[Deutsch](README.de.md) ·
Italiano ·
[中文](README.zh-CN.md) ·
[日本語](README.ja.md)

```text
                                  +@@@@@@@@@@-
                             .@@@@@@@@@@@@@@@@@@@@.
               .@@@@@@    :@@@@@@@@@@@@@@@@@@@@@@@@@@-    @@@@@@-
              %@%.%@@@@  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@  @@@@@.+@@
                   @@@@ :@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@= @@@@
                  @@@@. @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@ .@@@@
     @@@@@@@@@  #@@@@  @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@  @@@@%  @@@@@@@@@
   *@@@@%%@@@@@@=@@   #@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@   @@-@@@@@@%%@@@@=
   @@       .@@@@@    @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@.   @@@@@        @@
  =@     @@@@@:@@@@  %@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@  @@@@=@@@@%     @-
   @  .@@@@@=  @@@@   @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@   @@@@  -@@@@@   @
     @@@@@*    *@@@     @@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@.    @@@#    -@@@@@:
   @@@@@@      @@@@      +@@@@@@@@@@@@@@@@@@@@@@@@@@@@%      @@@@      @@@@@@
  @@@@@.       @@@. +@   : @@@@@@@@@@@@@@@@@@@@@@@@@@ .   @* .@@@.      .@@@@@
 @@@@@        @@@@:@@@-   @%.@@@@@@@@@@@@@@@@@@@@@@-%@   .@@@:@@@@        @@@@@
#@@@@-       @@@@:@@@@@@@ @@@ @@@@@@@@@@@@@@@@@@@@ @@@ @@@@@@@-@@@@       =@@@@#
@@@@@      *@@@@ @@@@@@@@+@@@@+ @@@@@@@@@@@@@@@@ +@@@@+@@@@@@@@ @@@@*      @@@@@
@@@@@     @@@@#  *@@@@@@@@@@@  . @@@@@@@@@@@@@@ .  @@@@@@@@@@@*  #@@@@     @@@@@
+@@@@+  .@@@@     @@@@@@@@@@@*     @@@@@@@@@@     #@@@@@@@@@@@     @@@@.  +@@@@+
 @@@@@#*@@@@       @@:    @@@@@@-   @@@@@@@@   .@@@@@@    -@@       @@@@+*@@@@@
  @@@@@@@.+              *@@@@@@@@@* :@@@@: :@@@@@@@@@#              * @@@@@@@
    @@@@@@@@@@@@@+==+@@@@@@@@@@@@@@@@*    :@@@@@@@@@@@@@@@@+::-+@@@@@@@@@@@@
    @:.@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@#  *@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@@.+@
   @@@@@    .%@@@@@@@@@@@#=-=#@@@@@@@@@  @@@@@@@@@@@%@@@@@@@@@@@@@*.    @@@@@
   @@@@       ++       :@@@@@@@@#%@@@@@:.@@@@@%*@@@@@@@@#       =+       @@@@.
  @@@@@    @@@@@@@@* @@@@@@@@.+@@@@@@@@++@@@@%@@@+.@@@@@@@@ *@@@@@@@@    @@@@@
  @@@@@   @@    .@+@@@@@@%    @@@@.@@@@@@@@@@ @@@@    %@@@@@@=#.    @@   @@@@@
  =@@@@    =    -@@@@@@       @@@@#@@@@@@@@@@*@@@@       @@@@@@*         @@@@=
   @@@@@       @@@@@@ =      @@@@@@@@@@@@@@@@@@@@@@      . @@@@@@       @@@@@
    @@@@@@   =@@@@@ @@@    @@@@@@  @@@@#+@@@@  @@@@@@    @@@ @@@@@*   @@@@@@
     @@@@@@@@# -@# @@@@%@@@@@@@:   @@@@::@@@@   -@@@@@@@%@@@@ *@# *@@@@@@@@
       @@@@@@@@@@@@@@@@@@@@@@      @@@@  @@@@      @@@@@@@@@@@@@@@@@@@@@@.
          %@@@@@@@@@@@@@=         @@@@@  @@@@@         +@@@@@@@@@@@@@%
           %@@@@    @@@@   @@@@@@ @@@@*  =@@@@ @@@@@@   @@@@    @@@@@
            @@@@@.  #+ *@@@@@@@@ @@@@@=  +@@@@@ #@@@@@@@@ -#  .@@@@@.
            :@@@@@@@@@@@@@@-    @@@@@ @@@@ @@@@@    -@@@@@@@@@@@@@@-
              *@@@@@@@@@.-@  %@@@@@@  @@@@  @@@@@@#  @:.@@@@@@@@@+
                      @@@@@@@@@@@@-  :@..@:  -@@@@@@@@@@@@
                       .@@@@@@@@    @@    @@    %@@@@@@@:
```

*La mascotte di Higgs Mythos.*

Higgs Mythos (abbreviato **HIGGS** nel codice e nei nomi dei moduli) è un
framework per applicazioni in C++ open source e multipiattaforma per creare
applicazioni desktop e mobili, inclusi plug-in audio nei formati VST, VST3,
AU, AUv3, AAX e LV2 e gli host che li caricano. Lo stesso codice sorgente si
compila per macOS, Windows, Linux, iOS e Android senza cambiare API: il
framework incapsula il sistema di finestre, il disegno 2D, l'audio e il MIDI di
ogni sistema operativo dietro classi comuni, e il progetto descrive soltanto
ciò che deve costruire.

Il framework si integra in progetti esistenti tramite CMake oppure si usa da
**Mythos Studio**, il suo strumento di configurazione dei progetti, che esporta
progetti per Xcode (macOS e iOS), Visual Studio, Android Studio e Makefile
Linux e include un editor di codice. È pensato sia per applicazioni generiche
con interfaccia grafica, come strumenti di produttività o utilità di sistema,
sia per software audio professionale che richiede elaborazione in tempo reale,
una gestione rigorosa dei thread e compatibilità con i formati di plug-in usati
dalle workstation audio digitali.

Higgs Mythos è distribuito sotto licenza **AGPL-3.0-or-later**. Le note di
copyright degli autori originali sono conservate in ogni file sorgente, e gli
esempi sono pubblicati sotto licenza ISC affinché possano essere copiati come
punto di partenza di un proprio progetto senza restrizioni aggiuntive.

## Primi passi

Il framework risiede nella directory `modules`. Ogni modulo si chiama
`higgs_<area>`, il suo header pubblico è `<modulo>/<modulo>.h`, e tutta l'API
C++ è dichiarata nel namespace `higgs`. Un modulo è un'unità autonoma: dichiara
nel suo header principale da quali altri moduli dipende, quali header pubblici
espone e quali librerie di sistema richiede su ogni piattaforma, così che il
sistema di compilazione possa determinare l'insieme minimo di sorgenti da
compilare per un dato progetto.

Per iniziare basta includere l'header del modulo necessario e derivare dalla
classe corrispondente. L'esempio seguente crea un componente di interfaccia
grafica con una dimensione iniziale di 800 per 600 pixel; `Component` è la
classe base di ogni elemento visivo del framework, dalla finestra completa
fino al pulsante:

```cpp
#include <higgs_core/higgs_core.h>
#include <higgs_gui_basics/higgs_gui_basics.h>

class MainComponent final : public higgs::Component
{
public:
    MainComponent() { setSize (800, 600); }
};
```

Le macro di configurazione mantengono il prefisso `HIGGS_` (per esempio
`HIGGS_MAJOR_VERSION` o `HIGGS_MODULE_AVAILABLE_higgs_core`), e i numeri di
versione sono definiti in
[higgs_StandardHeader.h](/modules/higgs_core/system/higgs_StandardHeader.h).
Le macro `HIGGS_WINDOWS`, `HIGGS_MAC`, `HIGGS_LINUX`, `HIGGS_IOS` e
`HIGGS_ANDROID` identificano la piattaforma di destinazione quando un progetto
ha bisogno di codice specifico per un sistema. Le altre convenzioni di
denominazione (namespace, funzioni CMake, estensione di progetto `.mythos`)
sono raccolte in [docs/higgs-mythos.md](/docs/higgs-mythos.md).

## Moduli

Higgs Mythos è organizzato in ventiquattro moduli indipendenti che si
combinano in base alle esigenze di ogni progetto. Un'applicazione da console
può usare solo `higgs_core`; un'applicazione grafica aggiunge `higgs_events`,
`higgs_graphics` e `higgs_gui_basics`; un plug-in audio aggiunge i moduli
audio e `higgs_audio_plugin_client`. Ogni modulo viene compilato soltanto
quando è collegato, così il binario finale non trascina codice di aree che non
utilizza. La tabella seguente riassume lo scopo di ciascuno; la documentazione
completa di ogni modulo si trova sul sito della documentazione e negli header
Doxygen.

I moduli si raggruppano in livelli. La base è `higgs_core`, da cui dipendono
tutti gli altri; su di esso poggiano `higgs_events` e `higgs_data_structures`,
che non hanno bisogno di interfaccia grafica e servono quindi anche agli
strumenti da console e ai processi in background. Il livello grafico
(`higgs_graphics`, `higgs_gui_basics`, `higgs_gui_extra`, `higgs_animation`,
`higgs_opengl`) costruisce l'interfaccia, e il livello audio va dai dati
(`higgs_audio_basics`) fino ai dispositivi, ai formati di file, ai processori
e ai formati di plug-in. Ogni modulo dichiara le proprie dipendenze nel suo
header principale, e Mythos Studio o CMake le risolvono automaticamente quando
lo si aggiunge a un progetto.

| Modulo | Scopo |
|---|---|
| `higgs_core` | Classi di base: stringhe, contenitori, memoria, thread, file, rete, XML, JSON, ZIP, tempo e test unitari. |
| `higgs_events` | Ciclo dei messaggi, timer, messaggi asincroni, broadcaster e listener, classe base dell'applicazione e IPC. |
| `higgs_data_structures` | `ValueTree`, `Value`, `UndoManager` e gestione delle preferenze. |
| `higgs_cryptography` | RSA, Blowfish, numeri primi e digest MD5, SHA-256 e Whirlpool. |
| `higgs_graphics` | Motore 2D: `Graphics`, geometria, colori, font, immagini, SVG ed effetti. |
| `higgs_gui_basics` | `Component`, finestre, widget, menu, layout, look-and-feel e accessibilità. |
| `higgs_gui_extra` | Editor di codice, `WebBrowserComponent`, selettore di colori, area di notifica e notifiche. |
| `higgs_animation` | Animazione dichiarativa con funzioni di easing sincronizzate allo schermo. |
| `higgs_opengl` | Rendering OpenGL e OpenGL ES all'interno dei componenti. |
| `higgs_video` | Riproduzione video e acquisizione dalla fotocamera. |
| `higgs_audio_basics` | `AudioBuffer`, sorgenti audio, sintetizzatore, MIDI 1.0, MPE e MIDI 2.0 (UMP). |
| `higgs_audio_devices` | Ingresso e uscita audio e MIDI con CoreAudio, WASAPI, ASIO, ALSA, JACK, Oboe e OpenSL ES. |
| `higgs_audio_formats` | Lettura e scrittura di WAV, AIFF, FLAC, Ogg Vorbis, Opus, MP3 e altri. |
| `higgs_audio_processors_headless` | `AudioProcessor`, parametri, `AudioProcessorGraph` e hosting di plug-in senza interfaccia. |
| `higgs_audio_processors` | Editor del processore, `AudioProcessorValueTreeState` e scansione dei plug-in. |
| `higgs_audio_plugin_client` | Wrapper VST2, VST3, AU, AUv3, AAX, LV2, Unity e Standalone. |
| `higgs_audio_utils` | `AudioAppComponent`, selettori di dispositivo, miniature audio e tastiere MIDI. |
| `higgs_dsp` | Filtri, oscillatori, dinamica, riverbero, convoluzione, FFT e SIMD. |
| `higgs_midi_ci` | MIDI Capability Inquiry: rilevamento, profili e proprietà. |
| `higgs_osc` | Open Sound Control su UDP. |
| `higgs_javascript` | Interprete JavaScript basato su QuickJS. |
| `higgs_box2d` | Il motore fisico 2D Box2D con renderer integrato. |
| `higgs_analytics` | Raccolta e invio a lotti di statistiche d'uso. |
| `higgs_product_unlocking` | Acquisti in-app e licenze con sblocco online. |

## CMake

È richiesto CMake 3.22 o superiore. Il modo più diretto per verificare che
l'ambiente sia pronto è compilare gli strumenti e gli esempi inclusi nel
repository: il primo comando configura l'albero di compilazione nella
directory `cmake-build`, attivando esempi ed extra, e il secondo compila
`DemoRunner`, l'applicazione che raccoglie tutte le dimostrazioni del
framework:

```bash
cmake . -B cmake-build -DHIGGS_BUILD_EXAMPLES=ON -DHIGGS_BUILD_EXTRAS=ON
cmake --build cmake-build --target DemoRunner
```

Su Windows bisogna prima caricare l'ambiente di Visual Studio
(`vcvars64.bat`) perché CMake trovi il compilatore MSVC, sia con Ninja sia con
MSBuild. Su Linux è necessario installare i pacchetti di sviluppo elencati in
[docs/Linux Dependencies.md](/docs/Linux%20Dependencies.md) prima di
configurare.

L'API CMake usa il prefisso `higgs_`: `higgs_add_gui_app` crea
un'applicazione grafica, `higgs_add_console_app` una da console,
`higgs_add_plugin` un plug-in con un target per formato, `higgs_add_module`
registra un modulo proprio e `higgs_generate_higgs_header` genera l'header
aggregato del progetto. Esistono anche `higgs_add_binary_data` per incorporare
risorse e `higgs_set_<kind>_sdk_path` per indicare il percorso degli SDK VST2,
AAX o ARA. I target importati si chiamano `higgs::higgs_core`,
`higgs::higgs_gui_basics` e così via, e si collegano con
`target_link_libraries` come qualsiasi altra libreria. Per integrare il
framework in un progetto esistente basta `add_subdirectory` su una copia del
repository oppure `find_package(HIGGS)` su un'installazione. Il riferimento
completo è in [docs/CMake API.md](/docs/CMake%20API.md), e
[examples/CMake](/examples/CMake) contiene tre progetti minimi (`GuiApp`,
`ConsoleApp` e `AudioPlugin`) pensati per essere copiati come punto di
partenza.

## Mythos Studio

Mythos Studio è il generatore di progetti distribuito in `extras/MythosStudio`.
Salva la configurazione di un progetto in un file `.mythos` e, a partire da
esso, esporta progetti nativi per ogni IDE e piattaforma: Xcode per macOS e
iOS, Visual Studio 2019, 2022 e 2026, Makefile Linux e Android Studio con
Gradle. Uno stesso file `.mythos` può avere più esportatori
contemporaneamente, ciascuno con le proprie configurazioni Debug e Release,
percorsi degli SDK e opzioni di compilatore e linker, così che il progetto resti
in un unico posto e i progetti degli IDE vengano rigenerati quando cambiano i
moduli o i file sorgente.

Lo strumento include una pagina iniziale con modelli di applicazione e di
plug-in, un gestore di moduli che risolve le dipendenze, un editor di codice
con evidenziazione della sintassi e un'interfaccia a riga di comando
(`--resave`, `--set-version`, `--encode-binary`, `--create-project-from-pip`,
tra le altre) per automatizzare attività senza aprire la finestra. Il suo
compagno `higgsaide` genera dal terminale i dati binari e le altre risorse che
i progetti incorporano. Android è supportato solo tramite Mythos Studio,
poiché l'API CMake non genera progetti per quella piattaforma.

Mythos Studio si compila da `extras/MythosStudio/Builds` (progetti Visual
Studio, Xcode o Makefile Linux) oppure tramite CMake con
`-DHIGGS_BUILD_EXTRAS=ON` e il target `MythosStudio`. Sono disponibili anche
eseguibili precompilati, non firmati, per Windows x64, macOS universale e Linux
x64, prodotti dal workflow GitHub Actions `HIGGS Mythos Studio Build` e
scaricabili come artefatti di ogni esecuzione.

## Strumenti ed esempi

Oltre a Mythos Studio, la directory `extras` raccoglie gli strumenti che
accompagnano il framework e che si compilano con `-DHIGGS_BUILD_EXTRAS=ON`.
`AudioPluginHost` è un host di plug-in completo che scansiona i formati
installati nel sistema, li collega in un grafo di processori e permette di
provare un proprio plug-in senza dipendere da una workstation commerciale.
`UnitTestRunner` esegue i test unitari che ogni modulo dichiara con la classe
`UnitTest`; consente di filtrare per categoria o per nome ed è il riferimento
di verifica del progetto. `BinaryBuilder` converte file in dati incorporati,
`AudioPerformanceTest` misura le prestazioni dell'elaborazione audio in tempo
reale, `NetworkGraphicsDemo` dimostra il disegno distribuito tra più macchine e
`SBOM` genera e verifica l'inventario delle dipendenze di terze parti.

La directory `examples` contiene dimostrazioni complete e compilabili della
maggior parte delle funzionalità, organizzate per area: `GUI`, `Audio`, `DSP`,
`Plugins`, `Utilities` e `Assets`. Sono tutte PIP: progetti completi contenuti
in un unico file header che inizia con un blocco di metadati (nome,
dipendenze, tipo e classe principale), che `DemoRunner` carica e mostra accanto
al suo codice sorgente, così da poter leggere l'implementazione di ogni demo
mentre è in esecuzione. `examples/CMake` contiene i tre progetti minimi già
citati, ed `examples/Plugins` include plug-in completi (sintetizzatori, un
sampler, un arpeggiatore, un noise gate, un host dentro un plug-in e un esempio
ARA) che servono da riferimento per il ciclo di vita completo di un
`AudioProcessor`.

## Documentazione

La guida utente completa in spagnolo è pubblicata su
<https://jalexiscv.github.io/higgs-mythos/> e le sue sorgenti risiedono in
[documentacion](/documentacion). Copre i primi passi (installazione, prima
applicazione e primo plug-in), i concetti del framework (architettura, ciclo
dei messaggi, thread, memoria e convenzioni), una guida per ogni area
(interfaccia grafica, grafica, audio, MIDI, DSP, plug-in, hosting, OpenGL,
video, web, accessibilità, localizzazione), una pagina di riferimento per
modulo, l'API CMake, gli strumenti e le note del progetto (modifiche
incompatibili, licenza, sicurezza e glossario). Il sito è costruito con MkDocs
e distribuito automaticamente su GitHub Pages quando cambiano la
documentazione, i moduli o gli esempi nel ramo `main`; le pagine di
riferimento vengono rigenerate dall'albero dei sorgenti a ogni distribuzione.

La documentazione dell'API è generata direttamente dai commenti Doxygen degli
header dei moduli, e il codice sorgente è la documentazione più completa e
aggiornata che esista. L'header principale di ogni modulo,
`higgs_<modulo>/higgs_<modulo>.h`, elenca i suoi header pubblici e le sue
dipendenze; il formato dei moduli è descritto in
[docs/Higgs Mythos Module Format.md](/docs/Higgs%20Mythos%20Module%20Format.md).
Per individuare una classe basta cercarne la dichiarazione negli header e
leggere il blocco di commento posto sopra:

```bash
grep -rlE "^\s*(class|struct)\s+(HIGGS_API\s+)?AudioProcessorValueTreeState\b" modules
```

[BREAKING_CHANGES.md](BREAKING_CHANGES.md) descrive, versione per versione,
che cosa è cambiato e come aggiornare il codice;
[CHANGE_LIST.md](CHANGE_LIST.md) riassume i titoli di ogni versione, e
[docs/README.md](/docs/README.md) è l'indice della documentazione tecnica del
repository (formato dei moduli, API CMake, identità del framework e protocolli
di lavoro).

## Requisiti minimi di sistema

Higgs Mythos richiede C++17 come standard minimo perché i suoi header pubblici
usano funzionalità di quello standard (ogni modulo lo dichiara nel campo
`minimumCppStandard` del suo header principale); quando è attivo
`HIGGS_USE_WINDOWS_MIDI_SERVICES` il progetto deve essere compilato con C++20,
poiché l'SDK Windows MIDI Services lo richiede. Le versioni di compilatore
indicate di seguito sono le più vecchie che vengono testate: versioni più
recenti di Xcode, Visual Studio, GCC e Clang funzionano senza modifiche. Su
Linux, oltre al compilatore, servono le librerie di sviluppo di X11, FreeType,
ALSA e altre elencate in
[docs/Linux Dependencies.md](/docs/Linux%20Dependencies.md); su Android il
progetto si costruisce con Gradle e l'NDK da Android Studio, tramite i progetti
che Mythos Studio esporta. Le due liste seguenti distinguono la macchina di
sviluppo dalla macchina di esecuzione: la prima indica con che cosa si
compila, la seconda dove viene eseguito il risultato. Per esempio, un plug-in
per Windows Arm64 si compila da Visual Studio su qualsiasi macchina Windows
compatibile, e le applicazioni iOS e Android si compilano rispettivamente da
macOS o da un desktop con Android Studio.

### Compilare progetti HIGGS

Questa lista raccoglie, per piattaforma di sviluppo, lo strumento minimo con
cui il framework si compila: l'IDE o il compilatore e la versione del sistema
operativo della macchina di sviluppo. Su macOS e iOS si usa Xcode, che include
Clang e gli SDK di Apple; la versione minima del sistema dipende dal fatto che
la macchina sia Intel o Apple Silicon, perché Xcode 12.4 richiede sistemi
diversi su ciascuna architettura. Su Windows si usa Visual Studio con il carico
di lavoro "Sviluppo di applicazioni desktop con C++" e l'SDK di Windows 10, sia
dall'IDE sia dalla riga di comando dopo aver caricato `vcvars64.bat`. Su Linux
va bene qualsiasi GCC o Clang moderno insieme alle librerie di sviluppo del
sistema, e i Makefile esportati da Mythos Studio o i progetti CMake si
compilano dal terminale. Su Android servono Android Studio, il plugin Gradle e
l'NDK, con cui si compilano i progetti esportati da Mythos Studio; la macchina
host può essere Windows, macOS o Linux. Le versioni indicate sono minimi
testati, non raccomandazioni: le versioni attuali di ogni strumento funzionano
senza modifiche.

- __Standard C++__: 17 (20 quando si compila con `HIGGS_USE_WINDOWS_MIDI_SERVICES` attivo)
- __macOS/iOS__: Xcode 12.4 (macOS 10.15.4 su Intel, macOS 11.0 su Apple Silicon)
- __Windows__: Visual Studio 2019 (Windows 10 versione 1607)
- __Linux__: g++ 7.0 o Clang 6.0 (vedi [docs/Linux Dependencies.md](/docs/Linux%20Dependencies.md))
- __Android__: Android Studio Narwhal 3 (2025.1.3) con Android Gradle plugin 8.13.2
  e NDK 28 su Windows, macOS o Linux

### Piattaforme di destinazione

I valori seguenti sono le versioni minime del sistema operativo su cui girano
le applicazioni e i plug-in compilati con il framework, e le architetture di
processore per cui vengono generati i binari. Su macOS e Windows è abituale
produrre binari universali o più architetture dallo stesso progetto; su
Android, Mythos Studio genera le ABI selezionate in un unico pacchetto.
Scegliere una versione minima più alta per un progetto specifico (per esempio
macOS 11 invece di 10.11) è possibile dalle impostazioni dell'esportatore in
Mythos Studio o con le consuete variabili di CMake, e consente di usare API di
sistema più recenti; scendere sotto i valori dell'elenco non è supportato,
perché il framework usa API che non esistono nelle versioni precedenti. Su
iOS, il simulatore x86_64 permette di provare sui Mac con processore Intel, e
i dispositivi fisici usano Arm64 o Arm64e. Su Android conviene includere
almeno arm64-v8a, l'architettura di tutti i dispositivi attuali, e aggiungere
le altre solo quando il pubblico di destinazione lo giustifica, perché ogni
ABI aggiuntiva aumenta la dimensione del pacchetto.

- __macOS__: macOS 10.11 (x86_64, Arm64)
- __Windows__: Windows 10 versione 1607 (x86_64, x86, Arm64, Arm64EC)
- __Linux__: distribuzioni principali (x86_64, Arm64/aarch64)
- __iOS__: iOS 12 (Arm64, Arm64e, simulatore x86_64)
- __Android__: Android 7 - Nougat (API Level 24) (arm64-v8a, armeabi-v7a, x86_64, x86)

## Contribuire

I contributi si ricevono tramite GitHub. Le richieste di funzionalità e le
domande generali vanno nella sezione *Discussions* del repository; gli errori
riproducibili si registrano come issue indicando la versione di Higgs Mythos,
i moduli interessati, la piattaforma e una riproduzione minima, usando i
modelli di `.github/ISSUE_TEMPLATE` quando possibile. Le vulnerabilità di
sicurezza non si pubblicano mai in una issue: si comunicano in privato tramite
il canale descritto in [SECURITY.md](SECURITY.md).

Ogni pull request deve concentrarsi su una sola modifica e descrivere che cosa
corregge o che cosa aggiunge. Prima di aprirla bisogna compilare i target
interessati ed eseguire i test unitari con `UnitTestRunner`. Il codice, i
commenti e i messaggi di commit si scrivono in inglese nel formato
`area: riepilogo`. Inviando un contributo si accetta di concederlo in licenza
agli stessi termini del progetto (AGPL-3.0-or-later) e si conferma di essere
autori del codice o di avere il diritto di inviarlo; non è ammesso codice
copiato da binari proprietari né da sorgenti con licenza incompatibile con
l'AGPL. La guida completa è in
[.github/contributing.md](.github/contributing.md), e
[CODE_OF_CONDUCT.md](CODE_OF_CONDUCT.md) stabilisce le regole di convivenza
della comunità.

## Licenza

Higgs Mythos è software libero: può essere ridistribuito e modificato secondo
i termini della GNU Affero General Public License pubblicata dalla Free
Software Foundation, nella versione 3 o, a scelta dell'utente, in qualsiasi
versione successiva (**AGPL-3.0-or-later**). Il testo completo e le note sulla
sua applicazione si trovano in [LICENSE.md](LICENSE.md). L'AGPL è una licenza
*copyleft*: qualsiasi opera derivata che venga distribuita, o offerta agli
utenti attraverso una rete, deve essere pubblicata sotto la stessa licenza e
con il codice sorgente disponibile. Ciò include le applicazioni e i plug-in che
collegano il framework.

Higgs Mythos deriva da una base di codice open source precedente il cui marchio
è stato rimosso del tutto da nomi, moduli, macro e risorse. Le note di
copyright degli autori originali sono conservate intatte nell'intestazione di
ogni file sorgente, come richiede la licenza, e non devono essere modificate né
eliminate. Gli esempi di `examples` sono pubblicati sotto la licenza ISC, molto
più permissiva, affinché possano essere copiati come base di un nuovo progetto.
L'inventario delle dipendenze di terze parti e delle loro licenze è in
[HiggsMythos.spdx.json](HiggsMythos.spdx.json), in formato SPDX, e gli SDK dei
plug-in (VST3, AAX, ARA, LV2) sono utilizzati secondo le proprie licenze e
accordi senza essere inclusi nel repository quando queste non lo consentono.

## Plug-in AAX

I plug-in AAX devono essere firmati digitalmente con gli strumenti di firma di
PACE Anti-Piracy prima di poter essere eseguiti nelle versioni commerciali di
Pro Tools. Avid fornisce questi strumenti gratuitamente. Prima di ottenerli è
necessario usare una build speciale di Pro Tools, chiamata Pro Tools Developer,
per provare i plug-in non firmati. I passaggi per ottenere Pro Tools Developer
sono:

1. Registrarsi come sviluppatore AAX su <https://developer.avid.com/aax/>.
2. Richiedere un codice di attivazione del Pro Tools Developer Bundle
   scrivendo a <devauth@avid.com>.
3. Scaricare l'ultima build di Pro Tools Developer dal proprio account
   sviluppatore Avid.

Quando i plug-in sono stati provati e sottoposti a debug in Pro Tools Developer
e sono pronti per essere firmati, bisogna scrivere a <audiosdk@avid.com> con
oggetto "PACE Eden Signing Tools Request". Il messaggio deve includere una
descrizione di ogni plug-in insieme a una registrazione dello schermo che
mostri il plug-in in esecuzione in Pro Tools Developer, con audio se
possibile, oltre alle seguenti informazioni:

- Nome dell'azienda
- Nome completo dell'amministratore
- Numero di telefono

Una volta inviata la richiesta, PACE Anti-Piracy si metterà in contatto
direttamente con informazioni sulla firma dei plug-in. Il target AAX di
`higgs_add_plugin` genera il pacchetto `.aaxplugin` pronto per la firma; il
percorso dell'SDK AAX si indica con `higgs_set_aax_sdk_path` prima di
dichiarare il plug-in.
