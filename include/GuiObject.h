#ifndef GW1K_GUIOBJECT_H_
#define GW1K_GUIOBJECT_H_

#include "providers/MouseEventProvider.h"
#include "providers/KeyEventProvider.h"
#include "Point.h"
#include "Rect.h"

#include <list>
#include <vector>

namespace gw1k
{


class GuiObject : public MouseEventProvider, public KeyEventProvider
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

    virtual void addSubObject(GuiObject* o);

    virtual void removeSubObject(GuiObject* o);

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

private:

    Rect rect_;

    bool bIsInteractive_;

    bool bIsClickThrough_;

};


} // namespace gw1k

#endif // GW1K_GUIOBJECT_H_
