#include "test_thread.h"
#include <iostream>
#include <nel/misc/sstring.h>

using namespace std;
using namespace NLMISC;

void SaveThread::init()
{
	_DBOptQueqe.init( 10 );
}

bool SaveThread::pushSQL( const char* sql, const uint64 id )
{
	DBOpt* opt = new DBOpt(sql,id);

	if( opt != NULL )
	{
		if ( id != 0 )
		{
			_MutexUniqueSql.enter();
			TUniqueSQLQuery::iterator iter = _UniqueSqlCount.find(opt->id);

			if ( iter == _UniqueSqlCount.end() )
			{
				_UniqueSqlCount[opt->id] = 1;
			}
			else
			{
				iter->second+=1;
			}

			_MutexUniqueSql.leave();
		}
		
		if ( _DBOptQueqe.push_back(opt) )
		{
			return true;
		}
		else
		{
			delete opt;
		}
	}
	return false;
}

void SaveThread::run ()
{
	_run = true;
	DBOpt* opt = NULL;

	while( (opt = _DBOptQueqe.pop_front()) || _run )
	{
		if( opt != NULL )
		{
			bool bQuery = true;
			if ( opt->id != 0 )
			{
				_MutexUniqueSql.enter();
				TUniqueSQLQuery::iterator iter = _UniqueSqlCount.find(opt->id);

				if ( iter != _UniqueSqlCount.end() )
				{
					if ( iter->second > 1 )
					{
						bQuery = false;
						iter->second -= 1;
					}
					else
					{
						_UniqueSqlCount.erase(iter);
					}
				}
				_MutexUniqueSql.leave();
			}

			delete opt;
			NLMISC::nlSleep(0);
		}
		else
		{
			NLMISC::nlSleep(5);
		}
	}
}



void test_thread()
{
    SaveThread   save_thread1;
    SaveThread   save_thread2;
    SaveThread   save_thread3;
    SaveThread   save_thread4;
    SaveThread   save_thread5;
    SaveThread   save_thread6;

    save_thread1.init();
    save_thread2.init();
    save_thread3.init();
    save_thread4.init();
    save_thread5.init();
    save_thread6.init();

    NLMISC::IThread* _thread = NULL;
    _thread = NLMISC::IThread::create (&save_thread1); _thread->start ();
    _thread = NLMISC::IThread::create (&save_thread2); _thread->start ();
    _thread = NLMISC::IThread::create (&save_thread3); _thread->start ();
    _thread = NLMISC::IThread::create (&save_thread4); _thread->start ();
    _thread = NLMISC::IThread::create (&save_thread5); _thread->start ();
    _thread = NLMISC::IThread::create (&save_thread6); _thread->start ();


    uint64 idx = 0;

    while(true)
    {
        ++idx;
        CSString output = "test string ";
        output<<idx;

        save_thread1.pushSQL(output.c_str(),0);
        save_thread2.pushSQL(output.c_str(),0);
        save_thread1.pushSQL(output.c_str(),0);
        save_thread2.pushSQL(output.c_str(),0);
        save_thread1.pushSQL(output.c_str(),0);
        save_thread2.pushSQL(output.c_str(),0);

        if ( idx % 100000 == 0 )
        {
            cout<<idx<<endl;
        }
        //nlSleep(0);
    }
}
