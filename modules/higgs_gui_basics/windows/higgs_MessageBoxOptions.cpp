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

MessageBoxOptions MessageBoxOptions::withButton (MessageBoxButtonOptions opt) const
{
    auto copy = *this;
    copy.buttons.push_back (std::move (opt));

    // Only a maximum of one action may be preferred!
    // Check your MessageBoxOptions and make sure that you haven't called
    // "setPreferredAction (true)" more than once.
    jassert (std::count_if (copy.buttons.begin(),
                            copy.buttons.end(),
                            [] (const auto& b) { return b.isPreferredAction(); }) <= 1);

    // Only a maximum of one action may have the cancel role!
    // Check your MessageBoxOptions and make sure that you haven't called
    // "setRole (MessageBoxButtonRole::cancel)" more than once.
    jassert (std::count_if (copy.buttons.begin(),
                            copy.buttons.end(),
                            [] (const auto& b) { return b.getRole() == MessageBoxButtonRole::cancel; }) <= 1);

    return copy;
}

MessageBoxOptions MessageBoxOptions::makeOptionsOk (MessageBoxIconType iconType,
                                                    const String& title,
                                                    const String& message,
                                                    const String& buttonText,
                                                    Component* associatedComponent)
{
    return MessageBoxOptions()
        .withIconType (iconType)
        .withTitle (title)
        .withMessage (message)
        .withButton (MessageBoxButtonOptions { buttonText.isEmpty() ? TRANS ("OK") : buttonText, 0 }.setPreferredAction()
                                                                                                    .setRole (MessageBoxButtonRole::cancel))
        .withAssociatedComponent (associatedComponent);
}

MessageBoxOptions MessageBoxOptions::makeOptionsOkCancel (MessageBoxIconType iconType,
                                                          const String& title,
                                                          const String& message,
                                                          const String& button1Text,
                                                          const String& button2Text,
                                                          Component* associatedComponent)
{
    return MessageBoxOptions()
        .withIconType (iconType)
        .withTitle (title)
        .withMessage (message)
        .withButton (MessageBoxButtonOptions { button1Text.isEmpty() ? TRANS ("OK") : button1Text, 1 }.setPreferredAction())
        .withButton (MessageBoxButtonOptions { button2Text.isEmpty() ? TRANS ("Cancel") : button2Text, 0 }.setRole (MessageBoxButtonRole::cancel))
        .withAssociatedComponent (associatedComponent);
}

MessageBoxOptions MessageBoxOptions::makeOptionsYesNo (MessageBoxIconType iconType,
                                                       const String& title,
                                                       const String& message,
                                                       const String& button1Text,
                                                       const String& button2Text,
                                                       Component* associatedComponent)
{
    return MessageBoxOptions()
        .withIconType (iconType)
        .withTitle (title)
        .withMessage (message)
        .withButton (button1Text.isEmpty() ? TRANS ("Yes") : button1Text, 1)
        .withButton (button2Text.isEmpty() ? TRANS ("No") : button2Text, 0)
        .withAssociatedComponent (associatedComponent);
}

MessageBoxOptions MessageBoxOptions::makeOptionsYesNoCancel (MessageBoxIconType iconType,
                                                             const String& title,
                                                             const String& message,
                                                             const String& button1Text,
                                                             const String& button2Text,
                                                             const String& button3Text,
                                                             Component* associatedComponent)
{
    return MessageBoxOptions()
        .withIconType (iconType)
        .withTitle (title)
        .withMessage (message)
        .withButton (button1Text.isEmpty() ? TRANS ("Yes") : button1Text, 1)
        .withButton (button2Text.isEmpty() ? TRANS ("No") : button2Text, 2)
        .withButton (MessageBoxButtonOptions { button3Text.isEmpty() ? TRANS ("Cancel") : button3Text, 0 }.setRole (MessageBoxButtonRole::cancel))
        .withAssociatedComponent (associatedComponent);
}

} // namespace higgs
