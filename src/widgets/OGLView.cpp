#include "widgets/OGLView.h"

#include "Render.h"

#include <GL/gl.h>
#include <iostream>

#define MSG(x) std::cout << x << std::endl

namespace gw1k
{


OGLView::OGLView(const Point& pos, const Point& size)
:   Box(pos, size),
    transl_(geom::Point2D()),
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
    transl_ += geom::Point2D(delta.x, -delta.y) * widgToRelSize_ * 2;;
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
        // Translate so (0,0) is at top-left corner of our widget
        Point pos = getPos() + offset;
        glTranslatef(pos.x, pos.y, 0);

        // Transform so (0,0) is in the center and the shorter edge's size
        // corresponds to the range (-1,1) (thus, the longer edge will have a
        // greater range, depending on the aspect ratio)
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


} // namespace gw1k

