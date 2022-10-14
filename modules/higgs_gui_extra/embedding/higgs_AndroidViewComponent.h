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

#if HIGGS_ANDROID || DOXYGEN

//==============================================================================
/**
    An Android-specific class that can create and embed a View inside itself.

    To use it, create one of these, put it in place and make sure it's visible in a
    window, then use setView() to assign a View to it. The view will then be
    moved and resized to follow the movements of this component.

    Of course, since the view is a native object, it'll obliterate any
    HIGGS components that may overlap this component, but that's life.

    @tags{GUI}
*/
class HIGGS_API  AndroidViewComponent   : public Component
{
public:
    //==============================================================================
    /** Create an initially-empty container */
    AndroidViewComponent();

    /** Destructor. */
    ~AndroidViewComponent() override;

    /** Assigns a View to this peer.

        The view will be retained and released by this component for as long as
        it is needed. To remove the current view, just call setView (nullptr).
    */
    void setView (void* uiView);

    /** Returns the current View. */
    void* getView() const;

    /** Resizes this component to fit the view that it contains. */
    void resizeToFitView();

    //==============================================================================
    /** @internal */
    void paint (Graphics&) override;
    /** @internal */
    std::unique_ptr<AccessibilityHandler> createAccessibilityHandler() override;

private:
    class Pimpl;
    std::unique_ptr<Pimpl> pimpl;

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (AndroidViewComponent)
};

#endif

} // namespace higgs
