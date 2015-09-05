#include <nel/net/naming_client.h>
#include <server_share/server_def.h>
#include <game_share/static_table/static_table_mgr.h>

#include "persistant_data_service.h"
#include "db_logic.h"
#include "db_manager_messages.h"
#include "db_connect.h"
#include "db_mgr.h"

#ifdef NL_OS_WINDOWS
#	define NOMINMAX
#	include <windows.h>
#endif 

using namespace std;
using namespace NLMISC;
using namespace NLNET;

TUnifiedCallbackItem	DbManagerCbArray[] =
{
    { "DB_PUT",                                 cbPut                       },
    { "LoadPlayer",                             cbLoadPlayer                },

    { "CP",                                     cbCreatePlayer              },

    { "EnterGame_0" ,                           cbEnterGame                 },
    { "SPBTC" ,                                 cbPlayerBaseToClient        },
    
    
    { "NP" ,                                    cbRequestNextPlayer         },
};

NLMISC::CFileDisplayer *Fd = NULL;

extern void admin_modules_forceLink();
void foo()
{
    admin_modules_forceLink();
}

// a new logic service connecting to me, add it
void cbPLSConnection (const std::string &serviceName, NLNET::TServiceId  sid, void *arg)
{
    NLNET::CMessage msgout("LSIS");
    msgout.serial( DBMgr.m_ServerInfo );
    NLNET::CUnifiedNetwork::getInstance()->send (sid, msgout);
}

void cbRnkConnection(const std::string &serviceName, NLNET::TServiceId  sid, void *arg)
{
    DB_RECORD::CDBLogicPlayer* pPlayer = DBMgr.get_first_player();

    if ( pPlayer!=NULL )
    {
        SenderLogin.SendPreLoadPlayerFPSEnd( sid, pPlayer->RecordPlayer.pid );
    }
    else
    {
        SenderLogin.SendPreLoadPlayerDone(sid);
    }
}

void	CPersistantDataService::init()
{
    uint32 _elspale = CTime::getSecondsSince1970();

    std::string DatabaseName        = ConfigFile.getVar("DatabaseName").asString ();
    std::string DatabaseHost        = ConfigFile.getVar("DatabaseHost").asString ();
    std::string DatabaseLogin       = ConfigFile.getVar("DatabaseLogin").asString ();
    std::string DatabasePassword    = ConfigFile.getVar("DatabasePassword").asString ();
    short       DatabasePort        = ConfigFile.getVar("DatabasePort").asInt ();

    nlinfo("DatabaseName '%s'", DatabaseName.c_str());
    nlinfo("DatabaseHost '%s'", DatabaseHost.c_str());
    nlinfo("DatabaseLogin '%s'", DatabaseLogin.c_str());
    nlinfo("DatabasePassword '%s'", DatabasePassword.c_str());

    ICommand::execute ("loadconfig", *InfoLog);

    //	CUnifiedNetwork::getInstance()->setServiceDownCallback("*", onServiceDown);

    // db connect;
    MysqlConnect.init_db_connect( DatabaseHost.c_str() , 
        DatabaseLogin.c_str(),
        DatabasePassword.c_str(),
        DatabaseName.c_str(),
        DatabasePort );


    MysqlConnect.start_db_threads();

    ///  预加载数据 
    MysqlConnect.db_pre_treatment();

    DBMgr.load_player_info();

    MysqlConnect.db_after_treatment();

    //DBLogic.init( DatabaseHost, DatabaseLogin, DatabasePassword, DatabaseName );

    // init range id;
    sint32 region_id  = IService::getInstance()->ConfigFile.getVar("ShardId").asInt();

    DBMgr.init( region_id );

    NLNET::CUnifiedNetwork::getInstance()->setServiceUpCallback( LogicService    , cbPLSConnection );
    NLNET::CUnifiedNetwork::getInstance()->setServiceUpCallback( "RNK"           , cbRnkConnection );
    

    nlinfo ("Persistant Data Service initialized ( %d second ) " , CTime::getSecondsSince1970() - _elspale );
}

/*
* Release
*/
void	CPersistantDataService::release()
{
    MysqlConnect.close_db_threads();
    //MysqlConnect.close_egs_threads();
}

/*
* Update
*/
bool	CPersistantDataService::update()
{
    // frame_move db mgr;
    DBMgr.db_frame_move();
    // frame_move db connect;
    MysqlConnect.frame_move();

    return true;
}


NLNET_SERVICE_MAIN( CPersistantDataService, "PDS", "persistant_data_service", 50002, DbManagerCbArray, "", "" );


NLMISC_COMMAND (info, "service information.", "")
{
    if(args.size() != 0) return false;

    log.displayNL ("> base info :" );
    log.displayNL ("---> UserNum = %d", DBMgr.get_user_num() );
    log.displayNL ("---> PlayerNum = %d", DBMgr.get_player_num() );
    log.displayNL ("> db threads info :" );
    log.displayNL ("---> LoadPlayer     = %d", SelectThread(ThreadLoadPlayer).sub_event_size() );
    log.displayNL ("---> PlayerInfo     = %d", SelectThread(ThreadPlayerInfo).sub_event_size() );
    return true;
}

NLMISC_COMMAND (sqlstat, "sql execute status.", "")
{
    if(args.size() != 0) return false;
    //log.displayNL ( DBLogic.report().c_str() );
    return true;
}

NLMISC_COMMAND (loadconfig, "reload config file.", "")
{
    if(args.size() != 0) return false;

    log.displayNL ("PDS Load config files...");
    StaticTableMgr.init();
    log.displayNL ("PDS Load config done.");
    return true;
}

NLMISC_COMMAND ( topmsg, "", "")
{
    return true;
}

NLMISC_COMMAND ( playerinfo, "player info", "")
{
    return true;
}

