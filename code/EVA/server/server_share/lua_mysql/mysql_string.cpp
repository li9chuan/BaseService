#include "mysql_stmt.h"
#include <string>

CLuaMysqlString::CLuaMysqlString( void )
    : m_str_( 0 ),
      m_off_( 0 ),
      m_sz_( 0 )
{
}

CLuaMysqlString::CLuaMysqlString( char const *str_ )
    : m_str_( 0 ),
      m_off_( 0 ),
      m_sz_( 0 )
{
    assign( str_ );
}

CLuaMysqlString::CLuaMysqlString( char const *str_, size_t sz_ )
    : m_str_( 0 ),
      m_off_( 0 ),
      m_sz_( 0 )
{
    assign( str_, sz_ );
}

CLuaMysqlString::CLuaMysqlString( CLuaMysqlString const &str_ )
    : m_str_( 0 ),
      m_off_( 0 ),
      m_sz_( 0 )
{
    assign( str_ );
}

CLuaMysqlString::CLuaMysqlString( char ch_, size_t sz_ )
    : m_str_( 0 ),
      m_off_( 0 ),
      m_sz_( 0 )
{
    assign( ch_, sz_ );
}

CLuaMysqlString::CLuaMysqlString( size_t sz_ )
    : m_str_( 0 ),
      m_off_( 0 ),
      m_sz_( 0 )
{
    assign( '\0', sz_ > 0 ? ( sz_ - 1 ) : sz_ );
}

CLuaMysqlString::~CLuaMysqlString( void )
{
    if ( 0 != m_str_ )      
    {
        delete[] m_str_;
        m_str_ = 0;
        m_off_ = 0;
        m_sz_  = 0;
    }
}

// to c-string.
CLuaMysqlString::operator char const* ( void ) const
{
    return m_str_;
}

// operator [].
char& CLuaMysqlString::operator [] ( int idx )
{
    return m_str_[idx];
}

// operator =.
CLuaMysqlString& CLuaMysqlString::operator = ( CLuaMysqlString const &str_ )
{
    return assign( str_ );
}

// operator =.
CLuaMysqlString& CLuaMysqlString::operator = ( char const *str_ )
{
    return assign( str_ );
}

// operator =.
CLuaMysqlString& CLuaMysqlString::operator = ( char ch_ )
{
    return assign( ch_, 1 );
}

// operator +=.
CLuaMysqlString& CLuaMysqlString::operator += ( char ch_ )
{
    return append( ch_, 1 );
}

// operator ==.
bool CLuaMysqlString::operator == ( CLuaMysqlString const &str_ ) const
{
    return ( 0 == compare( str_ ) );
}

// operator !=.
bool CLuaMysqlString::operator == ( char const *str_ ) const
{
    return ( 0 == compare( str_ ) );
}

// operator !=.
bool CLuaMysqlString::operator != ( CLuaMysqlString const &str_ ) const
{
    return ( 0 != compare( str_ ) );
}

// operator !=.
bool CLuaMysqlString::operator != ( char const *str_ ) const
{
    return ( 0 != compare( str_ ) );
}

// operator <.
bool CLuaMysqlString::operator < ( CLuaMysqlString const &str_ ) const
{
    return ( compare( str_ ) < 0 );
}

// operator <.
bool CLuaMysqlString::operator < ( char const *str_ ) const
{
    return ( compare( str_ ) < 0 );
}

// operator <=.
bool CLuaMysqlString::operator <= ( CLuaMysqlString const &str_ ) const
{
    return ( compare( str_ ) <= 0 );
}

// operator <=.
bool CLuaMysqlString::operator <= ( char const *str_ ) const
{
    return ( compare( str_ ) <= 0 );
}

// operator >.
bool CLuaMysqlString::operator > ( CLuaMysqlString const &str_ ) const
{
    return ( compare( str_ ) > 0 );
}

// operator >.
bool CLuaMysqlString::operator > ( char const *str_ ) const
{
    return ( compare( str_ ) > 0 );
}

// operator >=.
bool CLuaMysqlString::operator >= ( CLuaMysqlString const &str_ ) const
{
    return ( compare( str_ ) >= 0 );
}

// operator >=.
bool CLuaMysqlString::operator >= ( char const *str_ ) const
{
    return ( compare( str_ ) >= 0 );
}

CLuaMysqlString& CLuaMysqlString::assign( CLuaMysqlString const &str_ )
{
    if ( this != &str_ )
    {
        if ( 0 != str_.m_str_ )
        {
            m_off_ = str_.m_off_;
            if ( m_off_ >= m_sz_ )
            {
                if ( 0 != m_str_ )
                {
                    delete[] m_str_;
                    m_str_ = 0;
                }
                m_sz_ = m_off_ + 1;
                m_str_ = new( std::nothrow ) char[ m_sz_ ];
            }

            //memcpy( m_str_, str_.m_str_, m_off_ );
            //m_str_[m_off_] = 0;
            _strncpy( m_str_, str_.m_str_, m_off_ );
        }
        else if ( 0 != m_str_ )
        {
            *m_str_ = '\0';
            m_off_  = 0;
        }
    }

    return ( *this );
}

CLuaMysqlString& CLuaMysqlString::assign( char const *str_ )
{
    if ( 0 != str_ )
    {
        m_off_ = ::strlen( str_ );
        if ( m_off_ >= m_sz_ )
        {
            if ( 0 != m_str_ )
            {
                delete[] m_str_;
                m_str_ = 0;
            }
            m_sz_ = m_off_ + 1;
            m_str_ = new( std::nothrow ) char[ m_sz_ ];
        }

        _strncpy( m_str_, str_, m_off_ );
    }
    else if ( 0 != m_str_ )
    {
        *m_str_ = '\0';
        m_off_  = 0;
    }

    return ( *this );
}

CLuaMysqlString& CLuaMysqlString::assign( char const *str_, size_t sz_ )
{
    if ( 0 != str_ && sz_ > 0 )
    {
        m_off_ = sz_;
        if ( m_off_ >= m_sz_ )
        {
            if ( 0 != m_str_ )
            {
                delete[] m_str_;
                m_str_ = 0;
            }
            m_sz_ = sz_ + 1;
            m_str_ = new( std::nothrow ) char[ m_sz_ ];
        }

        memcpy( m_str_, str_, sz_ );
        m_str_[sz_] = 0;
        //_strncpy( m_str_, str_, m_off_ );
    }
    else if ( 0 != m_str_ )
    {
        *m_str_ = '\0';
        m_off_  = 0;
    }

    return ( *this );
}

CLuaMysqlString& CLuaMysqlString::assign( char ch_, size_t sz_ )
{
    m_off_ = sz_;
    if ( m_off_ >= m_sz_ )
    {
        if ( 0 != m_str_ )
        {
            delete[] m_str_;
            m_str_ = 0;
        }
        m_sz_ = sz_ + 1;
        m_str_ = new( std::nothrow ) char[ m_sz_ ];
    }

    char *_str = m_str_;
    while( sz_-- )
        *_str++ = ch_;
    *_str = '\0';

    return ( *this );
}

CLuaMysqlString& CLuaMysqlString::append( CLuaMysqlString const &str_ )
{
    size_t _off, _sz;
    char  *_str;

    if ( this != &str_ )
    {
        if ( NULL != str_.m_str_ )
        {
            _off = m_off_ + str_.m_off_;
            if ( _off >= m_sz_ )
            {
                _sz = _off + 1;
                _str = new( std::nothrow ) char[ _sz ];

                // copy
                if ( NULL != m_str_ )
                {
                    _strncpy( _str, m_str_, m_off_ );
                    delete[] m_str_;
                }

                m_str_ = _str;
                m_sz_  = _sz;
            }

            _strncpy( m_str_ + m_off_, str_.m_str_, str_.m_off_ );
            m_off_ = _off;
        }
    }
    else if ( NULL != m_str_ )
    {
        _off = m_off_ << 1;
        if ( _off >= m_sz_ )
        {
            _sz  = _off + 1;
            _str = new( std::nothrow ) char[ _sz ];

            // copy
            _strncpy( _str, m_str_, m_off_ );
            _strncpy( _str + m_off_, m_str_, m_off_ );

            delete[] m_str_;
            m_str_ = _str;
            m_sz_  = _sz;
            m_off_ = _off;
        }
        else
        {
            _strncpy( m_str_ + m_off_, m_str_, m_off_ );
            m_off_ = _off;
        }
    }

    return ( *this );
}

CLuaMysqlString& CLuaMysqlString::append( char const *str_ )
{
    size_t _off, _sz;
    char  *_str;

    if ( NULL != str_ )
    {
        _off = m_off_ + ::strlen( str_ );
        if ( _off >= m_sz_ )
        {
            _sz  = _off + 1;
            _str = new( std::nothrow ) char[ _sz ];

            // copy
            if ( NULL != m_str_ )
            {
                _strncpy( _str, m_str_, m_off_ );
                delete[] m_str_;
            }

            m_str_ = _str;
            m_sz_  = _sz;
        }

        _strncpy( m_str_ + m_off_, str_, _off - m_off_ );
        m_off_ = _off;
    }

    return ( *this );
}

CLuaMysqlString& CLuaMysqlString::append( char const *str_, size_t sz_ )
{
    size_t _off, _sz;
    char  *_str;

    if ( NULL != str_ && sz_ > 0 )
    {
        _off = m_off_ + sz_;
        if ( _off >= m_sz_ )
        {
            _sz  = _off + 1;
            _str = new( std::nothrow ) char[ _sz ];
            // copy
            if ( NULL != m_str_ )
            {
                _strncpy( _str, m_str_, m_off_ );
                delete[] m_str_;
            }

            m_str_ = _str;
            m_sz_  = _sz;
        }

        _strncpy( m_str_ + m_off_, str_, sz_ );
        m_off_ = _off;
    }

    return ( *this );
}

CLuaMysqlString& CLuaMysqlString::append( char ch_, size_t sz_ )
{
    size_t _off, _sz;
    char  *_str;

    _off = m_off_ + sz_;
    if ( _off >= m_sz_ )
    {
        _sz  = _off + 1;
        _str = new( std::nothrow ) char[ _sz ];

        // copy
        if ( NULL != m_str_ )
        {
            _strncpy( _str, m_str_, m_off_ );
            delete[] m_str_;
            m_str_ = 0;
        }

        m_str_ = _str;
        m_sz_  = _sz;
    }

    _str = m_str_ + m_off_;
    while ( sz_-- )
        *_str++ = ch_;
    *_str = '\0';
    m_off_ = _off;

    return ( *this );
}

int CLuaMysqlString::compare( CLuaMysqlString const &str_ ) const
{
    if ( this == &str_ )
    {
        return 0;
    }
    else if ( 0 != m_str_ && 0 != str_.m_str_ )
    {
        int     _ret = 0;
        size_t  _sz0 = m_off_;
        size_t  _sz1 = str_.m_off_;
        const char *_str0 = m_str_;
        const char *_str1 = str_.m_str_;

        while ( !( _ret = (int)( *_str0 - *_str1 ) )
                && _sz0 && _sz1 )
        {
            ++ _str0;
            ++ _str1;
            -- _sz0;
            -- _sz1;
        }

        if ( _ret < 0 )
            _ret = -1;
        else if ( _ret > 0 )
            _ret = 1;
        else if ( _sz0 )
            _ret = 1;
        else if ( _sz1 )
            _ret = -1;

        return _ret;
    }
    else if ( 0 != m_str_ )
    {
        return 1;
    }
    else if ( 0 != str_.m_str_ )
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

int CLuaMysqlString::compare( const char *str_ ) const
{
    if ( 0 != m_str_ && 0 != str_ )
    {
        int    _ret = 0;
        size_t _sz0 = m_off_;
        const char *_str0 = m_str_;
        const char *_str1 = str_;

        while ( !( _ret = (int)( *_str0 - *_str1 ) )
                && _sz0 && *_str1 )
        {
            ++ _str0;
            ++ _str1;
            -- _sz0;
        }

        if ( _ret < 0 )
            _ret = -1;
        else if ( _ret > 0 )
            _ret = 1;
        else if ( _sz0 )
            _ret = 1;
        else if ( *_str1 )
            _ret = -1;

        return _ret;
    }
    else if ( 0 != m_str_ )
    {
        return 1;
    }
    else if ( 0 != str_ )
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

int CLuaMysqlString::compare( const char *str_, size_t sz_ ) const
{
    if ( 0 != m_str_ && 0 != str_ && sz_ > 0 )
    {
        int     _ret = 0;
        size_t  _sz0 = m_off_;
        size_t  _sz1 = sz_;
        const char *_str0 = m_str_;
        const char *_str1 = str_;

        while ( !( _ret = (int)( *_str0 - *_str1 ) )
                && _sz0 && _sz1 )
        {
            ++ _str0;
            ++ _str1;
            -- _sz0;
            -- _sz1;
        }

        if ( _ret < 0 )
            _ret = -1;
        else if ( _ret > 0 )
            _ret = 1;
        else if ( _sz0 )
            _ret = 1;
        else if ( _sz1 )
            _ret = -1;

        return _ret;
    }
    else if ( 0 != m_str_ && sz_ > 0 )
    {
        return 1;
    }
    else if ( 0 != str_ && sz_ > 0 )
    {
        return -1;
    }
    else
    {
        return 0;
    }
}

const char* CLuaMysqlString::c_str( void ) const
{
    return m_str_;
}

size_t CLuaMysqlString::size( void ) const
{
    return m_off_;
}

bool CLuaMysqlString::empty( void ) const
{
    return ( 0 == m_off_ );
}

void CLuaMysqlString::clear( void )
{
    if ( 0 != m_str_ )
    {
        m_str_[0] = '\0';
        m_off_    = 0;
    }
}

char* CLuaMysqlString::_strncpy( char *dst_, char const *src_, size_t sz_ )
{
    if ( 0 != dst_ && 0 != src_ )
    {
        char *_str = dst_;

        while ( sz_-- )
            *_str++ = *src_++;
        *_str = '\0';

        return dst_;
    }

    return 0;
}


