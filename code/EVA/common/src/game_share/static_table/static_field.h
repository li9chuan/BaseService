#ifndef GAME_SHARED_STATIC_FIELD
#define GAME_SHARED_STATIC_FIELD

#include "game_def.h"
#include <string>

namespace StaticData
{
	class FieldBase
	{
	public:
		ATTRIB_TYPE::TAttribType    m_type;
		sint64                      m_int;
		std::string                 m_str;
		bool                        m_bool;

        typedef  std::vector<sint64>    TParams;
        TParams                     m_param;
	public:

		virtual void set( ATTRIB_TYPE::TAttribType field_attrib, std::string field_type, std::string value );
	};


	/**
	*   @brief 装备升级需要的材料
	*/
	class FieldNeedMeta : public FieldBase
	{
	public:
        enum { NEED_META_MAX = 6 };
		DEF::TEMPLATE_ID  m_metaid;
		uint  m_num;

		virtual void set( ATTRIB_TYPE::TAttribType field_attrib, std::string field_type, std::string value );
	};

}

#endif /// GAME_SHARED_STATIC_FIELD

