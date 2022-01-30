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

static std::unique_ptr<XmlElement> findFontsConfFile()
{
    static const char* pathsToSearch[] = { "/etc/fonts/fonts.conf",
                                           "/usr/share/fonts/fonts.conf",
                                           "/usr/local/etc/fonts/fonts.conf",
                                           "/usr/share/defaults/fonts/fonts.conf" };

    for (auto* path : pathsToSearch)
        if (auto xml = parseXML (File (path)))
            return xml;

    return {};
}

StringArray FTTypefaceList::getDefaultFontDirectories()
{
    StringArray fontDirs;

    fontDirs.addTokens (String (CharPointer_UTF8 (getenv ("HIGGS_FONT_PATH"))), ";,", "");
    fontDirs.removeEmptyStrings (true);

    if (fontDirs.isEmpty())
    {
        if (auto fontsInfo = findFontsConfFile())
        {
            for (auto* e : fontsInfo->getChildWithTagNameIterator ("dir"))
            {
                auto fontPath = e->getAllSubText().trim();

                if (fontPath.isNotEmpty())
                {
                    if (e->getStringAttribute ("prefix") == "xdg")
                    {
                        auto xdgDataHome = SystemStats::getEnvironmentVariable ("XDG_DATA_HOME", {});

                        if (xdgDataHome.trimStart().isEmpty())
                            xdgDataHome = "~/.local/share";

                        fontPath = File (xdgDataHome).getChildFile (fontPath).getFullPathName();
                    }

                    fontDirs.add (fontPath);
                }
            }
        }
    }

    if (fontDirs.isEmpty())
        fontDirs.add ("/usr/X11R6/lib/X11/fonts");

    fontDirs.removeDuplicates (false);
    return fontDirs;
}

void Typeface::scanFolderForFonts (const File& folder)
{
    FTTypefaceList::getInstance()->scanFontPaths (StringArray (folder.getFullPathName()));
}

StringArray Font::findAllTypefaceNamesImpl()
{
    return FTTypefaceList::getInstance()->findAllFamilyNames();
}

StringArray Font::findAllTypefaceStylesImpl (const String& family)
{
    return FTTypefaceList::getInstance()->findAllTypefaceStyles (family);
}

//==============================================================================
struct DefaultFontInfo
{
    DefaultFontInfo()
        : defaultSans  (getDefaultSansSerifFontName()),
          defaultSerif (getDefaultSerifFontName()),
          defaultFixed (getDefaultMonospacedFontName())
    {
    }

    String getRealFontName (const String& faceName) const
    {
        if (faceName == Font::getDefaultSansSerifFontName())    return defaultSans;
        if (faceName == Font::getDefaultSerifFontName())        return defaultSerif;
        if (faceName == Font::getDefaultMonospacedFontName())   return defaultFixed;

        return faceName;
    }

    String defaultSans, defaultSerif, defaultFixed;

private:
    template <typename Range>
    static String pickBestFont (const StringArray& names, Range&& choicesArray)
    {
        for (auto& choice : choicesArray)
            if (names.contains (choice, true))
                return choice;

        for (auto& choice : choicesArray)
            for (auto& name : names)
                if (name.startsWithIgnoreCase (choice))
                    return name;

        for (auto& choice : choicesArray)
            for (auto& name : names)
                if (name.containsIgnoreCase (choice))
                    return name;

        for (auto& name : names)
            if (name.isNotEmpty())
                return name;

        jassertfalse;
        return {};
    }

    static String getDefaultSansSerifFontName()
    {
        StringArray allFonts;
        FTTypefaceList::getInstance()->getSansSerifNames (allFonts);

        static constexpr const char* targets[] { "Verdana",
                                                 "Bitstream Vera Sans",
                                                 "Luxi Sans",
                                                 "Liberation Sans",
                                                 "DejaVu Sans",
                                                 "Sans" };
        return pickBestFont (allFonts, targets);
    }

    static String getDefaultSerifFontName()
    {
        StringArray allFonts;
        FTTypefaceList::getInstance()->getSerifNames (allFonts);

        static constexpr const char* targets[] { "Bitstream Vera Serif",
                                                 "Times",
                                                 "Nimbus Roman",
                                                 "Liberation Serif",
                                                 "DejaVu Serif",
                                                 "Serif" };
        return pickBestFont (allFonts, targets);
    }

    static String getDefaultMonospacedFontName()
    {
        StringArray allFonts;
        FTTypefaceList::getInstance()->getMonospacedNames (allFonts);

        static constexpr const char* targets[] { "DejaVu Sans Mono",
                                                 "Bitstream Vera Sans Mono",
                                                 "Sans Mono",
                                                 "Liberation Mono",
                                                 "Courier",
                                                 "DejaVu Mono",
                                                 "Mono" };
        return pickBestFont (allFonts, targets);
    }

    HIGGS_DECLARE_NON_COPYABLE (DefaultFontInfo)
};

Typeface::Ptr Font::Native::getDefaultPlatformTypefaceForFont (const Font& font)
{
    static const DefaultFontInfo defaultInfo;

    Font f (font);

    const auto name = font.getTypefaceName();
    const auto realName = defaultInfo.getRealFontName (name);

    if (realName.isEmpty())
        return nullptr;

    f.setTypefaceName (realName);
    return Typeface::createSystemTypefaceFor (f);
}

} // namespace higgs
