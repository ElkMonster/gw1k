#ifndef GW1K_DRAGGEDLISTENER_H_
#define GW1K_DRAGGEDLISTENER_H_

#include "../Point.h"

namespace gw1k
{


class GuiObject;


class DraggedListener
{

public:

    virtual void dragged(const Point& delta, GuiObject* receiver) = 0;

};

} // namespace gw1k

#endif // GW1K_DRAGGEDLISTENER_H_

