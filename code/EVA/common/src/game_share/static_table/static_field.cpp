#include "static_field.h"
#include <nel/misc/string_common.h>

using namespace NLMISC;

namespace StaticData
{
	void FieldBase::set( ATTRIB_TYPE::TAttribType field_attrib, std::string field_type, std::string value )
	{
		m_type = field_attrib;

		if ( field_type=="int" )
		{
			fromString( value, m_int );
		}
		else if ( field_type=="string" )
		{
			m_str = value;
		}
		else if ( field_type=="bool" )
		{
			if ( toUpper(value)=="TRUE" )
			{
				m_bool = true;
			}
			else
			{
				m_bool = false;
			}
		}
        else //if ( field_type=="class" )
        {
            CVectorSString result;
            CSString cstring(value);
            cstring.splitBySeparator(',',result);

            for ( uint i=0; i<result.size(); ++i )
            {
                m_param.push_back( trim(result[i]).atoi() );
            }
        }
	}

	void FieldNeedMeta::set( ATTRIB_TYPE::TAttribType field_attrib, std::string field_type, std::string value )
	{
		m_type = field_attrib;
		CVectorSString result;
		CSString cstring(value);
		cstring.splitBySeparator(',',result);

		nlassert( result.size()==2 );

		m_metaid    = trim(result[0]).atoui();
		m_num       = trim(result[1]).atoui();
	}

}

