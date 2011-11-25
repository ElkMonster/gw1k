#include "widgets/internal/AbstractSliderBase.h"

#include "Log.h"
#include "Math.h"

namespace gw1k
{


AbstractSliderBase::AbstractSliderBase(
    const Point& pos,
    const Point& size,
    const float range[2],
    MapType mapType,
    const char* colorScheme)
:   WiBox(pos, size, colorScheme),
    mapType_(mapType)
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
    return mapValue(internalVal, range_, mapType_);
}


float
AbstractSliderBase::getUnmappedValue(float rangeVal) const
{
    return unmapValue(rangeVal, range_, mapType_);
}


} // namespace gw1k
