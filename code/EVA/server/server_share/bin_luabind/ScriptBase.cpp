#ifndef NL_OS_WINDOWS
#include "ScriptBase.h"
#include "ScriptExporter.h"
#include "ScriptProxy.h"

namespace bin
{
    /*
    template <typename O>	// If a object, must be a proxy
    int TToLua<O*>::Make(O* o, lua_State* pL)
    {
        int nRet = 0;
        if (o)
        {
            if (o->GetScriptObject().IsExported())
            {
                nRet = o->GetScriptObject().GetUserData(pL);
            }
            else
            {
                nRet = bin::ScriptExporterManager().AddScriptObject(o, pL);
            }
        }

        if (!nRet)
        {
            lua_pushnil(pL);

            nRet = 1;
        }

        return nRet;
    }
    

    template <typename O>	// If a object, must be a proxy
    int TFmLua<O*>::Make(lua_State* pL, int nIdx, O*& o)
    {
        o = NULL;

        if (!lua_isuserdata(pL, nIdx))
        {
            return 0;
        }

        void* pUd = lua_touserdata(pL, nIdx);
        if (!pUd)
        {
            return 0;
        }

        SScriptProxy* pProxy = (SScriptProxy*)pUd;

        if (!(pProxy->ePT&SScriptProxy::EPT_OBJECT))
        {
            return 0;
        }

        if (!pProxy->objRef.pObject)
        {
            return 0;
        }

        o = reinterpret_cast<O*>(pProxy->objRef.pObject->m_pThis);

        return 1;
    }
    */
};



#endif // !NL_OS_WINDOWS

