#include "test_misc.h"
#include <game_share/ranking_slot.h>
#include <game_share/ranking_change_near.h>
#include <nel/misc/random.h>
#include <nel/misc/time_nl.h>

using namespace std;
using namespace NLMISC;

CRankingChangeNear   Ranking;

void test_ranking()
{
    cout<<"---------------  test_ranking ----------------"<<endl;

    const uint16 SCORE_RANDOM_ADD = 100;

    NLMISC::CRandom   random;
 
    std::vector<uint>   m_vctScore;
    m_vctScore.resize(1000001);

    for ( uint i=1000000; i>0; --i )
    {
        m_vctScore[i] = (i/100) + random.rand(SCORE_RANDOM_ADD);;
    }

    uint32 start_time = CTime::getLocalTime();

    for ( uint i=1000000; i>0; --i )
    {
        Ranking.SetScore( i, m_vctScore[i] );
    }

    cout<<"cost time:"<<CTime::getLocalTime()-start_time<<endl;


    for ( uint i=1000000; i>0; --i )
    {
        m_vctScore[i] = m_vctScore[i]+random.rand(SCORE_RANDOM_ADD);
    }

    start_time = CTime::getLocalTime();

    for ( uint i=1000000; i>0; --i )
    {
        Ranking.SetScore( i, m_vctScore[i] );
    }

    cout<<"cost time:"<<CTime::getLocalTime()-start_time<<endl;

    system("pause");
}


CRankingSlot   rnk_min;

void test_ranking_min()
{
    cout<<"---------------  test_ranking_min ----------------"<<endl;

    rnk_min.InitRanking(0,50);
    const uint16 SCORE_RANDOM_ADD = 32767;

    NLMISC::CRandom   random;

    std::vector<uint>   m_vctScore;
    m_vctScore.resize(1000001);

    for ( uint i=1000000; i>0; --i )
    {
        m_vctScore[i] = random.rand(SCORE_RANDOM_ADD);
    }

    uint32 start_time = CTime::getLocalTime();

    for ( uint i=1000000; i>0; --i )
    {
        rnk_min.SetScore( i, m_vctScore[i] );
    }

    cout<<"cost time:"<<CTime::getLocalTime()-start_time<<endl;


    for ( uint i=1000000; i>0; --i )
    {
        m_vctScore[i] = random.rand(SCORE_RANDOM_ADD);
    }

    start_time = CTime::getLocalTime();

    for ( uint i=1000000; i>0; --i )
    {
        rnk_min.SetScore( i, m_vctScore[i] );
    }

    cout<<"cost time:"<<CTime::getLocalTime()-start_time<<endl;

    start_time = CTime::getLocalTime();

    for ( uint i=0; i<1; ++i )
        rnk_min.UpdateRanking();

    cout<<"cost time:"<<CTime::getLocalTime()-start_time<<endl;

    system("pause");
}


