#include "providers/MouseEventProvider.h"

#include <algorithm>

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
    MouseListnrIter i = std::find(mouseListeners_.begin(),
                                  mouseListeners_.end(), ml);

    if (i != mouseListeners_.end())
    {
        mouseListeners_.remove(ml);
    }
}


} // namespace gw1k
