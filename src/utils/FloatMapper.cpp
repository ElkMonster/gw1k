#include "utils/FloatMapper.h"

#include "Math.h"

namespace gw1k
{

float
mapValue(float nonMappedVal, const float range[2], MapType mapping)
{
    float d = range[1] - range[0];
    float v = 0.f;
    if (nonMappedVal > 0.f)
    {
        switch(mapping)
        {
        case MAP_LINEAR:
            v = nonMappedVal;
            break;
        case MAP_QUADRATIC:
            v = nonMappedVal * nonMappedVal;
            break;
        case MAP_CUBIC:
            v = nonMappedVal * nonMappedVal * nonMappedVal;
            break;
        case MAP_SQRT:
            v = std::sqrt(nonMappedVal);
            break;
        case MAP_CUBICRT:
            v = std::pow(nonMappedVal, 1.f / 3.f);
            break;
        }
    }

    return v * d + range[0];
}


float
unmapValue(float mappedVal, const float range[2], MapType mapping)
{
    float d = range[1] - range[0];
    float v = 0.f;
    float r = (mappedVal - range[0]) / d;

    if (r != 0.f)
    {
        switch(mapping)
        {
        case MAP_LINEAR:
            v = r;
            break;
        case MAP_QUADRATIC:
            v = sqrt(r);
            break;
        case MAP_CUBIC:
            v = pow(r, 1.f / 3.f);
            break;
        case MAP_SQRT:
            v = r * r;
            break;
        case MAP_CUBICRT:
            v = r * r * r;
            break;
        }
    }

    return v;
}

} // namespace gw1k
