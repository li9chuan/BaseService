#include "db_thread.h"


#ifdef NL_OS_WINDOWS
#pragma comment(linker,"/nodefaultlib:LIBCMTD.lib")
#endif

namespace nl
{
__db_s_msg::~__db_s_msg( void )
{

}

db_threads::db_threads( void )
    : m_thread_( NULL )
    , m_is_exit( true )
{
    mysql_library_init( 0 , NULL , NULL );
    mysql_thread_init();
}

db_threads::db_threads ( const db_threads& rhs )
    : m_thread_( NULL )
    , m_is_exit( true )
{
    mysql_library_init( 0 , NULL , NULL );
    mysql_thread_init();
}

db_threads::~db_threads( void )
{
    mysql_thread_end();
    mysql_library_end();
}

bool db_threads::connect( sint32  db_type, 
                          char const *_host, /// IP;
                          char const *_user, /// 用户名;
                          char const *_pwd,  /// 数据库密码;
                          char const *_db ,  /// 数据库
                          short _port ,      /// 端口;
                          _init_sql_worker _sql_init )
{
    if ( NULL != _host     &&
         NULL != _user     &&
         NULL != _pwd      &&
         NULL != _db       &&
         _port >=0         &&
         NULL != _sql_init )
    {
        if ( m_db_mgr._db_main_worker[ db_type ].connect( _host , _user , _pwd , _db , _port ) )
        {
            s_db_info* _db_info = &m_db_mgr._db_info[ db_type ];

            if ( NULL != _db_info )
            {
                _db_info->host         = _host;
                _db_info->user         = _user;
                _db_info->pwd          = _pwd;
                _db_info->db           = _db;
                _db_info->port         = _port;
                _db_info->_init_worker = _sql_init;

                // init sql;
                ( *_sql_init )( &m_db_mgr._db_main_worker[ db_type ] );

                return true;
            }
        }
        else
        {
            nlerror( " error : connect mysql faile  host = %s , user = %s , pwd = %s , _db_name = %s ,  port = %d " , _host , _user , _pwd , _db , _port );
            m_db_mgr._db_main_worker.erase( db_type );
        }
    }

    return false;
}

sql_worker* db_threads::get_worker( sint32 db_type, bool _main )
{
    if ( _main )
    {
        return &(m_db_mgr._db_main_worker[ db_type ]);
    }

   return &(m_db_mgr._db_sub_worker[ db_type ]);
}

void db_threads::post_sub_msg( DBMsgData* data , _proc_msg func )
{
    db_s_msg* _msg = new ( std::nothrow ) db_s_msg();

    if ( NULL != _msg )
    {
        _msg->data = data;
        _msg->func = func;

        if ( !m_main_threads_.push_event( _msg ) )
        {
            safe_delete( data );
            safe_delete( _msg );
        }
    }
    else
    {
        safe_delete( data );
    }
}

void db_threads::post_main_msg( DBMsgData* data , _proc_msg func )
{
    db_s_msg* _msg = new( std::nothrow ) db_s_msg();

    if ( NULL != _msg )
    {
        _msg->data = data;
        _msg->func = func;

        if ( !m_sub_threads_.push_event( _msg ) )
        {
            safe_delete( data );
            safe_delete( _msg );
        }
    }
    else
    {
        safe_delete( data );
    }
}

void db_threads::frame_move( void )
{
    while ( true )
    {
        db_s_msg* _db_msg = m_sub_threads_.peek_event();

        if ( !_db_msg ){
            break;
        }

        if ( _db_msg->func  )
        {
            ( *_db_msg->func )( _db_msg->data  );

            if( _db_msg->data ){ delete _db_msg->data; }
            _db_msg->data = NULL;

            delete _db_msg;
            _db_msg = NULL;
        }
    }
}

void db_threads::run( void )
{
    // threads connect mysql;
    mysql_thread_init();

    if ( !m_db_mgr._db_info.empty() )
    {
        for ( DBMgrInfo::TDBInfo::iterator  _it  = m_db_mgr._db_info.begin() ;
                                            _it != m_db_mgr._db_info.end() ;
                                            ++_it )
        {
            sql_worker* _worker = &m_db_mgr._db_sub_worker[ _it->first ];

            if ( _worker && _worker->connect( _it->second.host.c_str() , 
                                              _it->second.user.c_str() ,
                                              _it->second.pwd.c_str() ,
                                              _it->second.db.c_str(),
                                              _it->second.port ) )
            {
                ( *_it->second._init_worker )( _worker );
            }
            else
            {
                nlerror( " connect mysql faile host = %s , user = %s , pwd = %s , db_name = %s , port = %d " , _it->second.host.c_str() , 
                                                                                                               _it->second.user.c_str() ,
                                                                                                               _it->second.pwd.c_str()  ,
                                                                                                               _it->second.db.c_str()   ,
                                                                                                               _it->second.port );

                m_db_mgr._db_sub_worker.erase( _it->first );
            }
        }

        if ( !m_db_mgr._db_sub_worker.empty() )
        {
            while ( true )
            {
                db_s_msg* _db_msg = m_main_threads_.peek_event();

                if ( _db_msg && _db_msg->func )
                {
                    ( *_db_msg->func )( _db_msg->data );

                    if( _db_msg->data ){ delete _db_msg->data; }
                    _db_msg->data = NULL;

                    delete _db_msg;
                    _db_msg = NULL;
                }
                else if ( m_is_exit )
                {   // normal exit threads;
                    break;
                }
                else
                {   // cpu sleep 3ms;
                    NLMISC::nlSleep( 3 );
                }
            }
        }
    }

    mysql_thread_end();
}

bool db_threads::start( void )
{
    m_thread_ = NLMISC::IThread::create( this );

    if ( m_thread_ )
    {
        m_thread_->start();
        m_is_exit = false;
    }

    return ( NULL != m_thread_ );
}

void db_threads::close( void )
{
    if ( NULL != m_thread_ )
    {
        m_is_exit = true;
        m_thread_->wait();
    }
}

}


