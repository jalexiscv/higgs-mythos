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
class ModuleDescription
{
public:
    ModuleDescription() = default;

    ModuleDescription (const File& folder)
       : moduleFolder (folder),
         moduleInfo (parseHIGGSHeaderMetadata (getHeader()))
    {
    }

    bool isValid() const                    { return getID().isNotEmpty(); }

    String getID() const                    { return moduleInfo [Ids::ID_uppercase].toString(); }
    String getVendor() const                { return moduleInfo [Ids::vendor].toString(); }
    String getVersion() const               { return moduleInfo [Ids::version].toString(); }
    String getName() const                  { return moduleInfo [Ids::name].toString(); }
    String getDescription() const           { return moduleInfo [Ids::description].toString(); }
    String getLicense() const               { return moduleInfo [Ids::license].toString(); }
    String getMinimumCppStandard() const    { return moduleInfo [Ids::minimumCppStandard].toString(); }
    String getPreprocessorDefs() const      { return moduleInfo [Ids::defines].toString(); }
    String getExtraSearchPaths() const      { return moduleInfo [Ids::searchpaths].toString(); }
    var getModuleInfo() const               { return moduleInfo; }
    File getModuleFolder() const            { return moduleFolder; }

    File getFolder() const
    {
        jassert (moduleFolder != File());

        return moduleFolder;
    }

    File getHeader() const
    {
        if (moduleFolder != File())
        {
            static const char* extensions[] = { ".h", ".hpp", ".hxx" };

            for (auto e : extensions)
            {
                auto header = moduleFolder.getChildFile (moduleFolder.getFileName() + e);

                if (header.existsAsFile())
                    return header;
            }
        }

        return {};
    }

    StringArray getDependencies() const
    {
        auto moduleDependencies = StringArray::fromTokens (moduleInfo ["dependencies"].toString(), " \t;,", "\"'");
        moduleDependencies.trim();
        moduleDependencies.removeEmptyStrings();

        return moduleDependencies;
    }

private:
    File moduleFolder;
    var moduleInfo;
    URL url;
};
