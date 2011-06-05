#ifndef GW1K_MATH_H_
#define GW1K_MATH_H_

#include <cmath>

namespace gw1k
{


inline int round_pos(float f)
{
    return std::floor(f + 0.5f);
}


inline bool is_fraction(float f)
{
    return (f > 0.f) && (f < 1.f);
}


} // namespace gw1k

#endif // GW1K_MATH_H_
