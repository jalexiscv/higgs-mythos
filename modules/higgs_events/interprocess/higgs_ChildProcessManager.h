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
    /** Manages a set of ChildProcesses and periodically checks their return value. Upon completion
        it calls listeners added with addChildProcessExitedListener().

        This class is mostly aimed for usage on Linux, where terminated child processes are only
        cleaned up if their return code is read after termination. In order to ensure this one needs
        to call ChildProcess::isFinished() until it returns false or
        ChildProcess::waitForProcessToFinish() until it returns true.

        This class will keep querying the return code on a Timer thread until the process
        terminates. This can be handy if one wants to start and stop multiple ChildProcesses on
        Linux that could take a long time to complete.

        Since this class uses a Timer to check subprocess status, it's generally only safe to
        access the returned ChildProcesses from the message thread.

        @see ChildProcessManagerSingleton

        @tags{Events}
    */
    class HIGGS_API  ChildProcessManager final : private DeletedAtShutdown
    {
    public:
        /** @cond */
        HIGGS_DECLARE_SINGLETON_SINGLETHREADED_MINIMAL_INLINE (ChildProcessManager)
        /** @endcond */

        /** Creates a new ChildProcess and starts it with the provided arguments.

            The arguments are the same as the overloads to ChildProcess::start().

            The manager will keep the returned ChildProcess object alive until it terminates and its
            return value has been queried. Calling ChildProcess::kill() on the returned object will
            eventually cause its removal from the ChildProcessManager after it terminates.
        */
        template <typename... Args>
        std::shared_ptr<ChildProcess> createAndStartManagedChildProcess (Args&&... args)
        {
            auto p = std::make_shared<ChildProcess>();

            if (! p->start (std::forward<Args> (args)...))
                return nullptr;

            processes.insert (p);
            timer.startTimer (1000);

            return p;
        }

        /** Registers a callback function that is called for every ChildProcess that terminated.

            This registration is deleted when the returned ErasedScopedGuard is deleted.
        */
        auto addChildProcessExitedListener (std::function<void (ChildProcess*)> listener)
        {
            return listeners.addListener (std::move (listener));
        }

        /** Returns true if the ChildProcessManager contains any running ChildProcesses that it's
            monitoring.
        */
        auto hasRunningProcess() const
        {
            return timer.isTimerRunning();
        }

    private:
        ChildProcessManager() = default;
        ~ChildProcessManager() override  { clearSingletonInstance(); }

        void checkProcesses();

        std::set<std::shared_ptr<ChildProcess>> processes;
        detail::CallbackListenerList<ChildProcess*> listeners;
        TimedCallback timer { [this] { checkProcesses(); } };
    };

} // namespace higgs
