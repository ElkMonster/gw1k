#include "widgets/WiBox.h"

#include "utils/Helpers.h"
#include "Render.h"
#include "ThemeManager.h"

#include <iostream>

namespace gw1k
{


WiBox::WiBox(const Point& pos, const Point& size, const char* colorScheme)
:   Box(pos, size)
{
    setColors(colorScheme);
}


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
    // OpenGL will render Quads one pixel too small on the right and bottom
    // edges if we give it the last pixel, so give it the end (which does not
    // belong to our box anymore)
    Point end = getEnd() + offset;
    fillRect(pointToGeomPoint2D(pos), pointToGeomPoint2D(end));
}


void
WiBox::setColors(const char* colorScheme)
{
    ThemeManager::getInstance()->setColors(this, colorScheme, "WiBox");
}


} // namespace gw1k
