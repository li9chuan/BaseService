#ifdef HAVE_CONFIG_H
#include "config.h"
#endif // HAVE_CONFIG_H

#include "login_mgr.h"
#include "auth_mgr.h"

#include <nel/misc/config_file.h>
#include <nel/misc/displayer.h>
#include <nel/misc/command.h>
#include <nel/misc/log.h>
#include <nel/misc/window_displayer.h>
#include <nel/misc/path.h>

#include "login_service.h"
#include "connection_client.h"
#include "fes_mgr.h"
#include <server_share/server_def.h>

//
// Namespaces
//

using namespace std;
using namespace DEF;
using namespace NLMISC;
using namespace NLNET;

extern void admin_modules_forceLink();
extern void ls_modules_forceLink();

void foo()
{
    ls_modules_forceLink();
    admin_modules_forceLink();
}

//
// Variables
//

// store specific user information
NLMISC::CFileDisplayer *Fd = NULL;
NLMISC::CStdDisplayer Sd;

// transform "192.168.1.1:80" into "192.168.1.1"
string removePort (const string &addr)
{
	return addr.substr (0, addr.find (":"));
}

void displayShards ()
{
	ICommand::execute ("shards", *InfoLog);
}

void displayUsers ()
{
	ICommand::execute ("users", *InfoLog);
}

////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
/////////////// SERVICE IMPLEMENTATION /////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////
////////////////////////////////////////////////////////////////////////////////////////////////

void beep (uint freq, uint nb, uint beepDuration, uint pauseDuration)
{
#ifdef NL_OS_WINDOWS
	try
	{
		if (IService::getInstance()->ConfigFile.getVar ("Beep").asInt() == 1)
		{
			for (uint i = 0; i < nb; i++)
			{
				Beep (freq, beepDuration);
				nlSleep (pauseDuration);
			}
		}
	}
	catch (Exception &)
	{
	}
#endif // NL_OS_WINDOWS
}

NLNET::TUnifiedCallbackItem LSCallbackArray[] =
{
	{ "SET_FES_INFO",			cbFesInfo },
	{ "SET_FES_PLAYERS",		cbFesCurrPlayers },
//	{ "VLP",    cbClientRequestVLP            },                // from FES
	{ "LOGIN",  cbLogin                       },                // from FES

};


class CLoginService : public IService
{
public:
	
	CLoginService ()  { }

	/// Init the service, load the universal time.
	void init ()
	{
		beep ();

		//string language = ConfigFile.getVar("Language").asString();
		CUnifiedNetwork::getInstance()->setServiceUpCallback("FES", cbFESConnection);
		CUnifiedNetwork::getInstance()->setServiceDownCallback("FES", cbFESDisConnection);

		//AuthMgr.init();
		LoginMgr.init();
	}

	bool update ()
	{
		LoginMgr.update();
		return true;
	}

	void release ()
	{
	}
};

// Service instantiation
NLNET_SERVICE_MAIN (CLoginService, "LS", "login_service", 49999, LSCallbackArray, "", "");

//
// Variables
//

//NLMISC_DYNVARIABLE(uint, OnlineUsersNumber, "number of actually connected users")
//{
//	// we can only read the value
//	if (get)
//	{
//		//uint32 nbusers = 0;
//		//for (uint i = 0; i < Users.size(); i++)
//		//{
//		//	if (Users[i].State == CUser::Online)
//		//		nbusers++;
//		//}
//		*pointer = NbPlayers;
//	}
//}

//
// Commands
//

NLMISC_COMMAND (tokensize, "displays the number of all cache tokens.", "")
{
	if(args.size() != 0) return false;

	log.displayNL ("Display the number of all cache tokens :");
	log.displayNL ("-> LoginTokenSize = %d", LoginMgr.LoginTokenSize());
	log.displayNL ("-> LoginUIDSize   = %d", LoginMgr.LoginUIDSize());
	log.displayNL ("End of the cache tokens");

	return true;
}

NLMISC_COMMAND (linkservice, "displays the list of all link service.", "")
{
	LS::CFesMgr::TFrontEndCont& front_ends = FesMgr.getFrontEndCont();
	
	for ( uint32 i=0; i<front_ends.size(); ++i )
	{
		log.displayNL ("----> IDX %d", i);
		log.displayNL ("      Shard  ID = %d", front_ends[i].ShardId);
        log.displayNL ("      Server Name = %s %s", front_ends[i].HostName.c_str(), front_ends[i].UnifiedName.c_str());
		log.displayNL ("      Server Host = %s:%d", front_ends[i].HostName.c_str(), front_ends[i].Port);
	}

	return true;
	//return ICommand::execute ("l5InternalTables", *InfoLog);
}

