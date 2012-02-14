#include "WManager.h"

#include "Log.h"

#ifdef __APPLE__
#include <OpenGL/glew.h>
#else
#include <GL/glew.h>
#endif

#include <iostream>
#include <sys/time.h>

//#define MSG(x) std::cout << x << std::endl
#define MSG(x)

#define GW1K_ENABLE_GL_ERROR_CHECKS
#include "GLErrorCheck.h"


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


/*static*/
void
WManager::cleanup()
{
    if (pInstance_)
    {
        delete pInstance_;
    }
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
    //MSG("WManager::feedMouseMoveInternal [begin]: o = " << (void*)o);

    // A clicked object receives all the mouse movement events regardless of the
    // mouse leaving it. This allows for things like sliders which will follow
    // mouse movement even if their area is left.
    if (clickedObj_)
    {
        // Refresh the containsMouse status to prevent that objects claim to
        // contain the mouse when they actually don't
        // TODO If clickedObj doesn't contain mouse anymore, because mouse has jumped
        // somewhere, what is the status of all the objects in the hierarchy
        // below clickedObj that have their containsMouse status set to true and
        // clickedObj itself? -> containsMouse status should be reset top-down,
        // starting with clickedObj and going down its parent list.
        mainWin_->getContainingObject(pos);

        MSG("WManager::feedMouseMoveInternal: clickedObj_ = " << (void*)clickedObj_);
        clickedObj_->triggerMouseMovedEvent(GW1K_M_HOVERED, pos, delta);

        if (clickedObj_->isEmbedded())
        {
            // Trigger mouse event for all embedded parents and the first non-
            // embedded parent
            GuiObject* p = clickedObj_->getParent();
            while (p)
            {
                p->triggerMouseMovedEvent(GW1K_M_HOVERED, pos, delta);
                p = p->isEmbedded() ? p->getParent() : 0;
            }
        }

        if (clickedObj_->isDraggable())
        {
            Point relMousePos = mousePos_ - clickedObj_->getGlobalPos();
            Point realDelta = clickedObj_->drag(relMousePos, lastMouseButton_);
            clickedObj_->triggerDraggedEvent(realDelta);
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

    //MSG("WManager::feedMouseMoveInternal [end]");
}


void
WManager::feedMouseMoveHandleOldHoveredObj(
    const Point& pos,
    const Point& delta,
    const GuiObject* newHoveredObj) const
{
    if (!hoveredObj_)
    {
        return;
    }

    if (hoveredObj_->isEmbedded())
    {
        hoveredObj_->triggerMouseMovedEvent(GW1K_M_LEFT, pos, delta);

        // Check which embedded parents have been left as well and trigger event
        // for them. After the loop, hPar will point to the non-embedded parent.
        GuiObject* hPar = hoveredObj_->getParent();
        while (hPar->isEmbedded())
        {
            hPar->triggerMouseMovedEvent(
                hPar->containsMouse() ? GW1K_M_HOVERED : GW1K_M_LEFT, pos, delta);
            hPar = hPar->getParent();
        }

        GuiObject* nPar = 0;
        if (newHoveredObj && newHoveredObj->isEmbedded())
        {
            nPar = newHoveredObj->getNonEmbeddedParent();
        }

        // Check if the containing non-embedded parent is also left; this is
        // either true if there is no new hovered object, or if the new
        // hovered object is not embedded (but has the same parent), or when
        // it is embedded, but in another parent
        if (!newHoveredObj || (hPar != nPar))
        {
            hPar->triggerMouseMovedEvent(GW1K_M_LEFT, pos, delta);
        }
        else
        {
            hPar->triggerMouseMovedEvent(GW1K_M_HOVERED, pos, delta);
        }
    }
    else // hoveredObj_ NOT embedded
    {
        // If new hovered object is an embedded sub-object of hoveredObj_,
        // do nothing (that is, no LEFT event), otherwise trigger LEFT event
        bool bIsEmbeddedSubObj = newHoveredObj && newHoveredObj->isEmbedded()
            && (newHoveredObj->getNonEmbeddedParent() == hoveredObj_);
        if (!bIsEmbeddedSubObj)
        {
            hoveredObj_->triggerMouseMovedEvent(GW1K_M_LEFT, pos, delta);
        }
    }
}


void
WManager::feedMouseMoveHandleNewHoveredObj(
    const Point& pos,
    const Point& delta) const
{
    if (hoveredObj_)
    {
        hoveredObj_->triggerMouseMovedEvent(
            hoveredObj_->isHovered() ? GW1K_M_HOVERED : GW1K_M_ENTERED, pos,
            delta);

        if (hoveredObj_->isEmbedded())
        {
            // Trigger events for all embedded parents and first non-embedded
            // parent
            GuiObject* p = hoveredObj_->getParent();
            while (p)
            {
                p->triggerMouseMovedEvent(
                    p->isHovered() ? GW1K_M_HOVERED : GW1K_M_ENTERED, pos, delta);
                p = p->isEmbedded() ? p->getParent() : 0;
            }
        }
    }
}


void
WManager::feedMouseClick(MouseButton b, StateEvent ev)
{
    MSG("WManager::feedMouseClick [begin]: ev = " << (ev == GW1K_RELEASED ? "RELEASED" : "PRESSED"));
    bool eventHandled = false;
    lastMouseButton_ = b;

    // Trigger Release for object that was previously clicked (this makes sure
    // that the object is released even if it has moved away when it was clicked)
    if (ev == GW1K_RELEASED)
    {
        if (clickedObj_)
        {
            clickedObj_->triggerMouseButtonEvent(b, ev);

            if (clickedObj_->isEmbedded())
            {
                // Trigger event for all embedded parents and first non-
                // embedded parent
                GuiObject* p = clickedObj_->getParent();
                while (p)
                {
                    p->triggerMouseButtonEvent(b, ev);
                    p = p->isEmbedded() ? p->getParent() : 0;
                }
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
        clickedObj_->setClickedPos(mousePos_ - clickedObj_->getGlobalPos());
        MSG("WManager::feedMouseClick: clickedObj_ = " << (void*)clickedObj_);
        currHoveredObj->triggerMouseButtonEvent(b, ev);

        if (currHoveredObj->isEmbedded())
        {
            // Trigger event for all embedded parents and first non-embedded one
            GuiObject* p = currHoveredObj->getParent();
            while (p)
            {
                p->triggerMouseButtonEvent(b, ev);
                p = p->isEmbedded() ? p->getParent() : 0;
            }

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
            // Trigger event for all embedded parents and first non-embedded one
            GuiObject* p = hoveredObj_->getParent();
            while (p)
            {
                p->triggerMouseWheelEvent(delta);
                p = p->isEmbedded() ? p->getParent() : 0;
            }
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

    checkTimers();

    for (std::list<GuiObject*>::iterator i = preRenderDeleteQueue_.begin();
        i != preRenderDeleteQueue_.end(); ++i)
    {
        GuiObject* p = (*i)->getParent();
        if (p)
        {
            p->removeSubObject(*i);
        }
        delete *i;
    }
    preRenderDeleteQueue_.clear();

    for (std::list<GuiObject*>::iterator i = preRenderUpdateQueue_.begin();
        i != preRenderUpdateQueue_.end(); ++i)
    {
        (*i)->preRenderUpdate();
    }
    preRenderUpdateQueue_.clear();

    PRINT_IF_GL_ERROR;
    mainWin_->render(mainWin_->getPos());
    PRINT_IF_GL_ERROR;
}


void
WManager::registerForPreRenderUpdate(GuiObject* o)
{
    preRenderUpdateQueue_.push_back(o);
}


void
WManager::markForDeletion(GuiObject* o)
{
    if (o == 0)
    {
        Log::warning("WManager", "Passed null object to markForDeletion()");
    }
    else
    {
        preRenderDeleteQueue_.push_back(o);
    }
}


void
WManager::indicateRemovedObject(const GuiObject* o)
{
    if (o == clickedObj_)
    {
        MSG("WManager::indicateRemovedObject [clickedObj_]: " << (void*)clickedObj_);
        clickedObj_ = 0;
    }

    if (o == hoveredObj_)
    {
        MSG("WManager::indicateRemovedObject [hoveredObj_]: " << (void*)hoveredObj_);
        hoveredObj_ = 0;

        // Make sure that we're not left in a state where no object is hovered.
        // This can happen when widgets are deleted outside of event handler
        // code (e.g., if a close button only queues a widget for removal, but
        // it is actually removed later; the removed widget would disappear, but
        // leave hoveredObj_ set to 0 because of GuiObject's call to
        // indicateRemovedObject()).
        if (o != mainWin_)
        {
            feedMouseMoveInternal(mousePos_, Point(), 0);
        }
    }

    // Remove all timers running for this GuiObject
    for (std::list<Timer*>::iterator i = timerList_.begin();
        i != timerList_.end(); )
    {
        // Cast to void* since we're comparing TimerListener* and GuiObject*
        if ((*i)->target == o)
        {
            i = timerList_.erase(i);
        }
        else
        {
            ++i;
        }
    }
}


void
WManager::addTimer(double seconds, TimerListener* target, int userdata)
{
    Timer* timer = new Timer(seconds, target, userdata);

    if (timerList_.size() == 0)
    {
        timerList_.push_back(timer);
    }
    else
    {
        for (std::list<Timer*>::iterator i = timerList_.begin();
            i != timerList_.end(); ++i)
        {
            if (*timer >= (*i)->getExpirationTime())
            {
                // Insert before i
                timerList_.insert(i, timer);
                break;
            }
        }
    }
}


void
WManager::removeTimers(TimerListener* target)
{
    for (std::list<Timer*>::iterator i = timerList_.begin();
        i != timerList_.end(); )
    {
        if ((*i)->target == target)
        {
            i = timerList_.erase(i);
        }
        else
        {
            ++i;
        }
    }
}


void
WManager::removeTimers(TimerListener* target, int token)
{
    for (std::list<Timer*>::iterator i = timerList_.begin();
        i != timerList_.end(); )
    {
        if (((*i)->target == target) && ((*i)->token == token))
        {
            i = timerList_.erase(i);
        }
        else
        {
            ++i;
        }
    }
}


void
WManager::checkTimers()
{
    timeval now;
    gettimeofday(&now, 0);

    while (!timerList_.empty() && timerList_.front()->expired(now))
    {
        Timer* t = timerList_.front();
        t->target->timerExpired(t->token);
        timerList_.pop_front();
    }
}


} // namespace gw1k
