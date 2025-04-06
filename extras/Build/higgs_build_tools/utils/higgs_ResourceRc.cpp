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

namespace higgs::build_tools
{

    static String getCommaSeparatedVersionNumber (const String& version)
    {
        auto versionParts = StringArray::fromTokens (version, ",.", "");
        versionParts.trim();
        versionParts.removeEmptyStrings();
        while (versionParts.size() < 4)
            versionParts.add ("0");

        return versionParts.joinIntoString (",");
    }

    void ResourceRcOptions::write (const File& resourceRcFile) const
    {
        MemoryOutputStream mo;

        mo << "#pragma code_page(65001)" << newLine
           << newLine
           << "#ifdef HIGGS_USER_DEFINED_RC_FILE" << newLine
           << " #include HIGGS_USER_DEFINED_RC_FILE" << newLine
           << "#else" << newLine
           << newLine
           << "#undef  WIN32_LEAN_AND_MEAN" << newLine
           << "#define WIN32_LEAN_AND_MEAN" << newLine
           << "#include <windows.h>" << newLine
           << newLine
           << "VS_VERSION_INFO VERSIONINFO" << newLine
           << "FILEVERSION  " << getCommaSeparatedVersionNumber (version) << newLine
           << "PRODUCTVERSION  " << getCommaSeparatedVersionNumber (version) << newLine
           << "BEGIN" << newLine
           << "  BLOCK \"StringFileInfo\"" << newLine
           << "  BEGIN" << newLine
           << "    BLOCK \"040904E4\"" << newLine
           << "    BEGIN" << newLine;

        const auto writeRCValue = [&] (const String& n, const String& value)
        {
            if (value.isNotEmpty())
                mo << "      VALUE \"" << n << "\",  \""
                   << value.replace ("\"", "\"\"") << "\\0\"" << newLine;
        };

        writeRCValue ("CompanyName",     companyName);
        writeRCValue ("LegalCopyright",  companyCopyright);
        writeRCValue ("FileDescription", projectName);
        writeRCValue ("FileVersion",     version);
        writeRCValue ("ProductName",     projectName);
        writeRCValue ("ProductVersion",  version);

        mo << "    END" << newLine
           << "  END" << newLine
           << newLine
           << "  BLOCK \"VarFileInfo\"" << newLine
           << "  BEGIN" << newLine
           << "    VALUE \"Translation\", 0x409, 1252" << newLine
           << "  END" << newLine
           << "END" << newLine
           << newLine
           << "#endif" << newLine;

        if (icon.existsAsFile())
            mo << newLine
               << "IDI_ICON1 ICON DISCARDABLE " << icon.getFileName().quoted()
               << newLine
               << "IDI_ICON2 ICON DISCARDABLE " << icon.getFileName().quoted();

        overwriteFileIfDifferentOrThrow (resourceRcFile, mo);
    }

} // namespace higgs::build_tools
