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
    class OSXMessageBox final : public ScopedMessageBoxInterface
    {
    public:
        explicit OSXMessageBox (const MessageBoxOptions& opts)
            : options (opts) {}

        void runAsync (std::function<void (std::optional<int>)> recipient) override
        {
            const auto resultCodes = makeAlert();

            const auto onDone = [resultCodes, recipient, opt = options] (NSModalResponse result)
            {
                const auto index = convertResult (result);
                const auto code = index.has_value() ? std::make_optional (resultCodes[*index]) : std::nullopt;

                recipient (code);
            };

            if (auto* comp = options.getAssociatedComponent())
            {
                if (auto* peer = comp->getPeer())
                {
                    if (auto* view = static_cast<NSView*> (peer->getNativeHandle()))
                    {
                        if (auto* window = [view window])
                        {
                            [alertWindow.get() beginSheetModalForWindow: window completionHandler: ^(NSModalResponse result)
                            {
                                onDone (result);
                            }];

                            return;
                        }
                    }
                }
            }

            const NSUniquePtr<NSAlert> alert { [alertWindow.get() retain] };
            onDone ([alert.get() runModal]);
        }

        std::optional<int> runSync() override
        {
            const auto resultCodes = makeAlert();
            const auto index = convertResult ([alertWindow.get() runModal]);

            return index.has_value() ? std::make_optional (resultCodes[*index]) : std::nullopt;
        }

        void close() override
        {
            if (alertWindow == nullptr)
                return;

            auto* window = [alertWindow.get() window];

            if ([NSApp modalWindow] == window)
                [NSApp stopModalWithCode: NSModalResponseAbort];
            else if (auto* parent = [window sheetParent])
                [parent endSheet: window returnCode: NSModalResponseAbort];

            [window close];
        }

    private:
        static std::optional<int> convertResult (NSModalResponse response)
        {
            switch (response)
            {
                case NSAlertFirstButtonReturn:   return 0;
                case NSAlertSecondButtonReturn:  return 1;
                case NSAlertThirdButtonReturn:   return 2;
                default:                         break;
            }

            return {};
        }

        Array<int> makeAlert()
        {
            NSAlert* alert = [[NSAlert alloc] init];

            [alert setMessageText:     higgsStringToNS (options.getTitle())];
            [alert setInformativeText: higgsStringToNS (options.getMessage())];

            [alert setAlertStyle: options.getIconType() == MessageBoxIconType::WarningIcon ? NSAlertStyleCritical
                                                                                           : NSAlertStyleInformational];

            const auto buttonOrder = AlertWindowHelpers::getSuggestedButtonOrder (options);

            const String defaultButtonStrings[] { TRANS ("OK"), String{}, String{} };
            Array<int> resultCodes;
            NSButton* cancelButton = nullptr;

            for (const auto [index, fallback] : enumerate (defaultButtonStrings, size_t{}))
            {
                const auto* button = index < buttonOrder.size()
                                   ? &options.getButtons()[(size_t) buttonOrder[index]]
                                   : nullptr;
                const auto text = button != nullptr ? button->getText() : fallback;

                if (text.isEmpty())
                    continue;

                [alert addButtonWithTitle: higgsStringToNS (text)];
                resultCodes.add (button != nullptr ? button->getCode() : resultCodes.size());

                if (button == nullptr)
                    continue;

                if (button->getRole() == MessageBoxButtonRole::destroy)
                {
                    if (@available (macOS 11, *))
                    {
                        [[[alert buttons] lastObject] setHasDestructiveAction: true];
                    }
                }
                else if (button->getRole() == MessageBoxButtonRole::cancel)
                {
                    cancelButton = [[alert buttons] lastObject];
                }
            }

            if (cancelButton != nullptr && cancelButton != [[alert buttons] firstObject])
                [cancelButton setKeyEquivalent: @"\x1b"]; // The escape code from Events.h

            alertWindow.reset (alert);

            return resultCodes;
        }

        NSUniquePtr<NSAlert> alertWindow;
        MessageBoxOptions options;
        std::unique_ptr<ModalComponentManager::Callback> callback;

        HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OSXMessageBox)
    };

    return std::make_unique<OSXMessageBox> (options);
}

} // namespace higgs::detail
