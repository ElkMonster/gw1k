#ifndef GW1K_RENDERABLE_H_
#define GW1K_RENDERABLE_H_

#include "GuiObject.h"

#include "gw1k/include/Color4i.h"

namespace gw1k
{


class Renderable : public GuiObject
{

public:

    Renderable(const char* colorScheme = 0);

    virtual ~Renderable();

public:

    virtual void render(const Point& offset) const;

    Renderable& setFgColor(const gw1k::Color4i* col);

    Renderable& setBgColor(const gw1k::Color4i* col);

    Renderable& setHoveredFgColor(const gw1k::Color4i* col);

    Renderable& setHoveredBgColor(const gw1k::Color4i* col);

    Renderable& setClickedFgColor(const gw1k::Color4i* col);

    Renderable& setClickedBgColor(const gw1k::Color4i* col);

    virtual void setColors(const char* colorScheme);

protected:

    void renderSelf(const Point& offset) const;

    virtual void renderSubObjects(const Point& offset) const;

    virtual void renderFg(const Point& offset) const = 0;

    virtual void renderBg(const Point& offset) const = 0;

    void setColor(const gw1k::Color4i* col, gw1k::Color4i*& dest);

    void selectColors(gw1k::Color4i*& fg, gw1k::Color4i*& bg) const;

protected:

    gw1k::Color4i* fgCol_;

    gw1k::Color4i* bgCol_;

    gw1k::Color4i* hoveredFgCol_;

    gw1k::Color4i* hoveredBgCol_;

    gw1k::Color4i* clickedFgCol_;

    gw1k::Color4i* clickedBgCol_;

};


} // namespace gw1k

#endif // GW1K_RENDERABLE_H_
