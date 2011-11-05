#include "widgets/TextureWiBox.h"

namespace gw1k
{


TextureWiBox::TextureWiBox(
    const Point& pos,
    const Point& size,
    const std::string& filename,
    bool bResizeToImageSize,
    const char* colorScheme)
:   WiBox(pos, size, colorScheme)
{
    texView_ = new TextureView(Point(1, 1), size - Point(2, 2), filename, bResizeToImageSize);
    addSubObject(texView_);
    texView_->setEmbedded();

    if (bResizeToImageSize)
    {
        const Point& s = texView_->getSize();
        WiBox::setSize(s.x + 2, s.y + 2);
    }
}


TextureWiBox::~TextureWiBox()
{
    removeSubObject(texView_);
    delete texView_;
}


void
TextureWiBox::setPadding(const Point& padding)
{
    padding_ = max(padding, Point());
    texView_->setPos(padding_.x, padding_.y);
    const Point& size = getSize();
    setSize(size.x, size.y);
}


const Point&
TextureWiBox::setSize(float width, float height)
{
    const Point& size = WiBox::setSize(width, height);
    Point texSize = size - Point(2, 2) - padding_ -padding_;
    const Point& size2 = texView_->setSize(texSize.x, texSize.y) + Point(2, 2);

    return (size2 == size) ? size : WiBox::setSize(size2.x + 2 * padding_.x, size2.y + 2 * padding_.y);
}


void
TextureWiBox::setAspectRatioAutoResize(TextureView::AspectRatioAutoAdapt a)
{
    texView_->setAspectRatioAutoResize(a);
    const Point& size = getSize();
    setSize(size.x, size.y);
}


bool
TextureWiBox::loadTexture(const std::string& filename)
{
    return texView_->loadTexture(filename);
}


void
TextureWiBox::setTexMulColor(const Color4i* c)
{
    texView_->setTexMulColor(c);
}


const Point&
TextureWiBox::getTextureSize() const
{
    return texView_->getTextureSize();
}


} // namespace gw1k