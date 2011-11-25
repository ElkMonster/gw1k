#ifndef GW1K_RANGESLIDER_H_
#define GW1K_RANGESLIDER_H_

#include "internal/AbstractSliderBase.h"
#include "../providers/ActionEventProvider.h"

namespace gw1k
{


class RangeSlider : public AbstractSliderBase, public ActionEventProvider
{

public:

    RangeSlider(const Point& pos,
                const Point& size,
                const float range[2],
                MapType mapType = MAP_LINEAR,
                const char* colorScheme = 0);

    virtual ~RangeSlider();

public:

    void setLValue(float v);

    float getLValue() const;

    void setRValue(float v);

    float getRValue() const;

    void getHandles(const WiBox*& lHandle, const WiBox*& rHandle) const;

    virtual const Point& setSize(float width, float height);

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

    void calculateValues();

    void calculatePositions(int& left, int& right) const;

private:

    int handleSize_;

    WiBox* lHandle_;

    WiBox* rHandle_;

    WiBox* rangeBar_;

    float lValue_;

    float rValue_;

};


} // namespace gw1k

#endif // GW1K_RANGESLIDER_H_
