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

namespace higgs::dsp
{

/** @cond */
//==============================================================================
/** The contents of this namespace are used to implement ProcessorChain and should
    not be used elsewhere. Their interfaces (and existence) are liable to change!
*/
namespace detail
{
    template <typename Fn, typename Tuple, size_t... Ix>
    constexpr void forEachInTuple (Fn&& fn, Tuple&& tuple, std::index_sequence<Ix...>)
    {
        (fn (std::get<Ix> (tuple), std::integral_constant<size_t, Ix>()), ...);
    }

    template <typename T>
    using TupleIndexSequence = std::make_index_sequence<std::tuple_size_v<std::remove_cv_t<std::remove_reference_t<T>>>>;

    template <typename Fn, typename Tuple>
    constexpr void forEachInTuple (Fn&& fn, Tuple&& tuple)
    {
        forEachInTuple (std::forward<Fn> (fn), std::forward<Tuple> (tuple), TupleIndexSequence<Tuple>{});
    }

    template <typename Context, size_t Ix>
    inline constexpr auto useContextDirectly = ! Context::usesSeparateInputAndOutputBlocks() || Ix == 0;
}
/** @endcond */

/** This variadically-templated class lets you join together any number of processor
    classes into a single processor which will call process() on them all in sequence.

    @tags{DSP}
*/
template <typename... Processors>
class ProcessorChain
{
public:
    /** Get a reference to the processor at index `Index`. */
    template <int Index>       auto& get()       noexcept { return std::get<Index> (processors); }

    /** Get a reference to the processor at index `Index`. */
    template <int Index> const auto& get() const noexcept { return std::get<Index> (processors); }

    /** Set the processor at index `Index` to be bypassed or enabled. */
    template <int Index>
    void setBypassed (bool b) noexcept  { bypassed[(size_t) Index] = b; }

    /** Query whether the processor at index `Index` is bypassed. */
    template <int Index>
    bool isBypassed() const noexcept    { return bypassed[(size_t) Index]; }

    /** Prepare all inner processors with the provided `ProcessSpec`. */
    void prepare (const ProcessSpec& spec)
    {
        detail::forEachInTuple ([&] (auto& proc, auto) { proc.prepare (spec); }, processors);
    }

    /** Reset all inner processors. */
    void reset()
    {
        detail::forEachInTuple ([] (auto& proc, auto) { proc.reset(); }, processors);
    }

    /** Process `context` through all inner processors in sequence. */
    template <typename ProcessContext>
    void process (const ProcessContext& context) noexcept
    {
        detail::forEachInTuple ([this, &context] (auto& proc, auto index) noexcept { this->processOne (context, proc, index); },
                                processors);
    }

private:
    template <typename Context, typename Proc, size_t Ix>
    void processOne (const Context& context, Proc& proc, std::integral_constant<size_t, Ix>) noexcept
    {
        if constexpr (detail::useContextDirectly<Context, Ix>)
        {
            auto contextCopy = context;
            contextCopy.isBypassed = (bypassed[Ix] || context.isBypassed);

            proc.process (contextCopy);
        }
        else
        {
            jassert (context.getOutputBlock().getNumChannels() == context.getInputBlock().getNumChannels());
            ProcessContextReplacing<typename Context::SampleType> replacingContext (context.getOutputBlock());
            replacingContext.isBypassed = (bypassed[Ix] || context.isBypassed);

            proc.process (replacingContext);
        }
    }

    std::tuple<Processors...> processors;
    std::array<bool, sizeof... (Processors)> bypassed { {} };
};

/** Non-member equivalent of ProcessorChain::get which avoids awkward
    member template syntax.
*/
template <int Index, typename... Processors>
inline auto& get (ProcessorChain<Processors...>& chain) noexcept
{
    return chain.template get<Index>();
}

/** Non-member equivalent of ProcessorChain::get which avoids awkward
    member template syntax.
*/
template <int Index, typename... Processors>
inline auto& get (const ProcessorChain<Processors...>& chain) noexcept
{
    return chain.template get<Index>();
}

/** Non-member equivalent of ProcessorChain::setBypassed which avoids awkward
    member template syntax.
*/
template <int Index, typename... Processors>
inline void setBypassed (ProcessorChain<Processors...>& chain, bool bypassed) noexcept
{
    chain.template setBypassed<Index> (bypassed);
}

/** Non-member equivalent of ProcessorChain::isBypassed which avoids awkward
    member template syntax.
*/
template <int Index, typename... Processors>
inline bool isBypassed (const ProcessorChain<Processors...>& chain) noexcept
{
    return chain.template isBypassed<Index>();
}

} // namespace higgs::dsp

/** @cond */
namespace std
{

HIGGS_BEGIN_IGNORE_WARNINGS_GCC_LIKE ("-Wmismatched-tags")

/** Adds support for C++17 structured bindings. */
template <typename... Processors>
struct tuple_size<::higgs::dsp::ProcessorChain<Processors...>> : integral_constant<size_t, sizeof... (Processors)> {};

/** Adds support for C++17 structured bindings. */
template <size_t I, typename... Processors>
struct tuple_element<I, ::higgs::dsp::ProcessorChain<Processors...>> : tuple_element<I, tuple<Processors...>> {};

HIGGS_END_IGNORE_WARNINGS_GCC_LIKE

} // namespace std
/** @endcond */
