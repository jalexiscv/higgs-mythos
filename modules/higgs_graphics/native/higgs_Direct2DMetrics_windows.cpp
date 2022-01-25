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

#if HIGGS_DIRECT2D_METRICS

namespace higgs
{

String Direct2DMetricsHub::getProcessString() noexcept
{
    auto processID = GetCurrentProcessId();
    return String::toHexString ((pointer_sized_int) processID);
}

void Direct2DMetricsHub::HubPipeServer::messageReceived (const MemoryBlock& message)
{
    int requestType = *(int*) message.getData();
    switch (requestType)
    {
        case getValuesRequest:
        {
            ScopedLock locker { owner.lock };

            auto foregroundWindow = GetFocus();
            Direct2DMetrics::Ptr metrics = nullptr;
            for (int i = 0; i < owner.metricsArray.size(); ++i)
            {
                auto arrayEntry = owner.metricsArray[i];
                if (arrayEntry->windowHandle && arrayEntry->windowHandle == foregroundWindow)
                {
                    metrics = arrayEntry;
                    break;
                }
            }

            if (! metrics)
            {
                if (owner.lastMetrics && owner.metricsArray.contains (owner.lastMetrics))
                    metrics = owner.lastMetrics;
            }

            if (metrics)
            {
                MemoryBlock block { sizeof (GetValuesResponse), true };

                auto* response = (GetValuesResponse*) block.getData();
                response->responseType = getValuesRequest;
                response->windowHandle = metrics->windowHandle;

                for (size_t i = 0; i <= Direct2DMetrics::drawGlyphRunTime; ++i)
                {
                    auto& accumulator = metrics->getAccumulator (i);
                    response->values[i].count = accumulator.getCount();
                    response->values[i].total = metrics->getSum (i);
                    response->values[i].average = accumulator.getAverage();
                    response->values[i].minimum = accumulator.getMinValue();
                    response->values[i].maximum = accumulator.getMaxValue();
                    response->values[i].stdDev = accumulator.getStandardDeviation();
                }

                // Track bitmap operations common to all device contexts
                for (size_t i = Direct2DMetrics::createBitmapTime; i <= Direct2DMetrics::unmapBitmapTime; ++i)
                {
                    auto& accumulator = owner.imageContextMetrics->getAccumulator (i);
                    response->values[i].count = accumulator.getCount();
                    response->values[i].total = metrics->getSum (i);
                    response->values[i].average = accumulator.getAverage();
                    response->values[i].minimum = accumulator.getMinValue();
                    response->values[i].maximum = accumulator.getMaxValue();
                    response->values[i].stdDev = accumulator.getStandardDeviation();
                }

                sendMessage (block);

                owner.lastMetrics = metrics.get();
            }
            break;
        }

        case resetValuesRequest:
        {
            owner.resetAll();
            break;
        }
    }
}

void Direct2DMetricsHub::resetAll()
{
    ScopedLock locker { lock };

    imageContextMetrics->reset();
    for (auto metrics : metricsArray)
    {
        metrics->reset();
    }
}

} // namespace higgs

#endif
