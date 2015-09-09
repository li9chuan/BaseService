#ifndef PDS_DB_MGR_H
#define PDS_DB_MGR_H

#include <game_share/game_def.h>
#include <server_share/msg_struct/server_info.h>
#include <server_share/database/db_logic_chunk.h>
#include <server_share/id_generate.h>
#include <nel/net/message.h>
#include "sender_mgr/sender_login.h"

class CDBMgr : public NLMISC::CSingleton<CDBMgr>
{

public:

    CDBMgr( void );
   ~CDBMgr( void );

    void  init( uint32 rgdid );

    /// 创建角色;
    DB_RECORD::CDBLogicPlayer* create_player( DEF::UID, std::string _name );
    /// 查找角色 UID;
    DB_RECORD::CDBLogicPlayer* find_player_uid( DEF::UID, uint8 _index );

    /// 使用uid和索引查询玩家的pid， 没找到返回0
    DEF::PID   find_pid( DEF::UID _accid , uint8 _index );

    /// 查找角色 PID;
    DB_RECORD::CDBLogicPlayer* find_player_pid( DEF::PID );

    DB_RECORD::CDBLogicPlayer* get_first_player();
    DB_RECORD::CDBLogicPlayer* get_next_player( DEF::PID );


    /// 角色数量;
    uint32 get_player_num( void )        { return m_DBPlayers.size(); }
    /// 账号数量
    uint32 get_user_num()              { return m_playerlist.size(); }


    /// 预加载角色信息;
    void load_player_info( void );
    /// 保存角色详细信息;
    void save_player_detail( DB_RECORD::CDBLogicPlayer* _player_detail, bool copy_data=true );
    /// 保存角色列表数据;
    void save_playerlist( DEF::UID, DEF::PID _roleid );
    /// 删除角色列表;
    void clear_playerlist( DEF::UID );

    /// 更新函数;
    void db_frame_move( void );

    /// 获取本帧内需要更新到数据库的玩家 
    void need_update_players( std::list< DEF::PID >& update_players );

    /// 更新角色;
    bool update_player( TDBSaveOPT _opt, NLNET::CMessage& _msg );

    /// 更新角色基础数据;
    bool update_player_info( TDBSaveOPT _opt , NLNET::CMessage& _msg );

    /// 更新角色最大ID;
    inline void update_player_max_id( DEF::PID );

    ServerInfo           m_ServerInfo;

private:

    bool is_exec_limit();

    /// 执行角色表;
    void execute_player_table( DB_RECORD::CRecordPlayer& _rhs );

private:

    DEF::PID     m_SaveCurrPID;
    CIDGenerate  m_GeneratePID;

    /// 角色列表;
    typedef CHashMap< DEF::UID , std::vector<DEF::PID> >      TDBPlayerList;
    TDBPlayerList  m_playerlist;

    /// 角色实例;
    typedef CHashMap< DEF::PID, DB_RECORD::CDBLogicPlayer* > TDBLogicPlayer;
    TDBLogicPlayer m_DBPlayers;

};

#define  DBMgr  CDBMgr::instance()

#endif // PDS_DB_MGR_H

