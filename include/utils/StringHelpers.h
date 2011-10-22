#ifndef GW1K_STRINGHELPERS_H_
#define GW1K_STRINGHELPERS_H_

#include <sstream>
#include <string>

namespace gw1k
{


template<typename T>
inline std::string
toString(T arg)
{
    std::stringstream ss;
    ss << arg;
    return ss.str();
}


} // namespace gw1k

#endif // GW1K_STRINGHELPERS_H_
