#ifndef GW1K_GW1KSETTINGS_H_
#define GW1K_GW1KSETTINGS_H_

#include <string>

namespace gw1k
{


class Gw1kSettings
{

public:

    static int init()
    {
        return 0;
    }

    /** arial.ttf */
    static std::string defaultFontName;

    /** -1 */
    static int defaultFontSize;

};


} // namespace gw1k

#endif // GW1K_GW1KSETTINGS_H_
