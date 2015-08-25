#include "sql_worker.h"
#include "game_share/utils.h"

namespace nl
{

sql_worker::sql_worker( void )
{
    m_stmt_.resize(STMT_MAX, NULL);
}

sql_worker::~sql_worker( void )
{
    close();
}

// connect.
bool sql_worker::connect( char const *host_,
                          char const *user_,
                          char const *pwd_,
                          char const *db_,
                          int         port_ )
{
    return m_conn_.connect( host_, user_, pwd_, db_, port_ );
}

// close.
void sql_worker::close( void )
{
    for ( uint i=0; i<STMT_MAX; ++i )
    {
        if ( NULL != m_stmt_[i] )
        {
            m_stmt_[i]->release();
            m_stmt_[i] = NULL;
        }
    }

    m_stmt_.clear();
    m_conn_.close();
}

// add a prepared statement.
void sql_worker::add_stmt( size_t type_, char const *sql_ )
{
    nlassertd( type_<STMT_MAX );

    if ( NULL != sql_ )
    {
        sql_stmt *_stmt = m_conn_.create_stmt( sql_, strlen( sql_ ) );

        if ( NULL != _stmt )
        {
            if ( m_stmt_[type_] != NULL )
            {
                nlwarning("sql_worker::add_stmt .");
                m_stmt_[type_]->release();
            }

            m_stmt_[type_] = _stmt;
        }
    }
}

// get a prepared statement by type.
sql_stmt* sql_worker::get_stmt( size_t type_ )
{
    nlassertd( type_<STMT_MAX );
    return m_stmt_[type_];
}

// execute.
int sql_worker::execute( sql_stmt *stmt_ )
{
    return m_conn_.execute( stmt_ );
}

// query.
int sql_worker::query( sql_stmt *stmt_, sql_result **result_ )
{
    return m_conn_.query( stmt_, result_ );
}

// errno.
int sql_worker::error( void ) const
{
    return m_conn_.error();
}

} // namepsace ex
