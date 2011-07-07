#ifndef GW1K_WMANAGER_H_
#define GW1K_WMANAGER_H_

#include "GuiObject.h"
#include "widgets/Box.h"
#include "Point.h"
#include "WindowStack.h"

#include <vector>

namespace gw1k
{


class WManager
{

    friend class WindowStack;

public:

    static WManager* getInstance();

private:

    WManager();

    WManager(const WManager&) {};

    ~WManager();

public:

    void feedMouseMove(int x, int y);

    void feedMouseClick(MouseButton b, StateEvent ev);

    void feedKey(StateEvent ev, int key);

    void setWindowSize(int width, int height);

    const Point& getWindowSize() const;

    const Point& getMousePos() const;

    void addObject(GuiObject* o);

    void removeObject(GuiObject* o);

    void pushGlScissor(const Point& pos, const Point& size);

    void popGlScissor();

    void pushGlScissorOffset(const Point& offset);

    void popGlScissorOffset();

    void render();

    /**
     * Allows objects to register for a call of their preRenderUpdate() method.
     *
     * This enables objects to update internal details right before they are
     * rendered. It is not intended to be used as a general purpose update
     * mechanism (though it may be used so without causing side-effects -
     * however, the preferred way is for the user to implement his or her own).
     *
     * This feature is useful if an object needs to perform actions "inside"
     * the render process, but cannot perform these actions when their render()
     * method is called (due to render()'s const state, probably).
     * [In particular, this method was needed in order to allow Labels to
     * correctly update their vertical text alignment (i.e., Y-position).]
     * Objects are processed in the order they were registered. Multiple
     * registration of one object will lead to multiple calls of
     * preRenderUpdate() for that object; this is to ensure that updates
     * depending on each other through object relations will not fail due to
     * silently modified order.
     */
    void registerForPreRenderUpdate(GuiObject* o);

    /**
     * This method allows to point out removed objects to WManager. This is
     * useful for objects that are removed when clicked, e.g. a close button,
     * as gw1k would automatically trigger a Released event on the object,
     * possibly resulting in unwanted behavior or even a segmentation fault in
     * case the object has been deleted.
     *
     * The given object argument is checked for matches with any internally
     * referenced objects (hovered and clicked), and references are dropped
     * accordingly in order to prevent any operation on the object.
     */
    void indicateRemovedObject(const GuiObject* o);

private:

    void feedMouseMoveInternal(const Point& pos,
                               const Point& delta,
                               GuiObject* o = 0);

private:

    static WManager* pInstance_;

    /** GUI element hovered by mouse the last time feedMouseMove() was called */
    GuiObject* hoveredObj_;

    /** GUI element that has been clicked and not released yet */
    GuiObject* clickedObj_;

    Box* mainWin_;

    Point winSize_;

    WindowStack scissorStack_;

    Point mousePos_;

    std::vector<GuiObject*> preRenderUpdateQueue_;

};

} // namespace gw1k

#endif // GW1K_WMANAGER_H_
