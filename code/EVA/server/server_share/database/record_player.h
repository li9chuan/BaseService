#ifndef RECORD_PLAYER_H
#define RECORD_PLAYER_H

#include <nel/misc/stream.h>
#include <game_share/proto_msg/msg_player.pb.h>
#include "record_base.h"

namespace DB_RECORD {

	struct CRecordPlayer : public CRecordBase
	{
		DEF::PID            pid;
		DEF::UID			uid;
		std::string			name;
		uint8               sex;
        uint16              level;
		uint32				exp;
        uint32              pve_dup_id;
		DEF::MONEY          money;
        uint32              last_online_time;

		CRecordPlayer()  { reset(); }
		virtual ~CRecordPlayer() { reset(); }

		void reset();

		void serial( NLMISC::IStream& s )
		{
			s.serial( pid );
			s.serial( uid );
			s.serial( name );
			s.serial( sex );
            s.serial( level );
			s.serial( exp );
            s.serial( pve_dup_id );
			s.serial( money );
        }

        void getMsgPlayerInfo( MsgPlayerInfo& player_info )
        {
            player_info.set_name(name);
            player_info.set_sex((TGender)sex);
            player_info.set_pid( pid );

            getMsgBase(player_info);
        }

        void getMsgBase( MsgPlayerInfo& player_info )
        {
            player_info.set_level(level);
            player_info.set_exp(exp);
            player_info.set_money(money);
        }

        ///  数据是否相等，存储时检查脏数据使用
        bool operator!=(const CRecordPlayer &record_player) const
        {
            return (    level       != record_player.level  ||
                        exp         != record_player.exp    ||
                        pve_dup_id  != record_player.pve_dup_id ||
                        money       != record_player.money  );
        }  
	};
}

#endif
