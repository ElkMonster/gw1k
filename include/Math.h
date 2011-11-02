#ifndef GW1K_MATH_H_
#define GW1K_MATH_H_

#include <cmath>

namespace gw1k
{


/**
 * Rounds to the integer closest to f.
 * round(1.5) -> 2
 * round(-1.5) -> -2
 */
inline int round(float f)
{
    return (f < 0.0f) ? std::ceil(f - 0.5f) : std::floor(f + 0.5f);
}


/**
 * Rounds to the integer closest to f. f should be positive. For negative f, the
 * results may not be as expected.
 * round_pos(1.5) -> 2
 * round_pos(-1.5) -> -1
 */
inline int round_pos(float f)
{
    return std::floor(f + 0.5f);
}


/**
 * Returns whether f fits in the range (0.0f, 1.0f) (not including interval
 * bounds).
 */
inline bool is_fraction(float f)
{
    return (f > 0.f) && (f < 1.f);
}


} // namespace gw1k

#endif // GW1K_MATH_H_
