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

LocalisedStrings::LocalisedStrings (const String& fileContents, bool ignoreCase)
{
    loadFromText (fileContents, ignoreCase);
}

LocalisedStrings::LocalisedStrings (const File& fileToLoad, bool ignoreCase)
{
    loadFromText (fileToLoad.loadFileAsString(), ignoreCase);
}

LocalisedStrings::LocalisedStrings (const LocalisedStrings& other)
    : languageName (other.languageName), countryCodes (other.countryCodes),
      translations (other.translations), fallback (createCopyIfNotNull (other.fallback.get()))
{
}

LocalisedStrings& LocalisedStrings::operator= (const LocalisedStrings& other)
{
    languageName = other.languageName;
    countryCodes = other.countryCodes;
    translations = other.translations;
    fallback.reset (createCopyIfNotNull (other.fallback.get()));
    return *this;
}

//==============================================================================
String LocalisedStrings::translate (const String& text) const
{
    if (fallback != nullptr && ! translations.containsKey (text))
        return fallback->translate (text);

    return translations.getValue (text, text);
}

String LocalisedStrings::translate (const String& text, const String& resultIfNotFound) const
{
    if (fallback != nullptr && ! translations.containsKey (text))
        return fallback->translate (text, resultIfNotFound);

    return translations.getValue (text, resultIfNotFound);
}

namespace
{
   #if HIGGS_CHECK_MEMORY_LEAKS
    // By using this object to force a LocalisedStrings object to be created
    // before the currentMappings object, we can force the static order-of-destruction to
    // delete the currentMappings object first, which avoids a bogus leak warning.
    // (Oddly, just creating a LocalisedStrings on the stack doesn't work in gcc, it
    // has to be created with 'new' for this to work.)
    struct LeakAvoidanceTrick
    {
        LeakAvoidanceTrick()
        {
            const std::unique_ptr<LocalisedStrings> dummy (new LocalisedStrings (String(), false));
        }
    };

    LeakAvoidanceTrick leakAvoidanceTrick;
   #endif

    SpinLock currentMappingsLock;
    std::unique_ptr<LocalisedStrings> currentMappings;

    static int findCloseQuote (const String& text, int startPos)
    {
        higgs_wchar lastChar = 0;
        auto t = text.getCharPointer() + startPos;

        for (;;)
        {
            auto c = t.getAndAdvance();

            if (c == 0 || (c == '"' && lastChar != '\\'))
                break;

            lastChar = c;
            ++startPos;
        }

        return startPos;
    }

    static String unescapeString (const String& s)
    {
        return s.replace ("\\\"", "\"")
                .replace ("\\\'", "\'")
                .replace ("\\t", "\t")
                .replace ("\\r", "\r")
                .replace ("\\n", "\n");
    }
}

void LocalisedStrings::loadFromText (const String& fileContents, bool ignoreCase)
{
    translations.setIgnoresCase (ignoreCase);

    StringArray lines;
    lines.addLines (fileContents);

    for (auto& l : lines)
    {
        auto line = l.trim();

        if (line.startsWithChar ('"'))
        {
            auto closeQuote = findCloseQuote (line, 1);
            auto originalText = unescapeString (line.substring (1, closeQuote));

            if (originalText.isNotEmpty())
            {
                auto openingQuote = findCloseQuote (line, closeQuote + 1);
                closeQuote = findCloseQuote (line, openingQuote + 1);
                auto newText = unescapeString (line.substring (openingQuote + 1, closeQuote));

                if (newText.isNotEmpty())
                    translations.set (originalText, newText);
            }
        }
        else if (line.startsWithIgnoreCase ("language:"))
        {
            languageName = line.substring (9).trim();
        }
        else if (line.startsWithIgnoreCase ("countries:"))
        {
            countryCodes.addTokens (line.substring (10).trim(), true);
            countryCodes.trim();
            countryCodes.removeEmptyStrings();
        }
    }

    translations.minimiseStorageOverheads();
}

void LocalisedStrings::addStrings (const LocalisedStrings& other)
{
    jassert (languageName == other.languageName);
    jassert (countryCodes == other.countryCodes);

    translations.addArray (other.translations);
}

void LocalisedStrings::setFallback (LocalisedStrings* f)
{
    fallback.reset (f);
}

//==============================================================================
void LocalisedStrings::setCurrentMappings (LocalisedStrings* newTranslations)
{
    const SpinLock::ScopedLockType sl (currentMappingsLock);
    currentMappings.reset (newTranslations);
}

LocalisedStrings* LocalisedStrings::getCurrentMappings()
{
    return currentMappings.get();
}

String LocalisedStrings::translateWithCurrentMappings (const String& text)  { return higgs::translate (text); }
String LocalisedStrings::translateWithCurrentMappings (const char* text)    { return higgs::translate (text); }

HIGGS_API String translate (const String& text)       { return higgs::translate (text, text); }
HIGGS_API String translate (const char* text)         { return higgs::translate (String (text)); }
HIGGS_API String translate (CharPointer_UTF8 text)    { return higgs::translate (String (text)); }

HIGGS_API String translate (const String& text, const String& resultIfNotFound)
{
    const SpinLock::ScopedLockType sl (currentMappingsLock);

    if (auto* mappings = LocalisedStrings::getCurrentMappings())
        return mappings->translate (text, resultIfNotFound);

    return resultIfNotFound;
}

} // namespace higgs
