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

static const int minNumberOfStringsForGarbageCollection = 300;
static const uint32 garbageCollectionInterval = 30000;


StringPool::StringPool() noexcept  : lastGarbageCollectionTime (0) {}

struct StartEndString
{
    StartEndString (String::CharPointerType s, String::CharPointerType e) noexcept : start (s), end (e) {}
    operator String() const   { return String (start, end); }

    String::CharPointerType start, end;
};

static int compareStrings (const String& s1, const String& s2) noexcept     { return s1.compare (s2); }
static int compareStrings (CharPointer_UTF8 s1, const String& s2) noexcept  { return s1.compare (s2.getCharPointer()); }

static int compareStrings (const StartEndString& string1, const String& string2) noexcept
{
    String::CharPointerType s1 (string1.start), s2 (string2.getCharPointer());

    for (;;)
    {
        const int c1 = s1 < string1.end ? (int) s1.getAndAdvance() : 0;
        const int c2 = (int) s2.getAndAdvance();
        const int diff = c1 - c2;

        if (diff != 0)  return diff < 0 ? -1 : 1;
        if (c1 == 0)    break;
    }

    return 0;
}

template <typename NewStringType>
static String addPooledString (Array<String>& strings, const NewStringType& newString)
{
    int start = 0;
    int end = strings.size();

    while (start < end)
    {
        const String& startString = strings.getReference (start);
        const int startComp = compareStrings (newString, startString);

        if (startComp == 0)
            return startString;

        const int halfway = (start + end) / 2;

        if (halfway == start)
        {
            if (startComp > 0)
                ++start;

            break;
        }

        const String& halfwayString = strings.getReference (halfway);
        const int halfwayComp = compareStrings (newString, halfwayString);

        if (halfwayComp == 0)
            return halfwayString;

        if (halfwayComp > 0)
            start = halfway;
        else
            end = halfway;
    }

    strings.insert (start, newString);
    return strings.getReference (start);
}

String StringPool::getPooledString (const char* const newString)
{
    if (newString == nullptr || *newString == 0)
        return {};

    const ScopedLock sl (lock);
    garbageCollectIfNeeded();
    return addPooledString (strings, CharPointer_UTF8 (newString));
}

String StringPool::getPooledString (String::CharPointerType start, String::CharPointerType end)
{
    if (start.isEmpty() || start == end)
        return {};

    const ScopedLock sl (lock);
    garbageCollectIfNeeded();
    return addPooledString (strings, StartEndString (start, end));
}

String StringPool::getPooledString (StringRef newString)
{
    if (newString.isEmpty())
        return {};

    const ScopedLock sl (lock);
    garbageCollectIfNeeded();
    return addPooledString (strings, newString.text);
}

String StringPool::getPooledString (const String& newString)
{
    if (newString.isEmpty())
        return {};

    const ScopedLock sl (lock);
    garbageCollectIfNeeded();
    return addPooledString (strings, newString);
}

void StringPool::garbageCollectIfNeeded()
{
    if (strings.size() > minNumberOfStringsForGarbageCollection
         && Time::getApproximateMillisecondCounter() > lastGarbageCollectionTime + garbageCollectionInterval)
        garbageCollect();
}

void StringPool::garbageCollect()
{
    const ScopedLock sl (lock);

    for (int i = strings.size(); --i >= 0;)
        if (strings.getReference (i).getReferenceCount() == 1)
            strings.remove (i);

    lastGarbageCollectionTime = Time::getApproximateMillisecondCounter();
}

StringPool& StringPool::getGlobalPool() noexcept
{
    static StringPool pool;
    return pool;
}

} // namespace higgs
