#ifndef SERVER_SHARD_SCRIPT_HANDLE_H
#define SERVER_SHARD_SCRIPT_HANDLE_H

#include "ScriptBase.h"

namespace bin
{
	struct SDBLinkNode
	{
		SDBLinkNode*	pPre;
		SDBLinkNode*	pNxt;

		SDBLinkNode()
			: pPre(NULL)
			, pNxt(NULL)
		{

		}

		void LinkAfter(SDBLinkNode* pNode)
		{
			pNxt = pNode->pNxt;
			pPre = pNode;

			pPre->pNxt = this;
			if(pNxt)
			{
				pNxt->pPre = this;
			}
		}

		void LinkBefore(SDBLinkNode* pNode)
		{
			pNxt = pNode;
			pPre = pNode->pPre;

			pNxt->pPre = this;
			if(pPre)
			{
				pPre->pNxt = this;
			}
		}

		void Unlink()
		{
			if(pPre)
			{
				pPre->pNxt = pNxt;
			}

			if(pNxt)
			{
				pNxt->pPre = pPre;	
			}

			pPre = NULL;
			pNxt = NULL;
		}
	};

	struct SScriptHandleRefNode : SDBLinkNode
	{
		typedef void (*UnLinker)(SScriptHandleRefNode*);

		UnLinker		unLinker;
		SScriptHandleRefNode()
			: unLinker(NULL)
		{

		}
	};

	class CScriptTable;
	class CScriptUserData;

	//! Encapsulate a lua_State, provide global setting and reading interface
	class CScriptHandle : INonCopyable
	{
	public:
		CScriptHandle()
			: m_bOwn(true)
			, m_pLua(NULL)
		{

		}

		~CScriptHandle()
		{
			Release();
		}
		
		//! Initialize a CScriptHandle, NB. This CScriptHandle must be released state
		int Init();

		//! Initialize a CScriptHandle from file.
		int Init(const char* pszFileName)
		{
			if(!Init())
			{
				return 0;
			}

			if(!Exec(pszFileName))
			{
				goto OPERATION_FAIL;
			}
			
			return 1;
OPERATION_FAIL:
			Release();

			return 0;
		}

		//! Initialize a CScriptHandle from a exist lua_State.
		//! \param bOwn  indicate whether CScriptHandle will own this pLua, if true, pLua will be closed while releasing this handle.
		int Init(lua_State* pLua, bool bOwn = false)
		{
			Release();

			m_pLua = pLua;
			m_bOwn = bOwn; 

			return 1;
		}

		//! Init from a lua string.
		int InitFmString(const char* pszString)
		{
			if(!Init())
			{
				return 0;
			}

			if(!ExecString(pszString))
			{
				goto OPERATION_FAIL;
			}

			return 1;
OPERATION_FAIL:
			Release();

			return 0;
		}

		//! Release this handle, if this handle owns lua_State, lua_State will be closed.
		//! All the tables and userdatas got from this handle will be unlinked( IsReferd() is false) 
		void Release()
		{
			// Unlink all relations
			SScriptHandleRefNode* pNode = &m_head;
			SScriptHandleRefNode* pNxt  = NULL;
			while(pNode)
			{
				pNxt = (SScriptHandleRefNode*)pNode->pNxt;
				if(pNode->unLinker)
				{
					pNode->unLinker(pNode);
				}

				pNode = pNxt;
			}

			if(m_pLua)
			{
				if(m_bOwn)
				{
					lua_close(m_pLua);
				}

				m_pLua = NULL;
			}

			assert(!m_head.pNxt && !m_head.pPre && !m_head.unLinker);
		}

		//! Exec a lua file
		int Exec(const char* pszFileName)
		{
			if(IsNull() || !pszFileName)
			{
				return 0;
			}

			CHECK_LUA_STACK(m_pLua);

			int nRet = luaL_dofile(m_pLua, pszFileName);
			if(nRet != LUA_OK)	// Error, the error message is on the top
			{
				LOG_MESSAGE("[ERROR] Exec File[%s] : %s", pszFileName, lua_tostring(m_pLua, -1));

				return 0;
			}

			return 1;
		}

		//! Exec a lua file
		int Exec(const char* pszFileName,  CScriptTable& ret);

		//! Exec a lua string
		int ExecString(const char* pszString)
		{
			if(IsNull() || !pszString)
			{
				return 0;
			}

			CHECK_LUA_STACK(m_pLua);

			int nRet = luaL_dostring(m_pLua, pszString);
			if(nRet != LUA_OK)	// Error, the error message is on the top
			{
				LOG_MESSAGE("[ERROR] Exec String : %s", lua_tostring(m_pLua, -1));

				return 0;
			}

			return 1;
		}

		//! Exec a lua string, ret contains the return value from string
		int ExecString(const char* pszString, CScriptTable& ret);

		//! Check if this handle is Null.
		bool IsNull() const
		{
			return m_pLua == NULL;
		}

		//! Get the lua_State associated with this handle.
		lua_State* GetHandle() const
		{
			return m_pLua;
		}

		//! Get any thing from the global table in this handle.
		//! \param pszName the name of the variable.
		//! \param v the out value.
		//! \return 0-fail
		template <typename T>
		int Get(const char* pszName, T& v)
		{
			if(IsNull())
			{
				return 0;
			}

			CHECK_LUA_STACK(m_pLua);

			lua_getglobal(m_pLua, pszName);

			return TFmLua<typename TArgumentType<T>::value_type>::Make(m_pLua, -1, v);
		}

		//! Set anything in global table.
		//! \param pszName name of the variable.
		//! \param v input value.
		//! \return 0-fail
		template <typename T>
		int Set(const char* pszName, T v)
		{
			if(IsNull())
			{
				return 0;
			}

			CHECK_LUA_STACK(m_pLua);

			if(!TToLua<typename TArgumentType<T>::value_type>::Make(v, m_pLua))
			{
				return 0;
			}

			lua_setglobal(m_pLua, pszName);

			return 1;
		}

		//! Set a table, template function must pass variable in value, so pass tbl in reference here.
		int Set(const char* pszName, CScriptTable& tbl)
		{
			return Set<CScriptTable&>(pszName, tbl);
		}

		//! See int Set(const char* pszName, CScriptTable& tbl)
		int Set(const char* pszName, CScriptUserData& ud)
		{
			return Set<CScriptUserData&>(pszName, ud);
		}

		//! See int Set(const char* pszName, CScriptTable& tbl)
		int Set(const char* pszName, const std::string& str)
		{
			return Set<const std::string&>(pszName, str);
		}

		//! Get a table from global table. if fail, tbl is invalid(IsRefered() is false).
		//! NB. tbl will be linked to this handle, so if handle is released, tbl will know this event.
		int Get(const char* pszName, CScriptTable& tbl);

		//! Get a userdata from global table. a userdata is a class you export to lua in fact.
		int Get(const char* pszName, CScriptUserData& ud);

		//! New a table, tbl will reference to it.
		int NewTable(CScriptTable& tbl);

		//! New a table, tbl will reference to it and this table will be set to global variable named pszName.
		int NewTable(const char* pszName, CScriptTable& tbl);

#define ARGUMENT_TYPE(arg) typename TArgumentType<arg>::argument_type
#define VALUE_TYPE(arg)    typename TArgumentType<arg>::value_type

#define FUNC_PREWORK() \
		InvlRetVal(r);\
		if(IsNull())\
		{\
			return 0;\
		}\
		CHECK_LUA_STACK(m_pLua);\
		lua_getglobal(m_pLua, pszName);\
		if(!lua_isfunction(m_pLua, -1))\
		{\
			return 0;\
		}

#define MAKE_ARG(type, vale)\
		if(!TToLua<VALUE_TYPE(type)>::Make((vale), m_pLua))\
		{\
			return 0;\
		}

#define MAKE_RET()\
	int __nFmRet = TFmLua<R>::Make(m_pLua, -1, r)

#define FUNC_POSWORK()\
	return PostFmLua(__nFmRet, r);

		//! Call a global function named pszName, r is the returned value.
		//! \return 0-fail.
		template <typename R>
		int CallFunc(const char* pszName, R& r)
		{
			FUNC_PREWORK();

			if(lua_pcall(m_pLua, 0, 1, 0) != LUA_OK)	// Error msg here
			{
				LOG_MESSAGE("[ERROR] Call Func[%s] : %s", pszName, lua_tostring(m_pLua, -1));
				return 0;
			}

			MAKE_RET();

			FUNC_POSWORK();
		}

		//! Call a global function named pszName, r is the returned value, a0 is the only argument.
		//! NB. you must indicate the argument type explicitly like CallFunc<Type>(name, arg, ret)
		//! NB. the return type will be interpreted by compiler, there is no need to indicate it's type.
		//! \return 0-fail.
		template <typename A0, typename R>
		int CallFunc(const char* pszName, ARGUMENT_TYPE(A0) a0, R& r)
		{
			FUNC_PREWORK();

			MAKE_ARG(A0, a0);

			if(lua_pcall(m_pLua, 1, 1, 0) != LUA_OK)	// Error msg here
			{
				LOG_MESSAGE("[ERROR] Call Func[%s] : %s", pszName, lua_tostring(m_pLua, -1));
				return 0;
			}

			MAKE_RET();
			FUNC_POSWORK();
		}

		//! Two args version, see One args version.
		template <typename A0, typename A1, typename R>
		int CallFunc(const char* pszName, ARGUMENT_TYPE(A0) a0, ARGUMENT_TYPE(A1) a1, R& r)
		{
			FUNC_PREWORK();

			MAKE_ARG(A0, a0);
			MAKE_ARG(A1, a1);

			if(lua_pcall(m_pLua, 2, 1, 0) != LUA_OK)	// Error msg here
			{
				LOG_MESSAGE("[ERROR] Call Func[%s] : %s", pszName, lua_tostring(m_pLua, -1));
				return 0;
			}

			MAKE_RET();

			FUNC_POSWORK();
		}

        //! Three args version, see One args version.
        template <typename A0, typename A1, typename A2, typename R>
        int CallFunc(const char* pszName, ARGUMENT_TYPE(A0) a0, ARGUMENT_TYPE(A1) a1, ARGUMENT_TYPE(A2) a2, R& r)
        {
            FUNC_PREWORK();

            MAKE_ARG(A0, a0);
            MAKE_ARG(A1, a1);
            MAKE_ARG(A2, a2);

            if (lua_pcall(m_pLua, 3, 1, 0) != LUA_OK)	// Error msg here
            {
                LOG_MESSAGE("[ERROR] Call Func[%s] : %s", pszName, lua_tostring(m_pLua, -1));
                return 0;
            }

            MAKE_RET();

            FUNC_POSWORK();
        }
#undef ARGUMENT_TYPE
#undef VALUE_TYPE
#undef FUNC_PREWORK
#undef MAKE_THIS
#undef MAKE_ARG
#undef MAKE_RET
#undef FUNC_POSWORK
	
	protected:
		template <typename T>
		int PostFmLua(int nFmLuaRet, T)
		{
			return nFmLuaRet;
		}

		int PostFmLua(int nFmLuaRet, CScriptTable& tbl);

		int PostFmLua(int nFmLuaRet, CScriptUserData& ud);

		template <typename T>
		void InvlRetVal(T& )
		{

		}

		void InvlRetVal(CScriptTable& tbl);

		void InvlRetVal(CScriptUserData& ud);

	private:
		bool					m_bOwn;
		lua_State*				m_pLua;
		SScriptHandleRefNode    m_head;
	};

	// Script abstract data base : abstract data means table and user data type
	// It's internal used, Never work with a table or userdata with this parent class 
	class IScriptADBase : INonCopyable
	{
	protected:
		struct SRefNode : SScriptHandleRefNode
		{
			SRefNode()
				: pLua(NULL)
				, nRef(LUA_NOREF)
			{
				unLinker = &UnLinker;
			}

			lua_State*				pLua;
			int						nRef;

			bool IsReferd() const
			{
				return pLua && nRef != LUA_NOREF;
			}

			void UnRef() 
			{
				if(unLinker)
				{
					unLinker(this);
				}
			}

		protected:
			// Static
			static void UnLinker(SScriptHandleRefNode* pT);
		};

		typedef SRefNode RefNode;

	public:
		IScriptADBase()
		{

		}

		~IScriptADBase()
		{
			UnRef();
		}

		bool IsReferd() const
		{
			return m_ref.IsReferd();
		}

		void UnRef()
		{
			m_ref.UnRef();
		}

		//! Get value from the ad type(table or userdata)
		template <typename T>
		int Get(const char* pszName, T& v)
		{
			if(!IsReferd())
			{
				return 0;
			}

			lua_State* pLua = m_ref.pLua;

			CHECK_LUA_STACK(pLua);

			if(!PrepareStack()) // this_table is on the top
			{
				return 0;
			}

			lua_getfield(pLua, -1, pszName);				// __bin_refs this_table value

			return TFmLua<typename TArgumentType<T>::value_type>::Make(pLua, -1, v);
		}

		template <typename T>
		int Set(const char* pszName, T v)
		{
			if(!IsReferd())
			{
				return 0;
			}

			lua_State* pLua = m_ref.pLua;

			CHECK_LUA_STACK(pLua);

			if(!PrepareStack())
			{
				return 0;
			}

			if(!TToLua<typename TArgumentType<T>::value_type>::Make(v, pLua))
			{
				return 0;
			}

			lua_setfield(pLua, -2, pszName);

			return 1;
		}

		int Set(const char* pszName, CScriptTable& tbl)
		{
			return Set<CScriptTable&>(pszName, tbl);
		}

		int Set(const char* pszName, CScriptUserData& ud)
		{
			return Set<CScriptUserData&>(pszName, ud);
		}

		int Set(const char* pszName, const std::string& str)
		{
			return Set<const std::string&>(pszName, str);
		}

		//! Get value from ad type(Call this method means you treat ad type as a array).
		//! NB. Array starts at 1 not 0 in lua.
		template <typename T>
		int Get(int nIdx, T& v)
		{
			if(!IsReferd())
			{
				return 0;
			}

			lua_State* pLua = m_ref.pLua;

			CHECK_LUA_STACK(pLua);

			if(!PrepareStack())
			{
				return 0;
			}

			lua_rawgeti(pLua, -1, nIdx);

			return TFmLua<typename TArgumentType<T>::value_type>::Make(pLua, -1, v);
		}

		//! Set a array value.
		template <typename T>
		int Set(int nIdx, T v)
		{
			if(!IsReferd())
			{
				return 0;
			}

			lua_State* pLua = m_ref.pLua;

			CHECK_LUA_STACK(pLua);

			if(!PrepareStack())
			{
				return 0;
			}

			if(!TToLua<typename TArgumentType<T>::value_type>::Make(v, pLua))
			{
				return 0;
			}

			lua_rawseti(pLua, -2, nIdx);

			return 1;
		}

		int Set(int nIdx, CScriptTable& tbl)
		{
			return Set<CScriptTable&>(nIdx, tbl);
		}

		int Set(int nIdx, CScriptUserData& ud)
		{
			return Set<CScriptUserData&>(nIdx, ud);
		}

		int Set(int nIdx, const std::string& str)
		{
			return Set<const std::string&>(nIdx, str);
		}

		int Get(const char* pszName, CScriptTable& tbl);

		int Get(const char* pszName, CScriptUserData& ud);

		int Get(int nIdx, CScriptTable& tbl);

		int Get(int nIdx, CScriptUserData& ud);

#define ARGUMENT_TYPE(arg) typename TArgumentType<arg>::argument_type

#define VALUE_TYPE(arg)    typename TArgumentType<arg>::value_type

#define MEM_FUNC_PREWORK() \
	InvlRetVal(r);\
	if(!IsReferd())\
		{\
		return 0;\
		}\
		lua_State* pLua = m_ref.pLua;\
		CHECK_LUA_STACK(pLua);\
		if(!PrepareStack())\
		{\
		return 0;\
		}\
		lua_getfield(pLua, -1, pszName);\
		if(!lua_isfunction(pLua, -1))\
		{\
		return 0;\
		}

#define MAKE_THIS()\
	lua_pushvalue(pLua, -2)

#define MAKE_ARG(type, vale)\
	if(!TToLua<VALUE_TYPE(type)>::Make((vale), pLua))\
		{\
		return 0;\
		}

#define MAKE_RET()\
	int __nFmRet = TFmLua<R>::Make(pLua, -1, r)

#define MEM_FUNC_POSWORK()\
	return PostFmLua(__nFmRet, r);

		//! Call a member function of this ad type.
		//! Member function means the first argument is this object(table or userdata)
		//! \return 0-fail
		template <typename R>
		int CallMemFunc(const char* pszName, R& r)
		{
			MEM_FUNC_PREWORK();

			MAKE_THIS();

			if(lua_pcall(pLua, 1, 1, 0) != LUA_OK)	// Error msg here
			{
				LOG_MESSAGE("[ERROR] Call MemFunc[%s] : %s", pszName, lua_tostring(pLua, -1));
				return 0;
			}

			MAKE_RET();

			MEM_FUNC_POSWORK();
		}

		//! One argument version, See the 0 argument version.
		template <typename A0, typename R>
		int CallMemFunc(const char* pszName, ARGUMENT_TYPE(A0) a0, R& r)
		{
			MEM_FUNC_PREWORK();

			MAKE_THIS();
			MAKE_ARG(A0, a0);

			if(lua_pcall(pLua, 2, 1, 0) != LUA_OK)	// Error msg here
			{
				LOG_MESSAGE("[ERROR] Call Func[%s] : %s", pszName, lua_tostring(pLua, -1));
				return 0;
			}

			MAKE_RET();

			MEM_FUNC_POSWORK();
		}

		//! Two arguments version, see 0 argument version.
		template <typename A0, typename A1, typename R>
		int CallMemFunc(const char* pszName, ARGUMENT_TYPE(A0) a0, ARGUMENT_TYPE(A1) a1, R& r)
		{
			MEM_FUNC_PREWORK();

			MAKE_THIS();
			MAKE_ARG(A0, a0);
			MAKE_ARG(A1, a1);

			if(lua_pcall(pLua, 3, 1, 0) != LUA_OK)	// Error msg here
			{
				LOG_MESSAGE("[ERROR] Call MemFunc[%s] : %s", pszName, lua_tostring(pLua, -1));
				return 0;
			}

			MAKE_RET();

			MEM_FUNC_POSWORK();
		}
#undef ARGUMENT_TYPE
#undef VALUE_TYPE
#undef MEM_FUNC_PREWORK
#undef MAKE_THIS
#undef MAKE_ARG
#undef MAKE_RET
#undef MEM_FUNC_POSWORK
	
		lua_State* GetHandle() const
		{
			return m_ref.pLua;
		}

		int PrepareStack();

	protected:

		template <typename T>
		int PostFmLua(int nFmLuaRet, T)
		{
			return nFmLuaRet;
		}

		int PostFmLua(int nFmLuaRet, CScriptTable& tbl);

		int PostFmLua(int nFmLuaRet, CScriptUserData& ud);

		template <typename T>
		void InvlRetVal(T& )
		{

		}

		void InvlRetVal(CScriptTable& tbl);

		void InvlRetVal(CScriptUserData& ud);

		SRefNode		m_ref;

		friend class CScriptHandle;
		friend class CScriptUserData;
	};

	//! Encapsulate a lua table.
	class CScriptTable : public IScriptADBase
	{
	public:
		CScriptTable()
		{

		}

		~CScriptTable()
		{
			
		}

#define ARGUMENT_TYPE(arg) typename TArgumentType<arg>::argument_type
#define VALUE_TYPE(arg)    typename TArgumentType<arg>::value_type

#define FUNC_PREWORK() \
		InvlRetVal(r);\
		if(!IsReferd())\
		{\
			return 0;\
		}\
		lua_State* pLua = m_ref.pLua;\
		CHECK_LUA_STACK(pLua);\
		if(!PrepareStack())\
		{\
			return 0;\
		}\
		lua_getfield(pLua, -1, pszName);\
		if(!lua_isfunction(pLua, -1))\
		{\
		return 0;\
		}

#define MAKE_ARG(type, vale)\
	if(!TToLua<VALUE_TYPE(type)>::Make((vale), pLua))\
		{\
		return 0;\
		}

#define MAKE_RET()\
	int __nFmRet = TFmLua<R>::Make(pLua, -1, r)

#define FUNC_POSWORK()\
	return PostFmLua(__nFmRet, r);

		template <typename R>
		int CallFunc(const char* pszName, R& r)
		{
			FUNC_PREWORK();

			if(lua_pcall(pLua, 0, 1, 0) != LUA_OK)	// Error msg here
			{
				LOG_MESSAGE("[ERROR] Call Func[%s] : %s", pszName, lua_tostring(pLua, -1));
				return 0;
			}

			MAKE_RET();

			FUNC_POSWORK();
		}

		template <typename A0, typename R>
		int CallFunc(const char* pszName, ARGUMENT_TYPE(A0) a0, R& r)
		{
			FUNC_PREWORK();

			MAKE_ARG(A0, a0);

			if(lua_pcall(pLua, 1, 1, 0) != LUA_OK)	// Error msg here
			{
				LOG_MESSAGE("[ERROR] Call Func[%s] : %s", pszName, lua_tostring(pLua, -1));
				return 0;
			}

			MAKE_RET();
			FUNC_POSWORK();
		}

		template <typename A0, typename A1, typename R>
		int CallFunc(const char* pszName, ARGUMENT_TYPE(A0) a0, ARGUMENT_TYPE(A1) a1, R& r)
		{
			FUNC_PREWORK();

			MAKE_ARG(A0, a0);
			MAKE_ARG(A1, a1);

			if(lua_pcall(pLua, 2, 1, 0) != LUA_OK)	// Error msg here
			{
				LOG_MESSAGE("[ERROR] Call Func[%s] : %s", pszName, lua_tostring(pLua, -1));
				return 0;
			}

			MAKE_RET();

			FUNC_POSWORK();
		}

#undef ARGUMENT_TYPE
#undef VALUE_TYPE
#undef FUNC_PREWORK
#undef MAKE_THIS
#undef MAKE_ARG
#undef MAKE_RET
#undef FUNC_POSWORK

		template <typename A> friend struct TFmLua; 
		template <typename A> friend struct TToLua;
		friend class CScriptHandle;
		friend class CScriptUserData;
	};

	//! Encapsulate a lua userdata.
	class CScriptUserData : public IScriptADBase
	{
	public:
		CScriptUserData()
		{

		}

		~CScriptUserData()
		{

		}

		template <typename A> friend struct TFmLua;
		template <typename A> friend struct TToLua;
		friend class CScriptHandle;
		friend class CScriptTable;
	};

	//! Utilities
	template <>
	struct TFmLua<CScriptTable>
	{
		static int Make(lua_State* pL, int nIdx, CScriptTable& tbl);
	};

	template <>
	struct TFmLua<CScriptUserData>
	{
		static int Make(lua_State* pL, int nIdx, CScriptUserData& ud);
	};

	template <>
	struct TToLua<CScriptTable>
	{
		static int Make(CScriptTable& tbl, lua_State* pL);
	};

	template <>
	struct TToLua<CScriptUserData>
	{
		static int Make(CScriptUserData& ud, lua_State* pL);
	};
};

#endif
