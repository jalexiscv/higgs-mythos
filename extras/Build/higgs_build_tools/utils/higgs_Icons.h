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

    class Icons
    {
    public:
        Icons() = default;

        static Icons fromFilesSmallAndBig (const File& small, const File& big);

        const Drawable* getSmall() const { return small.get(); }
        const Drawable* getBig()   const { return big.get(); }

    private:
        std::unique_ptr<Drawable> small;
        std::unique_ptr<Drawable> big;
    };

    Array<const Drawable*> asArray (const Icons&);
    void writeMacIcon (const Icons&, const File&);
    void writeWinIcon (const Icons&, const File&);

    Image getBestIconForSize (const Icons& icons,
                              int size,
                              bool returnNullIfNothingBigEnough);
    Image rescaleImageForIcon (const Drawable& d, int size);

    RelativePath createXcassetsFolderFromIcons (const Icons& icons,
                                                const File& targetFolder,
                                                String projectFilenameRootString,
                                                String iconName);

} // namespace higgs::build_tools
