#ifndef GW1K_OGLVIEW_H_
#define GW1K_OGLVIEW_H_

#include "Box.h"
#include <geometry/include/Point2D.h>

namespace gw1k
{


/**
 * Question: I've derived a custom widget from OGLView and it behaves strange
 * when clicking and moving it - it seems to move twice!?
 * Answer: Check if you're adding the widget to its own mouse listeners. OGLView
 * already implements MouseListener and "listens to itself", so adding it again
 * causes the mouse handler methods to be called twice per event.
 */
class OGLView : public Box, public MouseListener
{

public:

    OGLView(const Point& pos, const Point& size);

    ~OGLView();

public:

    virtual const Point& setSize(float width, float height);

    void setTranslation(const geom::Point2D& t);

    const geom::Point2D& getTranslation() const;

    void setZoomFactor(float z);

    float getZoomFactor() const;

    void translateViewByPx(const Point& delta);

    void translateViewByGLUnits(const geom::Point2D& delta);

    void centerViewAt(const geom::Point2D& p);

    void allowMouseControl(bool enabled = true);

    virtual void renderContent(const Point& offset) const;

    virtual void mouseMoved(MouseMovedEvent ev,
                            const Point& pos,
                            const Point& delta,
                            GuiObject* receiver);

    virtual void mouseClicked(MouseButton b,
                              StateEvent ev,
                              GuiObject* receiver);

    virtual void mouseWheeled(int delta, GuiObject* receiver);

    /**
     * Gets the OpenGL coordinate that correspond to the given pixel position.
     * Pixel position must be relative to widget position, i.e., in order to get
     * the GL coordinate for the top-left widget corner, relPos must be (0,0).
     * Example using this function with a mouse listener:
     *
     * mouseMoved(MouseMovedEvent ev, const Point& pos, ...)
     * {
     *     Point relPos = pos - widget->getGlobalPos();
     *     geom::Point2D glPos = pxToGLPos(relPos);
     *     ...
     * }
     *
     */
    geom::Point2D pxToGLPos(const Point& relPos) const;

    geom::Point2D pxToGLDelta(const Point& delta) const;

    geom::Point2D pxToGLUnit(const Point& v) const;

    Point glUnitToPx(const geom::Point2D& p) const;

    /**
     * Gets the pixel position that corresponds to the given OpenGL coordinate.
     * The returned pixel position is relative to widget position.
     */
    Point glPosToPx(const geom::Point2D& pos) const;

protected:

    virtual void renderOGLContent() const;

private:

    void updateTopLeftAndPxToGLFactor();

    void updateInternalVars();

protected:

    geom::Point2D transl_;

    float zoom_;

    bool bMouseControl_;

    geom::Point2D gHalfGLSize_;

    geom::Point2D gGLTopLeft_;

private:

    int minDimSize_;

    /**
     * Holds a factor that scales widget (pixel) size to relative (OpenGL basic
     * coordinate system) size. Its value is 1.0 / min(width, height). It is
     * calculated each time setSize() is called.
     */
    float widgToRelSize_;

    /**
     * Holds a factor that can be multiplied with a pixel coordinate to obtain
     * the equivalent value in GL coordinate space based in the current widget
     * size and zoom. The reverse (GL to pixel) can be calculated by dividing by
     * the factor.
     */
    float pxToGLFactor_;

    geom::Point2D gWidgSize_;

};


} // namespace gw1k

#endif // GW1K_OGLVIEW_H_
