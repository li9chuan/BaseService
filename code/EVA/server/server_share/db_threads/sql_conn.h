#ifndef _SQL_CONN_H_
#define _SQL_CONN_H_

#	ifdef _MSC_VER
    #   if _MSC_VER < 1700
        #include <my_global.h>
    #endif
#endif

#include <mysql.h>
#include <cstdio>
#include <string>

#define ERROR_SLEEP_TIME 3000   /// 执行SQL语句时出错，再次尝试执行间隔时间（毫秒）

namespace nl
{
class sql_stmt;
class sql_result;

class sql_conn
{
public:
    sql_conn( void );
    ~sql_conn( void );

    // connect.
    bool connect( char const *host_,
                  char const *user_,
                  char const *pwd_,
                  char const *db_,
                  int         port_ );

    // close.
    void close( void );

    // create statement.
    sql_stmt* create_stmt( char const *sql_, size_t sz_ );

    // execute.
    int execute( sql_stmt *stmt_ );

    // query.
    int query( sql_stmt *stmt_, sql_result **result_ );

    // errno.
    int error( void ) const;

protected:
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

} // namespace ex

#endif // _SQL_CONN_H_