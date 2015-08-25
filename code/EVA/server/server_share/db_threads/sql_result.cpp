#include "sql_result.h"
#include "sql_stmt.h"
#include <cstdio>

namespace nl
{

sql_result::sql_result( void ):m_RowCount(0),m_CurrRow(-1),m_FieldCount(0)
{
    //m_cur_ = m_res_.end();
}

sql_result::~sql_result( void )
{
    //m_res_.clear();
    //m_cur_ = m_res_.end();
    //m_row_ = 0;
}

// close
void sql_result::release( void )
{
    delete this;
}

size_t sql_result::count( void ) const
{
    return m_RowCount;
}

bool sql_result::next( void )
{
    bool _ret = false;

    if ( m_CurrRow+1 < m_RowCount )
    {
        ++m_CurrRow;
        _ret = true;
    }
    //if ( 0 == m_row_ &&
    //     m_row_ < m_res_.size() &&
    //     m_cur_ != m_res_.end() )
    //{
    //    ++ m_row_;
    //    _ret = true;
    //}
    //else
    //{
    //    if ( m_cur_ != m_res_.end() )
    //    {
    //        ++ m_cur_;
    //        if ( m_cur_ != m_res_.end() )
    //        {
    //            ++ m_row_;
    //            _ret = true;
    //        }
    //    }
    //}

    return _ret;
}

my_bool sql_result::get_bool( size_t idx_ )
{
    bool _ret = false;
    uint32 curr_idx = row_offset()+idx_;

    //if ( m_cur_ != m_res_.end() && idx_ < m_cur_->size() )
    {
        result_t& _res = m_Result[curr_idx];

        if ( !_res.is_null )
        {
            switch ( _res.type )
            {
            case MYSQL_TYPE_TINY:
                _ret = ( 0 != ( *(uint8*)_res.buf.c_str() ) );
                break;
            case MYSQL_TYPE_SHORT:
                _ret = ( 0 != ( *(uint16*)_res.buf.c_str() ) );
                break;
            case MYSQL_TYPE_INT24:
                _ret = ( 0 != ( *(uint32*)_res.buf.c_str() ) );
                break;
            case MYSQL_TYPE_LONG:
                _ret = ( 0 != ( *(uint32*)_res.buf.c_str() ) );
                break;
            case MYSQL_TYPE_LONGLONG:
                _ret = ( 0 != ( *(uint64*)_res.buf.c_str() ) );
                break;
            case MYSQL_TYPE_FLOAT:
                _ret = ( 0 != ( *(uint32*)_res.buf.c_str() ) );
                break;
            case MYSQL_TYPE_DOUBLE:
                _ret = ( 0 != ( *(uint64*)_res.buf.c_str() ) );
                break;
            default:
                break;
            }
        }
    }

    return _ret;
}

sint8 sql_result::get_int8( size_t idx_ )
{
    sint8 _ret = 0;
    uint32 curr_idx = row_offset()+idx_;

    //if ( m_cur_ != m_res_.end() && idx_ < m_cur_->size() )
    {
        result_t& _res = m_Result[curr_idx];

        if ( !_res.is_null )
        {
            switch ( _res.type )
            {
            case MYSQL_TYPE_TINY:
                _ret = (sint8)( *(uint8*)_res.buf.c_str() );
                break;
            case MYSQL_TYPE_SHORT:
                _ret = (sint8)( *(uint16*)_res.buf.c_str() );
                break;
            case MYSQL_TYPE_INT24:
                _ret = (sint8)( *(uint32*)_res.buf.c_str() );
                break;
            case MYSQL_TYPE_LONG:
                _ret = (sint8)( *(uint32*)_res.buf.c_str() );
                break;
            case MYSQL_TYPE_LONGLONG:
                _ret = (sint8)( *(uint64*)_res.buf.c_str() );
                break;
            case MYSQL_TYPE_FLOAT:
                _ret = (sint8)( *(float*)_res.buf.c_str() );
                break;
            case MYSQL_TYPE_DOUBLE:
                _ret = (sint8)( *(double*)_res.buf.c_str() );
                break;
            default:
                break;
            }
        }
    }

    return _ret;
}

uint8 sql_result::get_uint8( size_t idx_ )
{
    uint8 _ret = 0;
    uint32 curr_idx = row_offset()+idx_;

    //if ( m_cur_ != m_res_.end() && idx_ < m_cur_->size() )
    {
        result_t& _res = m_Result[curr_idx];

        if ( !_res.is_null )
        {
            switch ( _res.type )
            {
            case MYSQL_TYPE_TINY:
                _ret = (uint8)( *(uint8*)_res.buf.c_str() );
                break;
            case MYSQL_TYPE_SHORT:
                _ret = (uint8)( *(uint16*)_res.buf.c_str() );
                break;
            case MYSQL_TYPE_INT24:
                _ret = (uint8)( *(uint32*)_res.buf.c_str() );
                break;
            case MYSQL_TYPE_LONG:
                _ret = (uint8)( *(uint32*)_res.buf.c_str() );
                break;
            case MYSQL_TYPE_LONGLONG:
                _ret = (uint8)( *(uint64*)_res.buf.c_str() );
                break;
            case MYSQL_TYPE_FLOAT:
                _ret = (uint8)( *(float*)_res.buf.c_str() );
                break;
            case MYSQL_TYPE_DOUBLE:
                _ret = (uint8)( *(double*)_res.buf.c_str() );
                break;
            default:
                break;
            }
        }
    }

    return _ret;
}

sint16 sql_result::get_int16( size_t idx_ )
{
    sint16 _ret = 0;
    uint32 curr_idx = row_offset()+idx_;

    //if ( m_cur_ != m_res_.end() && idx_ < m_cur_->size() )
    {
        result_t& _res = m_Result[curr_idx];

        if ( !_res.is_null )
        {
            switch ( _res.type )
            {
            case MYSQL_TYPE_TINY:
                _ret = (sint16)( *(uint8*)_res.buf.c_str() );
                break;
            case MYSQL_TYPE_SHORT:
                _ret = (sint16)( *(uint16*)_res.buf.c_str() );
                break;
            case MYSQL_TYPE_INT24:
                _ret = (sint16)( *(uint32*)_res.buf.c_str() );
                break;
            case MYSQL_TYPE_LONG:
                _ret = (sint16)( *(uint32*)_res.buf.c_str() );
                break;
            case MYSQL_TYPE_LONGLONG:
                _ret = (sint16)( *(uint64*)_res.buf.c_str() );
                break;
            case MYSQL_TYPE_FLOAT:
                _ret = (sint16)( *(float*)_res.buf.c_str() );
                break;
            case MYSQL_TYPE_DOUBLE:
                _ret = (sint16)( *(double*)_res.buf.c_str() );
                break;
            default:
                break;
            }
        }
    }

    return _ret;
}

uint16 sql_result::get_uint16( size_t idx_ )
{
    uint16 _ret = 0;
    uint32 curr_idx = row_offset()+idx_;

    //if ( m_cur_ != m_res_.end() && idx_ < m_cur_->size() )
    {
        result_t& _res = m_Result[curr_idx];

        if ( !_res.is_null )
        {
            switch ( _res.type )
            {
            case MYSQL_TYPE_TINY:
                _ret = (uint16)( *(uint8*)_res.buf.c_str() );
                break;
            case MYSQL_TYPE_SHORT:
                _ret = (uint16)( *(uint16*)_res.buf.c_str() );
                break;
            case MYSQL_TYPE_INT24:
                _ret = (uint16)( *(uint32*)_res.buf.c_str() );
                break;
            case MYSQL_TYPE_LONG:
                _ret = (uint16)( *(uint32*)_res.buf.c_str() );
                break;
            case MYSQL_TYPE_LONGLONG:
                _ret = (uint16)( *(uint64*)_res.buf.c_str() );
                break;
            case MYSQL_TYPE_FLOAT:
                _ret = (uint16)( *(float*)_res.buf.c_str() );
                break;
            case MYSQL_TYPE_DOUBLE:
                _ret = (uint16)( *(double*)_res.buf.c_str() );
                break;
            default:
                break;
            }
        }
    }

    return _ret;
}

sint32 sql_result::get_int32( size_t idx_ )
{
    sint32 _ret = 0;
    uint32 curr_idx = row_offset()+idx_;

    //if ( m_cur_ != m_res_.end() && idx_ < m_cur_->size() )
    {
        result_t& _res = m_Result[curr_idx];

        if ( !_res.is_null )
        {
            switch ( _res.type )
            {
            case MYSQL_TYPE_TINY:
                _ret = (sint32)( *(uint8*)_res.buf.c_str() );
                break;
            case MYSQL_TYPE_SHORT:
                _ret = (sint32)( *(uint16*)_res.buf.c_str() );
                break;
            case MYSQL_TYPE_INT24:
                _ret = (sint32)( *(uint32*)_res.buf.c_str() );
                break;
            case MYSQL_TYPE_LONG:
                _ret = (sint32)( *(uint32*)_res.buf.c_str() );
                break;
            case MYSQL_TYPE_LONGLONG:
                _ret = (sint32)( *(uint64*)_res.buf.c_str() );
                break;
            case MYSQL_TYPE_FLOAT:
                _ret = (sint32)( *(float*)_res.buf.c_str() );
                break;
            case MYSQL_TYPE_DOUBLE:
                _ret = (sint32)( *(double*)_res.buf.c_str() );
                break;
            default:
                break;
            }
        }
    }

    return _ret;
}

uint32 sql_result::get_uint32( size_t idx_ )
{
    uint32 _ret = 0;
    uint32 curr_idx = row_offset()+idx_;

    //if ( m_cur_ != m_res_.end() && idx_ < m_cur_->size() )
    {
        result_t& _res = m_Result[curr_idx];

        if ( !_res.is_null )
        {
            switch ( _res.type )
            {
            case MYSQL_TYPE_TINY:
                _ret = (uint32)( *(uint8*)_res.buf.c_str() );
                break;
            case MYSQL_TYPE_SHORT:
                _ret = (uint32)( *(uint16*)_res.buf.c_str() );
                break;
            case MYSQL_TYPE_INT24:
                _ret = (uint32)( *(uint32*)_res.buf.c_str() );
                break;
            case MYSQL_TYPE_LONG:
                _ret = (uint32)( *(uint32*)_res.buf.c_str() );
                break;
            case MYSQL_TYPE_LONGLONG:
                _ret = (uint32)( *(uint64*)_res.buf.c_str() );
                break;
            case MYSQL_TYPE_FLOAT:
                _ret = (uint32)( *(float*)_res.buf.c_str() );
                break;
            case MYSQL_TYPE_DOUBLE:
                _ret = (uint32)( *(double*)_res.buf.c_str() );
                break;
            default:
                break;
            }
        }
    }

    return _ret;
}

sint64 sql_result::get_int64( size_t idx_ )
{
    sint64 _ret = 0;
    uint32 curr_idx = row_offset()+idx_;

    //if ( m_cur_ != m_res_.end() && idx_ < m_cur_->size() )
    {
        result_t& _res = m_Result[curr_idx];

        if ( !_res.is_null )
        {
            switch ( _res.type )
            {
            case MYSQL_TYPE_TINY:
                _ret = (sint64)( *(uint8*)_res.buf.c_str() );
                break;
            case MYSQL_TYPE_SHORT:
                _ret = (sint64)( *(uint16*)_res.buf.c_str() );
                break;
            case MYSQL_TYPE_INT24:
                _ret = (sint64)( *(uint32*)_res.buf.c_str() );
                break;
            case MYSQL_TYPE_LONG:
                _ret = (sint64)( *(uint32*)_res.buf.c_str() );
                break;
            case MYSQL_TYPE_LONGLONG:
                _ret = (sint64)( *(uint64*)_res.buf.c_str() );
                break;
            case MYSQL_TYPE_FLOAT:
                _ret = (sint64)( *(float*)_res.buf.c_str() );
                break;
            case MYSQL_TYPE_DOUBLE:
                _ret = (sint64)( *(double*)_res.buf.c_str() );
                break;
            default:
                break;
            }
        }
    }

    return _ret;
}

uint64 sql_result::get_uint64( size_t idx_ )
{
    uint64 _ret = 0;
    uint32 curr_idx = row_offset()+idx_;

    //if ( m_cur_ != m_res_.end() && idx_ < m_cur_->size() )
    {
        result_t& _res = m_Result[curr_idx];

        if ( !_res.is_null )
        {
            switch ( _res.type )
            {
            case MYSQL_TYPE_TINY:
                _ret = (uint64)( *(uint8*)_res.buf.c_str() );
                break;
            case MYSQL_TYPE_SHORT:
                _ret = (uint64)( *(uint16*)_res.buf.c_str() );
                break;
            case MYSQL_TYPE_INT24:
                _ret = (uint64)( *(uint32*)_res.buf.c_str() );
                break;
            case MYSQL_TYPE_LONG:
                _ret = (uint64)( *(uint32*)_res.buf.c_str() );
                break;
            case MYSQL_TYPE_LONGLONG:
                _ret = (uint64)( *(uint64*)_res.buf.c_str() );
                break;
            case MYSQL_TYPE_FLOAT:
                _ret = (uint64)( *(float*)_res.buf.c_str() );
                break;
            case MYSQL_TYPE_DOUBLE:
                _ret = (uint64)( *(double*)_res.buf.c_str() );
                break;
            default:
                break;
            }
        }
    }

    return _ret;
}

float sql_result::get_float( size_t idx_ )
{
    float _ret = 0;
    uint32 curr_idx = row_offset()+idx_;

    //if ( m_cur_ != m_res_.end() && idx_ < m_cur_->size() )
    {
        result_t& _res = m_Result[curr_idx];

        if ( !_res.is_null )
        {
            switch ( _res.type )
            {
            case MYSQL_TYPE_TINY:
                _ret = (float)( *(uint8*)_res.buf.c_str() );
                break;
            case MYSQL_TYPE_SHORT:
                _ret = (float)( *(uint16*)_res.buf.c_str() );
                break;
            case MYSQL_TYPE_INT24:
                _ret = (float)( *(uint32*)_res.buf.c_str() );
                break;
            case MYSQL_TYPE_LONG:
                _ret = (float)( *(uint32*)_res.buf.c_str() );
                break;
            case MYSQL_TYPE_LONGLONG:
                _ret = (float)( *(uint64*)_res.buf.c_str() );
                break;
            case MYSQL_TYPE_FLOAT:
                _ret = (float)( *(float*)_res.buf.c_str() );
                break;
            case MYSQL_TYPE_DOUBLE:
                _ret = (float)( *(double*)_res.buf.c_str() );
                break;
            default:
                break;
            }
        }
    }

    return _ret;
}

double sql_result::get_double( size_t idx_ )
{
    double _ret = 0;
    uint32 curr_idx = row_offset()+idx_;

    //if ( m_cur_ != m_res_.end() && idx_ < m_cur_->size() )
    {
        result_t& _res = m_Result[curr_idx];

        if ( !_res.is_null )
        {
            switch ( _res.type )
            {
            case MYSQL_TYPE_TINY:
                _ret = (double)( *(uint8*)_res.buf.c_str() );
                break;
            case MYSQL_TYPE_SHORT:
                _ret = (double)( *(uint16*)_res.buf.c_str() );
                break;
            case MYSQL_TYPE_INT24:
                _ret = (double)( *(uint32*)_res.buf.c_str() );
                break;
            case MYSQL_TYPE_LONG:
                _ret = (double)( *(uint32*)_res.buf.c_str() );
                break;
            case MYSQL_TYPE_LONGLONG:
                _ret = (double)( *(uint64*)_res.buf.c_str() );
                break;
            case MYSQL_TYPE_FLOAT:
                _ret = (double)( *(float*)_res.buf.c_str() );
                break;
            case MYSQL_TYPE_DOUBLE:
                _ret = (double)( *(double*)_res.buf.c_str() );
                break;
            default:
                break;
            }
        }
    }

    return _ret;
}

std::pair<size_t, char const*> sql_result::get_string( size_t idx_ )
{
    std::pair<size_t, char const*> _ret( 0, "" );
    uint32 curr_idx = row_offset()+idx_;

    //if ( m_cur_ != m_res_.end() && idx_ < m_cur_->size() )
    {
        result_t& _res = m_Result[curr_idx];

        if ( !_res.is_null )
        {
            _ret.first  = _res.size;
            _ret.second = _res.buf.c_str();
            *(((char*)_ret.second) + _ret.first) = '\0';
        }
    }

    return _ret;
}

std::pair<size_t, void const*> sql_result::get_blob( size_t idx_ )
{
    std::pair<size_t, void const*> _ret( 0, "" );
    uint32 curr_idx = row_offset()+idx_;

    //if ( m_cur_ != m_res_.end() && idx_ < m_cur_->size() )
    {
        result_t& _res = m_Result[curr_idx];

        if ( !_res.is_null )
        {
            _ret.first  = _res.size;
            _ret.second = _res.buf.c_str();
        }
    }

    return _ret;
}

bool sql_result::_init( MYSQL_STMT *stmt_, MYSQL_RES *res_ )
{
    bool _ret = false;
    m_RowCount = 0;
    m_CurrRow = -1;

    if ( NULL != stmt_ && NULL != res_ )
    {
        size_t _row = mysql_stmt_num_rows( stmt_ );

        if ( _row > 0 )
        {
            size_t _count = 0;
            MYSQL_BIND *_bind = NULL;

            _count = mysql_stmt_field_count( stmt_ );
            if ( _count > 0 )
            {
                m_FieldCount = _count;
                _bind = (MYSQL_BIND*)malloc( _count * sizeof(MYSQL_BIND) );
                memset( _bind, 0, _count * sizeof(MYSQL_BIND) );
            }

            if ( NULL != _bind )
            {
                //std::map<size_t, result_t> _result;
                std::vector<result_t> _result;
                _result.resize(_count);

                size_t _idx = 0;
                size_t _buff_max_len = 0;
                MYSQL_FIELD *pField = mysql_fetch_field( res_ );

                while ( NULL != pField )
                {
                    size_t    _sz  = 0;

                    switch ( pField->type )
                    {
                    case MYSQL_TYPE_TINY:
                        _sz = 1;
                        break;
                    case MYSQL_TYPE_SHORT:
                        _sz = 2;
                        break;
                    case MYSQL_TYPE_INT24:
                        _sz = 4;
                        break;
                    case MYSQL_TYPE_LONG:
                        _sz = 4;
                        break;
                    case MYSQL_TYPE_LONGLONG:
                        _sz = 8;
                        break;
                    case MYSQL_TYPE_FLOAT:
                        _sz = 4;
                        break;
                    case MYSQL_TYPE_DOUBLE:
                        _sz = 8;
                        break;
                    case MYSQL_TYPE_STRING:
                    case MYSQL_TYPE_VAR_STRING:
                    case MYSQL_TYPE_BLOB:
                    case MYSQL_TYPE_TINY_BLOB:
                    case MYSQL_TYPE_MEDIUM_BLOB:
                    case MYSQL_TYPE_LONG_BLOB:
                        _sz = pField->max_length;
                        break;
                    default:
                        nlassert( false );
                        break;
                    }

                    result_t& _res          = _result[_idx];
                    _res.buf.assign( '\0', _sz );
                    _res.size              = _sz;
                    _res.type              = pField->type;
                    _res.is_null           = 0;

                    _bind[_idx].buffer_type   = pField->type;
                    _bind[_idx].buffer        = (void*)_res.buf.c_str();
                    _bind[_idx].buffer_length = _res.size;
                    _bind[_idx].length        = &_res.size;
                    _bind[_idx].is_unsigned   = pField->flags & UNSIGNED_FLAG;
                    _bind[_idx].is_null       = &_res.is_null;
                    _buff_max_len            += _res.size;

                    ++ _idx;
                    pField = mysql_fetch_field( res_ );
                }

                if (  _buff_max_len > 0 && 0 == mysql_stmt_bind_result( stmt_, _bind ) )
                {
                    m_Result.resize( _row*_count );
                    m_RowCount = _row;
                    uint32 ret_idx = 0;

                    //NLMISC::TTime  save_time = 0;
                    //uint32         while_count = 0;

                    while ( _row > 0 )
                    {
                        int _val = mysql_stmt_fetch( stmt_ );

                        -- _row;
                        if ( 0 == _val || MYSQL_DATA_TRUNCATED == _val )
                        {
                            uint32 row_offset = ret_idx*_count;

                            
                            for ( size_t _idx = 0; _idx < _count; ++ _idx )
                            {
                                result_t& src   = _result[_idx];
                                result_t& dest  = m_Result[row_offset+_idx];

                                //NLMISC::TTime sss = NLMISC::CTime::getLocalTime();
                                dest.buf.assign( src.buf.c_str(), src.size );
                                //save_time += NLMISC::CTime::getLocalTime() -  sss;
                                //++while_count;

                                //dest.buf     = src.buf;
                                dest.size    = src.size;
                                dest.type    = src.type;
                                dest.is_null = src.is_null;
                            }
                            

                            ++ret_idx;
                        }
                        else
                        {
                            m_RowCount = m_RowCount-_row;
                            m_Result.resize( m_RowCount*m_FieldCount );
                            _row = 0;
                        }
                    }

                    //nlinfo("save itme : %lld  while_count:%d",save_time,while_count);

                    _ret   = true;
                }

                free( _bind );
                _result.clear();
            }
        }
    }

    return _ret;
}

bool sql_result::_retval( MYSQL_STMT *stmt_, MYSQL_RES *res_ )
{
    bool _ret = false;

    //if ( NULL != stmt_ && NULL != res_ )
    //{
    //    size_t _row = 0;

    //    _row = mysql_stmt_num_rows( stmt_ );
    //    if ( 1 == _row )    // 输出参数返回的结果集只有一行
    //    {
    //        size_t _count = 0;
    //        MYSQL_BIND *_bind = NULL;

    //        _count = mysql_stmt_field_count( stmt_ );
    //        if ( _count > 0 )
    //        {
    //            _bind = (MYSQL_BIND*)malloc( _count * sizeof(MYSQL_BIND) );
    //            memset( _bind, 0, _count * sizeof(MYSQL_BIND) );
    //        }

    //        if ( NULL != _bind )
    //        {
    //            std::map<size_t, result_t> _result;
    //            size_t _idx = 0;
    //            size_t _buff_max_len = 0;
    //            MYSQL_FIELD *_field  = NULL;

    //            _field = mysql_fetch_field( res_ );
    //            while ( NULL != _field )
    //            {
    //                size_t    _sz  = 0;

    //                switch ( _field->type )
    //                {
    //                case MYSQL_TYPE_TINY:
    //                    _sz = 1;
    //                    break;
    //                case MYSQL_TYPE_SHORT:
    //                    _sz = 2;
    //                    break;
    //                case MYSQL_TYPE_INT24:
    //                    _sz = 4;
    //                    break;
    //                case MYSQL_TYPE_LONG:
    //                    _sz = 4;
    //                    break;
    //                case MYSQL_TYPE_LONGLONG:
    //                    _sz = 8;
    //                    break;
    //                case MYSQL_TYPE_FLOAT:
    //                    _sz = 4;
    //                    break;
    //                case MYSQL_TYPE_DOUBLE:
    //                    _sz = 8;
    //                    break;
    //                case MYSQL_TYPE_STRING:
    //                case MYSQL_TYPE_VAR_STRING:
    //                case MYSQL_TYPE_BLOB:
    //                case MYSQL_TYPE_TINY_BLOB:
    //                case MYSQL_TYPE_MEDIUM_BLOB:
    //                case MYSQL_TYPE_LONG_BLOB:
    //                    _sz = _field->max_length;
    //                    break;
    //                default:
    //                    break;
    //                }

    //                result_t& _res          = _result[_idx];
    //                _res.buf.assign( '\0', _sz );
    //                _res.size              = _sz;
    //                _res.type              = _field->type;
    //                _res.is_null           = 0;

    //                _bind[_idx].buffer_type   = _field->type;
    //                _bind[_idx].buffer        = (void*)_res.buf.c_str();
    //                _bind[_idx].buffer_length = _res.size;
    //                _bind[_idx].length        = &_res.size;
    //                _bind[_idx].is_unsigned   = _field->flags & UNSIGNED_FLAG;
    //                _bind[_idx].is_null       = &_res.is_null;
    //                _buff_max_len            += _res.size;

    //                ++ _idx;
    //                _field = mysql_fetch_field( res_ );
    //            }

    //            if ( _buff_max_len > 0 && 0 == mysql_stmt_bind_result( stmt_, _bind ) )
    //            {
    //                int _val = mysql_stmt_fetch( stmt_ );
    //                if ( 0 == _val || MYSQL_DATA_TRUNCATED == _val )
    //                {
    //                    result_t *_src = NULL;
    //                    result_t *_dest = NULL;

    //                    for ( size_t _idx = 0; _idx < _count; ++ _idx )
    //                    {
    //                        _src           = &_result[_idx];
    //                        _dest          = &m_ret_[_idx];
    //                        _dest->buf     = _src->buf;
    //                        _dest->size    = _src->size;
    //                        _dest->type    = _src->type;
    //                        _dest->is_null = _src->is_null;
    //                    }
    //                }
    //                _ret   = true;
    //            }

    //            free( _bind );
    //            _result.clear();
    //        }
    //    }
    //}

    return _ret;
}

} // namespace ex