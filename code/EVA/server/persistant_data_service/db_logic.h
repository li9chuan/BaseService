#ifndef EVA_PDS_DB_LOGIC_H
#define EVA_PDS_DB_LOGIC_H

#include <nel/misc/singleton.h>
#include <nel/net/message.h>
#include "db_mgr.h"

/**
* 封装对数据库的逻辑操作。
* \author li9chuan@qq.com
* \date 2014
*/
class CDBLogic : public NLMISC::CSingleton<CDBLogic>
{
public:

    bool    putData( NLNET::CMessage &msgin );

    void    PreLoadNextPlayerInfo( NLNET::TServiceId, DEF::PID );


    typedef  std::map<DB_OPT, uint64>  TSaveOptCount;
    TSaveOptCount     m_SaveOptCount;

private: 

    void    IncSaveCount( DB_OPT db_opt );
};

#define  DBLogic  CDBLogic::instance()

#endif
