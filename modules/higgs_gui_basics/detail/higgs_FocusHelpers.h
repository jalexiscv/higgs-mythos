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

struct FocusHelpers
{
    FocusHelpers() = delete;

    static int getOrder (const Component* c)
    {
        auto order = c->getExplicitFocusOrder();
        return order > 0 ? order : std::numeric_limits<int>::max();
    }

    static void findAllComponents (const Component* parent,
                                   std::vector<Component*>& components,
                                   bool (Component::* isFocusContainer)() const,
                                   FocusTraverser::SkipDisabledComponents skipDisabledComponents)
    {
        if (parent == nullptr || parent->getNumChildComponents() == 0)
            return;

        std::vector<Component*> localComponents;

        for (auto* c : parent->getChildren())
        {
            constexpr auto no = FocusTraverser::SkipDisabledComponents::no;

            if (c->isVisible() && (c->isEnabled() || skipDisabledComponents == no))
                localComponents.push_back (c);
        }

        const auto compareComponents = [&] (const Component* a, const Component* b)
        {
            const auto getComponentOrderAttributes = [] (const Component* c)
            {
                return std::make_tuple (getOrder (c),
                                        c->isAlwaysOnTop() ? 0 : 1,
                                        c->getY(),
                                        c->getX());
            };

            return getComponentOrderAttributes (a) < getComponentOrderAttributes (b);
        };

        // This will sort so that they are ordered in terms of explicit focus,
        // always on top, left-to-right, and then top-to-bottom.
        std::stable_sort (localComponents.begin(), localComponents.end(), compareComponents);

        for (auto* c : localComponents)
        {
            components.push_back (c);

            if (! (c->*isFocusContainer)())
                findAllComponents (c, components, isFocusContainer, skipDisabledComponents);
        }
    }

    enum class NavigationDirection { forwards, backwards };

    static Component* navigateFocus (const Component* current,
                                     const Component* focusContainer,
                                     NavigationDirection direction,
                                     bool (Component::* isFocusContainer)() const,
                                     FocusTraverser::SkipDisabledComponents skipDisabledComponents)
    {
        if (focusContainer == nullptr)
            return nullptr;

        std::vector<Component*> components;
        findAllComponents (focusContainer, components, isFocusContainer, skipDisabledComponents);

        const auto iter = std::find (components.cbegin(), components.cend(), current);

        if (iter == components.cend())
            return nullptr;

        switch (direction)
        {
            case NavigationDirection::forwards:
                if (iter != std::prev (components.cend()))
                    return *std::next (iter);

                break;

            case NavigationDirection::backwards:
                if (iter != components.cbegin())
                    return *std::prev (iter);

                break;
        }

        return nullptr;
    }
};

} // namespace higgs::detail
