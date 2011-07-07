#ifndef GW1K_SLIDER_H_
#define GW1K_SLIDER_H_


/**
 * If set to any non-0 value, the slider handle is made of a Label and displays
 * the slider value in the handle.
 */
#define DEBUG_SLIDER 0

#include "WiBox.h"
#include "../listeners/SliderListener.h"
#include "../listeners/MouseListener.h"

#if DEBUG_SLIDER
#include "Label.h"
#endif





namespace gw1k
{


class Slider : public WiBox, public MouseListener
{

public:

    Slider(const Point& pos,
           const Point& size,
           bool vertical = false,
           const char* colorScheme = 0);

    ~Slider();

public:

    float getValue() const;

    void setValue(float val);

    void setEnabled(bool enabled = true);

    void setHandleSize(float size);

    virtual void mouseMoved(MouseMovedEvent ev,
                    const Point& pos,
                    const Point& delta,
                    GuiObject* target);

    virtual void mouseClicked(MouseButton b, StateEvent ev, GuiObject* target);

    virtual void mouseWheeled(int delta, GuiObject* target);

    void addListener(SliderListener* sl);

    void removeListener(SliderListener* sl);

    virtual void setColors(const char* colorScheme);

protected:

    void calculateValue();

    void setHandlePosition(Point newPos);

    void informListeners(float delta);

protected:

#if DEBUG_SLIDER
    Label handle_;
#else
    WiBox handle_;
#endif

    bool bVertical_;

    bool bEnabled_;

    float value_;

    std::vector<SliderListener*> listeners_;
};


} // namespace gw1k

#endif // GW1K_SLIDER_H_
