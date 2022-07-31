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

/**
    Objects of this type can be used to programmatically close message boxes.

    @see NativeMessageBox::showScopedAsync(), AlertWindow::showScopedAsync()

    @tags{GUI}
*/
class HIGGS_API ScopedMessageBox
{
public:
    /** @internal */
    explicit ScopedMessageBox (std::shared_ptr<detail::ScopedMessageBoxImpl>);

    /** Constructor */
    ScopedMessageBox();

    /** Destructor */
    ~ScopedMessageBox() noexcept;

    /** Move constructor */
    ScopedMessageBox (ScopedMessageBox&&) noexcept;

    /** Move assignment operator */
    ScopedMessageBox& operator= (ScopedMessageBox&&) noexcept;

    /** Closes the message box, if it is currently showing.

        This is also called automatically during ~ScopedMessageBox. This is useful if you want
        to display a message corresponding to a particular view, and hide the message automatically
        when the view is hidden. This situation commonly arises when displaying messages in plugin
        editors.
    */
    void close();

private:
    std::shared_ptr<detail::ScopedMessageBoxImpl> impl;

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ScopedMessageBox)
};

} // namespace higgs
