#include "nel/misc/types_nl.h"

#include <stdlib.h>
#include <stdio.h>
#include <stddef.h>
#include <math.h>
#include <time.h>
#include <assert.h>

#include <string>
#include <vector>
#include <list>
#include <map>
#include <set>
#include <algorithm>
#include <sstream>
#include <exception>
#include <utility>
#include <deque>
#include <limits>
#include <queue>
#include <memory>
#include <functional>

#include <nel/misc/common.h>
#include <nel/misc/debug.h>

#include <nel/misc/stream.h>
#include <nel/misc/time_nl.h>
#include <nel/misc/vector.h>
#include <nel/misc/command.h>
#include <nel/misc/config_file.h>
#include <nel/misc/variable.h>
#include <nel/misc/shared_memory.h>
#include <nel/misc/file.h>
#include <nel/misc/path.h>
#include <nel/misc/singleton.h>
#include <nel/misc/string_common.h>
#include <nel/misc/sstring.h>
#include <nel/misc/bit_mem_stream.h>
#include <nel/misc/o_xml.h>
#include <nel/misc/i_xml.h>

#include <nel/net/unified_network.h>
#include <nel/net/service.h>

#ifdef NL_OS_WINDOWS
#	ifndef NL_COMP_MINGW
#		define NOMINMAX
#	endif
#	include <WinSock2.h>
#	include <Windows.h>
#endif
