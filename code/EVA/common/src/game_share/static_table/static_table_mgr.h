#ifndef GAME_SHARED_STATIC_TABLE_MGR
#define GAME_SHARED_STATIC_TABLE_MGR

#include <nel/misc/types_nl.h>
#include <nel/misc/singleton.h>
#include <nel/misc/random.h>
#include <nel/misc/sstring.h>
#include <game_share/game_def.h>
#include "static_record.h"
#include "static_tabel_struct.h"

#define GET_FIELD( __record_ptr, __field_type, __attrib )\
    __field_type* p##__field_type = dynamic_cast<__field_type*>(__record_ptr->getAttribute(__attrib));\
    if( p##__field_type!=NULL )


typedef std::map< uint,StaticData::RecordBase* >                                TStaticData;


class CStaticTableMgr : public NLMISC::CSingleton<CStaticTableMgr>
{
public:

    void init();
    
    GameConfig&                      GetGameConfig()            { return m_GameConfig; }



    void    ChangeVersion() { ++m_Version; }
    uint32  GetVersion()    { return m_Version; }


private: 

    void addToRecord( ATTRIB_TYPE::TAttribType field_attrib, NLMISC::CSString field_type, NLMISC::CSString field_val, NLMISC::CSString field_save, StaticData::RecordBase* pRecord );

    bool load_game_config();
    bool load_config( std::string xml_name, TStaticData& static_data );


    void clear_config(TStaticData& static_data);

    NLMISC::CRandom    m_random;
    GameConfig         m_GameConfig;

    enum StaticTableEnum
    {
        MAX_LEVEL   = 1024,
        MAX_INT32   = 0x7fffffff,
    };
    

    uint32 m_Version;
};


#define  StaticTableMgr  CStaticTableMgr::instance()

#define  GetFormula    StaticTableMgr.GetGameConfig().formula

#endif // GAME_SHARED_STATIC_TABLE_MGR
/* End of static_table.h */

