#include <string>
#include <stdio.h>
#include <iostream>

#include "nel/misc/config_file.h"
#include "nel/misc/path.h"
#include "game_share/utils.h"

#include "test_misc.h"
#include "thread/test_thread.h"
#include "test_sigslot.h"
#include <nel/misc/sstring.h>
#include <time.h>
#include "game_share/buf_fifo2.h"
#include "tools.h"

using namespace std;
using namespace NLMISC;

#include "nel/misc/eval_num_expr.h"
#include "nel/misc/random.h"

int main (int argc, char **argv)
{
	CApplicationContext myApplicationContext;
	CPath::addSearchPath(".");
	createDebug();

	cout<<CPath::getCurrentPath()<<endl;

    DEF::RPC_SESSION  session = 1 << 8;

    CBufFIFO2<uint> TestBuf;

    uint t0 = 0;

    TestBuf.init(10);
    TestBuf.push_back( &t0 );
    TestBuf.swap();
    TestBuf.pop_front();

    NLMISC::CRandom m_random ;

    m_random.srand( NLMISC::CTime::getSecondsSince1970() );

    m_random.rand( 6 );


	int startTime = (int)LocalTime.str2time("2015-4-7 3:0:0");
	int res = test_refresh_time( startTime, 18000 );

    test_callback();
    test_timer();
    test_ranking_min();
    //test_ranking();
    test_sigsolt();
    test_eval_expr();
    test_string_cow();
    test_protobuf();
    test_i18n();
    test_id_gen();
    test_udp();
	test_google_hash();
	test_dupstar();
    test_localtime();
    //test_thread();
    test_crash();
    test_hashset();
    test_set();




	return 0;
}
