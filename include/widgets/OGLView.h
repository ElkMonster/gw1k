#ifndef GW1K_OGLVIEW_H_
#define GW1K_OGLVIEW_H_

#include "Box.h"
#include "geometry/include/Point2D.h"

namespace gw1k
{


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

    void relativeTranslateBy(const Point& delta);

    //virtual void renderFg(const Point& offset) const;

    virtual void renderContent(const Point& offset) const;

    virtual void mouseMoved(MouseMovedEvent ev,
                            const Point& pos,
                            const Point& delta,
                            GuiObject* receiver);

    virtual void mouseClicked(MouseButton b,
                              StateEvent ev,
                              GuiObject* receiver);

    virtual void mouseWheeled(int delta, GuiObject* receiver);

protected:

    virtual void renderOGLContent() const;

    geom::Point2D pxToGLPos(const gw1k::Point& pos) const;

    geom::Point2D pxToGLDelta(const gw1k::Point& delta) const;

protected:

    geom::Point2D transl_;

    float zoom_;

private:

    int minDimSize_;

    /**
     * Holds a factor that scales widget (pixel) size to relative (OpenGL basic
     * coordinate system) size. Its value is 1.0 / min(width, height). It is
     * calculated each time setSize() is called.
     */
    float widgToRelSize_;

};


} // namespace gw1k

#endif // GW1K_OGLVIEW_H_
