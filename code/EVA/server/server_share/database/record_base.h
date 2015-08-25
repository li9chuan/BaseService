#ifndef record_base_h__
#define record_base_h__

#include <game_share/game_def.h>
#include <server_share/database/def_db.h>

enum TDBSaveOPT
{
    SAVE_OPT_NULL   = 0,
    SAVE_OPT_INSERT = 0x1,    /// 插入;
    SAVE_OPT_UPDATE = 0x2,    /// 更新;
    SAVE_OPT_DELETE = 0x4,    /// 删除;
};

namespace DB_RECORD {

    struct CRecordBase : public DBMsgData
    {
        CRecordBase( void )
            : _flag( SAVE_OPT_NULL )
        { }

        virtual ~CRecordBase()  {}

        void rest_flag( void )
        {
            _flag = SAVE_OPT_NULL;
        }

        void set_insert( void ) {
            _flag |= SAVE_OPT_INSERT;
        }

        void set_update( void ) 
        {
            _flag |= SAVE_OPT_UPDATE;
        }

        void set_delete( void ) {
            _flag |= SAVE_OPT_DELETE;
        }

        bool is_insert( void ) {
            return ( _flag & SAVE_OPT_INSERT );
        }

        bool is_update( void ) {
            return ( _flag & SAVE_OPT_UPDATE );
        }

        bool is_delete( void ) {
            return ( _flag & SAVE_OPT_DELETE );
        }

        void delete_charge_update( void )
        {
            _flag &= ~SAVE_OPT_DELETE;
            _flag |=  SAVE_OPT_UPDATE;
        }

        // 必须实现构造赋值,赋值;
        CRecordBase( const CRecordBase& _rhs )
        {
            _flag = _rhs._flag;
        }

        CRecordBase& operator = ( const CRecordBase& _rhs )
        {
            _flag |= _rhs._flag;
            return ( *this );
        }

    protected:
        uint8 _flag;
    };



}

#endif // record_base_h__