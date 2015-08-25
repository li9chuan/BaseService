#include "client_msg_desc.h"
#include <nel/misc/file.h>

#include <google/protobuf/descriptor.h>
#include <nel/net/callback_net_base.h>

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

            if ( rootPtr )
            {
                m_MsgDescTCP.clear();
                m_MsgDescUDP.clear();

                xmlNodePtr record=xml.getFirstChildNode (rootPtr, "leaf");

                for ( ; record; record=xml.getNextChildNode (record, "leaf") )
                {
                    MsgLeaf  msg_leaf;

                    msg_leaf.msgname        = xml.getStringProperty(record, "name", "");
                    msg_leaf.description    = xml.getStringProperty(record, "description", "");

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
                            descriptor = google::protobuf::DescriptorPool::generated_pool()->FindMessageTypeByName(vct_str[i]);

                            if (descriptor)
                            {
                                msg_leaf.format.push_back(MSG_FORMAT::ProtoMsg);
                                msg_leaf.format_msg.push_back(vct_str[i]);
                            }
                            else
                            {
                                nlassert(0);
                            }
                        }
                        else
                        {
                            msg_leaf.format.push_back(msg_format);
                        }
                    }

                    ///    根据使用协议插入到相应map
                    const CSString protocol = xml.getStringProperty(record, "protocol", "");

                    vct_str.clear();
                    protocol.splitBySeparator(' ', vct_str);
                    std::pair< TMsgDesc::iterator, bool > res;

                    for ( uint i=0; i<vct_str.size(); ++i )
                    {
                        if ( vct_str[i]=="TCP" )
                        {
                            res = m_MsgDescTCP.insert( make_pair(msg_leaf.msgname, msg_leaf) );
                            nlassert(res.second);
                        }
                        else if ( vct_str[i]=="UDP" )
                        {
                            res = m_MsgDescUDP.insert( make_pair(msg_leaf.msgname, msg_leaf) );
                            nlassert(res.second);
                        }
                        else
                        {
                            nlassert(0);
                        }
                    }

                    ///  默认使用UDP协议 - 如果没配置
                    if ( vct_str.size()==0 )
                    {
                        res = m_MsgDescUDP.insert( make_pair(msg_leaf.msgname, msg_leaf) );
                        nlassert(res.second);
                    }
                }
                res = true;
            }
        }
        file.close ();
    }

    nlassert(res);
}

MsgLeaf* CMsgDesc::GetTCPMsgLeaf( std::string msg_name )
{
    MsgLeaf* pMsgLeaf = NULL;
    TMsgDesc::iterator iter = m_MsgDescTCP.find(msg_name);

    if ( iter != m_MsgDescTCP.end() )
    {
        pMsgLeaf = &iter->second;
    }

    return pMsgLeaf;
}

MsgLeaf* CMsgDesc::GetUDPMsgLeaf( std::string msg_name )
{
    MsgLeaf* pMsgLeaf = NULL;
    TMsgDesc::iterator iter = m_MsgDescUDP.find(msg_name);

    if ( iter != m_MsgDescUDP.end() )
    {
        pMsgLeaf = &iter->second;
    }

    return pMsgLeaf;
}



//TCallbackItem* CMsgDesc::GetTCPCallbackItem()
//{
//    return NULL;
//}
