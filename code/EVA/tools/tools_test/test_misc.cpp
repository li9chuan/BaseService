#include "test_misc.h"
#include <iostream>
#include <ctime>
#include <game_share/tools.h>
#include <game_share/bit_set_ext2.h>
#include <game_share/eval_expression.h>
#include <nel/misc/random.h>
#include <nel/misc/sstring.h>


using namespace std;
using namespace NLMISC;


int test_refresh_time( int start_time/*开启时间 单位:秒*/ , int takt_time/*间隔时间 单位:秒*/ )
{
	int curr_time = (int)CTime::getSecondsSince1970();

	int res = curr_time +  ( takt_time - (curr_time - start_time) % takt_time );

	return res;
}


void test_localtime()
{
    cout<<"---------------  test_localtime ----------------"<<endl;
    cout<<time(0)<<endl;

    char cstime[25];
    tm tms = LocalTime.gettime();
    cout<<LocalTime.printtime( tms )<<endl;

    cout<<LocalTime.mktime( tms.tm_year,tms.tm_mon,tms.tm_mday, tms.tm_hour,tms.tm_min,tms.tm_sec )<<endl;


    /////////
    //cout<<"---------------  test_localtime   TimeOPT----------------"<<endl;
    //uint time = LocalTime.gettime( TimeOPT(NEXT_DAY,5) );
    //cout<< LocalTime.printtime( time ) <<endl;

    //vector<TimeOPT> vct_opt;
    //LocalTime.gettime( vct_opt );
}

#include <game_share/proto_msg/msg_player.pb.h>
#include <game_share/proto_msg/msg_c2s.pb.h>


google::protobuf::Message* createMessage(const std::string& typeName)
{
    google::protobuf::Message* message = NULL;
    const google::protobuf::Descriptor* descriptor = google::protobuf::DescriptorPool::generated_pool()->FindMessageTypeByName(typeName);
    if (descriptor)
    {
        const google::protobuf::Message* prototype = google::protobuf::MessageFactory::generated_factory()->GetPrototype(descriptor);
        if (prototype)
        {
            message = prototype->New();
        }
    }
    return message;
}

void test_protobuf()
{
    google::protobuf::Message* pMessage = createMessage("MsgPlayerInfo");

    if ( pMessage!=NULL )
    {
        delete pMessage;
    }

    MsgPlayerInfo  msg_player_info;
}

inline uint64 GetIDX(uint64 id)	{	return id>>10 & 0xfffffffffLL;	}

void test_id_gen()
{
    cout<<"---------------  test_id_gen ----------------"<<endl;
    uint16 region = 0xfff;
    uint8  service_id = 0x3f;

    uint64    m_BaseCode;
    uint64    m_CurrentID = 0x111111111;

    m_BaseCode  = service_id;
    m_BaseCode  = m_BaseCode << (46);
    m_BaseCode |= region & 0x3ff;

    m_CurrentID <<= 10;
    m_BaseCode  = m_CurrentID | m_BaseCode;

    m_CurrentID = GetIDX(m_BaseCode);

}

void test_string_cow()
{
    cout<<"---------------  test_string_cow ----------------"<<endl;
    char* str = "123456";
    std::string  c = str;

    printf("%llu\n", (uint64)str);
    printf("%llu\n", (uint64)c.data());

    std::string  a = "123456";
    std::string  b = a;

    printf("string1 %llu\n", (uint64)a.data());
    printf("string2 %llu\n", (uint64)b.data());

    CSString  str1 = "123456";
    CSString  str2 = str1;

    printf("CSString1 %llu\n", (uint64)str1.data());
    printf("CSString2 %llu\n", (uint64)str2.data());
}

void test_crash()
{
    cout<<"---------------  test_crash ----------------"<<endl;
    int* p = 0;
    *p = 0;
}

#include "game_share/timer.h"

class CTestTimerEvent : public CTimerEvent
{
public:

    void timerCallback(CTimer *owner)
    {
        static int i = 0;
        cout<<"timerCallback:"<<i++<<endl;
        owner->setTime(CTime::getSecondsSince1970(),this);
        //delete owner;
    }

    virtual ~CTestTimerEvent()
    { 
        cout<<"~CTestTimerEvent"<<endl;
    }

private:

};

#include "nel/net/service.h"

void test_timer()
{
    cout<<"---------------  test_timer ----------------"<<endl;

    TimerManager->init();

    CTimer*  pTestTimer = new CTimer();
    pTestTimer->setTime( CTime::getSecondsSince1970()+1, new CTestTimerEvent(),1 );
    //delete pTestTimer;   如果不删，有内存泄露。 回调中getOwner()==NULL。

    while (true)
    {
        cout<<NL_GET_INSTANCE_COUNTER(CTimerEvent)<<endl;
        TimerManager->tickUpdate();
        NLMISC::nlSleep(TimerManager->getUpdateTimeout());
    }

}

void test_dupstar()
{
    cout<<"---------------  test_dupstar ----------------"<<endl;

    CRandom  random;
    random.srand(time(0));

    CBitSetExt2  dup_star;

    for ( uint i=0; i<512; ++i )
    {
        //     dup_star.SetValue( i, random.rand(3) );
    }

    dup_star.SetValue( 4, 3 );
    dup_star.SetValue( 30, 3 );
    dup_star.SetValue( 510, 3 );
    dup_star.SetValue( 512, 3 );


    string s1 = dup_star.GetArrayStr();
    test_zlib( s1 );


    dup_star.SetValue( 501, 2 );
    string s2 = dup_star.GetArrayStr();

    if ( s1 == s2 )
    {
        cout<<"s1 == s2"<<endl;
    }

    if ( dup_star.GetValue(4)==3 )
    {
        cout<<dup_star.toString()<<endl;
    }
}

void test_eval_expr()
{
    CSString    str = "int(($a/5+$b)*10)";
    CEvalExpr   Expr;
    double res = Expr.Eval( str, 123456.123456789, 22 );

    cout<<res<<endl;
}

const uint32 PLYNUM = 10000000;

typedef std::set< std::string > TestSetType;

typedef CHashSet< std::string > TestHashSetType;

void test_hashset()
{
    TestSetType m_hashSet;

    sint64 lasttime = NLMISC::CTime::getLocalTime();

    NLMISC::CRandom m_random;

    m_random.srand( lasttime );

    for (int i = 0; i < PLYNUM; ++i)
    {
        sint nindex = m_random.rand( 100 );
        char str[64] = { '\0' };
        sprintf_s( str , sizeof( str ) , "%d" , nindex );
        m_hashSet.insert(str);
    }

    sint64 Curtime = NLMISC::CTime::getLocalTime();

    printf( " test_set time = %ull " , Curtime -  lasttime );
}

void test_set()
{
    TestHashSetType m_Set;

    sint64 lasttime = NLMISC::CTime::getLocalTime();
    NLMISC::CRandom m_random;

    m_random.srand( lasttime );

    for (int i = 0; i < PLYNUM; ++i)
    {
        sint nindex = m_random.rand( 100 );
        char str[64] = { '\0' };
        sprintf_s( str , sizeof( str ) , "%d" , nindex );
        m_Set.insert(str);
    }

    sint64 Curtime = NLMISC::CTime::getLocalTime();

    printf( " test_set time = %ull " , Curtime -  lasttime );
}



#include <nel/misc/callback.h>
#include <nel/misc/debug.h>

class CTestClass
{
public:
    void helloWorld(int y)
    {
        nldebug("Method call: %i, %i", y, x);
    }
    int x;
};

void functionCall(int i)
{
    nldebug("Function call: %i", i);
}

typedef NLMISC::CCallback<void, int> TCallbackType;

void test_callback()
{
    CTestClass tc;
    tc.x = 42;

    TCallbackType cbMethod = TCallbackType(&tc, &CTestClass::helloWorld);
    TCallbackType cbFunction = TCallbackType(functionCall);
    cbMethod(100);
    cbFunction(99);

    //getchar();
}

