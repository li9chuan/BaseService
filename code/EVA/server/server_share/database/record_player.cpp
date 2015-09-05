#include "record_player.h"
#include <nel/misc/time_nl.h>
#include <game_share/game_def.h>

void DB_RECORD::CRecordPlayer::reset()
{
    pid                 = 0;
    uid                 = 0;
    sex                 = MALE;
    level               = 1;
    exp                 = 0;
    money               = 0;
}
