#ifndef SERVER_SHARED_CLIENT_MSG_DESC_H
#define SERVER_SHARED_CLIENT_MSG_DESC_H

#include <nel/misc/singleton.h>
#include <nel/misc/sstring.h>
#include "game_def.h"
#include "msg_leaf.h"

class CMsgDesc : public NLMISC::CSingleton<CMsgDesc>
{
public:

    void LoadMsgXml();

    MsgLeaf*  GetTCPMsgLeaf( std::string msg_name );
    MsgLeaf*  GetUDPMsgLeaf( std::string msg_name );

//    TCallbackItem* GetTCPCallbackItem();

private:

    typedef std::map<std::string, MsgLeaf>  TMsgDesc;

    TMsgDesc             m_MsgDescUDP;
    TMsgDesc             m_MsgDescTCP;
};

#define  MsgDesc  CMsgDesc::instance()

#endif
