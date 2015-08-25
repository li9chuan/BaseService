#pragma once

#include <nel/misc/types_nl.h>
#include <nel/misc/log.h>
#include <string>

extern   "C"   
{   
#include   <lua.h> 
#include   <lualib.h>   
#include   <lauxlib.h>
}

NLMISC::CLog &LuaLoger();

class LuaParam
{
public:
    inline int Type()
    {
        return m_nType;
    }	
    inline void SetDataNum(lua_Number num)
    {
        m_nDataNumber = num;
        m_nType = LUA_TNUMBER;
    }
    inline void SetDataString(const char* string)
    {
        m_pData = (void*)string;
        m_nType = LUA_TSTRING;
    }
    inline void SetDataPointer(void* pointer)
    {
        m_pData = pointer;
        m_nType = LUA_TLIGHTUSERDATA;
    }
    inline void* Data()
    {
        if(m_nType == LUA_TNUMBER)
            return &m_nDataNumber;
        return (void*)m_pData;
    }
    inline lua_Number DataNumber()
    {
        return m_nDataNumber;
    }
    LuaParam(void):m_nDataNumber(0),m_nType(LUA_TNUMBER) { }
    LuaParam(void* pointer):m_pData(pointer),m_nType(LUA_TLIGHTUSERDATA) { }
    LuaParam(lua_Number luanumber):m_nDataNumber(luanumber),m_nType(LUA_TNUMBER) { }
    virtual ~LuaParam(void){};

private:
    int				m_nType;
    void*			m_pData;
    lua_Number		m_nDataNumber;
};

class LuaParams
{
public:
    enum TParamsDefine
    {
        MAX_PARAMS = 10,
    };

    LuaParams():m_Count(0){}
    ~LuaParams(){}

    LuaParams( lua_Number param1 ):
        m_Count(1)
    {
        m_Params[0].SetDataNum(param1);
    }

    LuaParams( lua_Number param1, lua_Number param2 ):
        m_Count(2)
    {
        m_Params[0].SetDataNum(param1);
        m_Params[1].SetDataNum(param2);
    }

    LuaParams( lua_Number param1, lua_Number param2, lua_Number param3 ):
        m_Count(3)
    {
        m_Params[0].SetDataNum(param1);
        m_Params[1].SetDataNum(param2);
        m_Params[2].SetDataNum(param3);
    }

    LuaParams( lua_Number param1, lua_Number param2, lua_Number param3 , lua_Number param4 ):
    m_Count(4)
    {
        m_Params[0].SetDataNum(param1);
        m_Params[1].SetDataNum(param2);
        m_Params[2].SetDataNum(param3);
        m_Params[3].SetDataNum(param4);
    }

    LuaParams( lua_Number param1, lua_Number param2, lua_Number param3 , lua_Number param4 , lua_Number param5 ):
    m_Count(5)
    {
        m_Params[0].SetDataNum(param1);
        m_Params[1].SetDataNum(param2);
        m_Params[2].SetDataNum(param3);
        m_Params[3].SetDataNum(param4);
        m_Params[4].SetDataNum(param5);
    }

    LuaParams( lua_Number param1, lua_Number param2, lua_Number param3 , lua_Number param4 , lua_Number param5 , lua_Number param6 ):
    m_Count(6)
    {
        m_Params[0].SetDataNum(param1);
        m_Params[1].SetDataNum(param2);
        m_Params[2].SetDataNum(param3);
        m_Params[3].SetDataNum(param4);
        m_Params[4].SetDataNum(param5);
        m_Params[5].SetDataNum(param6);
    }

    LuaParams( lua_Number param1, lua_Number param2, lua_Number param3 , lua_Number param4 , lua_Number param5 , lua_Number param6 , lua_Number param7 ):
    m_Count(7)
    {
        m_Params[0].SetDataNum(param1);
        m_Params[1].SetDataNum(param2);
        m_Params[2].SetDataNum(param3);
        m_Params[3].SetDataNum(param4);
        m_Params[4].SetDataNum(param5);
        m_Params[5].SetDataNum(param6);
        m_Params[6].SetDataNum(param7);
    }

    LuaParams( lua_Number param1, lua_Number param2, lua_Number param3 , lua_Number param4 , lua_Number param5 , lua_Number param6 , lua_Number param7 , lua_Number param8 , lua_Number param9 , lua_Number param10 ):
    m_Count(10)
    {
        m_Params[0].SetDataNum(param1);
        m_Params[1].SetDataNum(param2);
        m_Params[2].SetDataNum(param3);
        m_Params[3].SetDataNum(param4);
        m_Params[4].SetDataNum(param5);
        m_Params[5].SetDataNum(param6);
        m_Params[6].SetDataNum(param7);
        m_Params[7].SetDataNum(param8);
        m_Params[8].SetDataNum(param9);
        m_Params[9].SetDataNum(param10);
    }

    lua_Number  GetParam( uint index, lua_Number defaultValue=0 );


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
    

    LuaParam* GetParams()   { return m_Params; }
    uint      Count()       { return m_Count;  }

    void      resize( uint size )   { m_Count = size; }
private:

    uint        m_Count;
    LuaParam    m_Params[MAX_PARAMS];
    
    
};






