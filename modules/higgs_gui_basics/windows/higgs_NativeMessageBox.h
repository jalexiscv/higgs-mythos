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

//==============================================================================
/**
    This class contains some static methods for showing native alert windows.

    @tags{GUI}
*/
class NativeMessageBox
{
public:
   #if HIGGS_MODAL_LOOPS_PERMITTED
    /** Shows a dialog box that just has a message and a single 'ok' button to close it.

        The box is shown modally, and the method will block until the user has clicked its
        button (or pressed the escape or return keys).

        @param iconType     the type of icon to show.
        @param title        the headline to show at the top of the box.
        @param message      a longer, more descriptive message to show underneath the title.
        @param associatedComponent   if this is non-null, it specifies the component that the
                            alert window should be associated with. Depending on the look
                            and feel, this might be used for positioning of the alert window.
    */
    static void HIGGS_CALLTYPE showMessageBox (MessageBoxIconType iconType,
                                              const String& title,
                                              const String& message,
                                              Component* associatedComponent = nullptr);

    /** Shows a dialog box using the specified options.

        The box is shown modally, and the method will block until the user dismisses it.

        @param options  the options to use when creating the dialog.

        @returns  the result code of the button that was clicked.

        @see MessageBoxOptions
    */
    static int HIGGS_CALLTYPE showForOptions (const MessageBoxOptions& options);

    /** This function is now called showForOptions().

        When updating your code, make sure to check that your code correctly handles
        the result codes that are specified in the MessageBoxOptions.
    */
    static int HIGGS_CALLTYPE show (const MessageBoxOptions& options) = delete;
   #endif

    /** Shows a dialog box using the specified options.

        The box will be displayed and placed into a modal state, but this method will return
        immediately, and the callback will be invoked later when the user dismisses the box.

        @param options   the options to use when creating the dialog.
        @param callback  if this is non-null, the callback will receive a call to its
                         modalStateFinished() when the box is dismissed with the result code of the
                         button that was clicked as its argument.
                         The callback object will be owned and deleted by the system, so make sure
                         that it works safely and doesn't keep any references to objects that might
                         be deleted before it gets called.

        @see MessageBoxOptions
    */
    static void HIGGS_CALLTYPE showForOptionsAsync (const MessageBoxOptions& options,
                                                   ModalComponentManager::Callback* callback);

    /** This function is now called showForOptionsAsync().

        When updating your code, make sure to check that your code correctly handles
        the result codes that are specified in the MessageBoxOptions.
    */
    static void HIGGS_CALLTYPE showAsync (const MessageBoxOptions& options,
                                         ModalComponentManager::Callback* callback) = delete;

    /** Shows a dialog box using the specified options.

        The box will be displayed and placed into a modal state, but this method will return
        immediately, and the callback will be invoked later when the user dismisses the box.

        @param options   the options to use when creating the dialog.
        @param callback  if this is non-null, the callback will be called when the box is
                         dismissed with the result code of the button that was clicked as its argument.

        @see MessageBoxOptions
    */
    static void HIGGS_CALLTYPE showForOptionsAsync (const MessageBoxOptions& options,
                                                   std::function<void (int)> callback);

    /** This function is now called showForOptionsAsync().

        When updating your code, make sure to check that your code correctly handles
        the result codes that are specified in the MessageBoxOptions.
    */
    static void HIGGS_CALLTYPE showAsync (const MessageBoxOptions& options,
                                         std::function<void (int)> callback) = delete;

    /** Shows a dialog box that just has a message and a single 'ok' button to close it.

        The box will be displayed and placed into a modal state, but this method will return
        immediately, and the callback will be invoked later when the user dismisses the box.

        @param iconType     the type of icon to show.
        @param title        the headline to show at the top of the box.
        @param message      a longer, more descriptive message to show underneath the title.
        @param associatedComponent   if this is non-null, it specifies the component that the
                            alert window should be associated with. Depending on the look
                            and feel, this might be used for positioning of the alert window.
        @param callback     if this is non-null, the callback will receive a call to its
                            modalStateFinished() when the box is dismissed. The callback object
                            will be owned and deleted by the system, so make sure that it works
                            safely and doesn't keep any references to objects that might be deleted
                            before it gets called. You can use the ModalCallbackFunction to easily
                            pass in a lambda for this parameter.

        @see ModalCallbackFunction
    */
    static void HIGGS_CALLTYPE showMessageBoxAsync (MessageBoxIconType iconType,
                                                   const String& title,
                                                   const String& message,
                                                   Component* associatedComponent = nullptr,
                                                   ModalComponentManager::Callback* callback = nullptr);

    /** Shows a dialog box with two buttons.

        Ideal for ok/cancel or yes/no choices. The return key can also be used
        to trigger the first button, and the escape key for the second button.

        If the callback parameter is null and modal loops are enabled, the box is shown modally,
        and the method will block until the user has clicked the button (or pressed the escape or
        return keys). If the callback parameter is non-null, the box will be displayed and placed
        into a modal state, but this method will return immediately, and the callback will be invoked
        later when the user dismisses the box.

        @param iconType     the type of icon to show.
        @param title        the headline to show at the top of the box.
        @param message      a longer, more descriptive message to show underneath the title.
        @param associatedComponent   if this is non-null, it specifies the component that the
                            alert window should be associated with. Depending on the look
                            and feel, this might be used for positioning of the alert window.
        @param callback     if this is non-null, the box will be launched asynchronously,
                            returning immediately, and the callback will receive a call to its
                            modalStateFinished() when the box is dismissed, with its parameter
                            being 1 if the ok button was pressed, or 0 for cancel, The callback object
                            will be owned and deleted by the system, so make sure that it works
                            safely and doesn't keep any references to objects that might be deleted
                            before it gets called. You can use the ModalCallbackFunction to easily
                            pass in a lambda for this parameter.
        @returns true if button 1 was clicked, false if it was button 2. If the callback parameter
                 is not null, the method always returns false, and the user's choice is delivered
                 later by the callback.

        @see ModalCallbackFunction
    */
    [[deprecated ("Prefer NativeMessageBox::showScopedAsync()")]]
    static bool HIGGS_CALLTYPE showOkCancelBox (MessageBoxIconType iconType,
                                               const String& title,
                                               const String& message,
                                              #if HIGGS_MODAL_LOOPS_PERMITTED
                                               Component* associatedComponent = nullptr,
                                               ModalComponentManager::Callback* callback = nullptr);
                                              #else
                                               Component* associatedComponent,
                                               ModalComponentManager::Callback* callback);
                                              #endif

    /** Shows a dialog box with three buttons.

        Ideal for yes/no/cancel boxes.

        The escape key can be used to trigger the third button.

        If the callback parameter is null and modal loops are enabled, the box is shown modally,
        and the method will block until the user has clicked the button (or pressed the escape or
        return keys). If the callback parameter is non-null, the box will be displayed and placed
        into a modal state, but this method will return immediately, and the callback will be invoked
        later when the user dismisses the box.

        @param iconType     the type of icon to show.
        @param title        the headline to show at the top of the box.
        @param message      a longer, more descriptive message to show underneath the title.
        @param associatedComponent   if this is non-null, it specifies the component that the
                            alert window should be associated with. Depending on the look
                            and feel, this might be used for positioning of the alert window.
        @param callback     if this is non-null, the box will be launched asynchronously,
                            returning immediately, and the callback will receive a call to its
                            modalStateFinished() when the box is dismissed, with its parameter
                            being 1 if the "yes" button was pressed, 2 for the "no" button, or 0
                            if it was cancelled, The callback object will be owned and deleted by the
                            system, so make sure that it works safely and doesn't keep any references
                            to objects that might be deleted before it gets called. You can use the
                            ModalCallbackFunction to easily pass in a lambda for this parameter.

        @returns If the callback parameter has been set, this returns 0. Otherwise, it returns one
                 of the following values:
                 - 0 if 'cancel' was pressed
                 - 1 if 'yes' was pressed
                 - 2 if 'no' was pressed

        @see ModalCallbackFunction
    */
    [[deprecated ("Prefer NativeMessageBox::showScopedAsync()")]]
    static int HIGGS_CALLTYPE showYesNoCancelBox (MessageBoxIconType iconType,
                                                 const String& title,
                                                 const String& message,
                                                #if HIGGS_MODAL_LOOPS_PERMITTED
                                                 Component* associatedComponent = nullptr,
                                                 ModalComponentManager::Callback* callback = nullptr);
                                                #else
                                                 Component* associatedComponent,
                                                 ModalComponentManager::Callback* callback);
                                                #endif

    /** Shows a dialog box with two buttons.

        Ideal for yes/no boxes.

        The escape key can be used to trigger the no button.

        If the callback parameter is null and modal loops are enabled, the box is shown modally,
        and the method will block until the user has clicked the button (or pressed the escape or
        return keys). If the callback parameter is non-null, the box will be displayed and placed
        into a modal state, but this method will return immediately, and the callback will be invoked
        later when the user dismisses the box.

        @param iconType     the type of icon to show.
        @param title        the headline to show at the top of the box.
        @param message      a longer, more descriptive message to show underneath the title.
        @param associatedComponent   if this is non-null, it specifies the component that the
                            alert window should be associated with. Depending on the look
                            and feel, this might be used for positioning of the alert window.
        @param callback     if this is non-null, the box will be launched asynchronously,
                            returning immediately, and the callback will receive a call to its
                            modalStateFinished() when the box is dismissed, with its parameter
                            being 1 if the "yes" button was pressed or 0 for the "no" button was
                            pressed. The callback object will be owned and deleted by the
                            system, so make sure that it works safely and doesn't keep any references
                            to objects that might be deleted before it gets called. You can use the
                            ModalCallbackFunction to easily pass in a lambda for this parameter.

        @returns If the callback parameter has been set, this returns 0. Otherwise, it returns one
                 of the following values:
                 - 0 if 'no' was pressed
                 - 1 if 'yes' was pressed

        @see ModalCallbackFunction
    */
    [[deprecated ("Prefer NativeMessageBox::showScopedAsync()")]]
    static int HIGGS_CALLTYPE showYesNoBox (MessageBoxIconType iconType,
                                           const String& title,
                                           const String& message,
                                          #if HIGGS_MODAL_LOOPS_PERMITTED
                                           Component* associatedComponent = nullptr,
                                           ModalComponentManager::Callback* callback = nullptr);
                                          #else
                                           Component* associatedComponent,
                                           ModalComponentManager::Callback* callback);
                                          #endif

    /** Shows a dialog box using the specified options.

        The box will be displayed and placed into a modal state, but this method will return
        immediately, and the callback will be invoked later when the user dismisses the box.

        This function is always asynchronous, even if the callback is null.

        @param options   the options to use when creating the dialog.
        @param callback  if this is non-null, the callback will receive a call to its
                         modalStateFinished() when the box is dismissed with the result code of the
                         button that was clicked as its argument.
                         The callback object will be owned and deleted by the system, so make sure
                         that it works safely and doesn't keep any references to objects that might
                         be deleted before it gets called.
        @returns         a ScopedMessageBox instance. The message box will remain visible for no
                         longer than the ScopedMessageBox remains alive.

        @see MessageBoxOptions
    */
    [[nodiscard]] static ScopedMessageBox showScopedAsync (const MessageBoxOptions& options,
                                                           std::function<void (int)> callback);

private:
    NativeMessageBox() = delete;
    HIGGS_DECLARE_NON_COPYABLE (NativeMessageBox)
};

} // namespace higgs
