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

//==============================================================================
class ValueAnimator : public Animator::Impl
{
public:
    explicit ValueAnimator (ValueAnimatorBuilder optionsIn) : options (std::move (optionsIn)) {}

    auto getValue() const
    {
        using namespace detail::ArrayAndTupleOps;

        return options.getEasing() == nullptr ? getProgress() : options.getEasing() (getProgress());
    }

    float getProgress() const
    {
        if (isComplete())
            return 1.0f;

        return timeBasedProgress;
    }

    /** Returns the time in milliseconds that it takes for the progress to go from 0.0 to 1.0.

        This is the value returned even if the Animator is infinitely running.
    */
    double getDurationMs() const override
    {
        return options.getDurationMs();
    }

    bool isComplete() const override
    {
        return Animator::Impl::isComplete()
               || (! options.isInfinitelyRunning() && timeBasedProgress >= 1.0f);
    }

private:
    Animator::Status internalUpdate (double timestampMs) override
    {
        timeBasedProgress = (float) ((timestampMs - startedAtMs) / options.getDurationMs());

        NullCheckedInvocation::invoke (onValueChanged, getValue());

        if (! options.isInfinitelyRunning())
            return getProgress() >= 1.0 ? Animator::Status::finished : Animator::Status::inProgress;

        return Animator::Status::inProgress;
    }

    void onStart (double timeMs) override
    {
        startedAtMs = timeMs;
        timeBasedProgress = 0.0f;

        if (auto fn = options.getOnStartWithValueChanged())
            onValueChanged = fn();
    }

    void onComplete() override
    {
        NullCheckedInvocation::invoke (options.getOnComplete());
    }

    double startedAtMs = 0.0;
    float timeBasedProgress = 0.0f;

    const ValueAnimatorBuilder options;
    ValueAnimatorBuilder::ValueChangedCallback onValueChanged;
};

//==============================================================================
Animator ValueAnimatorBuilder::build() const&  { return Animator { std::make_unique<ValueAnimator> (*this) }; }
Animator ValueAnimatorBuilder::build() &&      { return Animator { std::make_unique<ValueAnimator> (std::move (*this)) }; }

} // namespace higgs
