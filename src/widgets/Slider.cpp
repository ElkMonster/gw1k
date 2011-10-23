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




namespace gw1k
{


Slider::Slider(
    const Point& pos,
    const Point& size,
    bool vertical,
    const char* colorScheme)
:   WiBox(pos, size),
#if DEBUG_SLIDER
    handle_(Point(1, 1), (vertical ? Point(size.x - 2, 40) : Point(40, size.y - 2)), "0", 12),
#else
    handle_(Point(1, 1), (vertical ? Point(size.x - 2, 40) : Point(40, size.y - 2))),
#endif
    bVertical_(vertical),
    bEnabled_(true),
    value_(0.f),
    mouseWheelStep_(0.1f)
{
#if DEBUG_SLIDER
    handle_.getTextWidget().setInteractive(false);
#endif

    addSubObject(&handle_);

    handle_.addMouseListener(this);
    addMouseListener(this);

    if ((vertical && (getSize().x > getSize().y))
        || (!vertical && (getSize().x < getSize().y)))
    {
        Log::warning("Slider", Log::os()
            << "Slider dimensions do not seem reasonable "
            << "- confused horizontal and vertical? "
            << "(Slider is " << (bVertical_ ? "vertical" : "horizontal")
            << ", size is " << getSize() << ")");
    }

    setColors(colorScheme);
}


Slider::~Slider()
{

}


float
Slider::getValue() const
{
    return value_;
}


void
Slider::setValue(float val)
{
    float oldValue = value_;
    value_ = std::max(0.f, std::min(1.f, val));

    float hSize = (bVertical_ ? handle_.getSize().y : handle_.getSize().x);
    float size = (bVertical_ ? getSize().y : getSize().x) - 2;
    float z = value_ * (size - hSize) + 1;
    Point newPos = handle_.getPos();

    if (bVertical_)
    {
        newPos.y = z;
    }
    else
    {
        newPos.x = z;
    }
    handle_.setPos(newPos.x, newPos.y);

    if (oldValue != value_)
    {
        informListeners(value_ - oldValue);
    }

#if DEBUG_SLIDER
    std::stringstream ss;
    ss << value_;
    handle_.setText(ss.str());
#endif
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

    if (receiver == &handle_ && handle_.isClicked())
    {
        Point newPos = handle_.getPos();
        if (bVertical_)
        {
            newPos.y += delta.y;
        }
        else
        {
            newPos.x += delta.x;
        }

        setHandlePosition(newPos);
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
        Point newPos = WManager::getInstance()->getMousePos() - getGlobalPos();

        if (bVertical_)
        {
            newPos.x = handle_.getPos().x;
            newPos.y -= handle_.getSize().y / 2;
        }
        else
        {
            newPos.x -= handle_.getSize().x / 2;
            newPos.y = handle_.getPos().y;
        }

        setHandlePosition(newPos);
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
    }
}


void
Slider::addListener(SliderListener* sl)
{
    listeners_.push_back(sl);
}


void
Slider::removeListener(SliderListener* sl)
{
    std::vector<SliderListener*>::iterator i =
        std::find(listeners_.begin(), listeners_.end(), sl);

    if (i != listeners_.end())
    {
        listeners_.erase(i);
    }
}


void
Slider::setColors(const char* colorScheme)
{
    ThemeManager* t = ThemeManager::getInstance();

    std::string baseName(colorScheme ? colorScheme : "Slider");
    t->setColors(this, colorScheme, "Slider");
    handle_.setColors((baseName + ".Handle").c_str());
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
    float hPos = (bVertical_ ? handle_.getPos().y : handle_.getPos().x) - 1;
    float hSize = (bVertical_ ? handle_.getSize().y : handle_.getSize().x);
    float size = (bVertical_ ? getSize().y : getSize().x) - 2;

    value_ = hPos / (size - hSize);
}


void
Slider::setHandlePosition(Point newPos)
{
    Point maxPos = getSize() - handle_.getSize() - Point(1, 1);
    if (bVertical_)
    {
        newPos.y = std::max(1, std::min(newPos.y, maxPos.y));
    }
    else
    {
        newPos.x = std::max(1, std::min(newPos.x, maxPos.x));
    }

    handle_.setPos(newPos.x, newPos.y);

    float oldValue = value_;
    calculateValue();
    if (oldValue != value_)
    {
        informListeners(value_ - oldValue);
    }

#if DEBUG_SLIDER
    std::stringstream ss;
    ss << value_;
    handle_.setText(ss.str());
#endif
}


void
Slider::informListeners(float delta)
{
    for (unsigned int i = 0; i != listeners_.size(); ++i)
    {
        listeners_[i]->sliderValueChanged(this, value_, delta);
    }
}


} // namespace gw1k
