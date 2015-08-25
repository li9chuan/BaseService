#pragma	  once
#include <nel/misc/types_nl.h>
#include <game_share/game_def.h>

//////////////////////////////////////////////////////////////////////////

class CLuaEvent
{
public:
	CLuaEvent()
	{
		reset();
	}

	inline void reset()
    {
		m_EventID	= EventInvalid;
		m_ID		= -1;
		m_nParam[0] = 0;
		m_nParam[1] = 0;
		m_szParam[0] = '\0';
	}

	inline DEF::EVENT_ID EventID()					{ return m_EventID;}
	inline void SetEventID(DEF::EVENT_ID EventID)	{ m_EventID = EventID; }

	inline bool operator == (const CLuaEvent& event)
	{
		return ((m_EventID == event.m_EventID) &&
			   (m_ID == event.m_ID) &&
			   (m_nParam[0] == event.m_nParam[0])&&
			   (m_nParam[1] == event.m_nParam[1]));
	}

	inline CLuaEvent& operator = (const CLuaEvent& event)
	{
		m_ID	= event.m_ID;
		m_EventID = event.m_EventID;
		m_nParam[0] = event.m_nParam[0];
		m_nParam[1] = event.m_nParam[1];
		memcpy( m_szParam, event.m_szParam, STR_PARAM_LEN );
		return *this;
	}

public:

	enum { STR_PARAM_LEN = 128 };
	DEF::EVENT_ID   m_EventID;
	sint64          m_ID;					// 事件发起者
	sint64          m_nParam[2];
	char            m_szParam[STR_PARAM_LEN];
};


