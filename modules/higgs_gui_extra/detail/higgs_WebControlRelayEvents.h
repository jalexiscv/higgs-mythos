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

namespace higgs::detail
{

#if HIGGS_WEB_BROWSER || DOXYGEN

struct WebSliderRelayEvents
{
    WebSliderRelayEvents() = delete;

    struct Event
    {
        String eventType;
        DynamicObject::Ptr object;

        static std::optional<Event> extract (const var& v)
        {
            auto* dynObj = v.getDynamicObject();

            if (dynObj == nullptr)
                return std::nullopt;

            const auto eventTypeProp = dynObj->getProperty (eventTypeKey);

            if (! eventTypeProp.isString())
                return std::nullopt;

            return Event { eventTypeProp.toString(), dynObj };
        }

        static inline const Identifier eventTypeKey { "eventType" };
    };

    struct ValueChanged
    {
        float newValue;

        static std::optional<ValueChanged> extract (const Event& event)
        {
            if (event.eventType != eventId.toString())
                return std::nullopt;

            const auto newValue = event.object->getProperty (newValueKey);

            if (! (newValue.isInt() || newValue.isInt64() || newValue.isDouble()))
                return std::nullopt;

            return ValueChanged { (float) newValue };
        }

        static inline const Identifier eventId { "valueChanged" };
        static inline const Identifier newValueKey { "value" };
    };

    struct SliderDragStarted
    {
        static std::optional<SliderDragStarted> extract (const Event& event)
        {
            if (event.eventType != eventId.toString())
                return std::nullopt;

            return SliderDragStarted{};
        }

        static inline const Identifier eventId { "sliderDragStarted" };
    };

    struct SliderDragEnded
    {
        static std::optional<SliderDragEnded> extract (const Event& event)
        {
            if (event.eventType != eventId.toString())
                return std::nullopt;

            return SliderDragEnded{};
        }

        static inline const Identifier eventId { "sliderDragEnded" };
    };

    struct InitialUpdateRequested
    {
        static std::optional<InitialUpdateRequested> extract (const Event& event)
        {
            if (event.eventType != eventId.toString())
                return std::nullopt;

            return InitialUpdateRequested{};
        }

        static inline const Identifier eventId { "requestInitialUpdate" };
    };
};

//==============================================================================
struct WebToggleButtonRelayEvents
{
    WebToggleButtonRelayEvents() = delete;

    struct Event
    {
        String eventType;
        DynamicObject::Ptr object;

        static std::optional<Event> extract (const var& v)
        {
            auto* dynObj = v.getDynamicObject();

            if (dynObj == nullptr)
                return std::nullopt;

            const auto eventTypeProp = dynObj->getProperty (eventTypeKey);

            if (! eventTypeProp.isString())
                return std::nullopt;

            return Event { eventTypeProp.toString(), dynObj };
        }

        static inline const Identifier eventTypeKey { "eventType" };
    };

    struct ToggleStateChanged
    {
        bool value;

        static std::optional<ToggleStateChanged> extract (const Event& event)
        {
            if (event.eventType != eventId.toString())
                return std::nullopt;

            const auto newState = event.object->getProperty (valueKey);

            if (! newState.isBool())
                return std::nullopt;

            return ToggleStateChanged { newState };
        }

        static inline const Identifier eventId  { "valueChanged" };
        static inline const Identifier valueKey { "value" };
    };

    struct InitialUpdateRequested
    {
        static std::optional<InitialUpdateRequested> extract (const Event& event)
        {
            if (event.eventType != eventId.toString())
                return std::nullopt;

            return InitialUpdateRequested{};
        }

        static inline const Identifier eventId { "requestInitialUpdate" };
    };
};

//==============================================================================
struct WebComboBoxRelayEvents
{
    WebComboBoxRelayEvents() = delete;

    struct Event
    {
        String eventType;
        DynamicObject::Ptr object;

        static std::optional<Event> extract (const var& v)
        {
            auto* dynObj = v.getDynamicObject();

            if (dynObj == nullptr)
                return std::nullopt;

            const auto eventTypeProp = dynObj->getProperty (eventTypeKey);

            if (! eventTypeProp.isString())
                return std::nullopt;

            return Event { eventTypeProp.toString(), dynObj };
        }

        static inline const Identifier eventTypeKey { "eventType" };
    };

    struct ValueChanged
    {
        float value;

        static std::optional<ValueChanged> extract (const Event& event)
        {
            if (event.eventType != eventId.toString())
                return std::nullopt;

            const auto newValue = event.object->getProperty (valueKey);

            if (! (newValue.isInt() || newValue.isInt64() || newValue.isDouble()))
                return std::nullopt;

            return ValueChanged { (float) newValue };
        }

        static inline const Identifier eventId  { "valueChanged" };
        static inline const Identifier valueKey { "value" };
    };

    struct InitialUpdateRequested
    {
        static std::optional<InitialUpdateRequested> extract (const Event& event)
        {
            if (event.eventType != eventId.toString())
                return std::nullopt;

            return InitialUpdateRequested{};
        }

        static inline const Identifier eventId { "requestInitialUpdate" };
    };
};

#endif
}
