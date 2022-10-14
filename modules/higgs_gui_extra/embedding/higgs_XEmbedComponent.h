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

/** @internal */
bool higgs_handleXEmbedEvent (ComponentPeer*, void*);
/** @internal */
unsigned long higgs_getCurrentFocusWindow (ComponentPeer*);

#if HIGGS_LINUX || HIGGS_BSD || DOXYGEN

/** Options for constructing an XEmbedComponent.

    @see XEmbedComponent
*/
class HIGGS_API XEmbedComponentOptions
{
public:
    /** Returns a copy of these options with the client window id specified. This corresponds to
        the client initiated embedding workflow.

        Omitting this option corresponds to the host initiated embedding workflow.
    */
    [[nodiscard]] XEmbedComponentOptions withClientWindow (unsigned long x) const
    {
        jassert (x != 0);
        return withMember (*this, &XEmbedComponentOptions::clientWindow, x);
    }

    /** Specifies that this Component wants to receive and forward keyboard focus.

        The default value is true.
    */
    [[nodiscard]] XEmbedComponentOptions withWantsKeyboardFocus (bool x) const
    {
        return withMember (*this, &XEmbedComponentOptions::wantsKeyboardFocus, x);
    }

    /** Specifies that the embedded window is allowed to resize the Component.

        The default value is false.
    */
    [[nodiscard]] XEmbedComponentOptions withAllowForeignWidgetToResizeComponent (bool x = true) const
    {
        return withMember (*this, &XEmbedComponentOptions::allowForeignWidgetToResizeComponent, x);
    }

    /** When this option is enabled the client window will be unconditionally mapped regardless of
        the value of the XEMBED_MAPPED flag.

        You should generally not use this option. The default value is false.

        It's used internally by HIGGS plugin hosting to accommodate some plugins that report
        XEmbedInfo compatibility, but don't set the flag.
    */
    [[nodiscard]] XEmbedComponentOptions withIgnoreXembedMapped (bool x = true) const
    {
        return withMember (*this, &XEmbedComponentOptions::ignoreXembedMapped, x);
    }

    /** @see withClientWindow() */
    [[nodiscard]] auto getClientWindow()                        const { return clientWindow; }

    /** @see withWantsKeyboardFocus() */
    [[nodiscard]] bool getWantsKeyboardFocus()                  const { return wantsKeyboardFocus; }

    /** @see withAllowForeignWidgetToResizeComponent() */
    [[nodiscard]] bool getAllowForeignWidgetToResizeComponent() const { return allowForeignWidgetToResizeComponent; }

    /** @see withIgnoreXembedMapped() */
    [[nodiscard]] bool getIgnoreXembedMapped()                  const { return ignoreXembedMapped; }

private:
    unsigned long clientWindow{};
    bool wantsKeyboardFocus = true;
    bool allowForeignWidgetToResizeComponent = false;
    bool ignoreXembedMapped = false;
};

//==============================================================================
/**
    A Linux-specific class that can embed a foreign X11 widget.

    Use this class to embed a foreign X11 widget from other toolkits such as
    GTK+ or QT.

    There are two ways to initiate the Xembed protocol. Either the client creates
    a window and passes this to the host (client initiated) or the host
    creates a window in which the client can reparent it's client widget
    (host initiated). XEmbedComponent supports both protocol types.

    This is how you embed a GTK+ widget: if you are using the client
    initiated version of the protocol, then create a new gtk widget with
    gtk_plug_new (0). Then query the window id of the plug via gtk_plug_get_id().
    Pass this id to the constructor of this class.

    If you are using the host initiated version of the protocol, then first create
    the XEmbedComponent using the default constructor. Use getHostWindowID to get
    the window id of the host, use this to construct your gtk plug via gtk_plug_new.

    A similar approach can be used to embed QT widgets via QT's QX11EmbedWidget
    class.

    Other toolkits or raw X11 widgets should follow the X11 embed protocol:
    https://specifications.freedesktop.org/xembed-spec/xembed-spec-latest.html

    @tags{GUI}
*/
class XEmbedComponent : public Component
{
public:
    //==============================================================================
    /** Creates a HIGGS component wrapping a foreign widget.

        Depending on the options passed, this constructor can be used for either
        the host initiated or client initiated version of the XEmbedProtocol.
    */
    explicit XEmbedComponent (const XEmbedComponentOptions& options);

    /** Creates a HIGGS component wrapping a foreign widget

        Use this constructor if you are using the host initiated version
        of the XEmbedProtocol. When using this version of the protocol
        you must call getHostWindowID() and pass this id to the foreign toolkit.
    */
    XEmbedComponent (bool wantsKeyboardFocus = true,
                     bool allowForeignWidgetToResizeComponent = false);

    /** Create a HIGGS component wrapping the foreign widget with id wID

        Use this constructor if you are using the client initiated version
        of the XEmbedProtocol.
    */
    XEmbedComponent (unsigned long wID, bool wantsKeyboardFocus = true,
                     bool allowForeignWidgetToResizeComponent = false);


    /** Destructor. */
    ~XEmbedComponent() override;

    /** Use this method to retrieve the host's window id when using the
        host initiated version of the XEmbedProtocol
    */
    unsigned long getHostWindowID();

    /** Removes the client window from the host. */
    void removeClient();

    /** Forces the embedded window to match the current size of this component. */
    void updateEmbeddedBounds();

protected:
    //==============================================================================
    /** @internal */
    void paint (Graphics&) override;
    void focusGainedWithDirection (FocusChangeType, FocusChangeDirection) override;
    void focusLost (FocusChangeType) override;
    void broughtToFront() override;

private:
    friend bool higgs::higgs_handleXEmbedEvent (ComponentPeer*, void*);
    friend unsigned long higgs_getCurrentFocusWindow (ComponentPeer*);

    class Pimpl;
    std::unique_ptr<Pimpl> pimpl;
};

#endif

} // namespace higgs
