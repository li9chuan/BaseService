#ifndef PLS_CHAT_LOGIC_H
#define PLS_CHAT_LOGIC_H

#include <nel/misc/singleton.h>
#include <game_share/proto_msg/msg_c2s.pb.h>
#include <game_share/game_def.h>

namespace PLS
{
	/**
	 * 聊天相关逻辑处理类
	 * Note: 各种聊天频道的操作接口，全局唯一。
	 * 
	 * \author li9chuan@qq.com
	 * \date 2015
	 */
    class CPlayer;

	class CChatLogic : public NLMISC::CSingleton<CChatLogic>
	{
	public:

		///  全球喊话    大喇叭
		void GlobleChat( DEF::PID pid, MsgChat& msg_chat );


	private:


    };

}

#define  ChatLogic  PLS::CChatLogic::instance()

#endif		//	PLS_CHAT_LOGIC_H









