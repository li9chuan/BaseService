#ifndef SERVER_SHARED_MSG_LEAF_H
#define SERVER_SHARED_MSG_LEAF_H

#include "nel/misc/types_nl.h"

namespace MSG_FORMAT
{
	enum TMsgFormat
	{
        s16,
        u64,
		PID,
        UID,
        PLS,
        ProtoMsg,
		UNKNOWN
	};

	TMsgFormat toEnum(const std::string &str);
	const std::string& toString(TMsgFormat format);
}; // MSG_FORMAT

struct MsgLeaf
{
    std::string  msgname;
    std::vector<std::string>                  sendto;
    std::vector<MSG_FORMAT::TMsgFormat>       format;
    std::vector<std::string>                  format_msg;
    std::string                               description;
};

#endif // SERVER_SHARED_MSG_LEAF_H
/* End of msg_leaf.h */
