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

  HIGGS_CPPFLAGS := $(DEPFLAGS) "-DLINUX=1" "-DDEBUG=1" "-D_DEBUG=1" "-DHIGGS_MYTHOSSTUDIO_VERSION=0x90005" "-DHIGGS_MODULE_AVAILABLE_higgs_build_tools=1" "-DHIGGS_MODULE_AVAILABLE_higgs_core=1" "-DHIGGS_MODULE_AVAILABLE_higgs_cryptography=1" "-DHIGGS_MODULE_AVAILABLE_higgs_data_structures=1" "-DHIGGS_MODULE_AVAILABLE_higgs_events=1" "-DHIGGS_MODULE_AVAILABLE_higgs_graphics=1" "-DHIGGS_MODULE_AVAILABLE_higgs_gui_basics=1" "-DHIGGS_MODULE_AVAILABLE_higgs_gui_extra=1" "-DHIGGS_WEBVIEW_INTEROP_LIBRARY_VERSION=\"1.0.0\"" "-DHIGGS_GLOBAL_MODULE_SETTINGS_INCLUDED=1" "-DHIGGS_LOG_ASSERTIONS=1" "-DHIGGS_USE_CURL=1" "-DHIGGS_LOAD_CURL_SYMBOLS_LAZILY=1" "-DHIGGS_ALLOW_STATIC_NULL_VARIABLES=0" "-DHIGGS_STRICT_REFCOUNTEDPOINTER=1" "-DHIGGS_WEB_BROWSER=0" "-DHIGGS_STANDALONE_APPLICATION=1" "-DMYTHOS_LINUX_MAKE_6D53C8B4=1" "-DHIGGS_APP_VERSION=9.0.5" "-DHIGGS_APP_VERSION_HEX=0x90005" $(shell $(PKG_CONFIG) --cflags freetype2 fontconfig) -pthread -I../../HiggsLibraryCode -I../../../Build -I../../../../modules $(CPPFLAGS)
  HIGGS_CPPFLAGS_APP :=  "-DHiggsPlugin_Build_VST=0" "-DHiggsPlugin_Build_VST3=0" "-DHiggsPlugin_Build_AU=0" "-DHiggsPlugin_Build_AUv3=0" "-DHiggsPlugin_Build_AAX=0" "-DHiggsPlugin_Build_Standalone=0" "-DHiggsPlugin_Build_Unity=0" "-DHiggsPlugin_Build_LV2=0"
  HIGGS_TARGET_APP := MythosStudio

  HIGGS_CFLAGS += $(HIGGS_CPPFLAGS) $(TARGET_ARCH) -g -ggdb -O0 $(CFLAGS)
  HIGGS_CXXFLAGS += $(HIGGS_CFLAGS) -std=c++17 $(CXXFLAGS)
  HIGGS_LDFLAGS += $(TARGET_ARCH) -L$(HIGGS_BINDIR) -L$(HIGGS_LIBDIR) $(shell $(PKG_CONFIG) --libs freetype2 fontconfig) -fvisibility=hidden -lrt -ldl -lpthread $(LDFLAGS)

  CLEANCMD = rm -rf $(HIGGS_OUTDIR)/$(HIGGS_TARGET_APP) $(HIGGS_OBJDIR)
endif

ifeq ($(CONFIG),Release)
  HIGGS_BINDIR := build
  HIGGS_LIBDIR := build
  HIGGS_OBJDIR := build/intermediate/Release
  HIGGS_OUTDIR := build

  ifeq ($(TARGET_ARCH),)
    TARGET_ARCH := 
  endif

  HIGGS_CPPFLAGS := $(DEPFLAGS) "-DLINUX=1" "-DNDEBUG=1" "-DHIGGS_MYTHOSSTUDIO_VERSION=0x90005" "-DHIGGS_MODULE_AVAILABLE_higgs_build_tools=1" "-DHIGGS_MODULE_AVAILABLE_higgs_core=1" "-DHIGGS_MODULE_AVAILABLE_higgs_cryptography=1" "-DHIGGS_MODULE_AVAILABLE_higgs_data_structures=1" "-DHIGGS_MODULE_AVAILABLE_higgs_events=1" "-DHIGGS_MODULE_AVAILABLE_higgs_graphics=1" "-DHIGGS_MODULE_AVAILABLE_higgs_gui_basics=1" "-DHIGGS_MODULE_AVAILABLE_higgs_gui_extra=1" "-DHIGGS_WEBVIEW_INTEROP_LIBRARY_VERSION=\"1.0.0\"" "-DHIGGS_GLOBAL_MODULE_SETTINGS_INCLUDED=1" "-DHIGGS_LOG_ASSERTIONS=1" "-DHIGGS_USE_CURL=1" "-DHIGGS_LOAD_CURL_SYMBOLS_LAZILY=1" "-DHIGGS_ALLOW_STATIC_NULL_VARIABLES=0" "-DHIGGS_STRICT_REFCOUNTEDPOINTER=1" "-DHIGGS_WEB_BROWSER=0" "-DHIGGS_STANDALONE_APPLICATION=1" "-DMYTHOS_LINUX_MAKE_6D53C8B4=1" "-DHIGGS_APP_VERSION=9.0.5" "-DHIGGS_APP_VERSION_HEX=0x90005" $(shell $(PKG_CONFIG) --cflags freetype2 fontconfig) -pthread -I../../HiggsLibraryCode -I../../../Build -I../../../../modules $(CPPFLAGS)
  HIGGS_CPPFLAGS_APP :=  "-DHiggsPlugin_Build_VST=0" "-DHiggsPlugin_Build_VST3=0" "-DHiggsPlugin_Build_AU=0" "-DHiggsPlugin_Build_AUv3=0" "-DHiggsPlugin_Build_AAX=0" "-DHiggsPlugin_Build_Standalone=0" "-DHiggsPlugin_Build_Unity=0" "-DHiggsPlugin_Build_LV2=0"
  HIGGS_TARGET_APP := MythosStudio

  HIGGS_CFLAGS += $(HIGGS_CPPFLAGS) $(TARGET_ARCH) -O3 $(CFLAGS)
  HIGGS_CXXFLAGS += $(HIGGS_CFLAGS) -std=c++17 $(CXXFLAGS)
  HIGGS_LDFLAGS += $(TARGET_ARCH) -L$(HIGGS_BINDIR) -L$(HIGGS_LIBDIR) $(shell $(PKG_CONFIG) --libs freetype2 fontconfig) -fvisibility=hidden -lrt -ldl -lpthread $(LDFLAGS)

  CLEANCMD = rm -rf $(HIGGS_OUTDIR)/$(HIGGS_TARGET_APP) $(HIGGS_OBJDIR)
endif

OBJECTS_APP := \
  $(HIGGS_OBJDIR)/mythos_NewProjectWizard_7817a7e4.o \
  $(HIGGS_OBJDIR)/mythos_StartPageComponent_de6b22ec.o \
  $(HIGGS_OBJDIR)/mythos_AutoUpdater_ca658dc2.o \
  $(HIGGS_OBJDIR)/mythos_CommandLine_f35de107.o \
  $(HIGGS_OBJDIR)/mythos_Main_f8488f5b.o \
  $(HIGGS_OBJDIR)/mythos_MainWindow_1e163aeb.o \
  $(HIGGS_OBJDIR)/mythos_DocumentEditorComponent_bc853a2f.o \
  $(HIGGS_OBJDIR)/mythos_OpenDocumentManager_ba866622.o \
  $(HIGGS_OBJDIR)/mythos_SourceCodeEditor_55965985.o \
  $(HIGGS_OBJDIR)/mythos_Modules_e20cbd10.o \
  $(HIGGS_OBJDIR)/mythos_HeaderComponent_1ebf72ba.o \
  $(HIGGS_OBJDIR)/mythos_Project_c131864a.o \
  $(HIGGS_OBJDIR)/mythos_ProjectExporter_cf377b25.o \
  $(HIGGS_OBJDIR)/mythos_ProjectSaver_4276639b.o \
  $(HIGGS_OBJDIR)/mythos_ResourceFile_74b61849.o \
  $(HIGGS_OBJDIR)/mythos_AppearanceSettings_fa5d736e.o \
  $(HIGGS_OBJDIR)/mythos_StoredSettings_c7168d2d.o \
  $(HIGGS_OBJDIR)/mythos_CodeHelpers_1e797672.o \
  $(HIGGS_OBJDIR)/mythos_FileHelpers_54f12f83.o \
  $(HIGGS_OBJDIR)/mythos_MiscUtilities_31fc8dd8.o \
  $(HIGGS_OBJDIR)/mythos_NewFileWizard_b233dda1.o \
  $(HIGGS_OBJDIR)/mythos_VersionInfo_46f3ed40.o \
  $(HIGGS_OBJDIR)/mythos_PIPGenerator_fd3402c7.o \
  $(HIGGS_OBJDIR)/mythos_Icons_d02d18f1.o \
  $(HIGGS_OBJDIR)/mythos_MythosTreeViewBase_9b9f2ff0.o \
  $(HIGGS_OBJDIR)/mythos_MythosStudioLookAndFeel_3b20291d.o \
  $(HIGGS_OBJDIR)/mythos_SlidingPanelComponent_4e7dc07e.o \
  $(HIGGS_OBJDIR)/BinaryData_ce4232d4.o \
  $(HIGGS_OBJDIR)/include_higgs_build_tools_f5069398.o \
  $(HIGGS_OBJDIR)/include_higgs_core_f26d17db.o \
  $(HIGGS_OBJDIR)/include_higgs_core_CompilationTime_9257742c.o \
  $(HIGGS_OBJDIR)/include_higgs_core_zlib_ac61cd39.o \
  $(HIGGS_OBJDIR)/include_higgs_cryptography_8cb807a8.o \
  $(HIGGS_OBJDIR)/include_higgs_data_structures_7471b1e3.o \
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

.PHONY: clean all strip

all : $(HIGGS_OUTDIR)/$(HIGGS_TARGET_APP)

$(HIGGS_OUTDIR)/$(HIGGS_TARGET_APP) : $(OBJECTS_APP) $(HIGGS_OBJDIR)/execinfo.cmd $(RESOURCES)
	@command -v $(PKG_CONFIG) >/dev/null 2>&1 || { echo >&2 "pkg-config not installed. Please, install it."; exit 1; }
	@$(PKG_CONFIG) --print-errors freetype2 fontconfig
	@echo Linking "MythosStudio - App"
	-$(V_AT)mkdir -p $(HIGGS_BINDIR)
	-$(V_AT)mkdir -p $(HIGGS_LIBDIR)
	-$(V_AT)mkdir -p $(HIGGS_OUTDIR)
	$(V_AT)$(CXX) -o $(HIGGS_OUTDIR)/$(HIGGS_TARGET_APP) $(OBJECTS_APP) $(HIGGS_LDFLAGS) $(shell cat $(HIGGS_OBJDIR)/execinfo.cmd) $(HIGGS_LDFLAGS_APP) $(RESOURCES) $(TARGET_ARCH)

$(HIGGS_OBJDIR)/mythos_NewProjectWizard_7817a7e4.o: ../../Source/Application/StartPage/mythos_NewProjectWizard.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling mythos_NewProjectWizard.cpp"
	$(V_AT)$(CXX) $(HIGGS_CXXFLAGS) $(HIGGS_CPPFLAGS_APP) $(HIGGS_CFLAGS_APP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/mythos_StartPageComponent_de6b22ec.o: ../../Source/Application/StartPage/mythos_StartPageComponent.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling mythos_StartPageComponent.cpp"
	$(V_AT)$(CXX) $(HIGGS_CXXFLAGS) $(HIGGS_CPPFLAGS_APP) $(HIGGS_CFLAGS_APP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/mythos_AutoUpdater_ca658dc2.o: ../../Source/Application/mythos_AutoUpdater.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling mythos_AutoUpdater.cpp"
	$(V_AT)$(CXX) $(HIGGS_CXXFLAGS) $(HIGGS_CPPFLAGS_APP) $(HIGGS_CFLAGS_APP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/mythos_CommandLine_f35de107.o: ../../Source/Application/mythos_CommandLine.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling mythos_CommandLine.cpp"
	$(V_AT)$(CXX) $(HIGGS_CXXFLAGS) $(HIGGS_CPPFLAGS_APP) $(HIGGS_CFLAGS_APP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/mythos_Main_f8488f5b.o: ../../Source/Application/mythos_Main.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling mythos_Main.cpp"
	$(V_AT)$(CXX) $(HIGGS_CXXFLAGS) $(HIGGS_CPPFLAGS_APP) $(HIGGS_CFLAGS_APP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/mythos_MainWindow_1e163aeb.o: ../../Source/Application/mythos_MainWindow.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling mythos_MainWindow.cpp"
	$(V_AT)$(CXX) $(HIGGS_CXXFLAGS) $(HIGGS_CPPFLAGS_APP) $(HIGGS_CFLAGS_APP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/mythos_DocumentEditorComponent_bc853a2f.o: ../../Source/CodeEditor/mythos_DocumentEditorComponent.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling mythos_DocumentEditorComponent.cpp"
	$(V_AT)$(CXX) $(HIGGS_CXXFLAGS) $(HIGGS_CPPFLAGS_APP) $(HIGGS_CFLAGS_APP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/mythos_OpenDocumentManager_ba866622.o: ../../Source/CodeEditor/mythos_OpenDocumentManager.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling mythos_OpenDocumentManager.cpp"
	$(V_AT)$(CXX) $(HIGGS_CXXFLAGS) $(HIGGS_CPPFLAGS_APP) $(HIGGS_CFLAGS_APP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/mythos_SourceCodeEditor_55965985.o: ../../Source/CodeEditor/mythos_SourceCodeEditor.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling mythos_SourceCodeEditor.cpp"
	$(V_AT)$(CXX) $(HIGGS_CXXFLAGS) $(HIGGS_CPPFLAGS_APP) $(HIGGS_CFLAGS_APP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/mythos_Modules_e20cbd10.o: ../../Source/Project/Modules/mythos_Modules.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling mythos_Modules.cpp"
	$(V_AT)$(CXX) $(HIGGS_CXXFLAGS) $(HIGGS_CPPFLAGS_APP) $(HIGGS_CFLAGS_APP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/mythos_HeaderComponent_1ebf72ba.o: ../../Source/Project/UI/mythos_HeaderComponent.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling mythos_HeaderComponent.cpp"
	$(V_AT)$(CXX) $(HIGGS_CXXFLAGS) $(HIGGS_CPPFLAGS_APP) $(HIGGS_CFLAGS_APP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/mythos_Project_c131864a.o: ../../Source/Project/mythos_Project.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling mythos_Project.cpp"
	$(V_AT)$(CXX) $(HIGGS_CXXFLAGS) $(HIGGS_CPPFLAGS_APP) $(HIGGS_CFLAGS_APP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/mythos_ProjectExporter_cf377b25.o: ../../Source/ProjectSaving/mythos_ProjectExporter.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling mythos_ProjectExporter.cpp"
	$(V_AT)$(CXX) $(HIGGS_CXXFLAGS) $(HIGGS_CPPFLAGS_APP) $(HIGGS_CFLAGS_APP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/mythos_ProjectSaver_4276639b.o: ../../Source/ProjectSaving/mythos_ProjectSaver.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling mythos_ProjectSaver.cpp"
	$(V_AT)$(CXX) $(HIGGS_CXXFLAGS) $(HIGGS_CPPFLAGS_APP) $(HIGGS_CFLAGS_APP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/mythos_ResourceFile_74b61849.o: ../../Source/ProjectSaving/mythos_ResourceFile.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling mythos_ResourceFile.cpp"
	$(V_AT)$(CXX) $(HIGGS_CXXFLAGS) $(HIGGS_CPPFLAGS_APP) $(HIGGS_CFLAGS_APP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/mythos_AppearanceSettings_fa5d736e.o: ../../Source/Settings/mythos_AppearanceSettings.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling mythos_AppearanceSettings.cpp"
	$(V_AT)$(CXX) $(HIGGS_CXXFLAGS) $(HIGGS_CPPFLAGS_APP) $(HIGGS_CFLAGS_APP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/mythos_StoredSettings_c7168d2d.o: ../../Source/Settings/mythos_StoredSettings.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling mythos_StoredSettings.cpp"
	$(V_AT)$(CXX) $(HIGGS_CXXFLAGS) $(HIGGS_CPPFLAGS_APP) $(HIGGS_CFLAGS_APP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/mythos_CodeHelpers_1e797672.o: ../../Source/Utility/Helpers/mythos_CodeHelpers.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling mythos_CodeHelpers.cpp"
	$(V_AT)$(CXX) $(HIGGS_CXXFLAGS) $(HIGGS_CPPFLAGS_APP) $(HIGGS_CFLAGS_APP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/mythos_FileHelpers_54f12f83.o: ../../Source/Utility/Helpers/mythos_FileHelpers.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling mythos_FileHelpers.cpp"
	$(V_AT)$(CXX) $(HIGGS_CXXFLAGS) $(HIGGS_CPPFLAGS_APP) $(HIGGS_CFLAGS_APP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/mythos_MiscUtilities_31fc8dd8.o: ../../Source/Utility/Helpers/mythos_MiscUtilities.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling mythos_MiscUtilities.cpp"
	$(V_AT)$(CXX) $(HIGGS_CXXFLAGS) $(HIGGS_CPPFLAGS_APP) $(HIGGS_CFLAGS_APP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/mythos_NewFileWizard_b233dda1.o: ../../Source/Utility/Helpers/mythos_NewFileWizard.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling mythos_NewFileWizard.cpp"
	$(V_AT)$(CXX) $(HIGGS_CXXFLAGS) $(HIGGS_CPPFLAGS_APP) $(HIGGS_CFLAGS_APP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/mythos_VersionInfo_46f3ed40.o: ../../Source/Utility/Helpers/mythos_VersionInfo.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling mythos_VersionInfo.cpp"
	$(V_AT)$(CXX) $(HIGGS_CXXFLAGS) $(HIGGS_CPPFLAGS_APP) $(HIGGS_CFLAGS_APP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/mythos_PIPGenerator_fd3402c7.o: ../../Source/Utility/PIPs/mythos_PIPGenerator.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling mythos_PIPGenerator.cpp"
	$(V_AT)$(CXX) $(HIGGS_CXXFLAGS) $(HIGGS_CPPFLAGS_APP) $(HIGGS_CFLAGS_APP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/mythos_Icons_d02d18f1.o: ../../Source/Utility/UI/mythos_Icons.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling mythos_Icons.cpp"
	$(V_AT)$(CXX) $(HIGGS_CXXFLAGS) $(HIGGS_CPPFLAGS_APP) $(HIGGS_CFLAGS_APP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/mythos_MythosTreeViewBase_9b9f2ff0.o: ../../Source/Utility/UI/mythos_MythosTreeViewBase.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling mythos_MythosTreeViewBase.cpp"
	$(V_AT)$(CXX) $(HIGGS_CXXFLAGS) $(HIGGS_CPPFLAGS_APP) $(HIGGS_CFLAGS_APP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/mythos_MythosStudioLookAndFeel_3b20291d.o: ../../Source/Utility/UI/mythos_MythosStudioLookAndFeel.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling mythos_MythosStudioLookAndFeel.cpp"
	$(V_AT)$(CXX) $(HIGGS_CXXFLAGS) $(HIGGS_CPPFLAGS_APP) $(HIGGS_CFLAGS_APP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/mythos_SlidingPanelComponent_4e7dc07e.o: ../../Source/Utility/UI/mythos_SlidingPanelComponent.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling mythos_SlidingPanelComponent.cpp"
	$(V_AT)$(CXX) $(HIGGS_CXXFLAGS) $(HIGGS_CPPFLAGS_APP) $(HIGGS_CFLAGS_APP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/BinaryData_ce4232d4.o: ../../HiggsLibraryCode/BinaryData.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling BinaryData.cpp"
	$(V_AT)$(CXX) $(HIGGS_CXXFLAGS) $(HIGGS_CPPFLAGS_APP) $(HIGGS_CFLAGS_APP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/include_higgs_build_tools_f5069398.o: ../../HiggsLibraryCode/include_higgs_build_tools.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_higgs_build_tools.cpp"
	$(V_AT)$(CXX) $(HIGGS_CXXFLAGS) $(HIGGS_CPPFLAGS_APP) $(HIGGS_CFLAGS_APP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/include_higgs_core_f26d17db.o: ../../HiggsLibraryCode/include_higgs_core.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_higgs_core.cpp"
	$(V_AT)$(CXX) $(HIGGS_CXXFLAGS) $(HIGGS_CPPFLAGS_APP) $(HIGGS_CFLAGS_APP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/include_higgs_core_CompilationTime_9257742c.o: ../../HiggsLibraryCode/include_higgs_core_CompilationTime.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_higgs_core_CompilationTime.cpp"
	$(V_AT)$(CXX) $(HIGGS_CXXFLAGS) $(HIGGS_CPPFLAGS_APP) $(HIGGS_CFLAGS_APP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/include_higgs_core_zlib_ac61cd39.o: ../../HiggsLibraryCode/include_higgs_core_zlib.c
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_higgs_core_zlib.c"
	$(V_AT)$(CC) $(HIGGS_CFLAGS) $(HIGGS_CPPFLAGS_APP) $(HIGGS_CFLAGS_APP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/include_higgs_cryptography_8cb807a8.o: ../../HiggsLibraryCode/include_higgs_cryptography.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_higgs_cryptography.cpp"
	$(V_AT)$(CXX) $(HIGGS_CXXFLAGS) $(HIGGS_CPPFLAGS_APP) $(HIGGS_CFLAGS_APP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/include_higgs_data_structures_7471b1e3.o: ../../HiggsLibraryCode/include_higgs_data_structures.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_higgs_data_structures.cpp"
	$(V_AT)$(CXX) $(HIGGS_CXXFLAGS) $(HIGGS_CPPFLAGS_APP) $(HIGGS_CFLAGS_APP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/include_higgs_events_fd7d695.o: ../../HiggsLibraryCode/include_higgs_events.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_higgs_events.cpp"
	$(V_AT)$(CXX) $(HIGGS_CXXFLAGS) $(HIGGS_CPPFLAGS_APP) $(HIGGS_CFLAGS_APP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/include_higgs_graphics_f817e147.o: ../../HiggsLibraryCode/include_higgs_graphics.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_higgs_graphics.cpp"
	$(V_AT)$(CXX) $(HIGGS_CXXFLAGS) $(HIGGS_CPPFLAGS_APP) $(HIGGS_CFLAGS_APP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/include_higgs_graphics_Harfbuzz_60c52ba2.o: ../../HiggsLibraryCode/include_higgs_graphics_Harfbuzz.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_higgs_graphics_Harfbuzz.cpp"
	$(V_AT)$(CXX) $(HIGGS_CXXFLAGS) $(HIGGS_CPPFLAGS_APP) $(HIGGS_CFLAGS_APP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/include_higgs_graphics_libjpg_1_ce62d530.o: ../../HiggsLibraryCode/include_higgs_graphics_libjpg_1.c
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_higgs_graphics_libjpg_1.c"
	$(V_AT)$(CC) $(HIGGS_CFLAGS) $(HIGGS_CPPFLAGS_APP) $(HIGGS_CFLAGS_APP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/include_higgs_graphics_libjpg_2_ce62d8f1.o: ../../HiggsLibraryCode/include_higgs_graphics_libjpg_2.c
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_higgs_graphics_libjpg_2.c"
	$(V_AT)$(CC) $(HIGGS_CFLAGS) $(HIGGS_CPPFLAGS_APP) $(HIGGS_CFLAGS_APP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/include_higgs_graphics_libjpg_3_ce62dcb2.o: ../../HiggsLibraryCode/include_higgs_graphics_libjpg_3.c
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_higgs_graphics_libjpg_3.c"
	$(V_AT)$(CC) $(HIGGS_CFLAGS) $(HIGGS_CPPFLAGS_APP) $(HIGGS_CFLAGS_APP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/include_higgs_graphics_libpng_6e070086.o: ../../HiggsLibraryCode/include_higgs_graphics_libpng.c
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_higgs_graphics_libpng.c"
	$(V_AT)$(CC) $(HIGGS_CFLAGS) $(HIGGS_CPPFLAGS_APP) $(HIGGS_CFLAGS_APP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/include_higgs_graphics_libwebp_dec_356f296c.o: ../../HiggsLibraryCode/include_higgs_graphics_libwebp_dec.c
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_higgs_graphics_libwebp_dec.c"
	$(V_AT)$(CC) $(HIGGS_CFLAGS) $(HIGGS_CPPFLAGS_APP) $(HIGGS_CFLAGS_APP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/include_higgs_graphics_libwebp_dsp_dec_df80faee.o: ../../HiggsLibraryCode/include_higgs_graphics_libwebp_dsp_dec.c
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_higgs_graphics_libwebp_dsp_dec.c"
	$(V_AT)$(CC) $(HIGGS_CFLAGS) $(HIGGS_CPPFLAGS_APP) $(HIGGS_CFLAGS_APP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/include_higgs_graphics_libwebp_dsp_dec_avx2_258a73aa.o: ../../HiggsLibraryCode/include_higgs_graphics_libwebp_dsp_dec_avx2.c
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_higgs_graphics_libwebp_dsp_dec_avx2.c"
	$(V_AT)$(CC) $(HIGGS_CFLAGS) $(HIGGS_CPPFLAGS_APP) $(HIGGS_CFLAGS_APP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/include_higgs_graphics_libwebp_dsp_dec_sse41_3dcad1b1.o: ../../HiggsLibraryCode/include_higgs_graphics_libwebp_dsp_dec_sse41.c
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_higgs_graphics_libwebp_dsp_dec_sse41.c"
	$(V_AT)$(CC) $(HIGGS_CFLAGS) $(HIGGS_CPPFLAGS_APP) $(HIGGS_CFLAGS_APP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/include_higgs_graphics_libwebp_dsp_enc_df9329c6.o: ../../HiggsLibraryCode/include_higgs_graphics_libwebp_dsp_enc.c
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_higgs_graphics_libwebp_dsp_enc.c"
	$(V_AT)$(CC) $(HIGGS_CFLAGS) $(HIGGS_CPPFLAGS_APP) $(HIGGS_CFLAGS_APP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/include_higgs_graphics_libwebp_dsp_enc_avx2_503bcbd2.o: ../../HiggsLibraryCode/include_higgs_graphics_libwebp_dsp_enc_avx2.c
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_higgs_graphics_libwebp_dsp_enc_avx2.c"
	$(V_AT)$(CC) $(HIGGS_CFLAGS) $(HIGGS_CPPFLAGS_APP) $(HIGGS_CFLAGS_APP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/include_higgs_graphics_libwebp_dsp_enc_sse41_69447e89.o: ../../HiggsLibraryCode/include_higgs_graphics_libwebp_dsp_enc_sse41.c
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_higgs_graphics_libwebp_dsp_enc_sse41.c"
	$(V_AT)$(CC) $(HIGGS_CFLAGS) $(HIGGS_CPPFLAGS_APP) $(HIGGS_CFLAGS_APP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/include_higgs_graphics_libwebp_enc_35815844.o: ../../HiggsLibraryCode/include_higgs_graphics_libwebp_enc.c
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_higgs_graphics_libwebp_enc.c"
	$(V_AT)$(CC) $(HIGGS_CFLAGS) $(HIGGS_CPPFLAGS_APP) $(HIGGS_CFLAGS_APP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/include_higgs_graphics_libwebp_sharpyuv_ee7d37d0.o: ../../HiggsLibraryCode/include_higgs_graphics_libwebp_sharpyuv.c
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_higgs_graphics_libwebp_sharpyuv.c"
	$(V_AT)$(CC) $(HIGGS_CFLAGS) $(HIGGS_CPPFLAGS_APP) $(HIGGS_CFLAGS_APP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/include_higgs_graphics_libwebp_utils_339e1d1b.o: ../../HiggsLibraryCode/include_higgs_graphics_libwebp_utils.c
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_higgs_graphics_libwebp_utils.c"
	$(V_AT)$(CC) $(HIGGS_CFLAGS) $(HIGGS_CPPFLAGS_APP) $(HIGGS_CFLAGS_APP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/include_higgs_graphics_lunasvg_92e75770.o: ../../HiggsLibraryCode/include_higgs_graphics_lunasvg.c
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_higgs_graphics_lunasvg.c"
	$(V_AT)$(CC) $(HIGGS_CFLAGS) $(HIGGS_CPPFLAGS_APP) $(HIGGS_CFLAGS_APP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/include_higgs_graphics_Sheenbidi_c310974d.o: ../../HiggsLibraryCode/include_higgs_graphics_Sheenbidi.c
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_higgs_graphics_Sheenbidi.c"
	$(V_AT)$(CC) $(HIGGS_CFLAGS) $(HIGGS_CPPFLAGS_APP) $(HIGGS_CFLAGS_APP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/include_higgs_gui_basics_e3f79785.o: ../../HiggsLibraryCode/include_higgs_gui_basics.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_higgs_gui_basics.cpp"
	$(V_AT)$(CXX) $(HIGGS_CXXFLAGS) $(HIGGS_CPPFLAGS_APP) $(HIGGS_CFLAGS_APP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/include_higgs_gui_basics_2_153c9db8.o: ../../HiggsLibraryCode/include_higgs_gui_basics_2.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_higgs_gui_basics_2.cpp"
	$(V_AT)$(CXX) $(HIGGS_CXXFLAGS) $(HIGGS_CPPFLAGS_APP) $(HIGGS_CFLAGS_APP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/include_higgs_gui_basics_3_154ab539.o: ../../HiggsLibraryCode/include_higgs_gui_basics_3.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_higgs_gui_basics_3.cpp"
	$(V_AT)$(CXX) $(HIGGS_CXXFLAGS) $(HIGGS_CPPFLAGS_APP) $(HIGGS_CFLAGS_APP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/include_higgs_gui_basics_4_1558ccba.o: ../../HiggsLibraryCode/include_higgs_gui_basics_4.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_higgs_gui_basics_4.cpp"
	$(V_AT)$(CXX) $(HIGGS_CXXFLAGS) $(HIGGS_CPPFLAGS_APP) $(HIGGS_CFLAGS_APP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/include_higgs_gui_basics_5_1566e43b.o: ../../HiggsLibraryCode/include_higgs_gui_basics_5.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_higgs_gui_basics_5.cpp"
	$(V_AT)$(CXX) $(HIGGS_CXXFLAGS) $(HIGGS_CPPFLAGS_APP) $(HIGGS_CFLAGS_APP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/include_higgs_gui_extra_6dee1c1a.o: ../../HiggsLibraryCode/include_higgs_gui_extra.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling include_higgs_gui_extra.cpp"
	$(V_AT)$(CXX) $(HIGGS_CXXFLAGS) $(HIGGS_CPPFLAGS_APP) $(HIGGS_CFLAGS_APP) -o "$@" -c "$<"

$(HIGGS_OBJDIR)/execinfo.cmd:
	-$(V_AT)mkdir -p $(@D)
	-@if [ -z "$(V_AT)" ]; then echo "Checking if we need to link libexecinfo"; fi
	$(V_AT)printf "int main() { return 0; }" | $(CXX) -x c++ -o $(@D)/execinfo.x -lexecinfo - >/dev/null 2>&1 && printf -- "-lexecinfo" > "$@" || touch "$@"

$(HIGGS_OBJDIR)/cxxfs.cmd:
	-$(V_AT)mkdir -p $(@D)
	-@if [ -z "$(V_AT)" ]; then echo "Checking if we need to link stdc++fs"; fi
	$(V_AT)printf "int main() { return 0; }" | $(CXX) -x c++ -o $(@D)/cxxfs.x -lstdc++fs - >/dev/null 2>&1 && printf -- "-lstdc++fs" > "$@" || touch "$@"

clean:
	@echo Cleaning MythosStudio
	$(V_AT)$(CLEANCMD)

strip:
	@echo Stripping MythosStudio
	-$(V_AT)$(STRIP) --strip-unneeded $(HIGGS_OUTDIR)/$(HIGGS_TARGET_APP)

-include $(OBJECTS_APP:%.o=%.d)
