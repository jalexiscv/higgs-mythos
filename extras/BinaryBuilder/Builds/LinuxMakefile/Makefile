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

  HIGGS_CPPFLAGS := $(DEPFLAGS) "-DLINUX=1" "-DDEBUG=1" "-D_DEBUG=1" "-DHIGGS_MYTHOSSTUDIO_VERSION=0x90005" "-DHIGGS_MODULE_AVAILABLE_higgs_core=1" "-DHIGGS_GLOBAL_MODULE_SETTINGS_INCLUDED=1" "-DHIGGS_STANDALONE_APPLICATION=1" "-DMYTHOS_LINUX_MAKE_6D53C8B4=1" "-DHIGGS_APP_VERSION=1.0.0" "-DHIGGS_APP_VERSION_HEX=0x10000" $(shell $(PKG_CONFIG) --cflags libcurl) -pthread -I../../HiggsLibraryCode -I../../../../modules $(CPPFLAGS)
  HIGGS_CPPFLAGS_CONSOLEAPP :=  "-DHiggsPlugin_Build_VST=0" "-DHiggsPlugin_Build_VST3=0" "-DHiggsPlugin_Build_AU=0" "-DHiggsPlugin_Build_AUv3=0" "-DHiggsPlugin_Build_AAX=0" "-DHiggsPlugin_Build_Standalone=0" "-DHiggsPlugin_Build_Unity=0" "-DHiggsPlugin_Build_LV2=0"
  HIGGS_TARGET_CONSOLEAPP := BinaryBuilder

  HIGGS_CFLAGS += $(HIGGS_CPPFLAGS) $(TARGET_ARCH) -g -ggdb -O0 $(CFLAGS)
  HIGGS_CXXFLAGS += $(HIGGS_CFLAGS) -std=c++17 $(CXXFLAGS)
  HIGGS_LDFLAGS += $(TARGET_ARCH) -L$(HIGGS_BINDIR) -L$(HIGGS_LIBDIR) $(shell $(PKG_CONFIG) --libs libcurl) -fvisibility=hidden -lrt -ldl -lpthread $(LDFLAGS)

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

  HIGGS_CPPFLAGS := $(DEPFLAGS) "-DLINUX=1" "-DNDEBUG=1" "-DHIGGS_MYTHOSSTUDIO_VERSION=0x90005" "-DHIGGS_MODULE_AVAILABLE_higgs_core=1" "-DHIGGS_GLOBAL_MODULE_SETTINGS_INCLUDED=1" "-DHIGGS_STANDALONE_APPLICATION=1" "-DMYTHOS_LINUX_MAKE_6D53C8B4=1" "-DHIGGS_APP_VERSION=1.0.0" "-DHIGGS_APP_VERSION_HEX=0x10000" $(shell $(PKG_CONFIG) --cflags libcurl) -pthread -I../../HiggsLibraryCode -I../../../../modules $(CPPFLAGS)
  HIGGS_CPPFLAGS_CONSOLEAPP :=  "-DHiggsPlugin_Build_VST=0" "-DHiggsPlugin_Build_VST3=0" "-DHiggsPlugin_Build_AU=0" "-DHiggsPlugin_Build_AUv3=0" "-DHiggsPlugin_Build_AAX=0" "-DHiggsPlugin_Build_Standalone=0" "-DHiggsPlugin_Build_Unity=0" "-DHiggsPlugin_Build_LV2=0"
  HIGGS_TARGET_CONSOLEAPP := BinaryBuilder

  HIGGS_CFLAGS += $(HIGGS_CPPFLAGS) $(TARGET_ARCH) -Os $(CFLAGS)
  HIGGS_CXXFLAGS += $(HIGGS_CFLAGS) -std=c++17 $(CXXFLAGS)
  HIGGS_LDFLAGS += $(TARGET_ARCH) -L$(HIGGS_BINDIR) -L$(HIGGS_LIBDIR) $(shell $(PKG_CONFIG) --libs libcurl) -fvisibility=hidden -lrt -ldl -lpthread $(LDFLAGS)

  CLEANCMD = rm -rf $(HIGGS_OUTDIR)/$(HIGGS_TARGET_CONSOLEAPP) $(HIGGS_OBJDIR)
endif

OBJECTS_CONSOLEAPP := \
  $(HIGGS_OBJDIR)/Main_90ebc5c2.o \
  $(HIGGS_OBJDIR)/include_higgs_core_f26d17db.o \
  $(HIGGS_OBJDIR)/include_higgs_core_CompilationTime_9257742c.o \
  $(HIGGS_OBJDIR)/include_higgs_core_zlib_ac61cd39.o \

.PHONY: clean all strip

all : $(HIGGS_OUTDIR)/$(HIGGS_TARGET_CONSOLEAPP)

$(HIGGS_OUTDIR)/$(HIGGS_TARGET_CONSOLEAPP) : $(OBJECTS_CONSOLEAPP) $(HIGGS_OBJDIR)/execinfo.cmd $(RESOURCES)
	@command -v $(PKG_CONFIG) >/dev/null 2>&1 || { echo >&2 "pkg-config not installed. Please, install it."; exit 1; }
	@$(PKG_CONFIG) --print-errors libcurl
	@echo Linking "BinaryBuilder - ConsoleApp"
	-$(V_AT)mkdir -p $(HIGGS_BINDIR)
	-$(V_AT)mkdir -p $(HIGGS_LIBDIR)
	-$(V_AT)mkdir -p $(HIGGS_OUTDIR)
	$(V_AT)$(CXX) -o $(HIGGS_OUTDIR)/$(HIGGS_TARGET_CONSOLEAPP) $(OBJECTS_CONSOLEAPP) $(HIGGS_LDFLAGS) $(shell cat $(HIGGS_OBJDIR)/execinfo.cmd) $(RESOURCES) $(TARGET_ARCH)

$(HIGGS_OBJDIR)/Main_90ebc5c2.o: ../../Source/Main.cpp
	-$(V_AT)mkdir -p $(@D)
	@echo "Compiling Main.cpp"
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

$(HIGGS_OBJDIR)/execinfo.cmd:
	-$(V_AT)mkdir -p $(@D)
	-@if [ -z "$(V_AT)" ]; then echo "Checking if we need to link libexecinfo"; fi
	$(V_AT)printf "int main() { return 0; }" | $(CXX) -x c++ -o $(@D)/execinfo.x -lexecinfo - >/dev/null 2>&1 && printf -- "-lexecinfo" > "$@" || touch "$@"

$(HIGGS_OBJDIR)/cxxfs.cmd:
	-$(V_AT)mkdir -p $(@D)
	-@if [ -z "$(V_AT)" ]; then echo "Checking if we need to link stdc++fs"; fi
	$(V_AT)printf "int main() { return 0; }" | $(CXX) -x c++ -o $(@D)/cxxfs.x -lstdc++fs - >/dev/null 2>&1 && printf -- "-lstdc++fs" > "$@" || touch "$@"

clean:
	@echo Cleaning BinaryBuilder
	$(V_AT)$(CLEANCMD)

strip:
	@echo Stripping BinaryBuilder
	-$(V_AT)$(STRIP) --strip-unneeded $(HIGGS_OUTDIR)/$(HIGGS_TARGET_CONSOLEAPP)

-include $(OBJECTS_CONSOLEAPP:%.o=%.d)
