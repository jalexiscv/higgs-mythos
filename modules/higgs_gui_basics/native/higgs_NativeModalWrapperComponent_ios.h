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

/**
    Sets up a native control to be hosted on top of a HIGGS component.
*/
class NativeModalWrapperComponent : public Component
{
public:
    void parentHierarchyChanged() final
    {
        auto* newPeer = dynamic_cast<UIViewComponentPeer*> (getPeer());

        if (std::exchange (peer, newPeer) == newPeer)
            return;

        if (peer == nullptr)
            return;

        if (isIPad())
        {
            getViewController().preferredContentSize = peer->view.frame.size;

            if (auto* popoverController = getViewController().popoverPresentationController)
            {
                popoverController.sourceView = peer->view;
                popoverController.sourceRect = CGRectMake (0.0f, (float) getHeight() - 10.0f, (float) getWidth(), 10.0f);
                popoverController.canOverlapSourceViewRect = YES;
                popoverController.delegate = popoverDelegate.get();
            }
        }

        if (auto* parentController = peer->controller)
            [parentController showViewController: getViewController() sender: parentController];

        peer->toFront (false);
    }

    void displayNativeWindowModally (Component* parent)
    {
        setOpaque (false);

        if (parent != nullptr)
        {
            [getViewController() setModalPresentationStyle: UIModalPresentationPageSheet];

            setBounds (parent->getLocalBounds());

            setAlwaysOnTop (true);
            parent->addAndMakeVisible (this);
        }
        else
        {
            if (SystemStats::isRunningInAppExtensionSandbox())
            {
                // Opening a native top-level window in an AUv3 is not allowed (sandboxing). You need to specify a
                // parent component (for example your editor) to parent the native file chooser window. To do this
                // specify a parent component in the FileChooser's constructor!
                jassertfalse;
                return;
            }

            auto chooserBounds = Desktop::getInstance().getDisplays().getPrimaryDisplay()->userBounds.getLargestIntegerWithin();
            setBounds (chooserBounds);

            setAlwaysOnTop (true);
            setVisible (true);
            addToDesktop (0);
        }
    }

private:
    virtual UIViewController* getViewController() const = 0;

    static bool isIPad()
    {
        return [UIDevice currentDevice].userInterfaceIdiom == UIUserInterfaceIdiomPad;
    }

    struct PopoverDelegateClass : public ObjCClass<NSObject<UIPopoverPresentationControllerDelegate>>
    {
        PopoverDelegateClass()
            : ObjCClass ("PopoverDelegateClass_")
        {
            addMethod (@selector (popoverPresentationController:willRepositionPopoverToRect:inView:), [] (id, SEL, UIPopoverPresentationController*, CGRect* rect, UIView** view)
            {
                const auto viewBounds = (*view).bounds;

                rect->origin.x = 0.f;
                rect->origin.y = viewBounds.size.height - 10.f;
                rect->size.width = viewBounds.size.width;
                rect->size.height = 10.f;
            });

            registerClass();
        }
    };

    UIViewComponentPeer* peer = nullptr;
    NSUniquePtr<NSObject<UIPopoverPresentationControllerDelegate>> popoverDelegate { []
    {
        static PopoverDelegateClass cls;
        return cls.createInstance();
    }() };
};

} // namespace higgs::detail
