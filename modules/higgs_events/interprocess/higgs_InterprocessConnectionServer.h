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
/**
    An object that waits for client sockets to connect to a port on this host, and
    creates InterprocessConnection objects for each one.

    To use this, create a class derived from it which implements the createConnectionObject()
    method, so that it creates suitable connection objects for each client that tries
    to connect.

    @see InterprocessConnection

    @tags{Events}
*/
class HIGGS_API  InterprocessConnectionServer    : private Thread
{
public:
    //==============================================================================
    /** Creates an uninitialised server object.
    */
    InterprocessConnectionServer();

    /** Destructor. */
    ~InterprocessConnectionServer() override;

    //==============================================================================
    /** Starts an internal thread which listens on the given port number.

        While this is running, if another process tries to connect with the
        InterprocessConnection::connectToSocket() method, this object will call
        createConnectionObject() to create a connection to that client.

        Use stop() to stop the thread running.

        @param portNumber    The port on which the server will receive
                             connections
        @param bindAddress   The address on which the server will listen
                             for connections. An empty string indicates
                             that it should listen on all addresses
                             assigned to this machine.

        @see createConnectionObject, stop
    */
    bool beginWaitingForSocket (int portNumber, const String& bindAddress = String());

    /** Terminates the listener thread, if it's active.

        @see beginWaitingForSocket
    */
    void stop();

    /** Returns the local port number to which this server is currently bound.

        This is useful if you need to know to which port the OS has actually bound your
        socket when calling beginWaitingForSocket with a port number of zero.

        Returns -1 if the function fails.
    */
    int getBoundPort() const noexcept;

protected:
    /** Creates a suitable connection object for a client process that wants to
        connect to this one.

        This will be called by the listener thread when a client process tries
        to connect, and must return a new InterprocessConnection object that will
        then run as this end of the connection.

        @see InterprocessConnection
    */
    virtual InterprocessConnection* createConnectionObject() = 0;

private:
    //==============================================================================
    std::unique_ptr<StreamingSocket> socket;

    void run() override;

    HIGGS_DECLARE_NON_COPYABLE_WITH_LEAK_DETECTOR (InterprocessConnectionServer)
};

} // namespace higgs
