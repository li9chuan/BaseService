#include "mysql_stmt.h"
#include "mysql_conn.h"
#include "mysql_result.h"
#include <cstdio>
#include <errmsg.h>


CLuaMysqlStmt::CLuaMysqlStmt(  )
    : m_bind_( NULL ),
      m_count_( 0 ),
      m_idx(0)
{
    m_Params.resize(64);
}

CLuaMysqlStmt::~CLuaMysqlStmt( void )
{
    if ( NULL != m_bind_ )
    {
        free( m_bind_ );
        m_bind_  = NULL;
        m_count_ = 0;
        m_idx = 0;
    }
}

// close.
void CLuaMysqlStmt::release( void )
{
    delete this;
}

// set bool value.
void CLuaMysqlStmt::set_bool( my_bool bval_ )
{
    param_t& param = m_Params[m_idx];

    param.buf.assign( (char)bval_, 1 );
    param.type = MYSQL_TYPE_TINY;
    param.is_unsigned = 0;
    ++m_idx;
}

// set int8 value.
void CLuaMysqlStmt::set_int8( sint8 i8_ )
{
    param_t& param = m_Params[m_idx];

    param.buf.assign( (char)i8_, 1 );
    param.type = MYSQL_TYPE_TINY;
    param.is_unsigned = 0;
    ++m_idx;
}

// set uint8 value.
void CLuaMysqlStmt::set_uint8( uint8 ui8_ )
{
    param_t& param = m_Params[m_idx];

    param.buf.assign( (char)ui8_, 1 );
    param.type = MYSQL_TYPE_TINY;
    param.is_unsigned = UNSIGNED_FLAG;
    ++m_idx;
}

// set int16 value.
void CLuaMysqlStmt::set_int16( sint16 i16_ )
{
    param_t& param = m_Params[m_idx];

    param.buf.assign( (char*)&i16_, 2 );
    param.type = MYSQL_TYPE_SHORT;
    param.is_unsigned = 0;
    ++m_idx;
}

// set uint16 value.
void CLuaMysqlStmt::set_uint16( uint16 ui16_ )
{
    param_t& param = m_Params[m_idx];

    param.buf.assign( (char*)&ui16_, 2 );
    param.type = MYSQL_TYPE_SHORT;
    param.is_unsigned = UNSIGNED_FLAG;
    ++m_idx;
}

// set int32 value.
void CLuaMysqlStmt::set_int32( sint32 i32_ )
{
    param_t& param = m_Params[m_idx];

    param.buf.assign( (char*)&i32_, 4 );
    param.type = MYSQL_TYPE_LONG;
    param.is_unsigned = 0;
    ++m_idx;
}

// set uint32 value.
void CLuaMysqlStmt::set_uint32( uint32 ui32_ )
{
    param_t& param = m_Params[m_idx];

    param.buf.assign( (char*)&ui32_, 4 );
    param.type = MYSQL_TYPE_LONG;
    param.is_unsigned = UNSIGNED_FLAG;
    ++m_idx;
}

// set int64 value.
void CLuaMysqlStmt::set_int64( sint64 i64_ )
{
    param_t& param = m_Params[m_idx];

    param.buf.assign( (char*)&i64_, 8 );
    param.type = MYSQL_TYPE_LONGLONG;
    param.is_unsigned = 0;
    ++m_idx;
}

// set uint64 value.
void CLuaMysqlStmt::set_uint64( uint64 ui64_ )
{
    param_t& param = m_Params[m_idx];

    param.buf.assign( (char*)&ui64_, 8 );
    param.type = MYSQL_TYPE_LONGLONG;
    param.is_unsigned = UNSIGNED_FLAG;
    ++m_idx;
}

// set float value.
void CLuaMysqlStmt::set_float( float fval_ )
{
    param_t& param = m_Params[m_idx];

    param.buf.assign( (char*)&fval_, 4 );
    param.type = MYSQL_TYPE_FLOAT;
    param.is_unsigned = 0;
    ++m_idx;
}

// set double value.
void CLuaMysqlStmt::set_double( double dval_ )
{
    param_t& param = m_Params[m_idx];

    param.buf.assign( (char*)&dval_, 8 );
    param.type = MYSQL_TYPE_DOUBLE;
    param.is_unsigned = 0;
    ++m_idx;
}

// set string.
void CLuaMysqlStmt::set_string( char const *str_ )
{
    param_t& param = m_Params[m_idx];

    param.buf.assign( str_ );
    param.type = MYSQL_TYPE_STRING;
    param.is_unsigned = 0;
    ++m_idx;
}

// set string.
void CLuaMysqlStmt::set_string( char const *str_, size_t sz_ )
{
    param_t& param = m_Params[m_idx];

    param.buf.assign( str_, sz_ );
    param.type = MYSQL_TYPE_STRING;
    param.is_unsigned = 0;
    ++m_idx;
}

// set blob.
void CLuaMysqlStmt::set_blob( void const *ptr_, size_t sz_ )
{
    param_t& param = m_Params[m_idx];

    param.buf.assign( (char*)ptr_, sz_ );
    param.type = MYSQL_TYPE_BLOB;
    param.is_unsigned = 0;
    ++m_idx;
}

void CLuaMysqlStmt::set_tinyblob( void const *ptr_, size_t sz_ )
{
    param_t& param = m_Params[m_idx];

    param.buf.assign( (char*)ptr_, sz_ );
    param.type = MYSQL_TYPE_TINY_BLOB;
    param.is_unsigned = 0;
    ++m_idx;
}

// bind parameters.
void CLuaMysqlStmt::_bind( size_t count_ )
{
    if ( m_count_ < count_ )
    {
        if ( NULL != m_bind_ )
        {
            free( m_bind_ );
            m_bind_ = NULL;
        }
        m_bind_ = (MYSQL_BIND*)malloc( count_ * sizeof(MYSQL_BIND) );
        m_count_ = count_;
    }

    if ( NULL != m_bind_ )
    {
        memset( m_bind_, 0, m_count_ * sizeof( MYSQL_BIND ) );

        for ( size_t _idx = 0; _idx < count_; ++ _idx )
        {
            param_t& param = m_Params[_idx];

            m_bind_[_idx].buffer        = (void*)param.buf.c_str();
            m_bind_[_idx].buffer_length = param.buf.size();
            m_bind_[_idx].buffer_type   = param.type;
            m_bind_[_idx].is_unsigned   = param.is_unsigned;
            m_bind_[_idx].is_null_value = param.is_null_val;
        }
    }
}




namespace bin
{
    BEGIN_SCRIPT_CLASS( MysqlStmt, CLuaMysqlStmt )

        DEFINE_CLASS_FUNCTION( SetBool, void, (bool lua_bool))
        {
            obj->set_bool(lua_bool);
            return 1;
        }

        DEFINE_CLASS_FUNCTION( SetInt8, void, (lua_Integer lua_val))
        {
            obj->set_int8(lua_val);
            return 1;
        }

        DEFINE_CLASS_FUNCTION( SetUint8, void, (lua_Integer lua_val))
        {
            obj->set_uint8(lua_val);
            return 1;
        }

        DEFINE_CLASS_FUNCTION( SetInt16, void, (lua_Integer lua_val))
        {
            obj->set_int16(lua_val);
            return 1;
        }

        DEFINE_CLASS_FUNCTION( SetUint16, void, (lua_Integer lua_val))
        {
            obj->set_uint16(lua_val);
            return 1;
        }

        DEFINE_CLASS_FUNCTION( SetInt32, void, (lua_Integer lua_val))
        {
            obj->set_int32(lua_val);
            return 1;
        }

        DEFINE_CLASS_FUNCTION( SetUint32, void, (lua_Integer lua_val))
        {
            obj->set_uint32(lua_val);
            return 1;
        }

        DEFINE_CLASS_FUNCTION( SetInt64, void, (lua_Integer lua_val))
        {
            obj->set_int64(lua_val);
            return 1;
        }

        DEFINE_CLASS_FUNCTION( SetUint64, void, (lua_Integer lua_int))
        {
            obj->set_uint64(lua_int);
            return 1;
        }

        DEFINE_CLASS_FUNCTION( SetFloat, void, (lua_Number lua_val))
        {
            obj->set_float(lua_val);
            return 1;
        }

        DEFINE_CLASS_FUNCTION( SetDouble, void, (lua_Number lua_val))
        {
            obj->set_double(lua_val);
            return 1;
        }

        DEFINE_CLASS_FUNCTION( SetString, void, (std::string lua_str))
        {
            obj->set_string(lua_str.c_str(), lua_str.size());
            return 1;
        }

        DEFINE_CLASS_FUNCTION( SetBlob, void, (const char* _buffer, int _len))
        {
            obj->set_blob(_buffer,_len);
            return 1;
        }

        DEFINE_STATIC_FUNCTION(NewInstance, CLuaMysqlStmt*, (std::string sql))
        {
            CLuaMysqlStmt* pStmt = new (std::nothrow ) CLuaMysqlStmt;

            if ( NULL!=pStmt )
            {
                pStmt->set_sql( sql );
                r = pStmt;
                r->GetScriptObject().SetDelByScr(true);
            }

            return 1;
        }

    END_SCRIPT_CLASS()




}
