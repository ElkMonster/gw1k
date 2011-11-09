#include "Renderable.h"

#include <GL/gl.h>

#include "utils/Helpers.h"
#include "ThemeManager.h"

//#define GW1K_ENABLE_GL_ERROR_CHECKS
#include "gw1k/include/GLErrorCheck.h"


namespace gw1k
{


Renderable::Renderable(const char* colorScheme)
{
    setColors(colorScheme);
}


Renderable::~Renderable()
{
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


Renderable&
Renderable::setFgColor(const Color4i* col)
{
    setColor(col, colorTable_.fgCol);
    return *this;
}


Renderable&
Renderable::setBgColor(const Color4i* col)
{
    setColor(col, colorTable_.bgCol);
    return *this;
}


Renderable&
Renderable::setHoveredFgColor(const Color4i* col)
{
    setColor(col, colorTable_.hoveredFgCol);
    return *this;
}


Renderable&
Renderable::setHoveredBgColor(const Color4i* col)
{
    setColor(col, colorTable_.hoveredBgCol);
    return *this;
}


Renderable&
Renderable::setClickedFgColor(const Color4i* col)
{
    setColor(col, colorTable_.clickedFgCol);
    return *this;
}


Renderable&
Renderable::setClickedBgColor(const Color4i* col)
{
    setColor(col, colorTable_.clickedBgCol);
    return *this;
}


void
Renderable::setColors(const char* colorScheme)
{
    ThemeManager::getInstance()->setColors(this, colorScheme, 0);
}


void
Renderable::queryColors(Color4i*& fg, Color4i*& bg, ColorState state) const
{
    const ColorTable& c = colorTable_;
    switch (state)
    {
    case STATE_CLICKED:
        fg = c.clickedFgCol ? c.clickedFgCol : (c.hoveredFgCol ? c.hoveredFgCol : c.fgCol);
        bg = c.clickedBgCol ? c.clickedBgCol : (c.hoveredBgCol ? c.hoveredBgCol : c.bgCol);
        break;
    case STATE_HOVERED:
        fg = c.hoveredFgCol ? c.hoveredFgCol : c.fgCol;
        bg = c.hoveredBgCol ? c.hoveredBgCol : c.bgCol;
        break;
    case STATE_NORMAL:
        fg = c.fgCol;
        bg = c.bgCol;
        break;
    }
}


void
Renderable::renderSelf(const Point& offset) const
{
    Color4i* fg, * bg;
    selectColors(fg, bg);

    if (bg)
    {
        glColor4f(bg->rf, bg->gf, bg->bf, bg->af);
        renderBg(offset);
    }

    renderContent(offset);

    if (fg)
    {
        glColor4f(fg->rf, fg->gf, fg->bf, fg->af);
        renderFg(offset);
    }
}


void
Renderable::renderSubObjects(const Point& offset) const
{
    for (unsigned int i = 0; i != subObjects_.size(); ++i)
    {
        subObjects_[i]->render(offset + getPos());
    }
}


void
Renderable::renderContent(const Point& offset) const
{
    // Do nothing by default
}


void
Renderable::setColor(const Color4i* col, Color4i*& dest)
{
    if (col && !dest)
    {
        dest = new Color4i(*col);
    }
    else if (!col && dest)
    {
        delete dest;
        dest = 0;
    }
    else if (col && dest)
    {
        *dest = *col;
    }
}


void
Renderable::selectColors(Color4i*& fg, Color4i*& bg) const
{
    ColorState state = (bIsClicked_ ? STATE_CLICKED : (bIsHovered_ ? STATE_HOVERED : STATE_NORMAL));
    queryColors(fg, bg, state);
}


} // namespace gw1k
