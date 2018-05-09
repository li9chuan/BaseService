#ifndef SERVER_SHARD_SCRIPT_BASE_H
#define SERVER_SHARD_SCRIPT_BASE_H

#include "Public.h"

namespace bin
{
	static const char* SCRIPT_OWN_OBJECTS	= "__bin_own_objects";
	static const char* SCRIPT_USE_OBJECTS   = "__bin_use_objects";
	static const char* SCRIPT_REFS		= "__bin_refs";
	static const char* SCRIPT_TYPES		= "__bin_types";

	//extern void LOG_MESSAGE(const char* pszFmt, ...);

	class INonCopyable 
	{
	public:
		INonCopyable(){}

		INonCopyable(INonCopyable&);
		INonCopyable& operator = (INonCopyable&);
	};

	struct SCheckLuaStack
	{
		lua_State*		pLua;
		int				nTop;
		SCheckLuaStack(lua_State* pL)
			: pLua(pL)
			, nTop(-1)
		{
			if(pLua)
			{
				nTop = lua_gettop(pLua);
			}
		}

		~SCheckLuaStack()
		{
			if(pLua)
			{
				lua_settop(pLua, nTop);
				pLua = NULL;
				nTop = -1;
			}
		}
	};

#define CHECK_LUA_STACK(l)	SCheckLuaStack __checkStack(l)

	struct SGuardLuaGC
	{
		lua_State*		pLua;
		SGuardLuaGC(lua_State* pL)
			: pLua(pL)
		{
			if(pLua)
			{
				lua_gc(pLua, LUA_GCSTOP, 0);
			}
		}

		~SGuardLuaGC()
		{
			if(pLua)
			{
				lua_gc(pLua, LUA_GCRESTART, 0);

				pLua = NULL;
			}
		}
	};
#define GUARD_LUA_GC(l)	SGuardLuaGC __guardGC(l)

	struct SVoidType
	{

	};

	inline SVoidType __ret_void_tag() { return SVoidType();}
#define RET_VOID bin::__ret_void_tag()

	template <typename R>
	struct TRetType
	{
		typedef R return_type;
	};

	template <>
	struct TRetType<void>
	{
		typedef SVoidType return_type;
	};

	template <typename R>
	struct TToLua;

	template <>
	struct TToLua<SVoidType>
	{
		static int Make(SVoidType v, lua_State* pL)
		{
			lua_pushnil(pL);

			return 1;
		}
	};

	template <>
	struct TToLua<bool>
	{
		static int Make(bool a, lua_State* pL)
		{
			lua_pushboolean(pL, a);

			return 1;
		}
	};

	template <>
	struct TToLua<int>
	{
		static int Make(int a, lua_State* pL)
		{
			lua_pushinteger(pL, (LUA_INTEGER)a);

			return 1;
		}
	};

	template <>
	struct TToLua<double>
	{
		static int Make(double a, lua_State* pL)
		{
			lua_pushnumber(pL, a);

			return 1;
		}
	};

	template <>
	struct TToLua<long long>
	{
		static int Make(long long a, lua_State* pL)
		{
			lua_pushinteger(pL, (LUA_INTEGER)a);

			return 1;
		}
	};

	template <>
	struct TToLua<char*>
	{
		static int Make(char* a, lua_State* pL)
		{
			lua_pushstring(pL, a);

			return 1;
		}
	};

	template <>
	struct TToLua<const char*>
	{
		static int Make(const char* a, lua_State* pL)
		{
			lua_pushstring(pL, a);

			return 1;
		}
	};

	template <>
	struct TToLua<std::string>
	{
		static int Make(const std::string& a, lua_State* pL)
		{
			lua_pushstring(pL, a.c_str());

			return 1;
		}
	};

	template <typename O>	// If a object, must be a proxy
	struct TToLua<O*>	
	{
		static int Make(O* o, lua_State* pL)
		{
			int nRet = 0;
			if(o)
			{
				if(o->GetScriptObject().IsExported())
				{
					nRet = o->GetScriptObject().GetUserData(pL);
				}
				else
				{
					nRet = ScriptExporterManager().AddScriptObject(o, pL);
				}
			}

			if(!nRet)
			{
				lua_pushnil(pL);

				nRet = 1;
			}

			return nRet;
		}
	};

	template <typename A>
	struct TFmLua;

	template <>
	struct TFmLua<SVoidType>
	{
		static int Make(lua_State* pL, int nIdx, SVoidType& v)
		{
			return 1;
		}
	};

	template <>
	struct TFmLua<bool>
	{
		static int Make(lua_State* pL, int nIdx, bool& b)
		{
			if(!lua_isboolean(pL, nIdx))
			{
				return 0;
			}

			b = lua_toboolean(pL, nIdx)!=0;

			return 1;
		}
	};
	
	template <>
	struct TFmLua<int>	
	{
		static int Make(lua_State* pL, int nIdx, int& a)
		{
			if(!lua_isnumber(pL, nIdx))
			{
				return 0;
			}

			a = (int)lua_tointeger(pL, nIdx);
			
			return 1;
		}
	};

	template <>
	struct TFmLua<long long>	
	{
		static int Make(lua_State* pL, int nIdx, long long& a)
		{
			if(!lua_isnumber(pL, nIdx))
			{
				return 0;
			}

			a = (long long)lua_tointeger(pL, nIdx);

			return 1;
		}
	};

	template <>
	struct TFmLua<double>
	{
		static int Make(lua_State* pL, int nIdx, double& d)
		{
			if(!lua_isnumber(pL, nIdx))
			{
				return 0;
			}

			d = lua_tonumber(pL, nIdx);

			return 1;
		}
	};

	template <>
	struct TFmLua<std::string>
	{
		static int Make(lua_State* pL, int nIdx, std::string& str)
		{
			if(!lua_isstring(pL, nIdx))
			{
				return 0;
			}

			str = lua_tostring(pL, nIdx);

			return 1;
		}
	};

	template <>
	struct TFmLua<const char*>
	{
		static int Make(lua_State* pL, int nIdx, const char*& cstr)
		{
			if(!lua_isstring(pL, nIdx))
			{
				return 0;
			}

			cstr = lua_tostring(pL, nIdx);

			return 1;
		}
	};

	// Partial specialization Not support by function, use class instead
	template <typename O>	// If a object, must be a proxy
	struct TFmLua<O*>	
	{
		static int Make(lua_State* pL, int nIdx, O*& o)
		{
			o = NULL;

			if(!lua_isuserdata(pL, nIdx))
			{
				return 0;
			}
			
			void* pUd = lua_touserdata(pL, nIdx);
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

			o = reinterpret_cast<O*>(pProxy->objRef.pObject->m_pThis);

			return 1;
		}
	};

	template <typename T>
	struct TArgumentType
	{
		typedef T			value_type;
		typedef T&			argument_type;
		typedef const T&	c_argument_type;
	};

	template <typename T>
	struct TArgumentType<T&>
	{
		typedef T			value_type;
		typedef T&			argument_type;
		typedef const T&	c_argument_type;
	};

	template <typename T>
	struct TArgumentType<const T&>
	{
		typedef T			value_type;
		typedef T&			argument_type;
		typedef const T&	c_argument_type;
	};

	template <typename T>
	struct TArgumentType<T*>
	{
		typedef T*			value_type;
		typedef T*			argument_type;
		typedef T*			c_argument_type;
	};

	template <>
	struct TArgumentType<void>
	{
		typedef SVoidType	value_type;
		typedef SVoidType	argument_type;
		typedef SVoidType	c_argument_type;
	};

	template <>
	struct TArgumentType<int>
	{
		typedef int		value_type;
		typedef int		argument_type;
		typedef int		c_argument_type;
	};

	template <>
	struct TArgumentType<bool>
	{
		typedef bool		value_type;
		typedef bool		argument_type;
		typedef bool		c_argument_type;
	};

	template <>
	struct TArgumentType<double>
	{
		typedef double		value_type;
		typedef double		argument_type;
		typedef double		c_argument_type;
	};

	template <>
	struct TArgumentType<long long>
	{
		typedef long long		value_type;
		typedef long long		argument_type;
		typedef long long		c_argument_type;
	};

	template <typename F, typename T>
	struct TLuaFuncCaller;

	template <typename F, typename R>
	struct TLuaFuncCaller<F, R()>
	{
		F*				pFun;
		lua_State*		pLua;

		TLuaFuncCaller(lua_State* pL, F* pF)
			: pLua(pL), pFun(pF)
		{
			assert(pLua && pFun);
		}

		int MakeArgs()
		{
			return 1;
		}

		int MakeCArgs()
		{
			TFmLua<typename F::class_type*>::Make(pLua, 1, pFun->obj);
			if(!pFun->obj)
			{
				return 0;
			}

			return 1;
		}

		int Call()
		{
			return pFun->Exec();
		}

		int MakeRet()
		{
			return TToLua<typename F::return_type>::Make(pFun->r, pLua);
		}
	};

	template <typename F, typename R, typename A>
	struct TLuaFuncCaller<F, R(A)>
	{
		typedef typename TArgumentType<A>::value_type Arg; 
		Arg								a;
		F*								pFun;
		lua_State*						pLua;
		
		TLuaFuncCaller(lua_State* pL, F* pF)
			: pLua(pL), pFun(pF)
		{
			assert(pLua && pFun);
		}

		int MakeArgs()
		{
			TFmLua<Arg>::Make(pLua, 1, a);
			return 1;
		}

		int MakeCArgs()
		{
			TFmLua<typename F::class_type*>::Make(pLua, 1, pFun->obj);
			if(!pFun->obj)
			{
				return 0;
			}

			TFmLua<Arg>::Make(pLua, 2, a);
			return 1;
		}

		int Call()
		{
			return pFun->Exec(a);
		}

		int MakeRet()
		{
			return TToLua<typename F::return_type>::Make(pFun->r, pLua);
		}
	};

	template <typename F, typename R, typename A0, typename A1>
	struct TLuaFuncCaller<F, R(A0, A1)>
	{
		typedef typename TArgumentType<A0>::value_type Arg0;
		typedef typename TArgumentType<A1>::value_type Arg1;

		Arg0							a0;
		Arg1							a1;
		F*								pFun;
		lua_State*						pLua;

		TLuaFuncCaller(lua_State* pL, F* pF)
			: pLua(pL), pFun(pF)
		{
			assert(pLua && pFun);
		}

		int MakeArgs()
		{
			TFmLua<Arg0>::Make(pLua, 1, a0);
			TFmLua<Arg1>::Make(pLua, 2, a1);

			return 1;
		}

		int MakeCArgs()
		{
			TFmLua<typename F::class_type*>::Make(pLua, 1, pFun->obj);
			if(!pFun->obj)
			{
				return 0;
			}
			
			TFmLua<Arg0>::Make(pLua, 2, a0);
			TFmLua<Arg1>::Make(pLua, 3, a1);
			
			return 1;
		}

		int Call()
		{
			return pFun->Exec(a0, a1);
		}

		int MakeRet()
		{
			return TToLua<typename F::return_type>::Make(pFun->r, pLua);
		}
	};

//! Define a script module(a lua table)
//! \param modName the name of table
#define BEGIN_SCRIPT_MODULE(modName)\
namespace __module_ ## modName\
{\
	namespace __module = __module_ ## modName;\
	static const char* __pszName = #modName;\
	struct SModuleFunction_ ## modName;\
	typedef SModuleFunction_ ## modName __mf_LinkNode;\
	static __mf_LinkNode* __pHead = NULL;\
	struct SModuleFunction_ ## modName : ModuleFunctionLinkNode\
	{\
		SModuleFunction_ ## modName(const char* pN, lua_CFunction pF)\
		{\
			pszName			  = pN;\
			pFunc			  = pF;\
			pNxt	          = __module::__pHead;\
			__module::__pHead = this;\
		}\
	};
	
//! Define a module function(a function of the lua tabel)
//! \param name name of function
//! \param ret return type
//! \param args the arguments
//! for example : 
//! DEFINE_MODULE_FUNCTION(func, std::string, (int a0, int a1))
//! {
//!     int v = a0+a1;				// a0 and a1 comes from lua
//!		r = "this is return value";	// The return value to lua
//!     return 1;					// return 1 means succeed, not the return value to lua
//! }
#define DEFINE_MODULE_FUNCTION(name, ret, args)\
	struct __mf_Impl_##name\
	{\
		typedef TRetType< ret >::return_type return_type;\
		return_type	 r;\
		bin::CScriptHandle lua;\
		int Exec ##  args;\
	};\
	static int __mf_lua_ ## name(lua_State* pL)\
	{\
		__mf_Impl_##name __mf_impl;\
		__mf_impl.lua.Init(pL);\
		TLuaFuncCaller<__mf_Impl_##name, ret##args> caller(pL, &__mf_impl);\
		if(!caller.MakeArgs() || !caller.Call() || !caller.MakeRet())\
		{\
			return 0;\
		}\
		return 1;\
	}\
	static __mf_LinkNode __mf_node_ ## name(#name, &__mf_lua_ ## name);\
	int __mf_Impl_##name::Exec ## args

//! End the definition of a module.
#define END_SCRIPT_MODULE()\
	static CModuleExporter __exporter(__module::__pszName, (ModuleFunctionLinkNode**)&__module::__pHead);\
};

//! Define a script class.
//! \param clsName the name of this class.
//! \param cppName the c++ class name
#define BEGIN_SCRIPT_CLASS(clsName, cppName)\
namespace __class_ ## clsName\
{\
	namespace __class = __class_ ## clsName;\
	typedef cppName	class_type;\
	const char* __pszName = #clsName;\
	const char* __pszSuper = NULL;\
    struct SClassFunction_ ## clsName;\
    typedef SClassFunction_ ## clsName __cf_LinkNode;\
    static __cf_LinkNode* __pHead = NULL;\
    struct SClassFunction_ ## clsName : ClassFunctionLinkNode\
    {\
        SClassFunction_ ## clsName(const char* pN, lua_CFunction pF)\
        {\
            pszName			  = pN;\
            pFunc			  = pF;\
            pNxt	          = __class::__pHead;\
            __class::__pHead = this;\
        }\
    };

//! Indicates the super class name.
//! \param clsName the lua class name.
//! \param cppName the c++ class name.
#define SUPER_CLASS(clsName, cppName)\
	struct SSetSuper_ ## clsName\
	{\
		SSetSuper_ ## clsName()\
		{\
			__class::__pszSuper = #clsName;\
		}\
	};\
	SSetSuper_ ## clsName __setSuper;

//! Define a class member function. you can access the c++ object by obj and return the value by set value to r.
//! If the userdata is invalid, obj is null.
#define DEFINE_CLASS_FUNCTION(name, ret, args)\
	struct __cf_Impl_##name\
	{\
	typedef TRetType< ret >::return_type return_type;\
	typedef __class::class_type			 class_type;\
	class_type*  obj;\
	return_type	 r;\
	int Exec ## args;\
	};\
	static int __cf_lua_ ## name(lua_State* pL)\
	{\
		__cf_Impl_##name __cf_impl;\
		TLuaFuncCaller<__cf_Impl_##name, ret##args> caller(pL, &__cf_impl);\
		if(!caller.MakeCArgs() || !caller.Call() || !caller.MakeRet())\
		{\
			return 0;\
		}\
		return 1;\
	}\
    static __cf_LinkNode __cf_node_ ## name(#name, &__cf_lua_ ## name);\
	int __cf_Impl_##name::Exec ## args

#define DEFINE_STATIC_FUNCTION(name, ret, args)\
	struct __cf_Impl_##name\
	{\
	typedef TRetType< ret >::return_type return_type;\
	typedef __class::class_type			 class_type;\
	return_type	 r;\
	int Exec ## args;\
	};\
	static int __cf_lua_ ## name(lua_State* pL)\
	{\
		__cf_Impl_##name __cf_impl;\
		TLuaFuncCaller<__cf_Impl_##name, ret##args> caller(pL, &__cf_impl);\
		if(!caller.MakeArgs() || !caller.Call() || !caller.MakeRet())\
		{\
			return 0;\
		}\
		return 1;\
	}\
    static __cf_LinkNode __cf_node_ ## name(#name, &__cf_lua_ ## name);\
	int __cf_Impl_##name::Exec ## args

//! End the definition of a class.
#define END_SCRIPT_CLASS()\
	static CClassExporter __exporter(__class::__pszSuper, __class::__pszName, (ClassFunctionLinkNode**)&__class::__pHead);\
    struct __cf_Record\
    {\
        __cf_Record()\
        {\
            __class::class_type::ClassExporter() = &__class::__exporter;\
        }\
    };\
    static __cf_Record __recorder;\
};

//! Declare a class will be a script class. this declaration must be the base class, any sub-class of the class must be taged with DECLARE_SCRIPT_SUB_CLASS.
#define DECLARE_SCRIPT_CLASS()\
public:\
	bin::SScriptObject           __m_scrObj;\
	const bin::SScriptObject& GetScriptObject() const\
	{\
		return __m_scrObj;\
	}\
	bin::SScriptObject& GetScriptObject()\
	{\
		return __m_scrObj;\
	}\
	bin::CScriptHandle& GetScriptHandle()\
	{\
		return GetScriptObject().GetScriptHandle();\
	}\
	int GetScriptUserData(bin::CScriptUserData& scriptUserData)\
	{\
		return GetScriptObject().GetUserData(scriptUserData);\
	}\
	bool IsExported()\
	{\
		return GetScriptObject().IsExported();\
	}\
	virtual bin::CClassExporter* GetExporter() const\
	{\
		return ClassExporter();\
	}\
	static bin::CClassExporter*& ClassExporter()\
    {\
        static bin::CClassExporter* s_exporter = NULL;\
        return s_exporter;\
    }\
private:

//! Taged a child class
#define DECLARE_SCRIPT_SUB_CLASS(superCls)\
	public:\
	typedef superCls Super;\
	virtual bin::CClassExporter* GetExporter() const\
	{\
		return ClassExporter();\
	}\
	static bin::CClassExporter*& ClassExporter()\
	{\
		static bin::CClassExporter* s_exporter = NULL;\
		return s_exporter;\
	}\
	private:
};

#endif
