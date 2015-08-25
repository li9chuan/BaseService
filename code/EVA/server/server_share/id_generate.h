#ifndef SERVER_SHARED_ID_GENERATE_H
#define SERVER_SHARED_ID_GENERATE_H

#include <game_share/game_def.h>

/**
 *  ID生成器  
 *    生成52位GUID  lua可用
 *    TPlaceHolder  表示占位
 *    拼接服务器ID：   64-53   52-PlaceHolderService  PlaceHolderIncreaseID  PlaceHolderRegion-0
 *    不拼接服务器ID:  64-53   52-                    PlaceHolderIncreaseID  PlaceHolderRegion-0
 *
 *    
 * \author li9chuan@qq.com
 * \date 2013
 */

class CIDGenerate
{
public:

    CIDGenerate();

    /**
    *  ID生成初始化函数
    *  @param region 游戏大区ID。
    *  @param service_id 服务ID。
    *  @param current_id 当前已经达到的最大ID， 只接受比m_CurrentID大的数字。
    */
	///@{
	void init( uint region, uint8 service_id, uint64 current_id );
    void init( uint region, uint64 current_id );
    ///@}

	inline uint64 Generate()
	{
		++m_CurrentID;
		return ( m_CurrentID << PlaceHolderRegion ) | m_BaseCode;
	}

	//uint64 GetID( uint64 idx )
	//{
	//	return idx | m_BaseCode;
	//}

    inline uint64 GetIDX(uint64 id)	{	return id>>PlaceHolderRegion & 0xfffffffffLL;	}

private:
	uint64    m_BaseCode;
	uint64    m_CurrentID;

    ///   10位区ID   6位服务器ID   36位自增ID
    enum TPlaceHolder
    {
        PlaceHolderService      = 6,
        PlaceHolderRegion       = 10,
        PlaceHolderIncreaseID   = 36, 
    };
};

#endif
