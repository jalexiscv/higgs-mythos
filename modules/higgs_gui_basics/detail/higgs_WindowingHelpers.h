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

namespace higgs::detail
{

struct WindowingHelpers
{
    WindowingHelpers() = delete;

    static Image createIconForFile (const File& file);

    #if HIGGS_WINDOWS
     static bool isEmbeddedInForegroundProcess (Component* c);
    #else
     static bool isEmbeddedInForegroundProcess (Component*) { return false; }
    #endif

    /*  Returns true if this process is in the foreground, or if the viewComponent
        is embedded into a window owned by the foreground process.
    */
    static bool isForegroundOrEmbeddedProcess (Component* viewComponent)
    {
        return Process::isForegroundProcess() || isEmbeddedInForegroundProcess (viewComponent);
    }

    template <typename Value>
    static BorderSize<int> roundToInt (BorderSize<Value> border)
    {
        return { ::higgs::roundToInt (border.getTop()),
                 ::higgs::roundToInt (border.getLeft()),
                 ::higgs::roundToInt (border.getBottom()),
                 ::higgs::roundToInt (border.getRight()) };
    }
};

} // namespace higgs::detail
