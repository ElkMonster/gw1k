#include "Renderable.h"

#ifdef __APPLE__
#include <OpenGL/glew.h>
#else
#include <GL/glew.h>
#endif

#include "utils/Helpers.h"
#include "ThemeManager.h"

//#define GW1K_ENABLE_GL_ERROR_CHECKS
#include "gw1k/include/GLErrorCheck.h"


namespace gw1k
{


Renderable::Renderable(const char* colorScheme)
:   adaptMode_(ADAPT_SELF)
{
    setColors(colorScheme);
}


Renderable::~Renderable()
{
}


void
Renderable::setAdaptMode(AdaptMode mode)
{
    adaptMode_ = mode;
}


Renderable::AdaptMode
Renderable::getAdaptMode() const
{
    return adaptMode_;
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
Renderable::selectColors(Color4i*& fg, Color4i*& bg) const
{
    ColorTable::ColorState state;
    if (adaptMode_ != ADAPT_SELF)
    {
        GuiObject* parent =
            (adaptMode_ == ADAPT_PARENT) ? parent_ : getNonEmbeddedParent();
        state = (parent->isClicked() ? ColorTable::STATE_CLICKED :
            (parent->isHovered() ? ColorTable::STATE_HOVERED : ColorTable::STATE_NORMAL));
    }
    else
    {
        state = (bIsClicked_ ? ColorTable::STATE_CLICKED :
            (bIsHovered_ ? ColorTable::STATE_HOVERED : ColorTable::STATE_NORMAL));
    }
    colorTable_.queryColors(fg, bg, state);
}


} // namespace gw1k
