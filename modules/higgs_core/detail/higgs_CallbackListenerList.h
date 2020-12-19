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

template <typename... Ts>
constexpr bool isValueOrLvalueReferenceToConst()
{
    return ((   (! std::is_reference_v<Ts>)
             || (std::is_lvalue_reference_v<Ts> && std::is_const_v<std::remove_reference_t<Ts>>)) && ...);
}

template <typename... Args>
class CallbackListenerList
{
public:
    static_assert (isValueOrLvalueReferenceToConst<Args...>(),
                   "CallbackListenerList can only forward values or const lvalue references");

    using Callback = std::function<void (Args...)>;

    ErasedScopeGuard addListener (Callback callback)
    {
        jassert (callback != nullptr);

        const auto it = callbacks.insert (callbacks.end(), std::move (callback));
        listeners.add (&*it);

        return ErasedScopeGuard { [this, it]
        {
            listeners.remove (&*it);
            callbacks.erase (it);
        } };
    }

    void call (Args... args)
    {
        listeners.call ([&] (auto& l) { l (std::forward<Args> (args)...); });
    }

private:
    std::list<Callback> callbacks;
    LightweightListenerList<Callback> listeners;
};

} // namespace higgs::detail
