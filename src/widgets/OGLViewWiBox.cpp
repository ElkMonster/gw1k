#include "widgets/OGLViewWiBox.h"


namespace gw1k
{


class OGLViewWiBox_OGLRenderer : public OGLView
{

public:

    OGLViewWiBox_OGLRenderer(const Point& pos,
                             const Point& size,
                             OGLViewWiBox* parent)
    :   OGLView(pos, size),
        parentWiBox_(parent)
    {}

    ~OGLViewWiBox_OGLRenderer() {}

public:

    /**
     * Calls the renderOGLContent() method of the OGLViewWiBox passed in as
     * parent in the constructor. Thus, OGLViewWiBox, despite only being a
     * normal WiBox and not derived from OGLView, will get its renderOGLContent()
     * method executed.
     */
    virtual void renderOGLContent() const
    {
        parentWiBox_->renderOGLContent();
    }

    /**
     * Always returns false. This results in the parent being reported as the
     * widget that receives clicks.
     */
    virtual bool containsMouse(const Point& p) const
    {
        return false;
    }

private:

    OGLViewWiBox* parentWiBox_;

}; // OGLViewWiBox_OGLRenderer


OGLViewWiBox::OGLViewWiBox(
    const Point& pos,
    const Point& size,
    const char* colorScheme)
:   WiBox(pos, size, colorScheme),
    oglRenderer_(
        new OGLViewWiBox_OGLRenderer(Point(1, 1), size - Point(2, 2), this))
{
    addSubObject(oglRenderer_);
}


OGLViewWiBox::~OGLViewWiBox()
{
    removeSubObject(oglRenderer_);
    delete oglRenderer_;
}


void
OGLViewWiBox::setTranslation(const geom::Point2D& t)
{
    oglRenderer_->setTranslation(t);
}


const geom::Point2D&
OGLViewWiBox::getTranslation() const
{
    return oglRenderer_->getTranslation();
}


void
OGLViewWiBox::setZoomFactor(float z)
{
    oglRenderer_->setZoomFactor(z);
}


float
OGLViewWiBox::getZoomFactor() const
{
    return oglRenderer_->getZoomFactor();
}


void
OGLViewWiBox::relativeTranslateBy(const Point& delta)
{

}


void
OGLViewWiBox::allowMouseControl(bool enabled)
{
    oglRenderer_->allowMouseControl(enabled);
}


const Point&
OGLViewWiBox::setSize(float width, float height)
{
    const Point& newSize = WiBox::setSize(width, height);
    oglRenderer_->setSize(newSize.x - 2, newSize.y - 2);
    return newSize;
}


geom::Point2D
OGLViewWiBox::pxToGLPos(const Point& relPos) const
{
    return oglRenderer_->pxToGLPos(relPos);
}


geom::Point2D
OGLViewWiBox::pxToGLDelta(const Point& delta) const
{
    return oglRenderer_->pxToGLDelta(delta);
}


geom::Point2D
OGLViewWiBox::pxToGLUnit(const Point& v) const
{
    return oglRenderer_->pxToGLUnit(v);
}


Point
OGLViewWiBox::glPosToPx(const geom::Point2D& pos) const
{
    return oglRenderer_->glPosToPx(pos);
}


void
OGLViewWiBox::setShadeColors(const ColorTable& colorTable)
{
    oglRenderer_->setShadeColors(colorTable);
}


void
OGLViewWiBox::setShadeColorScheme(const char* colorScheme)
{
    oglRenderer_->setShadeColorScheme(colorScheme);
}


} // namespace gw1k
