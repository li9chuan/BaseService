#ifndef db_thread_h__
#define db_thread_h__

#include "sql_worker.h"
#include <nel/misc/mutex.h>
#include <nel/misc/common.h>
#include <nel/misc/thread.h>
#include <game_share/buf_fifo_ring.h>
#include <game_share/buf_fifo2.h>
#include <game_def.h>
#include <server_share/database/def_db.h>

#include <deque>

namespace nl
{

//// db type;
//typedef enum  _e_db_type
//{
//    db_item                 = 0x2 ,    /// 道具数据;
//    db_item_param           = 0x3 ,    /// 道具属性;
//    db_role                 = 0x4 ,    /// 角色数据;
//    db_hero                 = 0x5 ,    /// 英雄数据;
//    db_mission_temp         = 0x6 ,    /// 重复任务;
//    db_mission_tracker      = 0x7 ,    /// 已接任务;
//    db_legion               = 0x8 ,    /// 军团数据;
//    db_legion_member        = 0x9 ,    /// 军团成员;
//    db_request_join_legion  = 10 ,     /// 请求加入军团;
//    db_castle_data          = 11 ,     /// 副本部署;
//    db_load_role            = 12 ,     /// 加载角色;
//    db_achievement          = 13 ,     /// 成就数据;
//    db_guard                = 14 ,     /// 守卫数据;
//    db_limituse             = 15 ,     /// 使用道具;
//    db_dupinfo              = 16 ,     /// 副本数据;
//    db_egs_player           = 17 ,     /// egs玩家基础数据;
//    db_egs_friend           = 18 ,     /// egs好友数据;
//    db_common_mail          = 19 ,     /// 玩家邮件;
//    db_system_mail          = 20 ,     /// 系统邮件;
//    db_oms_mail             = 21 ,     /// 运维系统邮件;
//    db_complate_achievement = 22 ,     /// 完成已领取奖励成就;
//    db_log_event            = 23 ,     /// 事件log
//    db_player_name          = 24 ,     /// 全局玩家名字数据;
//}e_dbtype;


// inlitialize sql worker;
typedef void ( *_init_sql_worker )( sql_worker* );
// db function processing msg;
typedef void* ( *_proc_msg )( void * );

// message;
typedef struct __db_s_msg
{
    DBMsgData*      data;
    _proc_msg       func;

    __db_s_msg( void )
        : data( NULL )
        , func( NULL )
    {}

    ~__db_s_msg( void );

}db_s_msg;

class db_event_queue
{
public:
    db_event_queue( void )
    {
        m_db_event_queue.init( 1024 * 1024 );
    }

   ~db_event_queue( void )
   { }

   bool push_event( db_s_msg* _db_s_msg )
   {
       return m_db_event_queue.push_back( _db_s_msg );
   }

    db_s_msg* peek_event( void )
    {
        return m_db_event_queue.pop_front();
    }

    uint32 size( void )
    {
        return m_db_event_queue.size();
    }

private:
    
    NLMISC::CBufFIFO2< db_s_msg > m_db_event_queue;
};

struct s_db_info
{
    std::string host;
    std::string user;
    std::string pwd;
    std::string db;
    short       port;
    _init_sql_worker _init_worker;

    s_db_info( void )
        : port( 0 )
        , _init_worker( NULL )
    { }
};

struct DBMgrInfo
{
    typedef std::map< sint32 , s_db_info  >   TDBInfo;
    typedef std::map< sint32 , sql_worker >   TSQLWorker;

    TDBInfo     _db_info;
    TSQLWorker  _db_main_worker;
    TSQLWorker  _db_sub_worker;
};

class db_threads : public NLMISC::IRunnable
{
public:
    db_threads( void );
    virtual ~db_threads( void );
    db_threads ( const db_threads& rhs );

    // connect db base;
    bool connect( sint32  db_type, 
                  char const *_host,
                  char const *_user,
                  char const *_pwd,
                  char const *_db ,
                  short _port ,
                  _init_sql_worker _sql_init );

    // get sql worker;
    sql_worker* get_worker( sint32 db_type, bool _main = false );


    // post sub threads;
    void post_sub_msg( DBMsgData* data , _proc_msg func );

    // post main threads;
    void post_main_msg( DBMsgData* data , _proc_msg func );

    // frame move;
    void frame_move( void );

    // start;
    bool start( void );

    // close;
    void close( void );

    // sub threads event size;
    uint32 sub_event_size( void ){  return m_main_threads_.size(); }

    // main threads event size;
    uint32 main_event_size( void ){ return m_sub_threads_.size(); }

protected:
    // sub threads run;
    void run( void );

private:
    volatile bool    m_is_exit;
    NLMISC::IThread* m_thread_;
    DBMgrInfo        m_db_mgr;
    db_event_queue   m_main_threads_; // main post msg;
    db_event_queue   m_sub_threads_;  // sub post put;
};


}

#endif // db_thread_h__
