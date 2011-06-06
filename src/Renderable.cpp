#include "Renderable.h"

#include <GL/gl.h>

#include "Helpers.h"
#include "ThemeManager.h"

//#define GW1K_ENABLE_GL_ERROR_CHECKS
#include "gw1k/include/GLErrorCheck.h"


namespace gw1k
{


Renderable::Renderable(const char* colorScheme)
:   fgCol_(0),
    bgCol_(0),
    hoveredFgCol_(0),
    hoveredBgCol_(0),
    clickedFgCol_(0),
    clickedBgCol_(0)
{
    setColors(colorScheme);
}


Renderable::~Renderable()
{
    DELETE_PTR(fgCol_);
    DELETE_PTR(bgCol_);
    DELETE_PTR(hoveredFgCol_);
    DELETE_PTR(hoveredBgCol_);
    DELETE_PTR(clickedFgCol_);
    DELETE_PTR(clickedBgCol_);
}


void
Renderable::render(const Point& offset) const
{
    if (bIsVisible_)
    {
        renderSelf(offset);
        renderSubObjects(offset);
    }
}


void
Renderable::renderSelf(const Point& offset) const
{
    gw1k::Color4i* fg, * bg;
    selectColors(fg, bg);

    if (bg)
    {
        glColor4f(bg->rf, bg->gf, bg->bf, bg->af);
        renderBg(offset);
    }

    if (fg)
    {
        glColor4f(fg->rf, fg->gf, fg->bf, fg->af);
        renderFg(offset);
    }
}


void
Renderable::renderSubObjects(const Point& offset) const
{
    Point offsett = offset;
    for (unsigned int i = 0; i != subObjects_.size(); ++i)
    {
        subObjects_[i]->render(offset + getPos());
    }
}


void
Renderable::setColor(const gw1k::Color4i* col, gw1k::Color4i*& dest)
{
    if (col && !dest)
    {
        dest = new gw1k::Color4i(*col);
    }
    else if (!col && dest)
    {
        delete dest;
        dest = 0;
    }
    else
    {
        *dest = *col;
    }
}


void
Renderable::selectColors(gw1k::Color4i*& fg, gw1k::Color4i*& bg) const
{
    if (bIsClicked_)
    {
        fg = clickedFgCol_ ? clickedFgCol_ : (hoveredFgCol_ ? hoveredFgCol_ : fgCol_);
        bg = clickedBgCol_ ? clickedBgCol_ : (hoveredBgCol_ ? hoveredBgCol_ : bgCol_);
    }
    else if (bIsHovered_)
    {
        fg = hoveredFgCol_ ? hoveredFgCol_ : fgCol_;
        bg = hoveredBgCol_ ? hoveredBgCol_ : bgCol_;
    }
    else
    {
        fg = fgCol_;
        bg = bgCol_;
    }
}

Renderable&
Renderable::setFgColor(const gw1k::Color4i* col)
{
    setColor(col, fgCol_);
    return *this;
}


Renderable&
Renderable::setBgColor(const gw1k::Color4i* col)
{
    setColor(col, bgCol_);
    return *this;
}


Renderable&
Renderable::setHoveredFgColor(const gw1k::Color4i* col)
{
    setColor(col, hoveredFgCol_);
    return *this;
}


Renderable&
Renderable::setHoveredBgColor(const gw1k::Color4i* col)
{
    setColor(col, hoveredBgCol_);
    return *this;
}


Renderable&
Renderable::setClickedFgColor(const gw1k::Color4i* col)
{
    setColor(col, clickedFgCol_);
    return *this;
}


Renderable&
Renderable::setClickedBgColor(const gw1k::Color4i* col)
{
    setColor(col, clickedBgCol_);
    return *this;
}


void
Renderable::setColors(const char* colorScheme)
{
    ThemeManager::getInstance()->setColors(this, colorScheme, 0);
}


} // namespace gw1k
