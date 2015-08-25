#include "sql_stmt.h"
#include "sql_conn.h"
#include "sql_result.h"
#include <cstdio>
#include <errmsg.h>

namespace nl
{

sql_stmt::sql_stmt(  )
    : m_bind_( NULL ),
      m_count_( 0 )
{
    m_Params.resize(64);
}

sql_stmt::~sql_stmt( void )
{
    if ( NULL != m_bind_ )
    {
        free( m_bind_ );
        m_bind_  = NULL;
        m_count_ = 0;
    }
}

// close.
void sql_stmt::release( void )
{
    delete this;
}

// set bool value.
void sql_stmt::set_bool( size_t idx_, my_bool bval_ )
{
    param_t& param = m_Params[idx_];

    param.buf.assign( (char)bval_, 1 );
    param.type = MYSQL_TYPE_TINY;
    param.is_unsigned = 0;
}

// set int8 value.
void sql_stmt::set_int8( size_t idx_, sint8 i8_ )
{
    param_t& param = m_Params[idx_];

    param.buf.assign( (char)i8_, 1 );
    param.type = MYSQL_TYPE_TINY;
    param.is_unsigned = 0;
}

// set uint8 value.
void sql_stmt::set_uint8( size_t idx_, uint8 ui8_ )
{
    param_t& param = m_Params[idx_];

    param.buf.assign( (char)ui8_, 1 );
    param.type = MYSQL_TYPE_TINY;
    param.is_unsigned = UNSIGNED_FLAG;
}

// set int16 value.
void sql_stmt::set_int16( size_t idx_, sint16 i16_ )
{
    param_t& param = m_Params[idx_];

    param.buf.assign( (char*)&i16_, 2 );
    param.type = MYSQL_TYPE_SHORT;
    param.is_unsigned = 0;
}

// set uint16 value.
void sql_stmt::set_uint16( size_t idx_, uint16 ui16_ )
{
    param_t& param = m_Params[idx_];

    param.buf.assign( (char*)&ui16_, 2 );
    param.type = MYSQL_TYPE_SHORT;
    param.is_unsigned = UNSIGNED_FLAG;
}

// set int32 value.
void sql_stmt::set_int32( size_t idx_, sint32 i32_ )
{
    param_t& param = m_Params[idx_];

    param.buf.assign( (char*)&i32_, 4 );
    param.type = MYSQL_TYPE_LONG;
    param.is_unsigned = 0;
}

// set uint32 value.
void sql_stmt::set_uint32( size_t idx_, uint32 ui32_ )
{
    param_t& param = m_Params[idx_];

    param.buf.assign( (char*)&ui32_, 4 );
    param.type = MYSQL_TYPE_LONG;
    param.is_unsigned = UNSIGNED_FLAG;
}

// set int64 value.
void sql_stmt::set_int64( size_t idx_, sint64 i64_ )
{
    param_t& param = m_Params[idx_];

    param.buf.assign( (char*)&i64_, 8 );
    param.type = MYSQL_TYPE_LONGLONG;
    param.is_unsigned = 0;
}

// set uint64 value.
void sql_stmt::set_uint64( size_t idx_, uint64 ui64_ )
{
    param_t& param = m_Params[idx_];

    param.buf.assign( (char*)&ui64_, 8 );
    param.type = MYSQL_TYPE_LONGLONG;
    param.is_unsigned = UNSIGNED_FLAG;
}

// set float value.
void sql_stmt::set_float( size_t idx_, float fval_ )
{
    param_t& param = m_Params[idx_];

    param.buf.assign( (char*)&fval_, 4 );
    param.type = MYSQL_TYPE_FLOAT;
    param.is_unsigned = 0;
}

// set double value.
void sql_stmt::set_double( size_t idx_, double dval_ )
{
    param_t& param = m_Params[idx_];

    param.buf.assign( (char*)&dval_, 8 );
    param.type = MYSQL_TYPE_DOUBLE;
    param.is_unsigned = 0;
}

// set string.
void sql_stmt::set_string( size_t idx_, char const *str_ )
{
    param_t& param = m_Params[idx_];

    param.buf.assign( str_ );
    param.type = MYSQL_TYPE_STRING;
    param.is_unsigned = 0;
}

// set string.
void sql_stmt::set_string( size_t idx_, char const *str_, size_t sz_ )
{
    param_t& param = m_Params[idx_];

    param.buf.assign( str_, sz_ );
    param.type = MYSQL_TYPE_STRING;
    param.is_unsigned = 0;
}

// set blob.
void sql_stmt::set_blob( size_t idx_, void const *ptr_, size_t sz_ )
{
    param_t& param = m_Params[idx_];

    param.buf.assign( (char*)ptr_, sz_ );
    param.type = MYSQL_TYPE_BLOB;
    param.is_unsigned = 0;
}

void sql_stmt::set_tinyblob( size_t idx_, void const *ptr_, size_t sz_ )
{
    param_t& param = m_Params[idx_];

    param.buf.assign( (char*)ptr_, sz_ );
    param.type = MYSQL_TYPE_TINY_BLOB;
    param.is_unsigned = 0;
}

// bind parameters.
void sql_stmt::_bind( size_t count_ )
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

} // namespace ex
