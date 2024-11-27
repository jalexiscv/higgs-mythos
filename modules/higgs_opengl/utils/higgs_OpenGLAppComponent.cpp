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

OpenGLAppComponent::OpenGLAppComponent()
{
    setOpaque (true);
    openGLContext.setRenderer (this);
    openGLContext.attachTo (*this);
    openGLContext.setContinuousRepainting (true);
}

OpenGLAppComponent::~OpenGLAppComponent()
{
    // Before your subclass's destructor has completed, you must call
    // shutdownOpenGL() to release the GL context. (Otherwise there's
    // a danger that it may invoke a GL callback on your class while
    // it's in the process of being deleted.
    jassert (! openGLContext.isAttached());

    shutdownOpenGL();
}

void OpenGLAppComponent::shutdownOpenGL()
{
    openGLContext.detach();
}

void OpenGLAppComponent::newOpenGLContextCreated()
{
    initialise();
}

void OpenGLAppComponent::renderOpenGL()
{
    ++frameCounter;
    render();
}

void OpenGLAppComponent::openGLContextClosing()
{
    shutdown();
}

} // namespace higgs
