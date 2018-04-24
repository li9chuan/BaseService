#include "msg_leaf.h"
#include "nel/misc/string_conversion.h"

using namespace std;
using namespace NLMISC;

namespace MSG_FORMAT
{
    NL_BEGIN_STRING_CONVERSION_TABLE (TMsgFormat)
        NL_STRING_CONVERSION_TABLE_ENTRY(f)
        NL_STRING_CONVERSION_TABLE_ENTRY(d)
        NL_STRING_CONVERSION_TABLE_ENTRY(b)
        NL_STRING_CONVERSION_TABLE_ENTRY(s)
        NL_STRING_CONVERSION_TABLE_ENTRY(s8)
        NL_STRING_CONVERSION_TABLE_ENTRY(s16)
        NL_STRING_CONVERSION_TABLE_ENTRY(s32)
        NL_STRING_CONVERSION_TABLE_ENTRY(s64)
        NL_STRING_CONVERSION_TABLE_ENTRY(u8)
        NL_STRING_CONVERSION_TABLE_ENTRY(u16)
        NL_STRING_CONVERSION_TABLE_ENTRY(u32)
        NL_STRING_CONVERSION_TABLE_ENTRY(u64)
        NL_STRING_CONVERSION_TABLE_ENTRY(UID)
        NL_STRING_CONVERSION_TABLE_ENTRY(PLS)
        NL_STRING_CONVERSION_TABLE_ENTRY(JSON)
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

