#include "GuiObject.h"


#include "WManager.h"
#include "Math.h"
#include "utils/Helpers.h"
#include "Exception.h"
#include "Log.h"
#include <iostream>

#define MSG(x) //std::cout << x << std::endl

namespace gw1k
{


GuiObject::GuiObject()
:   bIsHovered_(false),
    bIsClicked_(false),
    bIsVisible_(true),
    parent_(0),
    bIsEmbedded_(false),
    bContainsMouse_(false),
    bIsDraggable_(false),
    dragAreaPadding_(0, 0),
    bIsInteractive_(true),
    bIsClickThrough_(false),
    dragArea_(0),
    dragChecker_(0)
{}


GuiObject::~GuiObject()
{
    MSG("~GuiObject: " << (void*) this);

    DELETE_PTR(dragArea_);

    // Make sure subobjects don't reference this GuiObject when it is already
    // deleted
    if (subObjects_.size() != 0)
    {
        Log::error("~GuiObject()", Log::os()
            << "Destroying GuiObject that still contains sub-objects. Deleting "
            << "these for you now. If this is not what you intended, or if it "
            << "crashes your application, fix your code to properly remove all "
            << "sub-objects in the respective destructor.");
        removeAndDeleteAllSubObjects();
    }

    // Make sure that widget is not referenced anymore in case it was clicked or
    // hovered (especially important when the "click" removes and deletes the
    // widget, since no "release" must be trigger for it)
    WManager::getInstance()->indicateRemovedObject(this);
}


void
GuiObject::preRenderUpdate()
{
    // Do nothing by default
}


bool
GuiObject::containsMouse(const Point& p) const
{
    return rect_.containsPoint(p);
}


bool
GuiObject::containsMouse() const
{
    return bContainsMouse_;
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

    rect_.pos(round(x), round(y));
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
        else if (width < 1.f)
        {
            width = parSize.x * width;
        }

        if (height < 0.f)
        {
            height = parSize.y - thisPos.y + height;
        }
        else if (height < 1.f)
        {
            height = parSize.y * height;
        }
    }

    rect_.size(round_pos(width), round_pos(height));

    return rect_.size();
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


void
GuiObject::moveBy(const Point& delta)
{
    Point p = getPos() + delta;
    setPos(p.x, p.y);
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
GuiObject::setClickThrough(bool state)
{
    if (state && !bIsInteractive_)
    {
        bIsInteractive_ = true;
    }
    bIsClickThrough_ = state;
}


bool
GuiObject::isClickThrough() const
{
    return bIsClickThrough_;
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

    informMouseListenersMoved(ev, pos, delta, this);
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
        MSG("clicked\t\t" << (void*)this);
        bIsClicked_ = true;
    }
    else
    {
        MSG("released\t" << (void*)this);
        bIsClicked_ = false;
    }

    informMouseListenersClicked(b, ev, this);
}


void
GuiObject::triggerMouseWheelEvent(int delta)
{
    informMouseListenersWheeled(delta, this);
}


void
GuiObject::triggerDraggedEvent(const Point& delta)
{
    informDraggedListeners(delta, this);
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
            return;
        }
    }
    Log::warning("GuiObject", Log::os()
        << "Attempt to remove sub-object " << (void*)o
        << ", but couldn't be found in sub-object list");
}


void
GuiObject::removeAndDeleteSubObject(GuiObject* o)
{
    if (o)
    {
        removeSubObject(o);
        delete o;
    }
}


void
GuiObject::removeAndDeleteAllSubObjects()
{
    for (unsigned int i = 0; i != subObjects_.size(); ++i)
    {
        delete subObjects_[i];
    }
    subObjects_.clear();
}


GuiObject*
GuiObject::getContainingObject(const Point& p)
{
    bool bContainsMouseBefore = bContainsMouse_;
    bContainsMouse_ = bIsVisible_ && containsMouse(p);

    if (bContainsMouse_ && bIsInteractive_)
    {
        GuiObject* containingSubObj = 0;

        // Go through sub-objects and find the backmost one that contains p,
        // since the backmost one is assumed to also be the top-most one
        for (unsigned int i = 0; i != subObjects_.size(); ++i)
        {
            Point offset = getPos();
            GuiObject* o = subObjects_[i]->getContainingObject(p - offset);
            if (o)
            {
                containingSubObj = o;
            }
        }

        return
            (containingSubObj ? containingSubObj : (bIsClickThrough_ ? 0 : this));
    }
    else
    {
        // When mouse has left, recursively inform all sub-objects
        if (bContainsMouseBefore && !bContainsMouse_)
        {
            //MSG("not containing mouse anymore: "<<(void*)this);
            resetSubObjContainsMouseStatus();
        }

        return 0;
    }
}


int
GuiObject::getNumSubObjects() const
{
    return subObjects_.size();
}


void
GuiObject::setParent(GuiObject* parent)
{
    parent_ = parent;

    // Check for correctly embedded objects
    if (bIsEmbedded_)
    {
        while (parent && parent->bIsEmbedded_)
        {
            parent = parent->parent_;
        }
        if (!parent)
        {
            Log::error("GuiObject", Log::os()
                << "Embedded object has no non-embedded parent "
                << "(or no parent at all)! Your application might crash now.");
        }
    }
}


GuiObject*
GuiObject::getParent() const
{
    return parent_;
}


GuiObject*
GuiObject::getNonEmbeddedParent() const
{
    GuiObject* p = parent_;
    while (p && p->bIsEmbedded_)
    {
        p = p->parent_;
    }
    return p;
}


bool
GuiObject::isEmbedded() const
{
    return bIsEmbedded_;
}


void
GuiObject::setEmbedded(bool b)
{
    bIsEmbedded_ = b;
}


void
GuiObject::moveOnTop()
{
    if (parent_)
    {
        parent_->moveOnTop(this);
    }
}


void
GuiObject::setClickedPos(const Point& p)
{
    clickedPos_ = p;
}


const Point&
GuiObject::getClickedPos() const
{
    return clickedPos_;
}


void
GuiObject::setDraggable(bool state)
{
    bIsDraggable_ = state;
}


bool
GuiObject::isDraggable() const
{
    return bIsDraggable_;
}


void
GuiObject::setDraggableArea(const Rect* area, const Point& padding)
{
    if (area)
    {
        if (dragArea_)
        {
            *dragArea_ = *area;
        }
        else
        {
            dragArea_ = new Rect(*area);
        }
    }
    else
    {
        DELETE_PTR(dragArea_);
    }

    dragAreaPadding_ = padding;
}


void
GuiObject::getDraggableArea(Rect* area, Point* padding)
{
    if (area)
    {
        *area = (dragArea_ ? *dragArea_ : (parent_ ? parent_->rect_ : Rect()));
    }

    if (padding)
    {
        *padding = dragAreaPadding_;
    }
}


Point
GuiObject::drag(const Point& relMousePos, MouseButton b)
{
    // We want to move the object such that clickedPos becomes identical to
    // relMousePos; for this, we need to move the object by aspiredDelta
    Point aspiredDelta = relMousePos - clickedPos_;

    (dragChecker_ ? dragChecker_ : this)->checkDragDelta(aspiredDelta, b, this);

    Point newPos = getPos() + aspiredDelta;
    Point newEnd = getEnd() + aspiredDelta;

    const Rect area = dragArea_ ? *dragArea_ : Rect(Point(), parent_->rect_.size());
    bool moveOK = area.containsPoint(newPos - dragAreaPadding_)
        && area.containsPoint(newEnd + dragAreaPadding_);

    Point appliedDelta = aspiredDelta;


    if (!moveOK)
    {
        // We're reaching a border, so check how far we can move and use that as
        // our new position
        Point p = area.pos() + dragAreaPadding_;
        Point e = area.end() - getSize() - dragAreaPadding_;

        newPos = max(p, min(newPos, e));
        appliedDelta = newPos - getPos();
    }

    setPos(newPos.x, newPos.y);

    return appliedDelta;
}


void
GuiObject::checkDragDelta(
    Point& delta,
    MouseButton b,
    const GuiObject* dragReceiver)
{
    // Do nothing in default implementation
}


void
GuiObject::resetSubObjContainsMouseStatus()
{
    for (std::vector<GuiObject*>::iterator i = subObjects_.begin();
        i != subObjects_.end(); ++i)
    {
        if ((*i)->bContainsMouse_)
        {
            (*i)->resetSubObjContainsMouseStatus();
            (*i)->bContainsMouse_ = false;
        }
    }
}


void
GuiObject::moveOnTop(GuiObject* newTopSubObj)
{
    std::vector<GuiObject*>::iterator i = subObjects_.begin();
    unsigned int c = 0;
    for (; (*i != newTopSubObj) && (i != subObjects_.end()); ++i)
    {
        ++c;
    }

    if (i != subObjects_.end())
    {
        for (; c < subObjects_.size() - 1; ++c)
        {
            subObjects_[c] = subObjects_[c + 1];
        }
        subObjects_[c] = newTopSubObj;
    }
}


void
GuiObject::timerExpired(int token)
{
    // Default implementation
}


} // namespace gw1k

