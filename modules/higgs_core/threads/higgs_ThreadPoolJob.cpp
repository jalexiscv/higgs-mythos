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

ThreadPoolJob::ThreadPoolJob (const String& name)  : jobName (name)
{
}

ThreadPoolJob::~ThreadPoolJob()
{
    // you mustn't delete a job while it's still in a pool! Use ThreadPool::removeJob()
    // to remove it first!
    jassert (pool == nullptr || ! pool->contains (this));
}

String ThreadPoolJob::getJobName() const
{
    return jobName;
}

void ThreadPoolJob::setJobName (const String& newName)
{
    jobName = newName;
}

void ThreadPoolJob::signalJobShouldExit()
{
    shouldStop = true;
    listeners.call ([] (Thread::Listener& l) { l.exitSignalSent(); });
}

void ThreadPoolJob::addListener (Thread::Listener* listener)
{
    listeners.add (listener);
}

void ThreadPoolJob::removeListener (Thread::Listener* listener)
{
    listeners.remove (listener);
}

ThreadPoolJob* ThreadPoolJob::getCurrentThreadPoolJob()
{
    if (auto* t = dynamic_cast<ThreadPool::ThreadPoolThread*> (Thread::getCurrentThread()))
        return t->currentJob.load();

    return nullptr;
}

} // namespace higgs
