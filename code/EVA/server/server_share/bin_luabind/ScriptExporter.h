#ifndef SERVER_SHARD_SCRIPT_EXPORTER_H
#define SERVER_SHARD_SCRIPT_EXPORTER_H

#include "ScriptBase.h"
#include "ScriptProxy.h"

namespace bin
{
	struct SExporterInfo
	{
		enum EExporterType
		{
			EET_MODULE		= 0,			// A module, in lua just a table
			EET_FUNCTION,					// Function
			EET_CLASS,						// Class, in lua a userdata
			EET_SIZE,
		};

		const char*		pszName;
		EExporterType	eET;
		SExporterInfo()
			: pszName(NULL)
			, eET(EET_SIZE)
		{

		}
	};
		
	class IScriptExporter
	{
	public:
		virtual ~IScriptExporter(){}
		virtual int Export(lua_State* pL) = 0;
		virtual SExporterInfo GetInfo() const = 0;
	};

	class CScriptExporterManager
	{
	public:
		typedef CHashMap<std::string, IScriptExporter*>			ScriptExporters;
		typedef ScriptExporters::const_iterator					ExporterIterator;
	public:
		CScriptExporterManager()
		{

		}

		// Internal used, the life of pExporter must be global
		int RegisterExporter(IScriptExporter* pExporter)
		{
			if(!pExporter)
			{
				return 0;
			}

			SExporterInfo info = pExporter->GetInfo();
			std::pair<ExporterIterator, bool> ret = m_scriptExporters.insert(ScriptExporters::value_type(info.pszName, pExporter));
			assert(ret.second && "ScriptBase.h [RegisterExporter] : Exporter conflict");

			return 1;
		}

		// Internal used, The user data is on the top of stack
		template <typename T>
		int AddScriptObject(T* pObj, lua_State* pL)
		{
#if defined _DEBUG
			int nOldTop = lua_gettop(pL);
#endif

			SScriptProxy* pProxy = (SScriptProxy*)lua_newuserdata(pL, sizeof(SScriptProxy));
			if(!pProxy)
			{
				return 0;
			}

			new(pProxy) SScriptProxy;

			pProxy->ePT                 = SScriptProxy::EPT_OBJECT;
			pProxy->objRef.pObject      = &pObj->GetScriptObject();
			pProxy->objRef.pLua         = pL;
			
			// Add property table for this object, this table will store the object field value
			lua_newtable(pL);
			lua_pushvalue(pL, -1);
			lua_setfield(pL, -2, "__newindex");		// Assign support
			lua_pushvalue(pL, -1);
			lua_setfield(pL, -2, "__index");		// Access support
			lua_pushstring(pL, "object");
			lua_setfield(pL, -2, "type");

            {
                lua_pushvalue(pL, -1);
                lua_setfield(pL, -2, "__bin_objtable");    // Setup object table, so we can set a new type on this object
            }

			// Set metatable to the new table, this metatable provides "class member functions"
			{
				SExporterInfo exporterInfo = pObj->GetExporter()->GetInfo();
				assert(exporterInfo.eET == SExporterInfo::EET_CLASS);
				luaL_getmetatable(pL, exporterInfo.pszName);
			
				// Set __gc, __gc will be called when the userdata(or table) is destroyed 
				lua_getfield(pL, -1, "__bin_gc");
				lua_setfield(pL, -3, "__gc");
			
				lua_setmetatable(pL, -2);
			}

			// Set the new table as the metatable of this userdata, now the object is on the stack
			// This metatable provides "class member fields and lua defined member functions"
			lua_setmetatable(pL, -2);

			// Add the object to objects table, objects table is on the stack
			CheckObjectsTable(pL, pObj->GetScriptObject().IsWeaked());
			lua_pushvalue(pL, -2);	// Set object to top
			pProxy->objRef.nRef = luaL_ref(pL, -2);
			lua_pop(pL, 1);			// pop the objects table

			// Attach object to lua
			pObj->GetScriptObject().Attach(pObj, &pProxy->objRef);

#if defined _DEBUG
			int nNewTop = lua_gettop(pL);
			assert(nNewTop == nOldTop+1);
#endif

			return 1;
		}

		// Internal used, The user data is on the top of stack
		template <typename T>
		T* NewScriptObject(lua_State* pL, bool bDelByScr = true)
		{
			T* pObj = new T();
			pObj->GetScriptObject().SetDelByScr(bDelByScr);

			if(!AddScriptObject(pObj, pL))
			{
				delete pObj;
				pObj = NULL;
			}

			return pObj;
		}

		// Internal used, The user data is on the top of stack
		template <typename T>
		int AddScriptObject(const char* pszName, T* pObj, lua_State* pL)
		{
			if(!AddScriptObject(pObj, pL))
			{
				return 0;
			}
			
			lua_pushvalue(pL, -1);
			lua_setglobal(pL, pszName);

			return 1;
		}

		// Internal used
		int CheckObjectsTable(lua_State* pL, bool bWeaked)
		{
			const char* pszName = bWeaked ? SCRIPT_OWN_OBJECTS : SCRIPT_USE_OBJECTS;
			lua_getfield(pL, LUA_REGISTRYINDEX, pszName);
			if(lua_isnil(pL, -1))
			{
				lua_pop(pL, 1);		// Pop the nil value

				lua_newtable(pL);
				
				//Set objects table as value weak table
				if(bWeaked)
				{
					lua_newtable(pL);
					lua_pushstring(pL, "v");
					lua_setfield(pL, -2, "__mode");
					lua_setmetatable(pL, -2);
				}

				lua_pushvalue(pL, -1);
				lua_setfield(pL, LUA_REGISTRYINDEX, pszName);
			}

			return 1;
		}

		// Internal used
		int CheckRefsTable(lua_State* pL)
		{
			lua_getfield(pL, LUA_REGISTRYINDEX, SCRIPT_REFS);
			if(lua_isnil(pL, -1))
			{
				lua_pop(pL, 1);

				lua_newtable(pL);

				lua_pushvalue(pL, -1);
				lua_setfield(pL, LUA_REGISTRYINDEX, SCRIPT_REFS);
			}

			return 1;
		}

		// Internal used
		int CheckTypesTable(lua_State* pL)
		{
			lua_getfield(pL, LUA_REGISTRYINDEX, SCRIPT_TYPES);
			if(lua_isnil(pL, -1))
			{
				lua_pop(pL, 1);

				lua_newtable(pL);

				lua_pushvalue(pL, -1);
				lua_setfield(pL, LUA_REGISTRYINDEX, SCRIPT_TYPES);

				lua_pushvalue(pL, -1);
				lua_setglobal(pL, "bin_types");
			}

			return 1;
		}

		// Internal used
		int ExportClass(const char* pszName, lua_State* pL, const char* pszNameSpace = NULL);

		// Internal used
		int ExportModule(const char* pszName, lua_State* pL);

		// Internal used
		int ExportModuleTo(const char* pszName, lua_State* pL, CScriptTable& scriptTable);

		int ExportClass(const char* pszName, CScriptHandle& scriptHandle, const char* pszNameSpace = NULL)
		{
			if(scriptHandle.IsNull())
			{
				return 0;
			}

			return ExportClass(pszName, scriptHandle.GetHandle(), pszNameSpace);
		}

		int ExportModule(const char* pszName, CScriptHandle& scriptHandle)
		{
			if(scriptHandle.IsNull())
			{
				return 0;
			}

			return ExportModule(pszName, scriptHandle.GetHandle());
		}

		int ExportModuleTo(const char* pszName, CScriptHandle& scriptHandle, const char* pszTableName)
		{
			// Get the table
			CScriptTable scriptTable;
			scriptHandle.Get(pszTableName, scriptTable);
			
			bool bNewed = !scriptTable.IsReferd();
			int ret = ExportModuleTo(pszName, scriptHandle, scriptTable);
			if(ret && bNewed)	// The table is a new table ?
			{
				scriptHandle.Set(pszTableName, scriptTable);
			}

			return ret;
		}

		int ExportModuleTo(const char* pszName, CScriptHandle& scriptHandle, CScriptTable& scriptTable)
		{
			if(scriptHandle.IsNull())
			{
				return 0;
			}

			bool bNeedUnref = false;
			if(!scriptTable.IsReferd())
			{
				scriptHandle.NewTable(scriptTable);
				bNeedUnref = true;
			}

			int ret = ExportModuleTo(pszName, scriptHandle.GetHandle(), scriptTable);

			if(!ret && bNeedUnref)
			{
				scriptTable.UnRef();
			}

			return ret;
		}

	private:
		ScriptExporters			m_scriptExporters;
	};
	inline CScriptExporterManager& ScriptExporterManager()
	{
		static CScriptExporterManager s_scriptExpoterManager;

		return s_scriptExpoterManager;
	}

	struct SModuleFunction
	{
		const char*			pszName;
		lua_CFunction		pFunc;
		SModuleFunction(const char* pN = NULL, lua_CFunction pF = NULL)
			: pszName(pN), pFunc(pF)
		{

		}
	};

	struct SClassFunction
	{
		const char*			pszName;
		lua_CFunction		pFunc;
		SClassFunction(const char* pN = NULL, lua_CFunction pF = NULL)
			: pszName(pN), pFunc(pF)
		{

		}
	};

	template <typename T>
	struct TLinkNode : T
	{
		TLinkNode*	pNxt;
		TLinkNode()
			: pNxt(NULL)
		{

		}
	};

	typedef TLinkNode<SModuleFunction> ModuleFunctionLinkNode;
	typedef TLinkNode<SClassFunction>  ClassFunctionLinkNode;

	class CModuleExporter : public IScriptExporter
	{
	public:
		CModuleExporter(const char* pszName, ModuleFunctionLinkNode** ppHead)
			: m_pszModuleName(pszName)
			, m_ppHead(ppHead)
		{
			assert(m_pszModuleName && m_ppHead);

			ScriptExporterManager().RegisterExporter(this);
		}

		virtual int Export(lua_State* pL)
		{
			CHECK_LUA_STACK(pL);

			const ModuleFunctionLinkNode* pNode = GetModuleFunctionList();	// Empty module
			if(!pNode)
			{
				return 1;
			}

			const int SIZE_HINT = 10;
			luaL_pushmodule(pL, GetModuleName(), SIZE_HINT);

			while(pNode)
			{
				lua_pushcfunction(pL, pNode->pFunc);
				lua_setfield(pL, -2, pNode->pszName);

				pNode = pNode->pNxt;
			}

			lua_pushstring(pL, GetModuleName());
			lua_setfield(pL, -2, "name");

			lua_pushstring(pL, "module");
			lua_setfield(pL, -2, "type");

			lua_pushcfunction(pL, &__Imported);
			lua_setfield(pL, -2, "imported");

			return 1;
		}

		int ExportTo(lua_State* pL, CScriptTable& scriptTable)
		{
			assert(scriptTable.GetHandle() == pL);

			CHECK_LUA_STACK(pL);

			const ModuleFunctionLinkNode* pNode = GetModuleFunctionList();	// Empty module
			if(!pNode)
			{
				return 1;
			}

			scriptTable.PrepareStack();	// table

			while(pNode)
			{
				lua_pushcfunction(pL, pNode->pFunc);	// table, func
				lua_setfield(pL, -2, pNode->pszName);	// table

				pNode = pNode->pNxt;
			}

			return 1;
		}

		SExporterInfo GetInfo() const
		{
			SExporterInfo info;

			info.pszName = GetModuleName();
			info.eET     = SExporterInfo::EET_MODULE;

			return info;
		}

		const char* GetModuleName() const
		{
			return m_pszModuleName;
		}

		const ModuleFunctionLinkNode* GetModuleFunctionList() const
		{
			return *m_ppHead;
		}
	protected:
		static int __Imported(lua_State* pL)
		{
			lua_pushboolean(pL, true);

			return 1;
		}

	private:
		const char*					m_pszModuleName;
		ModuleFunctionLinkNode**	m_ppHead;
	};

	class CClassExporter : public IScriptExporter
	{
	public:
		CClassExporter(const char* pszSuper, const char* pszName, ClassFunctionLinkNode** ppHead)
			: m_pszSuperName(pszSuper)
			, m_pszClassName(pszName)
			, m_ppHead(ppHead)
		{
			assert(m_pszClassName && m_ppHead);

			ScriptExporterManager().RegisterExporter(this);
		}

		int Export(lua_State* pL, const char* pszNameSpace)
		{
			CHECK_LUA_STACK(pL);

			if(luaL_newmetatable(pL, GetClassName()) != 0)	// Has not been exported ?
			{
				lua_pushstring(pL, GetClassName());
				lua_setfield(pL, -2, "name");

				lua_pushstring(pL, "class");
				lua_setfield(pL, -2, "type");

				lua_pushvalue(pL, -1);
				lua_setfield(pL, -2, "__index");

				lua_pushcfunction(pL, &__GC);
				lua_setfield(pL, -2, "__bin_gc");

				lua_pushcfunction(pL, &__Imported);
				lua_setfield(pL, -2, "imported");

				if(m_pszSuperName)  // Export the super class
				{
					ScriptExporterManager().ExportClass(m_pszClassName, pL, pszNameSpace);

					luaL_getmetatable(pL, m_pszSuperName);  // newClass, superClass
                    lua_pushvalue(pL, -1);                  // newClass, superClass, superClass

                    lua_setfield(pL, -3, "super");          // newClass, superClass
                    lua_setmetatable(pL, -2);
				}

				const ClassFunctionLinkNode* pNode = GetClassFunctionList();	
				while(pNode)
				{
					lua_pushcfunction(pL, pNode->pFunc);
					lua_setfield(pL, -2, pNode->pszName);

					pNode = pNode->pNxt;
				}
			}
			// Now type is on the stack
			{
				int nTypeIdx = -2;
				CHECK_LUA_STACK(pL);

				ScriptExporterManager().CheckTypesTable(pL);
				if(pszNameSpace)
				{
					lua_getfield(pL, -1, pszNameSpace);	// type, types, value	
					if(lua_isnil(pL, -1))
					{
						lua_pop(pL, 1);					// type, types
						lua_newtable(pL);				// type, types, namespace 
						lua_pushvalue(pL, -1);			// type, types, namespace, namespace

						lua_setfield(pL, -3, pszNameSpace);// type, types, namespace
					}
					else if(!lua_istable(pL, -1))
					{
						luaL_error(pL, "namespace[%s] must be a table", pszNameSpace);
					}

					nTypeIdx = -3;
				}

				// Now the type table(or namespace) is on the stack
				lua_pushvalue(pL, nTypeIdx);				// tbl, type
				lua_setfield(pL, -2, GetClassName());			
			}
						
			return 1;
		}

		virtual int Export(lua_State* pL)
		{
			return Export(pL, NULL);
		}

		SExporterInfo GetInfo() const
		{
			SExporterInfo info;

			info.pszName = GetClassName();
			info.eET     = SExporterInfo::EET_CLASS;

			return info;
		}

		const char* GetClassName() const
		{
			return m_pszClassName;
		}

		const ClassFunctionLinkNode* GetClassFunctionList() const
		{
			return *m_ppHead;
		}

	private:
		const char*					m_pszSuperName;
		const char*					m_pszClassName;
		ClassFunctionLinkNode**	    m_ppHead;

		// Static
	protected:
		static int __GC(lua_State* pL)
		{
			void* pUd = lua_touserdata(pL, 1);
			if(!pUd)
			{
				return 0;
			}

			SScriptProxy* pProxy = (SScriptProxy*)pUd;

			if(!(pProxy->ePT&SScriptProxy::EPT_OBJECT))
			{
				return 0;
			}

			if(!pProxy->objRef.pObject)
			{
				return 0;
			}

			if(pProxy->objRef.pObject->GetDelByScr())
			{
				pProxy->objRef.pObject->ReleaseByScr();	// Will call objRef.Unlink();

				assert(!pProxy->objRef.pObject);
			}
			else
			{
				pProxy->objRef.Unlink();
			}

			return 0;
		}

		// Check whether this lua object still reference to a c++ object 
		static int __Imported(lua_State* pL)
		{
			bool bRet = false;
			SScriptProxy* pProxy = (SScriptProxy*)lua_touserdata(pL, 1);
			if(pProxy && (pProxy->ePT&SScriptProxy::EPT_OBJECT) && pProxy->objRef.pObject)
			{
				bRet = true;
			}
			lua_pushboolean(pL, bRet);

			return 1;
		}
	};
};

#endif
