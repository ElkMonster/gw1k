#include "widgets/Text.h"
#include "FTGLFontManager.h"

#include "Helpers.h"
#include "WManager.h"
#include "ThemeManager.h"

#include <GL/gl.h>

#include <iostream>

namespace gw1k
{


Text::Text(const char* colorScheme)
:   Renderable(colorScheme),
    font_(0),
    layout_(new FTSimpleLayout()),
    textLength_(0),
    text_(0)
{
    // Set to very large default such that we will end up
    setSize(99999, 99999);
    setColors(colorScheme);
}


Text::Text(const std::string& text, const char* colorScheme)
:   Renderable(colorScheme),
    font_(0),
    layout_(new FTSimpleLayout()),
    textLength_(0),
    text_(0)
{
    setSize(99999, 99999);
    setText(text);
    setColors(colorScheme);
}


Text::~Text()
{
    if (text_)
    {
        delete[] text_;
    }

    delete layout_;
}


void
Text::preRenderUpdate()
{
    if (bBBoxUpdateNeeded_)
    {
        updateBBox();
    }
}


void
Text::setText(const std::string& text)
{
    if (text_)
    {
        delete[] text_;
    }

    textLength_ = text.length();
    text_ = new char[textLength_ + 1];

    strcpy(text_, text.c_str());

    bBBoxUpdateNeeded_ = true;
    WManager::getInstance()->registerForPreRenderUpdate(this);
}


void
Text::setFont(const std::string& name, unsigned int faceSize)
{
    font_ = FTGLFontManager::Instance().GetFont(name.c_str(), faceSize);
    layout_->SetFont(font_);
}


void
Text::setHorizontalAlignment(TextProperty alignment)
{
    FTGL::TextAlignment ftglAlignment;
    switch(alignment)
    {
    case GW1K_ALIGN_CENTER:
        ftglAlignment = FTGL::ALIGN_CENTER;
        break;
    case GW1K_ALIGN_RIGHT:
        ftglAlignment = FTGL::ALIGN_RIGHT;
        break;
    case GW1K_ALIGN_JUSTIFY:
        ftglAlignment = FTGL::ALIGN_JUSTIFY;
        break;
    case GW1K_ALIGN_LEFT:
    default:
        ftglAlignment = FTGL::ALIGN_LEFT;
        break;
    }

    layout_->SetAlignment(ftglAlignment);
}


void
Text::renderFg(const Point& offset) const
{
    if (font_ && text_)
    {
        glPushMatrix();
        {
            // We have to tinker a little with the Y coordinate because FTGL
            // assumes the default OpenGL default coordinate system with X and Y
            // axis going right and up, respectively. Since our coordinate
            // system's origin is in the upper left corner and spreads right and
            // down, we have to transform a little:
            // First, move to global render position, though this actually
            // happens in FTGL space, which moves away from our view.
            // Second, mirror vertically to compensate for FTGL's and our
            // coordinate system's opposite Y directions, so the text is flipped
            // at the origin an appears at the right place in our coordinate
            // system.
            // (Note: The ftBB_ addenda are needed to adjust the text's position
            // to exactly fit the expected way - try centered text without it.)

            Point t = offset + getPos();
            float x = t.x - ftBB_.Lower().Xf();
            float y = t.y + ftBB_.Upper().Yf();
            glTranslatef(x, y, 0.f);
            glScalef(1.f, -1.f, 1.f);
            layout_->Render(text_, textLength_);
        }
        glPopMatrix();
    }
}


void
Text::renderBg(const Point& offset) const
{
    // Nothing to do here
}


bool
Text::containsMouse(const Point& p) const
{
    return false;
}


const Point&
Text::setSize(float width, float height)
{
    const Point& size = GuiObject::setSize(width, height);
    layout_->SetLineLength(getSize().x);
    return size;
}


void
Text::setColors(const char* colorScheme)
{
    ThemeManager::getInstance()->setColors(this, colorScheme, "Text");
}


void
Text::updateBBox()
{
    ftBB_ = layout_->BBox(text_);
    bBBoxUpdateNeeded_ = false;
}


} // namespace gw1k
