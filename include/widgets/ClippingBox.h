#ifndef GW1K_CLIPPINGBOX_H_
#define GW1K_CLIPPINGBOX_H_

#include "Box.h"


namespace gw1k
{


class ClippingBox : public Box
{

public:

    /**
     * If autoSize is true, then the size parameter only defines the size of the
     * ClippingBox, i.e. the "window" that determines the area which is visible.
     * The "real size" is set to the same value initially, but will be
     * automatically adjusted to fit in any added sub-objects or recalculated
     * when sub-objects are removed.
     * If autoSize is false, the "real size" is set to the same size as the
     * "window" and must be managed manually (via setRealSize()).
     */
    ClippingBox(const Point& pos,
                const Point& size,
                bool autoSize = true);

    ~ClippingBox();

public:

    virtual const Point& setSize(float width, float height);

    void setClippingOffset(const Point& offset);

    const Point& getClippingOffset() const;

    /** Real size cannot be smaller than the size of the ClippingBox */
    void setRealSize(const Point& size);

    const Point& getRealSize() const;

    const Point& getRealOrigin() const;

    virtual void renderSubObjects(const Point& offset) const;

    virtual void addSubObject(GuiObject* o);

    virtual void removeSubObject(GuiObject* o);

    virtual GuiObject* getContainingObject(const Point& p);

    virtual bool containsMouse(const Point& p) const;

    void recalculateBounds();

private:

    /**
     * This point determines the offset relative to realOrigin_ at which the
     * "window" starts through which one "sees" the pane content.
     * The window's size is the same
     * that is set in the constructor or via setSize().
     */
    Point clippingOffset_;

    /**
     * This point determines the coordinate at which the pane "begins", i.e. it
     * is the top left point of the pane. The lower right point of the pane lies
     * at (realOrigin_ + realSize_).
     */
    Point realOrigin_;

    /**
     * This determines the actual size of the pane of which we only see a
     * portion (given that getSize() < realSize_). realSize_ will always be the
     * same or greater than pane size.
     */
    Point realSize_;

    /**
     * If true, the ClippingBox manages realOrigin_ and realSize_ automatically,
     * increasing and decreasing realSize_ as needed to fit all sub-objects.
     * Sub-objects with negative position will lead to a negative realOrigin_.
     * If false, realOrigin_ will always stay (0,0), and realSize_ must be
     * set manually.
     */
    bool autoSize_;
};


} // namespace gw1k

#endif // GW1K_CLIPPINGBOX_H_
