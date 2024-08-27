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

#include <higgs_audio_plugin_client/detail/higgs_IncludeModuleHeaders.h>
#include <higgs_audio_plugin_client/detail/higgs_CreatePluginFilter.h>

namespace higgs::detail
{

struct PluginUtilities
{
    PluginUtilities() = delete;

    struct FlagsAndWindowsMultiTouch
    {
        FlagsAndWindowsMultiTouch (int d, bool w)
            : desktopFlags (d),
              windowsUsesMultiTouch (w)
        {
        }

        int desktopFlags{};
        bool windowsUsesMultiTouch{};
    };

    static FlagsAndWindowsMultiTouch getDesktopFlagsAndWindowsMultiTouchMode (const AudioProcessorEditor& editor)
    {
        auto flags = editor.wantsLayerBackedView()
                   ? 0
                   : ComponentPeer::windowRequiresSynchronousCoreGraphicsRendering;

        return { flags, editor.usesWindowsMultiTouch() };
    }

    static FlagsAndWindowsMultiTouch getDesktopFlagsAndWindowsMultiTouchMode (const AudioProcessorEditor* editor)
    {
        return editor != nullptr ? getDesktopFlagsAndWindowsMultiTouchMode (*editor)
                                 : FlagsAndWindowsMultiTouch { 0, false };
    }

    static void addToDesktop (AudioProcessorEditor& editor, void* parent)
    {
        const auto [flags, usesWindowsMultiTouch] = getDesktopFlagsAndWindowsMultiTouchMode (editor);
        editor.addToDesktop (flags, parent);

        if (auto* peer = editor.getPeer())
            peer->setWindowsCanUseMultiTouch (usesWindowsMultiTouch);
    }

    static const PluginHostType& getHostType()
    {
        static PluginHostType hostType;
        return hostType;
    }
};

} // namespace higgs::detail
