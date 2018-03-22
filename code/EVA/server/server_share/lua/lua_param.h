#ifndef SERVER_SHARD_LUA_PARAM_H
#define SERVER_SHARD_LUA_PARAM_H

#include <nel/misc/types_nl.h>
#include <nel/misc/log.h>
#include <string>

#include <lua.hpp>

class LuaParam
{
public:

    inline int Type()
    {
        return m_Type;
    }	

    inline void SetInt(lua_Integer num)
    {
        m_Int = num;
        m_Type = LUA_TNUMBER;
    }

    inline void SetString(const char* string, uint32 len)
    {
        m_DataHandle    = (void*)string;
        m_Type          = LUA_TSTRING;
        m_Int           = len;
    }

    inline uint32 GetStringLen() { return m_Int; }

    inline void SetDataPointer(void* pointer)
    {
        m_DataHandle = pointer;
        m_Type = LUA_TLIGHTUSERDATA;
    }

    inline void* Data()
    {
        if(m_Type == LUA_TNUMBER)
            return &m_Int;
        return (void*)m_DataHandle;
    }

    inline lua_Integer GetInt()
    {
        return m_Int;
    }

    LuaParam(void):m_Int(0),m_Type(LUA_TNUMBER),m_DataHandle(NULL) { }
    LuaParam(void* pointer):m_Int(0),m_DataHandle(pointer),m_Type(LUA_TLIGHTUSERDATA) { }
    LuaParam(lua_Integer luanumber):m_Int(luanumber),m_Type(LUA_TNUMBER),m_DataHandle(NULL) { }

    virtual ~LuaParam(void){};

private:
    sint16			m_Type;
    void*			m_DataHandle;
    lua_Integer		m_Int;
};

class LuaParams
{
public:
    enum TParamsDefine
    {
        MAX_PARAMS = 3,
    };

    LuaParams():m_Count(0){}
    ~LuaParams(){}

    LuaParams( lua_Number param1 ):
        m_Count(1)
    {
        m_Params[0].SetInt(param1);
    }

    LuaParams( lua_Number param1, lua_Number param2 ):
        m_Count(2)
    {
        m_Params[0].SetInt(param1);
        m_Params[1].SetInt(param2);
    }

    LuaParams( std::string& param1, std::string& param2 ):
        m_Count(2)
    {
        m_Params[0].SetString( param1.c_str(), param1.size() );
        m_Params[1].SetString( param2.c_str(), param2.size() );
    }

    LuaParams( lua_Integer param1, std::string& param2, std::string& param3 ):
        m_Count(3)
    {
        m_Params[0].SetInt(param1);
        m_Params[1].SetString( param2.c_str(), param2.size() );
        m_Params[2].SetString( param3.c_str(), param3.size() );
    }

    //LuaParams( lua_Number param1, lua_Number param2, lua_Number param3 , lua_Number param4 ):
    //m_Count(4)
    //{
    //    m_Params[0].SetInt(param1);
    //    m_Params[1].SetInt(param2);
    //    m_Params[2].SetInt(param3);
    //    m_Params[3].SetInt(param4);
    //}

    //LuaParams( lua_Number param1, lua_Number param2, lua_Number param3 , lua_Number param4 , lua_Number param5 ):
    //m_Count(5)
    //{
    //    m_Params[0].SetInt(param1);
    //    m_Params[1].SetInt(param2);
    //    m_Params[2].SetInt(param3);
    //    m_Params[3].SetInt(param4);
    //    m_Params[4].SetInt(param5);
    //}

    //LuaParams( lua_Number param1, lua_Number param2, lua_Number param3 , lua_Number param4 , lua_Number param5 , lua_Number param6 ):
    //m_Count(6)
    //{
    //    m_Params[0].SetInt(param1);
    //    m_Params[1].SetInt(param2);
    //    m_Params[2].SetInt(param3);
    //    m_Params[3].SetInt(param4);
    //    m_Params[4].SetInt(param5);
    //    m_Params[5].SetInt(param6);
    //}

    //LuaParams( lua_Number param1, lua_Number param2, lua_Number param3 , lua_Number param4 , lua_Number param5 , lua_Number param6 , lua_Number param7 ):
    //m_Count(7)
    //{
    //    m_Params[0].SetInt(param1);
    //    m_Params[1].SetInt(param2);
    //    m_Params[2].SetInt(param3);
    //    m_Params[3].SetInt(param4);
    //    m_Params[4].SetInt(param5);
    //    m_Params[5].SetInt(param6);
    //    m_Params[6].SetInt(param7);
    //}

    //LuaParams( lua_Number param1, lua_Number param2, lua_Number param3 , lua_Number param4 , lua_Number param5 , lua_Number param6 , lua_Number param7 , lua_Number param8 , lua_Number param9 , lua_Number param10 ):
    //m_Count(10)
    //{
    //    m_Params[0].SetInt(param1);
    //    m_Params[1].SetInt(param2);
    //    m_Params[2].SetInt(param3);
    //    m_Params[3].SetInt(param4);
    //    m_Params[4].SetInt(param5);
    //    m_Params[5].SetInt(param6);
    //    m_Params[6].SetInt(param7);
    //    m_Params[7].SetInt(param8);
    //    m_Params[8].SetInt(param9);
    //    m_Params[9].SetInt(param10);
    //}

    LuaParam&  GetParam( uint index );


    //bool AddParam( lua_Number param1 )
    //{
    //    if ( m_Count+1<MAX_PARAMS )
    //    {
    //        m_Params[m_Count]=param1;
    //        ++m_Count;
    //        return true;
    //    }
    //    return false;
    //}

    bool AddParams( LuaParams other )
    {
        if ( other.Count()+m_Count < MAX_PARAMS )
        {
            for ( uint i=0; i<other.Count(); ++i )
            {
                m_Params[m_Count]=other.GetParam(i);
                ++m_Count;
            }
            return true;
        }
        return false;
    }
    
    LuaParam&   at( uint idx ) { return m_Params[idx]; }

    LuaParam* GetParams()   { return m_Params; }
    uint      Count()       { return m_Count;  }

    void      resize( uint size )   { m_Count = size; }
private:

    uint        m_Count;
    LuaParam    m_Params[MAX_PARAMS];

    //std::vector<LuaParam>       m_Params;
    
    
};



#endif


