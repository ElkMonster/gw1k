#ifndef GW1K_MOUSELISTENER_H_
#define GW1K_MOUSELISTENER_H_

#include "../Point.h"
#include "../Gw1kConstants.h"

namespace gw1k
{


class GuiObject;


/**
 * Warning: In general, it is not advisable to delete GuiObjects within a mouse
 * callback, as this might delete the object that is the source of a callback.
 * The main problem arises due to the way events are handled and distributed by
 * WManager and GuiObject: Since all registered listeners are served the event,
 * deleting the event source also deletes the list of listeners which is being
 * iterated over in that very moment. This is clearly not a good idea.
 * In order to remove and delete GuiObjects, use WManager's markForDeletion()
 * method.
 */
class MouseListener
{

public:

    virtual void mouseMoved(MouseMovedEvent ev,
                            const Point& pos,
                            const Point& delta,
                            GuiObject* receiver) = 0;

    virtual void mouseClicked(MouseButton b,
                              StateEvent ev,
                              GuiObject* receiver) = 0;

    virtual void mouseWheeled(int delta, GuiObject* receiver) = 0;

};

} // namespace gw1k

#endif // GW1K_MOUSELISTENER_H_
