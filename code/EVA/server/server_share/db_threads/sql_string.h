#ifndef _SQL_STRING_H_
#define _SQL_STRING_H_

namespace nl
{

class sql_string
{
public:
    sql_string( void );
    sql_string( char const *str_ );
    sql_string( char const *str_, size_t sz_ );
    sql_string( sql_string const &str_ );
    sql_string( char ch_, size_t sz_ );
    sql_string( size_t sz_ );
    ~sql_string( void );

    // to c-string.
    operator char const* ( void ) const;

    // operator [].
    char& operator [] ( int idx );

    // operator =.
    sql_string& operator = ( sql_string const &str_ );

    // operator =.
    sql_string& operator = ( char const *str_ );

    // operator =.
    sql_string& operator = ( char ch_ );

    // operator +=.
    sql_string& operator += ( sql_string const &str_ );

    // operator +=.
    sql_string& operator += ( char const *str_ );

    // operator +=.
    sql_string& operator += ( char ch_ );

    // operator ==.
    bool operator == ( sql_string const &str_ ) const;

    // operator ==.
    bool operator == ( char const *str_ ) const;

    // operator !=.
    bool operator != ( sql_string const &str_ ) const;

    // operator !=.
    bool operator != ( char const *str_ ) const;

    // operator <.
    bool operator < ( sql_string const &str_ ) const;

    // operator <.
    bool operator < ( char const *str_ ) const;

    // operator <=.
    bool operator <= ( sql_string const &str_ ) const;

    // operator <=.
    bool operator <= ( char const *str_ ) const;

    // operator >.
    bool operator > ( sql_string const &str_ ) const;

    // operator >.
    bool operator > ( char const *str_ ) const;

    // operator >=.
    bool operator >= ( sql_string const &str_ ) const;

    // operator >=.
    bool operator >= ( char const *str_ ) const;

    // assign.
    sql_string& assign( sql_string const &str_ );

    // assign.
    sql_string& assign( char const *str_ );

    // assign.
    sql_string& assign( char const *str_, size_t sz_ );

    // assign.
    sql_string& assign( char ch_, size_t sz_ );

    // append.
    sql_string& append( sql_string const &str_ );

    // append.
    sql_string& append( char const *str_ );

    // append.
    sql_string& append( char const *str_, size_t sz_ );

    // assign.
    sql_string& append( char ch_, size_t sz_ );

    // compare.
    int compare( sql_string const &str_ ) const;

    // compare.
    int compare( const char *str_ ) const;

    // compare.
    int compare( const char *str_, size_t sz_ ) const;

    // c-string.
    const char* c_str( void ) const;

    // size.
    size_t size( void ) const;

    // empty.
    bool empty( void ) const;

    // clear.
    void clear( void );

private:
    // strncpy.
    char* _strncpy( char *dst_, char const *src_, size_t sz_ );

private:
    char  *m_str_;
    size_t m_off_;
    size_t m_sz_;
};

} // namespace ex

#endif // _SQL_STRING_H_