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

#pragma once


//==============================================================================
const char* getPreferredLineFeed();
String joinLinesIntoSourceFile (StringArray& lines);
String replaceLineFeeds (const String& content, const String& lineFeed);
String getLineFeedForFile (const String& fileContent);

var parseHIGGSHeaderMetadata (const File&);

String trimCommentCharsFromStartOfLine (const String& line);

String createAlphaNumericUID();
String createGUID (const String& seed); // Turns a seed into a windows GUID

String escapeSpaces (const String& text); // replaces spaces with blackslash-space
String escapeQuotesAndSpaces (const String& text);
String addQuotesIfContainsSpaces (const String& text);

StringPairArray parsePreprocessorDefs (const String& defs);
StringPairArray mergePreprocessorDefs (StringPairArray inheritedDefs, const StringPairArray& overridingDefs);
String createGCCPreprocessorFlags (const StringPairArray& defs);

StringArray getCleanedStringArray (StringArray);
StringArray getSearchPathsFromString (const String& searchPath);
StringArray getCommaOrWhitespaceSeparatedItems (const String&);

void setValueIfVoid (Value value, const var& defaultValue);

bool fileNeedsCppSyntaxHighlighting (const File& file);

void writeAutoGenWarningComment (OutputStream& outStream);

StringArray getHIGGSModules() noexcept;
bool isHIGGSModule (const String& moduleID) noexcept;

StringArray getModulesRequiredForConsole() noexcept;
StringArray getModulesRequiredForComponent() noexcept;
StringArray getModulesRequiredForAudioProcessor() noexcept;

bool isPIPFile (const File&) noexcept;
int findBestLineToScrollToForClass (StringArray, const String&, bool isPlugin = false);

bool isValidHIGGSExamplesDirectory (const File&) noexcept;

bool isHIGGSModulesFolder (const File&);
bool isHIGGSFolder (const File&);

//==============================================================================
int indexOfLineStartingWith (const StringArray& lines, const String& text, int startIndex);

void autoScrollForMouseEvent (const MouseEvent& e, bool scrollX = true, bool scrollY = true);

//==============================================================================
struct PropertyListBuilder
{
    void add (PropertyComponent* propertyComp)
    {
        components.add (propertyComp);
    }

    void add (PropertyComponent* propertyComp, const String& tooltip)
    {
        propertyComp->setTooltip (tooltip);
        add (propertyComp);
    }

    void addSearchPathProperty (const Value& value,
                                const String& name,
                                const String& mainHelpText)
    {
        add (new TextPropertyComponent (value, name, 16384, true),
             mainHelpText + " Use semi-colons or new-lines to separate multiple paths.");
    }

    void addSearchPathProperty (const ValueTreePropertyWithDefault& value,
                                const String& name,
                                const String& mainHelpText)
    {
        add (new TextPropertyComponent (value, name, 16384, true),
             mainHelpText + " Use semi-colons or new-lines to separate multiple paths.");
    }

    void setPreferredHeight (int height)
    {
        for (int j = components.size(); --j >= 0;)
            components.getUnchecked (j)->setPreferredHeight (height);
    }

    Array<PropertyComponent*> components;
};

//==============================================================================
// A ValueSource which takes an input source, and forwards any changes in it.
// This class is a handy way to create sources which re-map a value.
class ValueSourceFilter : public Value::ValueSource,
                          private Value::Listener
{
public:
    ValueSourceFilter (const Value& source)  : sourceValue (source)
    {
        sourceValue.addListener (this);
    }

protected:
    Value sourceValue;

private:
    void valueChanged (Value&) override      { sendChangeMessage (true); }

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (ValueSourceFilter)
};
