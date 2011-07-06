#ifndef GW1K_CLIPPINGBOX_H_
#define GW1K_CLIPPINGBOX_H_

#include "Box.h"

#include "ScrollPane.h"

namespace gw1k
{


/**
 * ClippingBox holds a number of sub-objects of arbitrary location and size, but
 * only provides a "window" view on them, i.e., only a certain part of the
 * sub-objects is visible. The window can be arbitrarily moved and resized.
 *
 * ClippingBox mainly serves as a hidden container for ScrollBox. It contains
 * the sub-objects added to ScrollBox.
 *
 * The real size will always be equal to or greater than the "window" size.
 * Sub-objects added to ClippingBox will get assigned as their parent the parent
 * of the ClippingBox. This is because ClippingBox is intended to serve as a
 * hidden container for ScrollBox.
 */
class ClippingBox : public Box
{

public:

    /**
     * The size parameter only defines the size of the ClippingBox, i.e. the
     * "window" that determines the area which is visible. The "real size" is
     * set to the same value initially, but will be automatically adjusted to
     * fit in any added sub-objects or to reduce to the required size when
     * sub-objects are removed.
     */
    ClippingBox(const Point& pos,
                const Point& size,
                ScrollPane::AutoAdjustSize autoAdjustSize);

    ~ClippingBox();

public:

    /**
     * Sets the ClippingBox' size and, if autoAdjustSize is enabled, adjusts
     * sub-object sizes and sets the according real size dimension to the same
     * value. The accommodation status is updated via checkAccommodation().
     */
    virtual const Point& setSize(float width, float height);

    void setClippingOffset(const Point& offset);

    const Point& getClippingOffset() const;

    const Point& getRealSize() const;

    const Point& getRealOrigin() const;

    virtual void renderSubObjects(const Point& offset) const;

    /**
     * Adds the given sub-object and, if autoAdjustSize is enabled, adjusts the
     * according dimension of o. If necessary, real size and/or real origin are
     * updated to let o fit, and the "window" position (clippingOffset_) is
     * modified to keep the current point of view.
     */
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

    /** Sets subObjAccommodationStatus. */
    void checkAccommodation();

    /** Calls autoAdjustSubObj() for all sub-objects. */
    void autoAdjustSubObjs();

    /**
     * Checks if o needs to be resized in the auto-adjusted dimension, and
     * resizes it accordingly.
     */
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
     * is the top left point of the pane. It can have any value equal to or less
     * than (0,0). The lower right point of the pane lies at
     * (realOrigin_ + realSize_).
     */
    Point realOrigin_;

    /**
     * This determines the actual size of the pane of which we only see a
     * portion (given that getSize() < realSize_). realSize_ will always be the
     * same or greater than pane size.
     */
    Point realSize_;

    /**
     * Specifies for both dimensions whether the real size fits in the "window"
     * size. A value of 0 means that the real size overlaps the "window" size in
     * that dimension, a value other than 0 means that the real size fits in the
     * window in that dimension.
     */
    Point subObjAccommodationStatus_;

    ScrollPane::AutoAdjustSize autoAdjustSize_;
};


} // namespace gw1k

#endif // GW1K_CLIPPINGBOX_H_
