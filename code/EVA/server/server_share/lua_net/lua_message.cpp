#include "lua_message.h"

void forLuaMessageForceLink()
{
    nlwarning("forLuaMessageForceLink");
}

namespace bin
{
    BEGIN_SCRIPT_CLASS( LuaMessage, CLuaMessage )

    DEFINE_CLASS_FUNCTION( invert, void, ())
    {
        obj->m_Msg.invert();
        return 1;
    }

    DEFINE_CLASS_FUNCTION( name, std::string, ())
    {
        r = obj->m_Msg.getName();
        return 1;
    }

    DEFINE_CLASS_FUNCTION( clear, void, (std::string name))
    {
        obj->m_Msg.clear();
        return 1;
    }

    DEFINE_CLASS_FUNCTION( wbuffer, void, (const char* buff, int len))
    {
        sint32  serial_val=len;
        obj->m_Msg.serial(serial_val);
        obj->m_Msg.serialBuffer((uint8*)buff, len);
        return 1;
    }

    DEFINE_CLASS_FUNCTION( wstring, void, (std::string& str))
    {
        obj->m_Msg.serial(str);
        return 1;
    }

    DEFINE_CLASS_FUNCTION(wtable, void, (CScriptTable& tbl))
    {
        try
        {
            std::string json_str = "";
            obj->GetScriptHandle().CallFunc<CScriptTable&, std::string>("Table2Json", tbl, json_str);
            obj->m_Msg.serial(json_str);
        }
        catch (const NLMISC::EStream&)
        {
            lua_Debug ar;
            lua_getstack(obj->GetScriptHandle().GetHandle(), 1, &ar);
            lua_getinfo(obj->GetScriptHandle().GetHandle(), "Sln", &ar);

            NLMISC::createDebug();
            NLMISC::INelContext::getInstance().getWarningLog()->setPosition(ar.currentline, ar.short_src, ar.name);
            NLMISC::INelContext::getInstance().getWarningLog()->displayNL("wtable exception");
        }
        return 1;
    }


    DEFINE_CLASS_FUNCTION( wbool, void, (bool in_val))
    {
        obj->m_Msg.serial(in_val);
        return 1;
    }

    DEFINE_CLASS_FUNCTION( wint, void, (sint64 in_val))
    {
        obj->m_Msg.serial(in_val);
        return 1;
    }

    DEFINE_CLASS_FUNCTION( wint32, void, (sint32 in_val))
    {
        obj->m_Msg.serial(in_val);
        return 1;
    }

    DEFINE_CLASS_FUNCTION( wint64, void, (sint64 in_val))
    {
        obj->m_Msg.serial(in_val);
        return 1;
    }

    DEFINE_CLASS_FUNCTION( wuint32, void, (lua_Integer in_val))
    {
        uint32  serial_val=in_val;
        obj->m_Msg.serial(serial_val);
        return 1;
    }

    DEFINE_CLASS_FUNCTION( rbool, bool, ())
    {
        nlassert(obj->m_Msg.isReading());
        try
        {
            obj->m_Msg.serial(r);
        }
        catch (const NLMISC::EStream&)
        {
            lua_Debug ar;
            lua_getstack(obj->GetScriptHandle().GetHandle(),1,&ar);
            lua_getinfo(obj->GetScriptHandle().GetHandle(), "Sln", &ar);

            NLMISC::createDebug();
            NLMISC::INelContext::getInstance().getWarningLog()->setPosition( ar.currentline, ar.short_src, ar.name );
            NLMISC::INelContext::getInstance().getWarningLog()->displayNL("rbool is nil");
        }
        return 1;
    }

    DEFINE_CLASS_FUNCTION( rint32, sint32, ())
    {
        nlassert(obj->m_Msg.isReading());
        try
        {
            obj->m_Msg.serial(r);
        }
        catch (const NLMISC::EStream&)
        {
            lua_Debug ar;
            lua_getstack(obj->GetScriptHandle().GetHandle(),1,&ar);
            lua_getinfo(obj->GetScriptHandle().GetHandle(), "Sln", &ar);

            NLMISC::createDebug();
            NLMISC::INelContext::getInstance().getWarningLog()->setPosition( ar.currentline, ar.short_src, ar.name );
            NLMISC::INelContext::getInstance().getWarningLog()->displayNL("rint32 is nil");
        }
        return 1;
    }

    DEFINE_CLASS_FUNCTION( ruint32, lua_Integer, ())
    {
        nlassert(obj->m_Msg.isReading());
        try
        {
            uint32  result=0;
            obj->m_Msg.serial(result);
            r = result;
        }
        catch (const NLMISC::EStream&)
        {
            lua_Debug ar;
            lua_getstack(obj->GetScriptHandle().GetHandle(),1,&ar);
            lua_getinfo(obj->GetScriptHandle().GetHandle(), "Sln", &ar);

            NLMISC::createDebug();
            NLMISC::INelContext::getInstance().getWarningLog()->setPosition( ar.currentline, ar.short_src, ar.name );
            NLMISC::INelContext::getInstance().getWarningLog()->displayNL("ruint32 is nil");
        }
        return 1;
    }

    DEFINE_CLASS_FUNCTION( rint64, sint64, ())
    {
        nlassert(obj->m_Msg.isReading());
        try
        {
            obj->m_Msg.serial(r);
        }
        catch (const NLMISC::EStream&)
        {
            lua_Debug ar;
            lua_getstack(obj->GetScriptHandle().GetHandle(),1,&ar);
            lua_getinfo(obj->GetScriptHandle().GetHandle(), "Sln", &ar);

            NLMISC::createDebug();
            NLMISC::INelContext::getInstance().getWarningLog()->setPosition( ar.currentline, ar.short_src, ar.name );
            NLMISC::INelContext::getInstance().getWarningLog()->displayNL("rint64 is nil");
        }
        return 1;
    }

    DEFINE_CLASS_FUNCTION( rdouble, double, ())
    {
        nlassert(obj->m_Msg.isReading());
        try
        {
            obj->m_Msg.serial(r);
        }
        catch (const NLMISC::EStream&)
        {
            lua_Debug ar;
            lua_getstack(obj->GetScriptHandle().GetHandle(),1,&ar);
            lua_getinfo(obj->GetScriptHandle().GetHandle(), "Sln", &ar);

            NLMISC::createDebug();
            NLMISC::INelContext::getInstance().getWarningLog()->setPosition( ar.currentline, ar.short_src, ar.name );
            NLMISC::INelContext::getInstance().getWarningLog()->displayNL("rdouble is nil");
        }
        return 1;
    }

    DEFINE_CLASS_FUNCTION( rint, sint64, ())
    {
        nlassert(obj->m_Msg.isReading());
        try
        {
            obj->m_Msg.serial(r);
        }
        catch (const NLMISC::EStream&)
        {
            lua_Debug ar;
            lua_getstack(obj->GetScriptHandle().GetHandle(),1,&ar);
            lua_getinfo(obj->GetScriptHandle().GetHandle(), "Sln", &ar);

            NLMISC::createDebug();
            NLMISC::INelContext::getInstance().getWarningLog()->setPosition( ar.currentline, ar.short_src, ar.name );
            NLMISC::INelContext::getInstance().getWarningLog()->displayNL("rint is nil");
        }
        return 1;
    }

    DEFINE_CLASS_FUNCTION( rstring, std::string, ())
    {
        nlassert(obj->m_Msg.isReading());
        try
        {
            obj->m_Msg.serial(r);
        }
        catch (const NLMISC::EStream&)
        {
            lua_Debug ar;
            lua_getstack(obj->GetScriptHandle().GetHandle(),1,&ar);
            lua_getinfo(obj->GetScriptHandle().GetHandle(), "Sln", &ar);

            NLMISC::createDebug();
            NLMISC::INelContext::getInstance().getWarningLog()->setPosition( ar.currentline, ar.short_src, ar.name );
            NLMISC::INelContext::getInstance().getWarningLog()->displayNL("rstring is nil");
        }
        return 1;
    }

    DEFINE_CLASS_FUNCTION( rtable, CScriptTable, ())
    {
        nlassert(obj->m_Msg.isReading());
        try
        {
            std::string json_str;
            obj->m_Msg.serial(json_str);
            obj->GetScriptHandle().NewTable(r);
            obj->GetScriptHandle().CallFunc<std::string&, CScriptTable>( "Json2Table", json_str, r );
        }
        catch (const NLMISC::EStream&)
        {
            lua_Debug ar;
            lua_getstack(obj->GetScriptHandle().GetHandle(),1,&ar);
            lua_getinfo(obj->GetScriptHandle().GetHandle(), "Sln", &ar);

            NLMISC::createDebug();
            NLMISC::INelContext::getInstance().getWarningLog()->setPosition( ar.currentline, ar.short_src, ar.name );
            NLMISC::INelContext::getInstance().getWarningLog()->displayNL("rtable is nil");
        }
        return 1;
    }

    DEFINE_CLASS_FUNCTION( rpb, CScriptTable, (std::string& pbstru))
    {
        nlassert(obj->m_Msg.isReading());
        try
        {
            std::string pb_data;
            obj->m_Msg.serial(pb_data);
            obj->GetScriptHandle().NewTable(r);

            CScriptTable    functbl;
            obj->GetScriptHandle().Get("protobuf", functbl);
            functbl.CallFunc<std::string&, std::string&, CScriptTable>("decode", pbstru, pb_data, r);
        }
        catch (const NLMISC::EStream&)
        {
            lua_Debug ar;
            lua_getstack(obj->GetScriptHandle().GetHandle(),1,&ar);
            lua_getinfo(obj->GetScriptHandle().GetHandle(), "Sln", &ar);

            NLMISC::createDebug();
            NLMISC::INelContext::getInstance().getWarningLog()->setPosition( ar.currentline, ar.short_src, ar.name );
            NLMISC::INelContext::getInstance().getWarningLog()->displayNL("rpb is nil");
        }
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


