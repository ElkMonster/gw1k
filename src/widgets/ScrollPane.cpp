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
    sliderSize_(20),
    sliderMargin_(0),
    hSlider_(new Slider(Point(0, size.y - sliderSize_), Point(size.x - sliderSize_, sliderSize_))),
    vSlider_(new Slider(Point(size.x - sliderSize_, 0), Point(sliderSize_, size.y - sliderSize_), true)),
    stickySliders_(stickySliders)
{
    Point sliderSpace = (stickySliders ? Point(sliderSize_, sliderSize_) : Point(0, 0));
    pane_ = new ClippingBox(Point(), size - sliderSpace, autoAdjustSize);

    GuiObject::addSubObject(vSlider_);
    GuiObject::addSubObject(hSlider_);
    GuiObject::addSubObject(pane_);

    vSlider_->addActionListener(this);
    hSlider_->addActionListener(this);

    updateSliders();

    setColors(colorScheme);
}


ScrollPane::~ScrollPane()
{
    GuiObject::removeSubObject(pane_);
    GuiObject::removeSubObject(hSlider_);
    GuiObject::removeSubObject(vSlider_);
    delete pane_;
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
    // Save whether objects fit before adding one in order to compare to the
    // state after
    Point objsFit = pane_->getAccommodationStatus();
    pane_->addSubObject(o);
    revalidatePaneAndSliders(objsFit != pane_->getAccommodationStatus());
}


void
ScrollPane::removeSubObject(GuiObject* o)
{
    // Save whether objects fit before removing one in order to compare to the
    // state after
    Point objsFit = pane_->getAccommodationStatus();
    pane_->removeSubObject(o);
    revalidatePaneAndSliders(objsFit != pane_->getAccommodationStatus());
}


void
ScrollPane::removeAndDeleteAllSubObjects()
{
    Point objsFit = pane_->getAccommodationStatus();
    pane_->removeAndDeleteAllSubObjects();
    revalidatePaneAndSliders(objsFit != pane_->getAccommodationStatus());
}


void
ScrollPane::actionPerformed(GuiObject* receiver)
{
    Point offset = pane_->getClippingOffset();
    const Point& paneSize = pane_->getSize();
    const Point& realSize = pane_->getRealSize();

    Point range = realSize - paneSize;

    if (receiver == hSlider_)
    {
        offset.x = range.x * hSlider_->getValue();
    }
    else if (receiver == vSlider_)
    {
        offset.y = range.y * vSlider_->getValue();
    }
    pane_->setClippingOffset(offset);
}


void
ScrollPane::refreshLayout()
{
    // Save whether objects fit before changing the layout in order to compare
    // to the state after
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
ScrollPane::setSliderSize(int size)
{
    sliderSize_ = std::max(size, 1);
    refreshLayout();
}


void
ScrollPane::setSliderMargin(int margin)
{
    sliderMargin_ = std::max(margin, 0);
    refreshLayout();
}


int
ScrollPane::getNumSubObjects() const
{
    return pane_->getNumSubObjects();
}


void
ScrollPane::resizePaneAndSliders()
{
    const Point& size = getSize();
    const Point& realPaneSize = pane_->getRealSize();

    // Take into account auto-sizing (which will take effect not until
    // pane_->setSize() is called) for slider space calculation. realPaneSize
    // is not going to overlap in the auto-sized dimension.
    AutoAdjustSize aas = pane_->getAutoAdjustSize();
    bool hOverlaps = (aas == ADJUST_HORIZ) ? false : (realPaneSize.x > size.x);
    bool vOverlaps = (aas == ADJUST_VERT) ? false : (realPaneSize.y > size.y);

    Point sliderSpace, sliderMargin;
    if (stickySliders_)
    {
        sliderSpace = Point(sliderSize_, sliderSize_);
    }
    else
    {
        if (vOverlaps)
        {
            sliderSpace.x = sliderSize_;
            sliderMargin.x = sliderMargin_;
        }
        if (hOverlaps)
        {
            sliderSpace.y = sliderSize_;
            sliderMargin.y = sliderMargin_;
        }
    }

    Point newPaneSize = size - sliderSpace - sliderMargin;
    pane_->setSize(newPaneSize.x, newPaneSize.y);

    const Point& hSliderSize = hSlider_->setSize(size.x - sliderSpace.x, sliderSize_);
    hSlider_->setPos(0, size.y - hSliderSize.y);

    const Point& vSliderSize = vSlider_->setSize(sliderSize_, size.y - sliderSpace.y);
    vSlider_->setPos(size.x - vSliderSize.x, 0);
}


void
ScrollPane::updateSliders()
{
    const Point& realSize = pane_->getRealSize();
    const Point& offset = pane_->getClippingOffset();
    const Point& paneSize = pane_->getSize();

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
