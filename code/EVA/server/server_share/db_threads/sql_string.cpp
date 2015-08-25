#include "sql_string.h"
#include <string>

namespace nl
{

sql_string::sql_string( void )
    : m_str_( 0 ),
      m_off_( 0 ),
      m_sz_( 0 )
{
}

sql_string::sql_string( char const *str_ )
    : m_str_( 0 ),
      m_off_( 0 ),
      m_sz_( 0 )
{
    assign( str_ );
}

sql_string::sql_string( char const *str_, size_t sz_ )
    : m_str_( 0 ),
      m_off_( 0 ),
      m_sz_( 0 )
{
    assign( str_, sz_ );
}

sql_string::sql_string( sql_string const &str_ )
    : m_str_( 0 ),
      m_off_( 0 ),
      m_sz_( 0 )
{
    assign( str_ );
}

sql_string::sql_string( char ch_, size_t sz_ )
    : m_str_( 0 ),
      m_off_( 0 ),
      m_sz_( 0 )
{
    assign( ch_, sz_ );
}

sql_string::sql_string( size_t sz_ )
    : m_str_( 0 ),
      m_off_( 0 ),
      m_sz_( 0 )
{
    assign( '\0', sz_ > 0 ? ( sz_ - 1 ) : sz_ );
}

sql_string::~sql_string( void )
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
sql_string::operator char const* ( void ) const
{
    return m_str_;
}

// operator [].
char& sql_string::operator [] ( int idx )
{
    return m_str_[idx];
}

// operator =.
sql_string& sql_string::operator = ( sql_string const &str_ )
{
    return assign( str_ );
}

// operator =.
sql_string& sql_string::operator = ( char const *str_ )
{
    return assign( str_ );
}

// operator =.
sql_string& sql_string::operator = ( char ch_ )
{
    return assign( ch_, 1 );
}

// operator +=.
sql_string& sql_string::operator += ( char ch_ )
{
    return append( ch_, 1 );
}

// operator ==.
bool sql_string::operator == ( sql_string const &str_ ) const
{
    return ( 0 == compare( str_ ) );
}

// operator !=.
bool sql_string::operator == ( char const *str_ ) const
{
    return ( 0 == compare( str_ ) );
}

// operator !=.
bool sql_string::operator != ( sql_string const &str_ ) const
{
    return ( 0 != compare( str_ ) );
}

// operator !=.
bool sql_string::operator != ( char const *str_ ) const
{
    return ( 0 != compare( str_ ) );
}

// operator <.
bool sql_string::operator < ( sql_string const &str_ ) const
{
    return ( compare( str_ ) < 0 );
}

// operator <.
bool sql_string::operator < ( char const *str_ ) const
{
    return ( compare( str_ ) < 0 );
}

// operator <=.
bool sql_string::operator <= ( sql_string const &str_ ) const
{
    return ( compare( str_ ) <= 0 );
}

// operator <=.
bool sql_string::operator <= ( char const *str_ ) const
{
    return ( compare( str_ ) <= 0 );
}

// operator >.
bool sql_string::operator > ( sql_string const &str_ ) const
{
    return ( compare( str_ ) > 0 );
}

// operator >.
bool sql_string::operator > ( char const *str_ ) const
{
    return ( compare( str_ ) > 0 );
}

// operator >=.
bool sql_string::operator >= ( sql_string const &str_ ) const
{
    return ( compare( str_ ) >= 0 );
}

// operator >=.
bool sql_string::operator >= ( char const *str_ ) const
{
    return ( compare( str_ ) >= 0 );
}

sql_string& sql_string::assign( sql_string const &str_ )
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

sql_string& sql_string::assign( char const *str_ )
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

sql_string& sql_string::assign( char const *str_, size_t sz_ )
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

sql_string& sql_string::assign( char ch_, size_t sz_ )
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

sql_string& sql_string::append( sql_string const &str_ )
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

sql_string& sql_string::append( char const *str_ )
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

sql_string& sql_string::append( char const *str_, size_t sz_ )
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

sql_string& sql_string::append( char ch_, size_t sz_ )
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

int sql_string::compare( sql_string const &str_ ) const
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

int sql_string::compare( const char *str_ ) const
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

int sql_string::compare( const char *str_, size_t sz_ ) const
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

const char* sql_string::c_str( void ) const
{
    return m_str_;
}

size_t sql_string::size( void ) const
{
    return m_off_;
}

bool sql_string::empty( void ) const
{
    return ( 0 == m_off_ );
}

void sql_string::clear( void )
{
    if ( 0 != m_str_ )
    {
        m_str_[0] = '\0';
        m_off_    = 0;
    }
}

char* sql_string::_strncpy( char *dst_, char const *src_, size_t sz_ )
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

} // namespace ex