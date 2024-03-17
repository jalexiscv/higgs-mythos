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

/**
    An interface to allow an AudioProcessor to implement extended AAX-specific functionality.

    To use this class, create an object that inherits from it, implement the methods, then return
    a pointer to the object in your AudioProcessor `getAAXClientExtensions()` method.

    @see AudioProcessor, VST2ClientExtensions, VST3ClientExtensions

    @tags{Audio}
*/
struct HIGGS_API AAXClientExtensions
{
    virtual ~AAXClientExtensions() = default;

    /** AAX plug-ins need to report a unique "plug-in id" for every audio layout
        configuration that your AudioProcessor supports on the main bus. Override this
        function if you want your AudioProcessor to use a custom "plug-in id" (for example
        to stay backward compatible with older versions of HIGGS).

        The default implementation will compute a unique integer from the input and output
        layout and add this value to the 4 character code 'jcaa' (for native AAX) or 'jyaa'
        (for AudioSuite plug-ins).
    */
    virtual int32 getPluginIDForMainBusConfig (const AudioChannelSet& mainInputLayout,
                                               const AudioChannelSet& mainOutputLayout,
                                               bool idForAudioSuite) const;

    /** Returns an optional filename (including extension) for a page file to be used.

        A page file allows an AAX plugin to specify how its parameters are displayed on
        various control surfaces. For more information read the Page Table Guide in the
        AAX SDK documentation.

        By default this file will be searched for in `*.aaxplugin/Contents/Resources`.

        @see getPageFileSearchPath
    */
    virtual String getPageFileName() const;

    /** Optionally returns a search path for finding a page table file.

        This can be useful for specifying a location outside the plugin bundle so users can
        make changes to a page table file without breaking any code signatures.

        If this function returns a default-constructed File, then a default location will be used.
        The AAX SDK states this location will be `*.aaxplugin/Contents/Resources`.

        @note The returned path should be an absolute path to a directory.

        @see getPageFileName
    */
    virtual File getPageFileSearchPath() const { return {}; }
};

} // namespace higgs
