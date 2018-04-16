#include "lua_message.h"

void forLuaMessageForceLink()
{
    nlwarning("forLuaMessageForceLink");
}

namespace bin
{
    BEGIN_SCRIPT_CLASS( LuaMessage, CLuaMessage )

    DEFINE_CLASS_FUNCTION( Invert, void, ())
    {
        obj->m_Msg.invert();
        return 1;
    }

    DEFINE_CLASS_FUNCTION( SetType, void, (std::string name))
    {
        obj->m_Msg.setType(name);
        return 1;
    }

    DEFINE_CLASS_FUNCTION( GetType, std::string, ())
    {
        r = obj->m_Msg.getName();
        return 1;
    }

    DEFINE_CLASS_FUNCTION( Clear, void, (std::string name))
    {
        obj->m_Msg.clear();
        return 1;
    }

    DEFINE_CLASS_FUNCTION( WriteBuffer, void, (char* buff, int len))
    {
        sint32  serial_val=len;
        obj->m_Msg.serial(serial_val);
        obj->m_Msg.serialBuffer((uint8*)buff, len);
        return 1;
    }

    DEFINE_CLASS_FUNCTION( WriteSint32, void, (lua_Integer in_val))
    {
        
        sint32  serial_val=in_val;
        obj->m_Msg.serial(serial_val);
        return 1;
    }

    DEFINE_CLASS_FUNCTION( WriteUint32, void, (lua_Integer in_val))
    {
        uint32  serial_val=in_val;
        obj->m_Msg.serial(serial_val);
        return 1;
    }

    DEFINE_CLASS_FUNCTION( ReadBuffer, std::string, ())
    {
        nlassert(obj->m_Msg.isReading());
        sint32  buff_len=0;
        obj->m_Msg.serial(buff_len);
        r.resize(buff_len);
        obj->m_Msg.serialBuffer((uint8*)r.data(), buff_len);
        return 1;
    }

    DEFINE_CLASS_FUNCTION( ReadSint32, lua_Integer, ())
    {
        nlassert(obj->m_Msg.isReading());
        sint32  result=0;
        obj->m_Msg.serial(result);
        r = result;
        return 1;
    }

    DEFINE_CLASS_FUNCTION( ReadUint32, lua_Integer, ())
    {
        nlassert(obj->m_Msg.isReading());
        uint32  result=0;
        obj->m_Msg.serial(result);
        r = result;
        return 1;
    }

    DEFINE_STATIC_FUNCTION(NewInstance, CLuaMessage*, (std::string name))
    {
        r = new CLuaMessage(name);
        r->GetScriptObject().SetDelByScr(true);

        return 1;
    }

    END_SCRIPT_CLASS()
}


