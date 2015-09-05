#include "db_logic.h"

#include <nel/misc/sstring.h>
#include <server_share/database/def_db.h>
#include <server_share/server_def.h>
#include <server_share/msg_struct/msg_struct_pls.h>

using namespace NLMISC;
using namespace NLNET;
using namespace DEF;
using namespace DB_RECORD;
using namespace std;

bool    CDBLogic::putData( NLNET::CMessage &msgin )
{
    bool send_msgout = false;

    DB_OPT opt = INVALD_OPT;
    msgin.serial(opt);

    IncSaveCount(opt);

    switch ( opt )
    {
    case UPDATE_PLAYER:
        {
            return DBMgr.update_player( SAVE_OPT_UPDATE, msgin );
        }
    case UPDATE_PLAYER_INFO:
        {
            return DBMgr.update_player_info( SAVE_OPT_UPDATE , msgin );
        }
    default:
        {
            nlwarning("DB OPT:%d no define.",opt);
        }
    }

    return send_msgout;
}

extern NLMISC::CVariable<uint32>	VAR_PRELOAD_PLAYERS_FPS;

void CDBLogic::PreLoadNextPlayerInfo( NLNET::TServiceId sid, DEF::PID pid )
{
    DB_RECORD::CDBLogicPlayer* pPlayer = DBMgr.find_player_pid(pid);
    uint32 load_count = 0;

    while ( pPlayer!=NULL && load_count < VAR_PRELOAD_PLAYERS_FPS.get()  )
    {
        ++load_count;

        CMessage  msgout("NPI");
        msgout.serial( pPlayer->RecordPlayer );
        Network->send( sid, msgout );

        pPlayer = DBMgr.get_next_player( pPlayer->RecordPlayer.pid );
    }

    if ( pPlayer!=NULL )
    {
        SenderLogin.SendPreLoadPlayerFPSEnd( sid, pPlayer->RecordPlayer.pid );
    }
    else
    {
        SenderLogin.SendPreLoadPlayerDone(sid);
    }
}


void CDBLogic::IncSaveCount( DB_OPT db_opt )
{
    TSaveOptCount::iterator iter = m_SaveOptCount.find(db_opt);

    if ( iter!=m_SaveOptCount.end() )
    {
        ++(iter->second);
    }
    else
    {
        m_SaveOptCount.insert( std::make_pair( db_opt, 1 ) );
    }
}

