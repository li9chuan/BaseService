#ifndef SERVER_SHARED_MSG_LEAF_H
#define SERVER_SHARED_MSG_LEAF_H

#include "nel/misc/types_nl.h"
#include <vector>
#include <string>

namespace MSG_FORMAT
{
	enum TMsgFormat
	{
        f,
        d,
        b,
        s,
        s8,
        s16,
        s32,
        s64,
        u8,
        u16,
        u32,
        u64,
        UID,
        PLS,
        ProtoMsg,
        JSON,
		UNKNOWN
	};

	TMsgFormat toEnum(const std::string &str);
	const std::string& toString(TMsgFormat format);
}; // MSG_FORMAT

struct MsgLeaf
{
    std::string  msgname;
    std::vector<std::string>                    sendto;
    std::vector<MSG_FORMAT::TMsgFormat>         format;
    std::vector<std::string>                    format_msg;
    std::string                                 description;
    bool                                        is_log_event;
};

#endif // SERVER_SHARED_MSG_LEAF_H
/* End of msg_leaf.h */
