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

#pragma once

#if HIGGS_MAC

#include <higgs_audio_plugin_client/detail/higgs_IncludeModuleHeaders.h>

namespace higgs::detail
{

struct VSTWindowUtilities
{
    VSTWindowUtilities() = delete;

    static void* attachComponentToWindowRefVST (Component* comp,
                                                int desktopFlags,
                                                void* parentWindowOrView)
    {
        HIGGS_AUTORELEASEPOOL
        {
            NSView* parentView = [(NSView*) parentWindowOrView retain];

            const auto defaultFlags = HiggsPlugin_EditorRequiresKeyboardFocus
                                    ? 0
                                    : ComponentPeer::windowIgnoresKeyPresses;
            comp->addToDesktop (desktopFlags | defaultFlags, parentView);

            // this workaround is because Wavelab provides a zero-size parent view
            if (approximatelyEqual ([parentView frame].size.height, 0.0))
                [((NSView*) comp->getWindowHandle()) setFrameOrigin: NSZeroPoint];

            comp->setVisible (true);
            comp->toFront (false);

            [[parentView window] setAcceptsMouseMovedEvents: YES];
            return parentView;
        }
    }

    static void detachComponentFromWindowRefVST (Component* comp,
                                                void* window)
    {
        HIGGS_AUTORELEASEPOOL
        {
            comp->removeFromDesktop();
            [(id) window release];
        }
    }

    static void setNativeHostWindowSizeVST (void* window,
                                            Component* component,
                                            int newWidth,
                                            int newHeight)
    {
        HIGGS_AUTORELEASEPOOL
        {
            if (NSView* hostView = (NSView*) window)
            {
                const int dx = newWidth  - component->getWidth();
                const int dy = newHeight - component->getHeight();

                NSRect r = [hostView frame];
                r.size.width += dx;
                r.size.height += dy;
                r.origin.y -= dy;
                [hostView setFrame: r];
            }
        }
    }
};

} // namespace higgs::detail

#endif
