#ifndef _SQL_WORKER_H_
#define _SQL_WORKER_H_

#include "sql_conn.h"
#include "sql_stmt.h"
#include "sql_result.h"

#include <vector>

namespace nl
{

class sql_worker
{
public:
    sql_worker( void );
    ~sql_worker( void );

    // connect.
    bool connect( char const *host_,
                  char const *user_,
                  char const *pwd_,
                  char const *db_,
                  int         port_ );

    // close.
    void close( void );

    // add a prepared statement.
    void add_stmt( size_t type_, char const *sql_ );

    // get a prepared statement by type.
    sql_stmt* get_stmt( size_t type_ );

    // execute.
    int execute( sql_stmt *stmt_ );

    // query.
    int query( sql_stmt *stmt_, sql_result **result_ );

    // errno.
    int error( void ) const;

private:

    enum
    {
        STMT_MAX = 512,
    };

    sql_conn                    m_conn_;
    //std::map<size_t, sql_stmt*> m_stmt_;
    std::vector<sql_stmt*>      m_stmt_;
};

} // namespace ex

#endif // _SQL_WORKER_H_
