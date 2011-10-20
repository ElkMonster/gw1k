#include "widgets/Text.h"
#include "FTGLFontManager.h"

#include "Helpers.h"
#include "WManager.h"
#include "ThemeManager.h"
#include "Math.h"

#include <GL/gl.h>

#include <iostream>
#include <cmath>

namespace gw1k
{


Text::Text(
    const Point& pos,
    const std::string& text,
    const std::string& fontName,
    const char* colorScheme)
:   Renderable(colorScheme),
    font_(0),
    layout_(new FTSimpleLayout()),
    textLength_(0),
    text_(0),
    size_(0, 0),
    bLineLengthSet_(false),
    fontName_(fontName)
{
    setPos(pos.x, pos.y);

    setHorizontalAlignment(GW1K_ALIGN_LEFT);
    setColors(colorScheme);
    setFont(fontName_, 12);

    setText(text);
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
Text::setText(const std::string& text)
{
    if (text_)
    {
        delete[] text_;
    }

    textLength_ = text.length();
    text_ = new char[textLength_ + 1];
    strncpy(text_, text.c_str(), textLength_ + 1);

    update();
}


void
Text::setFontSize(int fontSize)
{
    if (font_)
    {
        setFont(fontName_, fontSize);
    }
}


int
Text::getFontSize() const
{
    return font_ ? font_->FaceSize() : -1;
}


void
Text::setFont(const std::string& name, unsigned int faceSize)
{
    fontName_ = name;
    font_ = FTGLFontManager::Instance().GetFont(name.c_str(), faceSize);
    layout_->SetFont(font_);
    update();
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
    updateBBox();
}


const Point&
Text::setSize(float width, float height)
{
    if (width == 0.f)
    {
        bLineLengthSet_ = false;
        // Make text one-liner; using std::numeric_limits<float>::max() here
        // resulted in a zero-width bounding box, so a really big number is used
        // instead; note that this places the text a some very distant position
        // for centre and right alignment, so we need to trim the line length
        // afterwards
        layout_->SetLineLength(999999.f);
    }
    else
    {
        bLineLengthSet_ = true;
        size_.x = GuiObject::setSize(width, height).x;
        layout_->SetLineLength(size_.x);
    }

    updateBBox();
    updateWidth();
    updateHeight();

    return size_;
}


const Point&
Text::getSize() const
{
    return size_;
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

            Point t = offset + getPos();

            // We need to "neutralise" the offset that the text gets when it is
            // aligned centred or right and line length is not set (because the
            // text is then located somewhere far from our screen); if line
            // length is set, there's no need to neutralise (because the text
            // should be on-screen usually)
            float x = t.x + (bLineLengthSet_ ? 0 : -ftBB_.Lower().Xf());

            // We need to add the text's upper y extent to our y-offset; this
            // translates the text the correct (from our point of view) baseline
            // (try leaving it out, the text will be displaced)
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


void
Text::setColors(const char* colorScheme)
{
    ThemeManager::getInstance()->setColors(this, colorScheme, "Text");
}


void
Text::updateBBox()
{
    ftBB_ = (font_ && text_) ? layout_->BBox(text_) : FTBBox();
}


void
Text::updateHeight()
{
    size_.y = std::ceil(std::abs(ftBB_.Lower().Yf() - ftBB_.Upper().Yf()));
}


void
Text::updateWidth()
{
    if (bLineLengthSet_)
    {
        size_.x = layout_->GetLineLength();
    }
    else
    {
        size_.x = std::ceil(std::abs(ftBB_.Lower().Xf() - ftBB_.Upper().Xf()));
    }
}


void
Text::update()
{
    setSize((bLineLengthSet_ ? size_.x : 0.f), size_.y);
}


} // namespace gw1k
