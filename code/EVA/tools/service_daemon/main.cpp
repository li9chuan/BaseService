#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sstream>
#include <fstream>
#include <iostream>
#include <cstring>
#include <algorithm>

#include "nel/misc/debug.h"
#include "nel/misc/config_file.h"
#include "nel/misc/path.h"
#include "nel/misc/sstring.h"
#include "nel/misc/file.h"
#include "daemon_thread.h"

using namespace std;
using namespace NLMISC;

void check_service(CConfigFile& config)
{
    CConfigFile::CVar* pVar = config.getVarPtr("RegisteredServices");

    for ( uint i=0; i<pVar->size(); ++i )
    {
        if ( "aes"==pVar->asString(i) )
        {
            CConfigFile::CVar* pAliasVar = config.getVarPtr(pVar->asString(i));

            if ( pAliasVar!=NULL && pAliasVar->size()==3 )
            {
                if ( !CFile::isExists(pAliasVar->asString(0)+"aes") )
                {
                    string path = pAliasVar->asString(0)+"aes/";
                    NLMISC::CFile::createDirectoryTree(path);
                    DaemonThread::WriteFile(path+"aes"+CTRL_FILE, "LAUNCH");
                }
            }
        }
    }
}


int main (int /* argc */, char ** /* argv */)
{
    createDebug();
    CConfigFile config;
    CPath::addSearchPath(".");
//    CPath::addSearchPath("D:/MT/trunk/code/EVA/server");
    
    config.load (CPath::lookup("admin_executor_service.cfg"));

    CConfigFile::CVar* pVar = config.getVarPtr("RegisteredServices");
    
    if ( pVar==NULL )
    {
        cout<<"RegisteredServices is not find."<<endl;
        nlSleep(1000);
        return EXIT_FAILURE;
    }

    check_service(config);
    
    vector<DaemonThread>   vctDaemonThread;
    vctDaemonThread.resize(pVar->size());

    for ( uint i=0; i<pVar->size(); ++i )
    {
        cout<<pVar->asString(i)<<endl;

        CConfigFile::CVar* pAliasVar = config.getVarPtr(pVar->asString(i));

        if ( pAliasVar!=NULL && pAliasVar->size()==3 )
        {
            string exec = pAliasVar->asString(1) + " ";
            exec += "-A" +pAliasVar->asString(0) + " ";
            exec += pAliasVar->asString(2);

            vctDaemonThread[i].init(    pAliasVar->asString(0), 
                                        pVar->asString(i), 
                                        exec);

            cout<<"start daemon "<<pVar->asString(i)<<endl;
            IThread::create(&vctDaemonThread[i])->start ();
            nlSleep(1000);
        }
        else
        {
            cout<<pVar->asString(i)<<"Ã»ÓÐÏêÏ¸ÅäÖÃ"<<endl;
        }
    }

    while(true)
    {
        nlSleep(10000);
    }

    return EXIT_SUCCESS;
}


