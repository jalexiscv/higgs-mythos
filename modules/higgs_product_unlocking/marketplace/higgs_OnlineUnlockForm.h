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

/** Acts as a GUI which asks the user for their details, and calls the appropriate
    methods on your OnlineUnlockStatus object to attempt to register the app.

    You should create one of these components and add it to your parent window,
    or use a DialogWindow to display it as a pop-up. But if you're writing a plugin,
    then DO NOT USE A DIALOG WINDOW! Add it as a child component of your plugin's editor
    component instead. Plugins that pop up external registration windows are incredibly
    annoying, and cause all sorts of headaches for hosts. Don't be the person who
    writes that plugin that irritates everyone with a dialog box every time they
    try to scan for new plugins!

    Note that after adding it, you should put the component into a modal state,
    and it will automatically delete itself when it has completed.

    Although it deletes itself, it's also OK to delete it manually yourself
    if you need to get rid of it sooner.

    @see OnlineUnlockStatus

    @tags{ProductUnlocking}
*/
class HIGGS_API  OnlineUnlockForm  : public Component,
                                    private Button::Listener
{
public:
    /** Creates an unlock form that will work with the given status object.
        The userInstructions will be displayed above the email and password boxes.
    */
    OnlineUnlockForm (OnlineUnlockStatus&,
                      const String& userInstructions,
                      bool hasCancelButton = true,
                      bool overlayHasCancelButton = false);

    /** Destructor. */
    ~OnlineUnlockForm() override;

    /** This is called when the form is dismissed (either cancelled or when registration
        succeeds).
        By default it will delete this, but you can override it to do other things.
    */
    virtual void dismiss();

    /** @internal */
    void paint (Graphics&) override;
    /** @internal */
    void resized() override;
    /** @internal */
    void lookAndFeelChanged() override;

    Label message;
    TextEditor emailBox, passwordBox;
    TextButton registerButton, cancelButton;

private:
    OnlineUnlockStatus& status;
    std::unique_ptr<BubbleMessageComponent> bubble;

    bool showOverlayCancelButton;

    struct OverlayComp;
    friend struct OverlayComp;
    ScopedMessageBox messageBox;
    Component::SafePointer<Component> unlockingOverlay;

    void buttonClicked (Button*) override;
    void attemptRegistration();
    void showBubbleMessage (const String&, Component&);

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OnlineUnlockForm)
};

} // namespace higgs
