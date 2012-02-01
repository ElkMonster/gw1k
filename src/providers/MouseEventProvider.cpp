#include "providers/MouseEventProvider.h"

namespace gw1k
{


MouseEventProvider::MouseEventProvider()
{

}


MouseEventProvider::~MouseEventProvider()
{

}


void
MouseEventProvider::addMouseListener(MouseListener* ml)
{
    mouseListeners_.push_back(ml);
}


void
MouseEventProvider::removeMouseListener(MouseListener* ml)
{
    mouseListeners_.remove(ml);
}


void
MouseEventProvider::informMouseListenersMoved(
    MouseMovedEvent ev,
    const Point& pos,
    const Point& delta,
    GuiObject* receiver)
{
    for (MouseListnrIter i = mouseListeners_.begin(); i != mouseListeners_.end(); ++i)
    {
        (*i)->mouseMoved(ev, pos, delta, receiver);
    }
}


void
MouseEventProvider::informMouseListenersClicked(
    MouseButton b,
    StateEvent ev,
    GuiObject* receiver)
{
    for (MouseListnrIter i = mouseListeners_.begin(); i != mouseListeners_.end(); ++i)
    {
        (*i)->mouseClicked(b, ev, receiver);
    }
}


void
MouseEventProvider::informMouseListenersWheeled(int delta, GuiObject* receiver)
{
    for (MouseListnrIter i = mouseListeners_.begin(); i != mouseListeners_.end(); ++i)
    {
        (*i)->mouseWheeled(delta, receiver);
    }
}


} // namespace gw1k
