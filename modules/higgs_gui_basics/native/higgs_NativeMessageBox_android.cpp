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

namespace higgs::detail
{

std::unique_ptr<ScopedMessageBoxInterface> ScopedMessageBoxInterface::create (const MessageBoxOptions& options)
{
    class AndroidMessageBox final : public ScopedMessageBoxInterface
    {
    public:
        explicit AndroidMessageBox (const MessageBoxOptions& o) : opts (o) {}

        void runAsync (std::function<void (std::optional<int>)> recipient) override
        {
            const auto makeDialogListener = [&recipient] (std::optional<int> result)
            {
                return new DialogListener ([recipient, result]
                {
                    recipient (result);
                });
            };

            auto* env = getEnv();

            LocalRef<jobject> builder (env->NewObject (AndroidAlertDialogBuilder, AndroidAlertDialogBuilder.construct, getMainActivity().get()));

            const auto setText = [&] (auto method, const String& text)
            {
                builder = LocalRef<jobject> (env->CallObjectMethod (builder, method, javaString (text).get()));
            };

            setText (AndroidAlertDialogBuilder.setTitle,   opts.getTitle());
            setText (AndroidAlertDialogBuilder.setMessage, opts.getMessage());

            const auto& buttons = opts.getButtons();
            const auto cancelIter = buttons.size() == 1
                                  ? buttons.begin()
                                  : std::find_if (buttons.begin(),
                                                  buttons.end(),
                                                  [] (const auto& b) { return b.getRole() == MessageBoxButtonRole::cancel; });

            jboolean cancelable = cancelIter != buttons.end() || buttons.empty();
            builder = LocalRef<jobject> (env->CallObjectMethod (builder,
                                                                AndroidAlertDialogBuilder.setCancelable,
                                                                cancelable));

            builder = LocalRef<jobject> (env->CallObjectMethod (builder,
                                                                AndroidAlertDialogBuilder.setOnCancelListener,
                                                                CreateJavaInterface (makeDialogListener (cancelIter != buttons.end() ? std::make_optional (cancelIter->getCode()) : std::nullopt),
                                                                                     "android/content/DialogInterface$OnCancelListener").get()));

            const jmethodID buttonTextSetters[] { AndroidAlertDialogBuilder.setPositiveButton,
                                                  AndroidAlertDialogBuilder.setNegativeButton,
                                                  AndroidAlertDialogBuilder.setNeutralButton };

            const auto buttonOrder = AlertWindowHelpers::getSuggestedButtonOrder (opts);

            for (const auto [index, method] : enumerate (buttonTextSetters, size_t{}))
            {
                if (buttonOrder.size() <= index)
                    break;

                const auto buttonIndex = buttonOrder[index];
                const auto& button = buttons[(size_t) buttonIndex];

                if (button.getText().isEmpty())
                    continue;

                builder = LocalRef<jobject> (env->CallObjectMethod (builder,
                                                                    method,
                                                                    javaString (button.getText()).get(),
                                                                    CreateJavaInterface (makeDialogListener (button.getCode()),
                                                                                         "android/content/DialogInterface$OnClickListener").get()));
            }

            dialog = GlobalRef (LocalRef<jobject> (env->CallObjectMethod (builder, AndroidAlertDialogBuilder.create)));

            LocalRef<jobject> window (env->CallObjectMethod (dialog, AndroidDialog.getWindow));

            if (Desktop::getInstance().getKioskModeComponent() != nullptr)
            {
                env->CallVoidMethod (window, AndroidWindow.setFlags, FLAG_NOT_FOCUSABLE, FLAG_NOT_FOCUSABLE);
                LocalRef<jobject> decorView (env->CallObjectMethod (window, AndroidWindow.getDecorView));
                env->CallVoidMethod (decorView, AndroidView.setSystemUiVisibility, fullScreenFlags);
            }

            env->CallVoidMethod (dialog, AndroidDialog.show);

            if (Desktop::getInstance().getKioskModeComponent() != nullptr)
                env->CallVoidMethod (window, AndroidWindow.clearFlags, FLAG_NOT_FOCUSABLE);
        }

        std::optional<int> runSync() override
        {
            // Not implemented on this platform.
            jassertfalse;
            return {};
        }

        void close() override
        {
            if (dialog != nullptr)
                getEnv()->CallVoidMethod (dialog, AndroidDialogInterface.dismiss);
        }

    private:
        const MessageBoxOptions opts;
        GlobalRef dialog;
    };

    return std::make_unique<AndroidMessageBox> (options);
}

} // namespace higgs::detail
