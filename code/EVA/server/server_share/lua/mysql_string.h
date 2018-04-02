#ifndef LUA_MYSQL_STRING_H
#define LUA_MYSQL_STRING_H

class CLuaMysqlString
{
public:
    CLuaMysqlString( void );
    CLuaMysqlString( char const *str_ );
    CLuaMysqlString( char const *str_, size_t sz_ );
    CLuaMysqlString( CLuaMysqlString const &str_ );
    CLuaMysqlString( char ch_, size_t sz_ );
    CLuaMysqlString( size_t sz_ );
    ~CLuaMysqlString( void );

    // to c-string.
    operator char const* ( void ) const;

    // operator [].
    char& operator [] ( int idx );

    // operator =.
    CLuaMysqlString& operator = ( CLuaMysqlString const &str_ );

    // operator =.
    CLuaMysqlString& operator = ( char const *str_ );

    // operator =.
    CLuaMysqlString& operator = ( char ch_ );

    // operator +=.
    CLuaMysqlString& operator += ( CLuaMysqlString const &str_ );

    // operator +=.
    CLuaMysqlString& operator += ( char const *str_ );

    // operator +=.
    CLuaMysqlString& operator += ( char ch_ );

    // operator ==.
    bool operator == ( CLuaMysqlString const &str_ ) const;

    // operator ==.
    bool operator == ( char const *str_ ) const;

    // operator !=.
    bool operator != ( CLuaMysqlString const &str_ ) const;

    // operator !=.
    bool operator != ( char const *str_ ) const;

    // operator <.
    bool operator < ( CLuaMysqlString const &str_ ) const;

    // operator <.
    bool operator < ( char const *str_ ) const;

    // operator <=.
    bool operator <= ( CLuaMysqlString const &str_ ) const;

    // operator <=.
    bool operator <= ( char const *str_ ) const;

    // operator >.
    bool operator > ( CLuaMysqlString const &str_ ) const;

    // operator >.
    bool operator > ( char const *str_ ) const;

    // operator >=.
    bool operator >= ( CLuaMysqlString const &str_ ) const;

    // operator >=.
    bool operator >= ( char const *str_ ) const;

    // assign.
    CLuaMysqlString& assign( CLuaMysqlString const &str_ );

    // assign.
    CLuaMysqlString& assign( char const *str_ );

    // assign.
    CLuaMysqlString& assign( char const *str_, size_t sz_ );

    // assign.
    CLuaMysqlString& assign( char ch_, size_t sz_ );

    // append.
    CLuaMysqlString& append( CLuaMysqlString const &str_ );

    // append.
    CLuaMysqlString& append( char const *str_ );

    // append.
    CLuaMysqlString& append( char const *str_, size_t sz_ );

    // assign.
    CLuaMysqlString& append( char ch_, size_t sz_ );

    // compare.
    int compare( CLuaMysqlString const &str_ ) const;

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



#endif // LUA_MYSQL_STRING_H