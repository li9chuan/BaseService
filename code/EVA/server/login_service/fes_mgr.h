#ifndef LS_FES_MGR_H
#define LS_FES_MGR_H

#include <server_share/msg_struct/msg_struct_login.h>
#include <vector>
namespace LS
{

class CFesMgr : public NLMISC::CSingleton<CFesMgr>
{
public:

	typedef std::vector<MSG_STR::CFrontEnd>  TFrontEndCont;

	CFesMgr():_CurrPlayers(0),_MaxPlayres(0)
	{
	}

	MSG_STR::CFrontEnd* findFes( NLNET::TServiceId SId )
	{
		MSG_STR::CFrontEnd* pFes = NULL;
		for ( uint i=0; i<_vctFrontEnd.size(); ++i )
		{
			if ( _vctFrontEnd[i].ConnectId == SId )
			{
				pFes = &_vctFrontEnd[i];
				break;
			}
		}
		return pFes;
	}

	void removeFes( NLNET::TServiceId SId )
	{
		std::vector<MSG_STR::CFrontEnd>::iterator iter;

		for ( iter = _vctFrontEnd.begin(); iter!=_vctFrontEnd.end(); ++iter )
		{
			if ( iter->ConnectId == SId )
			{
				_vctFrontEnd.erase(iter);
				break;
			}
		}
	}

	bool addFes( MSG_STR::CFrontEnd& fes )
	{
		bool res = false;
		if ( findFes(fes.ConnectId) == NULL )
		{
			_vctFrontEnd.push_back(fes);
			res = true;
		}
		return res;
	}

	void   resetCurrPlayers()
	{
		_CurrPlayers = 0;
		for ( uint i=0; i<_vctFrontEnd.size(); ++i )
		{
			_CurrPlayers += _vctFrontEnd[i].CurrClientCount;
		}
	}

	//uint32 getCurrPlayers()	{	return _CurrPlayers;	}
	//uint32 getMaxPlayers()	{	return _MaxPlayres;		}

	bool   isFull()	{	return _CurrPlayers >= _MaxPlayres;	}

	TFrontEndCont&  getFrontEndCont()	{	return _vctFrontEnd;	}

private:

	TFrontEndCont  _vctFrontEnd;
	sint     _CurrPlayers;
	sint     _MaxPlayres;
};

}

#define  FesMgr  LS::CFesMgr::instance()

#endif // LS_FES_MGR_H

/* End of fes_mgr.h */
