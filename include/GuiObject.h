#ifndef GW1K_GUIOBJECT_H_
#define GW1K_GUIOBJECT_H_

#include "listeners/MouseListener.h"
#include "listeners/KeyListener.h"
#include "listeners/ResizeListener.h"
#include "Point.h"
#include "Rect.h"

#include <list>
#include <vector>

namespace gw1k
{


class GuiObject
{

public:

    GuiObject();

    virtual ~GuiObject();

public:

    virtual void render(const Point& offset) const = 0;

    /** This method is called when this object is registered for an update call
     *  via WManager's registerForPreRenderUpdate().
     */
    virtual void update();

    void addMouseListener(MouseListener* ml);

    void removeMouseListener(MouseListener* ml);

    void addKeyListener(KeyListener* kl);

    void removeKeyListener(KeyListener* kl);

    void addResizeListener(ResizeListener* rl);

    void removeResizeListener(ResizeListener* rl);

    virtual bool containsMouse(const Point& p) const;

    virtual void setPos(float x, float y);

    virtual const Point& getPos() const;

    virtual const Point& setSize(float width, float height);

    virtual const Point& getSize() const;

    const Point& getEnd() const;

    Point getGlobalPos() const;

    void setHovered(bool state = true);

    bool isHovered() const;

    void setClicked(bool state = true);

    bool isClicked() const;

    void setVisible(bool state = true);

    bool isVisible() const;

    void setInteractive(bool state = true);

    bool isInteractive() const;

    void triggerMouseMovedEvent(MouseMovedEvent ev,
                                const Point& pos,
                                const Point& delta);

    void triggerMouseButtonEvent(MouseButton b, StateEvent ev);

    virtual void addSubObject(GuiObject* o);

    virtual void removeSubObject(GuiObject* o);

    virtual GuiObject* getContainingObject(const Point& p);

    int getNumSubObjects() const;

/*protected:

    void setParent(GuiObject* parent);

    GuiObject* getParent() const;*/

protected:

    std::list<MouseListener*> mouseListeners_;

    std::list<KeyListener*> keyListeners_;

    std::vector<GuiObject*> subObjects_;

    std::vector<ResizeListener*> resizeListeners_;

    bool bIsHovered_;

    bool bIsClicked_;

    bool bIsVisible_;

    GuiObject* parent_;

private:

    Rect rect_;

    bool bIsInteractive_;

};


} // namespace gw1k

#endif // GW1K_GUIOBJECT_H_
