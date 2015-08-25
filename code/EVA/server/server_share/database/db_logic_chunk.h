#ifndef SERVER_SHARED_DB_LOGIC_CHUNK_H
#define SERVER_SHARED_DB_LOGIC_CHUNK_H

#include "record_player.h"

namespace DB_RECORD {

    /**
    * PDS的游戏逻辑组织形式
    * Note: 所有以PlayerID为索引的数据，都可以加入到这个结构中管理。
    *       该结构在玩家登录时由PDS加载到逻辑服务器，是PDS中玩家数据的组织形式。
    * 
    * \author li9chuan@qq.com
    * \date 2014
    */

    struct CDBLogicPlayer : public DBMsgData
    {
        CRecordPlayer               RecordPlayer;

        CDBLogicPlayer(){}
        virtual ~CDBLogicPlayer()   {}

        void serial( NLMISC::IStream& s )
        {
            s.serial( RecordPlayer );
        }
    };

}

#endif
