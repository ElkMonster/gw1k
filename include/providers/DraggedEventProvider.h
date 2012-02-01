#ifndef GW1K_DRAGGEDEVENTPROVIDER_H_
#define GW1K_DRAGGEDEVENTPROVIDER_H_

#include "../listeners/DraggedListener.h"

#include <list>

namespace gw1k
{


typedef std::list<DraggedListener*>::iterator DraggedListnrIter;


class DraggedEventProvider
{

public:

    DraggedEventProvider();

    virtual ~DraggedEventProvider();

public:

    void addDraggedListener(DraggedListener* dl);

    void removeDraggedListener(DraggedListener* dl);

protected:

    void informDraggedListeners(const Point& delta, GuiObject* receiver);

protected:

    std::list<DraggedListener*> draggedListeners_;

};


} // namespace gw1k

#endif // GW1K_DRAGGEDEVENTPROVIDER_H_

