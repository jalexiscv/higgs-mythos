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
#include "mythos_CodeHelpers.h"

//==============================================================================
namespace CodeHelpers
{
    String indent (const String& code, const int numSpaces, bool indentFirstLine)
    {
        if (numSpaces == 0)
            return code;

        auto space = String::repeatedString (" ", numSpaces);
        auto lines = StringArray::fromLines (code);

        for (auto& line : lines)
        {
            if (! indentFirstLine)
            {
                indentFirstLine = true;
                continue;
            }

            if (line.trimEnd().isNotEmpty())
                line = space + line;
        }

        return lines.joinIntoString (newLine);
    }

    String unindent (const String& code, const int numSpaces)
    {
        if (numSpaces == 0)
            return code;

        auto space = String::repeatedString (" ", numSpaces);
        auto lines = StringArray::fromLines (code);

        for (auto& line : lines)
            if (line.startsWith (space))
                line = line.substring (numSpaces);

        return lines.joinIntoString (newLine);
    }


    String createIncludeStatement (const File& includeFile, const File& targetFile)
    {
        return createIncludeStatement (build_tools::unixStylePath (build_tools::getRelativePathFrom (includeFile, targetFile.getParentDirectory())));
    }

    String createIncludeStatement (const String& includePath)
    {
        if (includePath.startsWithChar ('<') || includePath.startsWithChar ('"'))
            return "#include " + includePath;

        return "#include \"" + includePath + "\"";
    }

    String createIncludePathIncludeStatement (const String& includedFilename)
    {
        return "#include <" + includedFilename + ">";
    }

    String stringLiteral (const String& text, int maxLineLength)
    {
        if (text.isEmpty())
            return "higgs::String()";

        StringArray lines;

        {
            auto t = text.getCharPointer();
            bool finished = t.isEmpty();

            while (! finished)
            {
                for (auto startOfLine = t;;)
                {
                    switch (t.getAndAdvance())
                    {
                        case 0:     finished = true; break;
                        case '\n':  break;
                        case '\r':  if (*t == '\n') ++t; break;
                        default:    continue;
                    }

                    lines.add (String (startOfLine, t));
                    break;
                }
            }
        }

        if (maxLineLength > 0)
        {
            for (int i = 0; i < lines.size(); ++i)
            {
                auto& line = lines.getReference (i);

                if (line.length() > maxLineLength)
                {
                    const String start (line.substring (0, maxLineLength));
                    const String end (line.substring (maxLineLength));
                    line = start;
                    lines.insert (i + 1, end);
                }
            }
        }

        for (int i = 0; i < lines.size(); ++i)
            lines.getReference (i) = CppTokeniserFunctions::addEscapeChars (lines.getReference (i));

        lines.removeEmptyStrings();

        for (int i = 0; i < lines.size(); ++i)
            lines.getReference (i) = "\"" + lines.getReference (i) + "\"";

        String result (lines.joinIntoString (newLine));

        if (! CharPointer_ASCII::isValidString (text.toUTF8(), std::numeric_limits<int>::max()))
            result = "higgs::CharPointer_UTF8 (" + result + ")";

        return result;
    }

    String alignFunctionCallParams (const String& call, const StringArray& parameters, const int maxLineLength)
    {
        String result, currentLine (call);

        for (int i = 0; i < parameters.size(); ++i)
        {
            if (currentLine.length() >= maxLineLength)
            {
                result += currentLine.trimEnd() + newLine;
                currentLine = String::repeatedString (" ", call.length()) + parameters[i];
            }
            else
            {
                currentLine += parameters[i];
            }

            if (i < parameters.size() - 1)
                currentLine << ", ";
        }

        return result + currentLine.trimEnd() + ")";
    }

    String floatLiteral (double value, int numDecPlaces)
    {
        String s (value, numDecPlaces);

        if (s.containsChar ('.'))
            s << 'f';
        else
            s << ".0f";

        return s;
    }

    String boolLiteral (bool value)
    {
        return value ? "true" : "false";
    }

    String colourToCode (Colour col)
    {
        const Colour colours[] =
        {
            #define COL(col)  Colours::col,
            #include "mythos_Colours.h"
            #undef COL
            Colours::transparentBlack
        };

        static const char* colourNames[] =
        {
            #define COL(col)  #col,
            #include "mythos_Colours.h"
            #undef COL
            nullptr
        };

        for (int i = 0; i < numElementsInArray (colourNames) - 1; ++i)
            if (col == colours[i])
                return "higgs::Colours::" + String (colourNames[i]);

        return "higgs::Colour (0x" + build_tools::hexString8Digits ((int) col.getARGB()) + ')';
    }

    String justificationToCode (Justification justification)
    {
        switch (justification.getFlags())
        {
            case Justification::centred:                return "higgs::Justification::centred";
            case Justification::centredLeft:            return "higgs::Justification::centredLeft";
            case Justification::centredRight:           return "higgs::Justification::centredRight";
            case Justification::centredTop:             return "higgs::Justification::centredTop";
            case Justification::centredBottom:          return "higgs::Justification::centredBottom";
            case Justification::topLeft:                return "higgs::Justification::topLeft";
            case Justification::topRight:               return "higgs::Justification::topRight";
            case Justification::bottomLeft:             return "higgs::Justification::bottomLeft";
            case Justification::bottomRight:            return "higgs::Justification::bottomRight";
            case Justification::left:                   return "higgs::Justification::left";
            case Justification::right:                  return "higgs::Justification::right";
            case Justification::horizontallyCentred:    return "higgs::Justification::horizontallyCentred";
            case Justification::top:                    return "higgs::Justification::top";
            case Justification::bottom:                 return "higgs::Justification::bottom";
            case Justification::verticallyCentred:      return "higgs::Justification::verticallyCentred";
            case Justification::horizontallyJustified:  return "higgs::Justification::horizontallyJustified";
            default:                                    break;
        }

        jassertfalse;
        return "Justification (" + String (justification.getFlags()) + ")";
    }

    //==============================================================================
    String getLeadingWhitespace (String line)
    {
        line = line.removeCharacters (line.endsWith ("\r\n") ? "\r\n" : "\n");
        auto endOfLeadingWS = line.getCharPointer().findEndOfWhitespace();
        return String (line.getCharPointer(), endOfLeadingWS);
    }

    int getBraceCount (String::CharPointerType line)
    {
        int braces = 0;

        for (;;)
        {
            const higgs_wchar c = line.getAndAdvance();

            if (c == 0)                         break;
            else if (c == '{')                  ++braces;
            else if (c == '}')                  --braces;
            else if (c == '/')                  { if (*line == '/') break; }
            else if (c == '"' || c == '\'')     { while (! (line.isEmpty() || line.getAndAdvance() == c)) {} }
        }

        return braces;
    }

    bool getIndentForCurrentBlock (CodeDocument::Position pos, const String& tab,
                                   String& blockIndent, String& lastLineIndent)
    {
        int braceCount = 0;
        bool indentFound = false;

        while (pos.getLineNumber() > 0)
        {
            pos = pos.movedByLines (-1);

            auto line = pos.getLineText();
            auto trimmedLine = line.trimStart();

            braceCount += getBraceCount (trimmedLine.getCharPointer());

            if (braceCount > 0)
            {
                blockIndent = getLeadingWhitespace (line);
                if (! indentFound)
                    lastLineIndent = blockIndent + tab;

                return true;
            }

            if ((! indentFound) && trimmedLine.isNotEmpty())
            {
                indentFound = true;
                lastLineIndent = getLeadingWhitespace (line);
            }
        }

        return false;
    }
}
