/*
  ==============================================================================

   This file is part of the Higgs Mythos framework.
   Copyright (c) Raw Material Software Limited

   Higgs Mythos is free software: you can redistribute it and/or modify
   it under the terms of the GNU Affero General Public License as published
   by the Free Software Foundation, either version 3 of the License, or
   (at your option) any later version.

   Higgs Mythos is distributed in the hope that it will be useful, but
   WITHOUT ANY WARRANTY; without even the implied warranty of
   MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
   GNU Affero General Public License for more details.

   You should have received a copy of the GNU Affero General Public License
   along with Higgs Mythos. If not, see <https://www.gnu.org/licenses/>.

  ==============================================================================
*/

namespace higgs
{

String SystemStats::getHIGGSVersion()
{
    // Some basic tests, to keep an eye on things and make sure these types work ok
    // on all platforms. Let me know if any of these assertions fail on your system!
    static_assert (sizeof (pointer_sized_int) == sizeof (void*), "Basic sanity test failed: please report!");
    static_assert (sizeof (int8) == 1,                           "Basic sanity test failed: please report!");
    static_assert (sizeof (uint8) == 1,                          "Basic sanity test failed: please report!");
    static_assert (sizeof (int16) == 2,                          "Basic sanity test failed: please report!");
    static_assert (sizeof (uint16) == 2,                         "Basic sanity test failed: please report!");
    static_assert (sizeof (int32) == 4,                          "Basic sanity test failed: please report!");
    static_assert (sizeof (uint32) == 4,                         "Basic sanity test failed: please report!");
    static_assert (sizeof (int64) == 8,                          "Basic sanity test failed: please report!");
    static_assert (sizeof (uint64) == 8,                         "Basic sanity test failed: please report!");

    return "Higgs Mythos v" HIGGS_STRINGIFY (HIGGS_MAJOR_VERSION)
                "." HIGGS_STRINGIFY (HIGGS_MINOR_VERSION)
                "." HIGGS_STRINGIFY (HIGGS_BUILDNUMBER);
}

#if HIGGS_ANDROID && ! defined (HIGGS_DISABLE_HIGGS_VERSION_PRINTING)
 #define HIGGS_DISABLE_HIGGS_VERSION_PRINTING 1
#endif

#if HIGGS_DEBUG && ! HIGGS_DISABLE_HIGGS_VERSION_PRINTING
 struct HiggsVersionPrinter
 {
     HiggsVersionPrinter()
     {
         DBG (SystemStats::getHIGGSVersion());
     }
 };

 static HiggsVersionPrinter higgsVersionPrinter;
#endif

StringArray SystemStats::getDeviceIdentifiers()
{
    for (const auto flag : { MachineIdFlags::fileSystemId, MachineIdFlags::macAddresses  })
        if (auto ids = getMachineIdentifiers (flag); ! ids.isEmpty())
            return ids;

    jassertfalse; // Failed to create any IDs!
    return {};
}

String getLegacyUniqueDeviceID();

StringArray SystemStats::getMachineIdentifiers (MachineIdFlags flags)
{
    auto macAddressProvider = [] (StringArray& arr)
    {
        for (const auto& mac : MACAddress::getAllAddresses())
            arr.add (mac.toString());
    };

    auto fileSystemProvider = [] (StringArray& arr)
    {
       #if HIGGS_WINDOWS
        File f (File::getSpecialLocation (File::windowsSystemDirectory));
       #else
        File f ("~");
       #endif
        if (auto num = f.getFileIdentifier())
            arr.add (String::toHexString ((int64) num));
    };

    auto legacyIdProvider = [] ([[maybe_unused]] StringArray& arr)
    {
       #if HIGGS_WINDOWS
        arr.add (getLegacyUniqueDeviceID());
       #endif
    };

    auto uniqueIdProvider = [] (StringArray& arr)
    {
        arr.add (getUniqueDeviceID());
    };

    struct Provider { MachineIdFlags flag; void (*func) (StringArray&); };
    static const Provider providers[] =
    {
        { MachineIdFlags::macAddresses,   macAddressProvider },
        { MachineIdFlags::fileSystemId,   fileSystemProvider },
        { MachineIdFlags::legacyUniqueId, legacyIdProvider },
        { MachineIdFlags::uniqueId,       uniqueIdProvider }
    };

    StringArray ids;

    for (const auto& provider : providers)
    {
        if (hasBitValueSet (flags, provider.flag))
            provider.func (ids);
    }

    return ids;
}

//==============================================================================
struct CPUInformation
{
    CPUInformation() noexcept    { initialise(); }

    void initialise() noexcept;

    int numLogicalCPUs = 0, numPhysicalCPUs = 0;

    bool hasMMX      = false, hasSSE        = false, hasSSE2       = false, hasSSE3       = false,
         has3DNow    = false, hasFMA3       = false, hasFMA4       = false, hasSSSE3      = false,
         hasSSE41    = false, hasSSE42      = false, hasAVX        = false, hasAVX2       = false,
         hasAVX512F  = false, hasAVX512BW   = false, hasAVX512CD   = false,
         hasAVX512DQ = false, hasAVX512ER   = false, hasAVX512IFMA = false,
         hasAVX512PF = false, hasAVX512VBMI = false, hasAVX512VL   = false,
         hasAVX512VPOPCNTDQ = false,
         hasNeon = false;
};

static const CPUInformation& getCPUInformation() noexcept
{
    static CPUInformation info;
    return info;
}

int SystemStats::getNumCpus() noexcept          { return getCPUInformation().numLogicalCPUs; }
int SystemStats::getNumPhysicalCpus() noexcept  { return getCPUInformation().numPhysicalCPUs; }
bool SystemStats::hasMMX() noexcept             { return getCPUInformation().hasMMX; }
bool SystemStats::has3DNow() noexcept           { return getCPUInformation().has3DNow; }
bool SystemStats::hasFMA3() noexcept            { return getCPUInformation().hasFMA3; }
bool SystemStats::hasFMA4() noexcept            { return getCPUInformation().hasFMA4; }
bool SystemStats::hasSSE() noexcept             { return getCPUInformation().hasSSE; }
bool SystemStats::hasSSE2() noexcept            { return getCPUInformation().hasSSE2; }
bool SystemStats::hasSSE3() noexcept            { return getCPUInformation().hasSSE3; }
bool SystemStats::hasSSSE3() noexcept           { return getCPUInformation().hasSSSE3; }
bool SystemStats::hasSSE41() noexcept           { return getCPUInformation().hasSSE41; }
bool SystemStats::hasSSE42() noexcept           { return getCPUInformation().hasSSE42; }
bool SystemStats::hasAVX() noexcept             { return getCPUInformation().hasAVX; }
bool SystemStats::hasAVX2() noexcept            { return getCPUInformation().hasAVX2; }
bool SystemStats::hasAVX512F() noexcept         { return getCPUInformation().hasAVX512F; }
bool SystemStats::hasAVX512BW() noexcept        { return getCPUInformation().hasAVX512BW; }
bool SystemStats::hasAVX512CD() noexcept        { return getCPUInformation().hasAVX512CD; }
bool SystemStats::hasAVX512DQ() noexcept        { return getCPUInformation().hasAVX512DQ; }
bool SystemStats::hasAVX512ER() noexcept        { return getCPUInformation().hasAVX512ER; }
bool SystemStats::hasAVX512IFMA() noexcept      { return getCPUInformation().hasAVX512IFMA; }
bool SystemStats::hasAVX512PF() noexcept        { return getCPUInformation().hasAVX512PF; }
bool SystemStats::hasAVX512VBMI() noexcept      { return getCPUInformation().hasAVX512VBMI; }
bool SystemStats::hasAVX512VL() noexcept        { return getCPUInformation().hasAVX512VL; }
bool SystemStats::hasAVX512VPOPCNTDQ() noexcept { return getCPUInformation().hasAVX512VPOPCNTDQ; }
bool SystemStats::hasNeon() noexcept            { return getCPUInformation().hasNeon; }


//==============================================================================
String SystemStats::getStackBacktrace()
{
    String result;

   #if HIGGS_WINDOWS
    HANDLE process = GetCurrentProcess();
    SymInitialize (process, nullptr, TRUE);

    void* stack[128];
    int frames = (int) CaptureStackBackTrace (0, numElementsInArray (stack), stack, nullptr);

    HeapBlock<SYMBOL_INFO> symbol;
    symbol.calloc (sizeof (SYMBOL_INFO) + 256, 1);
    symbol->MaxNameLen = 255;
    symbol->SizeOfStruct = sizeof (SYMBOL_INFO);

    for (int i = 0; i < frames; ++i)
    {
        DWORD64 displacement = 0;

        if (SymFromAddr (process, (DWORD64) stack[i], &displacement, symbol))
        {
            result << i << ": ";

            IMAGEHLP_MODULE64 moduleInfo;
            zerostruct (moduleInfo);
            moduleInfo.SizeOfStruct = sizeof (moduleInfo);

            if (::SymGetModuleInfo64 (process, symbol->ModBase, &moduleInfo))
                result << moduleInfo.ModuleName << ": ";

            result << symbol->Name << " + 0x" << String::toHexString ((int64) displacement) << newLine;
        }
    }

   #elif HIGGS_MAC || HIGGS_IOS || defined (__GLIBC__)

    void* stack[128];
    auto frames = backtrace (stack, numElementsInArray (stack));
    char** frameStrings = backtrace_symbols (stack, frames);

    for (auto i = (decltype (frames)) 0; i < frames; ++i)
        result << frameStrings[i] << newLine;

    ::free (frameStrings);

   #else

    jassertfalse; // sorry, not implemented yet!

   #endif

    return result;
}

//==============================================================================
#if ! HIGGS_WASM

static SystemStats::CrashHandlerFunction globalCrashHandler = nullptr;

#if HIGGS_WINDOWS
static LONG WINAPI handleCrash (LPEXCEPTION_POINTERS ep)
{
    globalCrashHandler (ep);
    return EXCEPTION_EXECUTE_HANDLER;
}
#else
static void handleCrash (int signum)
{
    globalCrashHandler ((void*) (pointer_sized_int) signum);
    ::kill (getpid(), SIGKILL);
}
#endif

void SystemStats::setApplicationCrashHandler (CrashHandlerFunction handler)
{
    jassert (handler != nullptr); // This must be a valid function.
    globalCrashHandler = handler;

   #if HIGGS_WINDOWS
    SetUnhandledExceptionFilter (handleCrash);
   #else
    const int signals[] = { SIGFPE, SIGILL, SIGSEGV, SIGBUS, SIGABRT, SIGSYS };

    for (int i = 0; i < numElementsInArray (signals); ++i)
    {
        ::signal (signals[i], handleCrash);
        higgs_siginterrupt (signals[i], 1);
    }
   #endif
}

#endif

bool SystemStats::isRunningInAppExtensionSandbox() noexcept
{
   #if HIGGS_MAC || HIGGS_IOS
    static bool isRunningInAppSandbox = [&]
    {
        File bundle = File::getSpecialLocation (File::invokedExecutableFile).getParentDirectory();

       #if HIGGS_MAC
        bundle = bundle.getParentDirectory().getParentDirectory();
       #endif

        if (bundle.isDirectory())
            return bundle.getFileExtension() == ".appex";

        return false;
    }();

    return isRunningInAppSandbox;
   #else
    return false;
   #endif
}

#if HIGGS_UNIT_TESTS

class UniqueHardwareIDTest final : public UnitTest
{
public:
    //==============================================================================
    UniqueHardwareIDTest() : UnitTest ("UniqueHardwareID", UnitTestCategories::analytics) {}

    void runTest() override
    {
        beginTest ("getUniqueDeviceID returns usable data.");
        {
            expect (SystemStats::getUniqueDeviceID().isNotEmpty());
        }
    }
};

static UniqueHardwareIDTest uniqueHardwareIDTest;

#endif

} // namespace higgs
