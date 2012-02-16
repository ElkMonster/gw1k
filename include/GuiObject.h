#ifndef GW1K_GUIOBJECT_H_
#define GW1K_GUIOBJECT_H_

#include "providers/MouseEventProvider.h"
#include "providers/KeyEventProvider.h"
#include "providers/DraggedEventProvider.h"
#include "listeners/TimerListener.h"
#include "Point.h"
#include "Rect.h"

#include <list>
#include <vector>

namespace gw1k
{


class GuiObject : public MouseEventProvider, public KeyEventProvider,
    public DraggedEventProvider, public TimerListener
{

public:

    GuiObject();

    virtual ~GuiObject();

public:

    virtual void render(const Point& offset) const = 0;

    /** This method is called when this object is registered for an update call
     *  via WManager's registerForPreRenderUpdate().
     */
    virtual void preRenderUpdate();

    /**
     * Gets whether this widget contains the given point. The point must refer
     * to relative coordinates (i.e., the coordinate system used has its origin
     * at the top-left of the widget).
     * This function should usually only be used internally by the GuiObject
     * class. To get the latest mouse-containment status, use containsMouse().
     */
    virtual bool containsMouse(const Point& p) const;

    /**
     * Gets whether this object was found to contain the mouse at the last
     * update. The returned value is kept updated by getContainingObject().
     */
    bool containsMouse() const;

    virtual void setPos(float x, float y);

    virtual const Point& getPos() const;

    virtual const Point& setSize(float width, float height);

    virtual const Point& getSize() const;

    const Point& getEnd() const;

    Point getGlobalPos() const;

    /**
     * Convenience method calling setPos() with the result of getPos() + delta.
     */
    void moveBy(const Point& delta);

    void setHovered(bool state = true);

    bool isHovered() const;

    void setClicked(bool state = true);

    bool isClicked() const;

    void setVisible(bool state = true);

    bool isVisible() const;

    /**
     * Sets whether this widget can be interacted with. Non-interactive widgets
     * don't react to mouse actions, including their sub-objects(!).
     * The sub-widgets' contains-mouse status is not updated when this property
     * is set.
     * The interactive property can be used for widgets that solely display
     * information, or that are (temporarily) disabled.
     * See setClickTrough() for a less strict variant that still forwards mouse
     * actions to sub-widgets.
     */
    void setInteractive(bool state = true);

    bool isInteractive() const;

    /**
     * Sets whether this widget can receive mouse actions, but doesn't block
     * mouse actions occuring on sub-widgets. This basically means that one can
     * "click through" the object, resulting in a click on the object behind.
     * See setInteractive() for a more strict variant that entirely blocks the
     * widget and all its sub-widgets from reacting to mouse events.
     * Note that enabling the click-through property automatically enables the
     * interactive property.
     *
     */
    void setClickThrough(bool state = true);

    bool isClickThrough() const;

    void triggerMouseMovedEvent(MouseMovedEvent ev,
                                const Point& pos,
                                const Point& delta);

    void triggerMouseButtonEvent(MouseButton b, StateEvent ev);

    void triggerMouseWheelEvent(int delta);

    void triggerDraggedEvent(const Point& delta);

    virtual void addSubObject(GuiObject* o);

    virtual void removeSubObject(GuiObject* o);

    /**
     * Calls removeSubObject() with the given o and deletes it.
     *
     * If o is already null when this method is called, it simply returns
     * without doing anything.
     */
    void removeAndDeleteSubObject(GuiObject* o);

    /**
     * Deletes all sub-objects of this GuiObject.
     *
     * This method should be used with care: If a sub-object is referenced
     * elsewhere, the application will probably crash at some point after
     * calling this method. For this reason, gw1k's default widgets only use
     * removeAndDeleteSubObject().
     * Note that order is important when dealing with nested GuiObjects:
     * Innermost objects must be removed first, then their parent objects, etc.
     * It is therefore recommended to remove objects individually via
     * removeAndDeleteSubObject() in nested setups, and to use
     * removeAndDeleteAllSubObjects() for "simpler" setups where nothing can go
     * wrong.
     */
    virtual void removeAndDeleteAllSubObjects();

    virtual GuiObject* getContainingObject(const Point& p);

    int getNumSubObjects() const;

    /**
     * Sets the objects parent, overwriting any previously set parent.
     *
     * Manual parent setting should not be necessary normally. Instead,
     * automatic parent setting via addSubObject() should be preferred.
     * setParent() is mainly useful for re-parenting of objects and when an
     * object serves as a hidden container "layer" for another object (see
     * ScrollPane/ClippingBox for an example).
     */
    void setParent(GuiObject* parent);

    GuiObject* getParent() const;

    /**
     * Gets the first non-embedded parent in this GuiObject's parent hierarchy.
     */
    GuiObject* getNonEmbeddedParent() const;

    bool isEmbedded() const;

    /**
     * Embedded objects can be used to build combined widgets that more or less
     * act as if they were one widget, but in fact they are made up of two or
     * more. There has to be one non-embedded parent widget that serves as the
     * "master" container for all embedded widgets. In case of events occuring
     * on these embedded widgets, an appropriate event will be generated for the
     * non-embedded parent. Thus, the parent always reports an event when one of
     * its embedded sub-widgets receives one, making events easier to handle:
     * Inner details of the combined widget can be handled internally (with the
     * actual event receiver being correctly given in the callback), while an
     * external listener (registered to the non-embedded parent) will receive
     * only events stating the non-embedded parent as the receiver. Thus, it is
     * not necessary to have a global instance knowing about all widgets, but a
     * more structured approach is feasible where complexities are hidden in
     * parent widgets.
     * In contrast to non-embedded sub-objects, embedded sub-objects do not
     * trigger mouse-leave events for the containing non-embedded parent, but
     * instead, the non-embedded parent receives a mouse-move event.
     *
     */
    void setEmbedded(bool b = true);

    /**
     * Moves this object to the last position in its parent's render order, so
     * it is drawn after any of the other sub-objects.
     * This changes the internal order of sub-objects, so use it carefully. Once
     * changed, the previous order cannot be re-established.
     */
    void moveOnTop();

    /**
     * This method is called by WManager when a Timer registered for the object
     * has expired.
     * \param token A number that identifies the timer
     */
    virtual void timerExpired(int token);

    /**
     * Sets the coordinate of a click into this GuiObject.
     *
     * p should be a coordinate local to this GuiObject, i.e., it should be
     * greater than or equal to (0,0) and less than the GuiObject's size.
     * However, no check for this is performed as this method is for gw1k's
     * internal use only.
     */
    void setClickedPos(const Point& p);

    /**
     * Gets the point last clicked in this GuiObject.
     */
    const Point& getClickedPos() const;

    /**
     * Sets whether this object can be dragged.
     *
     * If not specified otherwise before via setDraggableArea(), the object's
     * drag area equals the parent's area by default. Parent size changes are
     * honoured automatically.
     */
    void setDraggable(bool state = true);

    /**
     * Gets whether this object can be dragged.
     */
    bool isDraggable() const;

    /**
     * Sets the area in which this object can be dragged around.
     *
     * Coordinates are relative to parent origin, i.e. (0,0) is the top-left
     * corner of the parent.
     * If area is 0, the whole parent area is used. If area is not 0, the passed
     * Rect object describes an area within the parent.
     * The padding parameter allows to specify a gap to the area borders that
     * cannot be crossed by the dragged object. In contrast, negative padding
     * values allow the dragged object to go beyond the specified area.
     */
    void setDraggableArea(const Rect* area,
                          const Point& padding = Point(0, 0));

    /**
     * Gets the drag area and padding.
     *
     * For non-null arguments, the respective value is copied to the argument;
     * null arguments are ignored.
     */
    void getDraggableArea(Rect* area, Point* padding = 0);

    /**
     * Sets the GuiObject whose checkDragDelta() method is called to modify drag
     * deltas before they're applied.
     *
     * This can be used to let another GuiObject govern drag actions if this
     * GuiObject cannot decide about drag deltas itself. The dragChecker must
     * override the checkDragDelta() method for this to work.
     */
    void setDragChecker(const GuiObject* dragChecker);

    /**
     * This method is called when the object is dragged.
     *
     * A GuiObject can be dragged if dragging has been enabled via
     * setDraggable().
     *
     * \param relMousePos the mouse position relative to this GuiObject's
     *                    coordinate system
     * \param b the mouse button currently pressed
     */
    Point drag(const Point& relMousePos, MouseButton b);

protected:

    /**
     * This method allows to check and modify the delta before it is applied to
     * the object when being dragged.
     *
     * Setting delta to (0,0) inside this method will inhibit drag actions.
     */
    virtual void checkDragDelta(Point& delta,
                                MouseButton b,
                                const GuiObject* dragReceiver);

    /**
     * Sets whether dragging continues to work when the mouse leaves this
     * object's area without the object being able to follow.
     * The ability of horizontal and vertical movement is specified separately,
     * e.g. to allow for comfortable slider bars that follow mouse movement even
     * if the mouse leaves the bar area (since it's really thin )
     */
    void setDraggableWhenNotContainingMouse(bool horizontal, bool vertical);

private:

    /**
     * Recursively resets all sub-objects' bContainsMouse_ to false.
     */
    void resetSubObjContainsMouseStatus();

    /**
     * Puts the given sub-object in the last position in subObjects_, moving all
     * sub-objects behind it closer to the front.
     */
    void moveOnTop(GuiObject* newTopSubObj);

protected:

    std::vector<GuiObject*> subObjects_;

    bool bIsHovered_;

    bool bIsClicked_;

    bool bIsVisible_;

    GuiObject* parent_;

    bool bIsEmbedded_;

    /**
     * Tells whether the mouse is currently within the bounds of this widget.
     * The value is refreshed every time getContainingObject() is called on this
     * widget.
     */
    bool bContainsMouse_;

    bool bIsDraggable_;

    Point dragAreaPadding_;

    Point clickedPos_;

private:

    Rect rect_;

    bool bIsInteractive_;

    bool bIsClickThrough_;

    /**
     * The area specifying where this GuiObject can be dragged to. If set to 0,
     * the parent frames the area.
     */
    Rect* dragArea_;

    bool bXDraggableWhenNotContainingMouse_;

    bool bYDraggableWhenNotContainingMouse_;

    GuiObject* dragChecker_;

};


} // namespace gw1k

#endif // GW1K_GUIOBJECT_H_
