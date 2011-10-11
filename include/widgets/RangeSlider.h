#ifndef GW1K_RANGESLIDER_H_
#define GW1K_RANGESLIDER_H_

#include "WiBox.h"

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
class RangeSlider : public WiBox, public MouseListener
{

public:

    enum RangeType
    {
        MAP_CUBICRT, MAP_SQRT, MAP_LINEAR, MAP_QUADRATIC, MAP_CUBIC
    };

    RangeSlider(const Point& pos,
                const Point& size,
                const float range[2],
                RangeType rangeTye = MAP_LINEAR,
                const char* colorScheme = 0);

    virtual ~RangeSlider();

public:

    void setLValue(float v);

    float getLValue() const;

    void setRValue(float v);

    float getRValue() const;

    void setRange(const float[2]);

    const float* getRange() const;

    virtual void mouseMoved(MouseMovedEvent ev,
                    const Point& pos,
                    const Point& delta,
                    GuiObject* receiver);

    virtual void mouseClicked(MouseButton b, StateEvent ev, GuiObject* receiver);

    virtual void mouseWheeled(int delta, GuiObject* receiver);

    virtual void setColors(const char* colorScheme);

private:

    void updateRangeBar();

    void setHandlePos(WiBox* handle, const Point& newPos);

    void restoreConsistency();

    /** internalVal should from the range [0, 1]. */
    float getMappedValue(float internalVal) const;

    float getUnmappedValue(float rangeVal) const;

    void calculateValues();

private:

    RangeType rangeType_;

    int handleSize_;

    WiBox* lHandle_;

    WiBox* rHandle_;

    WiBox* rangeBar_;

    float lValue_;

    float rValue_;

    float range_[2];


};


} // namespace gw1k

#endif // GW1K_RANGESLIDER_H_
