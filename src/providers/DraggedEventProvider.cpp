#include "providers/DraggedEventProvider.h"

namespace gw1k
{


DraggedEventProvider::DraggedEventProvider()
{

}


DraggedEventProvider::~DraggedEventProvider()
{

}


void
DraggedEventProvider::addDraggedListener(DraggedListener* dl)
{
    draggedListeners_.push_back(dl);
}


void
DraggedEventProvider::removeDraggedListener(DraggedListener* dl)
{
    draggedListeners_.remove(dl);
}


void
DraggedEventProvider::informDraggedListeners(
    const Point& delta,
    GuiObject* receiver)
{
    for (DraggedListnrIter i = draggedListeners_.begin(); i != draggedListeners_.end(); ++i)
    {
        (*i)->dragged(delta, receiver);
    }
}



} // namespace gw1k

