#ifndef PLS_PLAYER_TIMER_EVENT_H
#define PLS_PLAYER_TIMER_EVENT_H

#include <game_share/timer.h>

namespace PLS
{

	class CPlayer;

	// ***************************************************************************
	// different low frequency update
	class CPlayerTickUpdateTimerEvent : public CTimerEvent
	{
		NL_INSTANCE_COUNTER_DECL(CPlayerTickUpdateTimerEvent);
	public:

		CPlayerTickUpdateTimerEvent(CPlayer *parent);
		void timerCallback(CTimer *owner);
	private:
		CPlayer *_Parent;
	};

	// ***************************************************************************
	// frequent update of the database
	class CPlayerDbUpdateTimerEvent:public CTimerEvent
	{
		NL_INSTANCE_COUNTER_DECL(CPlayerDbUpdateTimerEvent);
	public:
		CPlayerDbUpdateTimerEvent(CPlayer *parent);
		void timerCallback(CTimer *owner);
	private:
		CPlayer *_Parent;
	};

	// ***************************************************************************
	// frequent update of the player HP/STA/SAP/FOCUS bars
	class CPlayerBarUpdateTimerEvent : public CTimerEvent
	{
		NL_INSTANCE_COUNTER_DECL(CPlayerBarUpdateTimerEvent);
	public:
		CPlayerBarUpdateTimerEvent(CPlayer *parent);
		void timerCallback(CTimer *owner);
	private:
		CPlayer *_Player;
	};

}


#endif		//	PLS_PLAYER_TIMER_EVENT_H
