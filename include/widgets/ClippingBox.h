#ifndef GW1K_CLIPPINGBOX_H_
#define GW1K_CLIPPINGBOX_H_

#include "Box.h"

#include "ScrollPane.h"

namespace gw1k
{


class ClippingBox : public Box
{

public:

    /**
     * The size parameter only defines the size of the
     * ClippingBox, i.e. the "window" that determines the area which is visible.
     * The "real size" is set to the same value initially, but will be
     * automatically adjusted to fit in any added sub-objects or recalculated
     * when sub-objects are removed.
     */
    ClippingBox(const Point& pos,
                const Point& size,
                ScrollPane::AutoAdjustSize autoAdjustSize);

    ~ClippingBox();

public:

    virtual const Point& setSize(float width, float height);

    void setClippingOffset(const Point& offset);

    const Point& getClippingOffset() const;

    const Point& getRealSize() const;

    const Point& getRealOrigin() const;

    virtual void renderSubObjects(const Point& offset) const;

    virtual void addSubObject(GuiObject* o);

    virtual void removeSubObject(GuiObject* o);

    virtual GuiObject* getContainingObject(const Point& p);

    virtual bool containsMouse(const Point& p) const;

    void recalculateBounds();

    /** Do not return a reference here because the returned point will most likely
     be used in before-after comparisons, so one has to make a copy anyway. */
    Point getAccommodationStatus() const;

    ScrollPane::AutoAdjustSize getAutoAdjustSize() const;

private:

    /** Sets subObjAccommodationStatus */
    void checkAccommodation();

    void autoAdjustSubObjs();

    void autoAdjustSubObj(GuiObject* o);

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

    Point subObjAccommodationStatus_;

    ScrollPane::AutoAdjustSize autoAdjustSize_;
};


} // namespace gw1k

#endif // GW1K_CLIPPINGBOX_H_
