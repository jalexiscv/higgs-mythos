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

class NativeScopedContentSharerInterface final : public detail::ScopedContentSharerInterface,
                                                 public detail::NativeModalWrapperComponent
{
public:
    NativeScopedContentSharerInterface (Component* parentIn, NSUniquePtr<NSArray> itemsIn)
        : parent (parentIn), items (std::move (itemsIn)) {}

    void runAsync (std::function<void (bool, const String&)> callback) override
    {
        if ([items.get() count] == 0)
        {
            jassertfalse;
            NullCheckedInvocation::invoke (callback, false, "No valid items found for sharing.");
            return;
        }

        controller.reset ([[UIActivityViewController alloc] initWithActivityItems: items.get()
                                                            applicationActivities: nil]);

        controller.get().excludedActivityTypes = nil;

        controller.get().completionWithItemsHandler = ^([[maybe_unused]] UIActivityType type, BOOL completed,
                                                        [[maybe_unused]] NSArray* returnedItems, NSError* error)
        {
            const auto errorDescription = error != nil ? nsStringToHiggs ([error localizedDescription])
                                                       : String();
            exitModalState (0);

            NullCheckedInvocation::invoke (callback, completed && errorDescription.isEmpty(), errorDescription);
        };

        displayNativeWindowModally (parent);

        enterModalState (true, nullptr, false);
    }

    void close() override
    {
        [controller.get() dismissViewControllerAnimated: YES completion: nil];
    }

private:
    UIViewController* getViewController() const override { return controller.get(); }

    Component* parent = nullptr;
    NSUniquePtr<UIActivityViewController> controller;
    NSUniquePtr<NSArray> items;
};

auto detail::ScopedContentSharerInterface::shareFiles (const Array<URL>& files, Component* parent) -> std::unique_ptr<ScopedContentSharerInterface>
{
    NSUniquePtr<NSMutableArray> urls ([[NSMutableArray arrayWithCapacity: (NSUInteger) files.size()] retain]);

    for (const auto& f : files)
    {
        NSString* nativeFilePath = nil;

        if (f.isLocalFile())
        {
            nativeFilePath = higgsStringToNS (f.getLocalFile().getFullPathName());
        }
        else
        {
            auto filePath = f.toString (false);

            auto* fileDirectory = filePath.contains ("/")
                                ? higgsStringToNS (filePath.upToLastOccurrenceOf ("/", false, false))
                                : [NSString string];

            auto fileName = higgsStringToNS (filePath.fromLastOccurrenceOf ("/", false, false)
                                            .upToLastOccurrenceOf (".", false, false));

            auto fileExt = higgsStringToNS (filePath.fromLastOccurrenceOf (".", false, false));

            if ([fileDirectory length] == NSUInteger (0))
                nativeFilePath = [[NSBundle mainBundle] pathForResource: fileName
                                                                 ofType: fileExt];
            else
                nativeFilePath = [[NSBundle mainBundle] pathForResource: fileName
                                                                 ofType: fileExt
                                                            inDirectory: fileDirectory];
        }

        if (nativeFilePath != nil)
            [urls.get() addObject: [NSURL fileURLWithPath: nativeFilePath]];
    }

    return std::make_unique<NativeScopedContentSharerInterface> (parent, std::move (urls));
}

auto detail::ScopedContentSharerInterface::shareText (const String& text, Component* parent) -> std::unique_ptr<ScopedContentSharerInterface>
{
    NSUniquePtr<NSArray> array ([[NSArray arrayWithObject: higgsStringToNS (text)] retain]);
    return std::make_unique<NativeScopedContentSharerInterface> (parent, std::move (array));
}

} // namespace higgs
