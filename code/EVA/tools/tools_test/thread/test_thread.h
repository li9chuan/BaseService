#ifndef TOOLS_TEST_THREAD_H
#define TOOLS_TEST_THREAD_H

#include <nel/misc/thread.h>
#include <nel/misc/sstring.h>
#include <game_share/buf_fifo_ring.h>

class SaveThread : public NLMISC::IRunnable
{
public:
	typedef CHashMap<uint64, uint>		TUniqueSQLQuery;

	volatile bool  _run;

	void init();
	bool pushSQL( const char* sql, const uint64 id );

private:

	struct DBOpt
	{
		std::string  sql;
		uint64       id;
		DBOpt( const char* in_sql, const uint64 in_id ):sql(in_sql),id(in_id){}
	};


	NLMISC::CBufFIFORing<DBOpt>          _DBOptQueqe;

	NLMISC::CMutex                       _MutexUniqueSql;
	TUniqueSQLQuery                      _UniqueSqlCount;
	void run ();

};

void  test_thread();

#endif

