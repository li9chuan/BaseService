#ifndef PLS_LUA_EVENT_CALLBACK_H
#define PLS_LUA_EVENT_CALLBACK_H

#include <game_share/game_def.h>
#include <game_share/sigslot.h>

namespace PLS
{
class CPlayer;

class CLuaEventCallback : public sigslot::has_slots<>
{
public:

    void ReloadConfigFileEvent();
    void RegisterMoniterEvent();

    static CLuaEventCallback &getInstance()
    {
        if(!Instance)
        {
            Instance = new CLuaEventCallback();
        }
        return *Instance;
    }

private:

    /// event callback
    void CallbackLogin( CPlayer* pPlayer );

private:

    CLuaEventCallback()  { }
    static CLuaEventCallback *Instance;
};



}

#endif
