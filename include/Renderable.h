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

    /**
     * If this object is visible, then this method calls renderSelf() and
     * renderSubObjects().
     */
    virtual void render(const Point& offset) const;

    Renderable& setFgColor(const gw1k::Color4i* col);

    Renderable& setBgColor(const gw1k::Color4i* col);

    Renderable& setHoveredFgColor(const gw1k::Color4i* col);

    Renderable& setHoveredBgColor(const gw1k::Color4i* col);

    Renderable& setClickedFgColor(const gw1k::Color4i* col);

    Renderable& setClickedBgColor(const gw1k::Color4i* col);

    virtual void setColors(const char* colorScheme);

    enum ColorState { STATE_NORMAL, STATE_HOVERED, STATE_CLICKED };

    /**
     * Gets foreground and background colours specified for the given state.
     */
    void queryColors(Color4i*& fg, Color4i*& bg, ColorState state) const;

protected:

    /**
     * This method performs four steps:
     * 1. Load foreground and background color via selectColors()
     * 2. If background color is present, set it via glColor() and call
     *    renderBg()
     * 3. Call renderContent()
     * 4. If foreground color is present, set it via glColor() and call
     *    renderFg()
     */
    void renderSelf(const Point& offset) const;

    /**
     * Renders all sub-objects in the order they were added.
     */
    virtual void renderSubObjects(const Point& offset) const;

    /**
     * This method should render all (widget decoration) foreground elements,
     * e.g. borders and texts. The method is called from renderSelf(), which
     * sets the foreground color specified for the specific widgets in the theme
     * file or in the code.
     * To render actual content that is not related to gw1k's themeing, use
     * renderContent().
     */
    virtual void renderFg(const Point& offset) const = 0;

    /**
     * This method should render everything that shall be displayed in the
     * widget, but is neither foreground nor background of the widget. E.g., if
     * the widget displays an image or renders a 3D scene, the code for this
     * should go into renderContent().
     * In its default implementation, renderContent() is an empty method.
     */
    virtual void renderContent(const Point& offset) const;

    /**
     * This method should render the widget background. Typically, this is one
     * single color that fills the whole area of the widget. The method is
     * called from renderSelf(), which sets the foreground color specified for
     * the specific widgets in the theme file or in the code.
     * To render actual content that is not related to gw1k's themeing, use
     * renderContent().
     */
    virtual void renderBg(const Point& offset) const = 0;

    void setColor(const gw1k::Color4i* col, gw1k::Color4i*& dest);

    /**
     * Calls queryColors() with the current state of this Renderable.
     */
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
