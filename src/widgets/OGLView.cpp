#include "widgets/OGLView.h"

#include "Render.h"
#include "utils/Helpers.h"
#include "Math.h"

#ifdef __APPLE__
#include <OpenGL/gl.h>
#else
#include <GL/gl.h>
#endif

#include <iostream>

#define MSG(x) std::cout << x << std::endl

namespace gw1k
{


OGLView::OGLView(const Point& pos, const Point& size)
:   Box(pos, size),
    transl_(geom::Point2D(0.f, 0.f)),
    zoom_(1.f, 1.f),
    bMouseControl_(false)
{
    updateInternalVars();
    addMouseListener(this);
}


OGLView::~OGLView()
{}


const Point&
OGLView::setSize(float width, float height)
{
    const Point& newSize = Box::setSize(width, height);
    updateInternalVars();
    return newSize;
}


void
OGLView::setTranslation(const geom::Point2D& t)
{
    transl_ = t;
    updateTopLeftAndPxToGLFactor();
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
        zoom_.x = z;
        zoom_.y = z;
        updateTopLeftAndPxToGLFactor();
    }
    MSG(zoom_.x);
}


void
OGLView::setZoom(const geom::Point2D& z)
{
    zoom_ = max(geom::Point2D(0.f, 0.f), z);
    updateTopLeftAndPxToGLFactor();
    MSG(zoom_.x << ", " << zoom_.y);
}


float
OGLView::getZoomFactor() const
{
    return zoom_.x;
}


const geom::Point2D&
OGLView::getZoom() const
{
    return zoom_;
}


void
OGLView::translateViewByPx(const Point& delta)
{
    translateViewByGLUnits(pxToGLDelta(delta));
}


void
OGLView::translateViewByGLUnits(const geom::Point2D& delta)
{
    transl_ += delta;
    updateTopLeftAndPxToGLFactor();
}


void
OGLView::centerViewAt(const geom::Point2D& p)
{
    setTranslation(-p);
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
        glTranslatef(size.x * widgToRelSize_.x, -size.y * widgToRelSize_.y, 0.f);

        glPushMatrix();
        {
            // Apply our transformations
            glScalef(zoom_.x, zoom_.y, 1.f);
            glTranslatef(transl_.x, transl_.y, 0.f);

            glPushMatrix();
            {
                renderOGLContent();
            }
            glPopMatrix();
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
            translateViewByPx(delta);
        }
    }

    //Point globPos = getGlobalPos();
    //geom::Point2D glp = pxToGLPos(pos - globPos);
    //MSG(" mouse pos: " << pos << "  rel mouse pos: " << (pos - globPos) << "  gl pos: " << glp << "  gl pos to px pos: " << glPosToPx(glp));

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
            float fdelta = delta * 0.05f;
            if (zoom_.x == zoom_.y)
            {
                float z = zoom_.x + fdelta;
                setZoomFactor(z);
            }
            else
            {
                geom::Point2D z;
                // Warning: When the two zoom values are very close, the
                // following could cause unexpected effects due to very big
                // numbers resulting from the division
                if (zoom_.x < zoom_.y)
                {
                    float d = zoom_.y / zoom_.x;
                    z.x = zoom_.x + fdelta;
                    z.y = zoom_.y + d * fdelta;
                }
                else // zoom_.x > zoom_.y
                {
                    float d = zoom_.x / zoom_.y;
                    z.x = zoom_.x + d * fdelta;
                    z.y = zoom_.y + fdelta;
                }
                setZoom(z);
            }
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
    return gGLTopLeft_ + geom::Point2D(relPos.x, -relPos.y) * pxToGLFactor_;
}


geom::Point2D
OGLView::pxToGLDelta(const Point& delta) const
{
    return geom::Point2D(delta.x, -delta.y) * pxToGLFactor_;
}


geom::Point2D
OGLView::pxToGLUnit(const Point& v) const
{
    return geom::Point2D(v.x, v.y) * pxToGLFactor_;
}


Point
OGLView::glUnitToPx(const geom::Point2D& p) const
{
    geom::Point2D q = p / pxToGLFactor_;
    return Point(round(q.x), round(-q.y));
}


Point
OGLView::glPosToPx(const geom::Point2D& pos) const
{
    geom::Point2D p = (pos - gGLTopLeft_) / pxToGLFactor_;
    return Point(round(p.x), round(-p.y));
}


void
OGLView::updateTopLeftAndPxToGLFactor()
{
    using namespace geom;

    // Top-left is calculated by taking the top-left point in initial state
    // (i.e., where GL coordinate system origin is centred in our widget and the
    // shorter edge dimension extends from +1 to -1), which corresponds to
    // the widget coordinate (0,0), and applying transformation to it so
    // glTopLeft corresponds to the GL coordinate system point that is currently
    // located at the widget's (0,0) coordinate
    gGLTopLeft_ = Point2D(-gHalfGLSize_.x, gHalfGLSize_.y) / zoom_ - transl_;

    pxToGLFactor_ = widgToRelSize_ * 2.f / zoom_;
}


void
OGLView::updateInternalVars()
{
    const Point& size = getSize();
    minDimSize_ = std::min(size.x, size.y);

    gWidgSize_ = pointToGeomPoint2D(size);

    widgToRelSize_ = geom::Point2D(1.f, 1.f) / minDimSize_;

    // Half of the basic size (width/2, height/2) in GL coordinate space
    gHalfGLSize_ = gWidgSize_ * widgToRelSize_;

    updateTopLeftAndPxToGLFactor();

}

} // namespace gw1k

