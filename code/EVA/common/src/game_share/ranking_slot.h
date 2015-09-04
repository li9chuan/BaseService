#ifndef GAME_SHARD_RANKING_SLOT_H
#define GAME_SHARD_RANKING_SLOT_H

#include <vector>
#include <map>
#include <set>
#include <nel/misc/time_nl.h>
#include "game_def.h"

/** 排名槽管理器
 *	使用PID作为索引，SCORE作为排名依据，获取排名。
 *  按积分槽排名。
 *  分数槽小于1W时效率较高:         UpdateRanking<2ms 。
 *  分数槽小于10W时(Release):      UpdateRanking<10ms 。
 *
 * \author li9chuan
 * \date 2015-1
 */

class CRankingSlot
{
public:

    typedef  std::set<DEF::PID>     _TPIDs;
    typedef  std::vector<DEF::PID>  _TVctPIDs;

    struct _Ranking
    {
        DEF::RANKING        Ranking;
        _TPIDs              PIDs;

        _Ranking():Ranking(0){}
    };
    typedef  std::map<DEF::PID, DEF::SCORE>     TMapScore;
    typedef  std::map<DEF::SCORE, _Ranking>     TMapRanking;

    CRankingSlot():_UpdateSeconds(0),_PreUpdateSecond1970(0),_FastSortMax(0){}

	/**
	*   初始化排行处理逻辑
	*
	*	\param  update_seconds  调用 UpdateRanking() 的最小间隔时间。
	*	\param  fast_sort_num   SetScore时，在fast_sort_num前的排名会即时更新。
	*/ 
    void InitRanking( uint32 update_seconds, uint32 fast_sort_num )
    {
        _UpdateSeconds  = update_seconds;
        _FastSortMax    = fast_sort_num;
    }

	/**
	*   设置玩家积分
	*
	*	\param  nID
	*	\param  nScore
	*/
	void SetScore( const DEF::PID pid, const DEF::SCORE score )
	{
        DEF::RANKING nRankingNew = 0;

        ///  移除玩家旧排名
        TMapScore::iterator itscr = _MapScore.find( pid );

        if ( itscr!=_MapScore.end() )
        {
            DEF::SCORE old_score = itscr->second;

            if ( old_score==score )
            {
                return;
            }

            TMapRanking::iterator itrnk = _MapRanking.find( old_score );

            if ( itrnk!=_MapRanking.end() )
            {
                itrnk->second.PIDs.erase(pid);

                if ( itrnk->second.PIDs.empty() )
                {
                    _MapRanking.erase(itrnk);
                }
            }

			itscr->second = score;
        }
        else
        {
            _MapScore.insert( std::make_pair(pid,score) );
        }


        /// 插入玩家新排名
        TMapRanking::iterator itrnk = _MapRanking.find( score );

        if ( itrnk!=_MapRanking.end() )
        {
            /// 如果已存在积分排名，插入对应积分槽内。
            itrnk->second.PIDs.insert( pid );
            nRankingNew = itrnk->second.Ranking;
        }
        else
        {
            /// 如果没有输入的积分，新插入一条
            _Ranking  Rnk;
            Rnk.PIDs.insert(pid);

            std::pair<TMapRanking::iterator,bool> ret =  _MapRanking.insert( std::make_pair( score, Rnk ) );
			
            if ( ret.first != _MapRanking.begin() )
            {
                TMapRanking::iterator it_cur = ret.first;
                TMapRanking::iterator it_pre = --ret.first;

                nRankingNew = it_pre->second.Ranking + it_pre->second.PIDs.size();
                it_cur->second.Ranking = nRankingNew;
            }
            else
            {
                nRankingNew = 1;
                ret.first->second.Ranking = nRankingNew;
            }
        }

        ///    如果是前几名，那么即时排，不等update

        if ( nRankingNew <= _FastSortMax )
        {
            _TopRnkPIDs.clear();

            uint  rnk_idx = 0;
            TMapRanking::reverse_iterator itrnk       = _MapRanking.rbegin();
            TMapRanking::reverse_iterator itrnk_end   = _MapRanking.rend();

            if ( itrnk!=itrnk_end )
            {
                itrnk->second.Ranking = 1;

                _TPIDs::iterator  itpids     = itrnk->second.PIDs.begin();
                _TPIDs::iterator  itpids_end = itrnk->second.PIDs.end();

                while ( itpids!=itpids_end )
                {
                    _TopRnkPIDs.push_back( *itpids );

                    ++rnk_idx;
                    ++itpids;

                    if ( rnk_idx>_FastSortMax )
                    {
                        return;
                    }
                }

                ++itrnk;
            }

            while ( itrnk!=itrnk_end )
            {
                TMapRanking::reverse_iterator it_pre = itrnk;
                --it_pre;
                itrnk->second.Ranking = it_pre->second.Ranking + it_pre->second.PIDs.size();

                _TPIDs::iterator  itpids     = itrnk->second.PIDs.begin();
                _TPIDs::iterator  itpids_end = itrnk->second.PIDs.end();

                while ( itpids!=itpids_end )
                {
                    _TopRnkPIDs.push_back( *itpids );

                    ++rnk_idx;
                    ++itpids;

                    if ( rnk_idx>_FastSortMax )
                    {
                        return;
                    }
                }

                ++itrnk;
            }
        }
	}

	/**
	*   获取排名
	*
	*	\param  nID
	*	\return 排名从1开始，0表示没有找到此ID
	*/
	DEF::RANKING GetRanking( const DEF::SCORE score )
	{
        DEF::RANKING  rnk = 0;
        TMapRanking::iterator iter = _MapRanking.find(score);

        if ( iter!=_MapRanking.end() )
        {
            rnk = iter->second.Ranking;
        }

		return rnk;
	}

    void UpdateRanking( )
    {
        uint32 curr_seconds = NLMISC::CTime::getSecondsSince1970();

        if ( curr_seconds - _PreUpdateSecond1970 < _UpdateSeconds )
        {
            return;
        }

        _PreUpdateSecond1970 = curr_seconds;
        _ResortRanking();
    }

    DEF::SCORE GetScore( DEF::PID unRoleID )
    {
        std::map<DEF::PID, DEF::SCORE> ::iterator it = _MapScore.find( unRoleID );
        if ( it == _MapScore.end() ) {
            return 0;
        }

        return it->second;
    }

    std::vector<DEF::PID>&  GetTopRanking()
    {
        return _TopRnkPIDs;
    }

private:

    void    _ResortRanking()
    {
        TMapRanking::reverse_iterator itrnk       = _MapRanking.rbegin();
        TMapRanking::reverse_iterator itrnk_end   = _MapRanking.rend();

        if ( itrnk!=itrnk_end )
        {
            itrnk->second.Ranking = 1;
            ++itrnk;
        }

        while ( itrnk!=itrnk_end )
        {
            TMapRanking::reverse_iterator it_pre = itrnk;
            --it_pre;
            itrnk->second.Ranking = it_pre->second.Ranking + it_pre->second.PIDs.size();
            ++itrnk;
        }
    }


    _TVctPIDs     _TopRnkPIDs;

    TMapRanking     _MapRanking;
    TMapScore       _MapScore;

    uint32      _UpdateSeconds;
    uint32      _PreUpdateSecond1970;
    uint32      _FastSortMax;
};


#endif // GAME_SHARD_RANKING_SLOT_H

/* End of ranking_solt.h */
