# Higgs Mythos

**Langue :** [Español](README.md) ·
[English](README.en.md) ·
[Português](README.pt.md) ·
Français ·
[Deutsch](README.de.md) ·
[Italiano](README.it.md) ·
[中文](README.zh-CN.md) ·
[日本語](README.ja.md)

<p align="center"><img src="documentacion/contenido/assets/higgs_mythos_banner.svg" alt="Higgs Mythos" width="100%"></p>

*La mascotte de Higgs Mythos. Version texte : [higgs_mythos_mascot.txt](documentacion/contenido/assets/higgs_mythos_mascot.txt).*

Higgs Mythos (abrégé **HIGGS** dans le code et dans les noms des modules) est
un framework d'applications C++ open source et multiplateforme pour créer des
applications de bureau et mobiles, y compris des plug-ins audio aux formats
VST, VST3, AU, AUv3, AAX et LV2 et les hôtes qui les chargent. Le même code
source se compile pour macOS, Windows, Linux, iOS et Android sans changer
d'API : le framework encapsule le système de fenêtres, le dessin 2D, l'audio et
le MIDI de chaque système d'exploitation derrière des classes communes, et le
projet ne décrit que ce qu'il doit construire.

Le framework s'intègre dans des projets existants via CMake ou s'utilise depuis
**Mythos Studio**, son outil de configuration de projets, qui exporte des
projets pour Xcode (macOS et iOS), Visual Studio, Android Studio et les
Makefiles Linux et inclut un éditeur de code. Il est conçu aussi bien pour des
applications générales à interface graphique, comme des outils de productivité
ou des utilitaires système, que pour des logiciels audio professionnels qui
exigent un traitement en temps réel, une gestion stricte des threads et la
compatibilité avec les formats de plug-in utilisés par les stations de travail
audio.

Higgs Mythos est distribué sous licence **AGPL-3.0-or-later**. Les mentions de
copyright des auteurs originaux sont conservées dans chaque fichier source, et
les exemples sont publiés sous licence ISC afin de pouvoir être copiés comme
point de départ d'un projet personnel sans restrictions supplémentaires.

## Premiers pas

Le framework réside dans le répertoire `modules`. Chaque module s'appelle
`higgs_<domaine>`, son en-tête public est `<module>/<module>.h`, et toute l'API
C++ est déclarée dans l'espace de noms `higgs`. Un module est une unité
autonome : il déclare dans son en-tête principal de quels autres modules il
dépend, quels en-têtes publics il expose et quelles bibliothèques système il
requiert sur chaque plateforme, de sorte que le système de compilation peut
déterminer l'ensemble minimal de sources à compiler pour un projet donné.

Pour commencer, il suffit d'inclure l'en-tête du module nécessaire et de
dériver de la classe correspondante. L'exemple suivant crée un composant
d'interface graphique d'une taille initiale de 800 par 600 pixels ;
`Component` est la classe de base de tout élément visuel du framework, de la
fenêtre complète jusqu'au bouton :

```cpp
#include <higgs_core/higgs_core.h>
#include <higgs_gui_basics/higgs_gui_basics.h>

class MainComponent final : public higgs::Component
{
public:
    MainComponent() { setSize (800, 600); }
};
```

Les macros de configuration conservent le préfixe `HIGGS_` (par exemple
`HIGGS_MAJOR_VERSION` ou `HIGGS_MODULE_AVAILABLE_higgs_core`), et les numéros
de version sont définis dans
[higgs_StandardHeader.h](/modules/higgs_core/system/higgs_StandardHeader.h).
Les macros `HIGGS_WINDOWS`, `HIGGS_MAC`, `HIGGS_LINUX`, `HIGGS_IOS` et
`HIGGS_ANDROID` identifient la plateforme cible lorsqu'un projet a besoin de
code spécifique à un système. Les autres conventions de nommage (espace de
noms, fonctions CMake, extension de projet `.mythos`) sont réunies dans
[docs/higgs-mythos.md](/docs/higgs-mythos.md).

## Modules

Higgs Mythos s'organise en vingt-quatre modules indépendants qui se combinent
selon les besoins de chaque projet. Une application console peut n'utiliser
que `higgs_core` ; une application graphique ajoute `higgs_events`,
`higgs_graphics` et `higgs_gui_basics` ; un plug-in audio ajoute les modules
audio et `higgs_audio_plugin_client`. Chaque module n'est compilé que
lorsqu'il est lié, de sorte que le binaire final n'embarque pas de code
provenant de domaines qu'il n'utilise pas. Le tableau suivant résume le rôle
de chacun ; la documentation complète de chaque module se trouve sur le site de
documentation et dans les en-têtes Doxygen.

Les modules se regroupent en couches. La base est `higgs_core`, dont dépendent
tous les autres ; `higgs_events` et `higgs_data_structures` s'appuient sur lui,
n'ont pas besoin d'interface graphique et servent donc aussi aux outils en
ligne de commande et aux processus d'arrière-plan. La couche graphique
(`higgs_graphics`, `higgs_gui_basics`, `higgs_gui_extra`, `higgs_animation`,
`higgs_opengl`) construit l'interface, et la couche audio va des données
(`higgs_audio_basics`) jusqu'aux périphériques, aux formats de fichier, aux
processeurs et aux formats de plug-in. Chaque module déclare ses dépendances
dans son en-tête principal, et Mythos Studio ou CMake les résolvent
automatiquement lorsqu'on l'ajoute à un projet.

| Module | Rôle |
|---|---|
| `higgs_core` | Classes de base : chaînes, conteneurs, mémoire, threads, fichiers, réseau, XML, JSON, ZIP, temps et tests unitaires. |
| `higgs_events` | Boucle de messages, minuteurs, messages asynchrones, diffuseurs et écouteurs, classe de base d'application et IPC. |
| `higgs_data_structures` | `ValueTree`, `Value`, `UndoManager` et gestion des préférences. |
| `higgs_cryptography` | RSA, Blowfish, nombres premiers et condensés MD5, SHA-256 et Whirlpool. |
| `higgs_graphics` | Moteur 2D : `Graphics`, géométrie, couleurs, polices, images, SVG et effets. |
| `higgs_gui_basics` | `Component`, fenêtres, widgets, menus, mises en page, look-and-feel et accessibilité. |
| `higgs_gui_extra` | Éditeur de code, `WebBrowserComponent`, sélecteur de couleurs, zone de notification et notifications. |
| `higgs_animation` | Animation déclarative avec fonctions d'accélération synchronisées à l'écran. |
| `higgs_opengl` | Rendu OpenGL et OpenGL ES dans des composants. |
| `higgs_video` | Lecture vidéo et capture caméra. |
| `higgs_audio_basics` | `AudioBuffer`, sources audio, synthétiseur, MIDI 1.0, MPE et MIDI 2.0 (UMP). |
| `higgs_audio_devices` | Entrées et sorties audio et MIDI avec CoreAudio, WASAPI, ASIO, ALSA, JACK, Oboe et OpenSL ES. |
| `higgs_audio_formats` | Lecture et écriture de WAV, AIFF, FLAC, Ogg Vorbis, Opus, MP3 et plus. |
| `higgs_audio_processors_headless` | `AudioProcessor`, paramètres, `AudioProcessorGraph` et hébergement de plug-ins sans interface. |
| `higgs_audio_processors` | Éditeurs de processeur, `AudioProcessorValueTreeState` et analyse des plug-ins. |
| `higgs_audio_plugin_client` | Wrappers VST2, VST3, AU, AUv3, AAX, LV2, Unity et Standalone. |
| `higgs_audio_utils` | `AudioAppComponent`, sélecteurs de périphériques, vignettes audio et claviers MIDI. |
| `higgs_dsp` | Filtres, oscillateurs, dynamique, réverbération, convolution, FFT et SIMD. |
| `higgs_midi_ci` | MIDI Capability Inquiry : découverte, profils et propriétés. |
| `higgs_osc` | Open Sound Control sur UDP. |
| `higgs_javascript` | Interpréteur JavaScript basé sur QuickJS. |
| `higgs_box2d` | Moteur physique 2D Box2D avec moteur de rendu intégré. |
| `higgs_analytics` | Collecte et envoi par lots de données d'utilisation. |
| `higgs_product_unlocking` | Achats intégrés et licences avec déverrouillage en ligne. |

## CMake

CMake 3.22 ou supérieur est requis. La manière la plus directe de vérifier que
l'environnement est prêt consiste à compiler les outils et les exemples
fournis dans le dépôt : la première commande configure l'arborescence de
compilation dans le répertoire `cmake-build`, en activant les exemples et les
extras, et la seconde compile `DemoRunner`, l'application qui rassemble toutes
les démonstrations du framework :

```bash
cmake . -B cmake-build -DHIGGS_BUILD_EXAMPLES=ON -DHIGGS_BUILD_EXTRAS=ON
cmake --build cmake-build --target DemoRunner
```

Sous Windows, il faut d'abord charger l'environnement de Visual Studio
(`vcvars64.bat`) pour que CMake trouve le compilateur MSVC, aussi bien avec
Ninja qu'avec MSBuild. Sous Linux, il faut installer les paquets de
développement énumérés dans
[docs/Linux Dependencies.md](/docs/Linux%20Dependencies.md) avant de
configurer.

L'API CMake utilise le préfixe `higgs_` : `higgs_add_gui_app` crée une
application graphique, `higgs_add_console_app` une application console,
`higgs_add_plugin` un plug-in avec une cible par format, `higgs_add_module`
enregistre un module personnel et `higgs_generate_higgs_header` génère
l'en-tête agrégé du projet. Il existe aussi `higgs_add_binary_data` pour
embarquer des ressources et `higgs_set_<kind>_sdk_path` pour indiquer le chemin
des SDK VST2, AAX ou ARA. Les cibles importées s'appellent
`higgs::higgs_core`, `higgs::higgs_gui_basics` et ainsi de suite, et se lient
avec `target_link_libraries` comme toute autre bibliothèque. Pour intégrer le
framework dans un projet existant, il suffit d'un `add_subdirectory` sur une
copie du dépôt ou d'un `find_package(HIGGS)` sur une installation. La référence
complète se trouve dans [docs/CMake API.md](/docs/CMake%20API.md), et
[examples/CMake](/examples/CMake) contient trois projets minimaux (`GuiApp`,
`ConsoleApp` et `AudioPlugin`) conçus pour être copiés comme point de départ.

## Mythos Studio

Mythos Studio est le générateur de projets livré dans `extras/MythosStudio`.
Il enregistre la configuration d'un projet dans un fichier `.mythos` et, à
partir de celui-ci, exporte des projets natifs pour chaque IDE et plateforme :
Xcode pour macOS et iOS, Visual Studio 2019, 2022 et 2026, Makefiles Linux et
Android Studio avec Gradle. Un même fichier `.mythos` peut avoir plusieurs
exportateurs à la fois, chacun avec ses configurations Debug et Release, ses
chemins de SDK et ses options de compilateur et d'éditeur de liens, de sorte
que le projet reste en un seul endroit et que les projets d'IDE sont régénérés
lorsque les modules ou les fichiers sources changent.

L'outil comprend une page d'accueil avec des modèles d'application et de
plug-in, un gestionnaire de modules qui résout les dépendances, un éditeur de
code avec coloration syntaxique et une interface en ligne de commande
(`--resave`, `--set-version`, `--encode-binary`, `--create-project-from-pip`,
entre autres) pour automatiser des tâches sans ouvrir la fenêtre. Son
compagnon `higgsaide` génère depuis le terminal les données binaires et autres
ressources que les projets embarquent. Android n'est pris en charge qu'à
travers Mythos Studio, car l'API CMake ne génère pas de projets pour cette
plateforme.

Mythos Studio se compile depuis `extras/MythosStudio/Builds` (projets Visual
Studio, Xcode ou Makefile Linux) ou via CMake avec `-DHIGGS_BUILD_EXTRAS=ON` et
la cible `MythosStudio`. Des exécutables précompilés, non signés, sont aussi
disponibles pour Windows x64, macOS universel et Linux x64, produits par le
workflow GitHub Actions `HIGGS Mythos Studio Build` et téléchargeables comme
artefacts de chaque exécution.

## Outils et exemples

Outre Mythos Studio, le répertoire `extras` rassemble les outils qui
accompagnent le framework et se compilent avec `-DHIGGS_BUILD_EXTRAS=ON`.
`AudioPluginHost` est un hôte de plug-ins complet qui analyse les formats
installés sur le système, les connecte dans un graphe de processeurs et permet
de tester son propre plug-in sans dépendre d'une station de travail
commerciale. `UnitTestRunner` exécute les tests unitaires que chaque module
déclare avec la classe `UnitTest` ; il permet de filtrer par catégorie ou par
nom et constitue la référence de vérification du projet. `BinaryBuilder`
convertit des fichiers en données embarquées, `AudioPerformanceTest` mesure
les performances du traitement audio en temps réel, `NetworkGraphicsDemo`
démontre le dessin distribué entre plusieurs machines et `SBOM` génère et
vérifie l'inventaire des dépendances tierces.

Le répertoire `examples` contient des démonstrations complètes et compilables
de la plupart des fonctionnalités, organisées par domaine : `GUI`, `Audio`,
`DSP`, `Plugins`, `Utilities` et `Assets`. Toutes sont des PIP : des projets
complets contenus dans un seul fichier d'en-tête qui commence par un bloc de
métadonnées (nom, dépendances, type et classe principale), que `DemoRunner`
charge et affiche à côté de son code source, de sorte qu'on peut lire
l'implémentation de chaque démo pendant son exécution. `examples/CMake`
contient les trois projets minimaux déjà mentionnés, et `examples/Plugins`
inclut des plug-ins complets (synthétiseurs, un sampler, un arpégiateur, un
noise gate, un hôte dans un plug-in et un exemple ARA) qui servent de référence
pour le cycle de vie complet d'un `AudioProcessor`.

## Documentation

Le guide utilisateur complet en espagnol est publié sur
<https://jalexiscv.github.io/higgs-mythos/> et ses sources résident dans
[documentacion](/documentacion). Il couvre les premiers pas (installation,
première application et premier plug-in), les concepts du framework
(architecture, boucle de messages, threads, mémoire et conventions), un guide
par domaine (interface graphique, graphismes, audio, MIDI, DSP, plug-ins,
hébergement, OpenGL, vidéo, web, accessibilité, localisation), une page de
référence par module, l'API CMake, les outils et les notes du projet
(changements incompatibles, licence, sécurité et glossaire). Le site est
construit avec MkDocs et déployé automatiquement sur GitHub Pages lorsque la
documentation, les modules ou les exemples changent sur la branche `main` ;
les pages de référence sont régénérées à partir de l'arborescence des sources
à chaque déploiement.

La documentation de l'API est générée directement à partir des commentaires
Doxygen des en-têtes des modules, et le code source constitue la documentation
la plus complète et la plus à jour qui existe. L'en-tête principal de chaque
module, `higgs_<module>/higgs_<module>.h`, énumère ses en-têtes publics et ses
dépendances ; le format de module est décrit dans
[docs/Higgs Mythos Module Format.md](/docs/Higgs%20Mythos%20Module%20Format.md).
Pour localiser une classe, il suffit de chercher sa déclaration dans les
en-têtes et de lire le bloc de commentaire situé au-dessus :

```bash
grep -rlE "^\s*(class|struct)\s+(HIGGS_API\s+)?AudioProcessorValueTreeState\b" modules
```

[BREAKING_CHANGES.md](BREAKING_CHANGES.md) décrit, version par version, ce qui
a changé et comment mettre à jour le code ; [CHANGE_LIST.md](CHANGE_LIST.md)
résume les points saillants de chaque version, et
[docs/README.md](/docs/README.md) est l'index de la documentation technique du
dépôt (format des modules, API CMake, identité du framework et protocoles de
travail).

## Configuration minimale requise

Higgs Mythos exige C++17 comme standard minimal parce que ses en-têtes publics
utilisent des fonctionnalités de ce standard (chaque module le déclare dans le
champ `minimumCppStandard` de son en-tête principal) ; lorsque
`HIGGS_USE_WINDOWS_MIDI_SERVICES` est activé, le projet doit être compilé en
C++20, car le SDK Windows MIDI Services l'exige. Les versions de compilateur
indiquées ci-dessous sont les plus anciennes testées : des versions plus
récentes de Xcode, Visual Studio, GCC et Clang fonctionnent sans modification.
Sous Linux, outre le compilateur, il faut les bibliothèques de développement de
X11, FreeType, ALSA et d'autres énumérées dans
[docs/Linux Dependencies.md](/docs/Linux%20Dependencies.md) ; sous Android, le
projet est construit avec Gradle et le NDK depuis Android Studio, à travers les
projets qu'exporte Mythos Studio. Les deux listes suivantes distinguent la
machine de développement de la machine d'exécution : la première indique avec
quoi l'on compile, la seconde où le résultat s'exécute. Par exemple, un plug-in
Windows Arm64 se compile depuis Visual Studio sur n'importe quelle machine
Windows compatible, et les applications iOS et Android se compilent depuis
macOS ou depuis un poste de travail équipé d'Android Studio, respectivement.

### Compiler des projets HIGGS

Cette liste réunit, par plateforme de développement, l'outil minimal avec
lequel le framework se compile : l'IDE ou le compilateur et la version du
système d'exploitation de la machine de développement. Sous macOS et iOS, on
utilise Xcode, qui inclut Clang et les SDK d'Apple ; la version minimale du
système dépend de la machine, Intel ou Apple Silicon, car Xcode 12.4 exige des
systèmes différents sur chaque architecture. Sous Windows, on utilise Visual
Studio avec la charge de travail « Développement Desktop en C++ » et le SDK
Windows 10, aussi bien depuis l'IDE que depuis la ligne de commande après
avoir chargé `vcvars64.bat`. Sous Linux, tout GCC ou Clang moderne convient
avec les bibliothèques de développement du système, et les Makefiles exportés
par Mythos Studio ou les projets CMake se compilent depuis le terminal. Sous
Android, il faut Android Studio, le plugin Gradle et le NDK, qui compilent les
projets exportés par Mythos Studio ; la machine hôte peut être Windows, macOS
ou Linux. Les versions indiquées sont des minimums testés, pas des
recommandations : les versions actuelles de chaque outil fonctionnent sans
modification.

- __Standard C++__ : 17 (20 lors d'une compilation avec `HIGGS_USE_WINDOWS_MIDI_SERVICES` activé)
- __macOS/iOS__ : Xcode 12.4 (macOS 10.15.4 sur Intel, macOS 11.0 sur Apple Silicon)
- __Windows__ : Visual Studio 2019 (Windows 10 version 1607)
- __Linux__ : g++ 7.0 ou Clang 6.0 (voir [docs/Linux Dependencies.md](/docs/Linux%20Dependencies.md))
- __Android__ : Android Studio Narwhal 3 (2025.1.3) avec Android Gradle plugin 8.13.2
  et NDK 28 sous Windows, macOS ou Linux

### Plateformes cibles

Les valeurs suivantes sont les versions minimales du système d'exploitation sur
lesquelles s'exécutent les applications et plug-ins compilés avec le framework,
ainsi que les architectures de processeur pour lesquelles des binaires sont
produits. Sous macOS et Windows, il est courant de produire des binaires
universels ou plusieurs architectures depuis le même projet ; sous Android,
Mythos Studio génère les ABI sélectionnées dans un seul paquet. Choisir une
version minimale plus élevée pour un projet donné (par exemple macOS 11 au lieu
de 10.11) est possible depuis les réglages de l'exportateur dans Mythos Studio
ou avec les variables habituelles de CMake, et permet d'utiliser des API plus
récentes du système ; descendre sous les valeurs de la liste n'est pas pris en
charge, car le framework utilise des API qui n'existent pas dans les versions
antérieures. Sous iOS, le simulateur x86_64 permet de tester sur les Mac à
processeur Intel, et les appareils physiques utilisent Arm64 ou Arm64e. Sous
Android, il est conseillé d'inclure au moins arm64-v8a, l'architecture de tous
les appareils actuels, et d'ajouter les autres seulement lorsque le public
visé le justifie, car chaque ABI supplémentaire augmente la taille du paquet.

- __macOS__ : macOS 10.11 (x86_64, Arm64)
- __Windows__ : Windows 10 version 1607 (x86_64, x86, Arm64, Arm64EC)
- __Linux__ : distributions principales (x86_64, Arm64/aarch64)
- __iOS__ : iOS 12 (Arm64, Arm64e, simulateur x86_64)
- __Android__ : Android 7 - Nougat (API Level 24) (arm64-v8a, armeabi-v7a, x86_64, x86)

## Contribuer

Les contributions sont reçues via GitHub. Les demandes de fonctionnalités et
les questions générales vont dans la section *Discussions* du dépôt ; les bugs
reproductibles sont signalés comme *issues* en indiquant la version de Higgs
Mythos, les modules concernés, la plateforme et une reproduction minimale, en
utilisant les modèles de `.github/ISSUE_TEMPLATE` lorsque c'est possible. Les
vulnérabilités de sécurité ne sont jamais publiées dans une *issue* : elles
sont communiquées en privé par le canal décrit dans
[SECURITY.md](SECURITY.md).

Chaque *pull request* doit se concentrer sur un seul changement et décrire ce
qu'elle corrige ou ajoute. Avant de l'ouvrir, il faut compiler les cibles
concernées et exécuter les tests unitaires avec `UnitTestRunner`. Le code, les
commentaires et les messages de commit sont rédigés en anglais au format
`area: résumé`. En soumettant une contribution, on accepte de la placer sous les
mêmes termes que le projet (AGPL-3.0-or-later) et on confirme être l'auteur du
code ou avoir le droit de le soumettre ; le code copié de binaires
propriétaires ou de sources dont la licence est incompatible avec l'AGPL n'est
pas accepté. Le guide complet se trouve dans
[.github/contributing.md](.github/contributing.md), et
[CODE_OF_CONDUCT.md](CODE_OF_CONDUCT.md) établit les règles de conduite de la
communauté.

## Licence

Higgs Mythos est un logiciel libre : il peut être redistribué et modifié selon
les termes de la GNU Affero General Public License publiée par la Free Software
Foundation, dans sa version 3 ou, au choix de l'utilisateur, toute version
ultérieure (**AGPL-3.0-or-later**). Le texte complet et les notes sur son
application se trouvent dans [LICENSE.md](LICENSE.md). L'AGPL est une licence
*copyleft* : toute œuvre dérivée distribuée, ou proposée aux utilisateurs via
un réseau, doit être publiée sous la même licence avec son code source
disponible. Cela inclut les applications et les plug-ins qui lient le
framework.

Higgs Mythos dérive d'une base de code open source antérieure dont la marque a
été entièrement retirée des noms, des modules, des macros et des ressources.
Les mentions de copyright des auteurs originaux sont conservées intactes dans
l'en-tête de chaque fichier source, comme l'exige la licence, et ne doivent
être ni modifiées ni supprimées. Les exemples de `examples` sont publiés sous
licence ISC, bien plus permissive, afin de pouvoir être copiés comme base d'un
nouveau projet. L'inventaire des dépendances tierces et de leurs licences se
trouve dans [HiggsMythos.spdx.json](HiggsMythos.spdx.json), au format SPDX, et
les SDK de plug-ins (VST3, AAX, ARA, LV2) sont utilisés sous leurs propres
licences et accords sans être inclus dans le dépôt lorsque celles-ci ne le
permettent pas.

## Plug-ins AAX

Les plug-ins AAX doivent être signés numériquement avec les outils de signature
de PACE Anti-Piracy avant de pouvoir s'exécuter dans les versions commerciales
de Pro Tools. Avid fournit ces outils gratuitement. Avant de les obtenir, il
faut utiliser une version spéciale de Pro Tools, appelée Pro Tools Developer,
pour tester les plug-ins non signés. Les étapes pour obtenir Pro Tools
Developer sont les suivantes :

1. S'inscrire comme développeur AAX sur <https://developer.avid.com/aax/>.
2. Demander un code d'activation du Pro Tools Developer Bundle en écrivant à
   <devauth@avid.com>.
3. Télécharger la dernière version de Pro Tools Developer depuis son compte
   développeur Avid.

Lorsque les plug-ins ont été testés et débogués dans Pro Tools Developer et
qu'ils sont prêts à être signés, il faut écrire à <audiosdk@avid.com> avec
l'objet « PACE Eden Signing Tools Request ». Le message doit inclure une
description de chaque plug-in ainsi qu'un enregistrement d'écran montrant le
plug-in en fonctionnement dans Pro Tools Developer, avec audio si possible, en
plus des informations suivantes :

- Nom de l'entreprise
- Nom complet de l'administrateur
- Numéro de téléphone

Une fois la demande envoyée, PACE Anti-Piracy prendra contact directement avec
des informations sur la signature des plug-ins. La cible AAX de
`higgs_add_plugin` produit le paquet `.aaxplugin` prêt à signer ; le chemin du
SDK AAX s'indique avec `higgs_set_aax_sdk_path` avant de déclarer le plug-in.
