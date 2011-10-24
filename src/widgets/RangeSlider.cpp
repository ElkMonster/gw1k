#include "widgets/RangeSlider.h"

#include "WManager.h"
#include "Math.h"

#include <iostream>

namespace gw1k
{


RangeSlider::RangeSlider(
    const Point& pos,
    const Point& size,
    const float range[2],
    RangeType rangeType,
    const char* colorScheme)
:   AbstractSliderBase(pos, size, range, rangeType, colorScheme),
    handleSize_(3)
{
    lValue_ = 0.f;
    rValue_ = 1.f;

    float hdlW = std::max(round_pos(size.x * .05f), handleSize_);
    Point hdlSize(hdlW, size.y - 2);

    lHandle_ = new WiBox(Point(1, 1), hdlSize);
    rHandle_ = new WiBox(Point(size.x - 1 - hdlSize.x, 1), hdlSize);
    rangeBar_ = new WiBox(Point(), Point());

    addSubObject(rangeBar_);
    addSubObject(lHandle_);
    addSubObject(rHandle_);

    lHandle_->setEmbedded();
    rHandle_->setEmbedded();
    rangeBar_->setEmbedded();

    lHandle_->addMouseListener(this);
    rHandle_->addMouseListener(this);
    rangeBar_->addMouseListener(this);
    addMouseListener(this);

    updateRangeBar();

    setColors(colorScheme);
}


RangeSlider::~RangeSlider()
{
    delete lHandle_;
    delete rHandle_;
    delete rangeBar_;
}


void
RangeSlider::setLValue(float v)
{
    float oldVal = lValue_;
    lValue_ = std::max(getUnmappedValue(v), 0.f);
    if (lValue_ != oldVal)
    {
        informActionListeners(this);
    }
}


float
RangeSlider::getLValue() const
{
    return getMappedValue(lValue_);
}


void
RangeSlider::setRValue(float v)
{
    float oldVal = rValue_;
    rValue_ = std::min(getUnmappedValue(v), 1.f);
    if (rValue_ != oldVal)
    {
        informActionListeners(this);
    }
}


float
RangeSlider::getRValue() const
{
    return getMappedValue(rValue_);
}


void
RangeSlider::getHandles(const WiBox*& lHandle, const WiBox*& rHandle) const
{
    lHandle = lHandle_;
    rHandle = rHandle_;
}


void
RangeSlider::updateRangeBar()
{
    const Point& s = lHandle_->getSize();
    int x = lHandle_->getPos().x + s.x;
    int w = rHandle_->getPos().x - x;
    rangeBar_->setPos(x, 1);
    rangeBar_->setSize(std::max(0, w), s.y);
}


void
RangeSlider::setHandlePos(WiBox* handle, const Point& newPos)
{
    int x = std::max(1, std::min(newPos.x, getSize().x - 1 - handle->getSize().x));
    handle->setPos(x, 1);
}


void
RangeSlider::restoreConsistency()
{
    if (lHandle_->getPos().x > rHandle_->getPos().x)
    {
        std::swap(lHandle_, rHandle_);
    }
    updateRangeBar();
}


void
RangeSlider::calculateValues()
{
    float d = 1.f / (getSize().x - lHandle_->getSize().x - 2);
    lValue_ = d * (lHandle_->getPos().x - 1);
    rValue_ = d * (rHandle_->getPos().x - 1);
    //std::cout
        //<< lValue_ << "  ->  "<< getMappedValue(lValue_) << " / "
        //<< rValue_ << "  ->  "<< getMappedValue(rValue_) << std::endl;
}


void
RangeSlider::mouseMoved(
    MouseMovedEvent ev,
    const Point& pos,
    const Point& delta,
    GuiObject* receiver)
{
    if (receiver->isClicked())
    {
        if ((receiver == lHandle_) || (receiver == rHandle_))
        {
            setHandlePos(dynamic_cast<WiBox*>(receiver),
                receiver->getPos() + delta);
            restoreConsistency();
            calculateValues();
            informActionListeners(this);
        }
        else if (receiver == rangeBar_)
        {

        }
    }
}


void
RangeSlider::mouseClicked(MouseButton b, StateEvent ev, GuiObject* receiver)
{
    if ((ev == GW1K_PRESSED) && ((receiver == this) || (receiver == rangeBar_)))
    {
        Point p = WManager::getInstance()->getMousePos() - getGlobalPos();
        int hdlWidth = lHandle_->getSize().x;
        int dl = std::abs(lHandle_->getPos().x + hdlWidth / 2 - p.x);
        int dr = std::abs(rHandle_->getPos().x + hdlWidth / 2 - p.x);
        WiBox* closestHandle = dl <= dr ? lHandle_ : rHandle_;
        setHandlePos(closestHandle, Point(p.x - hdlWidth / 2, 1));
        restoreConsistency();
        calculateValues();
        informActionListeners(this);
    }
}


void
RangeSlider::mouseWheeled(int delta, GuiObject* receiver)
{

}


void
RangeSlider::setColors(const char* colorScheme)
{
    std::string baseName(colorScheme ? colorScheme : "RangeSlider");
    AbstractSliderBase::setColors(baseName.c_str());
    std::string hdlName = baseName + ".Handle";
    lHandle_->setColors(hdlName.c_str());
    rHandle_->setColors(hdlName.c_str());
    rangeBar_->setColors((baseName + ".RangeBar").c_str());
}


} // namespace gw1k
