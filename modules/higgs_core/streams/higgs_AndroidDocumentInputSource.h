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
    An InputSource backed by an AndroidDocument.

    @see InputSource, AndroidDocument

    @tags{Core}
*/
class HIGGS_API  AndroidDocumentInputSource   : public InputSource
{
public:
    //==============================================================================
    /** Creates a new AndroidDocumentInputSource, backed by the provided document.
    */
    explicit AndroidDocumentInputSource (const AndroidDocument& doc)
        : document (doc) {}

    //==============================================================================
    /** Returns a new InputStream to read this item.

        @returns            an inputstream that the caller will delete, or nullptr if
                            the document can't be opened.
    */
    InputStream* createInputStream() override
    {
        return document.createInputStream().release();
    }

    /** @internal

        An AndroidDocument doesn't use conventional filesystem paths.
        Use the member functions of AndroidDocument to locate relative items.

        @param relatedItemPath  the relative pathname of the resource that is required
        @returns            an input stream if relatedItemPath was empty, otherwise
                            nullptr.
    */
    InputStream* createInputStreamFor (const String& relatedItemPath) override
    {
        return relatedItemPath.isEmpty() ? document.createInputStream().release() : nullptr;
    }

    /** Returns a hash code that uniquely represents this item.
    */
    int64 hashCode() const override
    {
        return document.getUrl().toString (true).hashCode64();
    }

private:
    AndroidDocument document;
};

} // namespace higgs
