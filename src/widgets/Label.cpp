#include "widgets/Label.h"

#include "Gw1kConstants.h"
#include "Math.h"
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
    bool autoSize,
    const char* colorScheme)
:   WiBox(pos, size),
    textBox_(Point(1, 1), size - Point(2, 2)),
    padding_(0, 0),
    textProps_(0),
    bAutoSized_(autoSize),
    text_(padding_, text),
    bVCenterVisually_(true),
    lineLength_(-1)
{
    int fSize = (faceSize < 0) ? std::max(1, size.y - 2) : faceSize;
    text_.setFont(fontName, fSize);
    setTextProperty(GW1K_ALIGN_CENTER);
    setTextProperty(GW1K_ALIGN_VERT_CENTER);

    if (bAutoSized_)
    {
        adaptToTextSize();
    }
    else
    {
        Point s = size - Point(2, 2) - padding_ - padding_;
        text_.setSize(s.x, s.y);
        updateTextAlignment();
    }

    addSubObject(&textBox_);
    textBox_.setEmbedded();
    textBox_.addSubObject(&text_);

    addMouseListener(this);

    setColors(colorScheme);
}


Label::~Label()
{}


void
Label::setAutoSized(bool b)
{
    if (!bAutoSized_ && b)
    {
        text_.setSize(0.f, 999999.f);
        adaptToTextSize();
    }
    else if (bAutoSized_ && !b)
    {
        const Point& size = getSize();
        text_.setSize(size.x - 2 - 2 * padding_.x, size.y - 2 - 2 * padding_.y);
        updateTextAlignment();
    }
    bAutoSized_ = b;
}


bool
Label::isAutoSized() const
{
    return bAutoSized_;
}


void
Label::setWrapText(bool wrap)
{
    if (wrap && (lineLength_ < 0))
    {
        updateWrappedTextLayout();
    }
    else if (!wrap && (lineLength_ >= 0))
    {
        // Disable wrapping, reset text to one line
        lineLength_ = -1;
        text_.setSize(0, 0);
    }

    if (bAutoSized_)
    {
        adaptToTextSize();
    }
}


void
Label::setText(const std::string& text)
{
    text_.setText(text);

    if (bAutoSized_)
    {
        adaptToTextSize();
    }
    else
    {
        // Update could be necessary if text's length has changed so much that
        // the number of lines changed
        updateTextAlignment();
    }
}


void
Label::setPadding(const Point& padding)
{
    padding_ = padding;

    if (lineLength_ >= 0)
    {
        updateWrappedTextLayout();
    }

    if (bAutoSized_)
    {
        adaptToTextSize();
    }
    else
    {
        const Point& s = getSize();
        setSize(s.x, s.y);
        updateTextAlignment();
    }
}


const Point&
Label::getPadding() const
{
    return padding_;
}


const Point&
Label::setSize(float width, float height)
{
    if (bAutoSized_)
    {
        // lineLength_ < 0: do nothing
        if (lineLength_ >= 0) // Wrapping is enabled
        {
            const Point& size = WiBox::setSize(width, height);
            lineLength_ = std::max(size.x - 2 - 2 * padding_.x, 1);
            text_.setSize(lineLength_, 0);
            adaptToTextSize();
            updateTextAlignment();
        }
    }
    else
    {
        const Point& size = WiBox::setSize(width, height);
        textBox_.setSize(size.x - 2, size.y - 2);

        if (lineLength_ < 0)
        {
            text_.setSize(0, 0);
        }
        else
        {
            lineLength_ = std::max(size.x - 2 - 2 * padding_.x, 1);
            text_.setSize(lineLength_, 0);
        }
        updateTextAlignment();

    }

    return getSize();
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

        textProps_ &= 0xFFFFFFFF ^ (GW1K_ALIGN_LEFT | GW1K_ALIGN_CENTER | GW1K_ALIGN_RIGHT
            | GW1K_ALIGN_JUSTIFY);
        textProps_ |= p;
        text_.setHorizontalAlignment(p);
        break;
    case GW1K_ALIGN_TOP:
    case GW1K_ALIGN_VERT_CENTER:
    case GW1K_ALIGN_BOTTOM:
        textProps_ &=0xFFFFFFFF ^ (GW1K_ALIGN_TOP | GW1K_ALIGN_VERT_CENTER
            | GW1K_ALIGN_BOTTOM);
        textProps_ |= p;
        break;
    }

    updateTextAlignment();
}


void
Label::adaptToTextSize()
{
    Point s = text_.getSize() + padding_ + padding_ + Point(2, 2);

    WiBox::setSize(s.x, s.y);
    textBox_.setSize(s.x - 2, s.y - 2);

    updateTextAlignment();
}


void
Label::updateWrappedTextLayout()
{
    // Enable wrapping by setting it to a value >= 0, even if not enough space
    // due to padding
    lineLength_ = std::max(1, textBox_.getSize().x - 2 * padding_.x);
    text_.setSize(lineLength_, 0);
}


Text&
Label::getTextWidget()
{
    return text_;
}


void
Label::mouseMoved(
    MouseMovedEvent ev,
    const Point& pos,
    const Point& delta,
    GuiObject* receiver)
{
    switch (ev)
    {
    case GW1K_M_ENTERED:
        text_.setHovered();
        break;
    case GW1K_M_LEFT:
        text_.setHovered(false);
        break;
    default: ;
    }
}


void
Label::mouseClicked(MouseButton b, StateEvent ev, GuiObject* receiver)
{
    switch(ev)
    {
    case GW1K_PRESSED:
        text_.setClicked();
        break;
    case GW1K_RELEASED:
        text_.setClicked(false);
        break;
    default: ;
    }
}


void
Label::mouseWheeled(int delta, GuiObject* receiver)
{

}


void
Label::updateTextAlignment()
{
    float x = 0;
    float y = 0;

    if (!bAutoSized_)
    {
        const Point& textSize = text_.getSize();

        // GW1K_ALIGN_TOP -> keep y = 0;
        if (!(textProps_ & GW1K_ALIGN_TOP))
        {

            float availableHeight = textBox_.getSize().y - 2 * padding_.y;

            if (textProps_ & GW1K_ALIGN_BOTTOM)
            {
                y = availableHeight - textSize.y;
            }
            else // GW1K_ALIGN_VERT_CENTER or not set
            {
                y = 0.5f * (availableHeight - textSize.y);
                if (bVCenterVisually_)
                {
                    y += 0.1f * text_.getFontSize();
                }
            }
        }

        // Make sure we pass absolute coordinates and not relative ones
        if (is_fraction(y))
        {
            y = round_pos(y);
        }

        // GW1K_ALIGN_LEFT -> keep x = 0;
        if (!(textProps_ & GW1K_ALIGN_LEFT))
        {
            float availableWidth = textBox_.getSize().x - 2 * padding_.x;

            if (textProps_ & GW1K_ALIGN_RIGHT)
            {
                x = availableWidth - textSize.x;
            }
            else // GW1K_ALIGN_CENTER or not set
            {
                x = 0.5f * (availableWidth - textSize.x);
            }
        }

        // Make sure we pass absolute coordinates and not relative ones
        if (is_fraction(x))
        {
            x = round_pos(x);
        }
    }

    text_.setPos(x + padding_.x, y + padding_.y);
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
