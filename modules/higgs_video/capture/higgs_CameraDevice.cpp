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

#if HIGGS_MAC
 #include "../native/higgs_CameraDevice_mac.h"
#elif HIGGS_WINDOWS
 #include "../native/higgs_CameraDevice_windows.h"
#elif HIGGS_IOS
 #include "../native/higgs_CameraDevice_ios.h"
#elif HIGGS_ANDROID
 #include "../native/higgs_CameraDevice_android.h"
#endif

#if HIGGS_ANDROID || HIGGS_IOS
//==============================================================================
class CameraDevice::CameraFactory
{
public:
    static CameraFactory& getInstance()
    {
        static CameraFactory factory;
        return factory;
    }

    void openCamera (int index, OpenCameraResultCallback resultCallback,
                     int minWidth, int minHeight, int maxWidth, int maxHeight, bool useHighQuality)
    {
        auto cameraId = getAvailableDevices()[index];

        if (getCameraIndex (cameraId) != -1)
        {
            // You are trying to open the same camera twice.
            jassertfalse;
            return;
        }

        std::unique_ptr<CameraDevice> device (new CameraDevice (cameraId, index,
                                                                minWidth, minHeight, maxWidth,
                                                                maxHeight, useHighQuality));

        camerasToOpen.add ({ nextRequestId++,
                             std::unique_ptr<CameraDevice> (device.release()),
                             resultCallback });

        auto& pendingOpen = camerasToOpen.getReference (camerasToOpen.size() - 1);

        pendingOpen.device->pimpl->open ([this] (const String& deviceId, const String& error)
                                         {
                                             int cIndex = getCameraIndex (deviceId);

                                             if (cIndex == -1)
                                                 return;

                                             auto& cameraPendingOpen = camerasToOpen.getReference (cIndex);

                                             if (error.isEmpty())
                                                 cameraPendingOpen.resultCallback (cameraPendingOpen.device.release(), error);
                                             else
                                                 cameraPendingOpen.resultCallback (nullptr, error);

                                             int id = cameraPendingOpen.requestId;

                                             MessageManager::callAsync ([this, id]() { removeRequestWithId (id); });
                                         });
    }

private:
    int getCameraIndex (const String& cameraId) const
    {
        for (int i = 0; i < camerasToOpen.size(); ++i)
        {
            auto& pendingOpen = camerasToOpen.getReference (i);

            if (pendingOpen.device->pimpl->getCameraId() == cameraId)
                return i;
        }

        return -1;
    }

    void removeRequestWithId (int id)
    {
        for (int i = camerasToOpen.size(); --i >= 0;)
        {
            if (camerasToOpen.getReference (i).requestId == id)
            {
                camerasToOpen.remove (i);
                return;
            }
        }
    }

    struct PendingCameraOpen
    {
        int requestId;
        std::unique_ptr<CameraDevice> device;
        OpenCameraResultCallback resultCallback;
    };

    Array<PendingCameraOpen> camerasToOpen;
    static int nextRequestId;
};

int CameraDevice::CameraFactory::nextRequestId = 0;

#endif

//==============================================================================
CameraDevice::CameraDevice (const String& nm, int index, int minWidth, int minHeight, int maxWidth, int maxHeight, bool useHighQuality)
   : name (nm), pimpl (new Pimpl (*this, name, index, minWidth, minHeight, maxWidth, maxHeight, useHighQuality))
{
}

CameraDevice::~CameraDevice()
{
    jassert (higgs::MessageManager::getInstance()->currentThreadHasLockedMessageManager());

    stopRecording();
    pimpl.reset();
}

Component* CameraDevice::createViewerComponent()
{
    return new ViewerComponent (*this);
}

void CameraDevice::takeStillPicture (std::function<void (const Image&)> pictureTakenCallback)
{
    pimpl->takeStillPicture (pictureTakenCallback);
}

void CameraDevice::startRecordingToFile (const File& file, int quality)
{
    stopRecording();
    pimpl->startRecordingToFile (file, quality);
}

Time CameraDevice::getTimeOfFirstRecordedFrame() const
{
    return pimpl->getTimeOfFirstRecordedFrame();
}

void CameraDevice::stopRecording()
{
    pimpl->stopRecording();
}

void CameraDevice::addListener (Listener* listenerToAdd)
{
    if (listenerToAdd != nullptr)
        pimpl->addListener (listenerToAdd);
}

void CameraDevice::removeListener (Listener* listenerToRemove)
{
    if (listenerToRemove != nullptr)
        pimpl->removeListener (listenerToRemove);
}

//==============================================================================
StringArray CameraDevice::getAvailableDevices()
{
    HIGGS_AUTORELEASEPOOL
    {
        return Pimpl::getAvailableDevices();
    }
}

CameraDevice* CameraDevice::openDevice ([[maybe_unused]] int index,
                                        [[maybe_unused]] int minWidth, [[maybe_unused]] int minHeight,
                                        [[maybe_unused]] int maxWidth, [[maybe_unused]] int maxHeight,
                                        [[maybe_unused]] bool useHighQuality)
{
    jassert (higgs::MessageManager::getInstance()->currentThreadHasLockedMessageManager());

   #if ! HIGGS_ANDROID && ! HIGGS_IOS
    std::unique_ptr<CameraDevice> d (new CameraDevice (getAvailableDevices() [index], index,
                                                       minWidth, minHeight, maxWidth, maxHeight, useHighQuality));
    if (d != nullptr && d->pimpl->openedOk())
        return d.release();
   #else
    // Use openDeviceAsync to open a camera device on iOS or Android.
    jassertfalse;
   #endif

    return nullptr;
}

void CameraDevice::openDeviceAsync (int index, OpenCameraResultCallback resultCallback,
                                    int minWidth, int minHeight, int maxWidth, int maxHeight, bool useHighQuality)
{
    jassert (higgs::MessageManager::getInstance()->currentThreadHasLockedMessageManager());

    if (resultCallback == nullptr)
    {
        // A valid callback must be passed.
        jassertfalse;
        return;
    }

   #if HIGGS_ANDROID || HIGGS_IOS
    CameraFactory::getInstance().openCamera (index, std::move (resultCallback),
                                             minWidth, minHeight, maxWidth, maxHeight, useHighQuality);
   #else
    auto* device = openDevice (index, minWidth, minHeight, maxWidth, maxHeight, useHighQuality);

    resultCallback (device, device != nullptr ? String() : "Could not open camera device");
   #endif
}

} // namespace higgs
