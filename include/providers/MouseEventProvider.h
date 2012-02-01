#ifndef GW1K_MOUSEEVENTPROVIDER_H_
#define GW1K_MOUSEEVENTPROVIDER_H_

#include "../listeners/MouseListener.h"

#include <list>

namespace gw1k
{


typedef std::list<MouseListener*>::iterator MouseListnrIter;


class MouseEventProvider
{

public:

    MouseEventProvider();

    virtual ~MouseEventProvider();

public:

    void addMouseListener(MouseListener* ml);

    void removeMouseListener(MouseListener* ml);

protected:

    void informMouseListenersMoved(MouseMovedEvent ev,
                                   const Point& pos,
                                   const Point& delta,
                                   GuiObject* receiver);

    void informMouseListenersClicked(MouseButton b,
                                     StateEvent ev,
                                     GuiObject* receiver);

    void informMouseListenersWheeled(int delta, GuiObject* receiver);

protected:

    std::list<MouseListener*> mouseListeners_;

};


} // namespace gw1k

#endif // GW1K_MOUSEEVENTPROVIDER_H_
