// NeL - MMORPG Framework <http://dev.ryzom.com/projects/nel/>
// Copyright (C) 2010  Winch Gate Property Limited
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.

#include "stdnet.h"

#include "nel/net/callback_server_tcp.h"
#include "nel/net/net_log.h"


#ifdef USE_MESSAGE_RECORDER
#include "nel/net/dummy_tcp_sock.h"
#endif

#include "event.h"
#include <event2/util.h>

using namespace std;
using namespace NLMISC;

namespace NLNET {


/*
 * Connection callback (the disconnection callback is in callback_net_base.cpp
 */
void cbsTcpNewConnection (TSockId from, void *data)
{
	nlassert (data != NULL);
	CCallbackServerTcp *server = (CCallbackServerTcp *)data;

	LNETL3_DEBUG("LNETL3S: WebSktNewConnection()");

#ifdef USE_MESSAGE_RECORDER
	// Record connection
	server->noticeConnection( from );
#endif

	// send all my association to the new client
// association are disactivated so we don t need to send them
//	server->sendAllMyAssociations (from);

	// call the client callback if necessary
	if (server->_ConnectionCallback != NULL)
		server->_ConnectionCallback (from, server->_ConnectionCbArg);
}


/*
 * Constructor
 */
CCallbackServerTcp::CCallbackServerTcp( TRecordingState rec, const string& recfilename, bool recordall, bool initPipeForDataAvailable ) :
	CCallbackNetBase( rec, recfilename, recordall ),
	_ConnectionCallback(NULL),
	_ConnectionCbArg(NULL)
{
#ifndef USE_MESSAGE_RECORDER
	nlassertex( rec==Off, ("LNETL3S: The message recorder is disabled at compilation time ; switch the recording state Off") );
#endif

	CBufServerTcp::setDisconnectionCallback (_NewDisconnectionCallback, this);
	CBufServerTcp::setConnectionCallback (cbsTcpNewConnection, this);

	_IsAServer = true;
	_DefaultCallback = NULL;
}


/*
 * Send a message to the specified host (pushing to its send queue)
 * Recorded : YES
 * Replayed : MAYBE
 */
void CCallbackServerTcp::send (const CMessage &buffer, TSockId hostid, bool /* log */)
{
	nlassert (connected ());
	nlassert (buffer.length() != 0);
    nlassert (buffer.typeIsSet());

	if (hostid == InvalidSockId)
	{
		// broadcast
		sint nb = nbConnections ();
		_BytesSent += buffer.length () * nb;
	}
	else
	{
		_BytesSent += buffer.length ();
	}

//	if (log)
	{
//		LNETL3_DEBUG ("LNETL3S: Server: send(%s, %s)", buffer.toString().c_str(), hostid->asString().c_str());
	}

#ifdef USE_MESSAGE_RECORDER
	if ( _MR_RecordingState != Replay )
	{
#endif

		// Send
		CBufServerTcp::send (buffer, hostid);

#ifdef USE_MESSAGE_RECORDER
		if ( _MR_RecordingState == Record )
		{
			// Record sent message
			_MR_Recorder.recordNext( _MR_UpdateCounter, Sending, hostid, const_cast<CMessage&>(buffer) );
		}
	}
#endif
}


/*
 * Updates the network (call this method evenly)
 * Recorded : YES (in baseUpdate())
 * Replayed : YES (in baseUpdate())
 */
void CCallbackServerTcp::update2 ( sint32 timeout, sint32 mintime )
{
	H_AUTO(L3UpdateServer);

	nlassert (connected ());

//	LNETL3_DEBUG ("LNETL3S: Client: update()");
	baseUpdate2 ( timeout, mintime ); // first receive

#ifdef USE_MESSAGE_RECORDER
	if ( _MR_RecordingState != Replay )
	{
#endif
		// L1-2 Update (nothing to do in replay mode)
		CBufServerTcp::update (); // then send

#ifdef USE_MESSAGE_RECORDER
	}
#endif

}

/*
 * Updates the network (call this method evenly) (legacy)
 * Recorded : YES (in baseUpdate())
 * Replayed : YES (in baseUpdate())
 */
void CCallbackServerTcp::update ( sint32 timeout )
{
	H_AUTO(L3UpdateServer);

	nlassert (connected ());

//	LNETL3_DEBUG ("LNETL3S: Client: update()");
	baseUpdate ( timeout ); // first receive

#ifdef USE_MESSAGE_RECORDER
	if ( _MR_RecordingState != Replay )
	{
#endif
		// L1-2 Update (nothing to do in replay mode)
		CBufServerTcp::update (); // then send

#ifdef USE_MESSAGE_RECORDER
	}
#endif
}


/*
 * Read the next message in the receive queue
 * Recorded : YES
 * Replayed : YES
 */
void CCallbackServerTcp::receive (CMessage &buffer, TSockId *hostid)
{
	nlassert (connected ());

    // Receive
	CBufServerTcp::receive (buffer, hostid);
	buffer.readType ();
}


/*
 * Disconnect a connection
 * Set hostid to InvalidSockId to disconnect all connections.
 * If hostid is not null and the socket is not connected, the method does nothing.
 * Before disconnecting, any pending data is actually sent.
 * Recorded : YES in noticeDisconnection called in the disconnection callback
 * Replayed : YES in noticeDisconnection called in the disconnection callback
 */
void CCallbackServerTcp::disconnect( TSockId hostid )
{
#ifdef USE_MESSAGE_RECORDER
	if ( _MR_RecordingState != Replay )
	{
#endif
		// Disconnect
		CBufServerTcp::disconnect( hostid );

#ifdef USE_MESSAGE_RECORDER
	}
	// else, no need to manually replay the disconnection, such as in CCallbackClient,
	// it will be replayed during the next update()
#endif
}


/*
 *
 */
TSockId CCallbackServerTcp::getSockId (TSockId hostid)
{
	nlassert (hostid != InvalidSockId);	// invalid hostid
	nlassert (connected ());
	nlassert (hostid != NULL);
	return hostid;
}


/*
 * Returns true if there are messages to read
 * Recorded : NO
 * Replayed : YES
 */
bool CCallbackServerTcp::dataAvailable ()
{
#ifdef USE_MESSAGE_RECORDER
	if ( _MR_RecordingState != Replay )
	{
#endif

		// Real dataAvailable()
		return CBufServerTcp::dataAvailable ();

#ifdef USE_MESSAGE_RECORDER
	}
	else
	{
		// Simulated dataAvailable()
		return CCallbackNetBase::replayDataAvailable();
	}
#endif
}


//-------------------------
#ifdef USE_MESSAGE_RECORDER


/*
 * Replay connection and disconnection callbacks, client version
 */
bool CCallbackServerTcp::replaySystemCallbacks()
{
    return false;
}


/*
 * Record or replay connection
 */
void CCallbackServerTcp::noticeConnection( TSockId hostid )
{
	nlassert (hostid != InvalidSockId);	// invalid hostid
	if ( _MR_RecordingState != Replay )
	{
		if ( _MR_RecordingState == Record )
		{
			// Record connection
			CMessage addrmsg;
			addrmsg.serial( const_cast<CInetAddress&>(hostAddress(hostid)) );
			_MR_Recorder.recordNext( _MR_UpdateCounter, Accepting, hostid, addrmsg );
		}
	}
}

#endif // USE_MESSAGE_RECORDER


} // NLNET
