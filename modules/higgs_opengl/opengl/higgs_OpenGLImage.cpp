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

class OpenGLFrameBufferImage final : public ImagePixelData,
                                     private OpenGLContext::NativeContextListener
{
public:
    using Ptr = ReferenceCountedObjectPtr<OpenGLFrameBufferImage>;

    OpenGLFrameBufferImage (OpenGLContext& c, int w, int h)
        : ImagePixelData (Image::ARGB, w, h),
          context (&c),
          pixelStride (4)
    {
        registerWith (c);
    }

    ~OpenGLFrameBufferImage() override
    {
        if (context != nullptr)
            unregisterFrom (*context);
    }

    bool initialise()
    {
        if (! frameBuffer.initialise (*context, width, height))
            return false;

        frameBuffer.clear (Colours::transparentBlack);
        return true;
    }

    std::unique_ptr<LowLevelGraphicsContext> createLowLevelContext() override
    {
        if (context == nullptr)
        {
            jassertfalse;
            return SoftwareImageType().create (pixelFormat, width, height, true)->createLowLevelContext();
        }

        sendDataChangeMessage();
        return createOpenGLGraphicsContext (*context, frameBuffer);
    }

    std::unique_ptr<ImageType> createType() const override     { return std::make_unique<OpenGLImageType>(); }

    ImagePixelData::Ptr clone() override
    {
        if (context == nullptr)
        {
            jassertfalse;
            return {};
        }

        std::unique_ptr<OpenGLFrameBufferImage> im (new OpenGLFrameBufferImage (*context, width, height));

        if (! im->initialise())
            return ImagePixelData::Ptr();

        Image newImage (im.release());
        Graphics g (newImage);
        g.drawImageAt (Image (*this), 0, 0, false);

        return ImagePixelData::Ptr (newImage.getPixelData());
    }

    void initialiseBitmapData (Image::BitmapData& bitmapData, int x, int y, Image::BitmapData::ReadWriteMode mode) override
    {
        bitmapData.pixelFormat = pixelFormat;
        bitmapData.pixelStride = pixelStride;

        auto releaser = std::make_unique<DataReleaser> (this, Rectangle { x, y, bitmapData.width, bitmapData.height }, mode);

        const auto unsignedLineStride = (((size_t) bitmapData.width * (size_t) bitmapData.pixelStride + 3) & ~((size_t) 3));
        bitmapData.lineStride = -((int) unsignedLineStride);
        bitmapData.size = (size_t) bitmapData.height * unsignedLineStride;

        // OpenGL mapped textures are stored in lines from bottom-to-top, but HIGGS expects lines to
        // be ordered top-to-bottom.
        // The data pointer points to the beginning of the *last* line, and lineStride steps backwards
        // through the lines.
        bitmapData.data = (uint8*) releaser->data.get() + (ptrdiff_t) bitmapData.size + (ptrdiff_t) bitmapData.lineStride;

        bitmapData.dataReleaser = std::move (releaser);

        if (mode != Image::BitmapData::readOnly)
            sendDataChangeMessage();
    }

    OpenGLContext* context;
    OpenGLFrameBuffer frameBuffer;

private:
    void contextWillPause() override {}
    void contextDidResume() override {}
    void contextWillBeDestroyed() override { context = nullptr; }

    int pixelStride;

    struct DataReleaser final : public Image::BitmapData::BitmapDataReleaser
    {
        DataReleaser (Ptr selfIn, Rectangle<int> areaIn, Image::BitmapData::ReadWriteMode modeIn)
            : self (selfIn),
              data ((size_t) (areaIn.getWidth() * areaIn.getHeight())),
              area (areaIn),
              mode (modeIn)
        {
            if (mode != Image::BitmapData::writeOnly)
                self->frameBuffer.readPixels (data.get(), getArea(), order);
        }

        ~DataReleaser() override
        {
            if (mode != Image::BitmapData::readOnly)
                self->frameBuffer.writePixels (data, getArea(), order);
        }

        Rectangle<int> getArea() const
        {
            return area.withBottomY (self->frameBuffer.getHeight() - area.getY());
        }

        Ptr self;
        HeapBlock<PixelARGB> data;
        Rectangle<int> area;
        Image::BitmapData::ReadWriteMode mode;

        static constexpr auto order = OpenGLFrameBuffer::RowOrder::fromBottomUp;
    };

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OpenGLFrameBufferImage)
};

//==============================================================================
class SoftwareBackedOpenGLPixelData final : public ImagePixelData,
                                            private ImagePixelData::Listener
{
public:
    explicit SoftwareBackedOpenGLPixelData (Ptr sourceIn)
        : ImagePixelData (sourceIn->pixelFormat, sourceIn->width, sourceIn->height),
          source (std::move (sourceIn))
    {
        source->listeners.add (this);
    }

    ~SoftwareBackedOpenGLPixelData() override
    {
        source->listeners.remove (this);
    }

    std::unique_ptr<LowLevelGraphicsContext> createLowLevelContext() override
    {
        return source->createLowLevelContext();
    }

    Ptr clone() override
    {
        if (auto cloned = source->clone())
            return *new SoftwareBackedOpenGLPixelData (std::move (cloned));

        return {};
    }

    std::unique_ptr<ImageType> createType() const override
    {
        return std::make_unique<OpenGLImageType>();
    }

    void initialiseBitmapData (Image::BitmapData& bitmapData, int x, int y, Image::BitmapData::ReadWriteMode mode) override
    {
        source->initialiseBitmapData (bitmapData, x, y, mode);
    }

    void applySingleChannelBoxBlurEffectInArea (Rectangle<int> bounds, int radius) override
    {
        source->applySingleChannelBoxBlurEffectInArea (bounds, radius);
    }

    void applyGaussianBlurEffectInArea (Rectangle<int> bounds, float radius) override
    {
        source->applyGaussianBlurEffectInArea (bounds, radius);
    }

    void multiplyAllAlphasInArea (Rectangle<int> bounds, float amount) override
    {
        source->multiplyAllAlphasInArea (bounds, amount);
    }

    void desaturateInArea (Rectangle<int> bounds) override
    {
        source->desaturateInArea (bounds);
    }

    int getSharedCount() const noexcept override { return getReferenceCount() + source->getSharedCount() - 1; }

private:
    void imageDataChanged (ImagePixelData*) override
    {
        sendDataChangeMessage();
    }

    void imageDataBeingDeleted (ImagePixelData*) override {}

    const Ptr source;

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (SoftwareBackedOpenGLPixelData)
};

//==============================================================================
OpenGLImageType::OpenGLImageType() {}
OpenGLImageType::~OpenGLImageType() {}

int OpenGLImageType::getTypeID() const
{
    return 3;
}

ImagePixelData::Ptr OpenGLImageType::create (Image::PixelFormat format, int width, int height, bool shouldClearImage) const
{
    if (format == Image::SingleChannel)
        return *new SoftwareBackedOpenGLPixelData (SoftwareImageType().create (format, width, height, shouldClearImage));

    OpenGLContext* currentContext = OpenGLContext::getCurrentContext();
    jassert (currentContext != nullptr); // an OpenGL image can only be created when a valid context is active!

    std::unique_ptr<OpenGLFrameBufferImage> im (new OpenGLFrameBufferImage (*currentContext, width, height));

    if (! im->initialise())
        return ImagePixelData::Ptr();

    return *im.release();
}

OpenGLFrameBuffer* OpenGLImageType::getFrameBufferFrom (const Image& image)
{
    if (OpenGLFrameBufferImage* const glImage = dynamic_cast<OpenGLFrameBufferImage*> (image.getPixelData().get()))
        return &(glImage->frameBuffer);

    return nullptr;
}

} // namespace higgs
