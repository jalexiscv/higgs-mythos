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

//==============================================================================
class ButtonAccessibilityHandler  : public AccessibilityHandler
{
public:
    ButtonAccessibilityHandler (Button& buttonToWrap, AccessibilityRole roleIn)
        : AccessibilityHandler (buttonToWrap,
                                isRadioButton (buttonToWrap) ? AccessibilityRole::radioButton : roleIn,
                                getAccessibilityActions (buttonToWrap),
                                getAccessibilityInterfaces (buttonToWrap)),
          button (buttonToWrap)
    {}


    AccessibleState getCurrentState() const override
    {
        auto state = AccessibilityHandler::getCurrentState();

        if (button.isToggleable())
        {
            state = state.withCheckable();

            if (button.getToggleState())
                state = state.withChecked();
        }

        return state;
    }


    String getTitle() const override
    {
        auto title = AccessibilityHandler::getTitle();

        if (title.isEmpty())
            return button.getButtonText();

        return title;
    }

    String getHelp() const override
    {
        return button.getTooltip();
    }


private:
    class ButtonValueInterface  : public AccessibilityTextValueInterface
    {
    public:
        explicit ButtonValueInterface (Button& buttonToWrap)
            : button (buttonToWrap)
        {
        }

        bool isReadOnly() const override                 { return true; }
        String getCurrentValueAsString() const override  { return button.getToggleState() ? "On" : "Off"; }
        void setValueAsString (const String&) override   {}

    private:
        Button& button;

        //==============================================================================
        HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ButtonValueInterface)
    };

    static bool isRadioButton (const Button& button) noexcept
    {
        return button.getRadioGroupId() != 0;
    }

    static AccessibilityActions getAccessibilityActions (Button& button)
    {
        auto actions = AccessibilityActions().addAction (AccessibilityActionType::press,
                                                         [&button] { button.triggerClick(); });

        if (button.isToggleable())
        {
            actions = actions.addAction (AccessibilityActionType::toggle,
                                         [&button]
                                         {
                                             if (! button.isEnabled())
                                                 return;

                                             button.setToggleState (! button.getToggleState(), sendNotification);
                                         });
        }

        return actions;
    }

    static Interfaces getAccessibilityInterfaces (Button& button)
    {
        if (button.isToggleable())
            return { std::make_unique<ButtonValueInterface> (button) };

        return {};
    }

    Button& button;

    //==============================================================================
    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ButtonAccessibilityHandler)
};

} // namespace higgs::detail
