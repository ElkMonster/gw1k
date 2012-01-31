#include "widgets/Slider.h"

#include "Math.h"
#include "WManager.h"
#include "Color4i.h"
#include "ThemeManager.h"
#include "Log.h"

#include <algorithm>
#include <iostream>

#if DEBUG_SLIDER
#include <sstream>
#endif

namespace {


const float defaultRange[] = { 0.f, 1.f };


} // namespace


namespace gw1k
{


Slider::Slider(
    const Point& pos,
    const Point& size,
    bool vertical,
    const char* colorScheme)
:   AbstractSliderBase(pos, size, defaultRange, MAP_LINEAR, colorScheme),
    handle_(Point(1, 1), (vertical ? Point(size.x - 2, 40) : Point(40, size.y - 2))),
    bVertical_(vertical),
    bEnabled_(true),
    value_(0.f),
    mouseWheelStep_(0.1f)
{
    addSubObject(&handle_);

    handle_.addMouseListener(this);
    addMouseListener(this);

    setColors(colorScheme);
}


Slider::Slider(
    const Point& pos,
    const Point& size,
    const float range[2],
    bool vertical,
    MapType mapType,
    const char* colorScheme)
:   AbstractSliderBase(pos, size, range, mapType_, colorScheme),
    bVertical_(vertical),
    bEnabled_(true),
    value_(0.f),
    mouseWheelStep_(0.1f)
{
    addSubObject(&handle_);

    handle_.addMouseListener(this);
    addMouseListener(this);

    setColors(colorScheme);
}



Slider::~Slider()
{

}


float
Slider::getValue() const
{
    return getMappedValue(value_);
}


void
Slider::setValue(float val)
{
    float oldVal = value_;
    value_ = std::min(getUnmappedValue(val), 1.f);

    if (value_ != oldVal)
    {
        informActionListeners(this);
    }
}


void
Slider::setEnabled(bool enabled)
{
    bEnabled_ = enabled;
    handle_.setVisible(enabled);
}


void
Slider::setHandleSize(float size)
{
    const Point& sliderSize = getSize();

    float actualSize = size;
    if (bVertical_)
    {
        if (is_fraction(size))
        {
            actualSize = (sliderSize.y - 2) * size;
        }

        handle_.setSize(handle_.getSize().x, round_pos(actualSize));
    }
    else
    {
        if (is_fraction(size))
        {
            actualSize = (sliderSize.x - 2) * size;
        }

        handle_.setSize(round_pos(actualSize), handle_.getSize().y);
    }
}


void
Slider::mouseMoved(
    MouseMovedEvent ev, const Point& pos, const Point& delta, GuiObject* receiver)
{
    if (!bEnabled_)
    {
        return;
    }

    if ((receiver == &handle_) && handle_.isClicked())
    {
        int newPos = 0;
        const Point& handlePos = handle_.getPos();
        if (bVertical_)
        {
            newPos = handlePos.y + delta.y;
        }
        else
        {
            newPos = handlePos.x + delta.x;
        }

        setHandlePosition(newPos);
        calculateValue();
        informActionListeners(this);
    }
}


void
Slider::mouseClicked(MouseButton b, StateEvent ev, GuiObject* receiver)
{
    if (!bEnabled_)
    {
        return;
    }

    if ((receiver == this) && (ev == GW1K_PRESSED))
    {
        Point relPos = WManager::getInstance()->getMousePos() - getGlobalPos();
        int newPos = 0;

        if (bVertical_)
        {
            newPos = relPos.y - handle_.getSize().y / 2;
        }
        else
        {
            newPos = relPos.x - handle_.getSize().x / 2;
        }

        setHandlePosition(newPos);
        calculateValue();
        informActionListeners(this);
    }
}


void
Slider::mouseWheeled(int delta, GuiObject* receiver)
{
    if ((receiver == &handle_) || (receiver == this))
    {
        // Sliders need to invert delta because a value of 0 = top/left, a value
        // of 1 = bottom/right, but deltas work the other way round
        setValue(value_ + (mouseWheelStep_ * -delta));
        setHandlePosition(calculateHandlePos());
        informActionListeners(this);
    }
}


void
Slider::setColors(const char* colorScheme)
{
    std::string baseName(colorScheme ? colorScheme : "Slider");
    AbstractSliderBase::setColors(baseName.c_str());
    std::string hdlName = baseName + ".Handle";
    handle_.setColors(hdlName.c_str());
}


void
Slider::setMouseWheelStep(float step)
{
    mouseWheelStep_ = std::max(0.f, std::min(1.f, step));
}


float
Slider::getMouseWheelStep() const
{
    return mouseWheelStep_;
}


void
Slider::calculateValue()
{
    float handlePos = (bVertical_ ? handle_.getPos().y : handle_.getPos().x) - 1;
    float handleSize = (bVertical_ ? handle_.getSize().y : handle_.getSize().x);
    float size = (bVertical_ ? getSize().y : getSize().x) - 2;

    value_ = handlePos / (size - handleSize);
}


void
Slider::setHandlePosition(int newPos)
{
    Point maxPos = getSize() - handle_.getSize() - Point(1, 1);
    Point p(1, 1);
    if (bVertical_)
    {
        p.y = std::max(1, std::min(newPos, maxPos.y));
    }
    else
    {
        p.x = std::max(1, std::min(newPos, maxPos.x));
    }

    handle_.setPos(p.x, p.y);
}


int
Slider::calculateHandlePos() const
{
    float handleSize = (bVertical_ ? handle_.getSize().y : handle_.getSize().x);
    float size = (bVertical_ ? getSize().y : getSize().x) - 2;

    return value_ * (size - handleSize) + 1;
}


} // namespace gw1k
