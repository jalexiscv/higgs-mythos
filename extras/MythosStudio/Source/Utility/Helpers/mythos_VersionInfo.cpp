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
#include "mythos_VersionInfo.h"


VersionInfo::VersionInfo (String versionIn, String releaseNotesIn, std::vector<Asset> assetsIn)
    : versionString (std::move (versionIn)),
      releaseNotes (std::move (releaseNotesIn)),
      assets (std::move (assetsIn))
{}

std::unique_ptr<VersionInfo> VersionInfo::fetchFromUpdateServer (const String& versionString)
{
    return fetch ("tags/" + versionString);
}

std::unique_ptr<VersionInfo> VersionInfo::fetchLatestFromUpdateServer()
{
    return fetch ("latest");
}

std::unique_ptr<InputStream> VersionInfo::createInputStreamForAsset (const Asset& asset, int& statusCode)
{
    URL downloadUrl (asset.url);
    StringPairArray responseHeaders;

    return std::unique_ptr<InputStream> (downloadUrl.createInputStream (URL::InputStreamOptions (URL::ParameterHandling::inAddress)
                                                                          .withExtraHeaders ("Accept: application/octet-stream")
                                                                          .withConnectionTimeoutMs (5000)
                                                                          .withResponseHeaders (&responseHeaders)
                                                                          .withStatusCode (&statusCode)
                                                                          .withNumRedirectsToFollow (1)));
}

bool VersionInfo::isNewerVersionThanCurrent()
{
    jassert (versionString.isNotEmpty());

    auto currentTokens = StringArray::fromTokens (ProjectInfo::versionString, ".", {});
    auto thisTokens    = StringArray::fromTokens (versionString, ".", {});

    jassert (thisTokens.size() == 3);

    if (currentTokens[0].getIntValue() == thisTokens[0].getIntValue())
    {
        if (currentTokens[1].getIntValue() == thisTokens[1].getIntValue())
            return currentTokens[2].getIntValue() < thisTokens[2].getIntValue();

        return currentTokens[1].getIntValue() < thisTokens[1].getIntValue();
    }

    return currentTokens[0].getIntValue() < thisTokens[0].getIntValue();
}

std::unique_ptr<VersionInfo> VersionInfo::fetch (const String& endpoint)
{
    URL latestVersionURL ("https://api.github.com/repos/jalexiscv/HIGGS/releases/" + endpoint);

    std::unique_ptr<InputStream> inStream (latestVersionURL.createInputStream (URL::InputStreamOptions (URL::ParameterHandling::inAddress)
                                                                                 .withConnectionTimeoutMs (5000)));

    if (inStream == nullptr)
        return nullptr;

    auto content = inStream->readEntireStreamAsString();
    auto latestReleaseDetails = JSON::parse (content);

    auto* json = latestReleaseDetails.getDynamicObject();

    if (json == nullptr)
        return nullptr;

    auto versionString = json->getProperty ("tag_name").toString();

    if (versionString.isEmpty())
        return nullptr;

    auto* assets = json->getProperty ("assets").getArray();

    if (assets == nullptr)
        return nullptr;

    auto releaseNotes = json->getProperty ("body").toString();
    std::vector<VersionInfo::Asset> parsedAssets;

    for (auto& asset : *assets)
    {
        if (auto* assetJson = asset.getDynamicObject())
        {
            parsedAssets.push_back ({ assetJson->getProperty ("name").toString(),
                                      assetJson->getProperty ("url").toString() });
            jassert (parsedAssets.back().name.isNotEmpty());
            jassert (parsedAssets.back().url.isNotEmpty());
        }
        else
        {
            jassertfalse;
        }
    }

    return std::unique_ptr<VersionInfo> (new VersionInfo { versionString, releaseNotes, std::move (parsedAssets) });
}
