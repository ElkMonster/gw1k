#include "widgets/advanced/LabeledRangeSlider.h"

#include "utils/StringHelpers.h"
#include "Math.h"

namespace gw1k
{


LabeledRangeSlider::LabeledRangeSlider(
    const Point& pos,
    const Point& size,
    const float range[2],
    RangeSlider::RangeType rangeTye,
    const char* colorScheme)
:   Box(pos, size)
{
    int h = size.y / 2;
    slider_ = new RangeSlider(Point(0, h), Point(size.x, h),
        range, rangeTye, colorScheme);

    int lh = gw1k::round_pos(h * 0.8f);
    range = slider_->getRange();
    lLabel_ = new NumberLabel(Point(0, h - lh), Point(50, lh), range[0]);
    rLabel_ = new NumberLabel(Point(size.x - 50, h - lh), Point(50, lh), range[1]);
    lLabel_->setAutoSized(true);
    rLabel_->setAutoSized(true);

    addSubObject(slider_);
    addSubObject(lLabel_);
    addSubObject(rLabel_);

    slider_->addActionListener(this);

    setColors(colorScheme);

}

LabeledRangeSlider::~LabeledRangeSlider()
{
    removeSubObject(slider_);
    removeSubObject(lLabel_);
    removeSubObject(rLabel_);

    delete slider_;
    delete lLabel_;
    delete rLabel_;
}


void
LabeledRangeSlider::setup()
{
    //lLabel_->setFont()
    //toString(slider_->getLValue())
}


void
LabeledRangeSlider::setLValue(float v)
{
    slider_->setLValue(v);
}


float
LabeledRangeSlider::getLValue() const
{
    return slider_->getLValue();
}


void
LabeledRangeSlider::setRValue(float v)
{
    slider_->setRValue(v);
}


float
LabeledRangeSlider::getRValue() const
{
    return slider_->getRValue();
}


void
LabeledRangeSlider::setLabelPrecision(int left, int right)
{
    lLabel_->setPrecision(left);
    rLabel_->setPrecision(right);
}


void
LabeledRangeSlider::actionPerformed(GuiObject* receiver)
{
    if (receiver == slider_)
    {
        updateLabels();
        informActionListeners(this);
    }
}


void
LabeledRangeSlider::setColors(const char* colorScheme)
{
    std::string baseName = colorScheme ? colorScheme : "LabeledRangeSlider";

    slider_->setColors(baseName.c_str());
    lLabel_->setColors((baseName + ".Label").c_str());
    rLabel_->setColors((baseName + ".Label").c_str());
}


void
LabeledRangeSlider::updateLabels()
{
    lLabel_->setNumber(slider_->getLValue());
    rLabel_->setNumber(slider_->getRValue());

    const WiBox* lHandle, *rHandle;
    slider_->getHandles(lHandle, rHandle);

    const Point& lsize = lLabel_->getSize();
    const Point& rsize = rLabel_->getSize();

    // Calculate handle centres
    int lhc = lHandle->getPos().x + lHandle->getSize().x / 2;
    int rhc = rHandle->getPos().x + rHandle->getSize().x / 2;

    // Don't let Labels go offscreen (off-widget, that is)
    int endx = slider_->getEnd().x;
    int lx = std::min(std::max(lhc - lsize.x / 2, 0), endx - lsize.x);
    int rx = std::min(std::max(rhc - rsize.x / 2, 0), endx - rsize.x);

    // Prevent overlapping, keep a few pixels minimum distance
    int overlap = lx + lsize.x - rx;
    if (overlap > 0)
    {
        int d = overlap / 2 + 2;
        lx -= d;
        rx += d;
        if (lx < 0)
        {
            rx += std::abs(lx);
            lx = 0;
        }
        else if (rx + rsize.x  > endx)
        {
            lx -= rx + rsize.x - endx;
            rx = endx - rsize.x;
        }
    }

    lLabel_->setPos(lx, lLabel_->getPos().y);
    rLabel_->setPos(rx, rLabel_->getPos().y);

}


} // namespace gw1k
