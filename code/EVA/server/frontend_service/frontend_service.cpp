#ifdef HAVE_CONFIG_H
#include "config.h"
#endif // HAVE_CONFIG_H

#include <map>
#include <utility>
#include <game_share/tools.h>
#include <server_share/server_def.h>
#include <server_share/client_msg_desc.h>
#include <server_share/lua/script_mgr.h>
#include <server_share/lua/lua_network.h>
#include <nel/misc/window_displayer.h>
#include <nel/net/naming_client.h>

#include "frontend_service.h"
#include "udp/fe_receive_task.h"
#include "session_mgr.h"
#include "client_mgr.h"
#include "msg_fes.h"
#include "msg_fes_broadcast.h"
#include "msg_client.h"
#include "udp_sender.h"
#include <game_share/timer.h>

#ifdef NL_OS_WINDOWS
#include <Windows.h>
#endif

using namespace std;
using namespace NLMISC;
using namespace NLNET;
using namespace FES;
using namespace DEF;

extern void admin_modules_forceLink();
void foo()  {   admin_modules_forceLink();  }

NLMISC::CVariable<uint32>	VAR_PLAYER_NUM("fes", "NbPlayers"  , "memo", 0);

//NLNET::TCallbackItem ClientCallbackArray[] =
//{
////    { "LOGIN",                      cbLogin                             },
//    { "CT_PLY",                     cbCreatePlayer                      },
//};

TUDPCallbackItem ClientUDPCallbackArray[] =
{
    { "LOGIN",                      cbUDPLogin                  },
    { "CT_PLY",                     cbUDPCreatePlayer           },
};

/**
 * Contains *all* callbacks from the shard
 */
NLNET::TUnifiedCallbackItem CallbackArray[] =
{
    { "T2C",                        cbTransport2Client            },	//	to client   from EGS && PLS
    { "U2C",                        cbUDP2Client                  },	//	to client   from EGS && PLS
    { "ADD_MSG_BUF",                cbAddMsgBuffer                },	//	to client   from EGS && PLS
    
    { "GC",                         cbGlobleChat                  },
    { "SYNC_CLIENT_SESSION",        cbSyncClientSession           },
    { "SyncUserPLS",                cbSyncUserLogicServer         },
    { "LOAD_SUCESS",                cbSetUserPID                  },

    { "CleanUpClient",              cbCleanUpClient               },  //  from other FES  or  EGS LogoutPlayer
};



void CFrontEndService::init()
{
    LocalTime.SetCurrTime( CTime::getLocalTime() );
    AcceptClients  = false;

    try
    {
        _inetPort                    = ConfigFile.getVar ("INetPort").asInt();
        uint32 playerMax             = ConfigFile.getVar ("ClientMax").asInt();

        CConfigFile::CVar& WaitServiceAccess = ConfigFile.getVar("WaitServiceAccess");
        for ( uint i=0; i!=WaitServiceAccess.size(); ++i )
        {
            _WaitServiceAccess.push_back( WaitServiceAccess.asString(i) );
        }
    }
    catch(const Exception &)
    {}


    //string fn = IService::getInstance()->SaveFilesDirectory.toString();
    //fn += ConfigFile.getVar("LogDir").asString();
    //fn += "frontend_service.stat";
    //nlinfo("Frontend stat in directory '%s'", fn.c_str());
    //NLMISC::CFileDisplayer *Fd = new NLMISC::CFileDisplayer(fn);
    //Loger().addDisplayer(Fd);
    //if (WindowDisplayer) Loger().addDisplayer (WindowDisplayer);

    ///////////////////////////////////////////////////



    MsgDesc.LoadMsgXml();
    TimerManager->init();

    ScriptMgr.init();
    LuaNetworkMgr.Init();

    //////////////////////////////////////////////////
    // TCP 相关
    //m_Clients = new NLNET::CCallbackServer();
    //nlassert (m_Clients != NULL);
    //m_Clients->addCallbackArray (ClientCallbackArray, sizeof(ClientCallbackArray)/sizeof(ClientCallbackArray[0]));
    //m_Clients->setDefaultCallback(TCPForwarding);
    //m_Clients->setConnectionCallback ( onConnectionClient, NULL );
    //m_Clients->setDisconnectionCallback ( onDisconnectClient, NULL );
    //m_Clients->setMaxExpectedBlockSize(MaxUDPPacketSize);
    //m_Clients->init (_inetPort);

    UDPSender.addCallbackArray(ClientUDPCallbackArray, sizeof(ClientUDPCallbackArray)/sizeof(ClientUDPCallbackArray[0]));

    /////////////////////////////////////////////////


    // add a connection to the LS
    string LSAddr;
    if (haveArg('T'))
    {
        // use the command line param if set
        LSAddr = getArg('T');
    }
    else if (ConfigFile.exists ("LSHost"))
    {
        // use the config file param if set
        LSAddr = ConfigFile.getVar("LSHost").asString();
    }
    // the config file must have a valid address where the login service is
    nlassert(!LSAddr.empty());

    // add default port if not set by the config file
    if (LSAddr.find (":") == string::npos)
        LSAddr += ":49999";
    CUnifiedNetwork::getInstance()->setServiceUpCallback("LS", cbLSConnection);
    CUnifiedNetwork::getInstance()->addService("LS", LSAddr);

    CUnifiedNetwork::getInstance()->setServiceDownCallback(LogicService, cbPLSDisconnection);
    CUnifiedNetwork::getInstance()->setServiceDownCallback(EntitiesServer, cbEGSDisconnection);

    SessionMgr.init( );

    //////////////////////////////////////////////////////////////////////////

    const uint32 UDPPort = _inetPort;
    _ReceiveSub.init( UDPPort, UDPPort, MaxUDPPacketSize);

    AcceptClients = true;
}

bool CFrontEndService::update()
{
    //AcceptClients = CNamingClient::IsCanAccess( _WaitServiceAccess );

    _CurrTicks = CTime::getLocalTime();
    LocalTime.SetCurrTime(_CurrTicks);

    //m_Clients->update ();

    _ReceiveSub.swapReadQueues();
    _ReceiveSub.readIncomingData();

    ///////////////////////////////////////

    TimerManager->tickUpdate();

    ////////////////////////////////////////////////

    SessionMgr.update( _CurrTicks );
    UDPSender.update();

    ScriptMgr.update();
    LuaNetworkMgr.Update();

    return true;
}

void CFrontEndService::release()
{
//    delete m_Clients; m_Clients = NULL;
    _ReceiveSub.release();
    TimerManager->release();

    ScriptMgr.release();
    LuaNetworkMgr.Release();

    google::protobuf::ShutdownProtobufLibrary();
}

void CFrontEndService::IncReceiveMsgCount( std::string msg_name )
{
    TReceiveMsgCount::iterator iter = m_ReceiveMsgCount.find(msg_name);

    if ( iter!=m_ReceiveMsgCount.end() )
    {
        iter->second += 1;
    }
    else
    {
        m_ReceiveMsgCount.insert( make_pair(msg_name, 1) );
    }
}

/****************************************************************************   
 * SanGuo FRONTEND SERVICE MAIN Function
 *
 * This call create a main function for a service:
 *
 *    - based on the "CFrontEndService" class
 *    - having the short name "FES"
 *    - having the long name "frontend_service"
 *    - listening on the port "0" (dynamically determined)
 *    - and shard callback set to "CallbackArray"
 *
 ****************************************************************************/
NLNET_SERVICE_MAIN (CFrontEndService, "FES", "frontend_service", 0, CallbackArray, "", "")

NLMISC_COMMAND (info, "service information.", "")
{
    if(args.size() != 0) return false;

    log.displayNL ("ClientNumber:%u", VAR_PLAYER_NUM.get());
    
    if ( FrontEndService->AcceptClients )
    {
        log.displayNL ("AcceptClients:true");
    }
    else
    {
        log.displayNL ("AcceptClients:false");
    }

    return true;
}

NLMISC_COMMAND (topmsg, "", "")
{
    if(args.size() != 0) return false;

    CFrontEndService::TReceiveMsgCount::iterator iter,it_end;
    iter    = FrontEndService->m_ReceiveMsgCount.begin();
    it_end  = FrontEndService->m_ReceiveMsgCount.end();

    std::multimap<uint64, std::string>  sortmap;

    while ( iter!=it_end )
    {
        sortmap.insert( make_pair( iter->second, iter->first ) );
        ++iter;
    }

    std::multimap<uint64, std::string>::reverse_iterator riter,rit_end;
    riter    = sortmap.rbegin();
    rit_end  = sortmap.rend();

    while ( riter!=rit_end )
    {
        log.displayNL("Count: %"NL_I64"u       Msg: %s", riter->first, riter->second.c_str() );
        ++riter;
    }

    return true;
}

NLMISC_COMMAND (loadmsg, "", "")
{
    if(args.size() != 0) return false;

    MsgDesc.LoadMsgXml();
    log.displayNL ("ReLoadMsgXml Done.");

    return true;
}

/* end of file */
