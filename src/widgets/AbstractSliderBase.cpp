#include "widgets/AbstractSliderBase.h"

#include "Log.h"
#include "Math.h"

namespace gw1k
{


AbstractSliderBase::AbstractSliderBase(
    const Point& pos,
    const Point& size,
    const float range[2],
    RangeType rangeType,
    const char* colorScheme)
:   WiBox(pos, size, colorScheme),
    rangeType_(rangeType)
{
    if (range)
    {
        setRange(range);
    }
    else
    {
        float r[] = { 0.f, 1.f };
        setRange(r);
    }
}


AbstractSliderBase::~AbstractSliderBase()
{}


void
AbstractSliderBase::setRange(const float range[2])
{
    range_[0] = range[0];
    range_[1] = range[1];

    if (range_[0] > range_[1])
    {
        std::swap(range_[0], range_[1]);
        Log::warning("AbstractSliderBase", "Wrong order of range values");
    }
}


const float*
AbstractSliderBase::getRange() const
{
    return range_;
}


float
AbstractSliderBase::getMappedValue(float internalVal) const
{
    float d = range_[1] - range_[0];
    float v = 0.f;
    if (internalVal > 0.f)
    {
        switch(rangeType_)
        {
        case MAP_LINEAR:
            v = internalVal;
            break;
        case MAP_QUADRATIC:
            v = internalVal * internalVal;
            break;
        case MAP_CUBIC:
            v = internalVal * internalVal * internalVal;
            break;
        case MAP_SQRT:
            v = std::sqrt(internalVal);
            break;
        case MAP_CUBICRT:
            v = std::pow(internalVal, 1.f / 3.f);
            break;
        }
    }

    return v * d + range_[0];
}


float
AbstractSliderBase::getUnmappedValue(float rangeVal) const
{
    float d = range_[1] - range_[0];
    float v = 0.f;
    float r = (rangeVal - range_[0]) / d;

    if (r != 0.f)
    {
        switch(rangeType_)
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
