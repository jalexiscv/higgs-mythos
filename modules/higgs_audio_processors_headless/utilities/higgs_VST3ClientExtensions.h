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

/** @cond */

// Forward declaration to avoid leaking implementation details.
namespace Steinberg
{
    class FUnknown;
    using TUID = char[16];
} // namespace Steinberg

/** @endcond */

namespace higgs
{

/**
    An interface to allow an AudioProcessor to implement extended VST3-specific functionality.

    To use this class, create an object that inherits from it, implement the methods, then return
    a pointer to the object in your AudioProcessor::getVST3ClientExtensions() method.

    @see AudioProcessor, AAXClientExtensions, VST2ClientExtensions

    @tags{Audio}
*/
struct VST3ClientExtensions
{
    virtual ~VST3ClientExtensions() = default;

    /** This function may be used by implementations of queryInterface()
        in the VST3's implementation of IEditController to return
        additional supported interfaces.
    */
    virtual int32_t queryIEditController (const Steinberg::TUID, void** obj)
    {
        *obj = nullptr;
        return -1;
    }

    /** This function may be used by implementations of queryInterface()
        in the VST3's implementation of IAudioProcessor to return
        additional supported interfaces.
    */
    virtual int32_t queryIAudioProcessor (const Steinberg::TUID, void** obj)
    {
        *obj = nullptr;
        return -1;
    }

    /** This may be called by the VST3 wrapper when the host sets an
        IComponentHandler for the plugin to use.

        You should not make any assumptions about how and when this will be
        called - this function may not be called at all!
    */
    virtual void setIComponentHandler (Steinberg::FUnknown*) {}

    /** This may be called shortly after the AudioProcessor is constructed
        with the current IHostApplication.

        You should not make any assumptions about how and when this will be
        called - this function may not be called at all!
    */
    virtual void setIHostApplication  (Steinberg::FUnknown*) {}

    /** This function will be called to check whether the first input bus
        should be designated as "kMain" or "kAux". Return true if the
        first bus should be kMain, or false if the bus should be kAux.

        All other input buses will always be designated kAux.
    */
    virtual bool getPluginHasMainInput() const  { return true; }

    /** This function should return a map of VST3 parameter IDs and the HIGGS
        parameters they map to.

        This information is used to implement the IRemapParamID interface.
        Hosts can use this to preserve automation data when a session was saved
        using a compatible plugin that has different parameter IDs.

        Not all hosts will take this information into account. Therefore,
        parameter IDs should be maintained between plugin versions. For HIGGS
        plugins migrating from VST2 to VST3 the best method for achieving this
        is enabling HIGGS_FORCE_LEGACY_PARAM_IDS. However, if a plugin has
        already been released without enabling this flag, this method offers an
        alternative approach that won't cause any further compatibility issues.

        The key in the map is an integer which may represent a VST3 parameter
        identifier (Vst::ParamID) or VST2 parameter index.
        You should include a map entry for every parameter ID in the compatible
        plugin.
        For VST2 or HIGGS plugins these IDs can be determined in the following ways.
        - Use the parameter index if any of the following apply:
          - the InterfaceId argument refers to a compatible VST2 plugin, or
          - the InterfaceId argument refers to a HIGGS VST3 plugin with
            HIGGS_FORCE_LEGACY_PARAM_IDS enabled, or
          - the InterfaceId argument refers to a HIGGS plugin, but the parameter
            in the compatible plugin doesn't inherit from
            HostedAudioProcessorParameter (this case is unlikely).
        - Otherwise, use convertHiggsParameterId() for HIGGS VST3 plugins where
          HIGGS_FORCE_LEGACY_PARAM_IDS is disabled, and where the compatible
          parameter derives from HostedAudioProcessorParameter.
        - For non-HIGGS VST3s, use the Vst::ParamIDs exported by the compatible
          VST3.

        The value in the map is the HIGGS parameter ID for the parameter to map
        to, or an empty string to indicate that there is no parameter to map to.
        If a parameter doesn't inherit from HostedAudioProcessorParameter its ID
        will be the parameter index as a string, for example "1". Otherwise,
        always use the actual parameter ID (even if HIGGS_FORCE_LEGACY_PARAM_IDS
        is enabled).

        In the unlikely event that two plugins share the same plugin ID, and
        both have a different parameters that share the same parameter ID, it
        may be possible to determine which version of the plugin is being loaded
        during setStateInformation(). This method will always be called after
        setStateInformation(), so that the map with the correct mapping can be
        provided when queried.

        Below is an example of how you might implement this function for a HIGGS
        VST3 plugin where HIGGS_VST3_CAN_REPLACE_VST2 is enabled, but
        HIGGS_FORCE_LEGACY_PARAM_IDS is disabled.

        @code
        std::map<uint32_t, String> getCompatibleParameterIds (const InterfaceId&) const override
        {
            return { { 0, "Frequency" },
                     { 1, "CutOff" },
                     { 2, "Gain" },
                     { 3, "Bypass" } };
        }
        @endcode

        @param compatibleClass  A plugin identifier, either for the current
                                plugin or one listed in HIGGS_VST3_COMPATIBLE_CLASSES.
                                This parameter allows the implementation to
                                return a different parameter map for each
                                compatible class. Use VST3Interface::higgsPluginId()
                                and VST3Interface::vst2PluginId() to determine
                                the class IDs used by HIGGS plugins. When
                                HIGGS_VST3_CAN_REPLACE_VST2 is set, the ID
                                denoting the VST2 version of the plugin will
                                match the ID of the VST3 that replaces it. In
                                this case, assuming there are no collisions
                                between the VST2 parameter indices and the VST3
                                ParamIDs you should only include the VST2
                                mappings in the returned map. In the unlikely
                                event of a collision you should inspect the
                                state that was most recently passed to
                                setStateInformation() to determine whether the
                                host is loading a VST2 state that requires
                                parameter remapping. If you determine that no
                                remapping is necessary, you can indicate this by
                                returning an empty map.

        @returns    A map where each key is a VST3 parameter ID in the compatible
                    plugin, and the value is the unique HIGGS parameter ID in the
                    current plugin that it should be mapped to.

        @see HIGGS_VST3_COMPATIBLE_CLASSES, VST3Interface::higgsPluginId, VST3Interface::vst2PluginId, VST3Interface::hexStringToId
    */
    virtual std::map<uint32_t, String> getCompatibleParameterIds (const VST3Interface::Id& compatibleClass) const;


    /** Returns the VST3 compatible parameter ID reported for a given HIGGS
        parameter.

        Internally HIGGS will use this method to determine the Vst::ParamID for
        a HostedAudioProcessorParameter, unless HIGGS_FORCE_LEGACY_PARAM_IDS is
        enabled, in which case it will use the parameter index.

        @see getCompatibleParameterIds
    */
    static uint32_t convertHiggsParameterId (const String& parameterId,
                                            bool studioOneCompatible = true);

private:
    /** Instead of overriding this function you should define the preprocessor
        definition HIGGS_VST3_COMPATIBLE_CLASSES as described in the docs.

        @see HIGGS_VST3_COMPATIBLE_CLASSES
    */
    virtual std::vector<VST3Interface::Id> getCompatibleClasses() const final
    {
        return {};
    }
};

#if DOXYGEN
 /** An optional user defined preprocessor definition for declaring the VST3
     plugin identifier as used by the host.

     This is useful if you're creating a VST3 plugin using HIGGS that needs to
     replace a VST3 plugin that was not originally created using HIGGS.

     If there are multiple identifiers this plugin needs to replace take a look
     at HIGGS_VST3_COMPATIBLE_CLASSES. If required, both preprocessor definitions
     can be used in combination for maximum compatibility.

     The definition of this preprocessor must be defined at the project
     level, normally in your CMake or MythosStudio project files.

     If HIGGS_VST3_CAN_REPLACE_VST2 is enabled, the VST3 plugin will have the
     same identifier as the VST2 plugin and therefore this preprocessor
     definition will have no effect!

     This preprocessor definition should be defined as a string containing 32
     hex characters, for example...

     @code
     HIGGS_VST3_COMPONENT_CLASS="0F1E2D3C4B5A69788796A5B4C3D2E1F0"
     @endcode

     @see HIGGS_VST3_COMPONENT_CLASS
 */
 #define HIGGS_VST3_COMPONENT_CLASS

 /** An optional user defined preprocessor definition for declaring a comma
     separated list of VST2 and VST3 plugin identifiers that this VST3 plugin
     can replace in a DAW session.

     At the time of writing not all hosts support this feature, if you don't
     have multiple plugins with different identifiers already released, consider
     using HIGGS_VST3_COMPONENT_CLASS instead.

     The definition of this preprocessor must be defined at the project
     level, normally in your CMake or MythosStudio project files.

     This information will be used to implement the IPluginCompatibility
     interface.

     If HIGGS_VST3_CAN_REPLACE_VST2 is enabled, the VST3 plugin will have the
     same identifier as the VST2 plugin and therefore you don't need to
     implement this preprocessor definition.

     This preprocessor definition can contain code that depends on any class
     or function defined as part of the VST3Interface struct but should avoid
     any other dependencies!

     Each compatible class is a 16-byte array that corresponds to the VST3
     interface identifier as reported by a plugins IComponent interface.
     For VST2 or HIGGS plugins these identifiers can be determined in the
     following ways:
     - Use VST3Interface::vst2PluginId() for any VST2 plugins or HIGGS VST3
         plugin with HIGGS_VST3_CAN_REPLACE_VST3 enabled
     - Use VST3Interface::higgsPluginId() for any other HIGGS VST3 plugins

     Examples

     @code
     // Defines a VST2 plugin this VST3 can replace
     HIGGS_VST3_COMPATIBLE_CLASSES=VST3Interface::vst2PluginId ('Plug', "Plugin Name")

     // Defines a VST3 plugin this VST3 can replace
     HIGGS_VST3_COMPATIBLE_CLASSES=VST3Interface::higgsPluginId ('Manu', 'Plug')

     // Defines both a VST2 and a VST3 plugin this VST3 can replace
     HIGGS_VST3_COMPATIBLE_CLASSES=VST3Interface::vst2PluginId ('Plug', "Plugin Name"), VST3Interface::higgsPluginId ('Manu', 'Plug')

     // Defines a non-HIGGS VST3 plugin this VST3 can replace
     HIGGS_VST3_COMPATIBLE_CLASSES=VST3Interface::hexStringToId ("0F1E2D3C4B5A69788796A5B4C3D2E1F0")
     @endcode

     If the parameter IDs between compatible versions differ
     VST3ClientExtensions::getCompatibleParameterIds() should also be overridden.

     @see HIGGS_VST3_COMPONENT_CLASS, VST3Interface, VST3ClientExtensions::getCompatibleParameterIds()
 */
 #define HIGGS_VST3_COMPATIBLE_CLASSES
#endif // DOXYGEN

} // namespace higgs
