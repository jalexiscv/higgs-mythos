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

struct CameraDevice::Pimpl
{
    Pimpl (CameraDevice& ownerToUse, const String& deviceNameToUse, int /*index*/,
           int /*minWidth*/, int /*minHeight*/,
           int /*maxWidth*/, int /*maxHeight*/,
           bool useHighQuality)
        : owner (ownerToUse),
          deviceName (deviceNameToUse)
    {
        imageOutput = []() -> std::unique_ptr<ImageOutputBase>
        {
            if (@available (macOS 10.15, *))
                return std::make_unique<PostCatalinaPhotoOutput>();

           return std::make_unique<PreCatalinaStillImageOutput>();
        }();

        session = [[AVCaptureSession alloc] init];

        session.sessionPreset = useHighQuality ? AVCaptureSessionPresetHigh
                                               : AVCaptureSessionPresetMedium;

        refreshConnections();

        static DelegateClass cls;
        callbackDelegate = (id<AVCaptureFileOutputRecordingDelegate>) [cls.createInstance() init];
        DelegateClass::setOwner (callbackDelegate, this);

        HIGGS_BEGIN_IGNORE_WARNINGS_GCC_LIKE ("-Wundeclared-selector")
        [[NSNotificationCenter defaultCenter] addObserver: callbackDelegate
                                                 selector: @selector (captureSessionRuntimeError:)
                                                     name: AVCaptureSessionRuntimeErrorNotification
                                                   object: session];
        HIGGS_END_IGNORE_WARNINGS_GCC_LIKE
    }

    ~Pimpl()
    {
        [[NSNotificationCenter defaultCenter] removeObserver: callbackDelegate];

        [session stopRunning];
        removeInput();
        removeImageCapture();
        removeMovieCapture();
        [session release];
        [callbackDelegate release];
    }

    //==============================================================================
    bool openedOk() const noexcept       { return openingError.isEmpty(); }

    void startSession()
    {
        if (! [session isRunning])
            [session startRunning];
    }

    void takeStillPicture (std::function<void (const Image&)> pictureTakenCallbackToUse)
    {
        if (pictureTakenCallbackToUse == nullptr)
        {
            jassertfalse;
            return;
        }

        pictureTakenCallback = std::move (pictureTakenCallbackToUse);

        triggerImageCapture();
    }

    void startRecordingToFile (const File& file, int /*quality*/)
    {
        stopRecording();
        refreshIfNeeded();
        firstPresentationTime = Time::getCurrentTime();
        file.deleteFile();

        startSession();
        isRecording = true;
        [fileOutput startRecordingToOutputFileURL: createNSURLFromFile (file)
                                recordingDelegate: callbackDelegate];
    }

    void stopRecording()
    {
        if (isRecording)
        {
            [fileOutput stopRecording];
            isRecording = false;
        }
    }

    Time getTimeOfFirstRecordedFrame() const
    {
        return firstPresentationTime;
    }

    void addListener (CameraDevice::Listener* listenerToAdd)
    {
        const ScopedLock sl (listenerLock);
        listeners.add (listenerToAdd);

        if (listeners.size() == 1)
            triggerImageCapture();
    }

    void removeListener (CameraDevice::Listener* listenerToRemove)
    {
        const ScopedLock sl (listenerLock);
        listeners.remove (listenerToRemove);
    }

    static NSArray* getCaptureDevices()
    {
        if (@available (macOS 10.15, *))
        {
            HIGGS_BEGIN_IGNORE_DEPRECATION_WARNINGS
            const auto deviceType = AVCaptureDeviceTypeExternalUnknown;
            HIGGS_END_IGNORE_DEPRECATION_WARNINGS

            auto* discovery = [AVCaptureDeviceDiscoverySession discoverySessionWithDeviceTypes: @[AVCaptureDeviceTypeBuiltInWideAngleCamera, deviceType]
                                                                                     mediaType: AVMediaTypeVideo
                                                                                      position: AVCaptureDevicePositionUnspecified];

            return [discovery devices];
        }

        HIGGS_BEGIN_IGNORE_DEPRECATION_WARNINGS
        return [AVCaptureDevice devicesWithMediaType: AVMediaTypeVideo];
        HIGGS_END_IGNORE_DEPRECATION_WARNINGS
    }

    static StringArray getAvailableDevices()
    {
        StringArray results;

        for (AVCaptureDevice* device : getCaptureDevices())
            results.add (nsStringToHiggs ([device localizedName]));

        return results;
    }

    AVCaptureSession* getCaptureSession()
    {
        return session;
    }

    NSView* createVideoCapturePreview()
    {
        // The video preview must be created before the capture session is
        // started. Make sure you haven't called `addListener`,
        // `startRecordingToFile`, or `takeStillPicture` before calling this
        // function.
        jassert (! [session isRunning]);
        startSession();

        HIGGS_AUTORELEASEPOOL
        {
            NSView* view = [[NSView alloc] init];
            [view setLayer: [AVCaptureVideoPreviewLayer layerWithSession: getCaptureSession()]];
            return view;
        }
    }

private:
    //==============================================================================
    struct DelegateClass  : public ObjCClass<NSObject>
    {
        DelegateClass()
            : ObjCClass ("HIGGSCameraDelegate_")
        {
            addIvar<Pimpl*> ("owner");
            addProtocol (@protocol (AVCaptureFileOutputRecordingDelegate));

            addMethod (@selector (captureOutput:didStartRecordingToOutputFileAtURL:  fromConnections:),       didStartRecordingToOutputFileAtURL);
            addMethod (@selector (captureOutput:didPauseRecordingToOutputFileAtURL:  fromConnections:),       didPauseRecordingToOutputFileAtURL);
            addMethod (@selector (captureOutput:didResumeRecordingToOutputFileAtURL: fromConnections:),       didResumeRecordingToOutputFileAtURL);
            addMethod (@selector (captureOutput:willFinishRecordingToOutputFileAtURL:fromConnections:error:), willFinishRecordingToOutputFileAtURL);

            HIGGS_BEGIN_IGNORE_WARNINGS_GCC_LIKE ("-Wundeclared-selector")
            addMethod (@selector (captureSessionRuntimeError:), sessionRuntimeError);
            HIGGS_END_IGNORE_WARNINGS_GCC_LIKE

            registerClass();
        }

        static void setOwner (id self, Pimpl* owner)   { object_setInstanceVariable (self, "owner", owner); }
        static Pimpl& getOwner (id self)               { return *getIvar<Pimpl*> (self, "owner"); }

    private:
        static void didStartRecordingToOutputFileAtURL (id, SEL, AVCaptureFileOutput*, NSURL*, NSArray*) {}
        static void didPauseRecordingToOutputFileAtURL (id, SEL, AVCaptureFileOutput*, NSURL*, NSArray*) {}
        static void didResumeRecordingToOutputFileAtURL (id, SEL, AVCaptureFileOutput*, NSURL*, NSArray*) {}
        static void willFinishRecordingToOutputFileAtURL (id, SEL, AVCaptureFileOutput*, NSURL*, NSArray*, NSError*) {}

        static void sessionRuntimeError (id self, SEL, NSNotification* notification)
        {
            HIGGS_CAMERA_LOG (nsStringToHiggs ([notification description]));

            NSError* error = [notification.userInfo objectForKey: AVCaptureSessionErrorKey];
            auto errorString = error != nil ? nsStringToHiggs (error.localizedDescription) : String();
            getOwner (self).cameraSessionRuntimeError (errorString);
        }
    };

    //==============================================================================
    struct ImageOutputBase
    {
        virtual ~ImageOutputBase() = default;

        virtual void addImageCapture (AVCaptureSession*) = 0;
        virtual void removeImageCapture (AVCaptureSession*) = 0;
        virtual NSArray<AVCaptureConnection*>* getConnections() const = 0;
        virtual void triggerImageCapture (Pimpl& p) = 0;
    };

    class API_AVAILABLE (macos (10.15)) PostCatalinaPhotoOutput  : public ImageOutputBase
    {
    public:
        PostCatalinaPhotoOutput()
        {
            static PhotoOutputDelegateClass cls;
            delegate.reset ([cls.createInstance() init]);
        }

        void addImageCapture (AVCaptureSession* s) override
        {
            if (imageOutput != nil)
                return;

            imageOutput = [[AVCapturePhotoOutput alloc] init];
            [s addOutput: imageOutput];
        }

        void removeImageCapture (AVCaptureSession* s) override
        {
            if (imageOutput == nil)
                return;

            [s removeOutput: imageOutput];
            [imageOutput release];
            imageOutput = nil;
        }

        NSArray<AVCaptureConnection*>* getConnections() const override
        {
            if (imageOutput != nil)
                return imageOutput.connections;

            return nil;
        }

        void triggerImageCapture (Pimpl& p) override
        {
            if (imageOutput == nil)
                return;

            PhotoOutputDelegateClass::setOwner (delegate.get(), &p);

            [imageOutput capturePhotoWithSettings: [AVCapturePhotoSettings photoSettings]
                                         delegate: delegate.get()];
        }

    private:
        class PhotoOutputDelegateClass : public ObjCClass<NSObject<AVCapturePhotoCaptureDelegate>>
        {
        public:
            PhotoOutputDelegateClass()
                : ObjCClass ("PhotoOutputDelegateClass_")
            {
                addMethod (@selector (captureOutput:didFinishProcessingPhoto:error:), [] (id self, SEL, AVCapturePhotoOutput*, AVCapturePhoto* photo, NSError* error)
                {
                    if (error != nil)
                    {
                        [[maybe_unused]] String errorString = error != nil ? nsStringToHiggs (error.localizedDescription) : String();

                        HIGGS_CAMERA_LOG ("Still picture capture failed, error: " + errorString);
                        jassertfalse;

                        return;
                    }

                    auto* imageData = [photo fileDataRepresentation];
                    auto image = ImageFileFormat::loadFrom (imageData.bytes, (size_t) imageData.length);

                    getOwner (self).imageCaptureFinished (image);
                });

                addIvar<Pimpl*> ("owner");
                registerClass();
            }

            static Pimpl& getOwner (id self) { return *getIvar<Pimpl*> (self, "owner"); }
            static void setOwner (id self, Pimpl* t) { object_setInstanceVariable (self, "owner", t); }
        };

        AVCapturePhotoOutput* imageOutput = nil;
        NSUniquePtr<NSObject<AVCapturePhotoCaptureDelegate>> delegate;
    };

    HIGGS_BEGIN_IGNORE_DEPRECATION_WARNINGS
    class PreCatalinaStillImageOutput  : public ImageOutputBase
    {
    public:
        void addImageCapture (AVCaptureSession* s) override
        {
            if (imageOutput != nil)
                return;

            const auto codecType = []
            {
                if (@available (macOS 10.13, *))
                   return AVVideoCodecTypeJPEG;

                return AVVideoCodecJPEG;
            }();

            imageOutput = [[AVCaptureStillImageOutput alloc] init];
            auto imageSettings = [[NSDictionary alloc] initWithObjectsAndKeys: codecType, AVVideoCodecKey, nil];
            [imageOutput setOutputSettings: imageSettings];
            [imageSettings release];
            [s addOutput: imageOutput];
        }

        void removeImageCapture (AVCaptureSession* s) override
        {
            if (imageOutput == nil)
                return;

            [s removeOutput: imageOutput];
            [imageOutput release];
            imageOutput = nil;
        }

        NSArray<AVCaptureConnection*>* getConnections() const override
        {
            if (imageOutput != nil)
                return imageOutput.connections;

            return nil;
        }

        void triggerImageCapture (Pimpl& p) override
        {
            if (auto* videoConnection = p.getVideoConnection())
            {
                [imageOutput captureStillImageAsynchronouslyFromConnection: videoConnection
                                                         completionHandler: ^(CMSampleBufferRef sampleBuffer, NSError* error)
                {
                    if (error != nil)
                    {
                        HIGGS_CAMERA_LOG ("Still picture capture failed, error: " + nsStringToHiggs (error.localizedDescription));
                        jassertfalse;
                        return;
                    }

                    auto* imageData = [AVCaptureStillImageOutput jpegStillImageNSDataRepresentation: sampleBuffer];
                    auto image = ImageFileFormat::loadFrom (imageData.bytes, (size_t) imageData.length);
                    p.imageCaptureFinished (image);
                }];
            }
        }

    private:
        AVCaptureStillImageOutput* imageOutput = nil;
    };
    HIGGS_END_IGNORE_DEPRECATION_WARNINGS

    //==============================================================================
    void addImageCapture()
    {
        imageOutput->addImageCapture (session);
    }

    void addMovieCapture()
    {
        if (fileOutput == nil)
        {
            fileOutput = [[AVCaptureMovieFileOutput alloc] init];
            [session addOutput: fileOutput];
        }
    }

    void removeImageCapture()
    {
        imageOutput->removeImageCapture (session);
    }

    void removeMovieCapture()
    {
        if (fileOutput != nil)
        {
            [session removeOutput: fileOutput];
            [fileOutput release];
            fileOutput = nil;
        }
    }

    void removeCurrentSessionVideoInputs()
    {
        if (session != nil)
        {
            NSArray<AVCaptureDeviceInput*>* inputs = session.inputs;

            for (AVCaptureDeviceInput* input : inputs)
                if ([input.device hasMediaType: AVMediaTypeVideo])
                    [session removeInput:input];
        }
    }

    void addInput()
    {
        if (currentInput == nil)
        {
            for (AVCaptureDevice* device : getCaptureDevices())
            {
                if (deviceName == nsStringToHiggs ([device localizedName]))
                {
                    removeCurrentSessionVideoInputs();

                    NSError* err = nil;
                    AVCaptureDeviceInput* inputDevice = [[AVCaptureDeviceInput alloc] initWithDevice: device
                                                                                               error: &err];

                    jassert (err == nil);

                    if ([session canAddInput: inputDevice])
                    {
                        [session addInput: inputDevice];
                        currentInput = inputDevice;
                    }
                    else
                    {
                        jassertfalse;
                        [inputDevice release];
                    }

                    return;
                }
            }
        }
    }

    void removeInput()
    {
        if (currentInput != nil)
        {
            [session removeInput: currentInput];
            [currentInput release];
            currentInput = nil;
        }
    }

    void refreshConnections()
    {
        [session beginConfiguration];
        removeInput();
        removeImageCapture();
        removeMovieCapture();
        addInput();
        addImageCapture();
        addMovieCapture();
        [session commitConfiguration];
    }

    void refreshIfNeeded()
    {
        if (getVideoConnection() == nullptr)
            refreshConnections();
    }

    AVCaptureConnection* getVideoConnection() const
    {
        auto* connections = imageOutput->getConnections();

        if (connections != nil)
            for (AVCaptureConnection* connection in connections)
                if ([connection isActive] && [connection isEnabled])
                    for (AVCaptureInputPort* port in [connection inputPorts])
                        if ([[port mediaType] isEqual: AVMediaTypeVideo])
                            return connection;

        return nil;
    }

    void imageCaptureFinished (const Image& image)
    {
        handleImageCapture (image);

        MessageManager::callAsync ([weakRef = WeakReference<Pimpl> { this }, image]() mutable
        {
            if (weakRef != nullptr)
                NullCheckedInvocation::invoke (weakRef->pictureTakenCallback, image);
        });
    }

    void handleImageCapture (const Image& image)
    {
        const ScopedLock sl (listenerLock);
        listeners.call ([=] (Listener& l) { l.imageReceived (image); });

        if (! listeners.isEmpty())
            triggerImageCapture();
    }

    void triggerImageCapture()
    {
        refreshIfNeeded();

        startSession();

        if (getVideoConnection() != nullptr)
            imageOutput->triggerImageCapture (*this);
    }

    void cameraSessionRuntimeError (const String& error)
    {
        HIGGS_CAMERA_LOG ("cameraSessionRuntimeError(), error = " + error);

        NullCheckedInvocation::invoke (owner.onErrorOccurred, error);
    }

    //==============================================================================
    CameraDevice& owner;
    String deviceName;

    AVCaptureSession* session = nil;
    AVCaptureMovieFileOutput* fileOutput = nil;
    std::unique_ptr<ImageOutputBase> imageOutput;
    AVCaptureDeviceInput* currentInput = nil;

    id<AVCaptureFileOutputRecordingDelegate> callbackDelegate = nil;
    String openingError;
    Time firstPresentationTime;
    bool isRecording = false;

    CriticalSection listenerLock;
    ListenerList<Listener> listeners;

    std::function<void (const Image&)> pictureTakenCallback = nullptr;

    //==============================================================================
    HIGGS_DECLARE_WEAK_REFERENCEABLE (Pimpl)
    HIGGS_DECLARE_NON_COPYABLE       (Pimpl)
};

//==============================================================================
struct CameraDevice::ViewerComponent  : public NSViewComponent
{
    ViewerComponent (CameraDevice& device)
    {
        setView (device.pimpl->createVideoCapturePreview());
    }

    ~ViewerComponent()
    {
        setView (nil);
    }

    HIGGS_DECLARE_NON_COPYABLE (ViewerComponent)
};

String CameraDevice::getFileExtension()
{
    return ".mov";
}
