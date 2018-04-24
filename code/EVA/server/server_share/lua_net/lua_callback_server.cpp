#include "lua_callback_server.h"
#include "lua_message.h"
#include <server_share/lua/script_mgr.h>
#include <nel/net/callback_server_websocket.h>
#include <nel/net/callback_server_tcp.h>
#include <server_share/client_msg_desc.h>

using namespace bin;
using namespace NLNET;

CVariable<bool>	VAR_MSG_COUNT("fes", "MsgCount", "memo", false, 0, true);
CVariable<bool>	VAR_SAVE_EVENT("fes", "SaveEvent", "memo", false, 0, true);

static CLuaMessage* pLuaMsg = new CLuaMessage();
                              
void cbLuaServiceMsg ( CMessage &msgin, TSockId from, CCallbackNetBase &netbase )
{
    CLuaCallbackServer* pServer = (CLuaCallbackServer*)netbase.getUserData();
    pServer->IncReceiveMsgCount(msgin.getName());

    MsgLeaf* pMsgLeaf = MsgDesc.GetMsgLeaf( msgin.getName() );

    if ( pMsgLeaf!=NULL )
    {
        ClientData* pClient = pServer->GetClientData(from);

        if ( pClient != NULL )
        {
            pServer->ForwardingMsg( pClient, msgin, pMsgLeaf );
        }
    }
    else
    {
        CScriptTable    functbl;
        ScriptMgr.GetScriptHandle()->Get("NetWorkHandler", functbl);

        int nRet = 0;
        pLuaMsg->m_Msg.swap(msgin);

        functbl.CallFunc<lua_Integer, CLuaMessage*, int>("OnMessage", (lua_Integer)from, pLuaMsg, nRet);
    }
}

CLuaCallbackServer::CLuaCallbackServer( std::string& name, std::string& protocal, uint16 port ) : m_NetName(name)
{
    if( protocal=="ws" || protocal=="websocket" )
    {
        CCallbackServerWebSocket* pServer = new CCallbackServerWebSocket();
        pServer->setConnectionCallback( cbLuaSvrConnect, this );
        pServer->setDisconnectionCallback( cbLuaSvrDisConnect, this );
        pServer->setDefaultCallback(cbLuaServiceMsg);
        pServer->init (port);

        m_CallbackServerHandle = pServer;
    }
    else if ( protocal=="tcp" )
    {
        CCallbackServerTcp* pServer = new CCallbackServerTcp();
        pServer->setConnectionCallback( cbLuaSvrConnect, this );
        pServer->setDisconnectionCallback( cbLuaSvrDisConnect, this );
        pServer->setDefaultCallback(cbLuaServiceMsg);
        pServer->init (port);

        m_CallbackServerHandle = pServer;
    }

    m_CallbackServerHandle->setUserData(this);

    nlassert(m_CallbackServerHandle!=NULL);
    LuaNetworkMgr.RegisterNetModule( name, this );
}

CLuaCallbackServer::~CLuaCallbackServer()
{
    LuaNetworkMgr.RemoveNetModule(m_NetName);
    delete m_CallbackServerHandle; 
}

void CLuaCallbackServer::IncReceiveMsgCount( std::string msg_name )
{
    if( VAR_MSG_COUNT )
    {
        TMsgCount::iterator iter = m_ReceiveMsgCount.find(msg_name);

        if ( iter!=m_ReceiveMsgCount.end() )
        {
            iter->second += 1;
        }
        else
        {
            m_ReceiveMsgCount.insert( make_pair(msg_name, 1) );
        }
    }
}

bool CLuaCallbackServer::ForwardingMsg( ClientData* pClient, NLNET::CMessage& msgin, MsgLeaf* pMsgLeaf )
{
    google::protobuf::Message* pMessage = NULL;

    try
    {
        NLNET::CMessage  msgout( pMsgLeaf->msgname );
        std::vector<MSG_FORMAT::TMsgFormat>& format = pMsgLeaf->format;
        std::vector<std::string>& format_msg = pMsgLeaf->format_msg;
        m_SaveEventStr.clear();
        uint64  id = 0;

        for ( uint i=0,j=0; i<format.size(); ++i )
        {
            switch (format[i])
            {
            case MSG_FORMAT::UID:
                {
                    msgout.serial(pClient->uid);
                    id = pClient->uid;
                    break;
                }
            case MSG_FORMAT::PLS:
                {
                    msgout.serial(pClient->pls_sid);
                    break;
                }
            case MSG_FORMAT::JSON:
                {
                    msgin.serial(m_StrBuffer);
                    // check json
                    msgout.serial(m_StrBuffer);

                    if ( VAR_SAVE_EVENT && pMsgLeaf->is_log_event )
                    {
                        m_SaveEventStr.append( " " );
                        m_SaveEventStr.append( m_StrBuffer );
                    }

                    break;
                }
            case MSG_FORMAT::ProtoMsg:
                {
                    if ( j>=format_msg.size() )
                    {
                        return false;
                    }

                    pMessage = CreateMessage(format_msg[j]);

                    if ( pMessage!=NULL )
                    {
                        msgin.serial(pMessage);
                        msgout.serial(pMessage);

                        if ( VAR_SAVE_EVENT && pMsgLeaf->is_log_event )
                        {
                            m_SaveEventStr.append( " " );
                            m_SaveEventStr.append( pMessage->ShortDebugString() );
                        }

                        SAFE_DELETE(pMessage);
                    }
                    break;
                }
            case MSG_FORMAT::d:
                {
                    double   double_val;
                    msgin.serial(double_val);
                    msgout.serial(double_val);
                    break;
                }
            case MSG_FORMAT::b:
                {
                    bool   bool_val;
                    msgin.serial(bool_val);
                    msgout.serial(bool_val);
                    break;
                }
            case MSG_FORMAT::s:
                {
                    msgin.serial(m_StrBuffer);
                    msgout.serial(m_StrBuffer);
                    break;
                }
            case MSG_FORMAT::s8:
                {
                    sint8   sint8_val;
                    msgin.serial(sint8_val);
                    msgout.serial(sint8_val);
                    break;
                }
            case MSG_FORMAT::s16:
                {
                    sint16   sint16_val;
                    msgin.serial(sint16_val);
                    msgout.serial(sint16_val);
                    break;
                }
            case MSG_FORMAT::s32:
                {
                    sint32   sint32_val;
                    msgin.serial(sint32_val);
                    msgout.serial(sint32_val);
                    break;
                }
            case MSG_FORMAT::s64:
                {
                    sint64   sint64_val;
                    msgin.serial(sint64_val);
                    msgout.serial(sint64_val);
                    break;
                }
            case MSG_FORMAT::u8:
                {
                    uint8   uint8_val;
                    msgin.serial(uint8_val);
                    msgout.serial(uint8_val);
                    break;
                }
            case MSG_FORMAT::u16:
                {
                    uint16   uint16_val;
                    msgin.serial(uint16_val);
                    msgout.serial(uint16_val);
                    break;
                }
            case MSG_FORMAT::u32:
                {
                    uint32   uint32_val;
                    msgin.serial(uint32_val);
                    msgout.serial(uint32_val);
                    break;
                }
            case MSG_FORMAT::u64:
                {
                    uint64   uint64_val;
                    msgin.serial(uint64_val);
                    msgout.serial(uint64_val);
                    break;
                }
            default:
                return false;
            }
        }

        std::vector<std::string>& sendto = pMsgLeaf->sendto;
        for ( uint i=0; i<sendto.size(); ++i )
        {
            if ( sendto[i] == LogicService )
            {
                if ( pClient->pls_sid != NLNET::TServiceId::InvalidId )
                {
                    NLNET::CMessage msgluasvr("_LS");
                    msgluasvr.serialMessage(msgout);
                    Network->send( pClient->pls_sid, msgluasvr );
                }
            }
            else
            {
                NLNET::CMessage msgluasvr("_LS");
                msgluasvr.serialMessage(msgout);
                Network->send ( sendto[i], msgluasvr );
            }
        }

        if ( VAR_SAVE_EVENT && pMsgLeaf->is_log_event )
        {
            //LogEvent( id, msgin.getName(), m_SaveEventStr.strip() );
        }

        return true;
    }
    catch (NLMISC::Exception &)
    {
        SAFE_DELETE(pMessage);
        return false;
    }

    return false;
}

NLMISC_COMMAND (topmsg, "", "")
{
    if(args.size() != 0) return false;

    //CFrontEndService::TReceiveMsgCount::iterator iter,it_end;
    //iter    = FrontEndService->m_ReceiveMsgCount.begin();
    //it_end  = FrontEndService->m_ReceiveMsgCount.end();




    //std::multimap<uint64, std::string>  sortmap;

    //while ( iter!=it_end )
    //{
    //    sortmap.insert( make_pair( iter->second, iter->first ) );
    //    ++iter;
    //}

    //std::multimap<uint64, std::string>::reverse_iterator riter,rit_end;
    //riter    = sortmap.rbegin();
    //rit_end  = sortmap.rend();

    //while ( riter!=rit_end )
    //{
    //    log.displayNL("Count: %" NL_I64 "u       Msg: %s", riter->first, riter->second.c_str() );
    //    ++riter;
    //}

    return true;
}

NLMISC_COMMAND (loadmsg, "", "")
{
    if(args.size() != 0) return false;

    MsgDesc.LoadMsgXml();
    log.displayNL ("ReLoadMsgXml Done.");

    return true;
}








