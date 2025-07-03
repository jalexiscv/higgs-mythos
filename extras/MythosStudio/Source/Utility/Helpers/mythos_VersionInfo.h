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
class VersionInfo
{
public:
    struct Asset
    {
        const String name;
        const String url;
    };

    static std::unique_ptr<VersionInfo> fetchFromUpdateServer (const String& versionString);
    static std::unique_ptr<VersionInfo> fetchLatestFromUpdateServer();
    static std::unique_ptr<InputStream> createInputStreamForAsset (const Asset& asset, int& statusCode);

    bool isNewerVersionThanCurrent();

    const String versionString;
    const String releaseNotes;
    const std::vector<Asset> assets;

private:
    VersionInfo (String version, String releaseNotes, std::vector<Asset> assets);

    static std::unique_ptr<VersionInfo> fetch (const String&);
};
