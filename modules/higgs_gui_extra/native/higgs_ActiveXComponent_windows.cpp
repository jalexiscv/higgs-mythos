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

extern int64 getMouseEventTime();

HIGGS_DECLARE_UUID_GETTER (IOleObject,       "00000112-0000-0000-C000-000000000046")
HIGGS_DECLARE_UUID_GETTER (IOleWindow,       "00000114-0000-0000-C000-000000000046")
HIGGS_DECLARE_UUID_GETTER (IOleInPlaceSite,  "00000119-0000-0000-C000-000000000046")

namespace ActiveXHelpers
{
    //==============================================================================
    struct HiggsIStorage final : public ComBaseClassHelper<IStorage>
    {
        HiggsIStorage() = default;

        HIGGS_COMRESULT CreateStream (const WCHAR*, DWORD, DWORD, DWORD, IStream**)                        override { return E_NOTIMPL; }
        HIGGS_COMRESULT OpenStream (const WCHAR*, void*, DWORD, DWORD, IStream**)                          override { return E_NOTIMPL; }
        HIGGS_COMRESULT CreateStorage (const WCHAR*, DWORD, DWORD, DWORD, IStorage**)                      override { return E_NOTIMPL; }
        HIGGS_COMRESULT OpenStorage (const WCHAR*, IStorage*, DWORD, SNB, DWORD, IStorage**)               override { return E_NOTIMPL; }
        HIGGS_COMRESULT CopyTo (DWORD, IID const*, SNB, IStorage*)                                         override { return E_NOTIMPL; }
        HIGGS_COMRESULT MoveElementTo (const OLECHAR*,IStorage*, const OLECHAR*, DWORD)                    override { return E_NOTIMPL; }
        HIGGS_COMRESULT Commit (DWORD)                                                                     override { return E_NOTIMPL; }
        HIGGS_COMRESULT Revert()                                                                           override { return E_NOTIMPL; }
        HIGGS_COMRESULT EnumElements (DWORD, void*, DWORD, IEnumSTATSTG**)                                 override { return E_NOTIMPL; }
        HIGGS_COMRESULT DestroyElement (const OLECHAR*)                                                    override { return E_NOTIMPL; }
        HIGGS_COMRESULT RenameElement (const WCHAR*, const WCHAR*)                                         override { return E_NOTIMPL; }
        HIGGS_COMRESULT SetElementTimes (const WCHAR*, FILETIME const*, FILETIME const*, FILETIME const*)  override { return E_NOTIMPL; }
        HIGGS_COMRESULT SetClass (REFCLSID)                                                                override { return S_OK; }
        HIGGS_COMRESULT SetStateBits (DWORD, DWORD)                                                        override { return E_NOTIMPL; }
        HIGGS_COMRESULT Stat (STATSTG*, DWORD)                                                             override { return E_NOTIMPL; }
    };

    //==============================================================================
    struct HiggsOleInPlaceFrame final : public ComBaseClassHelper<IOleInPlaceFrame>
    {
        HiggsOleInPlaceFrame (HWND hwnd)   : window (hwnd) {}

        HIGGS_COMRESULT GetWindow (HWND* lphwnd)                                 override { *lphwnd = window; return S_OK; }
        HIGGS_COMRESULT ContextSensitiveHelp (BOOL)                              override { return E_NOTIMPL; }
        HIGGS_COMRESULT GetBorder (LPRECT)                                       override { return E_NOTIMPL; }
        HIGGS_COMRESULT RequestBorderSpace (LPCBORDERWIDTHS)                     override { return E_NOTIMPL; }
        HIGGS_COMRESULT SetBorderSpace (LPCBORDERWIDTHS)                         override { return E_NOTIMPL; }
        HIGGS_COMRESULT InsertMenus (HMENU, LPOLEMENUGROUPWIDTHS)                override { return E_NOTIMPL; }
        HIGGS_COMRESULT SetMenu (HMENU, HOLEMENU, HWND)                          override { return S_OK; }
        HIGGS_COMRESULT RemoveMenus (HMENU)                                      override { return E_NOTIMPL; }
        HIGGS_COMRESULT SetStatusText (LPCOLESTR)                                override { return S_OK; }
        HIGGS_COMRESULT EnableModeless (BOOL)                                    override { return S_OK; }
        HIGGS_COMRESULT TranslateAccelerator (LPMSG, WORD)                       override { return E_NOTIMPL; }

        HIGGS_COMRESULT SetActiveObject (IOleInPlaceActiveObject* a, LPCOLESTR)  override
        {
            activeObject = ComSmartPtr (a, IncrementRef::yes);
            return S_OK;
        }

        HRESULT OfferKeyTranslation (LPMSG lpmsg)
        {
            if (activeObject != nullptr)
                return activeObject->TranslateAcceleratorW (lpmsg);

            return S_FALSE;
        }

        HWND window;
        ComSmartPtr<IOleInPlaceActiveObject> activeObject;
    };

    //==============================================================================
    struct HiggsIOleInPlaceSite final : public ComBaseClassHelper<IOleInPlaceSite>
    {
        HiggsIOleInPlaceSite (HWND hwnd)
            : window (hwnd),
              frame (new HiggsOleInPlaceFrame (window))
        {}

        ~HiggsIOleInPlaceSite() override
        {
            frame->Release();
        }

        HIGGS_COMRESULT GetWindow (HWND* lphwnd)      override { *lphwnd = window; return S_OK; }
        HIGGS_COMRESULT ContextSensitiveHelp (BOOL)   override { return E_NOTIMPL; }
        HIGGS_COMRESULT CanInPlaceActivate()          override { return S_OK; }
        HIGGS_COMRESULT OnInPlaceActivate()           override { return S_OK; }
        HIGGS_COMRESULT OnUIActivate()                override { return S_OK; }

        HIGGS_COMRESULT GetWindowContext (LPOLEINPLACEFRAME* lplpFrame, LPOLEINPLACEUIWINDOW* lplpDoc, LPRECT, LPRECT, LPOLEINPLACEFRAMEINFO lpFrameInfo) override
        {
            /* Note: If you call AddRef on the frame here, then some types of object (e.g. web browser control) cause leaks.
               If you don't call AddRef then others crash (e.g. QuickTime). Bit of a catch-22, so letting it leak is probably preferable.
            */
            if (lplpFrame != nullptr) { frame->AddRef(); *lplpFrame = frame; }
            if (lplpDoc != nullptr)   *lplpDoc = nullptr;
            lpFrameInfo->fMDIApp = FALSE;
            lpFrameInfo->hwndFrame = window;
            lpFrameInfo->haccel = nullptr;
            lpFrameInfo->cAccelEntries = 0;
            return S_OK;
        }

        HIGGS_COMRESULT Scroll (SIZE)                 override { return E_NOTIMPL; }
        HIGGS_COMRESULT OnUIDeactivate (BOOL)         override { return S_OK; }
        HIGGS_COMRESULT OnInPlaceDeactivate()         override { return S_OK; }
        HIGGS_COMRESULT DiscardUndoState()            override { return E_NOTIMPL; }
        HIGGS_COMRESULT DeactivateAndUndo()           override { return E_NOTIMPL; }
        HIGGS_COMRESULT OnPosRectChange (LPCRECT)     override { return S_OK; }

        LRESULT offerEventToActiveXControl (::MSG& msg)
        {
            if (frame != nullptr)
                return frame->OfferKeyTranslation (&msg);

            return S_FALSE;
        }

        HWND window;
        HiggsOleInPlaceFrame* frame;
    };

    //==============================================================================
    struct HiggsIOleClientSite final : public ComBaseClassHelper<IOleClientSite>
    {
        HiggsIOleClientSite (HWND window)  : inplaceSite (new HiggsIOleInPlaceSite (window))
        {}

        ~HiggsIOleClientSite() override
        {
            inplaceSite->Release();

            if (dispatchEventHandler != nullptr)
            {
                dispatchEventHandler->Release();
                dispatchEventHandler = nullptr;
            }
        }

        HIGGS_COMRESULT QueryInterface (REFIID type, void** result) override
        {
            HIGGS_BEGIN_IGNORE_WARNINGS_GCC_LIKE ("-Wlanguage-extension-token")

            if (type == __uuidof (IOleInPlaceSite))
            {
                inplaceSite->AddRef();
                *result = static_cast<IOleInPlaceSite*> (inplaceSite);
                return S_OK;
            }
            else if (type == __uuidof (IDispatch) && dispatchEventHandler != nullptr)
            {
                dispatchEventHandler->AddRef();
                *result = dispatchEventHandler;
                return S_OK;
            }

            return ComBaseClassHelper <IOleClientSite>::QueryInterface (type, result);

            HIGGS_END_IGNORE_WARNINGS_GCC_LIKE
        }

        HIGGS_COMRESULT SaveObject()                                  override { return E_NOTIMPL; }
        HIGGS_COMRESULT GetMoniker (DWORD, DWORD, IMoniker**)         override { return E_NOTIMPL; }
        HIGGS_COMRESULT GetContainer (LPOLECONTAINER* ppContainer)    override { *ppContainer = nullptr; return E_NOINTERFACE; }
        HIGGS_COMRESULT ShowObject()                                  override { return S_OK; }
        HIGGS_COMRESULT OnShowWindow (BOOL)                           override { return E_NOTIMPL; }
        HIGGS_COMRESULT RequestNewObjectLayout()                      override { return E_NOTIMPL; }

        LRESULT offerEventToActiveXControl (::MSG& msg)
        {
            if (inplaceSite != nullptr)
                return inplaceSite->offerEventToActiveXControl (msg);

            return S_FALSE;
        }

        void setEventHandler (void* eventHandler)
        {
            IDispatch* newEventHandler = nullptr;

            if (eventHandler != nullptr)
            {
                HIGGS_BEGIN_IGNORE_WARNINGS_GCC_LIKE ("-Wlanguage-extension-token")

                auto iidIDispatch = __uuidof (IDispatch);

                if (static_cast<IUnknown*> (eventHandler)->QueryInterface (iidIDispatch, (void**) &newEventHandler) != S_OK
                    || newEventHandler == nullptr)
                    return;

               HIGGS_END_IGNORE_WARNINGS_GCC_LIKE
            }

            if (dispatchEventHandler != nullptr)
                dispatchEventHandler->Release();

            dispatchEventHandler = newEventHandler;
        }

        HiggsIOleInPlaceSite* inplaceSite;
        IDispatch* dispatchEventHandler = nullptr;
    };

    //==============================================================================
    static Array<ActiveXControlComponent*> activeXComps;

    static HWND getHWND (const ActiveXControlComponent* const component)
    {
        HIGGS_BEGIN_IGNORE_WARNINGS_GCC_LIKE ("-Wlanguage-extension-token")

        HWND hwnd = {};
        const IID iid = __uuidof (IOleWindow);

        if (auto* window = (IOleWindow*) component->queryInterface (&iid))
        {
            window->GetWindow (&hwnd);
            window->Release();
        }

        return hwnd;

        HIGGS_END_IGNORE_WARNINGS_GCC_LIKE
    }

    static void offerActiveXMouseEventToPeer (ComponentPeer* peer, HWND hwnd, UINT message, LPARAM lParam)
    {
        switch (message)
        {
            case WM_MOUSEMOVE:
            case WM_LBUTTONDOWN:
            case WM_MBUTTONDOWN:
            case WM_RBUTTONDOWN:
            case WM_LBUTTONUP:
            case WM_MBUTTONUP:
            case WM_RBUTTONUP:
            {
                RECT activeXRect, peerRect;
                GetWindowRect (hwnd, &activeXRect);
                GetWindowRect ((HWND) peer->getNativeHandle(), &peerRect);

                peer->handleMouseEvent (MouseInputSource::InputSourceType::mouse,
                                        { (float) (GET_X_LPARAM (lParam) + activeXRect.left - peerRect.left),
                                          (float) (GET_Y_LPARAM (lParam) + activeXRect.top  - peerRect.top) },
                                        ComponentPeer::getCurrentModifiersRealtime(),
                                        MouseInputSource::defaultPressure,
                                        MouseInputSource::defaultOrientation,
                                        getMouseEventTime());
                break;
            }

            default:
                break;
        }
    }
}

//==============================================================================
class ActiveXControlComponent::Pimpl  : public ComponentMovementWatcher
{
public:
    Pimpl (HWND hwnd, ActiveXControlComponent& activeXComp)
        : ComponentMovementWatcher (&activeXComp),
          owner (activeXComp),
          storage (new ActiveXHelpers::HiggsIStorage()),
          clientSite (new ActiveXHelpers::HiggsIOleClientSite (hwnd))
    {
    }

    ~Pimpl() override
    {
        // If the wndproc of the ActiveX HWND isn't set back to it's original
        // wndproc, then clientSite will leak when control is released
        if (controlHWND != nullptr)
            SetWindowLongPtr ((HWND) controlHWND, GWLP_WNDPROC, (LONG_PTR) originalWndProc);

        if (control != nullptr)
        {
            control->Close (OLECLOSE_NOSAVE);
            control->Release();
        }

        clientSite->Release();
        storage->Release();
    }

    void setControlBounds (Rectangle<int> newBounds) const
    {
        if (controlHWND != nullptr)
        {
            if (auto* peer = owner.getTopLevelComponent()->getPeer())
                newBounds = (newBounds.toDouble() * peer->getPlatformScaleFactor()).toNearestInt();

            MoveWindow (controlHWND, newBounds.getX(), newBounds.getY(), newBounds.getWidth(), newBounds.getHeight(), TRUE);
        }
    }

    void setControlVisible (bool shouldBeVisible) const
    {
        if (controlHWND != nullptr)
            ShowWindow (controlHWND, shouldBeVisible ? SW_SHOWNA : SW_HIDE);
    }

    //==============================================================================
    using ComponentMovementWatcher::componentMovedOrResized;

    void componentMovedOrResized (bool /*wasMoved*/, bool /*wasResized*/) override
    {
        if (auto* peer = owner.getTopLevelComponent()->getPeer())
            setControlBounds (peer->getAreaCoveredBy (owner));
    }

    void componentPeerChanged() override {}

    using ComponentMovementWatcher::componentVisibilityChanged;

    void componentVisibilityChanged() override
    {
        setControlVisible (owner.isShowing());
    }

    void nativeScaleFactorChanged()
    {
        componentMovedOrResized (true, true);
    }

    // intercepts events going to an activeX control, so we can sneakily use the mouse events
    static LRESULT CALLBACK activeXHookWndProc (HWND hwnd, UINT message, WPARAM wParam, LPARAM lParam)
    {
        for (auto* ax : ActiveXHelpers::activeXComps)
        {
            if (ax->control != nullptr && ax->control->controlHWND == hwnd)
            {
                switch (message)
                {
                    case WM_MOUSEMOVE:
                    case WM_LBUTTONDOWN:
                    case WM_MBUTTONDOWN:
                    case WM_RBUTTONDOWN:
                    case WM_LBUTTONUP:
                    case WM_MBUTTONUP:
                    case WM_RBUTTONUP:
                    case WM_LBUTTONDBLCLK:
                    case WM_MBUTTONDBLCLK:
                    case WM_RBUTTONDBLCLK:
                        if (ax->isShowing())
                        {
                            if (auto* peer = ax->getPeer())
                            {
                                ActiveXHelpers::offerActiveXMouseEventToPeer (peer, hwnd, message, lParam);

                                if (! ax->areMouseEventsAllowed())
                                    return 0;
                            }
                        }
                        break;

                    default:
                        break;
                }

                return CallWindowProc (ax->control->originalWndProc, hwnd, message, wParam, lParam);
            }
        }

        return DefWindowProc (hwnd, message, wParam, lParam);
    }

    ActiveXControlComponent& owner;
    ComponentPeer* currentPeer = nullptr;
    HWND controlHWND = {};
    IStorage* storage = nullptr;
    ActiveXHelpers::HiggsIOleClientSite* clientSite = nullptr;
    IOleObject* control = nullptr;
    WNDPROC originalWndProc = nullptr;
    NativeScaleFactorNotifier notifier { &owner, [this] (auto) { nativeScaleFactorChanged(); } };
};

//==============================================================================
ActiveXControlComponent::ActiveXControlComponent()
{
    ActiveXHelpers::activeXComps.add (this);
}

ActiveXControlComponent::~ActiveXControlComponent()
{
    deleteControl();
    ActiveXHelpers::activeXComps.removeFirstMatchingValue (this);
}

void ActiveXControlComponent::paint (Graphics& g)
{
    if (control == nullptr)
        g.fillAll (Colours::lightgrey);
}

bool ActiveXControlComponent::createControl (const void* controlIID)
{
    deleteControl();

    if (auto* peer = getPeer())
    {
        auto controlBounds = peer->getAreaCoveredBy (*this);
        auto hwnd = (HWND) peer->getNativeHandle();

        std::unique_ptr<Pimpl> newControl (new Pimpl (hwnd, *this));

        HIGGS_BEGIN_IGNORE_WARNINGS_GCC_LIKE ("-Wlanguage-extension-token")

        HRESULT hr = OleCreate (*(const IID*) controlIID, __uuidof (IOleObject), 1 /*OLERENDER_DRAW*/, nullptr,
                                newControl->clientSite, newControl->storage,
                                (void**) &(newControl->control));

        HIGGS_END_IGNORE_WARNINGS_GCC_LIKE

        if (hr == S_OK)
        {
            newControl->control->SetHostNames (L"HIGGS", nullptr);

            if (OleSetContainedObject (newControl->control, TRUE) == S_OK)
            {
                RECT rect;
                rect.left   = controlBounds.getX();
                rect.top    = controlBounds.getY();
                rect.right  = controlBounds.getRight();
                rect.bottom = controlBounds.getBottom();

                if (newControl->control->DoVerb (OLEIVERB_SHOW, nullptr, newControl->clientSite, 0, hwnd, &rect) == S_OK)
                {
                    control = std::move (newControl);
                    control->controlHWND = ActiveXHelpers::getHWND (this);

                    if (control->controlHWND != nullptr)
                    {
                        control->setControlBounds (controlBounds);

                        control->originalWndProc = (WNDPROC) GetWindowLongPtr ((HWND) control->controlHWND, GWLP_WNDPROC);
                        SetWindowLongPtr ((HWND) control->controlHWND, GWLP_WNDPROC, (LONG_PTR) Pimpl::activeXHookWndProc);
                    }

                    return true;
                }
            }
        }
    }
    else
    {
        // the component must have already been added to a real window when you call this!
        jassertfalse;
    }

    return false;
}

void ActiveXControlComponent::deleteControl()
{
    control = nullptr;
}

void* ActiveXControlComponent::queryInterface (const void* iid) const
{
    void* result = nullptr;

    if (control != nullptr && control->control != nullptr
         && SUCCEEDED (control->control->QueryInterface (*(const IID*) iid, &result)))
        return result;

    return nullptr;
}

void ActiveXControlComponent::setMouseEventsAllowed (const bool eventsCanReachControl)
{
    mouseEventsAllowed = eventsCanReachControl;
}

intptr_t ActiveXControlComponent::offerEventToActiveXControl (void* ptr)
{
    if (control != nullptr && control->clientSite != nullptr)
        return (intptr_t) control->clientSite->offerEventToActiveXControl (*reinterpret_cast<::MSG*> (ptr));

    return S_FALSE;
}

intptr_t ActiveXControlComponent::offerEventToActiveXControlStatic (void* ptr)
{
    for (auto* ax : ActiveXHelpers::activeXComps)
    {
        auto result = ax->offerEventToActiveXControl (ptr);

        if (result != S_FALSE)
            return result;
    }

    return S_FALSE;
}

void ActiveXControlComponent::setEventHandler (void* eventHandler)
{
    if (control->clientSite != nullptr)
        control->clientSite->setEventHandler (eventHandler);
}

LRESULT higgs_offerEventToActiveXControl (::MSG& msg);
LRESULT higgs_offerEventToActiveXControl (::MSG& msg)
{
    if (msg.message >= WM_KEYFIRST && msg.message <= WM_KEYLAST)
        return ActiveXControlComponent::offerEventToActiveXControlStatic (&msg);

    return S_FALSE;
}

} // namespace higgs
