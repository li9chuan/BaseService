#include "stdpch.h"
#include "player_timer_event.h"
#include "player.h"
#include "player_mgr.h"

namespace PLS
{
	using namespace NLMISC;

	NL_INSTANCE_COUNTER_IMPL(CPlayerTickUpdateTimerEvent);
	NL_INSTANCE_COUNTER_IMPL(CPlayerDbUpdateTimerEvent);
	NL_INSTANCE_COUNTER_IMPL(CPlayerBarUpdateTimerEvent);
	

	// ***************************************************************************
	CPlayerTickUpdateTimerEvent:: CPlayerTickUpdateTimerEvent(CPlayer *parent)
	{
		_Parent = parent;
	}

	void CPlayerTickUpdateTimerEvent::timerCallback(CTimer* owner)
	{
		H_AUTO(CPlayerTickUpdateTimerEvent);

		//uint32 timeToNextTick= _Parent->tickUpdate();
		//owner->setRemaining(timeToNextTick,this);
	}

	// ***************************************************************************
	CPlayerDbUpdateTimerEvent:: CPlayerDbUpdateTimerEvent(CPlayer *parent)
	{
		_Parent = parent;
	}

	void CPlayerDbUpdateTimerEvent::timerCallback(CTimer* owner)
	{
		H_AUTO(CPlayerDbUpdateTimerEvent);
	
		//_Parent->databaseUpdate();
		owner->setRemaining(2,this);
	}


	// ***************************************************************************
	CPlayerBarUpdateTimerEvent::CPlayerBarUpdateTimerEvent(CPlayer *parent)
	{
		_Player = parent;
	}

	void CPlayerBarUpdateTimerEvent::timerCallback(CTimer *owner)
	{
		H_AUTO(CPlayerBarUpdateTimerEvent);


		owner->setRemaining(5000,this);
	}

}

