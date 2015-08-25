#ifndef SERVER_SHARED_MSG_EGS
#define SERVER_SHARED_MSG_EGS

#include <game_share/game_def.h>
#include <nel/net/unified_network.h>
#include <server_share/database/db_logic_chunk.h>
#include <server_share/database/db_egs_chunk.h>

namespace MSG_STR{

    /// 同步玩家基础信息;
    struct CPlayerBaseData
    {
        DEF::UID uid ;
        DEF::PID pid ;
        uint16 Level;
        uint16 State;
        std::string      NickName;
        DEF::HERO_ID     LeaderHeroID;
        DEF::TEMPLATE_ID LeaderTemplateID;
        uint32           LastOnlineTime;
        DEF::SCORE       nPvPScore;

        EGS_RECORD::CDBLogicPlayer basePlayer;

        CPlayerBaseData( void )
        { }

        void serial( NLMISC::IStream & stream )
        {
            stream.serial( uid );
            stream.serial( pid );
            stream.serial( Level );
            stream.serial( State );
            stream.serial( NickName );
            stream.serial( basePlayer );
            stream.serial( LeaderHeroID );
            stream.serial( LeaderTemplateID );
            stream.serial( LastOnlineTime );
            stream.serial( nPvPScore );
        }
    };

    /// 请求城堡数据;
    struct QueryCaseleData
    {
        DEF::PID             unSelfID;
        uint8                unState;
        std::set< DEF::PID > sMathchList;

        QueryCaseleData( void )
            : unSelfID( 0 )
            , unState( 0 )
        {
            sMathchList.clear();
        }
        void serial( NLMISC::IStream& stream )
        {
            stream.serial( unSelfID );
            stream.serial( unState );
            stream.serialCont( sMathchList );
        }
    };

    /// 城堡PVP战斗数据;
    struct CastleDefenseData
    {
        DEF::PID        role_id;
        uint32          castle_id;
        DEF::MONEY      money;
        DEF::PVP_SCORE  pvp_score;
        DEF::SOUL       soul;

        DB_RECORD::TRecordGuard      guard_data;
        DB_RECORD::TRecordCastleData castle_data;
        DB_RECORD::TRecordGrimoire   grimoire_data;

        CastleDefenseData( void )
        : role_id ( 0 ) 
        , castle_id ( 0 )
        , money ( 0 )
        , pvp_score( 0 )
        , soul( 0 )
        {
            guard_data.clear();
            castle_data.clear();
            grimoire_data.clear();
        };

       ~CastleDefenseData( void ) {};

        void serial( NLMISC::IStream & stream )
        {
            stream.serial( role_id );
            stream.serial( castle_id );
            stream.serial( money );
            stream.serial( soul );
            stream.serialCont( guard_data );
            stream.serialCont( castle_data );
            stream.serialCont( grimoire_data );
        }
    };

    struct CastleDefenseList
    {
        DEF::PID    unSelfID;
        DEF::MONEY  nSelfmoney;
        DEF::SOUL   nSelfsoul;
        uint8       unSelfState;
        std::vector< CastleDefenseData > sCastleInfo;

        CastleDefenseList( void )
            : nSelfmoney( 0 )
            , unSelfID( 0 )
            , nSelfsoul( 0 )
            , unSelfState( 0 )
        {
            sCastleInfo.clear();
        }
        ~CastleDefenseList( void ){}

        void serial( NLMISC::IStream& stream )
        {
            stream.serial( unSelfID );
            stream.serial( nSelfmoney );
            stream.serial( nSelfsoul );
            stream.serial( unSelfState );
            stream.serialCont( sCastleInfo );
        }
    };

    /// 竞技场战斗数据;
    struct CArenaPvPData
    {
        DEF::PID               role_id;
        DB_RECORD::TRecordHero hero_data;

        CArenaPvPData( void ){};
       ~CArenaPvPData( void ){};

        void serial( NLMISC::IStream& stream )
        {
            stream.serial( role_id );
            stream.serialCont( hero_data );
        }
    };

    /// 竞技场战斗记录;
    struct CArenaFightLog
    {
        DEF::PID       unRoleID;
        bool           bIsWin;
        int            nRanking;
        uint           unState;

        CArenaFightLog( void )
            : unRoleID( 0 )
            , bIsWin( false )
            , nRanking( 0 )
            , unState( 0 )
        { }

        ~CArenaFightLog( void )
        { }
    };

    /// 城堡战斗记录;
    struct CCastleFightLog
    {
        DEF::PID        unRoleID;
        bool            bIsWin;
        DEF::MONEY      nMoney;
        DEF::PVP_SCORE  nPvPScore;
        DEF::SOUL       nSoul;
        uint8           unState;

        CCastleFightLog( void )
            : unRoleID( 0 )
            , bIsWin( false )
            , nMoney( 0 )
            , nPvPScore( 0 )
            , unState( 0 )
            , nSoul( 0 )
        { }

        ~CCastleFightLog( void )
        { }
    };

    /// 竞技场每日奖励;
    struct CPvPAwardRanking : public DBMsgData
    {
        uint32   unRanking;
        DEF::PID unRoleID;
        uint16   unLevel;
        uint32   unLastTime;

        CPvPAwardRanking( void )
            : unRanking( 0 )
            , unRoleID ( 0 )
            , unLevel( 0 )
            , unLastTime( 0 )
        { }

        ~CPvPAwardRanking()
        { }

        void serial( NLMISC::IStream& stream )
        {
            stream.serial( unRanking );
            stream.serial( unRoleID );
            stream.serial( unLevel );
            stream.serial( unLastTime );
        }
    };

    struct CFightCount
    {
        uint8     unFightCount;
        uint32    unRefreshTime;
        bool      bIsFirst;

        CFightCount( void )
            : unFightCount( 10 )
            , unRefreshTime( 0 )
            , bIsFirst( true )
        { }

        void AddFightCount( void ){
            unFightCount--;
        }

        void ResetFightCount( void ) {
            unFightCount = 10;
        }

        void ResetFightTime( uint32 unTime ) {
            unRefreshTime = unTime;
        }
    };


}

#endif // SERVER_SHARED_MSG_EGS;