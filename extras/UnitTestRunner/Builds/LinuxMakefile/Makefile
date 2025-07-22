# Automatically generated makefile, created by the MythosStudio
# Don't edit this file! Your changes will be overwritten when you re-save the MythosStudio project!

# build with "V=1" for verbose builds
ifeq ($(V), 1)
V_AT =
else
V_AT = @
endif

# (this disables dependency generation if multiple architectures are set)
DEPFLAGS := $(if $(word 2, $(TARGET_ARCH)), , -MMD)

ifndef PKG_CONFIG
  PKG_CONFIG=pkg-config
endif

ifndef STRIP
  STRIP=strip
endif

ifndef AR
  AR=ar
endif

ifndef CONFIG
  CONFIG=Debug
endif

HIGGS_ARCH_LABEL := $(shell uname -m)

ifeq ($(CONFIG),Debug)
  HIGGS_BINDIR := build
  HIGGS_LIBDIR := build
  HIGGS_OBJDIR := build/intermediate/Debug
  HIGGS_OUTDIR := build

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := 
  endif

  HIGGS_CPPFLAGS := $(DEPFLAGS) "-DLINUX=1" "-DDEBUG=1" "-D_DEBUG=1" "-DHIGGS_MYTHOSSTUDIO_VERSION=0x90005" "-DHIGGS_MODULE_AVAILABLE_higgs_analytics=1" "-DHIGGS_MODULE_AVAILABLE_higgs_audio_basics=1" "-DHIGGS_MODULE_AVAILABLE_higgs_audio_devices=1" "-DHIGGS_MODULE_AVAILABLE_higgs_audio_formats=1" "-DHIGGS_MODULE_AVAILABLE_higgs_audio_processors=1" "-DHIGGS_MODULE_AVAILABLE_higgs_audio_processors_headless=1" "-DHIGGS_MODULE_AVAILABLE_higgs_audio_utils=1" "-DHIGGS_MODULE_AVAILABLE_higgs_core=1" "-DHIGGS_MODULE_AVAILABLE_higgs_cryptography=1" "-DHIGGS_MODULE_AVAILABLE_higgs_data_structures=1" "-DHIGGS_MODULE_AVAILABLE_higgs_dsp=1" "-DHIGGS_MODULE_AVAILABLE_higgs_events=1" "-DHIGGS_MODULE_AVAILABLE_higgs_graphics=1" "-DHIGGS_MODULE_AVAILABLE_higgs_gui_basics=1" "-DHIGGS_MODULE_AVAILABLE_higgs_gui_extra=1" "-DHIGGS_MODULE_AVAILABLE_higgs_javascript=1" "-DHIGGS_MODULE_AVAILABLE_higgs_midi_ci=1" "-DHIGGS_MODULE_AVAILABLE_higgs_opengl=1" "-DHIGGS_MODULE_AVAILABLE_higgs_osc=1" "-DHIGGS_MODULE_AVAILABLE_higgs_product_unlocking=1" "-DHIGGS_WEBVIEW_INTEROP_LIBRARY_VERSION=\"1.0.0\"" "-DHIGGS_GLOBAL_MODULE_SETTINGS_INCLUDED=1" "-DHIGGS_PLUGINHOST_VST3=1" "-DHIGGS_PLUGINHOST_LV2=1" "-DHIGGS_STRICT_REFCOUNTEDPOINTER=1" "-DHIGGS_STANDALONE_APPLICATION=1" "-DHIGGS_UNIT_TESTS=1" "-DMYTHOS_LINUX_MAKE_6D53C8B4=1" "-DHIGGS_APP_VERSION=1.0.0" "-DHIGGS_APP_VERSION_HEX=0x10000" $(shell $(PKG_CONFIG) --cflags $(shell ($(PKG_CONFIG) --exists webkit2gtk-4.1 && echo webkit2gtk-4.1) || echo webkit2gtk-4.0) alsa freetype2 fontconfig egl gl libcurl gtk+-x11-3.0) -pthread -I../../../../modules/higgs_audio_processors_headless/format_types/LV2_SDK/lilv/src -I../../../../modules/higgs_audio_processors_headless/format_types/LV2_SDK/lilv -I../../../../modules/higgs_audio_processors_headless/format_types/LV2_SDK/sratom -I../../../../modules/higgs_audio_processors_headless/format_types/LV2_SDK/sord/src -I../../../../modules/higgs_audio_processors_headless/format_types/LV2_SDK/sord -I../../../../modules/higgs_audio_processors_headless/format_types/LV2_SDK/serd -I../../../../modules/higgs_audio_processors_headless/format_types/LV2_SDK/lv2 -I../../../../modules/higgs_audio_processors_headless/format_types/LV2_SDK -I../../../../modules/higgs_audio_processors_headless/format_types/VST3_SDK -I../../HiggsLibraryCode -I../../../../modules $(CPPFLAGS)
  HIGGS_CPPFLAGS_CONSOLEAPP :=  "-DHiggsPlugin_Build_VST=0" "-DHiggsPlugin_Build_VST3=0" "-DHiggsPlugin_Build_AU=0" "-DHiggsPlugin_Build_AUv3=0" "-DHiggsPlugin_Build_AAX=0" "-DHiggsPlugin_Build_Standalone=0" "-DHiggsPlugin_Build_Unity=0" "-DHiggsPlugin_Build_LV2=0"
  HIGGS_TARGET_CONSOLEAPP := UnitTestRunner

  HIGGS_CFLAGS += $(HIGGS_CPPFLAGS) $(TARGET_ARCH) -g -ggdb -O0 $(CFLAGS)
  HIGGS_CXXFLAGS += $(HIGGS_CFLAGS) -std=c++17 $(CXXFLAGS)
  HIGGS_LDFLAGS += $(TARGET_ARCH) -L$(HIGGS_BINDIR) -L$(HIGGS_LIBDIR) $(shell $(PKG_CONFIG) --libs alsa freetype2 fontconfig egl gl libcurl) -fvisibility=hidden -lrt -ldl -lpthread $(LDFLAGS)

  CLEANCMD = rm -rf $(HIGGS_OUTDIR)/$(HIGGS_TARGET_CONSOLEAPP) $(HIGGS_OBJDIR)
endif

ifeq ($(CONFIG),Release)
  HIGGS_BINDIR := build
  HIGGS_LIBDIR := build
  HIGGS_OBJDIR := build/intermediate/Release
  HIGGS_OUTDIR := build

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := 
  endif

  HIGGS_CPPFLAGS := $(DEPFLAGS) "-DLINUX=1" "-DNDEBUG=1" "-DHIGGS_MYTHOSSTUDIO_VERSION=0x90005" "-DHIGGS_MODULE_AVAILABLE_higgs_analytics=1" "-DHIGGS_MODULE_AVAILABLE_higgs_audio_basics=1" "-DHIGGS_MODULE_AVAILABLE_higgs_audio_devices=1" "-DHIGGS_MODULE_AVAILABLE_higgs_audio_formats=1" "-DHIGGS_MODULE_AVAILABLE_higgs_audio_processors=1" "-DHIGGS_MODULE_AVAILABLE_higgs_audio_processors_headless=1" "-DHIGGS_MODULE_AVAILABLE_higgs_audio_utils=1" "-DHIGGS_MODULE_AVAILABLE_higgs_core=1" "-DHIGGS_MODULE_AVAILABLE_higgs_cryptography=1" "-DHIGGS_MODULE_AVAILABLE_higgs_data_structures=1" "-DHIGGS_MODULE_AVAILABLE_higgs_dsp=1" "-DHIGGS_MODULE_AVAILABLE_higgs_events=1" "-DHIGGS_MODULE_AVAILABLE_higgs_graphics=1" "-DHIGGS_MODULE_AVAILABLE_higgs_gui_basics=1" "-DHIGGS_MODULE_AVAILABLE_higgs_gui_extra=1" "-DHIGGS_MODULE_AVAILABLE_higgs_javascript=1" "-DHIGGS_MODULE_AVAILABLE_higgs_midi_ci=1" "-DHIGGS_MODULE_AVAILABLE_higgs_opengl=1" "-DHIGGS_MODULE_AVAILABLE_higgs_osc=1" "-DHIGGS_MODULE_AVAILABLE_higgs_product_unlocking=1" "-DHIGGS_WEBVIEW_INTEROP_LIBRARY_VERSION=\"1.0.0\"" "-DHIGGS_GLOBAL_MODULE_SETTINGS_INCLUDED=1" "-DHIGGS_PLUGINHOST_VST3=1" "-DHIGGS_PLUGINHOST_LV2=1" "-DHIGGS_STRICT_REFCOUNTEDPOINTER=1" "-DHIGGS_STANDALONE_APPLICATION=1" "-DHIGGS_UNIT_TESTS=1" "-DMYTHOS_LINUX_MAKE_6D53C8B4=1" "-DHIGGS_APP_VERSION=1.0.0" "-DHIGGS_APP_VERSION_HEX=0x10000" $(shell $(PKG_CONFIG) --cflags $(shell ($(PKG_CONFIG) --exists webkit2gtk-4.1 && echo webkit2gtk-4.1) || echo webkit2gtk-4.0) alsa freetype2 fontconfig egl gl libcurl gtk+-x11-3.0) -pthread -I../../../../modules/higgs_audio_processors_headless/format_types/LV2_SDK/lilv/src -I../../../../modules/higgs_audio_processors_headless/format_types/LV2_SDK/lilv -I../../../../modules/higgs_audio_processors_headless/format_types/LV2_SDK/sratom -I../../../../modules/higgs_audio_processors_headless/format_types/LV2_SDK/sord/src -I../../../../modules/higgs_audio_processors_headless/format_types/LV2_SDK/sord -I../../../../modules/higgs_audio_processors_headless/format_types/LV2_SDK/serd -I../../../../modules/higgs_audio_processors_headless/format_types/LV2_SDK/lv2 -I../../../../modules/higgs_audio_processors_headless/format_types/LV2_SDK -I../../../../modules/higgs_audio_processors_headless/format_types/VST3_SDK -I../../HiggsLibraryCode -I../../../../modules $(CPPFLAGS)
  HIGGS_CPPFLAGS_CONSOLEAPP :=  "-DHiggsPlugin_Build_VST=0" "-DHiggsPlugin_Build_VST3=0" "-DHiggsPlugin_Build_AU=0" "-DHiggsPlugin_Build_AUv3=0" "-DHiggsPlugin_Build_AAX=0" "-DHiggsPlugin_Build_Standalone=0" "-DHiggsPlugin_Build_Unity=0" "-DHiggsPlugin_Build_LV2=0"
  HIGGS_TARGET_CONSOLEAPP := UnitTestRunner

  HIGGS_CFLAGS += $(HIGGS_CPPFLAGS) $(TARGET_ARCH) -O3 $(CFLAGS)
  HIGGS_CXXFLAGS += $(HIGGS_CFLAGS) -std=c++17 $(CXXFLAGS)
  HIGGS_LDFLAGS += $(TARGET_ARCH) -L$(HIGGS_BINDIR) -L$(HIGGS_LIBDIR) $(shell $(PKG_CONFIG) --libs alsa freetype2 fontconfig egl gl libcurl) -fvisibility=hidden -lrt -ldl -lpthread $(LDFLAGS)

  CLEANCMD = rm -rf $(HIGGS_OUTDIR)/$(HIGGS_TARGET_CONSOLEAPP) $(HIGGS_OBJDIR)
endif

OBJECTS_CONSOLEAPP := \
  $(HIGGS_OBJDIR)/Main_90ebc5c2.o \
  $(HIGGS_OBJDIR)/include_higgs_analytics_f8e9fa94.o \
  $(HIGGS_OBJDIR)/include_higgs_audio_basics_8a4e984a.o \
  $(HIGGS_OBJDIR)/include_higgs_audio_devices_63111d02.o \
  $(HIGGS_OBJDIR)/include_higgs_audio_formats_15f82001.o \
  $(HIGGS_OBJDIR)/include_higgs_audio_formats_flac_1_32fae422.o \
  $(HIGGS_OBJDIR)/include_higgs_audio_formats_flac_2_32fae7e3.o \
  $(HIGGS_OBJDIR)/include_higgs_audio_formats_ogg_ccb28111.o \
  $(HIGGS_OBJDIR)/include_higgs_audio_formats_opus_ca23cec7.o \
  $(HIGGS_OBJDIR)/include_higgs_audio_formats_opusfile_c2979c43.o \
  $(HIGGS_OBJDIR)/include_higgs_audio_processors_10c03666.o \
  $(HIGGS_OBJDIR)/include_higgs_audio_processors_headless_1902bafc.o \
  $(HIGGS_OBJDIR)/include_higgs_audio_processors_headless_ara_8deeb88d.o \
  $(HIGGS_OBJDIR)/include_higgs_audio_processors_headless_lv2_libs_36180e32.o \
  $(HIGGS_OBJDIR)/include_higgs_audio_utils_9f9fb2d6.o \
  $(HIGGS_OBJDIR)/include_higgs_core_f26d17db.o \
  $(HIGGS_OBJDIR)/include_higgs_core_CompilationTime_9257742c.o \
  $(HIGGS_OBJDIR)/include_higgs_core_zlib_ac61cd39.o \
  $(HIGGS_OBJDIR)/include_higgs_cryptography_8cb807a8.o \
  $(HIGGS_OBJDIR)/include_higgs_data_structures_7471b1e3.o \
  $(HIGGS_OBJDIR)/include_higgs_dsp_aeb2060f.o \
  $(HIGGS_OBJDIR)/include_higgs_events_fd7d695.o \
  $(HIGGS_OBJDIR)/include_higgs_graphics_f817e147.o \
  $(HIGGS_OBJDIR)/include_higgs_graphics_Harfbuzz_60c52ba2.o \
  $(HIGGS_OBJDIR)/include_higgs_graphics_libjpg_1_ce62d530.o \
  $(HIGGS_OBJDIR)/include_higgs_graphics_libjpg_2_ce62d8f1.o \
  $(HIGGS_OBJDIR)/include_higgs_graphics_libjpg_3_ce62dcb2.o \
  $(HIGGS_OBJDIR)/include_higgs_graphics_libpng_6e070086.o \
  $(HIGGS_OBJDIR)/include_higgs_graphics_libwebp_dec_356f296c.o \
  $(HIGGS_OBJDIR)/include_higgs_graphics_libwebp_dsp_dec_df80faee.o \
  $(HIGGS_OBJDIR)/include_higgs_graphics_libwebp_dsp_dec_avx2_258a73aa.o \
  $(HIGGS_OBJDIR)/include_higgs_graphics_libwebp_dsp_dec_sse41_3dcad1b1.o \
  $(HIGGS_OBJDIR)/include_higgs_graphics_libwebp_dsp_enc_df9329c6.o \
  $(HIGGS_OBJDIR)/include_higgs_graphics_libwebp_dsp_enc_avx2_503bcbd2.o \
  $(HIGGS_OBJDIR)/include_higgs_graphics_libwebp_dsp_enc_sse41_69447e89.o \
  $(HIGGS_OBJDIR)/include_higgs_graphics_libwebp_enc_35815844.o \
  $(HIGGS_OBJDIR)/include_higgs_graphics_libwebp_sharpyuv_ee7d37d0.o \
  $(HIGGS_OBJDIR)/include_higgs_graphics_libwebp_utils_339e1d1b.o \
  $(HIGGS_OBJDIR)/include_higgs_graphics_lunasvg_92e75770.o \
  $(HIGGS_OBJDIR)/include_higgs_graphics_Sheenbidi_c310974d.o \
  $(HIGGS_OBJDIR)/include_higgs_gui_basics_e3f79785.o \
  $(HIGGS_OBJDIR)/include_higgs_gui_basics_2_153c9db8.o \
  $(HIGGS_OBJDIR)/include_higgs_gui_basics_3_154ab539.o \
  $(HIGGS_OBJDIR)/include_higgs_gui_basics_4_1558ccba.o \
  $(HIGGS_OBJDIR)/include_higgs_gui_basics_5_1566e43b.o \
  $(HIGGS_OBJDIR)/include_higgs_gui_extra_6dee1c1a.o \
  $(HIGGS_OBJDIR)/include_higgs_javascript_84e88969.o \
  $(HIGGS_OBJDIR)/include_higgs_midi_ci_1fda4092.o \
  $(HIGGS_OBJDIR)/include_higgs_opengl_a8a032b.o \
  $(HIGGS_OBJDIR)/include_higgs_osc_f3df604d.o \
  $(HIGGS_OBJDIR)/include_higgs_product_unlocking_8278fcdc.o \

.PHONY: clean all strip

all : $(HIGGS_OUTDIR)/$(HIGGS_TARGET_CONSOLEAPP)

$(HIGGS_OUTDIR)/$(HIGGS_TARGET_CONSOLEAPP) : $(OBJECTS_CONSOLEAPP) $(HIGGS_OBJDIR)/execinfo.cmd $(RESOURCES)
	@command -v $(PKG_CONFIG) >/dev/null 2>&1 || { echo >&2 "pkg-config not installed. Please, install it."; exit 1; }
	@$(PKG_CONFIG) --print-errors alsa freetype2 fontconfig egl gl libcurl
	@echo Linking "UnitTestRunner - ConsoleApp"
	-$(V_AT)mkdir -p $(HIGGS_BINDIR)
	-$(V_AT)mkdir -p $(HIGGS_LIBDIR)
	-$(V_AT)mkdir -p $(HIGGS_OUTDIR)
	$(V_AT)$(CXX) -o $(HIGGS_OUTDIR)/$(HIGGS_TARGET_CONSOLEAPP) $(OBJECTS_CONSOLEAPP) $(HIGGS_LDFLAGS) $(shell cat $(HIGGS_OBJDIR)/execinfo.cmd) $(RESOURCES) $(TARGET_ARCH)

$(HIGGS_OBJDIR)/Main_90ebc5c2.o: ../../Source/Main.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling Main.cpp"
	$(V_AT)$(CXX) $(HIGGS_CXXFLAGS) $(HIGGS_CPPFLAGS_CONSOLEAPP) $(HIGGS_CFLAGS_CONSOLEAPP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/include_higgs_analytics_f8e9fa94.o: ../../HiggsLibraryCode/include_higgs_analytics.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_higgs_analytics.cpp"
	$(V_AT)$(CXX) $(HIGGS_CXXFLAGS) $(HIGGS_CPPFLAGS_CONSOLEAPP) $(HIGGS_CFLAGS_CONSOLEAPP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/include_higgs_audio_basics_8a4e984a.o: ../../HiggsLibraryCode/include_higgs_audio_basics.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_higgs_audio_basics.cpp"
	$(V_AT)$(CXX) $(HIGGS_CXXFLAGS) $(HIGGS_CPPFLAGS_CONSOLEAPP) $(HIGGS_CFLAGS_CONSOLEAPP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/include_higgs_audio_devices_63111d02.o: ../../HiggsLibraryCode/include_higgs_audio_devices.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_higgs_audio_devices.cpp"
	$(V_AT)$(CXX) $(HIGGS_CXXFLAGS) $(HIGGS_CPPFLAGS_CONSOLEAPP) $(HIGGS_CFLAGS_CONSOLEAPP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/include_higgs_audio_formats_15f82001.o: ../../HiggsLibraryCode/include_higgs_audio_formats.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_higgs_audio_formats.cpp"
	$(V_AT)$(CXX) $(HIGGS_CXXFLAGS) $(HIGGS_CPPFLAGS_CONSOLEAPP) $(HIGGS_CFLAGS_CONSOLEAPP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/include_higgs_audio_formats_flac_1_32fae422.o: ../../HiggsLibraryCode/include_higgs_audio_formats_flac_1.c
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_higgs_audio_formats_flac_1.c"
	$(V_AT)$(CC) $(HIGGS_CFLAGS) $(HIGGS_CPPFLAGS_CONSOLEAPP) $(HIGGS_CFLAGS_CONSOLEAPP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/include_higgs_audio_formats_flac_2_32fae7e3.o: ../../HiggsLibraryCode/include_higgs_audio_formats_flac_2.c
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_higgs_audio_formats_flac_2.c"
	$(V_AT)$(CC) $(HIGGS_CFLAGS) $(HIGGS_CPPFLAGS_CONSOLEAPP) $(HIGGS_CFLAGS_CONSOLEAPP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/include_higgs_audio_formats_ogg_ccb28111.o: ../../HiggsLibraryCode/include_higgs_audio_formats_ogg.c
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_higgs_audio_formats_ogg.c"
	$(V_AT)$(CC) $(HIGGS_CFLAGS) $(HIGGS_CPPFLAGS_CONSOLEAPP) $(HIGGS_CFLAGS_CONSOLEAPP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/include_higgs_audio_formats_opus_ca23cec7.o: ../../HiggsLibraryCode/include_higgs_audio_formats_opus.c
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_higgs_audio_formats_opus.c"
	$(V_AT)$(CC) $(HIGGS_CFLAGS) $(HIGGS_CPPFLAGS_CONSOLEAPP) $(HIGGS_CFLAGS_CONSOLEAPP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/include_higgs_audio_formats_opusfile_c2979c43.o: ../../HiggsLibraryCode/include_higgs_audio_formats_opusfile.c
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_higgs_audio_formats_opusfile.c"
	$(V_AT)$(CC) $(HIGGS_CFLAGS) $(HIGGS_CPPFLAGS_CONSOLEAPP) $(HIGGS_CFLAGS_CONSOLEAPP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/include_higgs_audio_processors_10c03666.o: ../../HiggsLibraryCode/include_higgs_audio_processors.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_higgs_audio_processors.cpp"
	$(V_AT)$(CXX) $(HIGGS_CXXFLAGS) $(HIGGS_CPPFLAGS_CONSOLEAPP) $(HIGGS_CFLAGS_CONSOLEAPP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/include_higgs_audio_processors_headless_1902bafc.o: ../../HiggsLibraryCode/include_higgs_audio_processors_headless.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_higgs_audio_processors_headless.cpp"
	$(V_AT)$(CXX) $(HIGGS_CXXFLAGS) $(HIGGS_CPPFLAGS_CONSOLEAPP) $(HIGGS_CFLAGS_CONSOLEAPP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/include_higgs_audio_processors_headless_ara_8deeb88d.o: ../../HiggsLibraryCode/include_higgs_audio_processors_headless_ara.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_higgs_audio_processors_headless_ara.cpp"
	$(V_AT)$(CXX) $(HIGGS_CXXFLAGS) $(HIGGS_CPPFLAGS_CONSOLEAPP) $(HIGGS_CFLAGS_CONSOLEAPP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/include_higgs_audio_processors_headless_lv2_libs_36180e32.o: ../../HiggsLibraryCode/include_higgs_audio_processors_headless_lv2_libs.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_higgs_audio_processors_headless_lv2_libs.cpp"
	$(V_AT)$(CXX) $(HIGGS_CXXFLAGS) $(HIGGS_CPPFLAGS_CONSOLEAPP) $(HIGGS_CFLAGS_CONSOLEAPP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/include_higgs_audio_utils_9f9fb2d6.o: ../../HiggsLibraryCode/include_higgs_audio_utils.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_higgs_audio_utils.cpp"
	$(V_AT)$(CXX) $(HIGGS_CXXFLAGS) $(HIGGS_CPPFLAGS_CONSOLEAPP) $(HIGGS_CFLAGS_CONSOLEAPP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/include_higgs_core_f26d17db.o: ../../HiggsLibraryCode/include_higgs_core.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_higgs_core.cpp"
	$(V_AT)$(CXX) $(HIGGS_CXXFLAGS) $(HIGGS_CPPFLAGS_CONSOLEAPP) $(HIGGS_CFLAGS_CONSOLEAPP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/include_higgs_core_CompilationTime_9257742c.o: ../../HiggsLibraryCode/include_higgs_core_CompilationTime.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_higgs_core_CompilationTime.cpp"
	$(V_AT)$(CXX) $(HIGGS_CXXFLAGS) $(HIGGS_CPPFLAGS_CONSOLEAPP) $(HIGGS_CFLAGS_CONSOLEAPP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/include_higgs_core_zlib_ac61cd39.o: ../../HiggsLibraryCode/include_higgs_core_zlib.c
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_higgs_core_zlib.c"
	$(V_AT)$(CC) $(HIGGS_CFLAGS) $(HIGGS_CPPFLAGS_CONSOLEAPP) $(HIGGS_CFLAGS_CONSOLEAPP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/include_higgs_cryptography_8cb807a8.o: ../../HiggsLibraryCode/include_higgs_cryptography.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_higgs_cryptography.cpp"
	$(V_AT)$(CXX) $(HIGGS_CXXFLAGS) $(HIGGS_CPPFLAGS_CONSOLEAPP) $(HIGGS_CFLAGS_CONSOLEAPP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/include_higgs_data_structures_7471b1e3.o: ../../HiggsLibraryCode/include_higgs_data_structures.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_higgs_data_structures.cpp"
	$(V_AT)$(CXX) $(HIGGS_CXXFLAGS) $(HIGGS_CPPFLAGS_CONSOLEAPP) $(HIGGS_CFLAGS_CONSOLEAPP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/include_higgs_dsp_aeb2060f.o: ../../HiggsLibraryCode/include_higgs_dsp.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_higgs_dsp.cpp"
	$(V_AT)$(CXX) $(HIGGS_CXXFLAGS) $(HIGGS_CPPFLAGS_CONSOLEAPP) $(HIGGS_CFLAGS_CONSOLEAPP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/include_higgs_events_fd7d695.o: ../../HiggsLibraryCode/include_higgs_events.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_higgs_events.cpp"
	$(V_AT)$(CXX) $(HIGGS_CXXFLAGS) $(HIGGS_CPPFLAGS_CONSOLEAPP) $(HIGGS_CFLAGS_CONSOLEAPP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/include_higgs_graphics_f817e147.o: ../../HiggsLibraryCode/include_higgs_graphics.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_higgs_graphics.cpp"
	$(V_AT)$(CXX) $(HIGGS_CXXFLAGS) $(HIGGS_CPPFLAGS_CONSOLEAPP) $(HIGGS_CFLAGS_CONSOLEAPP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/include_higgs_graphics_Harfbuzz_60c52ba2.o: ../../HiggsLibraryCode/include_higgs_graphics_Harfbuzz.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_higgs_graphics_Harfbuzz.cpp"
	$(V_AT)$(CXX) $(HIGGS_CXXFLAGS) $(HIGGS_CPPFLAGS_CONSOLEAPP) $(HIGGS_CFLAGS_CONSOLEAPP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/include_higgs_graphics_libjpg_1_ce62d530.o: ../../HiggsLibraryCode/include_higgs_graphics_libjpg_1.c
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_higgs_graphics_libjpg_1.c"
	$(V_AT)$(CC) $(HIGGS_CFLAGS) $(HIGGS_CPPFLAGS_CONSOLEAPP) $(HIGGS_CFLAGS_CONSOLEAPP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/include_higgs_graphics_libjpg_2_ce62d8f1.o: ../../HiggsLibraryCode/include_higgs_graphics_libjpg_2.c
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_higgs_graphics_libjpg_2.c"
	$(V_AT)$(CC) $(HIGGS_CFLAGS) $(HIGGS_CPPFLAGS_CONSOLEAPP) $(HIGGS_CFLAGS_CONSOLEAPP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/include_higgs_graphics_libjpg_3_ce62dcb2.o: ../../HiggsLibraryCode/include_higgs_graphics_libjpg_3.c
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_higgs_graphics_libjpg_3.c"
	$(V_AT)$(CC) $(HIGGS_CFLAGS) $(HIGGS_CPPFLAGS_CONSOLEAPP) $(HIGGS_CFLAGS_CONSOLEAPP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/include_higgs_graphics_libpng_6e070086.o: ../../HiggsLibraryCode/include_higgs_graphics_libpng.c
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_higgs_graphics_libpng.c"
	$(V_AT)$(CC) $(HIGGS_CFLAGS) $(HIGGS_CPPFLAGS_CONSOLEAPP) $(HIGGS_CFLAGS_CONSOLEAPP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/include_higgs_graphics_libwebp_dec_356f296c.o: ../../HiggsLibraryCode/include_higgs_graphics_libwebp_dec.c
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_higgs_graphics_libwebp_dec.c"
	$(V_AT)$(CC) $(HIGGS_CFLAGS) $(HIGGS_CPPFLAGS_CONSOLEAPP) $(HIGGS_CFLAGS_CONSOLEAPP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/include_higgs_graphics_libwebp_dsp_dec_df80faee.o: ../../HiggsLibraryCode/include_higgs_graphics_libwebp_dsp_dec.c
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_higgs_graphics_libwebp_dsp_dec.c"
	$(V_AT)$(CC) $(HIGGS_CFLAGS) $(HIGGS_CPPFLAGS_CONSOLEAPP) $(HIGGS_CFLAGS_CONSOLEAPP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/include_higgs_graphics_libwebp_dsp_dec_avx2_258a73aa.o: ../../HiggsLibraryCode/include_higgs_graphics_libwebp_dsp_dec_avx2.c
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_higgs_graphics_libwebp_dsp_dec_avx2.c"
	$(V_AT)$(CC) $(HIGGS_CFLAGS) $(HIGGS_CPPFLAGS_CONSOLEAPP) $(HIGGS_CFLAGS_CONSOLEAPP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/include_higgs_graphics_libwebp_dsp_dec_sse41_3dcad1b1.o: ../../HiggsLibraryCode/include_higgs_graphics_libwebp_dsp_dec_sse41.c
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_higgs_graphics_libwebp_dsp_dec_sse41.c"
	$(V_AT)$(CC) $(HIGGS_CFLAGS) $(HIGGS_CPPFLAGS_CONSOLEAPP) $(HIGGS_CFLAGS_CONSOLEAPP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/include_higgs_graphics_libwebp_dsp_enc_df9329c6.o: ../../HiggsLibraryCode/include_higgs_graphics_libwebp_dsp_enc.c
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_higgs_graphics_libwebp_dsp_enc.c"
	$(V_AT)$(CC) $(HIGGS_CFLAGS) $(HIGGS_CPPFLAGS_CONSOLEAPP) $(HIGGS_CFLAGS_CONSOLEAPP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/include_higgs_graphics_libwebp_dsp_enc_avx2_503bcbd2.o: ../../HiggsLibraryCode/include_higgs_graphics_libwebp_dsp_enc_avx2.c
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_higgs_graphics_libwebp_dsp_enc_avx2.c"
	$(V_AT)$(CC) $(HIGGS_CFLAGS) $(HIGGS_CPPFLAGS_CONSOLEAPP) $(HIGGS_CFLAGS_CONSOLEAPP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/include_higgs_graphics_libwebp_dsp_enc_sse41_69447e89.o: ../../HiggsLibraryCode/include_higgs_graphics_libwebp_dsp_enc_sse41.c
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_higgs_graphics_libwebp_dsp_enc_sse41.c"
	$(V_AT)$(CC) $(HIGGS_CFLAGS) $(HIGGS_CPPFLAGS_CONSOLEAPP) $(HIGGS_CFLAGS_CONSOLEAPP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/include_higgs_graphics_libwebp_enc_35815844.o: ../../HiggsLibraryCode/include_higgs_graphics_libwebp_enc.c
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_higgs_graphics_libwebp_enc.c"
	$(V_AT)$(CC) $(HIGGS_CFLAGS) $(HIGGS_CPPFLAGS_CONSOLEAPP) $(HIGGS_CFLAGS_CONSOLEAPP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/include_higgs_graphics_libwebp_sharpyuv_ee7d37d0.o: ../../HiggsLibraryCode/include_higgs_graphics_libwebp_sharpyuv.c
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_higgs_graphics_libwebp_sharpyuv.c"
	$(V_AT)$(CC) $(HIGGS_CFLAGS) $(HIGGS_CPPFLAGS_CONSOLEAPP) $(HIGGS_CFLAGS_CONSOLEAPP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/include_higgs_graphics_libwebp_utils_339e1d1b.o: ../../HiggsLibraryCode/include_higgs_graphics_libwebp_utils.c
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_higgs_graphics_libwebp_utils.c"
	$(V_AT)$(CC) $(HIGGS_CFLAGS) $(HIGGS_CPPFLAGS_CONSOLEAPP) $(HIGGS_CFLAGS_CONSOLEAPP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/include_higgs_graphics_lunasvg_92e75770.o: ../../HiggsLibraryCode/include_higgs_graphics_lunasvg.c
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_higgs_graphics_lunasvg.c"
	$(V_AT)$(CC) $(HIGGS_CFLAGS) $(HIGGS_CPPFLAGS_CONSOLEAPP) $(HIGGS_CFLAGS_CONSOLEAPP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/include_higgs_graphics_Sheenbidi_c310974d.o: ../../HiggsLibraryCode/include_higgs_graphics_Sheenbidi.c
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_higgs_graphics_Sheenbidi.c"
	$(V_AT)$(CC) $(HIGGS_CFLAGS) $(HIGGS_CPPFLAGS_CONSOLEAPP) $(HIGGS_CFLAGS_CONSOLEAPP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/include_higgs_gui_basics_e3f79785.o: ../../HiggsLibraryCode/include_higgs_gui_basics.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_higgs_gui_basics.cpp"
	$(V_AT)$(CXX) $(HIGGS_CXXFLAGS) $(HIGGS_CPPFLAGS_CONSOLEAPP) $(HIGGS_CFLAGS_CONSOLEAPP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/include_higgs_gui_basics_2_153c9db8.o: ../../HiggsLibraryCode/include_higgs_gui_basics_2.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_higgs_gui_basics_2.cpp"
	$(V_AT)$(CXX) $(HIGGS_CXXFLAGS) $(HIGGS_CPPFLAGS_CONSOLEAPP) $(HIGGS_CFLAGS_CONSOLEAPP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/include_higgs_gui_basics_3_154ab539.o: ../../HiggsLibraryCode/include_higgs_gui_basics_3.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_higgs_gui_basics_3.cpp"
	$(V_AT)$(CXX) $(HIGGS_CXXFLAGS) $(HIGGS_CPPFLAGS_CONSOLEAPP) $(HIGGS_CFLAGS_CONSOLEAPP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/include_higgs_gui_basics_4_1558ccba.o: ../../HiggsLibraryCode/include_higgs_gui_basics_4.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_higgs_gui_basics_4.cpp"
	$(V_AT)$(CXX) $(HIGGS_CXXFLAGS) $(HIGGS_CPPFLAGS_CONSOLEAPP) $(HIGGS_CFLAGS_CONSOLEAPP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/include_higgs_gui_basics_5_1566e43b.o: ../../HiggsLibraryCode/include_higgs_gui_basics_5.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_higgs_gui_basics_5.cpp"
	$(V_AT)$(CXX) $(HIGGS_CXXFLAGS) $(HIGGS_CPPFLAGS_CONSOLEAPP) $(HIGGS_CFLAGS_CONSOLEAPP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/include_higgs_gui_extra_6dee1c1a.o: ../../HiggsLibraryCode/include_higgs_gui_extra.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_higgs_gui_extra.cpp"
	$(V_AT)$(CXX) $(HIGGS_CXXFLAGS) $(HIGGS_CPPFLAGS_CONSOLEAPP) $(HIGGS_CFLAGS_CONSOLEAPP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/include_higgs_javascript_84e88969.o: ../../HiggsLibraryCode/include_higgs_javascript.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_higgs_javascript.cpp"
	$(V_AT)$(CXX) $(HIGGS_CXXFLAGS) $(HIGGS_CPPFLAGS_CONSOLEAPP) $(HIGGS_CFLAGS_CONSOLEAPP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/include_higgs_midi_ci_1fda4092.o: ../../HiggsLibraryCode/include_higgs_midi_ci.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_higgs_midi_ci.cpp"
	$(V_AT)$(CXX) $(HIGGS_CXXFLAGS) $(HIGGS_CPPFLAGS_CONSOLEAPP) $(HIGGS_CFLAGS_CONSOLEAPP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/include_higgs_opengl_a8a032b.o: ../../HiggsLibraryCode/include_higgs_opengl.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_higgs_opengl.cpp"
	$(V_AT)$(CXX) $(HIGGS_CXXFLAGS) $(HIGGS_CPPFLAGS_CONSOLEAPP) $(HIGGS_CFLAGS_CONSOLEAPP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/include_higgs_osc_f3df604d.o: ../../HiggsLibraryCode/include_higgs_osc.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_higgs_osc.cpp"
	$(V_AT)$(CXX) $(HIGGS_CXXFLAGS) $(HIGGS_CPPFLAGS_CONSOLEAPP) $(HIGGS_CFLAGS_CONSOLEAPP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/include_higgs_product_unlocking_8278fcdc.o: ../../HiggsLibraryCode/include_higgs_product_unlocking.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_higgs_product_unlocking.cpp"
	$(V_AT)$(CXX) $(HIGGS_CXXFLAGS) $(HIGGS_CPPFLAGS_CONSOLEAPP) $(HIGGS_CFLAGS_CONSOLEAPP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/execinfo.cmd:
	-$(V_AT)mkdir -p $(@D)
	-@if [ -z "$(V_AT)" ]; then echo "Checking if we need to link libexecinfo"; fi
	$(V_AT)printf "int main() { return 0; }" | $(CXX) -x c++ -o $(@D)/execinfo.x -lexecinfo - >/dev/null 2>&1 && printf -- "-lexecinfo" > "$@" || touch "$@"

$(HIGGS_OBJDIR)/cxxfs.cmd:
	-$(V_AT)mkdir -p $(@D)
	-@if [ -z "$(V_AT)" ]; then echo "Checking if we need to link stdc++fs"; fi
	$(V_AT)printf "int main() { return 0; }" | $(CXX) -x c++ -o $(@D)/cxxfs.x -lstdc++fs - >/dev/null 2>&1 && printf -- "-lstdc++fs" > "$@" || touch "$@"

clean:
	@echo Cleaning UnitTestRunner
	$(V_AT)$(CLEANCMD)

strip:
	@echo Stripping UnitTestRunner
	-$(V_AT)$(STRIP) --strip-unneeded $(HIGGS_OUTDIR)/$(HIGGS_TARGET_CONSOLEAPP)

-include $(OBJECTS_CONSOLEAPP:%.o=%.d)
