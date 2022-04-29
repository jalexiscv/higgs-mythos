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
    A simple preview component that shows thumbnails of image files.

    @see FileChooserDialogBox, FilePreviewComponent

    @tags{GUI}
*/
class HIGGS_API  ImagePreviewComponent  : public FilePreviewComponent,
                                         private Timer
{
public:
    //==============================================================================
    /** Creates an ImagePreviewComponent. */
    ImagePreviewComponent();

    /** Destructor. */
    ~ImagePreviewComponent() override;

    //==============================================================================
    /** @internal */
    void selectedFileChanged (const File& newSelectedFile) override;
    /** @internal */
    void paint (Graphics&) override;
    /** @internal */
    void timerCallback() override;
    /** @internal */
    std::unique_ptr<AccessibilityHandler> createAccessibilityHandler() override;

private:
    File fileToLoad;
    Image currentThumbnail;
    String currentDetails;

    void getThumbSize (int& w, int& h) const;

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ImagePreviewComponent)
};

} // namespace higgs
