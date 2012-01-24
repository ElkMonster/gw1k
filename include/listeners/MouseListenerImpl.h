#ifndef GW1K_MOUSELISTENERIMPL_H_
#define GW1K_MOUSELISTENERIMPL_H_

#include "MouseListener.h"


namespace gw1k
{


class MouseListenerImpl : public MouseListener
{

public:

    virtual void mouseMoved(MouseMovedEvent ev,
                            const Point& pos,
                            const Point& delta,
                            GuiObject* receiver)
    {}

    virtual void mouseClicked(MouseButton b,
                              StateEvent ev,
                              GuiObject* receiver)
    {}

    virtual void mouseWheeled(int delta, GuiObject* receiver)
    {}

};


} // namespace gw1k

#endif // GW1K_MOUSELISTENERIMPL_H_

