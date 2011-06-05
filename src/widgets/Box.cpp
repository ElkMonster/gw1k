#include "widgets/Box.h"

#include "WManager.h"

#include <GL/gl.h>
#include <cmath>

#include <iostream>

namespace gw1k
{


Box::Box(const Point& pos, const Point& size)
:   Renderable()
{
    setPos(pos.x, pos.y);
    setSize(size.x, size.y);
}


Box::~Box()
{}


void
Box::renderSubObjects(const Point& offset) const
{
    Point coffset = offset;
    if (subObjects_.size() != 0)
    {
        WManager::getInstance()->pushGlScissor(getPos() + offset, getSize());

        // Position of "this" will be added to offset in the Renderable
        // implementation, so do not modify offset here
        Renderable::renderSubObjects(offset);

        WManager::getInstance()->popGlScissor();
    }
}


void
Box::renderFg(const Point& offset) const
{
    // Box is invisible
}


void
Box::renderBg(const Point& offset) const
{
    // Box is invisible
}


} // namespace gw1k
