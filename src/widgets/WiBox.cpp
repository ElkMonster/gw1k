#include "widgets/WiBox.h"

#include "Helpers.h"
#include "Render.h"
#include <iostream>
namespace gw1k
{


WiBox::WiBox(const Point& pos, const Point& size)
:   Box(pos, size)
{}


WiBox::~WiBox()
{}


void
WiBox::renderFg(const Point& offset) const
{
    Point pos = getPos() + offset;
    Point last = pos + getSize() - Point(1, 1);
    //std::cout << getPos() << ", " << getSize() << " [pos + offset = " << pos << "]" << std::endl;
    drawRect(pointToGeomPoint2D(pos), pointToGeomPoint2D(last));
}


void
WiBox::renderBg(const Point& offset) const
{
    Point pos = getPos() + offset;
    Point last = pos + getSize() - Point(1, 1);
    fillRect(pointToGeomPoint2D(pos), pointToGeomPoint2D(last));
}


} // namespace gw1k
