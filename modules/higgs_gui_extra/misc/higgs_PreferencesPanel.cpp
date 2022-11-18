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

PreferencesPanel::PreferencesPanel()
    : buttonSize (70)
{
}

PreferencesPanel::~PreferencesPanel()
{
}

int PreferencesPanel::getButtonSize() const noexcept
{
    return buttonSize;
}

void PreferencesPanel::setButtonSize (int newSize)
{
    buttonSize = newSize;
    resized();
}

//==============================================================================
void PreferencesPanel::addSettingsPage (const String& title,
                                        const Drawable* icon,
                                        const Drawable* overIcon,
                                        const Drawable* downIcon)
{
    auto* button = new DrawableButton (title, DrawableButton::ImageAboveTextLabel);
    buttons.add (button);

    button->setImages (icon, overIcon, downIcon);
    button->setRadioGroupId (1);
    button->onClick = [this] { clickedPage(); };
    button->setClickingTogglesState (true);
    button->setWantsKeyboardFocus (false);
    addAndMakeVisible (button);

    resized();

    if (currentPage == nullptr)
        setCurrentPage (title);
}

void PreferencesPanel::addSettingsPage (const String& title, const void* imageData, int imageDataSize)
{
    DrawableImage icon, iconOver, iconDown;
    icon.setImage (ImageCache::getFromMemory (imageData, imageDataSize));

    iconOver.setImage (ImageCache::getFromMemory (imageData, imageDataSize));
    iconOver.setOverlayColour (Colours::black.withAlpha (0.12f));

    iconDown.setImage (ImageCache::getFromMemory (imageData, imageDataSize));
    iconDown.setOverlayColour (Colours::black.withAlpha (0.25f));

    addSettingsPage (title, &icon, &iconOver, &iconDown);
}

//==============================================================================
void PreferencesPanel::showInDialogBox (const String& dialogTitle, int dialogWidth, int dialogHeight, Colour backgroundColour)
{
    setSize (dialogWidth, dialogHeight);

    DialogWindow::LaunchOptions o;
    o.content.setNonOwned (this);
    o.dialogTitle                   = dialogTitle;
    o.dialogBackgroundColour        = backgroundColour;
    o.escapeKeyTriggersCloseButton  = false;
    o.useNativeTitleBar             = false;
    o.resizable                     = false;

    o.launchAsync();
}

//==============================================================================
void PreferencesPanel::resized()
{
    for (int i = 0; i < buttons.size(); ++i)
        buttons.getUnchecked (i)->setBounds (i * buttonSize, 0, buttonSize, buttonSize);

    if (currentPage != nullptr)
        currentPage->setBounds (getLocalBounds().withTop (buttonSize + 5));
}

void PreferencesPanel::paint (Graphics& g)
{
    g.setColour (Colours::grey);
    g.fillRect (0, buttonSize + 2, getWidth(), 1);
}

void PreferencesPanel::setCurrentPage (const String& pageName)
{
    if (currentPageName != pageName)
    {
        currentPageName = pageName;

        currentPage.reset();
        currentPage.reset (createComponentForPage (pageName));

        if (currentPage != nullptr)
        {
            addAndMakeVisible (currentPage.get());
            currentPage->toBack();
            resized();
        }

        for (auto* b : buttons)
        {
            if (b->getName() == pageName)
            {
                b->setToggleState (true, dontSendNotification);
                break;
            }
        }
    }
}

void PreferencesPanel::clickedPage()
{
    for (auto* b : buttons)
    {
        if (b->getToggleState())
        {
            setCurrentPage (b->getName());
            break;
        }
    }
}

} // namespace higgs
