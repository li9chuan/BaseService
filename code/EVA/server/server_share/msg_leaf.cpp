#include "msg_leaf.h"
#include "nel/misc/string_conversion.h"

using namespace std;
using namespace NLMISC;

namespace MSG_FORMAT
{
    NL_BEGIN_STRING_CONVERSION_TABLE (TMsgFormat)
        NL_STRING_CONVERSION_TABLE_ENTRY(PID)
        NL_STRING_CONVERSION_TABLE_ENTRY(UID)
        NL_STRING_CONVERSION_TABLE_ENTRY(PLS)
//        NL_STRING_CONVERSION_TABLE_ENTRY(ProtoMsg)
    NL_END_STRING_CONVERSION_TABLE(TMsgFormat, MsgFormatConversion, UNKNOWN)

    TMsgFormat toEnum(const std::string &str)
    {
        return MsgFormatConversion.fromString(str);
    }

    const std::string& toString(TMsgFormat format)
    {
        return MsgFormatConversion.toString(format);
    }
}; // MSG_FORMAT

