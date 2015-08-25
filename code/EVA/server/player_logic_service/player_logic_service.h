#ifndef PLAYER_LOGIC_SERVICE_H
#define PLAYER_LOGIC_SERVICE_H

// we have to include windows.h because mysql.h uses it but not include it
#ifdef NL_OS_WINDOWS
#	define NOMINMAX
#	include <windows.h>
#endif

#include "nel/misc/types_nl.h"

#include "nel/misc/debug.h"
#include "nel/misc/config_file.h"
#include "nel/misc/displayer.h"
#include "nel/misc/log.h"

#include "nel/net/service.h"




#endif // PLAYER_LOGIC_SERVICE_H

/* End of player_logic_service.h */
