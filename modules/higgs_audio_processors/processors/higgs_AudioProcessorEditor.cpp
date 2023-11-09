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

AudioProcessorEditor::AudioProcessorEditor (AudioProcessor& p) noexcept  : processor (p)
{
    initialise();
}

AudioProcessorEditor::AudioProcessorEditor (AudioProcessor* p) noexcept  : processor (*p)
{
    // the filter must be valid
    jassert (p != nullptr);
    initialise();
}

AudioProcessorEditor::~AudioProcessorEditor()
{
    // if this fails, then the wrapper hasn't called editorBeingDeleted() on the
    // filter for some reason
    jassert (processor.getActiveEditor() != this);
    removeComponentListener (resizeListener.get());
}

void AudioProcessorEditor::setControlHighlight (ParameterControlHighlightInfo) {}
int AudioProcessorEditor::getControlParameterIndex (Component&)                { return -1; }

bool AudioProcessorEditor::supportsHostMIDIControllerPresence (bool)           { return true; }
void AudioProcessorEditor::hostMIDIControllerIsAvailable (bool)                {}

void AudioProcessorEditor::initialise()
{
    attachConstrainer (&defaultConstrainer);
    resizeListener.reset (new AudioProcessorEditorListener (*this));
    addComponentListener (resizeListener.get());
}

//==============================================================================
void AudioProcessorEditor::setResizable (bool allowHostToResize, bool useBottomRightCornerResizer)
{
    resizableByHost = allowHostToResize;

    const auto hasResizableCorner = (resizableCorner.get() != nullptr);

    if (useBottomRightCornerResizer != hasResizableCorner)
    {
        if (useBottomRightCornerResizer)
            attachResizableCornerComponent();
        else
            resizableCorner = nullptr;
    }
}

void AudioProcessorEditor::setResizeLimits (int newMinimumWidth,
                                            int newMinimumHeight,
                                            int newMaximumWidth,
                                            int newMaximumHeight) noexcept
{
    if (constrainer != nullptr && constrainer != &defaultConstrainer)
    {
        // if you've set up a custom constrainer then these settings won't have any effect
        jassertfalse;
        return;
    }

    resizableByHost = (newMinimumWidth != newMaximumWidth || newMinimumHeight != newMaximumHeight);

    defaultConstrainer.setSizeLimits (newMinimumWidth, newMinimumHeight,
                                      newMaximumWidth, newMaximumHeight);

    if (constrainer == nullptr)
        setConstrainer (&defaultConstrainer);

    if (resizableCorner != nullptr)
        attachResizableCornerComponent();

    setBoundsConstrained (getBounds());
}

void AudioProcessorEditor::setConstrainer (ComponentBoundsConstrainer* newConstrainer)
{
    if (constrainer != newConstrainer)
    {
        attachConstrainer (newConstrainer);

        if (constrainer != nullptr)
            resizableByHost = (newConstrainer->getMinimumWidth() != newConstrainer->getMaximumWidth()
                                || newConstrainer->getMinimumHeight() != newConstrainer->getMaximumHeight());

        if (resizableCorner != nullptr)
            attachResizableCornerComponent();
    }
}

void AudioProcessorEditor::attachConstrainer (ComponentBoundsConstrainer* newConstrainer)
{
    if (constrainer != newConstrainer)
    {
        constrainer = newConstrainer;
        updatePeer();
    }
}

void AudioProcessorEditor::attachResizableCornerComponent()
{
    resizableCorner = std::make_unique<ResizableCornerComponent> (this, constrainer);
    Component::addChildComponent (resizableCorner.get());
    resizableCorner->setAlwaysOnTop (true);
    editorResized (true);
}

void AudioProcessorEditor::setBoundsConstrained (Rectangle<int> newBounds)
{
    if (constrainer == nullptr)
    {
        setBounds (newBounds);
        return;
    }

    auto currentBounds = getBounds();

    constrainer->setBoundsForComponent (this,
                                        newBounds,
                                        newBounds.getY() != currentBounds.getY() && newBounds.getBottom() == currentBounds.getBottom(),
                                        newBounds.getX() != currentBounds.getX() && newBounds.getRight()  == currentBounds.getRight(),
                                        newBounds.getY() == currentBounds.getY() && newBounds.getBottom() != currentBounds.getBottom(),
                                        newBounds.getX() == currentBounds.getX() && newBounds.getRight()  != currentBounds.getRight());
}

AudioProcessorEditorARAExtension* AudioProcessorEditor::getARAClientExtensions()
{
   #if HiggsPlugin_Enable_ARA
    if (auto* extensions = dynamic_cast<AudioProcessorEditorARAExtension*> (this))
    {
        //  To silence this jassert there are two options:
        //
        //  1. - Override AudioProcessorEditor::getARAClientExtensions() and
        //       return the "this" pointer.
        //
        //     - This option has the advantage of being quick and easy,
        //       and avoids the above dynamic_cast.
        //
        //  2. - Create a new object that inherits from AudioProcessorEditorARAClientExtension.
        //
        //     - Port your existing functionality from the AudioProcessorEditor
        //       to the new object.
        //
        //     - Return a pointer to the object in AudioProcessorEditor::getARAClientExtensions().
        //
        //     - This option has the advantage of allowing you to break
        //       up your AudioProcessorEditor into smaller composable objects.
        jassertfalse;
        return extensions;
    }
   #endif

    return nullptr;
}

void AudioProcessorEditor::editorResized (bool wasResized)
{
    // The host needs to be able to rescale the plug-in editor and applying your own transform will
    // obliterate it! If you want to scale the whole of your UI use Desktop::setGlobalScaleFactor(),
    // or, for applying other transforms, consider putting the component you want to transform
    // in a child of the editor and transform that instead.
    jassert (getTransform() == hostScaleTransform);

    if (wasResized)
    {
        bool resizerHidden = false;

        if (auto* peer = getPeer())
            resizerHidden = peer->isFullScreen() || peer->isKioskMode();

        if (resizableCorner != nullptr)
        {
            resizableCorner->setVisible (! resizerHidden);

            const int resizerSize = 18;
            resizableCorner->setBounds (getWidth() - resizerSize,
                                        getHeight() - resizerSize,
                                        resizerSize, resizerSize);
        }
    }
}

void AudioProcessorEditor::updatePeer()
{
    if (isOnDesktop())
        if (auto* peer = getPeer())
            peer->setConstrainer (constrainer);
}

void AudioProcessorEditor::setScaleFactor (float newScale)
{
    hostScaleTransform = AffineTransform::scale (newScale);
    setTransform (hostScaleTransform);

    editorResized (true);
}

//==============================================================================
typedef ComponentPeer* (*createUnityPeerFunctionType) (Component&);
createUnityPeerFunctionType higgs_createUnityPeerFn = nullptr;

ComponentPeer* AudioProcessorEditor::createNewPeer (const DesktopWindowOptions& options)
{
    if (higgs_createUnityPeerFn != nullptr)
        return higgs_createUnityPeerFn (*this);

    return Component::createNewPeer (options);
}

bool AudioProcessorEditor::wantsLayerBackedView() const
{
   #if HIGGS_MODULE_AVAILABLE_higgs_opengl && HIGGS_MAC
    if (@available (macOS 10.14, *))
        return true;

    return false;
   #else
    return true;
   #endif
}

bool AudioProcessorEditor::usesWindowsMultiTouch() const
{
    return false;
}

} // namespace higgs
