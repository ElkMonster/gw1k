#include "WManager.h"

#include <GL/gl.h>
#include <iostream>

#define MSG(x) std::cout << x << std::endl


namespace gw1k
{


/*static*/
WManager* WManager::pInstance(0);


/*static*/
WManager*
WManager::getInstance()
{
    return pInstance ? pInstance : (pInstance = new WManager());
}


WManager::WManager()
:   hoveredObject_(0),
    clickedObject_(0),
    mainWin_(new Box(Point(), Point()))
{}


WManager::~WManager()
{
    delete mainWin_;
}


void
WManager::feedMouseMove(int x, int y)
{
    Point delta(x - mousePos_.x, y - mousePos_.y);
    mousePos_.x = x;
    mousePos_.y = y;
    feedMouseMoveInternal(mousePos_, delta, NULL);
}


void
WManager::feedMouseMoveInternal(const Point& pos, const Point& delta, GuiObject* o)
{
    // A clicked object receives all the mouse movement events regardless of the
    // mouse leaving it. This allows for things like sliders which will follow
    // mouse movement even if their area is left.
    if (clickedObject_)
    {
        clickedObject_->triggerMouseMovedEvent(GW1K_M_HOVERED, pos, delta);
    }
    else
    {
        if (!o)
        {
            o = mainWin_->getContainingObject(pos);
        }

        // Trigger Mouse-Left event for element that was previously hovered, but
        // is not anymore (because mouse totally left object or is hovering a
        // subobject now)
        if (hoveredObject_ && (hoveredObject_ != o))
        {
            hoveredObject_->triggerMouseMovedEvent(GW1K_M_LEFT, pos, delta);
            hoveredObject_ = 0;
        }

        // Trigger Mouse-Hovered or Mouse-Entered event for element that is
        // currently hovered and remember it for next round
        if (o)
        {
            o->triggerMouseMovedEvent(
                (o->isHovered() ? GW1K_M_HOVERED : GW1K_M_ENTERED), pos, delta);
            hoveredObject_ = o;
        }

    }
}


void
WManager::feedMouseClick(MouseButton b, StateEvent ev)
{
    bool eventHandled = false;

    // Trigger Release for object that was previously clicked (this makes sure
    // that the object is released even if it has moved away when it was clicked)
    if ((ev == GW1K_RELEASED) && clickedObject_)
    {
        clickedObject_->triggerMouseButtonEvent(b, ev);
        clickedObject_ = 0;

        // Don't let Release be triggered for another object
        eventHandled = true;
    }

    // Enable hover state for the object actually hovered now
    GuiObject* currentHoveredObj = mainWin_->getContainingObject(mousePos_);
    if (currentHoveredObj != hoveredObject_)
    {
        // Updates hoveredObject_
        feedMouseMoveInternal(mousePos_, Point(), currentHoveredObj);
    }

    // Trigger event for object that is currently hovered (only that can
    // possibly be the target of mouse clicks)
    if (!eventHandled && currentHoveredObj)
    {
        currentHoveredObj->triggerMouseButtonEvent(b, ev);
        clickedObject_ = currentHoveredObj;
    }
}

void
WManager::feedKey(StateEvent ev, int key)
{

}

void
WManager::setWindowSize(int width, int height)
{
    winSize_.x = width;
    winSize_.y = height;
    mainWin_->setSize(width, height);
}


const Point&
WManager::getWindowSize() const
{
    return winSize_;
}


const Point&
WManager::getMousePos() const
{
    return mousePos_;
}


void
WManager::addObject(GuiObject* o)
{
    mainWin_->addSubObject(o);
}


void
WManager::removeObject(GuiObject* o)
{
    mainWin_->addSubObject(o);
}


void
WManager::pushGlScissor(const Point& pos, const Point& size)
{
    scissorStack_.pushGlScissor(pos, size);
}


void
WManager::popGlScissor()
{
    scissorStack_.popGlScissor();
}


void
WManager::pushGlScissorOffset(const Point& offset)
{
    scissorStack_.pushGlScissorOffset(offset);
}


void
WManager::popGlScissorOffset()
{
    scissorStack_.popGlScissorOffset();
}


void
WManager::render()
{
    //MSG("WManager::render()");

    for (unsigned int i = 0; i != preRenderUpdateQueue_.size(); ++i)
    {
        preRenderUpdateQueue_[i]->update();
    }
    preRenderUpdateQueue_.clear();

    mainWin_->render(mainWin_->getPos());
}


void
WManager::registerForPreRenderUpdate(GuiObject* o)
{
    preRenderUpdateQueue_.push_back(o);
}


} // namespace gw1k
