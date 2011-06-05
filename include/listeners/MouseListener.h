#ifndef GW1K_MOUSELISTENER_H_
#define GW1K_MOUSELISTENER_H_

#include "../Point.h"
#include "../Gw1kConstants.h"

namespace gw1k
{


class GuiObject;


class MouseListener
{

public:

    virtual void mouseMoved(MouseMovedEvent ev,
                            const Point& pos,
                            const Point& delta,
                            GuiObject* target) = 0;

    virtual void mouseClicked(MouseButton b,
                              StateEvent ev,
                              GuiObject* target) = 0;

    //virtual void mouseWheel() const = 0;

};

} // namespace gw1k

#endif // GW1K_MOUSELISTENER_H_
