#include "i18n_def.h"
#include "nel/misc/string_conversion.h"
#include <cstdarg>

using namespace std;
using namespace NLMISC;

namespace I18N
{
    NL_BEGIN_STRING_CONVERSION_TABLE (TI18N)
        NL_STRING_CONVERSION_TABLE_ENTRY(LanguageName)
//        NL_STRING_CONVERSION_TABLE_ENTRY(UID)
    NL_END_STRING_CONVERSION_TABLE(TI18N, I18nConversion, UNKNOWN)

    TI18N toEnum(const std::string &str)
    {
        return I18nConversion.fromString(str);
    }

    const std::string& toString(TI18N i18n_str)
    {
        return I18nConversion.toString(i18n_str);
    }

    
    std::string	_gstring;
    const char* GetStr( const char* label )
    {
        _gstring = CI18N::get(label).toUtf8();
        return _gstring.c_str();
    }

    #define GET_BUFFER_MAX 512
    char _fstring[GET_BUFFER_MAX]={0};

    const char* Format( const char* format_ptr, ... )
    {
        va_list _args;
        va_start (_args, format_ptr);
        int _res = vsnprintf (_fstring, GET_BUFFER_MAX-1, format_ptr, _args);
        if (_res == -1 || _res == GET_BUFFER_MAX-1)
        {
            _fstring[GET_BUFFER_MAX-1] = '\0';
        }
        va_end (_args);

        return _fstring;
    }

}; // MSG_FORMAT

