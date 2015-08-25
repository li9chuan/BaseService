#include "static_table_mgr.h"
#include <nel/misc/file.h>
#include <nel/misc/i_xml.h>
#include <nel/net/service.h>
#include <ctime>
#include "stdpch.h"
#include "../eval_expression.h"

using namespace std;
using namespace NLMISC;
using namespace ATTRIB_TYPE;
using namespace StaticData;

#define GET_STATIC_RECORD( __static_data ) \
    StaticData::RecordBase*  pRecord = NULL;\
    TStaticData::iterator iter = __static_data.find(record_id);\
    if ( iter != __static_data.end() ){\
    pRecord = iter->second;\
    }\
    return pRecord;

/// 注意清空之前的数据，CStaticTableMgr::init()可能会调用多次。
void CStaticTableMgr::init()
{
    ChangeVersion();

    //nlassert( load_game_config() );
}

void CStaticTableMgr::addToRecord( ATTRIB_TYPE::TAttribType field_attrib, NLMISC::CSString field_type, NLMISC::CSString field_val, NLMISC::CSString field_save, StaticData::RecordBase* pRecord )
{
    FieldBase* pFieldBase = NULL;

    if ( field_type=="UpgradeItem" )
    {
        pFieldBase = new(std::nothrow) FieldNeedMeta();
    }
    else
    {
        ///   class类型的默认处理
        pFieldBase = new(std::nothrow) FieldBase();
    }

    pFieldBase->set( field_attrib, field_type, field_val );
    pRecord->addAttribute( pFieldBase, field_save );
}

bool CStaticTableMgr::load_game_config()
{
    bool res = false;
    string path = NLMISC::CPath::lookup("GameConfig.xml");
    CIFile file;
    CIXml xml;

    if (file.open (path))
    {
        if (xml.init (file))
        {
            xmlNodePtr rootPtr = xml.getRootNode();

            if ( rootPtr )
            {
                /// 各种公式
                xmlNodePtr pFormula = xml.getFirstChildNode( rootPtr , "formula" );
                m_GameConfig.formula.rmb2money              = CIXml::getStringProperty( xml.getFirstChildNode( pFormula, "rmb2money")  , "value" , "" );
                m_GameConfig.formula.rmb2soul               = CIXml::getStringProperty( xml.getFirstChildNode( pFormula, "rmb2soul")  , "value" , "" );


                ///  读取种植生长时间配置
                m_GameConfig.grow_time.clear();
                xmlNodePtr pGrowTimePtr = xml.getFirstChildNode( rootPtr , "grow_time" );
                xmlNodePtr pLeaf = xml.getFirstChildNode(pGrowTimePtr, XML_ELEMENT_NODE);
                for ( ; pLeaf!=NULL; pLeaf=xml.getNextChildNode(pLeaf,XML_ELEMENT_NODE) )
                {
                    uint32 quality = xml.getIntProperty( pLeaf , "quality" , 0 );
                    uint32 grow_time = xml.getIntProperty( pLeaf , "time" , 0 );

                    m_GameConfig.grow_time[ quality ] = grow_time;
                }

                res = true;
            }
        }

        file.close ();
    }

    return res;
}


//StaticData::RecordBase* CStaticTableMgr::GetDefense( uint32 record_id )
//{
//    GET_STATIC_RECORD( m_DefenseTools );
//}


bool CStaticTableMgr::load_config( std::string xml_name, TStaticData& static_data )
{
    bool res = false;
    string path = NLMISC::CPath::lookup(xml_name+".xml");
    CIFile file;

    if (file.open (path))
    {
        CIXml xml;
        if (xml.init (file))
        {
            xmlNodePtr rootPtr = xml.getRootNode();

            if ( rootPtr )
            {
                clear_config(static_data);

                xmlNodePtr record=xml.getFirstChildNode (rootPtr, xml_name.c_str());

                for ( ; record!=NULL; record=xml.getNextChildNode (record, xml_name.c_str()) )
                {
                    xmlNodePtr field = xml.getFirstChildNode(record,XML_ELEMENT_NODE);
                    StaticData::RecordBase*  pRecord = new(std::nothrow) StaticData::RecordBase();
                    for ( ; field!=NULL; field=xml.getNextChildNode(field,XML_ELEMENT_NODE) )
                    {
                        ATTRIB_TYPE::TAttribType   field_attrib;
                        if ( !TAttribType_Parse( string((const char *)field->name), &field_attrib ) )
                        {
                            nlwarning("TAttribType Parse Error. xmlname=%s field=%s",xml_name.c_str(),(const char *)field->name);
                            continue;
                        }

                        const CSString field_type   = xml.getStringProperty(field, "valueType", "");
                        const CSString field_val    = xml.getStringProperty(field, "value", "");
                        const CSString field_save   = xml.getStringProperty(field, "isSave", "");

                        nlassert( field_type.size()>0 );

                        if ( field_type=="int" || field_type=="string" || field_type=="bool" )
                        {
                            StaticData::FieldBase* pFieldBase = new(std::nothrow) StaticData::FieldBase();
                            pFieldBase->set( field_attrib, field_type, field_val );
                            pRecord->addAttribute( pFieldBase, field_save );
                        }
                        else
                        {
                            addToRecord( field_attrib, field_type, field_val, field_save, pRecord );
                        }
                    }
                    static_data[pRecord->getAttribute(ATTRIB_TYPE::ID)->m_int] = pRecord;
                }
                res = true;
            }
        }
        file.close ();
    }
    return res;
}

void CStaticTableMgr::clear_config( TStaticData& static_data )
{
    TStaticData::iterator iter = static_data.begin();

    while (iter!=static_data.end())
    {
        delete iter->second;
        ++iter;
    }

    static_data.clear();
}




