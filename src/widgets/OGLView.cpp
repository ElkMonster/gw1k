#include "widgets/OGLView.h"

#include "Render.h"

#include <GL/gl.h>
#include <iostream>

#define MSG(x) std::cout << x << std::endl

namespace gw1k
{


OGLView::OGLView(const Point& pos, const Point& size)
:   Box(pos, size),
    transl_(geom::Point2D(0.f, 0.f)),
    zoom_(1.f),
    minDimSize_(std::min(size.x, size.y)),
    widgToRelSize_(1.f / minDimSize_)
{
    addMouseListener(this);
}


OGLView::~OGLView()
{}


const Point&
OGLView::setSize(float width, float height)
{
    const Point& newSize = Box::setSize(width, height);

    minDimSize_ = std::min(newSize.x, newSize.y);
    widgToRelSize_ = 1.f / minDimSize_;

    return newSize;
}


void
OGLView::setTranslation(const geom::Point2D& t)
{
    transl_ = t;
}


const geom::Point2D&
OGLView::getTranslation() const
{
    return transl_;
}


void
OGLView::setZoomFactor(float z)
{
    if (z > 0.f)
    {
        zoom_ = z;
    }
    MSG(zoom_);
}


float
OGLView::getZoomFactor() const
{
    return zoom_;
}


void
OGLView::relativeTranslateBy(const Point& delta)
{
    transl_ += geom::Point2D(delta.x, -delta.y) * widgToRelSize_ * 2;
}


//void
//OGLView::renderFg(const Point& offset) const
//{
//    glColor3f(1.f, 1.f, 1.f);
//    Point pos = getPos() + offset;
//    Point last = pos + getSize() - Point(1, 1);
//    drawRect(pointToGeomPoint2D(pos), pointToGeomPoint2D(last));
//
//}


void
OGLView::renderContent(const Point& offset) const
{
    glPushMatrix();
    {
        // Translate so GL coordinate (0,0) is at top-left corner of our widget
        Point pos = getPos() + offset;
        glTranslatef(pos.x, pos.y, 0);

        // Transform so GL coordinate (0,0) is in the center and the shorter
        // edge's size corresponds to the GL coordinate range [-1,1] (thus, the
        // longer edge will have a greater range, depending on the aspect ratio)
        const Point& size = getSize();
        glScalef(0.5f * minDimSize_, -0.5f * minDimSize_, 1.f);
        glTranslatef(size.x * widgToRelSize_, -size.y * widgToRelSize_, 0.f);

        // Apply our transformations
        glTranslatef(transl_.x, transl_.y, 0.f);
        glScalef(zoom_, zoom_, 1.f);

        glPushMatrix();
        {
            renderOGLContent();
        }
        glPopMatrix();
    }
    glPopMatrix();
}


void
OGLView::mouseMoved(
    MouseMovedEvent ev,
    const Point& pos,
    const Point& delta,
    GuiObject* target)
{
    if ((target == this) && isClicked())
    {
        relativeTranslateBy(delta);
    }
}


void
OGLView::mouseClicked(MouseButton b, StateEvent ev, GuiObject* target)
{

}


void
OGLView::mouseWheeled(int delta, GuiObject* target)
{
    if (target == this)
    {
        float z = zoom_ + delta * 0.05f;
        setZoomFactor(z);
    }
}


void
OGLView::renderOGLContent() const
{
    using namespace geom;
    glColor3f(1.f, 0.f, 0.f);
    fillRect(Point2D(-0.5f, 0.f), Point2D(0.5f, 1.f));
}


geom::Point2D
OGLView::pxToGLPos(const gw1k::Point& pos) const
{
    using namespace geom;

    const gw1k::Point& widgSize = getSize();
    Point2D gWidgSize(widgSize.x, widgSize.y);

    // Half of the basic size (width/2, height/2) in GL coordinate space
    Point2D halfGLSize = gWidgSize * widgToRelSize_;

    // Top-left point in basic GL coordinate space (corresponds to widget
    // coordinate (0,0))
    Point2D glTopLeft(-halfGLSize.x, halfGLSize.y);

    // Apply transformation so glTopLeft corresponds to the GL coordinate system
    // point that is actually visible currently
    glTopLeft = (glTopLeft - transl_) / zoom_;

    // Calculate relative position of pos in the widget, invert y to match GL
    // coordinate system (y axis points up)
    Point2D pRel = Point2D(pos.x, -pos.y) / gWidgSize;

    // Transform relative position in widget to a vector that is based on the
    // currently visible GL coordinate system detail
    pRel *= halfGLSize * 2.f / zoom_;

    return glTopLeft + pRel;
}


geom::Point2D
OGLView::pxToGLDelta(const gw1k::Point& delta) const
{
    // TODO fix comments
    using namespace geom;

    const gw1k::Point& widgSize = getSize();
    Point2D gWidgSize(widgSize.x, widgSize.y);

    // Half of the basic size (width/2, height/2) in GL coordinate space
    Point2D glSize = gWidgSize * widgToRelSize_ * 2.f;

    // Calculate relative position of pos in the widget, invert y to match GL
    // coordinate system (y axis points up)
    Point2D dRel = Point2D(delta.x, -delta.y) / gWidgSize;

    // Transform relative delta in widget to a vector that is based on the
    // currently visible GL coordinate system detail
    dRel *= glSize / zoom_;

    return dRel;
}


} // namespace gw1k

