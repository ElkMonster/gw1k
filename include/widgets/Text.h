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
 * draw any decoration, but only pure letters. Furthermore, it doesn't react to
 * mouse or keyboard events, including colour changes.
 */
class Text : public Renderable
{

public:

    Text(const Point& pos,
         const std::string& text,
         const std::string& fontName = "arial.ttf",
         const char* colorScheme = 0);

    virtual ~Text();

public:

    void setText(const std::string& text);

    void setFontSize(int fontSize);

    /**
     * Gets the font size, or -1 if no font is set.
     */
    int getFontSize() const;

    void setFont(const std::string& name, unsigned int faceSize);

    void setHorizontalAlignment(TextProperty alignment);

    /**
     * Setting width to 0 leaves line length disregarded, meaning that the
     * Text's width is always set to the actual width of the text (as one line).
     */
    virtual const Point& setSize(float width, float height);

    virtual const Point& getSize() const;

    virtual void renderFg(const Point& offset) const;

    virtual void renderBg(const Point& offset) const;

    virtual bool containsMouse(const Point& p) const;

    virtual void setColors(const char* colorScheme);

private:

    void updateBBox();

    void updateWidth();

    void updateHeight();

    void update();

protected:

    FTFont* font_;

    FTSimpleLayout* layout_;

    int textLength_;

    char* text_;

    FTBBox ftBB_;

    Point size_;

    bool bLineLengthSet_;

    std::string fontName_;

};


} // namespace gw1k

#endif // GW1K_TEXT_H_
