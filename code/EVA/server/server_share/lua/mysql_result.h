#ifndef LUA_MYSQL_RESULT_H
#define LUA_MYSQL_RESULT_H

#include <nel/misc/types_nl.h>
#include <vector>
#include <mysql.h>

#include "mysql_string.h"
#include <server_share/bin_luabind/Public.hpp>

class CLuaMysqlResult
{
    DECLARE_SCRIPT_CLASS();
public:

    friend class CLuaMysqlConn;

    // close.
    void release( void );

    // next.
    bool next( void );

    // count.
    size_t count( void ) const;

    // get bool value.
    my_bool get_bool();

    // get int8 value.
    sint8 get_int8();

    // get uint8 value.
    uint8 get_uint8();

    // get int16 value.
    sint16 get_int16();

    // get uint16 value.
    uint16 get_uint16();

    // get int32 value.
    sint32 get_int32();

    // get uint32 value.
    uint32 get_uint32();

    // get int64 value.
    sint64 get_int64();

    // get uint64 value.
    uint64 get_uint64();

    // get float value.
    float get_float();

    // get double value.
    double get_double();

    // get string.
    std::pair<size_t, char const*> get_string();

    // get blob.
    std::pair<size_t, void const*> get_blob();


    CLuaMysqlResult( void );
    ~CLuaMysqlResult( void );

protected:

    typedef struct _result
    {
        CLuaMysqlString   buf;
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

    uint32              m_Idx;

    //std::map<size_t, _result>                        m_ret_; // out parameters.
};


#endif // LUA_MYSQL_RESULT_H