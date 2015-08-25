#include "nel/misc/types_nl.h"
#include <time.h>
#include "nel/misc/file.h"
#include "nel/misc/sstring.h"
#include "nel/misc/mutable_container.h"
#include "nel/net/service.h"
#include "nel/net/module.h"
#include "nel/net/module_builder_parts.h"

#include "login_modules_itf.h"

#include "../login_mgr.h"

using namespace std;
using namespace NLMISC;
using namespace NLNET;

void ls_forceLink() {}

class CLoginModule 
	:	public CEmptyModuleServiceBehav<CEmptyModuleCommBehav<CEmptySocketBehav<CModuleBase> > >,
		CLoginServiceWebItf,
		IModuleTrackerCb
{
public:
	CLoginModule()
	{}

	~CLoginModule()
	{}
		
	bool initModule(const TParsedCommandLine &pcl)
	{
		CModuleBase::initModule(pcl);

		// read the command line
		const TParsedCommandLine *webPort = pcl.getParam("webPort");
		nlassert(webPort != NULL);
		uint16 port;
		NLMISC::fromString(webPort->ParamValue, port);
		// open the web interface
		CLoginServiceWebItf::openItf(port);

		return true;
	}

	void onModuleUpdate()
	{
		H_AUTO(CLoginService_onModuleUpdate);
		CLoginServiceWebItf::update();
	}

	///////////////////////////////////////////////////////////////////////
	//// Virtuals from IModuleTrackerCb
	///////////////////////////////////////////////////////////////////////

	virtual void onTrackedModuleUp(IModuleProxy *moduleProxy)
	{
		nldebug("LOGIN module '%s' UP", moduleProxy->getModuleName().c_str());

	}
	virtual void onTrackedModuleDown(IModuleProxy *moduleProxy)
	{
		nldebug("LOGIN module '%s' DOWN", moduleProxy->getModuleName().c_str());

	}

	///////////////////////////////////////////////////////////////////////
	//// Virtuals from CLoginServiceWebItf
	///////////////////////////////////////////////////////////////////////

	virtual void on_login(NLNET::TSockId from, const sint32 uid, const std::string &token)
	{
		LoginMgr.AddLoginToken(uid,token);

        NLNET::CMessage message("LGR");
        _CallbackServer->send(message, from);
	}

};

NLNET_REGISTER_MODULE_FACTORY(CLoginModule, "LoginModule");

