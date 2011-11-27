#ifndef GW1K_FLOATMAPPER_H_
#define GW1K_FLOATMAPPER_H_

namespace gw1k
{


/** Describes different types of mapping. This should be self-explanatory. */
enum MapType
{
    MAP_CUBICRT, MAP_SQRT, MAP_LINEAR, MAP_QUADRATIC, MAP_CUBIC
};


/**
 * Maps an input value of the range [0,1] to a target range given by the range
 * parameter, and uses the given mapping type to modify the input value before.
 * Examples:
 * - inputVal = 0.5, range = [10, 50], mapping = linear -> result = 30
 *   (0.5 * 40 + 10)
 * - inputVal = 0.5, range = [10, 50], mapping = quadratic -> result = 20
 *   (0.5^2 * 40 + 10)
 */
float mapValue(float inputVal, const float range[2], MapType mapping);


/**
 * Gets the input value of the range [0,1] that corresponds to mappedVal. This
 * is the inverse operation of mapValue().
 */
float unmapValue(float mappedVal, const float range[2], MapType mapping);


} // namespace gw1k

#endif // GW1K_FLOATMAPPER_H_
