#include "providers/ResizedEventProvider.h"

namespace gw1k
{


ResizedEventProvider::ResizedEventProvider()
{

}


ResizedEventProvider::~ResizedEventProvider()
{

}


void
ResizedEventProvider::addResizedListener(ResizedListener* dl)
{
    resizedListeners_.push_back(dl);
}


void
ResizedEventProvider::removeResizedListener(ResizedListener* dl)
{
    resizedListeners_.remove(dl);
}


void
ResizedEventProvider::informResizedListeners(
    const Point& delta,
    Orientation orientation,
    GuiObject* receiver)
{
    for (ResizedListnrIter i = resizedListeners_.begin(); i != resizedListeners_.end(); ++i)
    {
        (*i)->resized(delta, orientation, receiver);
    }
}



} // namespace gw1k

