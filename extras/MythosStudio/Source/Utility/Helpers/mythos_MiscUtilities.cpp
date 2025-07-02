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

#include "../../Application/mythos_Headers.h"

//==============================================================================
String joinLinesIntoSourceFile (StringArray& lines)
{
    while (lines.size() > 10 && lines [lines.size() - 1].isEmpty())
        lines.remove (lines.size() - 1);

    return lines.joinIntoString (getPreferredLineFeed()) + getPreferredLineFeed();
}

String replaceLineFeeds (const String& content, const String& lineFeed)
{
    StringArray lines;
    lines.addLines (content);

    return lines.joinIntoString (lineFeed);
}

String getLineFeedForFile (const String& fileContent)
{
    auto t = fileContent.getCharPointer();

    while (! t.isEmpty())
    {
        switch (t.getAndAdvance())
        {
            case 0:     break;
            case '\n':  return "\n";
            case '\r':  if (*t == '\n') return "\r\n";
                        [[fallthrough]];
            default:    continue;
        }
    }

    return {};
}

String trimCommentCharsFromStartOfLine (const String& line)
{
    return line.trimStart().trimCharactersAtStart ("*/").trimStart();
}

String createAlphaNumericUID()
{
    String uid;
    const char chars[] = "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789";
    Random r;

    uid << chars[r.nextInt (52)]; // make sure the first character is always a letter

    for (int i = 5; --i >= 0;)
    {
        r.setSeedRandomly();
        uid << chars [r.nextInt (62)];
    }

    return uid;
}

String createGUID (const String& seed)
{
    auto hex = MD5 ((seed + "_guidsalt").toUTF8()).toHexString().toUpperCase();

    return "{" + hex.substring (0, 8)
         + "-" + hex.substring (8, 12)
         + "-" + hex.substring (12, 16)
         + "-" + hex.substring (16, 20)
         + "-" + hex.substring (20, 32)
         + "}";
}

String escapeSpaces (const String& s)
{
    return s.replace (" ", "\\ ");
}

String escapeQuotesAndSpaces (const String& s)
{
    return escapeSpaces (s).replace ("'", "\\'").replace ("\"", "\\\"");
}

String addQuotesIfContainsSpaces (const String& text)
{
    return (text.containsChar (' ') && ! text.isQuotedString()) ? text.quoted() : text;
}

void setValueIfVoid (Value value, const var& defaultValue)
{
    if (value.getValue().isVoid())
        value = defaultValue;
}

//==============================================================================
StringPairArray parsePreprocessorDefs (const String& text)
{
    StringPairArray result;
    auto s = text.getCharPointer();

    while (! s.isEmpty())
    {
        String token, value;
        s.incrementToEndOfWhitespace();

        while ((! s.isEmpty()) && *s != '=' && ! s.isWhitespace())
            token << s.getAndAdvance();

        s.incrementToEndOfWhitespace();

        if (*s == '=')
        {
            ++s;

            while ((! s.isEmpty()) && *s == ' ')
                ++s;

            while ((! s.isEmpty()) && ! s.isWhitespace())
            {
                if (*s == ',')
                {
                    ++s;
                    break;
                }

                if (*s == '\\' && (s[1] == ' ' || s[1] == ','))
                    ++s;

                value << s.getAndAdvance();
            }
        }

        if (token.isNotEmpty())
            result.set (token, value);
    }

    return result;
}

StringPairArray mergePreprocessorDefs (StringPairArray inheritedDefs, const StringPairArray& overridingDefs)
{
    for (int i = 0; i < overridingDefs.size(); ++i)
        inheritedDefs.set (overridingDefs.getAllKeys()[i], overridingDefs.getAllValues()[i]);

    return inheritedDefs;
}

String createGCCPreprocessorFlags (const StringPairArray& defs)
{
    String s;

    for (int i = 0; i < defs.size(); ++i)
    {
        auto def = defs.getAllKeys()[i];
        auto value = defs.getAllValues()[i];

        if (value.isNotEmpty())
            def << "=" << value;

        s += " \"" + ("-D" + def).replace ("\"", "\\\"") + "\"";
    }

    return s;
}

StringArray getSearchPathsFromString (const String& searchPath)
{
    StringArray s;
    s.addTokens (searchPath, ";\r\n", StringRef());
    return getCleanedStringArray (s);
}

StringArray getCommaOrWhitespaceSeparatedItems (const String& sourceString)
{
    StringArray s;
    s.addTokens (sourceString, ", \t\r\n", StringRef());
    return getCleanedStringArray (s);
}

StringArray getCleanedStringArray (StringArray s)
{
    s.trim();
    s.removeEmptyStrings();
    return s;
}

//==============================================================================
void autoScrollForMouseEvent (const MouseEvent& e, bool scrollX, bool scrollY)
{
    if (Viewport* const viewport = e.eventComponent->findParentComponentOfClass<Viewport>())
    {
        const MouseEvent e2 (e.getEventRelativeTo (viewport));
        viewport->autoScroll (scrollX ? e2.x : 20, scrollY ? e2.y : 20, 8, 16);
    }
}

//==============================================================================
int indexOfLineStartingWith (const StringArray& lines, const String& text, int index)
{
    const int len = text.length();

    for (const String* i = lines.begin() + index, * const e = lines.end(); i < e; ++i)
    {
        if (CharacterFunctions::compareUpTo (i->getCharPointer().findEndOfWhitespace(),
                                             text.getCharPointer(), len) == 0)
            return index;

        ++index;
    }

    return -1;
}

//==============================================================================
bool fileNeedsCppSyntaxHighlighting (const File& file)
{
    if (file.hasFileExtension (sourceOrHeaderFileExtensions))
        return true;

    // This is a bit of a bodge to deal with libc++ headers with no extension..
    char fileStart[128] = { 0 };
    FileInputStream fin (file);
    fin.read (fileStart, sizeof (fileStart) - 4);

    return CharPointer_UTF8::isValidString (fileStart, sizeof (fileStart))
             && String (fileStart).trimStart().startsWith ("// -*- C++ -*-");
}

//==============================================================================
void writeAutoGenWarningComment (OutputStream& outStream)
{
    outStream << "/*" << newLine << newLine
              << "    IMPORTANT! This file is auto-generated each time you save your" << newLine
              << "    project - if you alter its contents, your changes may be overwritten!" << newLine
              << newLine;
}

//==============================================================================
StringArray getHIGGSModules() noexcept
{
    static StringArray higgsModuleIds =
    {
        "higgs_analytics",
        "higgs_animation",
        "higgs_audio_basics",
        "higgs_audio_devices",
        "higgs_audio_formats",
        "higgs_audio_plugin_client",
        "higgs_audio_processors",
        "higgs_audio_processors_headless",
        "higgs_audio_utils",
        "higgs_box2d",
        "higgs_core",
        "higgs_cryptography",
        "higgs_data_structures",
        "higgs_dsp",
        "higgs_events",
        "higgs_graphics",
        "higgs_gui_basics",
        "higgs_gui_extra",
        "higgs_javascript",
        "higgs_opengl",
        "higgs_osc",
        "higgs_product_unlocking",
        "higgs_video",
        "higgs_midi_ci"
    };

    return higgsModuleIds;
}

bool isHIGGSModule (const String& moduleID) noexcept
{
    return getHIGGSModules().contains (moduleID);
}

StringArray getModulesRequiredForConsole() noexcept
{
    return
    {
        "higgs_core",
        "higgs_data_structures",
        "higgs_events"
    };
}

StringArray getModulesRequiredForComponent() noexcept
{
    return
    {
        "higgs_core",
        "higgs_data_structures",
        "higgs_events",
        "higgs_graphics",
        "higgs_gui_basics"
    };
}

StringArray getModulesRequiredForAudioProcessor() noexcept
{
    return
    {
        "higgs_audio_basics",
        "higgs_audio_devices",
        "higgs_audio_formats",
        "higgs_audio_plugin_client",
        "higgs_audio_processors",
        "higgs_audio_processors_headless",
        "higgs_audio_utils",
        "higgs_core",
        "higgs_data_structures",
        "higgs_events",
        "higgs_graphics",
        "higgs_gui_basics",
        "higgs_gui_extra"
    };
}

bool isPIPFile (const File& file) noexcept
{
    for (auto line : StringArray::fromLines (file.loadFileAsString()))
    {
        auto trimmedLine = trimCommentCharsFromStartOfLine (line);

        if (trimmedLine.startsWith ("BEGIN_HIGGS_PIP_METADATA"))
            return true;
    }

    return false;
}

bool isValidHIGGSExamplesDirectory (const File& directory) noexcept
{
    if (! directory.exists() || ! directory.isDirectory() || ! directory.containsSubDirectories())
        return false;

    return directory.getChildFile ("Assets").getChildFile ("higgs_icon.png").existsAsFile();
}

bool isHIGGSFolder (const File& f)
{
    return isHIGGSModulesFolder (f.getChildFile ("modules"));
}

bool isHIGGSModulesFolder (const File& f)
{
    return f.isDirectory() && f.getChildFile ("higgs_core").isDirectory();
}

//==============================================================================
static bool isDivider (const String& line)
{
    auto afterIndent = line.trim();

    if (afterIndent.startsWith ("//") && afterIndent.length() > 20)
    {
        afterIndent = afterIndent.substring (5);

        if (afterIndent.containsOnly ("=")
            || afterIndent.containsOnly ("/")
            || afterIndent.containsOnly ("-"))
        {
            return true;
        }
    }

    return false;
}

static int getIndexOfCommentBlockStart (const StringArray& lines, int endIndex)
{
    auto endLine = lines[endIndex];

    if (endLine.contains ("*/"))
    {
        for (int i = endIndex; i >= 0; --i)
            if (lines[i].contains ("/*"))
                return i;
    }

     if (endLine.trim().startsWith ("//") && ! isDivider (endLine))
     {
         for (int i = endIndex; i >= 0; --i)
             if (! lines[i].startsWith ("//") || isDivider (lines[i]))
                 return i + 1;
     }

    return -1;
}

int findBestLineToScrollToForClass (StringArray lines, const String& className, bool isPlugin)
{
    for (auto line : lines)
    {
        if (line.contains ("struct " + className) || line.contains ("class " + className)
            || (isPlugin && line.contains ("public AudioProcessor") && ! line.contains ("AudioProcessorEditor")))
        {
            auto index = lines.indexOf (line);

            auto commentBlockStartIndex = getIndexOfCommentBlockStart (lines, index - 1);

            if (commentBlockStartIndex != -1)
                index = commentBlockStartIndex;

            if (isDivider (lines[index - 1]))
                index -= 1;

            return index;
        }
    }

    return 0;
}

//==============================================================================
static var parseHIGGSHeaderMetadata (const StringArray& lines)
{
    auto* o = new DynamicObject();
    var result (o);

    for (auto& line : lines)
    {
        auto trimmedLine = trimCommentCharsFromStartOfLine (line);

        auto colon = trimmedLine.indexOfChar (':');

        if (colon >= 0)
        {
            auto key = trimmedLine.substring (0, colon).trim();
            auto value = trimmedLine.substring (colon + 1).trim();

            o->setProperty (key, value);
        }
    }

    return result;
}

static String parseMetadataItem (const StringArray& lines, int& index)
{
    String result = lines[index++];

    while (index < lines.size())
    {
        auto continuationLine = trimCommentCharsFromStartOfLine (lines[index]);

        if (continuationLine.isEmpty() || continuationLine.indexOfChar (':') != -1
            || continuationLine.startsWith ("END_HIGGS_"))
            break;

        result += " " + continuationLine;
        ++index;
    }

    return result;
}

var parseHIGGSHeaderMetadata (const File& file)
{
    StringArray lines;
    file.readLines (lines);

    for (int i = 0; i < lines.size(); ++i)
    {
        auto trimmedLine = trimCommentCharsFromStartOfLine (lines[i]);

        if (trimmedLine.startsWith ("BEGIN_HIGGS_"))
        {
            StringArray desc;
            auto j = i + 1;

            while (j < lines.size())
            {
                if (trimCommentCharsFromStartOfLine (lines[j]).startsWith ("END_HIGGS_"))
                    return parseHIGGSHeaderMetadata (desc);

                desc.add (parseMetadataItem (lines, j));
            }
        }
    }

    return {};
}
