// Ryzom - MMORPG Framework <http://dev.ryzom.com/projects/ryzom/>
// Copyright (C) 2010  Winch Gate Property Limited
//
// This program is free software: you can redistribute it and/or modify
// it under the terms of the GNU Affero General Public License as
// published by the Free Software Foundation, either version 3 of the
// License, or (at your option) any later version.
//
// This program is distributed in the hope that it will be useful,
// but WITHOUT ANY WARRANTY; without even the implied warranty of
// MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE.  See the
// GNU Affero General Public License for more details.
//
// You should have received a copy of the GNU Affero General Public License
// along with this program.  If not, see <http://www.gnu.org/licenses/>.



#ifndef NL_CLIENT_HOST_H
#define NL_CLIENT_HOST_H

#include "nel/misc/types_nl.h"
#include "nel/misc/vector.h"
#include "nel/misc/time_nl.h"
#include "nel/net/inet_address.h"
#include "nel/net/login_cookie.h"

#include "fe_types.h"
#include "fe_receive_task.h"

#include <vector>
#include <deque>


const uint32 FirstClientId = 1;
const uint16 InvalidClientId = 0xFFFF;

namespace NLNET
{
	class CUdpSock;
};


/// Get string for association state
const char *associationStateToString( uint8 as );


class CClientHost
{
public:

	void				disconnect() {  }
	bool				isDisconnected() { return true; }

private:

};


#endif // NL_CLIENT_HOST_H

/* End of client_host.h */
