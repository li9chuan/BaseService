//
#include "Public.h"

//
#include "ScriptExporter.h"

//
#include "ScriptBase.h"

//
#include <fstream>

namespace bin
{
//	void LOG_MESSAGE(const char* pszFmt, ...)
//	{
//		static char s_szBuffer[1024] = {0};
//
//		va_list ls;
//		va_start(ls, pszFmt);
//
//		_vsnprintf(s_szBuffer, 1024, pszFmt, ls);
//
//		va_end(ls);
//
//		static std::ofstream ofstr;
//		if(!ofstr.is_open())
//		{
//			ofstr.open("bin_script.log");
//		}
//
//		ofstr<< s_szBuffer<< std::endl;
//#if defined _DEBUG
//		// To Console
//		std::cout<< s_szBuffer<< std::endl;
//#endif
//
//	}

	int CScriptExporterManager::ExportClass(const char* pszName, lua_State* pL, const char* pszNameSpace /*= NULL*/)
	{
		ExporterIterator pos = m_scriptExporters.find(pszName);

		if(pos != m_scriptExporters.end())
		{
			SExporterInfo info = pos->second->GetInfo();
			if(info.eET != SExporterInfo::EET_CLASS)
			{
				return 0;
			}

			CClassExporter* pClsExp = (CClassExporter*)(pos->second);
			return pClsExp->Export(pL, pszNameSpace);
		}

		return 0;
	}

	int CScriptExporterManager::ExportModule(const char* pszName, lua_State* pL)
	{
		ExporterIterator pos = m_scriptExporters.find(pszName);

		if(pos != m_scriptExporters.end())
		{
			SExporterInfo info = pos->second->GetInfo();
			if(info.eET != SExporterInfo::EET_MODULE)
			{
				return 0;
			}

			return pos->second->Export(pL);
		}

		return 0;
	}

	int CScriptExporterManager::ExportModuleTo(const char* pszName, lua_State* pL, CScriptTable& scriptTable)
	{
		assert(scriptTable.GetHandle() == pL);

		ExporterIterator pos = m_scriptExporters.find(pszName);

		if(pos != m_scriptExporters.end())
		{
			SExporterInfo info = pos->second->GetInfo();
			if(info.eET != SExporterInfo::EET_MODULE)
			{
				return 0;
			}

			CModuleExporter& exporter = (CModuleExporter&)*(pos->second);
			return exporter.ExportTo(pL, scriptTable);
		}

		return 0;
	}

	BEGIN_SCRIPT_MODULE(exporterManager)
		DEFINE_MODULE_FUNCTION(exportClass, bool, (const char* pszName, const std::string& strNameSpace))
		{
			if(strNameSpace.empty())
			{
				r = ScriptExporterManager().ExportClass(pszName, lua) != 0;
			}
			else
			{
				r = ScriptExporterManager().ExportClass(pszName, lua, strNameSpace.c_str()) != 0;
			}

			return 1;
		}

		DEFINE_MODULE_FUNCTION(exportModule, bool, (const char* pszName, CScriptTable& tbl))
		{
			if(!tbl.IsReferd())
			{
				r = ScriptExporterManager().ExportModule(pszName, lua) != 0;
			}
			else
			{
				r = ScriptExporterManager().ExportModuleTo(pszName, lua, tbl) != 0;
			}

			return 1;
		}

	END_SCRIPT_MODULE()
};
