#ifndef NL_BASE64_H
#define NL_BASE64_H

#include "types_nl.h"

#include <string>

namespace NLMISC
{

std::string base64_encode( std::string const& );
std::string base64_decode( std::string const& );

}; // namespace NLMISC

#endif // NL_BASE64_H

/* End of base64.h */
