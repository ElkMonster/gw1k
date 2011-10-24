#include "widgets/ClippingBox.h"

#include "WManager.h"
#include "ThemeManager.h"

#include <GL/gl.h>
#include <iostream>

namespace gw1k
{


ClippingBox::ClippingBox(
    const Point& pos,
    const Point& size,
    ScrollPane::AutoAdjustSize autoAdjustSize
)
:   Box(pos, size),
    clippingOffset_(0, 0),
    realOrigin_(0, 0),
    realSize_(size),
    subObjAccommodationStatus_(1, 1),
    autoAdjustSize_(autoAdjustSize)
{}


ClippingBox::~ClippingBox()
{}


const Point&
ClippingBox::setSize(float width, float height)
{
    const Point& size = Box::setSize(width, height);
    if (autoAdjustSize_ != ScrollPane::ADJUST_NONE)
    {
        autoAdjustSubObjs();
        if (autoAdjustSize_ == ScrollPane::ADJUST_HORIZ)
        {
            realSize_.x = width;
        }
        else // autoAdjustSize == ScrollPane::ADJUST_VERT
        {
            realSize_.y = height;
        }
    }
    checkAccommodation();
    return size;
}


void
ClippingBox::setClippingOffset(const Point& offset)
{
    clippingOffset_ = offset;
}


const Point&
ClippingBox::getClippingOffset() const
{
    return clippingOffset_;
}


const Point&
ClippingBox::getRealSize() const
{
    return realSize_;
}


const Point&
ClippingBox::getRealOrigin() const
{
    return realOrigin_;
}


void
ClippingBox::renderSubObjects(const Point& offset) const
{
    if (subObjects_.size() != 0)
    {
        glPushMatrix();
        {
            Point p = realOrigin_ + clippingOffset_;
            glTranslatef(-p.x, -p.y, 0.f);

            WManager::getInstance()->pushGlScissorOffset(p);

            // Position of "this" will be added to offset in the Renderable
            // implementation, therefore do not add anything to offset here
            Box::renderSubObjects(offset);

            WManager::getInstance()->popGlScissorOffset();
        }
        glPopMatrix();
    }
}


void
ClippingBox::addSubObject(GuiObject* o)
{
    if (autoAdjustSize_ != ScrollPane::ADJUST_NONE)
    {
        autoAdjustSubObj(o);
    }

    Point realEnd = realOrigin_ + realSize_;

    Point newRealOrigin(min(o->getPos(), realOrigin_));
    Point newRealSize = Point(max(getEnd(), max(o->getEnd(), realEnd))) - newRealOrigin;

    if (autoAdjustSize_ == ScrollPane::ADJUST_HORIZ)
    {
        newRealOrigin.x = 0;
        newRealSize.x = getSize().x;
    }
    else if (autoAdjustSize_ == ScrollPane::ADJUST_VERT)
    {
        newRealOrigin.y = 0;
        newRealSize.y = getSize().y;
    }

    // Keep viewing window at its position
    clippingOffset_ += realOrigin_ - newRealOrigin;

    realOrigin_ = newRealOrigin;
    realSize_ = newRealSize;

    checkAccommodation();

    GuiObject::addSubObject(o);
    o->setParent(this->parent_);
}


void
ClippingBox::removeSubObject(GuiObject* o)
{
    GuiObject::removeSubObject(o);
    // TODO keep viewing window at its position like in addSubObject
    recalculateBounds();
}


GuiObject*
ClippingBox::getContainingObject(const Point& p)
{
    //std::cout << "getContainingObject " << (p + realOrigin_ + clippingOffset_) << std::endl;
    GuiObject* o =
        GuiObject::getContainingObject(p + realOrigin_ + clippingOffset_);

    // Since ClippingBox' parent is the one which gets added sub-objects, at
    // least from the user's perspective, we also have to return that parent as
    // the containing object if none of the sub-objects is hit
    return (o != this ? o : parent_);
}


bool
ClippingBox::containsMouse(const Point& p) const
{
    Point realEnd = realOrigin_ + realSize_;
    return (p.x >= realOrigin_.x) && (p.y >= realOrigin_.y) && (p.x < realEnd.x)
        && (p.y < realEnd.y);
}


void
ClippingBox::recalculateBounds()
{
    Point minPos = realOrigin_ + realSize_;
    Point maxPos = realOrigin_;
    Point oldRealOrigin = realOrigin_;

    for (unsigned int i = 0; i != subObjects_.size(); ++i)
    {
        GuiObject* q = subObjects_[i];
        const Point& pos = q->getPos();
        const Point& end = q->getEnd();

        if (pos.x < minPos.x) minPos.x = pos.x;
        if (pos.y < minPos.y) minPos.y = pos.y;
        if (end.x > maxPos.x) maxPos.x = end.x;
        if (end.y > maxPos.y) maxPos.y = end.y;
    }

    // Keep viewing window at its position
    clippingOffset_ += oldRealOrigin - realOrigin_;

    const Point& boxEnd = getEnd();

    // Don't let things get smaller than the visible window
    realOrigin_ = min(minPos, Point(0, 0));
    realSize_ = max(maxPos, boxEnd) - realOrigin_;

    checkAccommodation();
}


Point
ClippingBox::getAccommodationStatus() const
{
    return subObjAccommodationStatus_;
}


ScrollPane::AutoAdjustSize
ClippingBox::getAutoAdjustSize() const
{
    return autoAdjustSize_;
}


void
ClippingBox::checkAccommodation()
{
    const Point& size = getSize();
    subObjAccommodationStatus_.x = (size.x >= realSize_.x);
    subObjAccommodationStatus_.y = (size.y >= realSize_.y);
}


void
ClippingBox::autoAdjustSubObjs()
{
    for (unsigned int i = 0; i != subObjects_.size(); ++i)
    {
        autoAdjustSubObj(subObjects_[i]);
    }
}


void
ClippingBox::autoAdjustSubObj(GuiObject* o)
{
    const Point& end = getEnd();
    const Point& oPos = o->getPos();
    const Point& oSize = o->getSize();

    Point adjustedSize = oSize;
    bool bNeedsAdjustment = false;

    if (autoAdjustSize_ == ScrollPane::ADJUST_HORIZ)
    {
        if (oPos.x + oSize.x != end.x)
        {
            adjustedSize.x = end.x - oPos.x;
            bNeedsAdjustment = true;
        }
    }
    else // ADJUST_VERT
    {
        if (oPos.y + oSize.y != end.y)
        {
            adjustedSize.y = end.y - oPos.y;
            bNeedsAdjustment = true;
        }
    }

    if (bNeedsAdjustment)
    {
        o->setSize(adjustedSize.x, adjustedSize.y);
    }
}


} // namespace gw1k
