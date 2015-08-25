#ifndef GAME_SHARE_RANKING_CHANGE_NEAR_H
#define GAME_SHARE_RANKING_CHANGE_NEAR_H

#include <vector>
#include <map>
#include "game_def.h"

/** 排名管理器
 *	使用PID作为索引，SCORE作为排名依据，获取排名。
 *  每次SetScore() 分数或排名变动跨越的人数小于100时适用。
 *
 * \author li9chuan
 * \date 2013-1-14
 */

class CRankingChangeNear
{
public:
	CRankingChangeNear ()
	{
		_vctScore.reserve(1024*128);
		_vctScore.resize(1);	//	占用第0位，排名从1开始
	}
	~CRankingChangeNear (){}

	/**
	*   设置玩家积分
	*
	*	\param  nID
	*	\param  nScore
	*	\return 排名从1开始，0表示异常
	*/
	DEF::RANKING SetScore( const DEF::PID nID, const DEF::SCORE nScore )
	{
		DEF::RANKING nRankingNew = GetIDRanking(nID);

		if ( nRankingNew == 0 )
		{
			DEF::RANKING nRanking = _addRankingBack( nID, nScore );
			nRankingNew = _upRanking( nRanking, nScore );
		}
		else
		{
			nRankingNew = _adjustRanking( nRankingNew, nScore );
		}

		return nRankingNew;
	}

	/**
	*   获取排名
	*
	*	\param  nID
	*	\return 排名从1开始，0表示没有找到此ID
	*/
	DEF::RANKING GetRanking( const DEF::PID nID )
	{
		return GetIDRanking(nID);
	}

	unsigned int GetMaxRow()
	{
		return _vctScore.size();
	}

	uint32 GetPage( const DEF::PID nID, const uint32 nPageRow )
	{
		uint32 page = 0;
		DEF::RANKING ranking = GetRanking( nID );
		if ( ranking != 0 && nPageRow != 0 )
		{
			page = ranking / nPageRow;
			if ( ranking % nPageRow != 0 )
			{
				++page;
			}
		}
		return page;
	}

	/// 获得分页
	bool GetPage( const uint32 nPage, const uint32 nPageRow, std::vector<DEF::PID>& vct )
	{
		vct.clear();
		if ( nPage == 0 )	{	return false;	}

		uint32 pageStartIdx = (nPage-1)*nPageRow+1;
		uint32 pageEndIdx   = pageStartIdx + nPageRow;

		if ( pageEndIdx > _vctScore.size() )
		{
			pageEndIdx = (uint32)_vctScore.size();
		}

		if ( pageEndIdx >= pageStartIdx )
		{
			uint32 count = pageEndIdx-pageStartIdx;

			if ( count > 100 )
			{
				return false;
			}

			for ( pageStartIdx; pageStartIdx<pageEndIdx; ++pageStartIdx )
			{
				vct.push_back(_vctScore[pageStartIdx].nID);
			}
			return true;
		}

		return false;
	}

private:

	/**
	*   调整排名
	*       中间会改变多个玩家的名次。
	*
	*	\param  nRanking  调整前，玩家的排名
	*   \param  nScore    玩家的分数
	*	\return 调整后，玩家的排名
	*/
	DEF::RANKING _adjustRanking( const DEF::RANKING nRanking, const DEF::SCORE nScore )
	{
		DEF::RANKING nNewRanking = nRanking;

		if ( nScore > _vctScore[nRanking].nScore )		//	如果积分增加了
		{
			nNewRanking = _upRanking( nRanking, nScore );
		} 
		else if( nScore < _vctScore[nRanking].nScore )	//	如果积分降低了
		{
			nNewRanking = _downRanking( nRanking, nScore );
		}

		return nNewRanking;
	}

	/**
	*   增加一条新的排名积分记录
	*       直接增加到尾部，最后一名。
	*
	*   \param  nScore    玩家的分数
	*	\return 加入后，玩家的排名
	*/
	DEF::RANKING _addRankingBack( const DEF::PID nID, const DEF::SCORE nScore )
	{
		SPlayerInfo PlayerInfo;
		PlayerInfo.nScore   = nScore;
		PlayerInfo.nID      = nID;

		_vctScore.push_back(PlayerInfo);
        return SetIDRanking( nID, _vctScore.size()-1 );
	}

	void _updateMapRanking( const DEF::RANKING nRanking )
	{
        SetIDRanking( _vctScore[nRanking].nID, nRanking );
	}

	DEF::RANKING _upRanking( const DEF::RANKING nOldRanking, const DEF::SCORE nScore )
	{
		DEF::RANKING nNewRanking = nOldRanking;

		/// 缓存
		SPlayerInfo PlayerInfo;
		PlayerInfo.nID = _vctScore[nOldRanking].nID;
		PlayerInfo.nScore = nScore;

		/// 排名上移
		for ( nNewRanking; nNewRanking>1; --nNewRanking )
		{
			if (_vctScore[nNewRanking-1].nScore<nScore )
			{
				_vctScore[nNewRanking] = _vctScore[nNewRanking-1];
				_updateMapRanking(nNewRanking);
			}
			else
			{
				_vctScore[nNewRanking] = PlayerInfo;
				_updateMapRanking(nNewRanking);
				break;
			}
		}

		if ( nNewRanking == 1 )
		{
			_vctScore[nNewRanking] = PlayerInfo;
			_updateMapRanking(nNewRanking);
		}

		return nNewRanking;
	}

	DEF::RANKING _downRanking( const DEF::RANKING nOldRanking, const DEF::SCORE nScore )
	{
		DEF::RANKING nNewRanking = nOldRanking;

		/// 缓存
		SPlayerInfo PlayerInfo;
		PlayerInfo.nID = _vctScore[nOldRanking].nID;
		PlayerInfo.nScore = nScore;

		/// 排名下移
		for ( nNewRanking; nNewRanking<DEF::RANKING(_vctScore.size()-1); ++nNewRanking )
		{
			if (_vctScore[nNewRanking+1].nScore>nScore )
			{
				_vctScore[nNewRanking] = _vctScore[nNewRanking+1];
				_updateMapRanking(nNewRanking);
			}
			else
			{
				_vctScore[nNewRanking] = PlayerInfo;
				_updateMapRanking(nNewRanking);
				break;
			}
		}

		if ( nNewRanking == _vctScore.size()-1 )
		{
			_vctScore[nNewRanking] = PlayerInfo;
			_updateMapRanking(nNewRanking);
		}

		return nNewRanking;
	}

private:

    struct HashPIDRnk
    {
        DEF::PID  pid;
        DEF::RANKING   ranking;
        HashPIDRnk():pid(0)
        { }
    };

    struct SPlayerInfo
    {
        DEF::SCORE       nScore;
        DEF::PID    nID;

//        HashPIDRnk  pRnk;
    };

    std::vector<SPlayerInfo>            _vctScore;

    enum  _THashEnum
    {
        HashDepthMask = 0x1,
        HashDepth     = HashDepthMask+1,

        HashSpaceMask = 0xFFFFF,            //  100W
        HashSpaceMax  = HashSpaceMask+1,
    };

    HashPIDRnk   _HashIDRanking[HashSpaceMax];                      ///  一级哈希空间

    typedef std::map<DEF::PID, DEF::RANKING>     TIDRanking;
    TIDRanking                              _HashRankingSecond;     ///  二级哈希空间

    ///  return hash idx
    inline uint HashFunctionPID( DEF::PID pid )
    {
        //return (pid>>10) & HashSpaceMask;
        return (pid) & HashSpaceMask;
    }

    DEF::RANKING SetIDRanking( DEF::PID pid, DEF::RANKING ranking )
    {
        uint idx = HashFunctionPID(pid);

        if ( _HashIDRanking[idx].pid==pid )
        {
            _HashIDRanking[idx].ranking = ranking;
        }
        else if ( _HashIDRanking[idx].pid==0 )
        {
            _HashIDRanking[idx].pid     = pid;
            _HashIDRanking[idx].ranking = ranking;
        }
        else
        {
            TIDRanking::iterator iter = _HashRankingSecond.find(pid);

            if ( iter!=_HashRankingSecond.end() )
            {
                iter->second = ranking;
            }
            else
            {
                _HashRankingSecond.insert( std::make_pair(pid, ranking) );
            }
        }

        return ranking;
    }

    DEF::RANKING GetIDRanking( DEF::PID pid )
    {
        DEF::RANKING ranking = 0;
        uint idx = HashFunctionPID(pid);

        if ( _HashIDRanking[idx].pid==pid )
        {
            ranking = _HashIDRanking[idx].ranking;
        }
        else if ( _HashIDRanking[idx].pid==0 )
        {
        }
        else
        {
            TIDRanking::iterator iter = _HashRankingSecond.find(pid);

            if ( iter!=_HashRankingSecond.end() )
            {
                ranking = iter->second;
            }
        }

        return ranking;
    }
};


#endif // GAME_SHARE_RANKING_CHANGE_NEAR_H

/* End of ranking_change_near.h */
