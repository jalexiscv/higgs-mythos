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

String String::fromCFString (CFStringRef cfString)
{
    if (cfString == nullptr)
        return {};

    CFRange range = { 0, CFStringGetLength (cfString) };
    CFIndex bytesNeeded = 0;
    CFStringGetBytes (cfString, range, kCFStringEncodingUTF8, 0, false, nullptr, 0, &bytesNeeded);

    HeapBlock<UInt8> utf8 (bytesNeeded + 1);
    CFStringGetBytes (cfString, range, kCFStringEncodingUTF8, 0, false, utf8, bytesNeeded + 1, nullptr);

    return String (CharPointer_UTF8 ((const CharPointer_UTF8::CharType*) utf8.get()),
                   CharPointer_UTF8 ((const CharPointer_UTF8::CharType*) utf8.get() + bytesNeeded));
}

CFStringRef String::toCFString() const
{
    const char* const utf8 = toRawUTF8();

    if (CFStringRef result = CFStringCreateWithBytes (kCFAllocatorDefault, (const UInt8*) utf8,
                                                      (CFIndex) strlen (utf8), kCFStringEncodingUTF8, false))
        return result;

    // If CFStringCreateWithBytes fails, it probably means there was a UTF8 format
    // error, so we'll return an empty string rather than a null pointer.
    return String().toCFString();
}

String String::convertToPrecomposedUnicode() const
{
   #if HIGGS_IOS
    HIGGS_AUTORELEASEPOOL
    {
        return nsStringToHiggs ([higgsStringToNS (*this) precomposedStringWithCanonicalMapping]);
    }
   #else
    UnicodeMapping map;

    map.unicodeEncoding = CreateTextEncoding (kTextEncodingUnicodeDefault,
                                              kUnicodeNoSubset,
                                              kTextEncodingDefaultFormat);

    map.otherEncoding = CreateTextEncoding (kTextEncodingUnicodeDefault,
                                            kUnicodeCanonicalCompVariant,
                                            kTextEncodingDefaultFormat);

    map.mappingVersion = kUnicodeUseLatestMapping;

    UnicodeToTextInfo conversionInfo = {};
    String result;

    if (CreateUnicodeToTextInfo (&map, &conversionInfo) == noErr)
    {
        const size_t bytesNeeded = CharPointer_UTF16::getBytesRequiredFor (getCharPointer());

        HeapBlock<char> tempOut;
        tempOut.calloc (bytesNeeded + 4);

        ByteCount bytesRead = 0;
        ByteCount outputBufferSize = 0;

        if (ConvertFromUnicodeToText (conversionInfo,
                                      bytesNeeded, (ConstUniCharArrayPtr) toUTF16().getAddress(),
                                      kUnicodeDefaultDirectionMask,
                                      0, {}, {}, {},
                                      bytesNeeded, &bytesRead,
                                      &outputBufferSize, tempOut) == noErr)
        {
            result = String (CharPointer_UTF16 (reinterpret_cast<CharPointer_UTF16::CharType*> (tempOut.get())));
        }

        DisposeUnicodeToTextInfo (&conversionInfo);
    }

    return result;
   #endif
}

} // namespace higgs
