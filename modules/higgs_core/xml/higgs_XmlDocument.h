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

//==============================================================================
/**
    Parses a text-based XML document and creates an XmlElement object from it.

    The parser will expand entities that are defined in the document's DTD, but won't
    check the document for validity against the DTD. External DTD subsets and external
    parameter entities are only loaded through an input source supplied with
    setInputSource(). Documents whose elements are nested more than 256 levels deep, or
    whose entities are nested more than 40 levels deep or expand to many times the size
    of the document, are rejected as malformed.

    e.g.
    @code
    XmlDocument myDocument (File ("myfile.xml"));

    if (auto mainElement = myDocument.getDocumentElement())
    {
        use the element
    }
    else
    {
        String error = myDocument.getLastParseError();
    }
    @endcode

    Or you can use the helper functions for much less verbose parsing.

    @code
    if (auto xml = parseXML (myXmlFile))
    {
        if (xml->hasTagName ("foobar"))
        {
            ...etc
        }
    }
    @endcode

    @see XmlElement

    @tags{Core}
*/
class HIGGS_API  XmlDocument
{
public:
    //==============================================================================
    /** Creates an XmlDocument from the xml text.
        The text doesn't actually get parsed until the getDocumentElement() method is called.
    */
    XmlDocument (const String& documentText);

    /** Creates an XmlDocument from a file.
        The text doesn't actually get parsed until the getDocumentElement() method is called.
    */
    XmlDocument (const File& file);

    /** Destructor. */
    ~XmlDocument();

    //==============================================================================
    /** Creates an XmlElement object to represent the main document node.

        This method will do the actual parsing of the text, and if there's a
        parse error, it may returns nullptr (and you can find out the error using
        the getLastParseError() method).

        See also the parse() methods, which provide a shorthand way to quickly
        parse a file or string.

        @param onlyReadOuterDocumentElement     if true, the parser will only read the
                                                first section of the file, and will only
                                                return the outer document element - this
                                                allows quick checking of large files to
                                                see if they contain the correct type of
                                                tag, without having to parse the entire file
        @returns    a new XmlElement, or nullptr if there was an error.
        @see getLastParseError, getDocumentElementIfTagMatches
    */
    std::unique_ptr<XmlElement> getDocumentElement (bool onlyReadOuterDocumentElement = false);

    /** Does an inexpensive check to see whether the outer element has the given tag name, and
        then does a full parse if it matches.
        If the tag is different, or the XML parse fails, this will return nullptr.
    */
    std::unique_ptr<XmlElement> getDocumentElementIfTagMatches (StringRef requiredTag);

    /** Returns the parsing error that occurred the last time getDocumentElement was called.
        @returns the error, or an empty string if there was no error.
    */
    const String& getLastParseError() const noexcept;

    /** Sets an input source object to use for reading the document and any external
        entities that it references.

        Supplying an input source is the only way to enable the loading of external DTD
        subsets and external parameter entities; a document created from a file or a string
        will not load them on its own. A document can name any file or URL as an external
        entity, so an input source used with untrusted documents should restrict what its
        createInputStreamFor() method will open.

        The input source is also used to read the document itself if the document was
        constructed from a file or from an empty string, as any text passed to the
        constructor takes precedence over it.

        The object that is passed-in will be deleted automatically when no longer needed.

        @see InputSource
    */
    void setInputSource (InputSource* newSource) noexcept;

    /** Sets a flag to change the treatment of empty text elements.

        If this is true (the default state), then any text elements that contain only
        whitespace characters will be ignored during parsing. If you need to catch
        whitespace-only text, then you should set this to false before calling the
        getDocumentElement() method.
    */
    void setEmptyTextElementsIgnored (bool shouldBeIgnored) noexcept;

    //==============================================================================
    /** A handy static method that parses a file.
        This is a shortcut for creating an XmlDocument object and calling getDocumentElement() on it.
        @returns    a new XmlElement, or nullptr if there was an error.
    */
    static std::unique_ptr<XmlElement> parse (const File& file);

    /** A handy static method that parses some XML data.
        This is a shortcut for creating an XmlDocument object and calling getDocumentElement() on it.
        @returns    a new XmlElement, or nullptr if there was an error.
    */
    static std::unique_ptr<XmlElement> parse (const String& xmlData);


    //==============================================================================
private:
    String originalText;
    String::CharPointerType input { nullptr };
    bool outOfData = false, errorOccurred = false;
    String lastError, dtdText;
    StringArray tokenisedDTD, entitiesBeingExpanded;
    bool needToLoadDTD = false, ignoreEmptyTextElements = true, externalEntitiesAllowed = false;
    int elementNestingDepth = 0;
    int64 inputLength = 0, expandedEntityLength = 0;
    std::unique_ptr<InputSource> inputSource;

    std::unique_ptr<XmlElement> parseDocumentElement (String::CharPointerType, bool outer);
    void setLastError (const String&, bool carryOn);
    bool parseHeader();
    bool parseDTD();
    void skipNextWhiteSpace();
    higgs_wchar readNextChar() noexcept;
    XmlElement* readNextElement (bool alsoParseSubElements);
    void readChildElements (XmlElement&);
    void readQuotedString (String&);
    String readEntity (String&);

    String getFileContents (const String&) const;
    String expandEntity (const String&);
    String expandExternalEntity (const String&);
    void checkEntityExpansionLimit (const String&);
    String getParameterEntity (const String&);

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (XmlDocument)
};

//==============================================================================
/** Attempts to parse some XML text, returning a new XmlElement if it was valid.
    If the parse fails, this will return a nullptr - if you need more information about
    errors or more parsing options, see the XmlDocument class instead.
    @see XmlDocument, parseXMLIfTagMatches
*/
std::unique_ptr<XmlElement> parseXML (const String& textToParse);

/** Attempts to parse some XML text, returning a new XmlElement if it was valid.
    If the parse fails, this will return a nullptr - if you need more information about
    errors or more parsing options, see the XmlDocument class instead.
    @see XmlDocument, parseXMLIfTagMatches
*/
std::unique_ptr<XmlElement> parseXML (const File& fileToParse);

/** Does an inexpensive check to see whether the top-level element has the given tag
    name, and if that's true, does a full parse and returns the result.
    If the outer tag doesn't match, or the XML has errors, this will return nullptr;
    @see parseXML
*/
std::unique_ptr<XmlElement> parseXMLIfTagMatches (const String& textToParse, StringRef requiredTag);

/** Does an inexpensive check to see whether the top-level element has the given tag
    name, and if that's true, does a full parse and returns the result.
    If the outer tag doesn't match, or the XML has errors, this will return nullptr;
    @see parseXML
*/
std::unique_ptr<XmlElement> parseXMLIfTagMatches (const File& fileToParse, StringRef requiredTag);


} // namespace higgs
