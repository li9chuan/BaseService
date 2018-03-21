#ifndef SERVER_SHARD_SCRIPT_PROXY_H
#define SERVER_SHARD_SCRIPT_PROXY_H

#include "ScriptHandle.h"

namespace bin
{
	struct SScriptObjectRef : INonCopyable
	{
		struct SScriptObject*	pObject;
		lua_State*				pLua;
		int						nRef;

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

		// Unlink the connection between c++ object and lua object
		void Unlink();

		// Unlink the lua object reference to c++ object, NB. The c++ object still reference to lua object
		void SideUnlink();

		void OnChangeWeakedTo(bool bWeaked);
	};

	struct SScriptObject
	{
		SScriptObject()
			: m_pObjRef(NULL)
			, m_pThis(NULL)
			, m_pExporter(NULL)
			, m_bDelByScr(false)
			, m_pReleaser(NULL)
		{

		}

		~SScriptObject()
		{
			Unlink();
		}

		// Copy constructor and assign operator should do nothing 
		SScriptObject(const SScriptObject& r)
			: m_pObjRef(NULL)
			, m_pThis(NULL)
			, m_pExporter(NULL)
			, m_bDelByScr(false)		// Should use the r.m_bDelByScr ? 
			, m_pReleaser(NULL)
		{
			
		}

		SScriptObject& operator = (const SScriptObject& r)
		{
			// Copy nothing about script part
		}

		// Internal used, Attach this object to lua object
		template <typename T>
		void Attach(T* pThis, SScriptObjectRef* pObjRef)
		{
			assert(!m_pObjRef && !m_pThis);

			m_pObjRef   = pObjRef;
			m_pThis     = pThis;
			m_pExporter = pThis->GetExporter();
			assert(m_pExporter);

			m_pReleaser = &SScriptObject::ReleaseImpl<T>;

			InitScript();
		}

		// Internal used, unlink the reference to lua object, NB. lua object will still reference this c++ object
		void SideUnlink()
		{
			ReleaseScript();

			m_pObjRef   = NULL;
			m_pThis     = NULL;
			m_pExporter = NULL;
			m_bDelByScr = false;
		}

		// Internal used, Unlink the connection between c++ object and lua object
		void Unlink()
		{
			if(m_pObjRef)
			{
				m_pObjRef->Unlink();
			}

			assert(!m_pObjRef);
		}

		// Internal used, Release api for lua proxy, NB. this object will be deleted after calling this method
		void ReleaseByScr()
		{
			if(GetDelByScr())
			{
				(this->*m_pReleaser)();
			}
			// Never touch anything later, the object has been destroyed.
		} 

		bool IsExported() const
		{
			return m_pObjRef != NULL;
		}

		// Internal used, Push userdata on top in pL's stack, nil if not exported
		int GetUserData(lua_State* pL) ;

		//! Get the lua_State handle. you can access lua machine from it.
		CScriptHandle& GetScriptHandle()
		{
			return m_scriptHandle;
		}

		//! Get the userdata (lua representation of this class), you can access lua script part from it.
		//! GetUserData is not like GetScriptHandle to return what we want. If do it like that, lua gc can't get the chance to collect a userdata because the reference in ref table.
		//! NB. if you store scriptUserData for later using, scriptUserData will be invalid if the object has been destroyed automatically.
		int GetUserData(CScriptUserData& scriptUserData);

		//! Set if the lua deletes this object(Means lua manages the life of this object).
		void SetDelByScr(bool bDelByScr)
		{
			if(IsExported())
			{
				m_pObjRef->OnChangeWeakedTo(bDelByScr);
			}

			m_bDelByScr = bDelByScr;
		}

		//! Get the lua deleting tag.
		bool GetDelByScr() const
		{
			return m_bDelByScr;
		}

		bool IsWeaked() const
		{
			return GetDelByScr();
		}

	public:
		// Internal used, don't touch these things
		void*                   m_pThis;
		class CClassExporter*   m_pExporter;

	protected:
		template <typename T>
		void ReleaseImpl()
		{
			T* pThis = reinterpret_cast<T*>(m_pThis);

			delete pThis;
		}
		// Initialize script handle and script userdata
		int  InitScript();
		// Release script handle and script userdata
		void ReleaseScript();
	protected:
		bool                    m_bDelByScr;
		SScriptObjectRef*	    m_pObjRef;
		typedef void (SScriptObject::* Releaser)();
		Releaser                m_pReleaser;

		CScriptHandle           m_scriptHandle;
		// NB : this reference will cause the gc can't clean the weak objects table if there is no reference in lua(except the ref table). 
		//CScriptUserData			m_scriptUserData;
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
