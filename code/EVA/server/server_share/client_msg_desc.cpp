#include "client_msg_desc.h"
#include <nel/misc/file.h>
#include <nel/misc/i_xml.h>
#include <nel/net/callback_net_base.h>

#include <google/protobuf/descriptor.h>  
#include <google/protobuf/descriptor.pb.h>  
#include <google/protobuf/dynamic_message.h>  
#include <google/protobuf/compiler/importer.h>  

using namespace std;
using namespace NLMISC;

void CMsgDesc::LoadMsgXml()
{
    bool res = false;
    string path = NLMISC::CPath::lookup("msg.xml");
    CIFile file;

    if (file.open (path))
    {
        CIXml xml;
        if (xml.init (file))
        {
            xmlNodePtr rootPtr = xml.getRootNode();

            if ( rootPtr != NULL )
            {
                xmlNodePtr proto_desc   = xml.getFirstChildNode (rootPtr, "proto_src");
                std::string proto_path  = xml.getStringProperty(proto_desc, "path", "");

                if ( !proto_path.empty() )
                {
                    m_DiskSourceTree->MapPath("",proto_path);
                }

                xmlNodePtr proto_file   = xml.getFirstChildNode (proto_desc, "file");

                for ( ; proto_file; proto_file=xml.getNextChildNode (proto_file, "file") )
                {
                    std::string proto_name  = xml.getStringProperty(proto_file, "name", "");
                    m_Importer->Import(proto_name);
                }
            }

            if ( rootPtr != NULL )
            {
                m_MsgDesc.clear();
                xmlNodePtr record=xml.getFirstChildNode (rootPtr, "leaf");

                for ( ; record; record=xml.getNextChildNode (record, "leaf") )
                {
                    MsgLeaf  msg_leaf;

                    msg_leaf.msgname        = xml.getStringProperty(record, "name", "");
                    msg_leaf.description    = xml.getStringProperty(record, "description", "");
                    msg_leaf.is_log_event   = xml.getIntProperty(record, "log_event", 1) > 0 ? true: false;

                    ///  解析转发到哪个服务器
                    const CSString sendto   = xml.getStringProperty(record, "sendto", "");

                    CVectorSString  vct_str;
                    sendto.splitBySeparator(' ', vct_str);
                    nlassert(vct_str.size()>0);

                    for ( uint i=0; i<vct_str.size(); ++i )
                    {
                        msg_leaf.sendto.push_back(vct_str[i]);
                    }

                    /// 解析消息格式
                    const CSString format = xml.getStringProperty(record, "format", "");

                    vct_str.clear();
                    format.splitBySeparator(' ', vct_str);

                    for ( uint i=0; i<vct_str.size(); ++i )
                    {
                        MSG_FORMAT::TMsgFormat  msg_format = MSG_FORMAT::UNKNOWN;

                        msg_format = MSG_FORMAT::toEnum(vct_str[i]);

                        if ( msg_format==MSG_FORMAT::UNKNOWN )
                        {
                            const google::protobuf::Descriptor* descriptor = NULL;
                            descriptor = m_Importer->pool()->FindMessageTypeByName(vct_str[i]);

                            if (descriptor)
                            {
                                msg_leaf.format.push_back(MSG_FORMAT::ProtoMsg);
                                msg_leaf.format_msg.push_back(vct_str[i]);
                            }
                            else
                            {
                                nlwarning("not define %s", vct_str[i].c_str());
                            }
                        }
                        else
                        {
                            msg_leaf.format.push_back(msg_format);
                        }
                    }

                    ///    保存
                    std::pair< TMsgDesc::iterator, bool > res;
                    res = m_MsgDesc.insert( make_pair(msg_leaf.msgname, msg_leaf) );
                    nlassert(res.second);
                }
                res = true;
            }
        }
        file.close ();
    }

    nlassert(res);
}

MsgLeaf* CMsgDesc::GetMsgLeaf( std::string msg_name )
{
    MsgLeaf* pMsgLeaf = NULL;
    TMsgDesc::iterator iter = m_MsgDesc.find(msg_name);

    if ( iter != m_MsgDesc.end() )
    {
        pMsgLeaf = &iter->second;
    }

    return pMsgLeaf;
}

CMsgDesc::CMsgDesc()
{
    m_DiskSourceTree    = new google::protobuf::compiler::DiskSourceTree();
    m_Importer          = new google::protobuf::compiler::Importer(m_DiskSourceTree, NULL);
    m_Factory           = new google::protobuf::DynamicMessageFactory(m_Importer->pool());

}

CMsgDesc::~CMsgDesc()
{
    SAFE_DELETE(m_Factory);
    SAFE_DELETE(m_Importer);
    SAFE_DELETE(m_DiskSourceTree);
}

google::protobuf::Message* CMsgDesc::CreateMessage( const std::string& typeName )
{
    google::protobuf::Message* pMsg = NULL;
    const google::protobuf::Descriptor* pDescriptor = m_Importer->pool()->FindMessageTypeByName(typeName);

    if (pDescriptor)
    {
        const google::protobuf::Message* pPrototype = m_Factory->GetPrototype(pDescriptor);

        if (pPrototype)
        {
            pMsg = pPrototype->New();
        }
    }
    return pMsg;
}


//TCallbackItem* CMsgDesc::GetTCPCallbackItem()
//{
//    return NULL;
//}
