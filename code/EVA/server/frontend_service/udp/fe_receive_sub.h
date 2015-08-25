#ifndef NL_FE_RECEIVE_SUB_H
#define NL_FE_RECEIVE_SUB_H

#include "nel/misc/types_nl.h"
#include "nel/misc/buf_fifo.h"
#include "fe_receive_task.h"
#include "fe_types.h"

#include <game_share/game_def.h>
#include <list>
#include <map>


extern bool verbosePacketLost;

#ifdef NL_RELEASE
#define LOG_PACKET_LOST ;
#else
#define LOG_PACKET_LOST if (!verbosePacketLost) {} else nldebug
#endif

/// Types of message invalidity (used for THackingDesc::Reasons bitfield)
enum TBadMessageFormatType { InsufficientSize=1, NotSystemLoginCode=2, BadCookie=4, BadSystemCode=8, HackedSizeInBuffer=16, AccessClosed=32, IrrelevantSystemMessage=64, MalformedAction=128, UnknownExceptionType=256, UnknownFormatType=512, UnauthorizedCharacterSlot=1024 };

/// Return the string for the message invalidity reasons
std::string getBadMessageString( uint32 reasons );

/// Hacking description
struct THackingDesc
{
	THackingDesc() : InvalidMsgCounter(0), Reasons(0) {}	

	/// Number of invalid messages received
	uint32	InvalidMsgCounter;

	/// UserId if known
	//DEF::UID	UserId;

	/// Reason(s) of the invalidities (bitfield: see TBadMessageFormatType)
	uint32	Reasons;
};

/// Type of address set with counter
//typedef CHashMap<NLNET::CInetAddress,THackingDesc,CInetAddressHashMapTraits> THackingAddrSet;
typedef std::map<NLNET::CInetAddress,THackingDesc> THackingAddrSet;


class CFeReceiveSub
{
public:

	/// Constructor
	CFeReceiveSub() :
		_ReceiveTask(NULL),
		_ReceiveThread(NULL),
		//_ClientMap(),
		_Queue1(),
		_Queue2(),
		_CurrentReadQueue(NULL),
		_CurrentInMsg(),
		_RcvCounter(0),
		_RcvBytes(0),
		_PrevRcvBytes(0),
		_AccessOpen(true),
		ConnectionStatLog(NULL),
		_ConnectionStatDisp(NULL)
		{}

	/// Init
	void				init( uint16 firstAcceptableFrontendPort, uint16 lastAcceptableFrontendPort, uint32 dgrammaxlength );

	/// Update
	void				update();

	/// Release
	void				release();


	/// Return the number of connecged client
	//uint32				getNbClient();

	THostMap&			clientMap()					{ return _ClientMap; }
	NLNET::CUdpSock		*dataSock()					{ return _ReceiveTask->DataSock; }


	// Swap receive queues (to avoid high contention between the receive thread and the reading thread)
	void				swapReadQueues();

	// Read incoming data from the current read queue
	void				readIncomingData();

	/// Open or close the access for new clients
	void				openAccess( bool b ) { _AccessOpen = b; }

	/// Return the current access state
	bool				accessOpen() const { return _AccessOpen; }

	/// Do not accept the current message (hacking detection)
	void				rejectReceivedMessage( TBadMessageFormatType bmft );


	NLMISC::CLog		*ConnectionStatLog;

protected:

	/** Process current incoming message (handles client addition/removal/identification,
	 * then calls handleReceivedMsg() if not removal)
	 */
	void	handleIncomingMsg();

	/// Process current received message (called by handleIncomingMsg())
	void	handleReceivedMsg();

    void    handleReceivedProtoCommand();

	/// Display lost statistics
	void	displayDatagramStats( NLMISC::TTime lastdisplay );

private:

	/// Receive socket from the clients
	CFEReceiveTask		*_ReceiveTask;

	/// Receive thread
	NLMISC::IThread		*_ReceiveThread;

	/// Client map by address
	THostMap			_ClientMap;

	/// First queue
	NLMISC::CBufFIFO	_Queue1;

	/// Second queue
	NLMISC::CBufFIFO	_Queue2;

	/// Current read queue
	NLMISC::CBufFIFO	*_CurrentReadQueue;

	/// Current incoming message
	TReceivedMessage	*_CurrentInMsg;

	/// Number of messages received (stat)
	volatile uint32		_RcvCounter;

	/// Receive bytes in UDP (stat)
	volatile uint32		_RcvBytes;

	/// Previous RcvBytes (stat)
	volatile uint32		_PrevRcvBytes;

	/// Set of addresses of unidentified clients
	THackingAddrSet				_UnidentifiedFlyingClients;

	/// Do clients have the right to connect? (does not affect clients already connected)
	bool						_AccessOpen;

	NLMISC::CFileDisplayer		*_ConnectionStatDisp;
};


#endif // NL_FE_RECEIVE_SUB_H

/* End of fe_receive_sub.h */
