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
void AnimatorUpdater::addAnimator (const Animator& animator)
{
    addAnimator (animator, nullptr);
}

void AnimatorUpdater::addAnimator (const Animator& animator, std::function<void()> onComplete)
{
    Entry entry { animator.makeWeak(), std::move (onComplete) };
    animators[entry.animator.getKey()] = std::move (entry);
}

void AnimatorUpdater::removeAnimator (const Animator& animator)
{
    if (auto it = animators.find (animator.makeWeak().getKey()); it != animators.end())
    {
        if (it == currentIterator)
        {
            ++currentIterator;
            iteratorServiced = false;
        }

        animators.erase (it);
    }
}

void AnimatorUpdater::update()
{
    update (Time::getMillisecondCounterHiRes());
}

void AnimatorUpdater::update (double timestampMs)
{
    if (reentrancyGuard)
    {
        // If this is hit, one of the animators is trying to update itself
        // recursively. This is a bad idea! Inspect the callstack to find the
        // cause of the problem.
        jassertfalse;
        return;
    }

    const ScopedValueSetter setter { reentrancyGuard, true };

    for (currentIterator = animators.begin(); currentIterator != animators.end();)
    {
        auto& current = *currentIterator;

        if (const auto locked = current.second.animator.lock())
        {
            iteratorServiced = true;

            if (locked->update (timestampMs) == Animator::Status::finished)
                NullCheckedInvocation::invoke (current.second.onComplete);

            if (iteratorServiced && currentIterator != animators.end())
                ++currentIterator;
        }
        else
        {
            currentIterator = animators.erase (currentIterator);
        }
    }
}

} // namespace higgs
