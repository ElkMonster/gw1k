#include "widgets/OGLView.h"

#include "Render.h"
#include "Helpers.h"
#include "Math.h"

#include <GL/gl.h>
#include <iostream>

#define MSG(x) std::cout << x << std::endl

namespace gw1k
{


OGLView::OGLView(const Point& pos, const Point& size)
:   Box(pos, size),
    transl_(geom::Point2D(0.f, 0.f)),
    zoom_(1.f),
    bMouseControl_(false),
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

    gWidgSize_ = pointToGeomPoint2D(newSize);

    // Half of the basic size (width/2, height/2) in GL coordinate space
    gHalfGLSize_ = gWidgSize_ * widgToRelSize_;

    updateTopLeft();

    return newSize;
}


void
OGLView::setTranslation(const geom::Point2D& t)
{
    transl_ = t;
    updateTopLeft();
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
        updateTopLeft();
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
    updateTopLeft();
}


void
OGLView::allowMouseControl(bool state)
{
    bMouseControl_ = state;
}


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
    GuiObject* receiver)
{
    if (bMouseControl_)
    {
        if ((receiver == this) && isClicked())
        {
            relativeTranslateBy(delta);
        }
    }


    Point globPos = getGlobalPos();
    geom::Point2D glp = pxToGLPos(pos - globPos);
    MSG(" mouse pos: " << pos << "  rel mouse pos: " << (pos - globPos) << "  gl pos: " << glp << "  gl pos to px pos: " << glPosToPx(glp));

}


void
OGLView::mouseClicked(MouseButton b, StateEvent ev, GuiObject* receiver)
{

}


void
OGLView::mouseWheeled(int delta, GuiObject* receiver)
{
    if (bMouseControl_)
    {
        if (receiver == this)
        {
            float z = zoom_ + delta * 0.05f;
            setZoomFactor(z);
        }
    }
}


void
OGLView::renderOGLContent() const
{
    // Render x and y axis and a 1x1-sized box starting at the (OpenGL) origin
    using namespace geom;
    glColor4f(1.f, 0.f, 0.f, 0.33f);
    fillRect(Point2D(0.f, 0.f), Point2D(1.f, 1.f));
    glColor3f(1.f, 0.f, 0.f);
    glBegin(GL_LINES);
    {
        glVertex2f(-100.f, 0.f);
        glVertex2f(100.f, 0.f);

        glVertex2f(0.f, -100.f);
        glVertex2f(0.f, 100.f);
    }
    glEnd();
}


geom::Point2D
OGLView::pxToGLPos(const Point& relPos) const
{
    using namespace geom;
    // Calculate relative position of pos in the widget, invert y to match GL
    // coordinate system (y axis points up)
    Point2D pRel = Point2D(relPos.x, -relPos.y) / gWidgSize_;

    // Transform relative position in widget to a vector that is based on the
    // currently visible GL coordinate system detail
    pRel *= gHalfGLSize_ * 2.f / zoom_;

    return gGLTopLeft_ + pRel;
}


geom::Point2D
OGLView::pxToGLDelta(const Point& delta) const
{
    // TODO fix comments
    using namespace geom;

    // Half of the basic size (width/2, height/2) in GL coordinate space
    Point2D glSize = gWidgSize_ * widgToRelSize_ * 2.f;

    // Calculate relative position of pos in the widget, invert y to match GL
    // coordinate system (y axis points up)
    Point2D dRel = Point2D(delta.x, -delta.y) / gWidgSize_;

    // Transform relative delta in widget to a vector that is based on the
    // currently visible GL coordinate system detail
    dRel *= glSize / zoom_;

    return dRel;
}


Point
OGLView::glPosToPx(const geom::Point2D& pos) const
{
    geom::Point2D p = pos - gGLTopLeft_;
    p /= gHalfGLSize_ * 2.f / zoom_;
    p *= gWidgSize_;
    p.y = -p.y;

    return Point(round(p.x), round(p.y));
}


void
OGLView::updateTopLeft()
{
    // Top-left point in basic GL coordinate space (corresponds to widget
    // coordinate (0,0))
    gGLTopLeft_ = geom::Point2D(-gHalfGLSize_.x, gHalfGLSize_.y);

    // Apply transformation so glTopLeft corresponds to the GL coordinate system
    // point that is actually visible currently
    gGLTopLeft_ = (gGLTopLeft_ - transl_) / zoom_;
}


} // namespace gw1k

