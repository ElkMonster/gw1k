#include "widgets/Label.h"

#include "Gw1kConstants.h"
#include "Math.h"
#include "WManager.h"
#include "Color4i.h"
#include "ThemeManager.h"

#include <iostream>


namespace gw1k
{


Label::Label(
    const Point& pos,
    const Point& size,
    const std::string& text,
    int faceSize,
    const std::string& fontName,
    const char* colorScheme)
:   WiBox(pos, size, colorScheme),
    textProps_(0)
{
    text_.setFont(fontName, (faceSize < 0 ? size.y : faceSize));
    setText(text);
    setTextProperty(GW1K_ALIGN_CENTER);
    setTextProperty(GW1K_ALIGN_VERT_CENTER);
    text_.setPos(0, 0);
    text_.setSize(size.x, size.y);
    text_.setFgColor(gw1k::color::green);

    addSubObject(&text_);

    addMouseListener(this);
}


Label::~Label()
{}


void
Label::update()
{
    if (text_.bBBoxUpdateNeeded_)
    {
        text_.updateBBox();
    }

    updateVerticalAlignment();
}


void
Label::setText(const std::string& text)
{
    text_.setText(text);
    WManager::getInstance()->registerForPreRenderUpdate(this);
}


const Point&
Label::setSize(float width, float height)
{
    const Point& size = GuiObject::setSize(width, height);

    // Bounding box needs update in order for us to recalculate the vertical
    // alignment
    text_.bBBoxUpdateNeeded_ = true;
    WManager::getInstance()->registerForPreRenderUpdate(this);

    return size;
}


void
Label::setTextProperty(TextProperty p)
{
    switch (p)
    {
    case GW1K_ALIGN_LEFT:
    case GW1K_ALIGN_CENTER:
    case GW1K_ALIGN_RIGHT:
    case GW1K_ALIGN_JUSTIFY:
        // Erase current horizontal alignment
        textProps_ &= !(GW1K_ALIGN_LEFT | GW1K_ALIGN_CENTER | GW1K_ALIGN_RIGHT
            | GW1K_ALIGN_JUSTIFY);
        textProps_ |= p;
        text_.setHorizontalAlignment(p);
        break;
    case GW1K_ALIGN_TOP:
    case GW1K_ALIGN_VERT_CENTER:
    case GW1K_ALIGN_BOTTOM:
        textProps_ &= !(GW1K_ALIGN_TOP | GW1K_ALIGN_VERT_CENTER
            | GW1K_ALIGN_BOTTOM);
        textProps_ |= p;
        WManager::getInstance()->registerForPreRenderUpdate(this);
        break;
    }
}


Text&
Label::getTextWidget()
{
    return text_;
}


void
Label::mouseMoved(
    gw1k::MouseMovedEvent ev,
    const gw1k::Point& pos,
    const gw1k::Point& delta,
    gw1k::GuiObject* target)
{
    switch (ev)
    {
    case gw1k::GW1K_M_ENTERED:
        text_.setHovered();
        break;
    case gw1k::GW1K_M_LEFT:
        text_.setHovered(false);
        break;
    default: ;
    }
}

void
Label::mouseClicked(
    gw1k::MouseButton b, gw1k::StateEvent ev, gw1k::GuiObject* target)
{
    switch(ev)
    {
    case gw1k::GW1K_PRESSED:
        text_.setClicked();
        break;
    case gw1k::GW1K_RELEASED:
        text_.setClicked(false);
        break;
    default: ;
    }
}


void
Label::updateVerticalAlignment()
{
    float y;

    if (textProps_ & GW1K_ALIGN_TOP)
    {
        y = 0;
    }
    else
    {
        FTBBox& bb = text_.ftBB_;

        float textHeight = bb.Upper().Yf() - bb.Lower().Yf();
        float labelHeight = getSize().y;

        if (textProps_ & GW1K_ALIGN_BOTTOM)
        {
            y = labelHeight - textHeight;
        }
        else // GW1K_ALIGN_VERT_CENTER or not set
        {
            y = 0.5f * (labelHeight - textHeight);
        }
    }

    // Make sure we pass absolute coordinates and not relative ones
    if (is_fraction(y))
    {
        y = round_pos(y);
    }


    float x;

    if (textProps_ & GW1K_ALIGN_LEFT)
    {
        x = 0;
    }
    else
    {
        FTBBox& bb = text_.ftBB_;

        float textWidth = std::abs(bb.Upper().Xf() - bb.Lower().Xf());
        float labelWidth = getSize().x;

        if (textProps_ & GW1K_ALIGN_RIGHT)
        {
            x = labelWidth - textWidth;
        }
        else // GW1K_ALIGN_CENTER or not set
        {
            x = 0.5f * (labelWidth - textWidth);
        }
    }

    // Make sure we pass absolute coordinates and not relative ones
    if (is_fraction(x))
    {
        x = round_pos(x);
    }

    //text_.setPosition(text_.getPosition().x, y);
    text_.setPos(x, y);
    //text_.setSize()
}


void
Label::setColors(const char* colorScheme)
{
    ThemeManager* t = ThemeManager::getInstance();

    std::string baseName(colorScheme ? colorScheme : "Label");
    t->setColors(this, colorScheme, "Label");
    text_.setColors((baseName + ".Text").c_str());
}


} // namespace gw1k
