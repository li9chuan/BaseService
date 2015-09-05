#include "msg_forwarding.h"
#include "frontend_service.h"

NLMISC::CVariable<bool>	VAR_MSG_COUNT("fes", "MsgCount", "memo", false, 0, true);
NLMISC::CVariable<bool>	VAR_SAVE_EVENT("fes", "SaveEvent", "memo", false, 0, true);

bool ForwardingMsg( FES::CClient* pClient, DEF::RPC_SESSION rpc_session, NLNET::CMessage& msgin, MsgLeaf* pMsgLeaf )
{
    //if ( pMsgLeaf==NULL )
    //{
    //    return NULL;
    //}
    //nlassert(pMsgLeaf!=NULL);

    google::protobuf::Message* pMessage = NULL;

    try
    {
        //nldebug("pid:%"NL_I64"u   msg:%s   session:%d   tick:%"NL_I64"u   %s",
        //    pClient->pid, msgin.getName().c_str(), (sint32)rpc_session>>7, 
        //    (uint64)NLMISC::CTime::getLocalTime(), pMsgLeaf->description.c_str() );

        NLNET::CMessage  msgout( rpc_session, pMsgLeaf->msgname );

        std::vector<MSG_FORMAT::TMsgFormat>& format = pMsgLeaf->format;
        std::vector<std::string>& format_msg = pMsgLeaf->format_msg;
        bool    isSave = false;
        uint64  id=0;

        for ( uint i=0,j=0; i<format.size(); ++i )
        {
            switch (format[i])
            {
            case MSG_FORMAT::PID:
                {
                    id = pClient->pid;
                    msgout.serial(pClient->pid);
                    break;
                }
            case MSG_FORMAT::UID:
                {
                    id = pClient->uid;
                    msgout.serial(pClient->uid);
                    break;
                }
            case MSG_FORMAT::PLS:
                {
                    msgout.serial(pClient->conPLS);
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

                        //LogEvent( id, msgin.getName(), pMessage->DebugString() );
                        isSave = true;
                        //nldebug("%s", pMessage->DebugString().c_str());

                        SAFE_DELETE(pMessage);
                    }
                    break;
                }
            default:
                return false;
            }
        }

        std::vector<std::string>& sendto = pMsgLeaf->sendto;
        for ( uint i=0; i<sendto.size(); ++i )
        {
            //nldebug("sendto:%s",sendto[i].c_str());

            if ( sendto[i] == LogicService )
            {
                Network->send ( pClient->conPLS, msgout );
            }
            else
            {
                Network->send ( sendto[i], msgout );
            }
        }

        //if ( VAR_SAVE_EVENT.get() && !isSave )
        //{
        //    LogEvent( id, msgin.getName(), "" );
        //}

        if( VAR_MSG_COUNT.get() )
        {
            FrontEndService->IncReceiveMsgCount(msgin.getName());
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


