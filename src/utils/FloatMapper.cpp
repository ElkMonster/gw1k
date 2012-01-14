#include "utils/FloatMapper.h"

#include "Math.h"

namespace gw1k
{

float
mapValue(float inputVal, const float range[2], MapType mapping)
{
    float d = range[1] - range[0];
    float v = 0.f;
    if (inputVal > 0.f)
    {
        switch(mapping)
        {
        case MAP_LINEAR:
            v = inputVal;
            break;
        case MAP_QUADRATIC:
            v = inputVal * inputVal;
            break;
        case MAP_CUBIC:
            v = inputVal * inputVal * inputVal;
            break;
        case MAP_SQRT:
            v = std::sqrt(inputVal);
            break;
        case MAP_CUBICRT:
            v = std::pow(inputVal, 1.f / 3.f);
            break;
        }
    }

    return v * d + range[0];
}


float
unmapValue(float mappedVal, const float range[2], MapType mapping)
{
    float d = range[1] - range[0];
    if (d == 0.f)
    {
        // There is only one value to unmap to
        return range[0];
    }
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
