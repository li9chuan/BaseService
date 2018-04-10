#include <errmsg.h>
#include "mysql_conn.h"
#include "mysql_stmt.h"
#include "mysql_result.h"
#include "string"

#include "script_mgr.h"

#ifdef NL_OS_WINDOWS
static const char* DB_NAMES = "utf8";
#else
static const char* DB_NAMES = "utf8mb4";
#endif // NL_OS_WINDOWS

CLuaMysqlConn::CLuaMysqlConn( void )
    : m_mysql_( NULL ),
      m_port_( 3306 ),
      m_errno_( 0 ),
      m_error_( "" )
{
}

CLuaMysqlConn::~CLuaMysqlConn( void )
{
    close();
}

// connect.
bool CLuaMysqlConn::connect( char const *host_,
                        char const *user_,
                        char const *pwd_,
                        char const *db_,
                        int         port_ )
{
    close();

    m_mysql_ = mysql_init( NULL );

    if ( NULL != m_mysql_ )
    {
        // set character.
        mysql_options( m_mysql_, MYSQL_SET_CHARSET_NAME, DB_NAMES );

        if ( NULL != mysql_real_connect( m_mysql_,
                                         host_,
                                         user_,
                                         pwd_,
                                         db_,
                                         port_,
                                         NULL,
                                         0 ) )
        {
            m_host_.assign( host_ );
            m_user_.assign( user_ );
            m_pwd_.assign( pwd_ );
            m_db_.assign( db_ );
            m_port_ = port_;

            // set autocommit.
            mysql_autocommit( m_mysql_, 1 );
            // set character set.
            mysql_set_character_set( m_mysql_, DB_NAMES );

            mysql_query(m_mysql_, "SET character_set_client=binary");

            nlinfo( "connect mysql server succeed!" );
            nlinfo( "mysql client library : %s." , mysql_get_client_info() );
            nlinfo( "mysql server version: %s. " , mysql_get_server_info( m_mysql_ ) );

            return true;
        }
        else
        {
            m_errno_ = mysql_errno( m_mysql_ );
            m_error_ = mysql_error( m_mysql_ );

            nlwarning( "connect mysql server failed, error = %d!" , m_errno_ );
        }
    }
    else
    {
        nlwarning( " connect mysql server failed! " );
    }

    return false;
}

// close.
void CLuaMysqlConn::close( void )
{
    if ( NULL != m_mysql_ )
    {
        mysql_close( m_mysql_ );
        m_mysql_ = NULL;
    }
}

//CLuaMysqlStmt* CLuaMysqlConn::create_stmt( char const *sql_, size_t sz_ )
//{
//    CLuaMysqlStmt *_ret = NULL;
//
//    if ( NULL != m_mysql_ && NULL != sql_ && sz_ > 0 )
//    {
//        _ret = new (std::nothrow ) CLuaMysqlStmt ;
//        if ( NULL!=_ret )
//        {
//            _ret->m_sql_.assign( sql_, sz_ );
//        }
//    }
//
//    return _ret;
//}

// execute.
int CLuaMysqlConn::execute( CLuaMysqlStmt *stmt_ )
{
    int _ret = -1;

    if ( NULL != m_mysql_ && NULL != stmt_ )
    {
        MYSQL_STMT *_stmt = mysql_stmt_init( m_mysql_ );

        if ( NULL != _stmt )
        {
            if ( 0 == mysql_stmt_prepare( _stmt, stmt_->m_sql_.c_str(), stmt_->m_sql_.size() ) )
            {
                my_bool _bl = 1;

                mysql_stmt_attr_set( _stmt, STMT_ATTR_UPDATE_MAX_LENGTH, &_bl );
                stmt_->_bind( mysql_stmt_param_count( _stmt ) );

                if ( NULL != stmt_->m_bind_ )
                {
                    if ( 0 != mysql_stmt_bind_param( _stmt, stmt_->m_bind_ ) )
                    {
                        m_errno_ = mysql_errno( m_mysql_ );
                        m_error_ = mysql_error( m_mysql_ );
                        mysql_stmt_close( _stmt );
                        _stmt = NULL;
                        if ( _procerror( stmt_->m_sql_.c_str(), "mysql_stmt_bind_param" ) )
                        {
                            _ret = execute( stmt_ );
                        }
                    }
                }

                if ( NULL != _stmt )
                {
                    if ( 0 == mysql_stmt_execute( _stmt ) )
                    {
                        _ret = (int)mysql_stmt_affected_rows( _stmt );

                        /// 检查是否有结果集;
                        MYSQL_RES *_res = mysql_stmt_result_metadata( _stmt );
                        if ( NULL != _res )
                        {
                            unsigned int server_status = m_mysql_->server_status;

                            if ( server_status & SERVER_PS_OUT_PARAMS )
                            {
                                nlwarning( "此调用存储存在返回参数值，请检查sql语句和存储过程实现!" );
                                mysql_stmt_store_result( _stmt );
                                mysql_free_result( _res );
                                mysql_stmt_free_result( _stmt );
                            }
                            else
                            {
                                nlwarning( "执行查询操作不应该返回任何结果集，请检查sql语句和存储过程实现!" );
                                mysql_stmt_store_result( _stmt );
                                mysql_free_result( _res );
                                mysql_stmt_free_result( _stmt );

                                while ( 0 == mysql_stmt_next_result( _stmt ) )
                                {
                                    _res = mysql_stmt_result_metadata( _stmt );
                                    if ( NULL != _res )
                                    {
                                        server_status = m_mysql_->server_status;
                                        mysql_stmt_store_result( _stmt );
                                        mysql_free_result( _res );
                                        mysql_stmt_free_result( _stmt );
                                        if ( server_status & SERVER_PS_OUT_PARAMS )
                                        {
                                            nlwarning( " 此调用存储存在返回参数值，请检查sql语句和存储过程实现! " );
                                            break;
                                        }
                                        else
                                        {
                                            nlwarning( " 执行查询操作不应该返回任何结果集，请检查sql语句和存储过程实现! " );
                                        }
                                    }
                                }
                            }
                        }
                        mysql_stmt_close( _stmt );
                        _stmt = NULL;
                        _res  = NULL;
                    }
                    else
                    {
                        m_errno_ = mysql_errno( m_mysql_ );
                        m_error_ = mysql_error( m_mysql_ );
                        mysql_stmt_close( _stmt );
                        _stmt = NULL;
                        if ( _procerror( stmt_->m_sql_.c_str(), "mysql_stmt_execute" ) )
                        {
                            _ret = execute( stmt_ );
                        }
                    }
                }
            }
            else
            {
                m_errno_ = mysql_errno( m_mysql_ );
                m_error_ = mysql_error( m_mysql_ );
                mysql_stmt_close( _stmt );
                _stmt = NULL;
                if ( _procerror( stmt_->m_sql_.c_str(), "mysql_stmt_prepare" ) )
                {
                    _ret = execute( stmt_ );
                }
            }
        }
        else
        {
            m_errno_ = mysql_errno( m_mysql_ );
            m_error_ = mysql_error( m_mysql_ );
            if ( _procerror( stmt_->m_sql_.c_str(), "mysql_stmt_init" ) )
            {
                _ret = execute( stmt_ );
            }
        }
    }

    return _ret;
}

// query.
int CLuaMysqlConn::query( CLuaMysqlStmt *stmt_, CLuaMysqlResult **result_ )
{
    int _ret = -1;

    if ( NULL != m_mysql_ && NULL != stmt_ && NULL != result_ )
    {
        MYSQL_STMT *_stmt = mysql_stmt_init( m_mysql_ );

        //*result_ = NULL;
        if ( NULL != _stmt )
        {
            if ( 0 == mysql_stmt_prepare( _stmt, stmt_->m_sql_.c_str(), stmt_->m_sql_.size() ) )
            {
                my_bool _bl = 1;

                mysql_stmt_attr_set( _stmt, STMT_ATTR_UPDATE_MAX_LENGTH, &_bl );
                stmt_->_bind( mysql_stmt_param_count( _stmt ) );

                if ( NULL != stmt_->m_bind_ )
                {
                    if ( 0 != mysql_stmt_bind_param( _stmt, stmt_->m_bind_ ) )
                    {
                        m_errno_ = mysql_errno( m_mysql_ );
                        m_error_ = mysql_error( m_mysql_ );
                        mysql_stmt_close( _stmt );
                        _stmt = NULL;
                        if ( _procerror( stmt_->m_sql_.c_str(), "mysql_stmt_bind_param" ) )
                        {
                            _ret = query( stmt_, result_ );
                        }
                    }
                }

                if ( NULL != _stmt )
                {
                    if ( 0 == mysql_stmt_execute( _stmt ) )
                    {
                        _ret = (int)mysql_stmt_affected_rows( _stmt );
                        MYSQL_RES *_res = mysql_stmt_result_metadata( _stmt );
                        if ( NULL != _res )
                        {
                            unsigned int server_status = m_mysql_->server_status;

                            if ( 0 == mysql_stmt_store_result( _stmt ) )
                            {
                                CLuaMysqlResult *_result = *result_;

                                if ( *result_ == NULL )
                                {
                                    nlwarning("CLuaMysqlResult == NULL");
                                    _result = new( std::nothrow ) CLuaMysqlResult();
                                    *result_ = _result;
                                }

                                if ( server_status & SERVER_PS_OUT_PARAMS )
                                {
                                    _ret = 0;
                                    _result->_retval( _stmt, _res );
                                    mysql_free_result( _res );
                                    mysql_stmt_free_result( _stmt );
                                }
                                else
                                {
                                    _ret = (int)mysql_stmt_num_rows( _stmt );
                                    _result->_init( _stmt, _res );
                                    mysql_free_result( _res );
                                    mysql_stmt_free_result( _stmt );

                                    /// 检查是否还有结果集;
                                    while ( 0 == mysql_stmt_next_result( _stmt ) )
                                    {
                                        _res = mysql_stmt_result_metadata( _stmt );
                                        if ( NULL != _res )
                                        {
                                            server_status = m_mysql_->server_status;
                                            if ( 0 == mysql_stmt_store_result( _stmt ) )
                                            {
                                                if ( server_status & SERVER_PS_OUT_PARAMS )
                                                {
                                                    _result->_retval( _stmt, _res );
                                                    mysql_free_result( _res );
                                                    mysql_stmt_free_result( _stmt );
                                                    break;
                                                }
                                                else
                                                {
                                                    nlwarning( "执行查询操作不应该返回任何结果集，请检查sql语句和存储过程实现!" );
                                                    mysql_stmt_store_result( _stmt );
                                                    mysql_free_result( _res );
                                                    mysql_stmt_free_result( _stmt );
                                                }
                                            }
                                        }
                                    }
                                }
                                mysql_stmt_close( _stmt );
                                _stmt = NULL;
                            }
                            else
                            {
                                m_errno_ = mysql_errno( m_mysql_ );
                                m_error_ = mysql_error( m_mysql_ );
                                mysql_stmt_close( _stmt );
                                _stmt = NULL;
                                if ( _procerror( stmt_->m_sql_.c_str(), "mysql_stmt_store_result" ) )
                                {
                                    _ret = query( stmt_, result_ );
                                }
                                else
                                {
                                    _ret = -1;
                                }
                            }
                        }
                        else
                        {
                            mysql_stmt_close( _stmt );
                            _stmt = NULL;
                        }
                    }
                    else
                    {
                        m_errno_ = mysql_errno( m_mysql_ );
                        m_error_ = mysql_error( m_mysql_ );
                        mysql_stmt_close( _stmt );
                        _stmt = NULL;
                        if ( _procerror( stmt_->m_sql_.c_str(), "mysql_stmt_execute" ) )
                        {
                            _ret = query( stmt_, result_ );
                        }
                    }
                }
            }
            else
            {
                m_errno_ = mysql_errno( m_mysql_ );
                m_error_ = mysql_error( m_mysql_ );
                mysql_stmt_close( _stmt );
                _stmt = NULL;
                if ( _procerror( stmt_->m_sql_.c_str(), "mysql_stmt_prepare" ) )
                {
                    _ret = query( stmt_, result_ );
                }
            }
        }
        else
        {
            m_errno_ = mysql_errno( m_mysql_ );
            m_error_ = mysql_error( m_mysql_ );
            if ( _procerror( stmt_->m_sql_.c_str(), "mysql_stmt_init" ) )
            {
                _ret = query( stmt_, result_ );
            }
        }
    }

    return _ret;
}

// reconnect.
bool CLuaMysqlConn::_reconnect( void )
{
    if ( NULL != m_mysql_ && 0 == mysql_ping( m_mysql_ ) )
    {
        return true;
    }
    else
    {
        nlwarning( "try to reconnect mysql server...\n" );
        close();
        m_mysql_ = mysql_init( NULL );
        if ( NULL != m_mysql_ )
        {
            // set character.
            mysql_options( m_mysql_, MYSQL_SET_CHARSET_NAME, "utf8" );
            if ( NULL != mysql_real_connect( m_mysql_,
                                             m_host_.c_str(),
                                             m_user_.c_str(),
                                             m_pwd_.c_str(),
                                             m_db_.c_str(),
                                             m_port_,
                                             NULL,
                                             0 ) )
            {
                // set autocommit.
                mysql_autocommit( m_mysql_, 1 );
                // set character set.
                mysql_set_character_set( m_mysql_, "utf8" );

                mysql_query(m_mysql_, "SET character_set_client=binary");

                nlinfo( "reconnect mysql server succeed!" );
                nlinfo( "mysql client library: %s." , mysql_get_client_info() );
                nlinfo( "mysql server version: %s. " , mysql_get_server_info( m_mysql_ ) );

                return true;
            }
            else
            {
                nlwarning( "reconnect mysql server failed, error = %d! " , mysql_errno( m_mysql_ ) );
            }
        }
        else
        {
            nlwarning( " reconnect mysql server failed! " );
        }

    }

    return false;
}

// process error.
bool CLuaMysqlConn::_procerror( char const *op_/* = NULL*/, char const *func_/* = NULL*/ )
{
    bool _ret = false;

    if ( NULL != op_ && NULL != func_ )
    {
        nlinfo("op = %s, func = %s.\n" , op_ , func_ );
    }
    else if ( NULL != op_ )
    {
        nlinfo( " op = %s. " , op_ );
    }

    switch ( m_errno_ )
    {
    case CR_SERVER_GONE_ERROR:
        {
            nlwarning( "mysql server has gone away, errno = %d!"  , m_errno_ );

            if ( _reconnect() )
            {
                _ret = true;
            }
            else
            {
                if ( NULL != m_mysql_ )
                {
                    m_errno_ = mysql_errno( m_mysql_ );
                    m_error_ = mysql_error( m_mysql_ );
                    //OSSleep( ERROR_SLEEP_TIME );
                    _ret = _procerror( op_, __FUNCTION__ );
                }
            }
        }
        break;

    case CR_SERVER_LOST:
        {
            nlwarning( "lost the connection to mysql server, errno = %d!\n", m_errno_ );

            if ( _reconnect() )
            {
                _ret = true;
            }
            else
            {
                if ( NULL != m_mysql_ )
                {
                    m_errno_ = mysql_errno( m_mysql_ );
                    m_error_ = mysql_error( m_mysql_ );
                    //OSSleep( ERROR_SLEEP_TIME );
                    _ret = _procerror( op_, __FUNCTION__ );
                }
            }
        }
        break;

    case CR_INVALID_CONN_HANDLE:
        {
            nlwarning( "invalid connection handle, errno = %d!\n", m_errno_ );

            if ( _reconnect() )
            {
                _ret = true;
            }
            else
            {
                if ( NULL != m_mysql_ )
                {
                    m_errno_ = mysql_errno( m_mysql_ );
                    m_error_ = mysql_error( m_mysql_ );
                    //OSSleep( ERROR_SLEEP_TIME );
                    _ret = _procerror( op_, __FUNCTION__ );
                }
            }
        }
        break;

    case CR_SERVER_LOST_EXTENDED:
        {
            nlwarning( "lost the connection to mysql server error  = %d! " , m_errno_ );

            if ( _reconnect() )
            {
                _ret = true;
            }
            else
            {
                if ( NULL != m_mysql_ )
                {
                    m_errno_ = mysql_errno( m_mysql_ );
                    m_error_ = mysql_error( m_mysql_ );
                    //OSSleep( ERROR_SLEEP_TIME );
                    _ret = _procerror( op_, __FUNCTION__ );
                }
            }
        }
        break;

    default:
        {
            nlwarning( "%s, error = %d! " , m_error_.c_str(), m_errno_ );
        }
        break;
    }

    return _ret;
}

// errno.
int CLuaMysqlConn::error( void ) const
{
    return m_errno_;
}

void forLuaMysqlConnForceLink()
{
    ScriptMgr.ExecString( "" );
}

namespace bin
{
    BEGIN_SCRIPT_CLASS( MysqlConn, CLuaMysqlConn )

        DEFINE_CLASS_FUNCTION( Connect, bool, (CScriptTable& tb_msg))
        {
            if( tb_msg.IsReferd() )
            {
                std::string     myhost;
                std::string     myuser;
                std::string     mypwd;
                std::string     mydb;
                lua_Integer     myport;

                tb_msg.Get(1, myhost);
                tb_msg.Get(2, myuser);
                tb_msg.Get(3, mypwd);
                tb_msg.Get(4, mydb);
                tb_msg.Get(5, myport);

                r = obj->connect( myhost.c_str(), myuser.c_str(), mypwd.c_str(), mydb.c_str(), myport );
            }

            return 1;
        }

        DEFINE_CLASS_FUNCTION( Close, void, ())
        {
            obj->close();
            return 1;
        }

        DEFINE_CLASS_FUNCTION( Query, int, (CLuaMysqlStmt* pStmt, CLuaMysqlResult* pResult))
        {
            r = obj->query( pStmt, &pResult );
            return 1;
        }

        DEFINE_CLASS_FUNCTION( Exec, int, (CLuaMysqlStmt* pStmt))
        {
            r = obj->execute( pStmt );
            return 1;
        }

        DEFINE_STATIC_FUNCTION(NewInstance, CLuaMysqlConn*, ())
        {
            r = new CLuaMysqlConn();
            r->GetScriptObject().SetDelByScr(true);

            return 1;
        }

    END_SCRIPT_CLASS()



}
