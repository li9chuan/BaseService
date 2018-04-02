#include "mysql_result.h"
#include "mysql_stmt.h"
#include <cstdio>


CLuaMysqlResult::CLuaMysqlResult( void ):m_RowCount(0),m_CurrRow(-1),m_FieldCount(0),m_Idx(0)
{
    //m_cur_ = m_res_.end();
}

CLuaMysqlResult::~CLuaMysqlResult( void )
{
    //m_res_.clear();
    //m_cur_ = m_res_.end();
    //m_row_ = 0;
}

// close
void CLuaMysqlResult::release( void )
{
    delete this;
}

size_t CLuaMysqlResult::count( void ) const
{
    return m_RowCount;
}

bool CLuaMysqlResult::next( void )
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

my_bool CLuaMysqlResult::get_bool()
{
    bool _ret = false;
    uint32 curr_idx = row_offset()+m_Idx;

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

            ++m_Idx;
        }
    }

    return _ret;
}

sint8 CLuaMysqlResult::get_int8()
{
    sint8 _ret = 0;
    uint32 curr_idx = row_offset()+m_Idx;

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

            ++m_Idx;
        }
    }

    return _ret;
}

uint8 CLuaMysqlResult::get_uint8()
{
    uint8 _ret = 0;
    uint32 curr_idx = row_offset()+m_Idx;

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

            ++m_Idx;
        }
    }

    return _ret;
}

sint16 CLuaMysqlResult::get_int16()
{
    sint16 _ret = 0;
    uint32 curr_idx = row_offset()+m_Idx;

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

            ++m_Idx;
        }
    }

    return _ret;
}

uint16 CLuaMysqlResult::get_uint16()
{
    uint16 _ret = 0;
    uint32 curr_idx = row_offset()+m_Idx;

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

            ++m_Idx;
        }
    }

    return _ret;
}

sint32 CLuaMysqlResult::get_int32()
{
    sint32 _ret = 0;
    uint32 curr_idx = row_offset()+m_Idx;

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
            ++m_Idx;

        }
    }

    return _ret;
}

uint32 CLuaMysqlResult::get_uint32()
{
    uint32 _ret = 0;
    uint32 curr_idx = row_offset()+m_Idx;

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

            ++m_Idx;
        }
    }

    return _ret;
}

sint64 CLuaMysqlResult::get_int64()
{
    sint64 _ret = 0;
    uint32 curr_idx = row_offset()+m_Idx;

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

            ++m_Idx;
        }
    }

    return _ret;
}

uint64 CLuaMysqlResult::get_uint64()
{
    uint64 _ret = 0;
    uint32 curr_idx = row_offset()+m_Idx;

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

            ++m_Idx;
        }
    }

    return _ret;
}

float CLuaMysqlResult::get_float()
{
    float _ret = 0;
    uint32 curr_idx = row_offset()+m_Idx;

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

            ++m_Idx;
        }
    }

    return _ret;
}

double CLuaMysqlResult::get_double()
{
    double _ret = 0;
    uint32 curr_idx = row_offset()+m_Idx;

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

            ++m_Idx;
        }
    }

    return _ret;
}

std::pair<size_t, char const*> CLuaMysqlResult::get_string()
{
    std::pair<size_t, char const*> _ret( 0, "" );
    uint32 curr_idx = row_offset()+m_Idx;

    //if ( m_cur_ != m_res_.end() && idx_ < m_cur_->size() )
    {
        result_t& _res = m_Result[curr_idx];

        if ( !_res.is_null )
        {
            _ret.first  = _res.size;
            _ret.second = _res.buf.c_str();
            *(((char*)_ret.second) + _ret.first) = '\0';
        }

        ++m_Idx;
    }

    return _ret;
}

std::pair<size_t, void const*> CLuaMysqlResult::get_blob()
{
    std::pair<size_t, void const*> _ret( 0, "" );
    uint32 curr_idx = row_offset()+m_Idx;

    //if ( m_cur_ != m_res_.end() && m_Idx < m_cur_->size() )
    {
        result_t& _res = m_Result[curr_idx];

        if ( !_res.is_null )
        {
            _ret.first  = _res.size;
            _ret.second = _res.buf.c_str();
        }

        ++m_Idx;
    }

    return _ret;
}

bool CLuaMysqlResult::_init( MYSQL_STMT *stmt_, MYSQL_RES *res_ )
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

bool CLuaMysqlResult::_retval( MYSQL_STMT *stmt_, MYSQL_RES *res_ )
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

namespace bin
{
    BEGIN_SCRIPT_CLASS( MysqlResult, CLuaMysqlResult )

        DEFINE_CLASS_FUNCTION( Next, bool, ())
        {
            r = obj->next();
            return 1;
        }

        DEFINE_CLASS_FUNCTION( Count, lua_Integer, ())
        {
            r = obj->count();
            return 1;
        }

        DEFINE_CLASS_FUNCTION( GetBool, bool, ())
        {
            r = obj->get_bool();
            return 1;
        }

        DEFINE_CLASS_FUNCTION( GetInt8, lua_Integer, ())
        {
            r = obj->get_int8();
            return 1;
        }

        DEFINE_CLASS_FUNCTION( GetUint8, lua_Integer, ())
        {
            r = obj->get_uint8();
            return 1;
        }

        DEFINE_CLASS_FUNCTION( GetInt32, lua_Integer, ())
        {
            r = obj->get_int32();
            return 1;
        }

        DEFINE_CLASS_FUNCTION( GetUint32, lua_Integer, ())
        {
            r = obj->get_uint32();
            return 1;
        }

        DEFINE_CLASS_FUNCTION( GetInt64, lua_Integer, ())
        {
            r = obj->get_int64();
            return 1;
        }

        DEFINE_CLASS_FUNCTION( GetUInt64, lua_Integer, ())
        {
            r = obj->get_uint64();
            return 1;
        }

        DEFINE_CLASS_FUNCTION( GetFloat, lua_Number, ())
        {
            r = obj->get_float();
            return 1;
        }

        DEFINE_CLASS_FUNCTION( GetDouble, lua_Number, ())
        {
            r = obj->get_double();
            return 1;
        }

        DEFINE_CLASS_FUNCTION( GetBlob, CScriptTable, ())
        {
            std::pair<size_t, void const*> pair = obj->get_blob();

            r.Set(1, (const char*)pair.second);
            r.Set(2, (int)pair.first);

            return 1;
        }


        DEFINE_STATIC_FUNCTION(NewInstance, CLuaMysqlResult*, ())
        {
            r = new CLuaMysqlResult();
            r->GetScriptObject().SetDelByScr(true);

            return 1;
        }

    END_SCRIPT_CLASS()

}




