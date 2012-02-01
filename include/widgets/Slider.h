#ifndef GW1K_SLIDER_H_
#define GW1K_SLIDER_H_

#include "WiBox.h"
#include "internal/AbstractSliderBase.h"
#include "../providers/ActionEventProvider.h"
#include "../listeners/MouseListenerImpl.h"
#include "../listeners/DraggedListener.h"

namespace gw1k
{


class Slider : public AbstractSliderBase, public ActionEventProvider,
    public MouseListenerImpl, public DraggedListener
{

public:

    /**
     * Creates a Slider with a default range of [0, 1] and linear mapping.
     */
    Slider(const Point& pos,
           const Point& size,
           bool vertical = false,
           const char* colorScheme = 0);

    /**
     * Creates a Slider with the given range and settings.
     */
    Slider(const Point& pos,
           const Point& size,
           const float range[2],
           bool vertical = false,
           MapType mapType = MAP_LINEAR,
           const char* colorScheme = 0);

    ~Slider();

public:

    float getValue() const;

    void setValue(float val);

    void setEnabled(bool enabled = true);

    void setHandleSize(float size);

    virtual void mouseClicked(MouseButton b, StateEvent ev, GuiObject* receiver);

    virtual void mouseWheeled(int delta, GuiObject* receiver);

    virtual void dragged(const Point& delta, GuiObject* receiver);

    virtual void setColors(const char* colorScheme);

    void setMouseWheelStep(float step);

    float getMouseWheelStep() const;

private:

    void init(const char* colorScheme);

    void calculateValue();

    void setHandlePosition(int newPos);

    int calculateHandlePos() const;

protected:

    WiBox* handle_;

    bool bVertical_;

    bool bEnabled_;

    float value_;

    /**
     * Value between 0 and 1 that determines how much the value changes on mouse
     * wheel events. The step is multiplied with the wheel event's delta and
     * added to value_. Default value is 0.1.
     */
    float mouseWheelStep_;

};


} // namespace gw1k

#endif // GW1K_SLIDER_H_
