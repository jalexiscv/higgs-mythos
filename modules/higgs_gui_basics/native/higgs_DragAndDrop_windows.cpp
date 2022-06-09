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

namespace DragAndDropHelpers
{
    //==============================================================================
    struct HiggsDropSource final : public ComBaseClassHelper<IDropSource>
    {
        HiggsDropSource() = default;

        HIGGS_COMRESULT QueryContinueDrag (BOOL escapePressed, DWORD keys) override
        {
            if (escapePressed)
                return DRAGDROP_S_CANCEL;

            if ((keys & (MK_LBUTTON | MK_RBUTTON)) == 0)
                return DRAGDROP_S_DROP;

            return S_OK;
        }

        HIGGS_COMRESULT GiveFeedback (DWORD) override
        {
            return DRAGDROP_S_USEDEFAULTCURSORS;
        }
    };

    //==============================================================================
    struct HiggsEnumFormatEtc final : public ComBaseClassHelper<IEnumFORMATETC>
    {
        HiggsEnumFormatEtc (const FORMATETC* f)  : format (f) {}

        HIGGS_COMRESULT Clone (IEnumFORMATETC** result) override
        {
            if (result == nullptr)
                return E_POINTER;

            auto newOne = new HiggsEnumFormatEtc (format);
            newOne->index = index;
            *result = newOne;
            return S_OK;
        }

        HIGGS_COMRESULT Next (ULONG celt, LPFORMATETC lpFormatEtc, ULONG* pceltFetched) override
        {
            if (pceltFetched != nullptr)
                *pceltFetched = 0;
            else if (celt != 1)
                return S_FALSE;

            if (index == 0 && celt > 0 && lpFormatEtc != nullptr)
            {
                copyFormatEtc (lpFormatEtc [0], *format);
                ++index;

                if (pceltFetched != nullptr)
                    *pceltFetched = 1;

                return S_OK;
            }

            return S_FALSE;
        }

        HIGGS_COMRESULT Skip (ULONG celt) override
        {
            if (index + (int) celt >= 1)
                return S_FALSE;

            index += (int) celt;
            return S_OK;
        }

        HIGGS_COMRESULT Reset() override
        {
            index = 0;
            return S_OK;
        }

    private:
        const FORMATETC* const format;
        int index = 0;

        static void copyFormatEtc (FORMATETC& dest, const FORMATETC& source)
        {
            dest = source;

            if (source.ptd != nullptr)
            {
                dest.ptd = (DVTARGETDEVICE*) CoTaskMemAlloc (sizeof (DVTARGETDEVICE));

                if (dest.ptd != nullptr)
                    *(dest.ptd) = *(source.ptd);
            }
        }

        HIGGS_DECLARE_NON_COPYABLE (HiggsEnumFormatEtc)
    };

    //==============================================================================
    class HiggsDataObject final : public ComBaseClassHelper<IDataObject>
    {
    public:
        HiggsDataObject (const FORMATETC* f, const STGMEDIUM* m)
            : format (f), medium (m)
        {
        }

        ~HiggsDataObject() override
        {
            jassert (refCount == 0);
        }

        HIGGS_COMRESULT GetData (FORMATETC* pFormatEtc, STGMEDIUM* pMedium) override
        {
            if ((pFormatEtc->tymed & format->tymed) != 0
                 && pFormatEtc->cfFormat == format->cfFormat
                 && pFormatEtc->dwAspect == format->dwAspect)
            {
                pMedium->tymed = format->tymed;
                pMedium->pUnkForRelease = nullptr;

                if (format->tymed == TYMED_HGLOBAL)
                {
                    auto len = GlobalSize (medium->hGlobal);
                    void* const src = GlobalLock (medium->hGlobal);
                    void* const dst = GlobalAlloc (GMEM_FIXED, len);

                    if (src != nullptr && dst != nullptr)
                        memcpy (dst, src, len);

                    GlobalUnlock (medium->hGlobal);

                    pMedium->hGlobal = dst;
                    return S_OK;
                }
            }

            return DV_E_FORMATETC;
        }

        HIGGS_COMRESULT QueryGetData (FORMATETC* f) override
        {
            if (f == nullptr)
                return E_INVALIDARG;

            if (f->tymed == format->tymed
                  && f->cfFormat == format->cfFormat
                  && f->dwAspect == format->dwAspect)
                return S_OK;

            return DV_E_FORMATETC;
        }

        HIGGS_COMRESULT GetCanonicalFormatEtc (FORMATETC*, FORMATETC* pFormatEtcOut) override
        {
            pFormatEtcOut->ptd = nullptr;
            return E_NOTIMPL;
        }

        HIGGS_COMRESULT EnumFormatEtc (DWORD direction, IEnumFORMATETC** result) override
        {
            if (result == nullptr)
                return E_POINTER;

            if (direction == DATADIR_GET)
            {
                *result = new HiggsEnumFormatEtc (format);
                return S_OK;
            }

            *result = nullptr;
            return E_NOTIMPL;
        }

        HIGGS_COMRESULT GetDataHere (FORMATETC*, STGMEDIUM*)                  override { return DATA_E_FORMATETC; }
        HIGGS_COMRESULT SetData (FORMATETC*, STGMEDIUM*, BOOL)                override { return E_NOTIMPL; }
        HIGGS_COMRESULT DAdvise (FORMATETC*, DWORD, IAdviseSink*, DWORD*)     override { return OLE_E_ADVISENOTSUPPORTED; }
        HIGGS_COMRESULT DUnadvise (DWORD)                                     override { return E_NOTIMPL; }
        HIGGS_COMRESULT EnumDAdvise (IEnumSTATDATA**)                         override { return OLE_E_ADVISENOTSUPPORTED; }

    private:
        const FORMATETC* const format;
        const STGMEDIUM* const medium;

        HIGGS_DECLARE_NON_COPYABLE (HiggsDataObject)
    };

    //==============================================================================
    static HDROP createHDrop (const StringArray& fileNames)
    {
        size_t totalBytes = 0;
        for (int i = fileNames.size(); --i >= 0;)
            totalBytes += CharPointer_UTF16::getBytesRequiredFor (fileNames[i].getCharPointer()) + sizeof (WCHAR);

        struct Deleter
        {
            void operator() (void* ptr) const noexcept { GlobalFree (ptr); }
        };

        auto hDrop = std::unique_ptr<void, Deleter> ((HDROP) GlobalAlloc (GMEM_MOVEABLE | GMEM_ZEROINIT, sizeof (DROPFILES) + totalBytes + 4));

        if (hDrop != nullptr)
        {
            auto pDropFiles = (LPDROPFILES) GlobalLock (hDrop.get());

            if (pDropFiles == nullptr)
                return nullptr;

            pDropFiles->pFiles = sizeof (DROPFILES);
            pDropFiles->fWide = true;

            auto* fname = reinterpret_cast<WCHAR*> (addBytesToPointer (pDropFiles, sizeof (DROPFILES)));

            for (int i = 0; i < fileNames.size(); ++i)
            {
                auto bytesWritten = fileNames[i].copyToUTF16 (fname, 2048);
                fname = reinterpret_cast<WCHAR*> (addBytesToPointer (fname, bytesWritten));
            }

            *fname = 0;

            GlobalUnlock (hDrop.get());
        }

        return static_cast<HDROP> (hDrop.release());
    }

    struct DragAndDropJob final : public ThreadPoolJob
    {
        DragAndDropJob (FORMATETC f, STGMEDIUM m, DWORD d, std::function<void()>&& cb)
            : ThreadPoolJob ("DragAndDrop"),
              format (f), medium (m), whatToDo (d),
              completionCallback (std::move (cb))
        {
        }

        JobStatus runJob() override
        {
            [[maybe_unused]] const auto result = OleInitialize (nullptr);

            auto* source = new HiggsDropSource();
            auto* data = new HiggsDataObject (&format, &medium);

            DWORD effect;
            DoDragDrop (data, source, whatToDo, &effect);

            data->Release();
            source->Release();

            OleUninitialize();

            if (completionCallback != nullptr)
                MessageManager::callAsync (std::move (completionCallback));

            return jobHasFinished;
        }

        FORMATETC format;
        STGMEDIUM medium;
        DWORD whatToDo;

        std::function<void()> completionCallback;
    };

    class ThreadPoolHolder final : private DeletedAtShutdown
    {
    public:
        ThreadPoolHolder() = default;

        ~ThreadPoolHolder()
        {
            // Wait forever if there's a job running. The user needs to cancel the transfer
            // in the GUI.
            pool.removeAllJobs (true, -1);

            clearSingletonInstance();
        }

        HIGGS_DECLARE_SINGLETON_SINGLETHREADED_INLINE (ThreadPoolHolder, false)

        // We need to make sure we don't do simultaneous text and file drag and drops,
        // so use a pool that can only run a single job.
        ThreadPool pool { ThreadPoolOptions{}.withNumberOfThreads (1) };
    };
}

//==============================================================================
bool DragAndDropContainer::performExternalDragDropOfFiles (const StringArray& files, const bool canMove,
                                                           Component*, std::function<void()> callback)
{
    if (files.isEmpty())
        return false;

    FORMATETC format = { CF_HDROP, nullptr, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
    STGMEDIUM medium = { TYMED_HGLOBAL, { nullptr }, nullptr };

    medium.hGlobal = DragAndDropHelpers::createHDrop (files);

    auto& pool = DragAndDropHelpers::ThreadPoolHolder::getInstance()->pool;
    pool.addJob (new DragAndDropHelpers::DragAndDropJob (format, medium,
                                                         canMove ? (DROPEFFECT_COPY | DROPEFFECT_MOVE) : DROPEFFECT_COPY,
                                                         std::move (callback)),
                true);

    return true;
}

bool DragAndDropContainer::performExternalDragDropOfText (const String& text, Component*, std::function<void()> callback)
{
    if (text.isEmpty())
        return false;

    FORMATETC format = { CF_TEXT, nullptr, DVASPECT_CONTENT, -1, TYMED_HGLOBAL };
    STGMEDIUM medium = { TYMED_HGLOBAL, { nullptr }, nullptr };

    auto numBytes = CharPointer_UTF16::getBytesRequiredFor (text.getCharPointer());

    medium.hGlobal = GlobalAlloc (GMEM_MOVEABLE | GMEM_ZEROINIT, numBytes + 2);

    if (medium.hGlobal == nullptr)
        return false;

    auto* data = static_cast<WCHAR*> (GlobalLock (medium.hGlobal));

    text.copyToUTF16 (data, numBytes + 2);
    format.cfFormat = CF_UNICODETEXT;

    GlobalUnlock (medium.hGlobal);

    auto& pool = DragAndDropHelpers::ThreadPoolHolder::getInstance()->pool;
    pool.addJob (new DragAndDropHelpers::DragAndDropJob (format,
                                                        medium,
                                                        DROPEFFECT_COPY | DROPEFFECT_MOVE,
                                                        std::move (callback)),
                 true);

    return true;
}

} // namespace higgs
