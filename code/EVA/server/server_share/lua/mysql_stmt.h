#ifndef LUA_MSQL_PSTMT_H
#define LUA_MSQL_PSTMT_H

#include "mysql_result.h"
#include <server_share/bin_luabind/Public.hpp>

class CLuaMysqlStmt
{
    DECLARE_SCRIPT_CLASS();
public:
    friend class CLuaMysqlConn;

    // release.
    void release( void );

    // set bool value.
    void set_bool( my_bool bval_ );

    // set int8 value.
    void set_int8( sint8 i8_ );

    // set uint8 value.
    void set_uint8( uint8 ui8_ );

    // set int16 value.
    void set_int16( sint16 i16_ );

    // set uint16 value.
    void set_uint16( uint16 ui16_ );

    // set int32 value.
    void set_int32( sint32 i32_ );

    // set uint32 value.
    void set_uint32( uint32 ui32_ );

    // set int64 value.
    void set_int64( sint64 i64_ );

    // set uint64 value.
    void set_uint64( uint64 ui64_ );

    // set float value.
    void set_float( float fval_ );

    // set double value.
    void set_double( double dval_ );

    // set string.
    void set_string( char const *str_ );

    // set string.
    void set_string( char const *str_, size_t sz_ );

    // set blob.
    void set_blob( void const *ptr_, size_t sz_ );

    void set_tinyblob( void const *ptr_, size_t sz_ );

    void set_sql( std::string& sql ) { m_sql_.assign(sql.c_str(),sql.size()); }
    
//protected:
    CLuaMysqlStmt( void );
    ~CLuaMysqlStmt( void );

protected:
    // bind parameters.
    void _bind( size_t count_ );

private:

    typedef struct _param
    {
        CLuaMysqlString   buf;
        enum_field_types type;
        my_bool          is_unsigned;
        my_bool          is_null_val;

        _param( void )
            : type( MYSQL_TYPE_NULL ),
              is_unsigned( 0 ),
              is_null_val( 0 )
        {}

        _param( enum_field_types in_type, my_bool in_unsigned )
            : type( in_type ),
            is_unsigned( in_unsigned ),
            is_null_val( 0 )
        {}

    } param_t;

    MYSQL_BIND                  *m_bind_;
    size_t                      m_count_;   // count of MYSQL_BIND
    std::vector<param_t>        m_Params;
    std::string                 m_sql_;
    uint32                      m_idx;
};


#endif // LUA_MSQL_PSTMT_H