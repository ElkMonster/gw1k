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
    zoom_(1.f)
{
    setFgColor(color::red);
    addMouseListener(this);
}


OGLView::~OGLView()
{}


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
    const Point& size = getSize();
    float scf = std::min(size.x, size.y);

    float zr = 2 / zoom_;
    geom::Point2D relDelta = geom::Point2D(delta.x, -delta.y) / scf * zr / zoom_;

    transl_ += relDelta;
    std::cout << transl_ << " reldelta: " << relDelta << std::endl;
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
        // Translate by offset so that (0,0) is at top-left corner of our widget
        Point pos = getPos() + offset;
        glTranslatef(pos.x, pos.y, 0);

        // Transform so (0,0) is in the center and the shorter edge's size
        // corresponds to the range (-1,1) (thus, the longer edge will have a
        // greater range, depending on the aspect ratio)
        const Point& size = getSize();
        float scf = std::min(size.x, size.y);
        glScalef(0.5f * scf, -0.5f * scf, 1.f);
        glTranslatef(size.x * (1.f / scf), -size.y * (1.f / scf), 0.f);

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
    fillRect(Point2D(0.f, 0.f), Point2D(1.f, 1.f));
}

} // namespace gw1k

