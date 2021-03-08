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

WebInputStream::WebInputStream (const URL& url, const bool usePost)
    : pimpl (std::make_unique<Pimpl> (*this, url, usePost))
{
}

WebInputStream::~WebInputStream()
{
}

WebInputStream& WebInputStream::withExtraHeaders (const String& extra)         { pimpl->withExtraHeaders (extra);       return *this; }
WebInputStream& WebInputStream::withCustomRequestCommand (const String& cmd)   { pimpl->withCustomRequestCommand (cmd); return *this; }
WebInputStream& WebInputStream::withConnectionTimeout (int t)                  { pimpl->withConnectionTimeout (t);      return *this; }
WebInputStream& WebInputStream::withNumRedirectsToFollow (int num)             { pimpl->withNumRedirectsToFollow (num); return *this; }
StringPairArray WebInputStream::getRequestHeaders() const                      { return pimpl->getRequestHeaders(); }
StringPairArray WebInputStream::getResponseHeaders()                           { connect (nullptr); return pimpl->getResponseHeaders(); }
bool WebInputStream::isError() const                                           { return pimpl->isError(); }
void WebInputStream::cancel()                                                  { pimpl->cancel(); }
bool WebInputStream::isExhausted()                                             { return pimpl->isExhausted(); }
int64 WebInputStream::getPosition()                                            { return pimpl->getPosition(); }
int64 WebInputStream::getTotalLength()                                         { connect (nullptr); return pimpl->getTotalLength(); }
int WebInputStream::read (void* buffer, int bytes)                             { connect (nullptr); return pimpl->read (buffer, bytes); }
bool WebInputStream::setPosition (int64 pos)                                   { return pimpl->setPosition (pos); }
int WebInputStream::getStatusCode()                                            { connect (nullptr); return pimpl->getStatusCode(); }

bool WebInputStream::connect (Listener* listener)
{
    if (hasCalledConnect)
        return ! isError();

    hasCalledConnect = true;
    return pimpl->connect (listener);
}

StringPairArray WebInputStream::parseHttpHeaders (const String& headerData)
{
    StringPairArray headerPairs;
    auto headerLines = StringArray::fromLines (headerData);

    for (const auto& headersEntry : headerLines)
    {
        if (headersEntry.isNotEmpty())
        {
            const auto key = headersEntry.upToFirstOccurrenceOf (": ", false, false);

            auto value = [&headersEntry, &headerPairs, &key]
            {
                const auto currentValue = headersEntry.fromFirstOccurrenceOf (": ", false, false);
                const auto previousValue = headerPairs [key];

                if (previousValue.isNotEmpty())
                    return previousValue + "," + currentValue;

                return currentValue;
            }();

            headerPairs.set (key, value);
        }
    }

    return headerPairs;
}

void WebInputStream::createHeadersAndPostData (const URL& aURL,
                                               String& headers,
                                               MemoryBlock& data,
                                               bool addParametersToBody)
{
    aURL.createHeadersAndPostData (headers, data, addParametersToBody);
}

bool WebInputStream::Listener::postDataSendProgress ([[maybe_unused]] WebInputStream& request,
                                                     [[maybe_unused]] int bytesSent,
                                                     [[maybe_unused]] int totalBytes)
{
    return true;
}

} // namespace higgs
