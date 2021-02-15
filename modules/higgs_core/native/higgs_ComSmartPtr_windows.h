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

#if ! defined (_MSC_VER) && ! defined (__uuidof)
 #ifdef __uuidof
  #undef __uuidof
 #endif

 template <typename Type> struct UUIDGetter { static CLSID get() { jassertfalse; return {}; } };
 #define __uuidof(x)  UUIDGetter<x>::get()

 template <>
 struct UUIDGetter<::IUnknown>
 {
     static CLSID get()     { return { 0, 0, 0, { 0xc0, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00, 0x46 } }; }
 };

 #define HIGGS_DECLARE_UUID_GETTER(name, uuid) \
    template <> struct UUIDGetter<name> { static CLSID get()  { return uuidFromString (uuid); } };

 #define HIGGS_COMCLASS(name, guid) \
    struct name; \
    HIGGS_DECLARE_UUID_GETTER (name, guid) \
    struct name

#else
 #define HIGGS_DECLARE_UUID_GETTER(name, uuid)
 #define HIGGS_COMCLASS(name, guid)       struct DECLSPEC_UUID (guid) name
#endif

#define HIGGS_IUNKNOWNCLASS(name, guid)   HIGGS_COMCLASS(name, guid) : public IUnknown
#define HIGGS_COMRESULT                   HRESULT STDMETHODCALLTYPE
#define HIGGS_COMCALL                     virtual HRESULT STDMETHODCALLTYPE

HIGGS_BEGIN_IGNORE_WARNINGS_GCC_LIKE ("-Wlanguage-extension-token")

inline GUID uuidFromString (const char* s) noexcept
{
    uint32 ints[4] = {};

    for (uint32 digitIndex = 0; digitIndex < 32;)
    {
        auto c = (uint32) *s++;
        uint32 digit;

        if (c >= '0' && c <= '9')       digit = c - '0';
        else if (c >= 'a' && c <= 'f')  digit = c - 'a' + 10;
        else if (c >= 'A' && c <= 'F')  digit = c - 'A' + 10;
        else if (c == '-')              continue;
        else break;

        ints[digitIndex / 8] |= (digit << 4 * (7 - (digitIndex & 7)));
        ++digitIndex;
    }

    return { ints[0],
             (uint16) (ints[1] >> 16),
             (uint16) ints[1],
             { (uint8) (ints[2] >> 24), (uint8) (ints[2] >> 16), (uint8) (ints[2] >> 8), (uint8) ints[2],
               (uint8) (ints[3] >> 24), (uint8) (ints[3] >> 16), (uint8) (ints[3] >> 8), (uint8) ints[3] }};
}

//==============================================================================
/** A simple COM smart pointer.

    @tags{Core}
*/
template <class ComClass>
class ComSmartPtr
{
public:
    ComSmartPtr() noexcept = default;
    ComSmartPtr (std::nullptr_t) noexcept {}

    ComSmartPtr (ComClass* object, IncrementRef incrementRef) noexcept
        : p (object)
    {
        if (p != nullptr && incrementRef == IncrementRef::yes)
            p->AddRef();
    }

    template <typename U>
    ComSmartPtr (const ComSmartPtr<U>& other) : ComSmartPtr (other, IncrementRef::yes) {}
    ComSmartPtr (const ComSmartPtr&    other) : ComSmartPtr (other, IncrementRef::yes) {}

    ~ComSmartPtr() noexcept { release(); }

    template <typename U>
    ComSmartPtr& operator= (const ComSmartPtr<U>& newP) { ComSmartPtr copy { newP }; std::swap (copy.p, p); return *this; }
    ComSmartPtr& operator= (const ComSmartPtr&    newP) { ComSmartPtr copy { newP }; std::swap (copy.p, p); return *this; }

    ComClass* get() const noexcept { return p; }

    operator ComClass*() const noexcept     { return p; }
    ComClass& operator*() const noexcept    { return *p; }
    ComClass* operator->() const noexcept   { return p; }

    // Releases and nullifies this pointer and returns its address
    ComClass** resetAndGetPointerAddress()
    {
        release();
        return &p;
    }

    HRESULT CoCreateInstance (REFCLSID classUUID, DWORD dwClsContext = CLSCTX_INPROC_SERVER)
    {
        auto hr = ::CoCreateInstance (classUUID, nullptr, dwClsContext, __uuidof (ComClass), (void**) resetAndGetPointerAddress());
        jassert (hr != CO_E_NOTINITIALIZED); // You haven't called CoInitialize for the current thread!
        return hr;
    }

    template <class OtherComClass>
    HRESULT QueryInterface (REFCLSID classUUID, ComSmartPtr<OtherComClass>& destObject) const
    {
        if (p == nullptr)
            return E_POINTER;

        return p->QueryInterface (classUUID, (void**) destObject.resetAndGetPointerAddress());
    }

    template <class OtherComClass>
    HRESULT QueryInterface (ComSmartPtr<OtherComClass>& destObject) const
    {
        return this->QueryInterface (__uuidof (OtherComClass), destObject);
    }

    template <class OtherComClass>
    ComSmartPtr<OtherComClass> getInterface() const
    {
        ComSmartPtr<OtherComClass> destObject;

        if (const auto hr = QueryInterface (destObject); FAILED (hr))
            return {};

        return destObject;
    }

private:
    template <typename U>
    friend class ComSmartPtr;

    void release()
    {
        if (auto* q = std::exchange (p, nullptr))
            q->Release();
    }

    ComClass** operator&() noexcept; // private to avoid it being used accidentally

    ComClass* p = nullptr;
};

//==============================================================================
template <class First, class... ComClasses>
class ComBaseClassHelperBase   : public First, public ComClasses...
{
public:
    ComBaseClassHelperBase() = default;
    virtual ~ComBaseClassHelperBase() = default;

    ULONG STDMETHODCALLTYPE AddRef()    override { return ++refCount; }
    ULONG STDMETHODCALLTYPE Release()   override { auto r = --refCount; if (r == 0) delete this; return r; }

protected:
    ULONG refCount = 1;

    HIGGS_COMRESULT QueryInterface (REFIID refId, void** result) override
    {
        if (refId == __uuidof (IUnknown))
            return castToType<First> (result);

        *result = nullptr;
        return E_NOINTERFACE;
    }

    template <class Type>
    HIGGS_COMRESULT castToType (void** result)
    {
        this->AddRef();
        *result = dynamic_cast<Type*> (this);

        return S_OK;
    }
};

/** Handy base class for writing COM objects, providing ref-counting and a basic QueryInterface method.

    @tags{Core}
*/
template <class... ComClasses>
class ComBaseClassHelper   : public ComBaseClassHelperBase<ComClasses...>
{
public:
    ComBaseClassHelper() = default;

    HIGGS_COMRESULT QueryInterface (REFIID refId, void** result) override
    {
        const std::tuple<IID, void*> bases[]
        {
            std::make_tuple (__uuidof (ComClasses),
                             static_cast<void*> (static_cast<ComClasses*> (this)))...
        };

        for (const auto& base : bases)
        {
            if (refId == std::get<0> (base))
            {
                this->AddRef();
                *result = std::get<1> (base);
                return S_OK;
            }
        }

        return ComBaseClassHelperBase<ComClasses...>::QueryInterface (refId, result);
    }
};

HIGGS_END_IGNORE_WARNINGS_GCC_LIKE

} // namespace higgs
