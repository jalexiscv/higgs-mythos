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

#if HIGGS_MAC || DOXYGEN

//==============================================================================
/**
    A Mac-specific class that can create and embed an NSView inside itself.

    To use it, create one of these, put it in place and make sure it's visible in a
    window, then use setView() to assign an NSView to it. The view will then be
    moved and resized to follow the movements of this component.

    Of course, since the view is a native object, it'll obliterate any
    HIGGS components that may overlap this component, but that's life.

    @tags{GUI}
*/
class HIGGS_API  NSViewComponent   : public Component
{
public:
    //==============================================================================
    /** Create an initially-empty container. */
    NSViewComponent();

    /** Destructor. */
    ~NSViewComponent() override;

    /** Assigns an NSView to this peer.

        The view will be retained and released by this component for as long as
        it is needed. To remove the current view, just call setView (nullptr).

        Note: A void* is used here to avoid including the cocoa headers as
        part of HiggsHeader.h, but the method expects an NSView*.
    */
    void setView (void* nsView);

    /** Returns the current NSView.

        Note: A void* is returned here to avoid the needing to include the cocoa
        headers, so you should just cast the return value to an NSView*.
    */
    void* getView() const;

    /** Resizes this component to fit the view that it contains. */
    void resizeToFitView();

    /** Resizes the NSView to match the bounds of this component.

        Most of the time, this will be done for you automatically.
    */
    void resizeViewToFit();

    //==============================================================================
    /** @internal */
    void paint (Graphics&) override;
    /** @internal */
    void alphaChanged() override;
    /** @internal */
    static ReferenceCountedObject* attachViewToComponent (Component&, void*);
    /** @internal */
    std::unique_ptr<AccessibilityHandler> createAccessibilityHandler() override;

private:
    ReferenceCountedObjectPtr<ReferenceCountedObject> attachment;

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (NSViewComponent)
};

#endif

} // namespace higgs
