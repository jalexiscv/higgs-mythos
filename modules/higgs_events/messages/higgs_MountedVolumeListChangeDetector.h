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

#if HIGGS_MAC || HIGGS_WINDOWS || DOXYGEN

//==============================================================================
/**
    An instance of this class will provide callbacks when drives are
    mounted or unmounted on the system.

    Just inherit from this class and implement the pure virtual method
    to get the callbacks, there's no need to do anything else.

    @see File::findFileSystemRoots()

    @tags{Events}
*/
class HIGGS_API  MountedVolumeListChangeDetector
{
public:
    MountedVolumeListChangeDetector();
    virtual ~MountedVolumeListChangeDetector();

    /** This method is called when a volume is mounted or unmounted. */
    virtual void mountedVolumeListChanged() = 0;

private:
    HIGGS_PUBLIC_IN_DLL_BUILD (struct Pimpl)
    std::unique_ptr<Pimpl> pimpl;

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (MountedVolumeListChangeDetector)
};

#endif

} // namespace higgs
