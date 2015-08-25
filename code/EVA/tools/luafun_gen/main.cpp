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

using namespace std;
using namespace NLMISC;

string EXPORT_TMP_PKG = "export_lua_tmp.pkg";

int main (int argc, char *argv[])
{
	createDebug( NULL, false );
	CConfigFile config;
	CPath::addSearchPath(".");

    if ( argc!=2 )
    {
        cout<<"param : xxxxx.cfg"<<endl;;
        return 1;
    }

    config.load ( CPath::lookup(argv[1]) );	

	char ch1;
	char ch2;
	
	ifstream file_source( config.getVar ("export_head").asString() );
    ofstream file_final( EXPORT_TMP_PKG, ofstream::out );
	//ofstream file_final( config.getVar ("tmpfile").asString(), ofstream::out );
	
	while (file_source.get(ch1))
	{
		//cout<<ch1;
		file_final<<ch1;
	}

    file_source.close();

  
	std::ifstream ino(config.getVar ("export").asString().c_str()); //
	std::ostringstream tmp_function; 
	tmp_function << ino.rdbuf(); 
	
	CSString  cstring_function = tmp_function.str();
	CVectorSString vct_cstring_function;
	cstring_function.splitLines(vct_cstring_function);

	for ( uint i=0; i<vct_cstring_function.size(); ++i )
	{
		if ( (vct_cstring_function[i].size()>0) &&
			(vct_cstring_function[i].find("#")==string::npos)&&
		    (vct_cstring_function[i].at(0)!='/')&&
		    (vct_cstring_function[i].at(0)!='*')&&
		    vct_cstring_function[i].at(0)!=' ')
		{
			//cout<<vct_cstring_function[i]<<endl;
			file_final<<vct_cstring_function[i]<<endl;
			i++;
		}
	}
	file_final<<endl;


    if ( config.exists("proto_file") &&
         config.exists("proto_enum") &&
         config.getVar ("proto_enum").size()>0 &&
         config.getVar ("proto_file").asString()!="" )
    {
        std::ifstream proto_file(config.getVar ("proto_file").asString().c_str()); //
        std::ostringstream tmp_proto; 
        tmp_proto << proto_file.rdbuf(); 
        CSString  cstring_proto = tmp_proto.str();
        CVectorSString vct_cstring_proto;
        cstring_proto.splitLines(vct_cstring_proto);

        int enum_size = config.getVar ("proto_enum").size();

	    for ( uint i=0; i<vct_cstring_proto.size(); ++i )
	    {
		    for ( int j=0; j<enum_size; ++j )
		    {
			    if ( vct_cstring_proto[i].find(const_cast<char*>(config.getVar ("proto_enum").asString(j).c_str())) != string::npos )
		        {
			        while(vct_cstring_proto[i].find("}") == string::npos)
			        {
				
				        if (vct_cstring_proto[i].find(";")!=string::npos)
				        {
					        //cout<<vct_cstring_proto[i].replace(";",",")<<endl;
					        file_final<<vct_cstring_proto[i].replace(";",",")<<endl;
				        } 
				        else
				        {
					        //cout<<vct_cstring_proto[i]<<endl;
					        file_final<<vct_cstring_proto[i]<<endl;
				        }
				        i++;
			        }
			        //cout<<vct_cstring_proto[i]<<endl;
			        file_final<<vct_cstring_proto[i]<<endl;
                    file_final<<endl;
		        }
		    }
	    }
    }

	
	file_final.close();
	cout<<endl;

    string exec_str = config.getVar ("tolua_exec").asString() + " " + EXPORT_TMP_PKG;
    system( exec_str.c_str() );

    
    //string del_tempfile = "del " + config.getVar ("tmpfile").asString();
    string del_tempfile = "del " + EXPORT_TMP_PKG;
    system( del_tempfile.c_str() );
	//system("pause");
	return EXIT_SUCCESS;
}
