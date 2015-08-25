#ifndef SERVER_SHARED_I18N_DEF_H
#define SERVER_SHARED_I18N_DEF_H

#include "nel/misc/types_nl.h"
#include "nel/misc/i18n.h"

namespace I18N
{
    enum TI18N
    {
        LanguageName,
        UNKNOWN
    };

    TI18N toEnum(const std::string &str);
    const std::string& toString(TI18N i18n_str);

    const char* GetStr( const char* label );
    const char* Format( const char* format_ptr, ... );
}; // I18N

#define FORMAT_P0( __format )  I18N::GetStr(__format)

#define FORMAT_P1( __format, __param )  I18N::Format( I18N::GetStr(__format)  , __param )



#endif // SERVER_SHARED_I18N_DEF_H
/* End of i18n_def.h */
