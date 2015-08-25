#include "daemon_thread.h"
#include <iostream>
#include <nel/misc/sstring.h>

using namespace std;
using namespace NLMISC;

void DaemonThread::init( string path, string alias, std::string exec )
{
    PATH  = path;
    ALIAS = alias+"/"+alias;
    EXEC  = exec;
}

void DaemonThread::run ()
{
    _run = true;

    uint  start_counter = 0;
    WriteFile(PATH+ALIAS+START_COUNTER_FILE,"0");
    WriteFile(PATH+ALIAS+STATE_FILE,"STOPPED");

    while( _run )
    {
        string command = ReadFile(PATH+ALIAS+CTRL_FILE);

        if ( command=="LAUNCH" )
        {
            ++start_counter;
            WriteFile(PATH+ALIAS+START_COUNTER_FILE, toString(start_counter));

            cout<<EXEC<<endl;
            WriteFile(PATH+ALIAS+STATE_FILE,"RUNNING");
            system(EXEC.c_str());
            WriteFile(PATH+ALIAS+STATE_FILE,"STOPPED");

            CFile::deleteFile(PATH+ALIAS+CTRL_FILE);
        }

        if ( CFile::fileExists(PATH+ALIAS+NEXT_CTRL_FILE) )
        {
            string src  = PATH+ALIAS+NEXT_CTRL_FILE;
            string dest = PATH+ALIAS+CTRL_FILE;
            CFile::moveFile( dest.c_str(), src.c_str() );
        }
        else
        {

        }

        NLMISC::nlSleep(5000);
    }
}

std::string DaemonThread::ReadFile( std::string filename )
{
    FILE* f= fopen(filename.c_str(),"rt");
    if (f==NULL) return "";

    uint32 fileSize= NLMISC::CFile::getFileSize(f);
    std::string buff; buff.resize(fileSize);
    uint32 bytesRead= (uint32)fread(&buff[0],1,fileSize,f);
    fclose(f);
    return buff;
}

void DaemonThread::WriteFile( std::string filename, std::string out_txt )
{
    // open the file for writing
    FILE* f= fopen(filename.c_str(),"wt");
    if (f==NULL) return;

    // write the text to the file
    fprintf(f,"%s",out_txt.c_str());
    fclose(f);
}


