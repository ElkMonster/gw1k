#include "WManager.h"

#include <GL/gl.h>
#include <iostream>

//#define MSG(x) std::cout << x << std::endl
#define MSG(x)


namespace
{


inline gw1k::GuiObject* getNonEmbeddedParent(const gw1k::GuiObject* o)
{
    gw1k::GuiObject* p = o->getParent();
    while (p && p->isEmbedded())
    {
        p = p->getParent();
    }
    return p;
}


} // namespace


namespace gw1k
{


/*static*/
WManager* WManager::pInstance_(0);


/*static*/
WManager*
WManager::getInstance()
{
    return pInstance_ ? pInstance_ : (pInstance_ = new WManager());
}


WManager::WManager()
:   hoveredObj_(0),
    clickedObj_(0),
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
    MSG("WManager::feedMouseMoveInternal [begin]: o = " << (void*)o);
    // A clicked object receives all the mouse movement events regardless of the
    // mouse leaving it. This allows for things like sliders which will follow
    // mouse movement even if their area is left.
    if (clickedObj_)
    {
        MSG("WManager::feedMouseMoveInternal: clickedObj_ = " << (void*)clickedObj_);
        clickedObj_->triggerMouseMovedEvent(GW1K_M_HOVERED, pos, delta);

        if (clickedObj_->isEmbedded())
        {
            getNonEmbeddedParent(clickedObj_)
                ->triggerMouseMovedEvent(GW1K_M_HOVERED, pos, delta);
        }
    }
    else
    {
        if (!o)
        {
            o = mainWin_->getContainingObject(pos);
        }

        //GuiObject* prevHoveredObj = hoveredObj_;

        // Trigger Mouse-Left event for element that was previously hovered, but
        // is not anymore (because the mouse totally left the object, or is
        // hovering a sub-object now, or the object moved/disappeared)
        if (hoveredObj_ != o)
        {
            //MSG("WManager::feedMouseMoveInternal: hoveredObj_ = " << (void*)hoveredObj_ << ", o = " << (void*)o << ", setting hoveredObj_ to o " << (void*)o);
            feedMouseMoveHandleOldHoveredObj(pos, delta, o);
            hoveredObj_ = o;
        }

        // Trigger Mouse-Hovered or Mouse-Entered event for element that is
        // currently hovered
        feedMouseMoveHandleNewHoveredObj(pos, delta);
    }

    MSG("WManager::feedMouseMoveInternal [end]");
}


void
WManager::feedMouseMoveHandleOldHoveredObj(
    const Point& pos,
    const Point& delta,
    const GuiObject* newHoveredObj)
{
    if (hoveredObj_)
    {
        if (hoveredObj_->isEmbedded())
        {
            hoveredObj_->triggerMouseMovedEvent(GW1K_M_LEFT, pos, delta);

            // Check if the containing non-embedded parent is also left; this is
            // either true when there is no new hovered object, or when the new
            // hovered object is not embedded, or when it is embedded, but in
            // another parent
            GuiObject* hPar = getNonEmbeddedParent(hoveredObj_);
            GuiObject* nPar = 0;
            if (newHoveredObj && newHoveredObj->isEmbedded())
            {
                nPar = getNonEmbeddedParent(newHoveredObj);
            }

            if (!newHoveredObj || (hPar != nPar))
            {
                hPar->triggerMouseMovedEvent(GW1K_M_LEFT, pos, delta);
            }
        }
        else // hoveredObj_ NOT embedded
        {
            // If new hovered object is an embedded sub-object of hoveredObj_,
            // do nothing (that is, no LEFT event), otherwise trigger LEFT event
            bool bIsEmbeddedSubObj = newHoveredObj && newHoveredObj->isEmbedded()
                && (getNonEmbeddedParent(newHoveredObj) == hoveredObj_);
            if (!bIsEmbeddedSubObj)
            {
                hoveredObj_->triggerMouseMovedEvent(GW1K_M_LEFT, pos, delta);
            }
        }
    }
}


void
WManager::feedMouseMoveHandleNewHoveredObj(
    const Point& pos,
    const Point& delta)
{
    if (hoveredObj_)
    {
        MSG("WManager::feedMouseMoveInternal: o = " << (void*)o << ", hoveredObj_ = " << (void*)hoveredObj_);
        hoveredObj_->triggerMouseMovedEvent(
            hoveredObj_->isHovered() ? GW1K_M_HOVERED : GW1K_M_ENTERED, pos,
            delta);

        if (hoveredObj_->isEmbedded())
        {
            GuiObject* hPar = getNonEmbeddedParent(hoveredObj_);
            hPar->triggerMouseMovedEvent(
                hPar->isHovered() ? GW1K_M_HOVERED : GW1K_M_ENTERED, pos, delta);
        }
    }
}


void
WManager::feedMouseClick(MouseButton b, StateEvent ev)
{
    MSG("WManager::feedMouseClick [begin]: ev = " << (ev == GW1K_RELEASED ? "RELEASED" : "PRESSED"));
    bool eventHandled = false;

    // Trigger Release for object that was previously clicked (this makes sure
    // that the object is released even if it has moved away when it was clicked)
    if (ev == GW1K_RELEASED)
    {
        if (clickedObj_)
        {
            clickedObj_->triggerMouseButtonEvent(b, ev);

            if (clickedObj_->isEmbedded())
            {
                getNonEmbeddedParent(clickedObj_)->triggerMouseButtonEvent(b, ev);
            }
            clickedObj_ = 0;
        }

        // Don't let Release be triggered for another object
        eventHandled = true;
    }

    // Enable hover state for the object actually hovered now
    GuiObject* currHoveredObj = mainWin_->getContainingObject(mousePos_);
    MSG("WManager::feedMouseClick: currHoveredObj = " << (void*)currHoveredObj << ", hoveredObj_ = " << (void*)hoveredObj_);
    if (currHoveredObj != hoveredObj_)
    {
        MSG("WManager::feedMouseClick:   --> update hoveredObj_ with feedMouseMoveInternal");
        // Update hoveredObj_ using a fake mouse move with a delta of 0
        feedMouseMoveInternal(mousePos_, Point(0, 0), currHoveredObj);
    }

    // Trigger event for object that is currently hovered (only that can
    // possibly be the receiver of mouse clicks)
    if (!eventHandled && currHoveredObj)
    {
        // Order matters: clickedObj_ may be reset to 0 in the triggered
        // method in case that the clicked object is removed due to being
        // clicked (i.e., indicateRemovedObject() is called for it)
        clickedObj_ = currHoveredObj;
        MSG("WManager::feedMouseClick: clickedObj_ = " << (void*)clickedObj_);
        currHoveredObj->triggerMouseButtonEvent(b, ev);

        if (currHoveredObj->isEmbedded())
        {
            getNonEmbeddedParent(currHoveredObj)->triggerMouseButtonEvent(b, ev);
        }
    }
    MSG("WManager::feedMouseClick [end]: clickedObj_ = " << (void*)clickedObj_);
}


void
WManager::feedKey(int key, StateEvent ev)
{

}


void
WManager::feedMouseWheelEvent(int pos)
{
    //std::cout << "pos " << pos << std::endl;

    if (hoveredObj_)
    {
        int delta = pos - mouseWheelPos_;
        hoveredObj_->triggerMouseWheelEvent(delta);
        if (hoveredObj_->isEmbedded())
        {
            getNonEmbeddedParent(hoveredObj_)->triggerMouseWheelEvent(delta);
        }
    }

    mouseWheelPos_ = pos;
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

    // Use iterator instead of index access here so to make sure that additional
    // preRenderUpdate targets added during processing of the queue will also
    // be processed
    for (std::vector<GuiObject*>::iterator i = preRenderUpdateQueue_.begin();
        i != preRenderUpdateQueue_.end(); ++i)
    {
        (*i)->preRenderUpdate();
    }
    preRenderUpdateQueue_.clear();

    mainWin_->render(mainWin_->getPos());
}


void
WManager::registerForPreRenderUpdate(GuiObject* o)
{
    preRenderUpdateQueue_.push_back(o);
}


void
WManager::indicateRemovedObject(const GuiObject* o)
{
    if (o == hoveredObj_)
    {
        MSG("WManager::indicateRemovedObject [hoveredObj_]: " << (void*)hoveredObj_);
        hoveredObj_ = 0;
    }

    if (o == clickedObj_)
    {
        MSG("WManager::indicateRemovedObject [clickedObj_]: " << (void*)clickedObj_);
        clickedObj_ = 0;
    }
}


} // namespace gw1k
