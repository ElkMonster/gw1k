#ifndef GW1K_FLOATMAPPER_H_
#define GW1K_FLOATMAPPER_H_

namespace gw1k
{


enum MapType
{
    MAP_CUBICRT, MAP_SQRT, MAP_LINEAR, MAP_QUADRATIC, MAP_CUBIC
};

float mapValue(float nonMappedVal, const float range[2], MapType mapping);

float unmapValue(float mappedVal, const float range[2], MapType mapping);


} // namespace gw1k

#endif // GW1K_FLOATMAPPER_H_
