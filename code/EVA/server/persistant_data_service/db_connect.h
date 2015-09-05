#ifndef PDS_DB_CONNECT_H
#define PDS_DB_CONNECT_H

#include <nel/misc/singleton.h>
#include <server_share/db_threads/db_thread.h>

enum TDBConnectThreads
{
    ThreadLoadPlayer,           /// 加载角色线程;

    ThreadPlayerInfo,           /// 角色数据;         ( t_playerinfo );
    
    ThreadsMax
};

enum TPDSDBType
{
    DB_Player,
    DB_EGS_Player,
};

class CMysqlConnect : public NLMISC::CSingleton<CMysqlConnect>
{
public:

    CMysqlConnect();

    // 连接数据库;
    void  init_db_connect( char const* _host ,
                           char const* _user ,
                           char const* _pwd  ,
                           char const* _db   ,
                           short       _port );

    // 启动数据库线程;
    bool  start_db_threads( void );
    // 关闭数据库线程;
    void  close_db_threads( void );

    // 调用DB预处理存储过程;
    bool db_pre_treatment( void );
    // 调用DB后护理存储过程;
    bool db_after_treatment( void );

    // 更新子线程数据;
    void  frame_move( void );

    // 获取数据库表线程;
    nl::db_threads& GetDBThread( TDBConnectThreads thread_enum )    { return m_DBThreads[thread_enum]; }

private:

    std::vector<nl::db_threads>    m_DBThreads;
};

#define MysqlConnect CMysqlConnect::instance()
#define SelectThread CMysqlConnect::instance().GetDBThread

#endif // PDS_DB_CONNECT_H
