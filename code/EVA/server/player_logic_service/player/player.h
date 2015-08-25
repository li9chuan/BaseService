#ifndef PLS_PLAYER_H
#define PLS_PLAYER_H

#include <game_share/timer.h>
#include <server_share/base_object.h>

#include "player_data_helper.h"

namespace PLS
{
	class CPlayer// : public CBaseObject
	{
	public:
		CPlayer();
		~CPlayer();

		NLNET::TServiceId   getConFES()	{ return conFES; }
		void                setConFES( uint16 service_id )	{ conFES.set(service_id); }

		const NLMISC::TTime GetLastUpdateTime()	{ return m_LastUpdateTime; }
		void  SetLastUpdateTime( NLMISC::TTime curr_time )	{ m_LastUpdateTime=curr_time; }

		DEF::PID            getPID()	{ return DBLogicPlayer.RecordPlayer.pid; }
		DEF::UID            getUID()	{ return DBLogicPlayer.RecordPlayer.uid; }
		void                save();

		/**
		*   玩家登录时，PDS数据填充到本类。
		*   \param msgin PDS发来的初始化数据
		*/
		void          fillData( NLNET::CMessage &msgin );

        void          FirstLogin();

        DB_RECORD::CDBLogicPlayer&  getLogicPlayer()            { return DBLogicPlayer ; }

		///  helper
		CPlayerDataHelper&          getPlayerHelper()           { return m_DataHelper; }
       

	private:
	
		DB_RECORD::CDBLogicPlayer       DBLogicPlayer;

		CPlayerDataHelper               m_DataHelper;


		NLNET::TServiceId	conFES;
		NLMISC::TTime       m_LastUpdateTime;

        CTimer              m_OfflineTimer;
	};
}
#endif		//	PLS_PLAYER_H









