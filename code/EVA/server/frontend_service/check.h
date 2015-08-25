#ifndef FES_CHECK_H
#define FES_CHECK_H

#include <game_share/game_def.h>
#include <game_share/next_session.h>

namespace FES
{

class  CMsgCheck
{
public:
    CMsgCheck():_PreMaxIndex(0)
    {}

    inline void     SetSeed( sint32 seed );
    inline bool     Check( uint32 index, uint32 check );

    enum
    {
        CHECK_SCOPE_MASK    = 0x3f,
        CHECK_SCOPE         = CHECK_SCOPE_MASK+1,
        CHECK_STEP_MAX      = 0x3ff,
        INVALID_CHECK       = 0xffff,
    };

private:

    inline void    SetCheck( uint index );

    uint32                  _PreMaxIndex;
    uint16                  _CheckScope[CHECK_SCOPE];
    NLMISC::CNextSession    _NextSession;
};

void CMsgCheck::SetSeed( sint32 seed )
{
    _NextSession.set_seed(seed);
    _PreMaxIndex = CHECK_SCOPE_MASK;

    for ( uint i=0; i<CHECK_SCOPE; ++i )
    {
        _CheckScope[i] = _NextSession.next();
    }
}

bool CMsgCheck::Check( uint32 index, uint32 check )
{
    SetCheck( index );

    if ( index <= _PreMaxIndex )
    {
        uint32  read_idx = index&CHECK_SCOPE_MASK;
        
        if ( _CheckScope[read_idx]!=INVALID_CHECK )
        {
            if ( _CheckScope[read_idx] == check )
            {
                _CheckScope[read_idx] = INVALID_CHECK;
                return true;
            }
        }
    }

    return false;
}

void CMsgCheck::SetCheck( uint32 index )
{
    if ( index > _PreMaxIndex )
    {
        if ( index-_PreMaxIndex > CHECK_STEP_MAX )
        {
            return;
        }

        while ( _PreMaxIndex<index )
        {
            ++_PreMaxIndex;
            uint32 next_write = _PreMaxIndex&CHECK_SCOPE_MASK;
            _CheckScope[next_write] = _NextSession.next();
        }
    }
}

}



#endif
