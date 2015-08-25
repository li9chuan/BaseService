#ifndef FES_LOGIN_MODULES_ITF
#define FES_LOGIN_MODULES_ITF
#include "nel/misc/types_nl.h"
#include <memory>
#include "nel/misc/hierarchical_timer.h"
#include "nel/misc/string_conversion.h"
#include "nel/net/message.h"
#include "nel/net/module.h"
#include "nel/net/module_builder_parts.h"
#include "nel/net/module_message.h"
#include "nel/net/module_gateway.h"

#include "game_share/callback_adaptor.h"

#include "nel/misc/time_nl.h"


// This is the interface used by PHP to call methods
// on the Admin service module

class CLoginServiceWebItf
{
protected:

	/// the callback server adaptor
	std::auto_ptr<ICallbackServerAdaptor>	_CallbackServer;

	void getCallbakArray(NLNET::TCallbackItem *&arrayPtr, uint32 &arraySize)
	{
		static NLNET::TCallbackItem callbackArray[] =
		{
			{	"LG",	CLoginServiceWebItf::cb_WebLogin	},
		};

		arrayPtr = callbackArray;
		arraySize = sizeofarray(callbackArray);
	}

	static void _cbConnection(NLNET::TSockId from, void *arg)
	{
		H_AUTO(CLoginServiceWeb__cbConnection);
		CLoginServiceWebItf *_this = reinterpret_cast<CLoginServiceWebItf *>(arg);
	}

	static void _cbDisconnection(NLNET::TSockId from, void *arg)
	{
		H_AUTO(CLoginServiceWeb__cbDisconnection);
		CLoginServiceWebItf *_this = reinterpret_cast<CLoginServiceWebItf *>(arg);
	}


public:
	/** Constructor, if you specify a replacement adaptor, then the object
		*	become owner of the adaptor (and it will be released with the
		*	interface).
		*/
	CLoginServiceWebItf(ICallbackServerAdaptor *replacementAdaptor = NULL)
	{
		if (replacementAdaptor == NULL)
		{
			// use default callback server
			_CallbackServer = std::auto_ptr<ICallbackServerAdaptor>(new CNelCallbackServerAdaptor(this));
		}
		else
		{
			// use the replacement one
			_CallbackServer = std::auto_ptr<ICallbackServerAdaptor>(replacementAdaptor);
		}
	}

	virtual ~CLoginServiceWebItf()
	{
	}

	/// Open the interface socket in the specified port
	void openItf(uint16 port)
	{
		NLNET::TCallbackItem *arrayPtr;
		uint32 arraySize;



		getCallbakArray(arrayPtr, arraySize);
		_CallbackServer->addCallbackArray(arrayPtr, arraySize);

		_CallbackServer->setConnectionCallback (_cbConnection, this);
		_CallbackServer->setDisconnectionCallback (_cbDisconnection, this);

		_CallbackServer->init(port);
	}

	/** Must be called evenly, update the network subclass to receive message
		*	and dispatch method invokation.
		*/
	void update()
	{
		H_AUTO(CLoginServiceWeb_update);

		try
		{
			_CallbackServer->update();
		}
		catch (...)
		{
			nlwarning("CLoginServiceWeb : Exception launch in callback server update");
		}
	}


	//NLNET::CMessage message("CMDR");
	//nlWrite(message, serial, const_cast < std::string& > (serviceAlias));
	//nlWrite(message, serial, const_cast < std::string& > (result));
	//_CallbackServer->send(message, dest);

	static void cb_WebLogin (NLNET::CMessage &message, NLNET::TSockId from, NLNET::CCallbackNetBase &netbase)
	{
		H_AUTO(cb_WebLogin);
#ifdef NL_DEBUG
		nldebug("CLoginServiceWeb::cb_WebLogin received from class '%s'", typeid(netbase).name());
#endif
		ICallbackServerAdaptor *adaptor = static_cast< ICallbackServerAdaptor *>(netbase.getUserData());

		CLoginServiceWebItf *callback = (CLoginServiceWebItf *)adaptor->getContainerClass();

		if (callback  == NULL)
			return;

		uint32  uid;
		std::string	token;
		nlRead(message, serial, uid);
		nlRead(message, serial, token);

		callback->on_login(from, uid, token);

#ifdef NL_DEBUG
		nldebug("CLoginServiceWeb::cb_WebLogin : calling cb_WebLogin");
#endif

	}

	virtual void on_login(NLNET::TSockId from, const sint32 uid, const std::string &ipaddr) =0;

};


#endif	// FES_LOGIN_MODULES_ITF
