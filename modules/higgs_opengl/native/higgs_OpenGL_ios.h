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

@interface HiggsGLView   : UIView
{
}
+ (Class) layerClass;
@end

@implementation HiggsGLView
+ (Class) layerClass
{
    return [CAEAGLLayer class];
}
@end

extern "C" GLvoid glResolveMultisampleFramebufferAPPLE();

namespace higgs
{

bool OpenGLHelpers::isOpenGLES()
{
    return true;
}

class OpenGLContext::NativeContext
{
public:
    NativeContext (Component& c,
                   const OpenGLPixelFormat& pixFormat,
                   void* contextToShare,
                   bool multisampling,
                   [[maybe_unused]] API apiIn,
                   Version versionIn,
                   [[maybe_unused]] Profile profileIn)
        : component (c),
          useDepthBuffer (pixFormat.depthBufferBits > 0),
          useMSAA (multisampling)
    {
        // Only OpenGL ES is supported on iOS
        jassert (apiIn == OpenGLAPI::openGLES);
        // Only core profile is supported on iOS
        jassert (profileIn == OpenGLProfile::core);

        HIGGS_AUTORELEASEPOOL
        {
            if (auto* peer = component.getPeer())
            {
                auto bounds = peer->getAreaCoveredBy (component);

                view = [[HiggsGLView alloc] initWithFrame: convertToCGRect (bounds)];
                view.opaque = YES;
                view.hidden = NO;
                view.backgroundColor = [UIColor blackColor];
                view.userInteractionEnabled = NO;

                glLayer = (CAEAGLLayer*) [view layer];
                glLayer.opaque = true;

                updateWindowPosition();

                [((UIView*) peer->getNativeHandle()) addSubview: view];

                const auto shouldUseES3 = versionIn >= Version { 3, 0 }
                                       && [[UIDevice currentDevice].systemVersion floatValue] >= 7.0;

                [[maybe_unused]] const auto gotContext = (shouldUseES3 && createContext (kEAGLRenderingAPIOpenGLES3, contextToShare))
                                                         || createContext (kEAGLRenderingAPIOpenGLES2, contextToShare);

                jassert (gotContext);

                if (context != nil)
                {
                    // I'd prefer to put this stuff in the initialiseOnRenderThread() call, but doing
                    // so causes mysterious timing-related failures.
                    [EAGLContext setCurrentContext: context.get()];
                    gl::loadFunctions();
                    openGLVersion = getOpenGLVersion();
                    createGLBuffers();
                    deactivateCurrentContext();
                }
                else
                {
                    jassertfalse;
                }
            }
            else
            {
                jassertfalse;
            }
        }
    }

    ~NativeContext()
    {
        context.reset();
        [view removeFromSuperview];
        [view release];
    }

    InitResult initialiseOnRenderThread (OpenGLContext&)    { return InitResult::success; }

    void shutdownOnRenderThread()
    {
        HIGGS_CHECK_OPENGL_ERROR
        freeGLBuffers();
        deactivateCurrentContext();
    }

    bool createdOk() const noexcept             { return getRawContext() != nullptr; }
    void* getRawContext() const noexcept        { return context.get(); }
    GLuint getFrameBufferID() const noexcept    { return useMSAA ? msaaBufferHandle : frameBufferHandle; }

    bool makeActive() const noexcept
    {
        if (! [EAGLContext setCurrentContext: context.get()])
            return false;

        glBindFramebuffer (GL_FRAMEBUFFER, useMSAA ? msaaBufferHandle
                                                   : frameBufferHandle);
        return true;
    }

    bool isActive() const noexcept
    {
        return [EAGLContext currentContext] == context.get();
    }

    static void deactivateCurrentContext()
    {
        [EAGLContext setCurrentContext: nil];
    }

    void swapBuffers()
    {
        if (useMSAA)
        {
            glBindFramebuffer (GL_DRAW_FRAMEBUFFER, frameBufferHandle);
            glBindFramebuffer (GL_READ_FRAMEBUFFER, msaaBufferHandle);

            if (openGLVersion >= Version { 3, 0 })
            {
                const auto w = lastBounds.getWidth();
                const auto h = lastBounds.getHeight();

                glBlitFramebuffer (0, 0, w, h,
                                   0, 0, w, h,
                                   GL_COLOR_BUFFER_BIT,
                                   GL_NEAREST);
            }
            else
            {
                ::glResolveMultisampleFramebufferAPPLE();
            }
        }

        glBindRenderbuffer (GL_RENDERBUFFER, colorBufferHandle);
        [context.get() presentRenderbuffer: GL_RENDERBUFFER];

        if (! std::exchange (needToRebuildBuffers, false))
            return;

        freeGLBuffers();
        createGLBuffers();
        makeActive();
    }

    void updateWindowPosition()
    {
        auto* peer = component.getTopLevelComponent()->getPeer();

        if (peer == nullptr)
            return;

        const auto bounds = peer->getAreaCoveredBy (component);
        view.frame = convertToCGRect (bounds);
        glLayer.contentsScale = (CGFloat) (Desktop::getInstance().getDisplays().getPrimaryDisplay()->scale
                                            / component.getDesktopScaleFactor());

        const auto msaaBounds = bounds * glLayer.contentsScale;
        needToRebuildBuffers |= std::exchange (lastBounds, msaaBounds) != msaaBounds;
    }

    bool setSwapInterval (int numFramesPerSwap) noexcept
    {
        swapFrames = numFramesPerSwap;
        return false;
    }

    int getSwapInterval() const noexcept    { return swapFrames; }

    struct Locker
    {
        explicit Locker (NativeContext& ctx) : lock (ctx.mutex) {}
        const ScopedLock lock;
    };

private:
    CriticalSection mutex;
    Component& component;
    HiggsGLView* view = nil;
    CAEAGLLayer* glLayer = nil;
    NSUniquePtr<EAGLContext> context;
    Version openGLVersion{};
    const bool useDepthBuffer, useMSAA;

    GLuint frameBufferHandle = 0, colorBufferHandle = 0, depthBufferHandle = 0,
           msaaColorHandle = 0, msaaBufferHandle = 0;

    Rectangle<int> lastBounds;
    int swapFrames = 0;
    bool needToRebuildBuffers = false;

    bool createContext (EAGLRenderingAPI type, void* contextToShare)
    {
        jassert (context == nil);
        context.reset ([EAGLContext alloc]);

        if (contextToShare != nullptr)
            [context.get() initWithAPI: type  sharegroup: [(EAGLContext*) contextToShare sharegroup]];
        else
            [context.get() initWithAPI: type];

        return context != nil;
    }

    //==============================================================================
    void createGLBuffers()
    {
        glGenFramebuffers (1, &frameBufferHandle);
        glGenRenderbuffers (1, &colorBufferHandle);

        glBindFramebuffer (GL_FRAMEBUFFER, frameBufferHandle);
        glBindRenderbuffer (GL_RENDERBUFFER, colorBufferHandle);

        glFramebufferRenderbuffer (GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, colorBufferHandle);

        [[maybe_unused]] bool ok = [context.get() renderbufferStorage: GL_RENDERBUFFER fromDrawable: glLayer];
        jassert (ok);

        GLint width, height;
        glGetRenderbufferParameteriv (GL_RENDERBUFFER, GL_RENDERBUFFER_WIDTH, &width);
        glGetRenderbufferParameteriv (GL_RENDERBUFFER, GL_RENDERBUFFER_HEIGHT, &height);

        if (useMSAA)
        {
            glGenFramebuffers (1, &msaaBufferHandle);
            glGenRenderbuffers (1, &msaaColorHandle);

            glBindFramebuffer (GL_FRAMEBUFFER, msaaBufferHandle);
            glBindRenderbuffer (GL_RENDERBUFFER, msaaColorHandle);

            glRenderbufferStorageMultisample (GL_RENDERBUFFER, 4, GL_RGBA8, width, height);

            glFramebufferRenderbuffer (GL_FRAMEBUFFER, GL_COLOR_ATTACHMENT0, GL_RENDERBUFFER, msaaColorHandle);
        }

        if (useDepthBuffer)
        {
            glGenRenderbuffers (1, &depthBufferHandle);
            glBindRenderbuffer (GL_RENDERBUFFER, depthBufferHandle);

            if (useMSAA)
                glRenderbufferStorageMultisample (GL_RENDERBUFFER, 4, GL_DEPTH_COMPONENT16, width, height);
            else
                glRenderbufferStorage (GL_RENDERBUFFER, GL_DEPTH_COMPONENT16, width, height);

            glFramebufferRenderbuffer (GL_FRAMEBUFFER, GL_DEPTH_ATTACHMENT, GL_RENDERBUFFER, depthBufferHandle);
        }

        jassert (glCheckFramebufferStatus (GL_FRAMEBUFFER) == GL_FRAMEBUFFER_COMPLETE);
        HIGGS_CHECK_OPENGL_ERROR
    }

    void freeGLBuffers()
    {
        HIGGS_CHECK_OPENGL_ERROR
        [context.get() renderbufferStorage: GL_RENDERBUFFER fromDrawable: nil];

        deleteFrameBuffer (frameBufferHandle);
        deleteFrameBuffer (msaaBufferHandle);
        deleteRenderBuffer (colorBufferHandle);
        deleteRenderBuffer (depthBufferHandle);
        deleteRenderBuffer (msaaColorHandle);

        HIGGS_CHECK_OPENGL_ERROR
    }

    static void deleteFrameBuffer  (GLuint& i)   { if (i != 0) glDeleteFramebuffers  (1, &i); i = 0; }
    static void deleteRenderBuffer (GLuint& i)   { if (i != 0) glDeleteRenderbuffers (1, &i); i = 0; }

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NativeContext)
};

//==============================================================================
bool OpenGLHelpers::isContextActive()
{
    return [EAGLContext currentContext] != nil;
}

} // namespace higgs
