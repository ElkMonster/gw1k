#ifndef GW1K_TEXT_H_
#define GW1K_TEXT_H_

#include "../Renderable.h"
#include "../Gw1kConstants.h"


#include <string>
#include <FTGL/ftgl.h>

namespace gw1k
{


class Text : public Renderable
{

public:

    friend class Label;

    Text();

    Text(const std::string& text);

    virtual ~Text();

public:

    virtual void update();

    void setText(const std::string& text);

    void setFont(const std::string& name, unsigned int faceSize);

    void setHorizontalAlignment(TextProperty alignment);

    virtual const Point& setSize(float width, float height);

    virtual void renderFg(const Point& offset) const;

    virtual void renderBg(const Point& offset) const;

    virtual bool containsMouse(const Point& p) const;

protected:

    /** Only call this inside render() (garbles font textures otherwise, it seems) */
    void updateBBox();

protected:

    FTFont* font_;

    FTSimpleLayout* layout_;

    int textLength_;

    char* text_;

    FTBBox ftBB_;

private:

    bool bBBoxUpdateNeeded_;

};


} // namespace gw1k

#endif // GW1K_TEXT_H_
