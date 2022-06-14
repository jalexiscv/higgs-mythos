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
    class MessageBox final : public ScopedMessageBoxInterface
    {
    public:
        explicit MessageBox (const MessageBoxOptions& opts) : options (opts) {}

        void runAsync (std::function<void (std::optional<int>)> recipient) override
        {
            auto* peerToUse = std::invoke ([&]() -> UIViewComponentPeer*
            {
                if (auto* comp = options.getAssociatedComponent())
                    if (auto* peer = comp->getPeer())
                        return static_cast<UIViewComponentPeer*> (peer);

                return iOSGlobals::currentlyFocusedPeer;
            });

            if (peerToUse == nullptr)
            {
                // Since iOS8, alert windows need to be associated with a window, so you need to
                // have at least one window on screen when you use this
                jassertfalse;
                return;
            }

            alert.reset ([[UIAlertController alertControllerWithTitle: higgsStringToNS (options.getTitle())
                                                              message: higgsStringToNS (options.getMessage())
                                                       preferredStyle: UIAlertControllerStyleAlert] retain]);

           #if HIGGS_IOS_API_VERSION_CAN_BE_BUILT (16, 0)
            if (@available (iOS 16, *))
            {
                const auto severity = options.getIconType() == MessageBoxIconType::WarningIcon
                                    ? UIAlertControllerSeverityCritical
                                    : UIAlertControllerSeverityDefault;
                [alert.get() setSeverity: severity];
            }
           #endif

            auto buttonOrder = AlertWindowHelpers::getSuggestedButtonOrder (options);

            // The interface guidelines for iOS say that a two-button alert should have
            // the preferred action on the right, but buttons are presented left-to-right
            if (buttonOrder.size() == 2)
                std::reverse (buttonOrder.begin(), buttonOrder.end());

            const auto& buttons = options.getButtons();

            for (const auto buttonIndex : buttonOrder)
            {
                const auto& button = buttons[(size_t) buttonIndex];
                const auto text = button.getText();

                if (text.isEmpty())
                    continue;

                const auto style = std::invoke ([&]
                {
                    if (buttons.size() == 1)
                        return UIAlertActionStyleCancel;

                    switch (button.getRole())
                    {
                        case MessageBoxButtonRole::normal:
                            return UIAlertActionStyleDefault;

                        case MessageBoxButtonRole::cancel:
                            return UIAlertActionStyleCancel;

                        case MessageBoxButtonRole::destroy:
                            return UIAlertActionStyleDestructive;
                    }

                    return UIAlertActionStyleDefault;
                });

                auto* action = [UIAlertAction actionWithTitle: higgsStringToNS (text)
                                                        style: style
                                                      handler: ^(UIAlertAction*)
                {
                    const auto ind = buttonIndex;
                    const auto opt = options;
                    const auto r = recipient;
                    MessageManager::callAsync ([ind, opt, r]
                    {
                        const auto code = opt.getButtons()[(size_t) ind].getCode();
                        NullCheckedInvocation::invoke (r, code);
                    });
                }];

                [alert.get() addAction: action];

                if (button.isPreferredAction())
                    [alert.get() setPreferredAction: action];
            }

            if ([alert.get() preferredAction] == nullptr)
            {
                const auto positionSelector = buttonOrder.size() == 2
                                            ? @selector (lastObject)
                                            : @selector (firstObject);
                [alert.get() setPreferredAction: [[alert.get() actions] performSelector: positionSelector]];
            }

            [peerToUse->controller presentViewController: alert.get()
                                                animated: YES
                                              completion: nil];
        }

        std::optional<int> runSync() override
        {
            bool gotResult = false;
            std::optional<int> result;

            HIGGS_AUTORELEASEPOOL
            {
                runAsync ([&gotResult, &result] (std::optional<int> r)
                {
                    gotResult = true;
                    result = r;
                });

                while (! gotResult)
                {
                    HIGGS_AUTORELEASEPOOL
                    {
                        [[NSRunLoop mainRunLoop] runUntilDate: [NSDate dateWithTimeIntervalSinceNow: 0.01]];
                    }
                }
            }

            return result;
        }

        void close() override
        {
            if (auto* alertViewController = alert.get())
                [alertViewController dismissViewControllerAnimated: YES completion: nil];
        }

    private:
        const MessageBoxOptions options;
        NSUniquePtr<UIAlertController> alert;
        HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MessageBox)
    };

    return std::make_unique<MessageBox> (options);
}

} // namespace higgs::detail
