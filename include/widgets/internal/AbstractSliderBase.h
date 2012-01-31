#ifndef GW1K_ABSTRACTSLIDERBASE_H_
#define GW1K_ABSTRACTSLIDERBASE_H_

#include "../WiBox.h"

#include "../../utils/FloatMapper.h"

namespace gw1k
{


/**
 * The range internally used is always [0, 1]. Values from this range are mapped
 * to the given range at the moment they are retrieved via get{L|R}Value().
 * External range: [A, B]
 *  0..1
 *
 *  |  |  x = { x^(1/3) | sqrt(x) | x | x^2 | x^3 }
 *  v  v
 *
 *  0..1
 *
 *  |  |  *(B-A)
 *  v  v
 *
 *  0..(B-A)
 *
 *  |  |  +A
 *  v  v
 *
 *  A..B
 */
class AbstractSliderBase : public WiBox
{

public:

    AbstractSliderBase(const Point& pos,
                       const Point& size,
                       const float range[2],
                       MapType mapType = MAP_LINEAR,
                       const char* colorScheme = 0);

    virtual ~AbstractSliderBase();

public:

    void setRange(const float[2]);

    const float* getRange() const;

protected:

    /** internalVal should be from the range [0, 1]. */
    float getMappedValue(float internalVal) const;

    float getUnmappedValue(float rangeVal) const;

protected:

    MapType mapType_;

    float range_[2];

};


} // namespace gw1k

#endif // GW1K_ABSTRACTSLIDERBASE_H_
