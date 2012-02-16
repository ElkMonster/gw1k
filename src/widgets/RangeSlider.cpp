#include "widgets/RangeSlider.h"

#include "WManager.h"
#include "Math.h"

#include <iostream>
#include <cstdlib>

namespace gw1k
{


RangeSlider::RangeSlider(
    const Point& pos,
    const Point& size,
    const float range[2],
    MapType mapType,
    const char* colorScheme)
:   AbstractSliderBase(pos, size, range, mapType, colorScheme),
    handleSize_(3)
{
    lValue_ = 0.f;
    rValue_ = 1.f;

    float hdlW = std::max(round_pos(size.x * .05f), handleSize_);
    Point hdlSize(hdlW, size.y - 2);

    lHandle_ = new WiBox(Point(1, 1), hdlSize);
    rHandle_ = new WiBox(Point(size.x - 1 - hdlSize.x, 1), hdlSize);

    GuiObject* obj[] = { lHandle_, rHandle_ };
    for (int i = 0; i != 2; ++i)
    {
        obj[i]->addDraggedListener(this);
        obj[i]->setDraggable();
        obj[i]->setDraggableArea(0, Point(1, 1));
    }

    rangeBar_ = new WiBox(Point(), Point());

    GuiObject* obj2[] = { rangeBar_, lHandle_, rHandle_ };
    for (int i = 0; i != 3; ++i)
    {
        addSubObject(obj2[i]);
        obj2[i]->setEmbedded();
        obj2[i]->addMouseListener(this);
    }

    addMouseListener(this);

    updateRangeBar();
    setColors(colorScheme);
}


RangeSlider::~RangeSlider()
{
    removeAndDeleteSubObject(lHandle_);
    removeAndDeleteSubObject(rHandle_);
    removeAndDeleteSubObject(rangeBar_);
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
RangeSlider::calculatePositions(int& left, int& right) const
{
    int w = getSize().x;
    int hw = lHandle_->getSize().x;
    left = round_pos(lValue_ * (w - hw - 2)) + 1;
    right = round_pos(rValue_ * (w - hw - 2)) + 1;
}


const Point&
RangeSlider::setSize(float width, float height)
{
    const Point& newSize = AbstractSliderBase::setSize(width, height);

    float hdlW = std::max(round_pos(newSize.x * .05f), handleSize_);

    lHandle_->setSize(hdlW, newSize.y - 2);
    rHandle_->setSize(hdlW, newSize.y - 2);

    int lx, rx;
    calculatePositions(lx, rx);
    lHandle_->setPos(lx, 1);
    rHandle_->setPos(rx, 1);

    updateRangeBar();

    return newSize;
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
RangeSlider::dragged(const Point& delta, GuiObject* receiver)
{
    if ((receiver == lHandle_) || (receiver == rHandle_))
    {
        restoreConsistency();
        calculateValues();
        informActionListeners(this);
    }
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
