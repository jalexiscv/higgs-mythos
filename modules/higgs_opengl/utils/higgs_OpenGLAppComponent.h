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

//==============================================================================
/**
    A base class for writing simple one-page graphical apps.

    A subclass can inherit from this and implement just a few methods such as
    paint() and mouse-handling. The base class provides some simple abstractions
    to take care of continuously repainting itself.

    @tags{OpenGL}
*/
class HIGGS_API  OpenGLAppComponent   : public Component,
                                       private OpenGLRenderer
{
public:
    OpenGLAppComponent();

    /** Destructor. */
    ~OpenGLAppComponent() override;

    /** Returns the number of times that the render method has been called since
        the component started running.
    */
    int getFrameCounter() const noexcept        { return frameCounter; }

    /** This must be called from your subclass's destructor, to shut down
        the GL system and stop it calling render() before your class is destroyed.
    */
    void shutdownOpenGL();

    /** Implement this method to set up any GL objects that you need for rendering.
        The GL context will be active when this method is called.

        Note that because the GL context could be destroyed and re-created ad-hoc by
        the underlying platform, the shutdown() and initialise() calls could be called
        multiple times while your app is running. So don't make your code assume that
        this will only be called once!
    */
    virtual void initialise() = 0;

    /** Implement this method to free any GL objects that you created during rendering.
        The GL context will still be active when this method is called.

        Note that because the GL context could be destroyed and re-created ad-hoc by
        the underlying platform, the shutdown() and initialise() calls could be called
        multiple times while your app is running. So don't make your code assume that
        this will only be called once!
    */
    virtual void shutdown() = 0;

    /** Called to render your openGL.
        @see OpenGLRenderer::render()
    */
    virtual void render() = 0;

    /** The GL context */
    OpenGLContext openGLContext;

private:
    //==============================================================================
    int frameCounter = 0;

    void newOpenGLContextCreated() override;
    void renderOpenGL() override;
    void openGLContextClosing() override;

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (OpenGLAppComponent)
};

} // namespace higgs
