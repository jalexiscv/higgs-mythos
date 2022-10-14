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

#if HIGGS_WINDOWS || DOXYGEN

//==============================================================================
/**
    A Windows-specific class that can create and embed a HWND inside itself.

    To use it, create one of these, put it in place and make sure it's visible in a
    window, then use setHWND() to assign a HWND to it. The window will then be
    moved and resized to follow the movements of this component.

    Of course, since the window is a native object, it'll obliterate any
    HIGGS components that may overlap this component, but that's life.

    @tags{GUI}
*/
class HIGGS_API  HWNDComponent   : public Component
{
public:
    //==============================================================================
    /** Create an initially-empty container. */
    HWNDComponent();

    /** Destructor. */
    ~HWNDComponent() override;

    /** Assigns a HWND to this peer.

        The window will be retained and released by this component for as long as
        it is needed. To remove the current HWND, just call setHWND (nullptr).

        Note: A void* is used here to avoid including the Windows headers as
        part of HiggsHeader.h, but the method expects a HWND.
    */
    void setHWND (void* hwnd);

    /** Returns the current HWND.

        Note: A void* is returned here to avoid the needing to include the Windows
        headers, so you should just cast the return value to a HWND.
    */
    void* getHWND() const;

    /** Resizes this component to fit the HWND that it contains. */
    void resizeToFit();

    /** Forces the bounds of the HWND to match the bounds of this component. */
    void updateHWNDBounds();

    /** @internal */
    void paint (Graphics&) override;

private:
    class Pimpl;
    std::unique_ptr<Pimpl> pimpl;

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (HWNDComponent)
};

#endif

} // namespace higgs
