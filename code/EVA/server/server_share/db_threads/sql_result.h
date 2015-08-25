#ifndef _SQL_RESULT_H_
#define _SQL_RESULT_H_

#include <nel/misc/types_nl.h>
#include <vector>
#include <mysql.h>

#include "sql_string.h"

namespace nl
{

class sql_result
{
public:
    friend class sql_conn;

    // close.
    void release( void );

    // next.
    bool next( void );

    // count.
    size_t count( void ) const;

    // get bool value.
    my_bool get_bool( size_t idx_ );

    // get int8 value.
    sint8 get_int8( size_t idx_ );

    // get uint8 value.
    uint8 get_uint8( size_t idx_ );

    // get int16 value.
    sint16 get_int16( size_t idx_ );

    // get uint16 value.
    uint16 get_uint16( size_t idx_ );

    // get int32 value.
    sint32 get_int32( size_t idx_ );

    // get uint32 value.
    uint32 get_uint32( size_t idx_ );

    // get int64 value.
    sint64 get_int64( size_t idx_ );

    // get uint64 value.
    uint64 get_uint64( size_t idx_ );

    // get float value.
    float get_float( size_t idx_ );

    // get double value.
    double get_double( size_t idx_ );

    // get string.
    std::pair<size_t, char const*> get_string( size_t idx_ );

    // get blob.
    std::pair<size_t, void const*> get_blob( size_t idx_ );

protected:
    sql_result( void );
    ~sql_result( void );

    typedef struct _result
    {
        nl::sql_string    buf;
        unsigned long     size;
        enum_field_types  type;
        my_bool           is_null;

        _result( void )
            : size( 0 ),
              type( MYSQL_TYPE_NULL ),
              is_null( 0 )
        {}
    } result_t;

    // initialize.
    bool _init( MYSQL_STMT *stmt_, MYSQL_RES *res_ );

    // returned output parameters.
    bool _retval( MYSQL_STMT *stmt_, MYSQL_RES *res_ );

    uint32 row_offset() { return m_CurrRow<0?0:(m_CurrRow*m_FieldCount); }

private:

    typedef std::vector<_result>        TResult;
    TResult             m_Result;

    uint32              m_RowCount;
    sint32              m_CurrRow;
    uint32              m_FieldCount;

    //std::map<size_t, _result>                        m_ret_; // out parameters.
};

} // namespace ex

#endif // _SQL_RESULT_H_