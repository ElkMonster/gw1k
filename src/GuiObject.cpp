#include "GuiObject.h"


#include <algorithm>
#include "Math.h"
#include "Helpers.h"
#include "Exception.h"
#include <iostream>

#define MSG(x) std::cout << x << std::endl

namespace gw1k
{


typedef std::list<MouseListener*>::iterator MListIter;
typedef std::list<KeyListener*>::iterator KListIter;


GuiObject::GuiObject()
:   bIsHovered_(false),
    bIsClicked_(false),
    bIsVisible_(true),
    parent_(0),
    bIsInteractive_(true)
{}


GuiObject::~GuiObject()
{
    for (MListIter i = mouseListeners_.begin(); i != mouseListeners_.end(); ++i)
    {
        delete *i;
    }

    for (KListIter i = keyListeners_.begin(); i != keyListeners_.end(); ++i)
    {
        delete *i;
    }
}


void
GuiObject::update()
{
    // Do nothing by default
}


void
GuiObject::addMouseListener(MouseListener* ml)
{
    mouseListeners_.push_back(ml);
}


void
GuiObject::removeMouseListener(MouseListener* ml)
{

    MListIter i = std::find(mouseListeners_.begin(), mouseListeners_.end(), ml);

    if (i != mouseListeners_.end())
    {
        mouseListeners_.remove(ml);
    }
}


void
GuiObject::addKeyListener(KeyListener* ml)
{
    keyListeners_.push_back(ml);
}


void
GuiObject::removeKeyListener(KeyListener* kl)
{

    KListIter i = std::find(keyListeners_.begin(), keyListeners_.end(), kl);

    if (i != keyListeners_.end())
    {
        keyListeners_.remove(kl);
    }
}


void
GuiObject::addResizeListener(ResizeListener* rl)
{
    resizeListeners_.push_back(rl);
}


void
GuiObject::removeResizeListener(ResizeListener* rl)
{
    for (std::vector<ResizeListener*>::iterator i = resizeListeners_.begin();
        i != resizeListeners_.end(); ++i)
    {
        if (*i == rl)
        {
            resizeListeners_.erase(i);
            break;
        }
    }
}


bool
GuiObject::containsMouse(const Point& p) const
{
    return rect_.containsPoint(p);
}


void
GuiObject::setPos(float x, float y)
{
    if (parent_)
    {
        if (is_fraction(x))
        {
            x = parent_->rect_.size().x * x;
        }

        if (is_fraction(y))
        {
            y = parent_->rect_.size().y * y;
        }
    }

    rect_.pos(round_pos(x), round_pos(y));
}


const Point&
GuiObject::setSize(float width, float height)
{
    if (parent_)
    {
        const Point& parSize = parent_->rect_.size();
        const Point& thisPos = rect_.pos();
        if (width < 0.f)
        {
            width = parSize.x - thisPos.x + width;
        }
        else if (width <= 1.f)
        {
            width = parSize.x * width;
        }

        if (height < 0.f)
        {
            height = parSize.y - thisPos.y + height;
        }
        else if (height <= 1.f)
        {
            height = parSize.y * height;
        }
    }

    rect_.size(round_pos(width), round_pos(height));

    const Point& newSize = rect_.size();
    for (unsigned int i = 0; i != resizeListeners_.size(); ++i)
    {
        resizeListeners_[i]->notifyResized(this, newSize);
    }

    return newSize;
}


const Point&
GuiObject::getPos() const
{
    return rect_.pos();
}


const Point&
GuiObject::getSize() const
{
    return rect_.size();
}


const Point&
GuiObject::getEnd() const
{
    return rect_.end();
}


Point
GuiObject::getGlobalPos() const
{
    Point p = getPos();

    GuiObject* parent = parent_;
    while (parent != 0)
    {
        p += parent->getPos();
        parent = parent->parent_;
    }

    return p;
}


bool
GuiObject::isHovered() const
{
    return bIsHovered_;
}


void
GuiObject::setHovered(bool state)
{
    bIsHovered_ = state;
}


bool
GuiObject::isClicked() const
{
    return bIsClicked_;
}


void
GuiObject::setClicked(bool state)
{
    bIsClicked_ = state;
}


void
GuiObject::setVisible(bool state)
{
    bIsVisible_ = state;
}


bool
GuiObject::isVisible() const
{
    return bIsVisible_;
}

void
GuiObject::setInteractive(bool state)
{
    bIsInteractive_ = state;
}


bool
GuiObject::isInteractive() const
{
    return bIsInteractive_;
}


void
GuiObject::triggerMouseMovedEvent(
    MouseMovedEvent ev, const Point& pos, const Point& delta)
{
    if (ev == GW1K_M_LEFT)
    {
        MSG("left\t\t" << (void*)this);
        bIsHovered_ = false;
    }
    else if (!bIsHovered_)
    {
        MSG("hovered\t\t" << (void*)this);
        bIsHovered_ = true;
    }

    for (MListIter i = mouseListeners_.begin(); i != mouseListeners_.end(); ++i)
    {
        (*i)->mouseMoved(ev, pos, delta, this);
    }
}


void
GuiObject::triggerMouseButtonEvent(MouseButton b, StateEvent ev)
{
    // Note: This doesn't handle multiple clicks at the same time (e.g., left
    // and right button pressed at the same time or after another without
    // releasing one), so clicked status will be set to false even though the
    // second button is still pressed
    if (ev == GW1K_PRESSED)
    {
        //MSG("clicked\t\t" << (void*)this);
        bIsClicked_ = true;
    }
    else
    {
        //MSG("released\t" << (void*)this);
        bIsClicked_ = false;
    }

    for (MListIter i = mouseListeners_.begin(); i != mouseListeners_.end(); ++i)
    {
        (*i)->mouseClicked(b, ev, this);
    }
}


void
GuiObject::addSubObject(GuiObject* o)
{
    if (o->parent_)
    {
        throw Exception(
            "GuiObject::addSubObject: Object to be added must not have a parent");
    }
    else if (o == this)
    {
        throw Exception(
            "GuiObject::addSubObject: Object cannot be added to itself");
    }
    else
    {
        o->parent_ = this;
    }

    subObjects_.push_back(o);
}


void
GuiObject::removeSubObject(GuiObject* o)
{
    for (std::vector<GuiObject*>::iterator i = subObjects_.begin();
        i != subObjects_.end(); ++i)
    {
        if (*i == o)
        {
            subObjects_.erase(i);
            o->parent_ = 0;
            break;
        }
    }
}


GuiObject*
GuiObject::getContainingObject(const Point& p)
{
    Point cp = p;
    if (bIsVisible_ && bIsInteractive_ && containsMouse(p))
    {
        for (unsigned int i = 0; i != subObjects_.size(); ++i)
        {
            Point offset = getPos();
            GuiObject* o = subObjects_[i]->getContainingObject(p - offset);
            if (o)
            {
                return o;
            }
        }

        return this;
    }
    else
    {
        return 0;
    }
}


int
GuiObject::getNumSubObjects() const
{
    return subObjects_.size();
}


} // namespace gw1k
