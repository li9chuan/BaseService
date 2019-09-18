#ifndef SERVER_SHARED_CLIENT_MSG_DESC_H
#define SERVER_SHARED_CLIENT_MSG_DESC_H

#include <nel/misc/singleton.h>
#include <nel/misc/sstring.h>
#include <nel/misc/i_xml.h>
#include "game_def.h"
#include "msg_leaf.h"

namespace google {
    namespace protobuf {
        class Message;
        class DynamicMessageFactory;
        namespace compiler {
            class Importer;
            class DiskSourceTree;
        }
    }
}

class CMsgDesc : public NLMISC::CSingleton<CMsgDesc>
{
public:

    CMsgDesc();
    ~CMsgDesc();

    void LoadMsgXml();

    MsgLeaf*  GetMsgLeaf( std::string msg_name );

    google::protobuf::Message* CreateMessage(const std::string& typeName);
//    TCallbackItem* GetTCPCallbackItem();

private:

    typedef std::map<std::string, MsgLeaf>  TMsgDesc;

    TMsgDesc                                        m_MsgDesc;
    google::protobuf::compiler::DiskSourceTree*     m_DiskSourceTree;
    google::protobuf::compiler::Importer*           m_Importer;  
    google::protobuf::DynamicMessageFactory*        m_Factory;
};

#define  MsgDesc  CMsgDesc::instance()

#endif
