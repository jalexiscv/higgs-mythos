# Higgs Mythos

**言語：** [Español](README.md) ·
[English](README.en.md) ·
[Português](README.pt.md) ·
[Français](README.fr.md) ·
[Deutsch](README.de.md) ·
[Italiano](README.it.md) ·
[中文](README.zh-CN.md) ·
日本語

<p align="center"><img src="documentacion/contenido/assets/higgs_mythos_banner.svg" alt="Higgs Mythos" width="100%"></p>

*Higgs Mythos のマスコット。テキスト版：[higgs_mythos_mascot.txt](documentacion/contenido/assets/higgs_mythos_mascot.txt)。*

Higgs Mythos（コードおよびモジュール名では **HIGGS** と略記）は、デスクトップ
およびモバイルアプリケーションを作成するためのオープンソースのクロスプラット
フォーム C++ アプリケーションフレームワークです。VST、VST3、AU、AUv3、AAX、
LV2 形式のオーディオプラグインと、それらを読み込むホストの開発にも対応して
います。同じソースコードが API を変えることなく macOS、Windows、Linux、iOS、
Android 向けにビルドできます。フレームワークは各オペレーティングシステムの
ウィンドウシステム、2D 描画、オーディオ、MIDI を共通のクラスの背後にカプセル
化し、プロジェクトはビルドすべき内容だけを記述します。

このフレームワークは CMake を通じて既存のプロジェクトに統合できるほか、
プロジェクト設定ツールである **Mythos Studio** から使用することもできます。
Mythos Studio は Xcode（macOS と iOS）、Visual Studio、Android Studio、Linux
Makefile 向けのプロジェクトを書き出し、ソースコードエディタも備えています。
生産性ツールやシステムユーティリティのような GUI を持つ汎用アプリケーションと、
リアルタイム処理、厳密なスレッド管理、デジタルオーディオワークステーションが
使用するプラグイン形式との互換性を要求するプロフェッショナル向けオーディオ
ソフトウェアの両方を想定して設計されています。

Higgs Mythos は **AGPL-3.0-or-later** ライセンスの下で配布されています。
原著作者の著作権表示は各ソースファイルに保持されており、サンプルは追加の
制約なしに自分のプロジェクトの出発点としてコピーできるよう ISC ライセンスで
公開されています。

## はじめに

フレームワークは `modules` ディレクトリにあります。各モジュールは
`higgs_<領域>` という名前で、公開ヘッダーは `<モジュール>/<モジュール>.h`、
C++ API はすべて `higgs` 名前空間で宣言されています。モジュールは自己完結した
単位です。メインヘッダーで、どのモジュールに依存するか、どの公開ヘッダーを
公開するか、各プラットフォームでどのシステムライブラリが必要かを宣言するため、
ビルドシステムは特定のプロジェクトのためにコンパイルすべき最小限のソース
集合を解決できます。

始めるには、必要なモジュールのヘッダーをインクルードし、対応するクラスから
派生させるだけです。次の例は初期サイズ 800 × 600 ピクセルの GUI コンポーネント
を作成します。`Component` は、ウィンドウ全体からボタンに至るまで、フレーム
ワーク内のあらゆる視覚要素の基底クラスです。

```cpp
#include <higgs_core/higgs_core.h>
#include <higgs_gui_basics/higgs_gui_basics.h>

class MainComponent final : public higgs::Component
{
public:
    MainComponent() { setSize (800, 600); }
};
```

設定マクロは `HIGGS_` 接頭辞を保持しており（例：`HIGGS_MAJOR_VERSION`、
`HIGGS_MODULE_AVAILABLE_higgs_core`）、バージョン番号は
[higgs_StandardHeader.h](/modules/higgs_core/system/higgs_StandardHeader.h)
で定義されています。プロジェクトがシステム固有のコードを必要とする場合、
`HIGGS_WINDOWS`、`HIGGS_MAC`、`HIGGS_LINUX`、`HIGGS_IOS`、`HIGGS_ANDROID` の
各マクロがターゲットプラットフォームを識別します。その他の命名規則（名前空間、
CMake 関数、`.mythos` プロジェクト拡張子）は
[docs/higgs-mythos.md](/docs/higgs-mythos.md) にまとめられています。

## モジュール

Higgs Mythos は 24 個の独立したモジュールで構成され、各プロジェクトの必要に
応じて組み合わせます。コンソールアプリケーションは `higgs_core` だけを使う
こともできます。GUI アプリケーションは `higgs_events`、`higgs_graphics`、
`higgs_gui_basics` を追加し、オーディオプラグインはさらにオーディオモジュール
と `higgs_audio_plugin_client` を追加します。各モジュールはリンクされるときに
のみコンパイルされるため、最終的なバイナリは使用しない領域のコードを含み
ません。次の表は各モジュールの目的をまとめたものです。各モジュールの完全な
ドキュメントはドキュメントサイトと Doxygen ヘッダーにあります。

モジュールは層ごとにまとまっています。基盤は `higgs_core` で、他のすべての
モジュールがこれに依存します。その上に `higgs_events` と
`higgs_data_structures` が乗り、これらは GUI を必要としないため、コンソール
ツールやバックグラウンドプロセスにも使えます。グラフィックス層
（`higgs_graphics`、`higgs_gui_basics`、`higgs_gui_extra`、`higgs_animation`、
`higgs_opengl`）がユーザーインターフェースを構築し、オーディオ層はデータ
（`higgs_audio_basics`）からデバイス、ファイル形式、プロセッサ、プラグイン
形式までを扱います。各モジュールはメインヘッダーで依存関係を宣言しており、
プロジェクトに追加すると Mythos Studio または CMake がそれらを自動的に解決
します。

| モジュール | 目的 |
|---|---|
| `higgs_core` | 基本クラス：文字列、コンテナ、メモリ、スレッド、ファイル、ネットワーク、XML、JSON、ZIP、時刻、単体テスト。 |
| `higgs_events` | メッセージループ、タイマー、非同期メッセージ、ブロードキャスターとリスナー、アプリケーション基底クラス、IPC。 |
| `higgs_data_structures` | `ValueTree`、`Value`、`UndoManager`、設定の管理。 |
| `higgs_cryptography` | RSA、Blowfish、素数、MD5・SHA-256・Whirlpool ダイジェスト。 |
| `higgs_graphics` | 2D エンジン：`Graphics`、幾何、色、フォント、画像、SVG、エフェクト。 |
| `higgs_gui_basics` | `Component`、ウィンドウ、ウィジェット、メニュー、レイアウト、ルック＆フィール、アクセシビリティ。 |
| `higgs_gui_extra` | コードエディタ、`WebBrowserComponent`、カラーセレクタ、システムトレイ、通知。 |
| `higgs_animation` | 画面更新に同期したイージング関数付きの宣言的アニメーション。 |
| `higgs_opengl` | コンポーネント内での OpenGL および OpenGL ES レンダリング。 |
| `higgs_video` | 動画再生とカメラキャプチャ。 |
| `higgs_audio_basics` | `AudioBuffer`、オーディオソース、シンセサイザー、MIDI 1.0、MPE、MIDI 2.0（UMP）。 |
| `higgs_audio_devices` | CoreAudio、WASAPI、ASIO、ALSA、JACK、Oboe、OpenSL ES によるオーディオと MIDI の入出力。 |
| `higgs_audio_formats` | WAV、AIFF、FLAC、Ogg Vorbis、Opus、MP3 などの読み書き。 |
| `higgs_audio_processors_headless` | `AudioProcessor`、パラメータ、`AudioProcessorGraph`、ヘッドレスなプラグインホスティング。 |
| `higgs_audio_processors` | プロセッサエディタ、`AudioProcessorValueTreeState`、プラグインスキャン。 |
| `higgs_audio_plugin_client` | VST2、VST3、AU、AUv3、AAX、LV2、Unity、Standalone のラッパー。 |
| `higgs_audio_utils` | `AudioAppComponent`、デバイスセレクタ、オーディオサムネイル、MIDI キーボード。 |
| `higgs_dsp` | フィルタ、オシレータ、ダイナミクス、リバーブ、畳み込み、FFT、SIMD。 |
| `higgs_midi_ci` | MIDI Capability Inquiry：検出、プロファイル、プロパティ。 |
| `higgs_osc` | UDP 上の Open Sound Control。 |
| `higgs_javascript` | QuickJS ベースの JavaScript インタープリタ。 |
| `higgs_box2d` | レンダラー統合済みの 2D 物理エンジン Box2D。 |
| `higgs_analytics` | 利用統計の収集とバッチ送信。 |
| `higgs_product_unlocking` | アプリ内課金とオンラインアンロック付きライセンス。 |

## CMake

CMake 3.22 以上が必要です。環境が整っているかを確認する最も直接的な方法は、
リポジトリに同梱されたツールとサンプルをビルドすることです。最初のコマンドは
`cmake-build` ディレクトリにビルドツリーを構成してサンプルとエクストラを有効化
し、2 番目のコマンドはフレームワークのすべてのデモを集めたアプリケーション
`DemoRunner` をビルドします。

```bash
cmake . -B cmake-build -DHIGGS_BUILD_EXAMPLES=ON -DHIGGS_BUILD_EXTRAS=ON
cmake --build cmake-build --target DemoRunner
```

Windows では、Ninja でも MSBuild でも、CMake が MSVC コンパイラを見つけられる
ように、先に Visual Studio の環境（`vcvars64.bat`）を読み込む必要があります。
Linux では、構成の前に
[docs/Linux Dependencies.md](/docs/Linux%20Dependencies.md) に列挙された
開発パッケージをインストールする必要があります。

CMake API は `higgs_` 接頭辞を使用します。`higgs_add_gui_app` は GUI
アプリケーションを、`higgs_add_console_app` はコンソールアプリケーションを、
`higgs_add_plugin` は形式ごとに 1 つのターゲットを持つプラグインを作成し、
`higgs_add_module` は独自モジュールを登録し、`higgs_generate_higgs_header`
はプロジェクトの集約ヘッダーを生成します。リソースを埋め込む
`higgs_add_binary_data` や、VST2、AAX、ARA の SDK のパスを指定する
`higgs_set_<kind>_sdk_path` もあります。インポートされたターゲットは
`higgs::higgs_core`、`higgs::higgs_gui_basics` などの名前を持ち、他のライブラリ
と同様に `target_link_libraries` でリンクします。既存のプロジェクトにフレーム
ワークを統合するには、リポジトリのコピーに対して `add_subdirectory` を呼ぶか、
インストール済みのものに対して `find_package(HIGGS)` を呼ぶだけです。完全な
リファレンスは [docs/CMake API.md](/docs/CMake%20API.md) にあり、
[examples/CMake](/examples/CMake) には出発点としてコピーできる 3 つの最小
プロジェクト（`GuiApp`、`ConsoleApp`、`AudioPlugin`）が含まれています。

## Mythos Studio

Mythos Studio は `extras/MythosStudio` に同梱されているプロジェクトジェネレータ
です。プロジェクトの設定を `.mythos` ファイルに保存し、そこから各 IDE と
プラットフォーム向けのネイティブプロジェクトを書き出します。対象は macOS と
iOS 向けの Xcode、Visual Studio 2019・2022・2026、Linux Makefile、Gradle を
使う Android Studio です。1 つの `.mythos` ファイルは複数のエクスポータを同時に
持つことができ、それぞれが独自の Debug・Release 構成、SDK パス、コンパイラと
リンカのオプションを持つため、プロジェクトは一か所で管理され、モジュールや
ソースファイルが変わると IDE プロジェクトが再生成されます。

このツールには、アプリケーションとプラグインのテンプレートを備えたスタート
ページ、依存関係を解決するモジュールマネージャ、構文ハイライト付きのコード
エディタ、そしてウィンドウを開かずにタスクを自動化するためのコマンドライン
インターフェース（`--resave`、`--set-version`、`--encode-binary`、
`--create-project-from-pip` など）が含まれています。付属ツールの `higgsaide`
は、プロジェクトが埋め込むバイナリデータやその他のリソースをターミナルから
生成します。CMake API はこのプラットフォーム向けのプロジェクトを生成しない
ため、Android は Mythos Studio を通じてのみサポートされます。

Mythos Studio は `extras/MythosStudio/Builds`（Visual Studio、Xcode、Linux
Makefile のプロジェクト）からビルドするか、CMake で `-DHIGGS_BUILD_EXTRAS=ON`
と `MythosStudio` ターゲットを指定してビルドします。GitHub Actions ワークフロー
`HIGGS Mythos Studio Build` が生成する、Windows x64、macOS ユニバーサル、Linux
x64 向けの署名されていないビルド済み実行ファイルもあり、各実行のアーティ
ファクトとしてダウンロードできます。

## ツールとサンプル

Mythos Studio のほかに、`extras` ディレクトリにはフレームワークに付属し
`-DHIGGS_BUILD_EXTRAS=ON` でビルドされるツールが集められています。
`AudioPluginHost` は完全なプラグインホストで、システムにインストールされた
形式をスキャンし、それらをプロセッサグラフに接続し、商用ワークステーションに
頼らずに自作プラグインをテストできます。`UnitTestRunner` は各モジュールが
`UnitTest` クラスで宣言する単体テストを実行します。カテゴリや名前で絞り込む
ことができ、プロジェクトの検証基準となっています。`BinaryBuilder` はファイルを
埋め込みデータに変換し、`AudioPerformanceTest` はリアルタイムオーディオ処理の
性能を測定し、`NetworkGraphicsDemo` は複数のマシン間の分散描画を実演し、
`SBOM` はサードパーティ依存関係の一覧を生成・検証します。

`examples` ディレクトリには、ほとんどの機能について完全でビルド可能なデモが
領域別に収められています：`GUI`、`Audio`、`DSP`、`Plugins`、`Utilities`、
`Assets`。これらはすべて PIP です。PIP とは、メタデータブロック（名前、依存
関係、種類、メインクラス）で始まる単一のヘッダーファイルに収められた完全な
プロジェクトで、`DemoRunner` がそれを読み込んでソースコードと並べて表示する
ため、各デモの実装を実行中に読むことができます。`examples/CMake` には前述の
3 つの最小プロジェクトが含まれ、`examples/Plugins` には `AudioProcessor` の
完全なライフサイクルの参考となる完全なプラグイン（シンセサイザー、サンプラー、
アルペジエータ、ノイズゲート、プラグイン内ホスト、ARA のサンプル）が含まれて
います。

## ドキュメント

スペイン語の完全なユーザーガイドは <https://jalexiscv.github.io/higgs-mythos/>
で公開されており、そのソースは [documentacion](/documentacion) にあります。
入門（インストール、最初のアプリケーション、最初のプラグイン）、フレーム
ワークの概念（アーキテクチャ、メッセージループ、スレッド、メモリ、規約）、
領域ごとのガイド（GUI、グラフィックス、オーディオ、MIDI、DSP、プラグイン、
ホスティング、OpenGL、動画、Web、アクセシビリティ、ローカライズ）、モジュール
ごとのリファレンスページ、CMake API、ツール、プロジェクトノート（非互換の
変更、ライセンス、セキュリティ、用語集）を扱っています。サイトは MkDocs で
構築され、`main` ブランチでドキュメント、モジュール、サンプルが変更されると
GitHub Pages に自動的にデプロイされます。デプロイのたびにリファレンスページは
ソースツリーから再生成されます。

API ドキュメントはモジュールヘッダーの Doxygen コメントから直接生成され、
ソースコードこそが最も完全で最新のドキュメントです。各モジュールのメイン
ヘッダー `higgs_<モジュール>/higgs_<モジュール>.h` は公開ヘッダーと依存関係を
列挙しています。モジュール形式は
[docs/Higgs Mythos Module Format.md](/docs/Higgs%20Mythos%20Module%20Format.md)
に記載されています。クラスを探すには、ヘッダー内でその宣言を検索し、その上に
あるコメントブロックを読むだけです。

```bash
grep -rlE "^\s*(class|struct)\s+(HIGGS_API\s+)?AudioProcessorValueTreeState\b" modules
```

[BREAKING_CHANGES.md](BREAKING_CHANGES.md) はバージョンごとに何が変わり、
コードをどう更新すべきかを説明し、[CHANGE_LIST.md](CHANGE_LIST.md) は各
バージョンの要点をまとめ、[docs/README.md](/docs/README.md) はリポジトリの
技術ドキュメント（モジュール形式、CMake API、フレームワークのアイデンティティ、
作業プロトコル）の索引です。

## 最小システム要件

Higgs Mythos は公開ヘッダーが C++17 の機能を使用するため、最低基準として C++17
を要求します（各モジュールはメインヘッダーの `minimumCppStandard` フィールドで
これを宣言しています）。`HIGGS_USE_WINDOWS_MIDI_SERVICES` を有効にした場合、
Windows MIDI Services SDK が要求するため、プロジェクトは C++20 でビルドする
必要があります。以下に示すコンパイラのバージョンはテストされている最も古い
ものです。より新しいバージョンの Xcode、Visual Studio、GCC、Clang は変更なしで
動作します。Linux ではコンパイラに加えて、
[docs/Linux Dependencies.md](/docs/Linux%20Dependencies.md) に列挙された X11、
FreeType、ALSA などの開発ライブラリが必要です。Android では、Mythos Studio が
書き出すプロジェクトを通じて、Android Studio の Gradle と NDK でビルドします。
以下の 2 つのリストは、開発マシンと実行マシンを区別しています。最初のリストは
何でビルドするかを、2 番目のリストは成果物がどこで実行されるかを示します。
たとえば Windows Arm64 向けプラグインは、互換性のある任意の Windows マシン上で
Visual Studio からビルドでき、iOS と Android のアプリケーションはそれぞれ
macOS から、または Android Studio を備えたデスクトップからビルドします。

### HIGGS プロジェクトのビルド

このリストは開発プラットフォームごとに、フレームワークをビルドできる最小限の
ツール、すなわち IDE またはコンパイラと、開発マシンのオペレーティングシステム
のバージョンをまとめたものです。macOS と iOS では、Clang と Apple の SDK を
含む Xcode を使用します。Xcode 12.4 はアーキテクチャごとに異なるシステムを
要求するため、最小システムバージョンはマシンが Intel か Apple Silicon かに
よって異なります。Windows では、「C++ によるデスクトップ開発」ワークロードと
Windows 10 SDK を備えた Visual Studio を、IDE からも、`vcvars64.bat` を読み
込んだ後のコマンドラインからも使用します。Linux では、最新の GCC または Clang
とシステムの開発ライブラリがあれば十分で、Mythos Studio が書き出す Makefile
や CMake プロジェクトはターミナルからビルドします。Android では Android
Studio、Gradle プラグイン、NDK が必要で、これらで Mythos Studio が書き出す
プロジェクトをビルドします。ホストマシンは Windows、macOS、Linux のいずれでも
構いません。記載されたバージョンはテスト済みの最小値であって推奨値ではなく、
各ツールの現行バージョンは変更なしで動作します。

- __C++ 標準__：17（`HIGGS_USE_WINDOWS_MIDI_SERVICES` を有効にしてビルドする場合は 20）
- __macOS/iOS__：Xcode 12.4（Intel は macOS 10.15.4、Apple Silicon は macOS 11.0）
- __Windows__：Visual Studio 2019（Windows 10 バージョン 1607）
- __Linux__：g++ 7.0 または Clang 6.0（[docs/Linux Dependencies.md](/docs/Linux%20Dependencies.md) を参照）
- __Android__：Android Studio Narwhal 3（2025.1.3）、Android Gradle プラグイン 8.13.2
  と NDK 28、Windows・macOS・Linux 上で利用可能

### デプロイ対象

以下の値は、フレームワークでビルドしたアプリケーションとプラグインが動作する
最小のオペレーティングシステムバージョンと、バイナリが生成されるプロセッサ
アーキテクチャです。macOS と Windows では、同じプロジェクトからユニバーサル
バイナリや複数のアーキテクチャを生成するのが一般的です。Android では、Mythos
Studio が選択した ABI を 1 つのパッケージに生成します。特定のプロジェクトで
より高い最小バージョン（たとえば 10.11 ではなく macOS 11）を選ぶことは、
Mythos Studio のエクスポータ設定や通常の CMake 変数から可能で、より新しい
システム API を使えるようになります。リストの値を下回ることはサポートされて
いません。フレームワークが以前のバージョンには存在しない API を使用している
ためです。iOS では、x86_64 シミュレータにより Intel Mac でテストでき、実機は
Arm64 または Arm64e を使用します。Android では、現行のすべてのデバイスの
アーキテクチャである arm64-v8a を少なくとも含め、ABI を追加するごとにパッケージ
サイズが増えるため、その他は対象ユーザーが必要とする場合にのみ追加することを
お勧めします。

- __macOS__：macOS 10.11（x86_64、Arm64）
- __Windows__：Windows 10 バージョン 1607（x86_64、x86、Arm64、Arm64EC）
- __Linux__：主要なディストリビューション（x86_64、Arm64/aarch64）
- __iOS__：iOS 12（Arm64、Arm64e、x86_64 シミュレータ）
- __Android__：Android 7 - Nougat（API Level 24）（arm64-v8a、armeabi-v7a、x86_64、x86）

## 貢献

貢献は GitHub を通じて受け付けています。機能要望や一般的な質問はリポジトリの
*Discussions* セクションへ投稿してください。再現可能なバグは、Higgs Mythos の
バージョン、影響を受けるモジュール、プラットフォーム、最小限の再現手順を記載
した issue として報告し、可能な限り `.github/ISSUE_TEMPLATE` のテンプレートを
使用してください。セキュリティ脆弱性は決して issue に公開せず、
[SECURITY.md](SECURITY.md) に記載された経路で非公開に報告します。

各 pull request は 1 つの変更に集中し、何を修正または追加するのかを説明する
必要があります。作成前に、影響を受けるターゲットをビルドし、`UnitTestRunner`
で単体テストを実行してください。コード、コメント、コミットメッセージは英語で、
`area: summary` の形式で書きます。貢献を提出することで、プロジェクトと同じ
条件（AGPL-3.0-or-later）でライセンスすることに同意し、コードを自分で書いた
か提出する権利があることを確認したことになります。プロプライエタリなバイナリ
や AGPL と互換性のないライセンスのソースからコピーしたコードは受け付けられ
ません。完全なガイドは [.github/contributing.md](.github/contributing.md) に
あり、[CODE_OF_CONDUCT.md](CODE_OF_CONDUCT.md) はコミュニティの行動規範を
定めています。

## ライセンス

Higgs Mythos はフリーソフトウェアです。Free Software Foundation が公開する GNU
Affero General Public License のバージョン 3、またはユーザーの選択によりそれ
以降の任意のバージョン（**AGPL-3.0-or-later**）の条件に従って再配布および
改変できます。全文と適用に関する注記は [LICENSE.md](LICENSE.md) にあります。
AGPL は *コピーレフト* ライセンスです。配布される、あるいはネットワークを通じて
ユーザーに提供されるあらゆる派生物は、同じライセンスの下でソースコードを利用
可能にして公開しなければなりません。これにはフレームワークをリンクする
アプリケーションとプラグインが含まれます。

Higgs Mythos は以前のオープンソースのコードベースから派生しており、その
ブランドは名前、モジュール、マクロ、リソースから完全に取り除かれています。
原著作者の著作権表示は、ライセンスが要求するとおり各ソースファイルの先頭に
そのまま保持されており、編集や削除をしてはなりません。`examples` のサンプルは、
新しいプロジェクトの土台としてコピーできるよう、はるかに緩やかな ISC ライセンス
で公開されています。サードパーティ依存関係とそのライセンスの一覧は SPDX 形式で
[HiggsMythos.spdx.json](HiggsMythos.spdx.json) にあり、プラグイン SDK（VST3、
AAX、ARA、LV2）はそれぞれのライセンスと契約の下で使用され、それらが許可しない
場合はリポジトリに含まれません。

## AAX プラグイン

AAX プラグインは、市販版の Pro Tools で動作させる前に、PACE Anti-Piracy の
署名ツールでデジタル署名する必要があります。これらのツールは Avid が無償で
提供しています。署名ツールを入手する前に、署名されていないプラグインをテスト
するために、Pro Tools Developer と呼ばれる特別なビルドの Pro Tools を使用する
必要があります。Pro Tools Developer を入手する手順は次のとおりです。

1. <https://developer.avid.com/aax/> で AAX 開発者として登録する。
2. <devauth@avid.com> にメールして Pro Tools Developer Bundle のアクティ
   ベーションコードを申請する。
3. Avid 開発者アカウントから最新の Pro Tools Developer ビルドをダウンロード
   する。

プラグインを Pro Tools Developer でテストおよびデバッグし、デジタル署名の準備が
できたら、件名を「PACE Eden Signing Tools Request」として <audiosdk@avid.com>
にメールしてください。各プラグインの概要と、Pro Tools Developer でプラグインが
動作している様子の画面録画（可能であれば音声付き）を添付し、さらに以下の情報を
含めてください。

- 会社名
- 管理者の氏名
- 電話番号

申請が送信されると、PACE Anti-Piracy からプラグインの署名に関する情報が直接
連絡されます。`higgs_add_plugin` の AAX ターゲットは署名可能な `.aaxplugin`
バンドルを生成します。AAX SDK のパスは、プラグインを宣言する前に
`higgs_set_aax_sdk_path` で指定します。
