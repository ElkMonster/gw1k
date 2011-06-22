#include "widgets/ScrollPane.h"
#include "widgets/Label.h"
#include "ThemeManager.h"

namespace gw1k
{


ScrollPane::ScrollPane(
    const Point& pos,
    const Point& size,
    bool autoSize,
    const char* colorScheme)
:   WiBox(pos, size),
    pane_(Point(0, 0), Point(size.x - 20, size.y - 20), autoSize),
    hSlider_(Point(0, size.y - 20), Point(size.x - 20, 20)),
    vSlider_(Point(size.x - 20, 0), Point(20, size.y - 20), true)
{
    GuiObject::addSubObject(&vSlider_);
    GuiObject::addSubObject(&hSlider_);
    GuiObject::addSubObject(&pane_);

    vSlider_.addListener(this);
    hSlider_.addListener(this);

    updateSliders();

    setColors(colorScheme);
}


ScrollPane::~ScrollPane()
{}


const Point&
ScrollPane::setSize(float width, float height)
{
    const Point& newSize = WiBox::setSize(width, height);

    pane_.setSize(newSize.x - 20, newSize.y - 20);

    hSlider_.setPos(0, newSize.y - 20);
    hSlider_.setSize(newSize.x - 20, 20);
    vSlider_.setPos(newSize.x - 20, 0);
    vSlider_.setSize(20, newSize.y - 20);

    refreshLayout();

    return newSize;
}


void
ScrollPane::addSubObject(GuiObject* o)
{
    pane_.addSubObject(o);
    updateSliders();
}


void
ScrollPane::removeSubObject(GuiObject* o)
{
    pane_.removeSubObject(o);
    updateSliders();
}


void
ScrollPane::sliderValueChanged(Slider* slider, float newVal, float delta)
{
    Point offset = pane_.getClippingOffset();
    Point paneSize = pane_.getSize();
    Point realSize = pane_.getRealSize();
    Point realOrigin = pane_.getRealOrigin();

    Point range = realSize - paneSize;

    if (slider == &hSlider_)
    {
        offset.x = range.x * newVal;
    }
    else if (slider == &vSlider_)
    {
        offset.y = range.y * newVal;
    }
    pane_.setClippingOffset(offset);
}


void
ScrollPane::refreshLayout()
{
    pane_.recalculateBounds();
    updateSliders();
}


void
ScrollPane::setColors(const char* colorScheme)
{
    std::string baseName(colorScheme ? colorScheme : "ScrollPane");
    ThemeManager::getInstance()->setColors(this, colorScheme, "ScrollPane");
    hSlider_.setColors((baseName + ".HSlider").c_str());
    vSlider_.setColors((baseName + ".VSlider").c_str());
}


const Point&
ScrollPane::getVisibleSize() const
{
    return pane_.getSize();
}


Slider&
ScrollPane::getHSlider()
{
    return hSlider_;
}



Slider&
ScrollPane::getVSlider()
{
    return vSlider_;
}


void
ScrollPane::updateSliders()
{
    Point realSize = pane_.getRealSize();
    Point realOrigin = pane_.getRealOrigin();
    Point offset = pane_.getClippingOffset();
    Point paneSize = pane_.getSize();

    Point range = realSize - paneSize;


    if (realSize.x > paneSize.x)
    {
        float val = static_cast<float>(offset.x) / range.x;
        hSlider_.setHandleSize((float)paneSize.x / realSize.x);
        hSlider_.setValue(val);
        hSlider_.setEnabled();
    }
    else
    {
        hSlider_.setHandleSize(1.f);
        hSlider_.setEnabled(false);
    }

    if (realSize.y > paneSize.y)
    {
        float val = static_cast<float>(offset.y) / range.y;
        vSlider_.setHandleSize((float)paneSize.y / realSize.y);
        vSlider_.setValue(val);
        vSlider_.setEnabled();
    }
    else
    {
        vSlider_.setHandleSize(1.f);
        vSlider_.setEnabled(false);
    }
}


} // namespace gw1k
