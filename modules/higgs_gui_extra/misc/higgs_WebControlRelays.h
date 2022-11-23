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

#if HIGGS_WEB_BROWSER || DOXYGEN

/** Helper class that relays audio parameter information to an object inside a WebBrowserComponent.

    In order to create a relay you need to specify an identifier for the relayed state. This will
    result in a Javascript object becoming available inside the WebBrowserComponent under the
    provided identifier.

    Pass the relay object to WebBrowserComponent::Options::withOptionsFrom() to associate it with
    a WebBrowserComponent instance.

    You can then use a WebSliderParameterAttachment as you would a SliderAttachment, to attach the
    relay to a RangedAudioParameter. This will synchronise the state and events of the Javascript
    object with the audio parameter at all times.

    @code
    // Add a relay to your AudioProcessorEditor members
    WebSliderRelay cutoffSliderRelay { "cutoffSlider" };
    WebBrowserComponent webComponent { WebBrowserComponent::Options{}::withOptionsFrom (cutoffSliderRelay) };
    @endcode

    @code
    // In your Javascript GUI code you obtain an object from the framework
    import * as Higgs from "@jalexiscv/webview";
    const sliderState = Higgs.getSliderState("cutoffSlider");
    @endcode

    @see WebSliderParameterAttachment

    @tags{GUI}
*/
class HIGGS_API  WebSliderRelay : public OptionsBuilder<WebBrowserComponent::Options>,
                                 private WebViewLifetimeListener
{
public:
    /** Creating a relay will ensure that a Javascript object under the provided name will be
        available in the specified WebBrowserComponent's context. Use the frontend framework's
        getSliderState function with the same name to get a hold of this object.
    */
    WebSliderRelay (StringRef nameIn);

    //==============================================================================
    /** @internal */
    struct Listener : public SliderListener<WebSliderRelay>
    {
        virtual void initialUpdateRequested (WebSliderRelay*) = 0;
    };

    /** @internal */
    void setValue (float newValue);

    /** @internal */
    float getValue() const;

    /** @internal */
    void addListener (Listener* l);

    /** @internal */
    void removeListener (Listener* l);

    /** @internal */
    WebBrowserComponent::Options buildOptions (const WebBrowserComponent::Options& initialOptions) override;

    /** @internal */
    void emitEvent (const var& payload);

private:
    void handleEvent (const var& event);
    void webViewConstructed (WebBrowserComponent*) override;
    void webViewDestructed (WebBrowserComponent*) override;

    WebBrowserComponent* browser = nullptr;
    String name;
    float value{};
    Identifier eventId { "__higgs__slider" + name };
    ListenerList<Listener> listeners;

    HIGGS_DECLARE_NON_COPYABLE (WebSliderRelay)
    HIGGS_DECLARE_NON_MOVEABLE (WebSliderRelay)
};

/** Helper class that relays audio parameter information to an object inside a WebBrowserComponent.

    In order to create a relay you need to specify an identifier for the relayed state. This will
    result in a Javascript object becoming available inside the WebBrowserComponent under the
    provided identifier.

    Pass the relay object to WebBrowserComponent::Options::withOptionsFrom() to associate it with
    a WebBrowserComponent instance.

    You can then use a WebToggleButtonParameterAttachment as you would a ButtonParameterAttachment,
    to attach the relay to a RangedAudioParameter. This will synchronise the state and events of
    the Javascript object with the audio parameter at all times.

    @code
    // Add a relay to your AudioProcessorEditor members
    WebToggleButtonRelay muteToggleRelay { "muteToggle" };
    WebBrowserComponent webComponent { WebBrowserComponent::Options{}::withOptionsFrom (muteToggleRelay) };
    @endcode

    @code
    // In your Javascript GUI code you obtain an object from the framework
    import * as Higgs from "@jalexiscv/webview";
    const checkboxState = Higgs.getToggleState("muteToggle");
    @endcode

    @see WebToggleButtonParameterAttachment

    @tags{GUI}
*/
class HIGGS_API  WebToggleButtonRelay  : public OptionsBuilder<WebBrowserComponent::Options>,
                                        private WebViewLifetimeListener
{
public:
    /** Creating a relay will ensure that a Javascript object under the provided name will be
        available in the specified WebBrowserComponent's context. Use the frontend framework's
        getToggleState function with the same name to get a hold of this object.
    */
    WebToggleButtonRelay (StringRef nameIn);

    //==============================================================================
    /** @internal */
    struct Listener
    {
        virtual ~Listener()                          = default;
        virtual void toggleStateChanged (bool)       = 0;
        virtual void initialUpdateRequested()        = 0;
    };

    /** @internal */
    void setToggleState (bool newState);

    /** @internal */
    void addListener (Listener* l);

    /** @internal */
    void removeListener (Listener* l);

    /** @internal */
    WebBrowserComponent::Options buildOptions (const WebBrowserComponent::Options& initialOptions) override;

    /** @internal */
    void emitEvent (const var& payload);

private:
    void handleEvent (const var& event);
    void webViewConstructed (WebBrowserComponent*) override;
    void webViewDestructed (WebBrowserComponent*) override;

    WebBrowserComponent* browser = nullptr;
    String name;
    Identifier eventId { "__higgs__toggle" + name };
    ListenerList<Listener> listeners;

    HIGGS_DECLARE_NON_COPYABLE (WebToggleButtonRelay)
    HIGGS_DECLARE_NON_MOVEABLE (WebToggleButtonRelay)
};

/** Helper class that relays audio parameter information to an object inside a WebBrowserComponent.

    In order to create a relay you need to specify an identifier for the relayed state. This will
    result in a Javascript object becoming available inside the WebBrowserComponent under the
    provided identifier.

    Pass the relay object to WebBrowserComponent::Options::withOptionsFrom() to associate it with
    a WebBrowserComponent instance.

    You can then use a WebComboBoxParameterAttachment as you would a ComboBoxParameterAttachment,
    to attach the relay to a RangedAudioParameter. This will synchronise the state and events of
    the Javascript object with the audio parameter at all times.

    @code
    // Add a relay to your AudioProcessorEditor members
    WebComboBoxRelay filterTypeComboRelay { "filterTypeCombo" };
    WebBrowserComponent webComponent { WebBrowserComponent::Options{}::withOptionsFrom (filterTypeComboRelay) };
    @endcode

    @code
    // In your Javascript GUI code you obtain an object from the framework
    import * as Higgs from "@jalexiscv/webview";
    const comboBoxState = Higgs.getComboBoxState("filterTypeCombo");
    @endcode

    @see WebComboBoxParameterAttachment

    @tags{GUI}
*/
class HIGGS_API  WebComboBoxRelay  : public OptionsBuilder<WebBrowserComponent::Options>,
                                    private WebViewLifetimeListener
{
public:
    /** Creating a relay will ensure that a Javascript object under the provided name will be
        available in the specified WebBrowserComponent's context. Use the frontend framework's
        getComboBoxState function with the same name to get a hold of this object.
    */
    WebComboBoxRelay (StringRef nameIn);

    //==============================================================================
    /** @internal */
    struct Listener
    {
        virtual ~Listener()                          = default;
        virtual void valueChanged (float)            = 0;
        virtual void initialUpdateRequested()        = 0;
    };

    /** @internal */
    void setValue (float newValue);

    /** @internal */
    void addListener (Listener* l);

    /** @internal */
    void removeListener (Listener* l);

    /** @internal */
    WebBrowserComponent::Options buildOptions (const WebBrowserComponent::Options& initialOptions) override;

    /** @internal */
    void emitEvent (const var& payload);

private:
    void handleEvent (const var& event);
    void webViewConstructed (WebBrowserComponent*) override;
    void webViewDestructed (WebBrowserComponent*) override;

    WebBrowserComponent* browser = nullptr;
    String name;
    Identifier eventId { "__higgs__comboBox" + name };
    ListenerList<Listener> listeners;

    HIGGS_DECLARE_NON_COPYABLE (WebComboBoxRelay)
    HIGGS_DECLARE_NON_MOVEABLE (WebComboBoxRelay)
};

#endif

}
