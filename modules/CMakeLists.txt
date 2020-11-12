# ==============================================================================
#
#  This file is part of the Higgs Mythos framework.
#  Copyright (c) Raw Material Software Limited
#
#  Higgs Mythos is free software: you can redistribute it and/or modify
#  it under the terms of the GNU Affero General Public License as published
#  by the Free Software Foundation, either version 3 of the License, or
#  (at your option) any later version.
#
#  Higgs Mythos is distributed in the hope that it will be useful, but
#  WITHOUT ANY WARRANTY; without even the implied warranty of
#  MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE. See the
#  GNU Affero General Public License for more details.
#
#  You should have received a copy of the GNU Affero General Public License
#  along with Higgs Mythos. If not, see <https://www.gnu.org/licenses/>.
#
# ==============================================================================

higgs_add_modules(
    INSTALL_PATH "include/HIGGS-${HIGGS_VERSION}/modules"
    ALIAS_NAMESPACE higgs
    higgs_analytics
    higgs_animation
    higgs_audio_basics
    higgs_audio_devices
    higgs_audio_formats
    higgs_audio_plugin_client
    higgs_audio_processors
    higgs_audio_processors_headless
    higgs_audio_utils
    higgs_box2d
    higgs_core
    higgs_cryptography
    higgs_data_structures
    higgs_dsp
    higgs_events
    higgs_graphics
    higgs_gui_basics
    higgs_gui_extra
    higgs_javascript
    higgs_midi_ci
    higgs_opengl
    higgs_osc
    higgs_product_unlocking
    higgs_video)
