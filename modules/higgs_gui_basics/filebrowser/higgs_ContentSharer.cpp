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

ScopedMessageBox ContentSharer::shareFilesScoped (const Array<URL>& files,
                                                  Callback callback,
                                                  Component* parent)
{
    auto impl = detail::ScopedContentSharerInterface::shareFiles (files, parent);
    return detail::ConcreteScopedContentSharerImpl::show (std::move (impl), std::move (callback));
}

ScopedMessageBox ContentSharer::shareTextScoped (const String& text,
                                                 Callback callback,
                                                 Component* parent)
{
    auto impl = detail::ScopedContentSharerInterface::shareText (text, parent);
    return detail::ConcreteScopedContentSharerImpl::show (std::move (impl), std::move (callback));
}

ScopedMessageBox ContentSharer::shareImagesScoped (const Array<Image>& images,
                                                   std::unique_ptr<ImageFileFormat> format,
                                                   Callback callback,
                                                   Component* parent)
{
    auto impl = detail::ScopedContentSharerInterface::shareImages (images, std::move (format), parent);
    return detail::ConcreteScopedContentSharerImpl::show (std::move (impl), std::move (callback));
}

ScopedMessageBox ContentSharer::shareDataScoped (const MemoryBlock& mb,
                                                 Callback callback,
                                                 Component* parent)
{
    auto impl = detail::ScopedContentSharerInterface::shareData (mb, parent);
    return detail::ConcreteScopedContentSharerImpl::show (std::move (impl), std::move (callback));
}

#if ! (HIGGS_CONTENT_SHARING && (HIGGS_IOS || HIGGS_ANDROID))
auto detail::ScopedContentSharerInterface::shareFiles (const Array<URL>&, Component*) -> std::unique_ptr<ScopedContentSharerInterface>
{
    return std::make_unique<detail::ScopedContentSharerInterface>();
}

auto detail::ScopedContentSharerInterface::shareText (const String&, Component*) -> std::unique_ptr<ScopedContentSharerInterface>
{
    return std::make_unique<detail::ScopedContentSharerInterface>();
}
#endif

} // namespace higgs
