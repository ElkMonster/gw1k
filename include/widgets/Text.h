#ifndef GW1K_TEXT_H_
#define GW1K_TEXT_H_

#include "../Renderable.h"
#include "../Gw1kConstants.h"


#include <string>
#include <FTGL/ftgl.h>

namespace gw1k
{


/**
 * Text is a pretty basic class that is not intended to be used directly as a
 * widget. Instead, use a (auto-sized) Label without decoration if you need to
 * display pure text in your application. Text does not entirely adhere to the
 * normal rendering process and disregards widget boundaries. Also, it does not
 * draw any decoration, but only pure letters.
 */
class Text : public Renderable
{

public:

    friend class Label;

    Text(const char* colorScheme = 0);

    Text(const std::string& text, const char* colorScheme = 0);

    virtual ~Text();

public:

    Point getTextSize() const;

    void setText(const std::string& text);

    void setFont(const std::string& name, unsigned int faceSize);

    void setHorizontalAlignment(TextProperty alignment);

    virtual const Point& setSize(float width, float height);

    virtual void renderFg(const Point& offset) const;

    virtual void renderBg(const Point& offset) const;

    virtual bool containsMouse(const Point& p) const;

    virtual void setColors(const char* colorScheme);

protected:

    /**
     * Only call this inside render() (garbles font textures otherwise, it
     * seems). Note that preRenderUpdate() is part of render().
     */
    void updateBBox();

protected:

    FTFont* font_;

    FTSimpleLayout* layout_;

    int textLength_;

    char* text_;

    FTBBox ftBB_;

};


} // namespace gw1k

#endif // GW1K_TEXT_H_
