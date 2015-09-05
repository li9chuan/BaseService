#ifndef PLS_PLAYER_DATA_HELPER_H
#define PLS_PLAYER_DATA_HELPER_H

#include <nel/net/unified_network.h>
#include <game_share/game_def.h>
#include <server_share/database/db_logic_chunk.h>
#include <server_share/msg_struct/log_rmb.h>

namespace PLS
{
	struct CMsgPlayerInfo;

	class CPlayer;

	/**
	 * 玩家数据助手
	 * Note: 保存玩家数据，提供基础操作。
	 * 
	 * \author li9chuan@qq.com
	 * \date 2014
	 */

	class CPlayerDataHelper
	{
	public:
		CPlayerDataHelper( DB_RECORD::CDBLogicPlayer& DBLogicPlayer );
		~CPlayerDataHelper(){};

		/**
		 * Player基础数据获取接口
		 */
		///@{

        DEF::PID            getPID();
        DEF::UID            getUID();

        std::string         getName();

        DEF::MONEY          addMoney( DEF::MONEY change_value, bool save=true );
        bool                subMoney( DEF::MONEY sub_money, bool save=true );
        DEF::MONEY          getMoney()      { return _RecordPlayer.money; }

		///@}

        
        /**
		 * 给客户端的消息接口
		 */
		///@{
        //void                SendToClientPlayerInfo( DEF::RPC_SESSION session, bool disconnect=true );
        ///@}

		const DB_RECORD::CRecordPlayer&     getRecordPlayer();

		//     仅存盘，不写数据到cache
		void                SavePlayerToDB( bool save=true );

		void                getMsgPlayerInfo( MsgPlayerInfo& player_info )  { _RecordPlayer.getMsgPlayerInfo(player_info); }
        void                getMsgBase( MsgPlayerInfo& player_info )  { _RecordPlayer.getMsgBase(player_info); }

		friend class CPlayer;		//	for init m_Player
	private:

        void                addTotalRMB( DEF::RMB add_rmb );

		CPlayer*                         m_Player;
        DB_RECORD::CRecordPlayer&        _RecordPlayer;
	};



};

#endif		/// PLS_PLAYER_DATA_HELPER_H
