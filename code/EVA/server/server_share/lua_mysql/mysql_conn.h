#ifndef LUA_MYSQL_CONN_H
#define LUA_MYSQL_CONN_H

#	ifdef _MSC_VER
    #   if _MSC_VER < 1700
        #include <my_global.h>
    #endif
#endif

#include <mysql.h>
#include <cstdio>
#include <string>
#include <server_share/bin_luabind/Public.hpp>

#define ERROR_SLEEP_TIME 3000   /// 执行SQL语句时出错，再次尝试执行间隔时间（毫秒）

class CLuaMysqlStmt;
class CLuaMysqlResult;

class CLuaMysqlConn
{
    DECLARE_SCRIPT_CLASS();
public:
    CLuaMysqlConn( void );
    ~CLuaMysqlConn( void );

    // connect.
    bool connect( char const *host_,
                  char const *user_,
                  char const *pwd_,
                  char const *db_,
                  int         port_ );

    // close.
    void close( void );

    // create statement.
    //CLuaMysqlStmt* create_stmt( char const *sql_, size_t sz_ );

    // execute.
    int execute( CLuaMysqlStmt *stmt_ );

    // query.
    int query( CLuaMysqlStmt *stmt_, CLuaMysqlResult **result_ );

    // errno.
    int error( void ) const;

private:
    // reconnect.
    bool _reconnect( void );

    // process error.
    bool _procerror( char const *op_ = NULL, char const *func_ = NULL );

private:
    MYSQL        *m_mysql_;
    std::string   m_host_;
    std::string   m_user_;
    std::string   m_pwd_;
    std::string   m_db_;
    int           m_port_;
    int           m_errno_;
    std::string   m_error_;
};


#endif // LUA_MYSQL_CONN_H
