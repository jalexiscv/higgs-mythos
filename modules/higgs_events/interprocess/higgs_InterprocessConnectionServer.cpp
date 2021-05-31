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

InterprocessConnectionServer::InterprocessConnectionServer() : Thread (SystemStats::getHIGGSVersion() + ": IPC server")
{
}

InterprocessConnectionServer::~InterprocessConnectionServer()
{
    stop();
}

//==============================================================================
bool InterprocessConnectionServer::beginWaitingForSocket (const int portNumber, const String& bindAddress)
{
    stop();

    socket.reset (new StreamingSocket());

    if (socket->createListener (portNumber, bindAddress))
    {
        startThread();
        return true;
    }

    socket.reset();
    return false;
}

void InterprocessConnectionServer::stop()
{
    signalThreadShouldExit();

    if (socket != nullptr)
        socket->close();

    stopThread (4000);
    socket.reset();
}

int InterprocessConnectionServer::getBoundPort() const noexcept
{
    return (socket == nullptr) ? -1 : socket->getBoundPort();
}

void InterprocessConnectionServer::run()
{
    while ((! threadShouldExit()) && socket != nullptr)
    {
        std::unique_ptr<StreamingSocket> clientSocket (socket->waitForNextConnection());

        if (clientSocket != nullptr)
            if (auto* newConnection = createConnectionObject())
                newConnection->initialiseWithSocket (std::move (clientSocket));
    }
}

} // namespace higgs
