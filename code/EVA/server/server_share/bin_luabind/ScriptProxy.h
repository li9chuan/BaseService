#ifndef SERVER_SHARD_SCRIPT_PROXY_H
#define SERVER_SHARD_SCRIPT_PROXY_H

namespace bin
{
    struct SScriptObject;

	struct SScriptObjectRef
	{
		struct SScriptObject*	pObject;
		lua_State*				pLua;
		int						nRef;

        // INonCopyable
        SScriptObjectRef(SScriptObjectRef&);
        SScriptObjectRef& operator = (SScriptObjectRef&);

		SScriptObjectRef()
			: pObject(NULL)
			, pLua(NULL)
			, nRef(LUA_NOREF)
		{

		}

		~SScriptObjectRef()
		{
			assert(!pObject);
		}

        void* GetScriptObj();

		// Unlink the connection between c++ object and lua object
		void Unlink();

		// Unlink the lua object reference to c++ object, NB. The c++ object still reference to lua object
		void SideUnlink();

		void OnChangeWeakedTo(bool bWeaked);
	};

	//! a c++ proxy to lua. this is the only way lua accesses to a c++ object. 
	struct SScriptProxy
	{
		enum EProxyType
		{
			EPT_NULL    = 0,
			EPT_OBJECT,
		};

		EProxyType				ePT;
		SScriptObjectRef		objRef;			// Valid only if Proxy takes a object

		SScriptProxy()
			: ePT(EPT_NULL)
		{

		}
	};
};

#endif
