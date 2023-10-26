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

#if HIGGS_INTERNAL_HAS_AU

#include <higgs_audio_processors_headless/format_types/higgs_AudioUnitPluginFormatImpl.h>

namespace higgs
{

class AudioUnitPluginWindowCocoa final : public AudioProcessorEditor
{
    // Audio Unit plugins expect hosts to listen to their view bounds, and to resize
    // the plugin window/view appropriately.
   #if HIGGS_IOS
    using HIGGS_IOS_MAC_VIEW = UIView;
    using ViewComponentBaseClass = UIViewComponent;
   #else
    using HIGGS_IOS_MAC_VIEW = NSView;
    using ViewComponentBaseClass = NSViewComponent;
   #endif

    struct AutoResizingNSViewComponent final : public ViewComponentBaseClass,
                                               private AsyncUpdater
    {
        void childBoundsChanged (Component*) override  { triggerAsyncUpdate(); }
        void handleAsyncUpdate() override              { resizeToFitView(); }
    };

public:
    AudioUnitPluginWindowCocoa (AudioUnitPluginInstanceHeadless& p, bool createGenericViewIfNeeded)
        : AudioProcessorEditor (&p),
          plugin (p)
    {
        addAndMakeVisible (wrapper);

        setOpaque (true);
        setVisible (true);
        setSize (100, 100);

        createView (createGenericViewIfNeeded);
    }

    ~AudioUnitPluginWindowCocoa() override
    {
        if (wrapper.getView() != nil)
        {
            wrapper.setVisible (false);
            removeChildComponent (&wrapper);
            wrapper.setView (nil);
            plugin.editorBeingDeleted (this);
        }
    }

    void embedViewController (HIGGS_IOS_MAC_VIEW* pluginView, [[maybe_unused]] const CGSize& size)
    {
        wrapper.setView (pluginView);
        waitingForViewCallback = false;

      #if HIGGS_MAC
        if (pluginView != nil)
            wrapper.resizeToFitView();
      #else
        [pluginView setBounds: CGRectMake (0.f, 0.f, static_cast<int> (size.width), static_cast<int> (size.height))];
        wrapper.setSize (static_cast<int> (size.width), static_cast<int> (size.height));
      #endif
    }

    bool isValid() const        { return wrapper.getView() != nil || waitingForViewCallback; }

    void paint (Graphics& g) override
    {
        g.fillAll (Colours::white);
    }

    void resized() override
    {
        wrapper.setSize (getWidth(), getHeight());
    }

    void childBoundsChanged (Component*) override
    {
        setSize (wrapper.getWidth(), wrapper.getHeight());
    }

private:
    AudioUnitPluginInstanceHeadless& plugin;
    AutoResizingNSViewComponent wrapper;

    typedef void (^ViewControllerCallbackBlock)(AUViewControllerBase *);

    bool waitingForViewCallback = false;

    bool createView ([[maybe_unused]] bool createGenericViewIfNeeded)
    {
        HIGGS_IOS_MAC_VIEW* pluginView = nil;
        UInt32 dataSize = 0;
        Boolean isWritable = false;

       #if HIGGS_MAC
        if (AudioUnitGetPropertyInfo (plugin.getAudioUnitHandle(), kAudioUnitProperty_CocoaUI, kAudioUnitScope_Global,
                                      0, &dataSize, &isWritable) == noErr
             && dataSize != 0
             && AudioUnitGetPropertyInfo (plugin.getAudioUnitHandle(), kAudioUnitProperty_CocoaUI, kAudioUnitScope_Global,
                                          0, &dataSize, &isWritable) == noErr)
        {
            HeapBlock<AudioUnitCocoaViewInfo> info;
            info.calloc (dataSize, 1);

            if (AudioUnitGetProperty (plugin.getAudioUnitHandle(), kAudioUnitProperty_CocoaUI, kAudioUnitScope_Global,
                                      0, info, &dataSize) == noErr)
            {
                NSString* viewClassName = (NSString*) (info->mCocoaAUViewClass[0]);
                CFUniquePtr<CFStringRef> path (CFURLCopyPath (info->mCocoaAUViewBundleLocation));
                NSString* unescapedPath = (NSString*) CFURLCreateStringByReplacingPercentEscapes (nullptr, path.get(), CFSTR (""));
                NSBundle* viewBundle = [NSBundle bundleWithPath: [unescapedPath autorelease]];
                Class viewClass = [viewBundle classNamed: viewClassName];

                if ([viewClass conformsToProtocol: @protocol (AUCocoaUIBase)]
                     && [viewClass instancesRespondToSelector: @selector (interfaceVersion)]
                     && [viewClass instancesRespondToSelector: @selector (uiViewForAudioUnit: withSize:)])
                {
                    id factory = [[[viewClass alloc] init] autorelease];
                    pluginView = [factory uiViewForAudioUnit: plugin.getAudioUnitHandle()
                                                    withSize: NSMakeSize (getWidth(), getHeight())];
                }

                for (int i = (dataSize - sizeof (CFURLRef)) / sizeof (CFStringRef); --i >= 0;)
                    CFRelease (info->mCocoaAUViewClass[i]);

                CFRelease (info->mCocoaAUViewBundleLocation);
            }
        }
       #endif

        dataSize = 0;
        isWritable = false;

        if (AudioUnitGetPropertyInfo (plugin.getAudioUnitHandle(), kAudioUnitProperty_RequestViewController, kAudioUnitScope_Global,
                                          0, &dataSize, &isWritable) == noErr
                && dataSize == sizeof (ViewControllerCallbackBlock))
        {
            waitingForViewCallback = true;
            auto callback = ^(AUViewControllerBase* controller) { this->requestViewControllerCallback (controller); };

            if (noErr == AudioUnitSetProperty (plugin.getAudioUnitHandle(), kAudioUnitProperty_RequestViewController, kAudioUnitScope_Global, 0, &callback, dataSize))
                return true;

            waitingForViewCallback = false;
        }

       #if HIGGS_MAC
        if (createGenericViewIfNeeded && (pluginView == nil))
        {
            {
                // This forces CoreAudio.component to be loaded, otherwise the AUGenericView will assert
                AudioComponentDescription desc;
                String name, version, manufacturer;
                AudioUnitFormatHelpers::getComponentDescFromIdentifier ("AudioUnit:Output/auou,genr,appl",
                                                                        desc, name, version, manufacturer);
            }

            pluginView = [[AUGenericView alloc] initWithAudioUnit: plugin.getAudioUnitHandle()];
        }
       #endif

        wrapper.setView (pluginView);

        if (pluginView != nil)
            wrapper.resizeToFitView();

        return pluginView != nil;
    }

    void requestViewControllerCallback (AUViewControllerBase* controller)
    {
        const auto viewSize = [&controller]
        {
            auto size = CGSizeZero;

            size = [controller preferredContentSize];

            if (approximatelyEqual (size.width, 0.0) || approximatelyEqual (size.height, 0.0))
                size = controller.view.frame.size;

            return CGSizeMake (jmax ((CGFloat) 20.0f, size.width),
                               jmax ((CGFloat) 20.0f, size.height));
        }();

        if (! MessageManager::getInstance()->isThisTheMessageThread())
        {
            struct AsyncViewControllerCallback final : public CallbackMessage
            {
                AudioUnitPluginWindowCocoa* owner;
                HIGGS_IOS_MAC_VIEW* controllerView;
                CGSize size;

                AsyncViewControllerCallback (AudioUnitPluginWindowCocoa* plugInWindow, HIGGS_IOS_MAC_VIEW* inView,
                                             const CGSize& preferredSize)
                    : owner (plugInWindow), controllerView ([inView retain]), size (preferredSize)
                {}

                void messageCallback() override
                {
                    owner->embedViewController (controllerView, size);
                    [controllerView release];
                }
            };

            (new AsyncViewControllerCallback (this, [controller view], viewSize))->post();
        }
        else
        {
            embedViewController ([controller view], viewSize);
        }
    }
};

class AudioUnitPluginInstance final : public AudioUnitPluginInstanceHeadless
{
public:
    using AudioUnitPluginInstanceHeadless::AudioUnitPluginInstanceHeadless;

    bool hasEditor() const override
    {
       #if HIGGS_MAC
        return true;
       #else
        UInt32 dataSize;
        Boolean isWritable;

        return (AudioUnitGetPropertyInfo (getAudioUnitHandle(), kAudioUnitProperty_RequestViewController,
                                          kAudioUnitScope_Global, 0, &dataSize, &isWritable) == noErr
                && dataSize == sizeof (uintptr_t) && isWritable != 0);
       #endif
    }

    AudioProcessorEditor* createEditor() override
    {
        std::unique_ptr<AudioProcessorEditor> w (new AudioUnitPluginWindowCocoa (*this, false));

        if (! static_cast<AudioUnitPluginWindowCocoa*> (w.get())->isValid())
            w.reset (new AudioUnitPluginWindowCocoa (*this, true)); // use AUGenericView as a fallback

        return w.release();
    }
};

void AudioUnitPluginFormat::createPluginInstance (const PluginDescription& desc,
                                                  double rate,
                                                  int blockSize,
                                                  PluginCreationCallback callback)
{
    createAudioUnitPluginInstance<AudioUnitPluginInstance> (*this, desc, rate, blockSize, callback);
}

} // namespace higgs

#endif
