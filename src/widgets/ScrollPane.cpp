#include "widgets/ScrollPane.h"
#include "widgets/ClippingBox.h"
#include "widgets/Label.h"
#include "ThemeManager.h"

namespace gw1k
{


ScrollPane::ScrollPane(
    const Point& pos,
    const Point& size,
    const char* colorScheme,
    AutoAdjustSize autoAdjustSize,
    bool stickySliders)
:   WiBox(pos, size),
    hSlider_(new Slider(Point(0, size.y - 20), Point(size.x - 20, 20))),
    vSlider_(new Slider(Point(size.x - 20, 0), Point(20, size.y - 20), true)),
    stickySliders_(stickySliders)
{
    Point sliderSpace = (stickySliders ? Point(20, 20) : Point(0, 0));
    pane_ = new ClippingBox(Point(), size - sliderSpace, autoAdjustSize);

    GuiObject::addSubObject(vSlider_);
    GuiObject::addSubObject(hSlider_);
    GuiObject::addSubObject(pane_);

    vSlider_->addListener(this);
    hSlider_->addListener(this);

    updateSliders();

    setColors(colorScheme);
}


ScrollPane::~ScrollPane()
{
    delete hSlider_;
    delete vSlider_;

}


const Point&
ScrollPane::setSize(float width, float height)
{
    const Point& newSize = WiBox::setSize(width, height);

    resizePaneAndSliders();
    updateSliders();

    return newSize;
}


void
ScrollPane::addSubObject(GuiObject* o)
{
    Point objsFit = pane_->getAccommodationStatus();

    pane_->addSubObject(o);

    revalidatePaneAndSliders(objsFit != pane_->getAccommodationStatus());
}


void
ScrollPane::removeSubObject(GuiObject* o)
{
    Point objsFit = pane_->getAccommodationStatus();

    pane_->removeSubObject(o);

    revalidatePaneAndSliders(objsFit != pane_->getAccommodationStatus());
}


void
ScrollPane::sliderValueChanged(Slider* slider, float newVal, float delta)
{
    Point offset = pane_->getClippingOffset();
    Point paneSize = pane_->getSize();
    Point realSize = pane_->getRealSize();
    Point realOrigin = pane_->getRealOrigin();

    Point range = realSize - paneSize;

    if (slider == hSlider_)
    {
        offset.x = range.x * newVal;
    }
    else if (slider == vSlider_)
    {
        offset.y = range.y * newVal;
    }
    pane_->setClippingOffset(offset);
}


void
ScrollPane::refreshLayout()
{
    Point objsFit = pane_->getAccommodationStatus();

    pane_->recalculateBounds();

    revalidatePaneAndSliders(objsFit != pane_->getAccommodationStatus());
}


void
ScrollPane::setColors(const char* colorScheme)
{
    std::string baseName(colorScheme ? colorScheme : "ScrollPane");
    ThemeManager::getInstance()->setColors(this, colorScheme, "ScrollPane");
    hSlider_->setColors((baseName + ".HSlider").c_str());
    vSlider_->setColors((baseName + ".VSlider").c_str());
}


const Point&
ScrollPane::getVisibleSize() const
{
    return pane_->getSize();
}


Slider&
ScrollPane::getHSlider()
{
    return *hSlider_;
}


Slider&
ScrollPane::getVSlider()
{
    return *vSlider_;
}


void
ScrollPane::resizePaneAndSliders()
{
    Point size = getSize();
    Point realPaneSize = pane_->getRealSize();

    // Take into account auto-sizing (which will take effect not until
    // pane_->setSize() is called) for slider space calculation. realPaneSize
    // is not going to overlap in the auto-sized dimension.
    AutoAdjustSize aas = pane_->getAutoAdjustSize();
    bool hOverlaps = (aas == ADJUST_HORIZ) ? false : (realPaneSize.x > size.x);
    bool vOverlaps = (aas == ADJUST_VERT) ? false : (realPaneSize.y > size.y);

    Point sliderSpace = (stickySliders_ ? Point(20, 20) :
        Point(vOverlaps ? 20 : 0, hOverlaps ? 20 : 0));

    Point newPaneSize = size - sliderSpace;
    pane_->GuiObject::setSize(newPaneSize);

    const Point& hSliderSize = hSlider_->setSize(size.x - sliderSpace.x, 20);
    hSlider_->setPos(0, size.y - hSliderSize.y);

    const Point& vSliderSize = vSlider_->setSize(20, size.y - sliderSpace.y);
    vSlider_->setPos(size.x - vSliderSize.x, 0);
}


void
ScrollPane::updateSliders()
{
    Point realSize = pane_->getRealSize();
    Point realOrigin = pane_->getRealOrigin();
    Point offset = pane_->getClippingOffset();
    Point paneSize = pane_->getSize();

    Point range = realSize - paneSize;

    updateSlider(hSlider_, realSize.x, paneSize.x, offset.x, range.x);
    updateSlider(vSlider_, realSize.y, paneSize.y, offset.y, range.y);
}


void
ScrollPane::updateSlider(
    Slider* slider,
    int realSize,
    int paneSize,
    int offset,
    int range)
{
    if (realSize > paneSize)
    {
        if (!stickySliders_ && !slider->isVisible())
        {
            slider->setVisible();
        }

        float val = static_cast<float>(offset) / range;
        slider->setHandleSize((float)paneSize / realSize);
        slider->setValue(val);
        slider->setEnabled();
    }
    else
    {
        if (!stickySliders_ && slider->isVisible())
        {
            slider->setVisible(false);
        }

        slider->setHandleSize(1.f);
        slider->setEnabled(false);
    }
}


void
ScrollPane::revalidatePaneAndSliders(bool accommodationStateChanged)
{
    if (!stickySliders_ && accommodationStateChanged)
    {
        resizePaneAndSliders();
    }

    updateSliders();
}


} // namespace gw1k
