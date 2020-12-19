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

template <typename Invokable, std::enable_if_t<std::is_same_v<std::invoke_result_t<Invokable&>, void>, int> = 0>
ThreadPoolJob::JobStatus runThreadPoolLambdaJob (Invokable& job)
{
    job();
    return ThreadPoolJob::jobHasFinished;
}

template <typename Invokable, std::enable_if_t<std::is_same_v<std::invoke_result_t<Invokable&>, ThreadPoolJob::JobStatus>, int> = 0>
ThreadPoolJob::JobStatus runThreadPoolLambdaJob (Invokable& job)
{
    return job();
}

template <typename, typename = void>
constexpr auto canRunThreadPoolLambdaJob = false;

template <typename Invokable>
constexpr auto canRunThreadPoolLambdaJob<Invokable, Void<decltype (runThreadPoolLambdaJob (std::declval<Invokable&>()))>> = true;

template <typename Invokable>
struct ThreadPoolLambdaJob final : public ThreadPoolJob
{
    explicit ThreadPoolLambdaJob (Invokable&& jobToRun)
        : ThreadPoolJob ("lambda"), job (std::move (jobToRun)) {}

    JobStatus runJob() final
    {
        return runThreadPoolLambdaJob (job);
    }

    Invokable job;
};

} // namespace higgs::detail
