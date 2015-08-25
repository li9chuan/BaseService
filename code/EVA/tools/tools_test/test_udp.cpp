#include "test_misc.h"
#include <iostream>
#include <ctime>
#include <game_share/tools.h>
#include <nel/net/udp_sim_sock.h>

using namespace std;
using namespace NLMISC;
using namespace NLNET;

CUdpSock	*UdpSock = NULL;

void test_udp()
{
    UdpSock = new CUdpSock( false );
    try
    {
        UdpSock->connect( CInetAddress ( "127.0.0.1", 9704) );
    }
    catch (const Exception &e)
    {
        //InfoLog->displayRawNL ("Cannot connect to remote UDP host '%s': %s", ServerAddr.c_str(), e.what() );
    }

    CMessage  msgout("L_ITEM");
    uint32 size = msgout.length();

    UdpSock->send (msgout.buffer(), size);

    UdpSock->sendTo( msgout.buffer(), size, CInetAddress ( "127.0.0.1", 9704) );
    UdpSock->close();

    while (true)
    {
        nlSleep(1000);
    }
}











