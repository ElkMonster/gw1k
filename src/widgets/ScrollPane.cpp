#include "widgets/ScrollPane.h"
#include "widgets/Label.h"
#include "ThemeManager.h"

namespace gw1k
{


ScrollPane::ScrollPane(
    const Point& pos,
    const Point& size,
    const char* colorScheme,
    bool autoSize,
    bool stickySliders)
:   WiBox(pos, size),
    hSlider_(new Slider(Point(0, size.y - 20), Point(size.x - 20, 20))),
    vSlider_(new Slider(Point(size.x - 20, 0), Point(20, size.y - 20), true)),
    stickySliders_(stickySliders)
{
    Point sliderSpace = (stickySliders ? Point(20, 20) : Point());
    pane_ = new ClippingBox(Point(), size - sliderSpace, autoSize),

    GuiObject::addSubObject(vSlider_);
    GuiObject::addSubObject(hSlider_);
    GuiObject::addSubObject(pane_);

    vSlider_->addListener(this);
    hSlider_->addListener(this);

    updateSliders();

    setColors(colorScheme);
}


ScrollPane::~ScrollPane()
{}


const Point&
ScrollPane::setSize(float width, float height)
{
    Point newSize = WiBox::setSize(width, height);

    Point realPaneSize = pane_->getRealSize();
    Point sliderSpace = (stickySliders_ ? Point(20, 20) :
        Point(realPaneSize.y > newSize.y ? 20 : 0, realPaneSize.x > newSize.x ? 20 : 0));

    Point paneSize = newSize - sliderSpace;
    pane_->setSize(paneSize.x, paneSize.y);

    hSlider_->setPos(0, newSize.y - 20);
    hSlider_->setSize(newSize.x - sliderSpace.x, 20);
    vSlider_->setPos(newSize.x - 20, 0);
    vSlider_->setSize(20, newSize.y - sliderSpace.y);

    refreshLayout();

    return newSize;
}


void
ScrollPane::addSubObject(GuiObject* o)
{
    pane_->addSubObject(o);

    // Recalculate object dimensions to take into account sliders that may
    // appear and take occupy some space
    if (!stickySliders_)
    {
        const Point& size = getSize();
        setSize(size.x, size.y);
    }

    updateSliders();
}


void
ScrollPane::removeSubObject(GuiObject* o)
{
    pane_->removeSubObject(o);

    // Recalculate object dimensions to take into account sliders that may
    // appear and take occupy some space
    if (!stickySliders_)
    {
        const Point& size = getSize();
        setSize(size.x, size.y);
    }

    updateSliders();
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
    pane_->recalculateBounds();
    updateSliders();
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
ScrollPane::updateSliders()
{
    Point realSize = pane_->getRealSize();
    Point realOrigin = pane_->getRealOrigin();
    Point offset = pane_->getClippingOffset();
    Point paneSize = pane_->getSize();

    Point range = realSize - paneSize;

    if (realSize.x > paneSize.x)
    {
        if (!stickySliders_ && !hSlider_->isVisible())
        {
            hSlider_->setVisible();
        }

        float val = static_cast<float>(offset.x) / range.x;
        hSlider_->setHandleSize((float)paneSize.x / realSize.x);
        hSlider_->setValue(val);
        hSlider_->setEnabled();
    }
    else
    {
        if (!stickySliders_ && hSlider_->isVisible())
        {
            hSlider_->setVisible(false);
        }

        hSlider_->setHandleSize(1.f);
        hSlider_->setEnabled(false);
    }

    if (realSize.y > paneSize.y)
    {
        if (!stickySliders_ && !vSlider_->isVisible())
        {
            vSlider_->setVisible();
        }

        float val = static_cast<float>(offset.y) / range.y;
        vSlider_->setHandleSize((float)paneSize.y / realSize.y);
        vSlider_->setValue(val);
        vSlider_->setEnabled();
    }
    else
    {
        if (!stickySliders_ && vSlider_->isVisible())
        {
            vSlider_->setVisible(false);
        }

        vSlider_->setHandleSize(1.f);
        vSlider_->setEnabled(false);
    }
}


} // namespace gw1k
