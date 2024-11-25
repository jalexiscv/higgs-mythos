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
    A base class that should be implemented by classes which want to render openGL
    on a background thread.

    @see OpenGLContext

    @tags{OpenGL}
*/
class HIGGS_API  OpenGLRenderer
{
public:
    OpenGLRenderer() = default;
    virtual ~OpenGLRenderer() = default;

    /** Called when a new GL context has been created.
        You can use this as an opportunity to create your textures, shaders, etc.
        When the method is invoked, the new GL context will be active.
        Note that this callback will be made on a background thread, so make sure
        that your implementation is thread-safe.
    */
    virtual void newOpenGLContextCreated() = 0;

    /** Called when you should render the next openGL frame.

        Note that this callback will be made on a background thread.

        If the context is attached to a component in order to do component rendering,
        then the MessageManager will be locked when this callback is made.

        If no component rendering is being done, then the MessageManager will not be
        locked, and you'll need to make sure your code is thread-safe in any
        interactions it has with your GUI classes.

        For information about how to trigger a render callback, see
        OpenGLContext::triggerRepaint() and OpenGLContext::setContinuousRepainting().

        IMPORTANT: Never take a MessageManagerLock inside this function! On
        macOS, the OpenGL context will be locked for the duration of this call.
        The main thread may also attempt to interact with the OpenGL context at
        any time, which will also require locking the OpenGL context. As a
        result, taking a MessageManagerLock inside renderOpenGL() may cause a
        hierarchical deadlock.
    */
    virtual void renderOpenGL() = 0;

    /** Called when the current openGL context is about to close.
        You can use this opportunity to release any GL resources that you may have
        created.

        Note that this callback will be made on a background thread, so make sure
        that your implementation is thread-safe.

        (Also note that on Android, this callback won't happen, because there's currently
        no way to implement it.)
    */
    virtual void openGLContextClosing() = 0;
};

} // namespace higgs
