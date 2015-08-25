#ifndef GAME_SHARED_STATIC_RECORD
#define GAME_SHARED_STATIC_RECORD

#include "static_field.h"

namespace StaticData
{
	class RecordBase
	{
	public:

		std::vector<FieldBase*>   vctAttribute;       // 属性
		std::vector<FieldBase*>   vctAttributeSave;   // 需要存盘的属性，初值

        virtual ~RecordBase()
        {
            for ( uint i=0; i<vctAttribute.size(); ++i )
            {
                delete vctAttribute[i];
            }
            vctAttribute.clear();

            for ( uint i=0; i<vctAttributeSave.size(); ++i )
            {
                delete vctAttributeSave[i];
            }
            vctAttributeSave.clear();
        }

		void addAttribute( FieldBase* field_base, std::string save )
		{
			if ( save=="true" )
			{
				vctAttributeSave.push_back(field_base);
			}
			else
			{
				vctAttribute.push_back(field_base);
			}
		}

        uint getArraySize( ATTRIB_TYPE::TAttribType attrib_type )
        {
            FieldBase* pField = getAttribute( attrib_type );
            if ( pField!=NULL )
            {
                return pField->m_param.size();
            }
            return 0;
        }

        sint64 getArray( ATTRIB_TYPE::TAttribType attrib_type, uint index, sint64 defaultValue )
        {
            FieldBase* pField = getAttribute( attrib_type );
            if ( pField!=NULL )
            {
                if ( index<pField->m_param.size() )
                {
                    return pField->m_param[index];
                }
            }
            return defaultValue;
        }

        sint64 getAttribInt( ATTRIB_TYPE::TAttribType attrib_type, sint64 defaultValue )
        {
            FieldBase* pField = getAttribute( attrib_type );
            if ( pField!=NULL )
            {
                return pField->m_int;
            }
            return defaultValue;
        }

        bool getAttribBool( ATTRIB_TYPE::TAttribType attrib_type, bool defaultValue )
        {
            FieldBase* pField = getAttribute( attrib_type );
            if ( pField!=NULL )
            {
                return pField->m_bool;
            }
            return defaultValue;
        }

        std::string getAttribStr( ATTRIB_TYPE::TAttribType attrib_type, std::string defaultValue )
        {
            FieldBase* pField = getAttribute( attrib_type );
            if ( pField!=NULL )
            {
                return pField->m_str;
            }
            return defaultValue;
        }

		FieldBase* getAttribute( ATTRIB_TYPE::TAttribType attrib_type )
		{
			FieldBase* field = NULL;
			for ( uint i=0; i<vctAttribute.size(); ++i )
			{
				if ( vctAttribute[i]->m_type == attrib_type )
				{
					field =  vctAttribute[i];
					break;
				}
			}
			return field;
		}

		bool checkAttribute( ATTRIB_TYPE::TAttribType attrib_type1, ATTRIB_TYPE::TAttribType attrib_type2 )
		{
			return checkAttribute(attrib_type1)&&checkAttribute(attrib_type2);
		}

		bool checkAttribute( ATTRIB_TYPE::TAttribType attrib_type1, ATTRIB_TYPE::TAttribType attrib_type2, ATTRIB_TYPE::TAttribType attrib_type3 )
		{
			return checkAttribute(attrib_type1,attrib_type2)&&checkAttribute(attrib_type3);
		}

		bool checkAttribute( ATTRIB_TYPE::TAttribType attrib_type1, ATTRIB_TYPE::TAttribType attrib_type2, ATTRIB_TYPE::TAttribType attrib_type3, ATTRIB_TYPE::TAttribType attrib_type4 )
		{
			return checkAttribute(attrib_type1,attrib_type2)&&checkAttribute(attrib_type3,attrib_type4);
		}

		bool checkAttribute( ATTRIB_TYPE::TAttribType attrib_type1, ATTRIB_TYPE::TAttribType attrib_type2, ATTRIB_TYPE::TAttribType attrib_type3, ATTRIB_TYPE::TAttribType attrib_type4, ATTRIB_TYPE::TAttribType attrib_type5 )
		{
			return checkAttribute(attrib_type1,attrib_type2)&&checkAttribute(attrib_type3,attrib_type4,attrib_type5);
		}

		bool checkAttribute( ATTRIB_TYPE::TAttribType attrib_type1, ATTRIB_TYPE::TAttribType attrib_type2, ATTRIB_TYPE::TAttribType attrib_type3, ATTRIB_TYPE::TAttribType attrib_type4, ATTRIB_TYPE::TAttribType attrib_type5, ATTRIB_TYPE::TAttribType attrib_type6 )
		{
			return checkAttribute(attrib_type1,attrib_type2,attrib_type3)&&checkAttribute(attrib_type4,attrib_type5,attrib_type6);
		}

    private:

        ///  check一个attrib的时候，使用getAttribute 并且判空。
        bool checkAttribute( ATTRIB_TYPE::TAttribType attrib_type )
        {
            if ( getAttribute(attrib_type)!=NULL ) { return true; }
            return false;
        }
	};
}


#endif // GAME_SHARED_STATIC_RECORD

