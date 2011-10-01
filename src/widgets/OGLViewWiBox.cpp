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
     * normal WiBox, will get its renderOGLContent() method executed even though
     * it is derived from OGLView.
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

};


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
    delete oglRenderer_;
}


const Point&
OGLViewWiBox::setSize(float width, float height)
{
    const Point& newSize = setSize(width, height);
    oglRenderer_->setSize(newSize.x - 2, newSize.y - 2);
    return newSize;
}


} // namespace gw1k
