#ifndef SERVER_SHARED_DB_LOGIC_MAIL_H
#define SERVER_SHARED_DB_LOGIC_MAIL_H

#include "record_mail_simple.h"
#include "record_mail_normal.h"

// Mail的游戏逻辑组织形式

namespace DB_RECORD {

	struct CDBLogicMail
	{
		std::map<DEF::MAIL_ID,DB_RECORD::CRecordMailSimple>           RecordMailSimple;
		std::map<DEF::MAIL_ID,DB_RECORD::CRecordMailNormal>           RecordMailNormal;

		void serial( NLMISC::IStream& s )
		{
			s.serialCont( RecordMailSimple );
			s.serialCont( RecordMailNormal );
		}

		void getMsgMail( MsgMail& msg_mail )
		{
			for ( uint i=0; i<RecordMailSimple.size(); ++i )
			{
				MsgMailSimple* pMsgMailSimple = msg_mail.add_mail_simple();
				RecordMailSimple[i].getMsgMailSimple( *pMsgMailSimple );
			}
			
			for ( uint i=0; i<RecordMailNormal.size(); ++i )
			{
				MsgMailNormal* pMsgMailNormal = msg_mail.add_mail_normal();
				RecordMailNormal[i].getMsgMailNormal( *pMsgMailNormal );
			}
		}
	};

	typedef CHashMap< DEF::PID, CDBLogicMail >         TDBMail;
}

#endif
