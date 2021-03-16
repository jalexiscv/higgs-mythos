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
    A type of InputSource that represents a URL.

    @see InputSource

    @tags{Core}
*/
class HIGGS_API  URLInputSource     : public InputSource
{
public:
    //==============================================================================
    /** Creates a URLInputSource for a url. */
    URLInputSource (const URL& url);

    /** Move constructor which will move the URL into the InputSource.

        This is useful when the url carries any security credentials.
    */
    URLInputSource (URL&& url);

    /** Destructor. */
    ~URLInputSource() override;

    InputStream* createInputStream() override;
    InputStream* createInputStreamFor (const String& relatedItemPath) override;
    int64 hashCode() const override;

private:
    //==============================================================================
    const URL u;

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (URLInputSource)
};

}
