/********************************************************************
    created:    2014/08/28
    created:    28:8:2014   13:15
    filename:   E:\MT\trunk\code\EVA\server\persistant_data_service\db_struct.h
    file path:  E:\MT\trunk\code\EVA\server\persistant_data_service
    file base:  db_struct
    file ext:   h
    author:     ChenKang

    purpose:    数据库线程执行结构;
*********************************************************************/
#ifndef PDS_DB_STRUCT_H
#define PDS_DB_STRUCT_H

#include <game_share/game_def.h>
#include <server_share/database/def_db.h>
#include <server_share/database/db_logic_chunk.h>

struct PlayerID : public DBMsgData
{
    DEF::PID    pid;

    PlayerID( DEF::PID _pid ):pid(_pid) {}
    virtual ~PlayerID()  {}
};

struct LoadPlayerParam : public DBMsgData
{
    DEF::PID            pid;
    DEF::RPC_SESSION    rpc_session;

    LoadPlayerParam( DEF::PID _pid, DEF::RPC_SESSION _rpc_session ):
        pid(_pid),rpc_session(_rpc_session) {}
    virtual ~LoadPlayerParam()  {}
};

struct LoadPlayerResult : public DBMsgData
{
    DB_RECORD::CDBLogicPlayer    db_player;
    DEF::RPC_SESSION             rpc_session;

    LoadPlayerResult( DB_RECORD::CDBLogicPlayer& _db_player, DEF::RPC_SESSION _rpc_session ):
        db_player(_db_player),rpc_session(_rpc_session) {}

    virtual ~LoadPlayerResult()  {}
};

struct PlayerListBaseData
{
    DEF::UID    uid;
    DEF::PID    pid;
    uint32      last_online_time;
};

struct PlayerListPage : public DBMsgData
{
    std::vector<PlayerListBaseData>   m_List;
    virtual ~PlayerListPage()  {}
};

#endif // PDS_DB_STRUCT_H