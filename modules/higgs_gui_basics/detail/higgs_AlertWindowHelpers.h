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

struct AlertWindowHelpers
{
    AlertWindowHelpers() = delete;

    static std::unique_ptr<ScopedMessageBoxInterface> create (const MessageBoxOptions& opts)
    {
        class AlertWindowImpl : public ScopedMessageBoxInterface
        {
        public:
            explicit AlertWindowImpl (const MessageBoxOptions& opts) : options (opts) {}

            void runAsync (std::function<void (std::optional<int>)> recipient) override
            {
                if (auto* comp = setUpAlert())
                    comp->enterModalState (true, ModalCallbackFunction::create (std::move (recipient)), true);
                else
                    NullCheckedInvocation::invoke (recipient, 0);
            }

            std::optional<int> runSync() override
            {
               #if HIGGS_MODAL_LOOPS_PERMITTED
                if (auto comp = rawToUniquePtr (setUpAlert()))
                    return comp->runModalLoop();
               #endif

                jassertfalse;
                return 0;
            }

            void close() override
            {
                if (alert != nullptr)
                    if (alert->isCurrentlyModal())
                        alert->exitModalState();

                alert = nullptr;
            }

        private:
            Component* setUpAlert()
            {
                auto* component = options.getAssociatedComponent();

                auto& lf = component != nullptr ? component->getLookAndFeel()
                                                : LookAndFeel::getDefaultLookAndFeel();

                alert = lf.createAlertWindowForOptions (options).release();

                if (alert == nullptr)
                {
                    // You have to return an alert box!
                    jassertfalse;
                    return nullptr;
                }

                if (auto* parent = options.getParentComponent())
                {
                    parent->addAndMakeVisible (alert);

                    if (options.getAssociatedComponent() == nullptr)
                        alert->setCentrePosition (parent->getLocalBounds().getCentre());
                }

                alert->setAlwaysOnTop (WindowUtils::areThereAnyAlwaysOnTopWindows());

                return alert;
            }

            const MessageBoxOptions options;
            Component::SafePointer<AlertWindow> alert;

            HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AlertWindowImpl)
        };

        return std::make_unique<AlertWindowImpl> (opts);
    }

    static std::vector<int> getSuggestedButtonOrder (const MessageBoxOptions& opt)
    {
        const auto& buttons = opt.getButtons();

        std::vector<int> buttonOrder;

        for (int i = 0; i < opt.getNumButtons(); ++i)
            buttonOrder.push_back (i);

        for (auto [index, button] : enumerate (buttons, int{}))
        {
            if (button.isPreferredAction())
            {
                buttonOrder.erase (std::find (buttonOrder.begin(), buttonOrder.end(), index));
                buttonOrder.insert (buttonOrder.begin(), index);
            }
            else if (button.getRole() == MessageBoxButtonRole::cancel)
            {
                buttonOrder.erase (std::find (buttonOrder.begin(), buttonOrder.end(), index));
                buttonOrder.insert (buttonOrder.end(), index);
            }
        }

        return buttonOrder;
    }
};

} // namespace higgs::detail
