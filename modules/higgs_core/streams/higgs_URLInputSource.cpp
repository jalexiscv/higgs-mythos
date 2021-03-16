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

URLInputSource::URLInputSource (const URL& url)
    : u (url)
{
}

URLInputSource::URLInputSource (URL&& url)
    : u (std::move (url))
{
}

URLInputSource::~URLInputSource()
{
}

InputStream* URLInputSource::createInputStream()
{
    return u.createInputStream (URL::InputStreamOptions (URL::ParameterHandling::inAddress)).release();
}

InputStream* URLInputSource::createInputStreamFor (const String& relatedItemPath)
{
    auto sub = u.getSubPath();
    auto parent = sub.containsChar (L'/') ? sub.upToLastOccurrenceOf ("/", false, false)
                                          : String();

    return u.withNewSubPath (parent)
            .getChildURL (relatedItemPath)
            .createInputStream (URL::InputStreamOptions (URL::ParameterHandling::inAddress))
            .release();
}

int64 URLInputSource::hashCode() const
{
    return u.toString (true).hashCode64();
}

} // namespace higgs
