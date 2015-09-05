#include "db_mgr.h"
#include "db_connect.h"
#include "db_sub_func.h"
#include <game_share/static_table/static_table_mgr.h>
#include <server_share/database/db_logic_chunk.h>

using namespace DB_RECORD;

NLMISC::CVariable<uint32>	VAR_PLAYER_NUM("pds", "NbPlayers"  , "memo", 0);

struct for_itemparam_insert
{
	template<  typename T >  void operator( )( T& rhs ){  rhs.set_insert(); }
};

struct for_itemparam_update
{
	template< typename T > void operator( )( T& rhs ){ rhs.set_update(); }
};

#define DBSave( __record_stru, __record_cont, __record_cont_data, __obj_id )\
DEF::PID _roleid ;\
_msg.serial( _roleid );\
DB_RECORD::CDBLogicPlayer* pPlayer = find_player_pid( _roleid );\
if ( NULL != pPlayer ) \
{\
    DB_RECORD::__record_stru _rhs;\
    _msg.serial( _rhs );\
    DB_RECORD::__record_cont::iterator _it = pPlayer->__record_cont_data.find( _rhs.__obj_id );\
    switch( _opt ) \
    {\
    case SAVE_OPT_INSERT:\
        {\
            if ( _it == pPlayer->__record_cont_data.end() ) \
            {\
                _rhs.set_insert();\
                pPlayer->__record_cont_data.insert( std::make_pair( _rhs.__obj_id , _rhs ) );\
            }\
            else\
            {\
                if( _it->second.is_delete() )\
                {\
                    _rhs.delete_charge_update();\
                    _it->second = _rhs;\
                }\
            }\
        }\
        break;\
    case SAVE_OPT_UPDATE:\
        {\
            if ( _it != pPlayer->__record_cont_data.end() )\
            {\
                _rhs.set_update();\
                _it->second = _rhs;\
            }\
        }\
        break;\
    case SAVE_OPT_DELETE:\
        {\
            if ( _it != pPlayer->__record_cont_data.end() )\
            {\
                _rhs.set_delete();\
                _it->second = _rhs;\
            }\
        }\
        break;\
    default:\
        break;\
    }\
    return true;\
}\
return false;

#define DBUpdateOnly( __record_data )\
DEF::PID pid;\
_msg.serial( pid );\
DB_RECORD::CDBLogicPlayer* pPlayer = find_player_pid( pid );\
if ( NULL != pPlayer )\
{\
    switch( _opt )\
    {\
    case SAVE_OPT_UPDATE:\
        {\
            _msg.serial( pPlayer->__record_data  );\
            pPlayer->__record_data.set_update();\
        }\
        break;\
    default:\
        nlwarning(#__record_data"  %d", _opt);\
        break;\
    }\
    return true;\
}\
return false;

CDBMgr::CDBMgr( void ):
m_SaveCurrPID(0)
{

}

CDBMgr::~CDBMgr( void )
{

}

void CDBMgr::init( uint32 rgdid )
{
    m_GeneratePID.init( rgdid , m_ServerInfo.player_max );
}

DB_RECORD::CDBLogicPlayer* CDBMgr::create_player( DEF::UID uid , std::string name )
{
    if ( name.empty() && name.size()>PLAYERNAME_LENGTH ) {
        return NULL;
    }

    DB_RECORD::CDBLogicPlayer* pPlayer = new ( std::nothrow ) DB_RECORD::CDBLogicPlayer( );
    if ( NULL == pPlayer ) {
        return NULL;
    }

    DEF::PID pid = m_GeneratePID.Generate();

    // 设置角色属性;
    pPlayer->RecordPlayer.uid    = uid;   // 账号ID;
    pPlayer->RecordPlayer.name   = name;  // 角色名字;
    pPlayer->RecordPlayer.pid    = pid;   // 角色ID;

    // 保存角色信息;
    std::pair< TDBLogicPlayer::iterator , bool > _res = m_DBPlayers.insert( std::make_pair( pid , pPlayer ) );

    if ( !_res.second )
    {
        SAFE_DELETE( pPlayer );
        return NULL;
    }

    VAR_PLAYER_NUM.set( m_DBPlayers.size(), false );

    // 插入标记;
    pPlayer->RecordPlayer.set_insert();

    // 保存角色列表;
    m_playerlist[ uid ].push_back( pid );

    // 新角色立即保存;
    execute_player_table( pPlayer->RecordPlayer );

    return pPlayer;
}

DB_RECORD::CDBLogicPlayer* CDBMgr::find_player_uid( DEF::UID uid , uint8 _index )
{
    TDBPlayerList::iterator _it = m_playerlist.find( uid );

    if ( _it == m_playerlist.end() ) {
        return NULL;
    }

    if ( _index < _it->second.size() )
    {
         return find_player_pid( _it->second.at( _index ) );
    }

    return NULL;
}

DEF::PID CDBMgr::find_pid( DEF::UID _accid , uint8 _index )
{
    TDBPlayerList::iterator _it = m_playerlist.find( _accid );

    if ( _it != m_playerlist.end() )
    {
        if ( _index < _it->second.size() )
        {
            return _it->second.at( _index );
        }
    }

    return 0;
}

DB_RECORD::CDBLogicPlayer* CDBMgr::find_player_pid( DEF::PID _roleid )
{
    TDBLogicPlayer::iterator _it_l = m_DBPlayers.find( _roleid );

    if ( _it_l != m_DBPlayers.end() )
    {
        // 是否正在删除角色;
        if ( !_it_l->second->RecordPlayer.is_delete() )
        {
            return _it_l->second;
        }
    }
    return NULL;
}
DB_RECORD::CDBLogicPlayer* CDBMgr::get_first_player( )
{
    TDBLogicPlayer::iterator _it_l = m_DBPlayers.begin();

    if ( _it_l != m_DBPlayers.end() )
    {
        return _it_l->second;
    }
    return NULL;
}

DB_RECORD::CDBLogicPlayer* CDBMgr::get_next_player( DEF::PID pid )
{
    TDBLogicPlayer::iterator iter = m_DBPlayers.find(pid);

    if ( iter != m_DBPlayers.end() )
    {
        ++iter;

        if ( iter != m_DBPlayers.end() )
        {
            return iter->second;
        }
    }
    return NULL;
}

void CDBMgr::save_playerlist( DEF::UID _accid , DEF::PID _roleid )
{
    m_playerlist[ _accid ].push_back( _roleid );
}

void CDBMgr::clear_playerlist( DEF::UID _accid )
{
    m_playerlist.erase( _accid );
}

void CDBMgr::save_player_detail( DB_RECORD::CDBLogicPlayer* _player_detail, bool copy_data/*true*/ )
{
    if ( NULL != _player_detail )
    {
        DB_RECORD::CDBLogicPlayer* pPlayer = find_player_pid(_player_detail->RecordPlayer.pid);

        if ( copy_data )
        {
            DB_RECORD::CDBLogicPlayer* pPlayer = new(std::nothrow) DB_RECORD::CDBLogicPlayer(*_player_detail);

            if ( pPlayer!=NULL )
            {
                m_DBPlayers.insert( std::make_pair( _player_detail->RecordPlayer.pid , pPlayer ) );
            }
        }
        else
        {
            m_DBPlayers.insert( std::make_pair( _player_detail->RecordPlayer.pid , _player_detail ) );
        }

        VAR_PLAYER_NUM.set( m_DBPlayers.size(), false );
    }
}

bool CDBMgr::update_player_info( TDBSaveOPT _opt , NLNET::CMessage& _msg )
{
    DBUpdateOnly(RecordPlayer)
}


void CDBMgr::update_player_max_id( DEF::PID pid )
{
    const uint64 idx = m_GeneratePID.GetIDX( pid );

    if ( idx > m_ServerInfo.player_max )
    {
        m_ServerInfo.player_max = idx;
    }
}











