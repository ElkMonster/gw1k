#include "widgets/advanced/LabeledRangeSlider.h"
#include "utils/StringHelpers.h"

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

    lLabel_ = new Label(Point(), Point(50, h), "Left");
    rLabel_ = new Label(Point(size.x - 50, 0), Point(50, h), "Right");

    addSubObject(slider_);
    addSubObject(lLabel_);
    addSubObject(rLabel_);

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
LabeledRangeSlider::actionPerformed(GuiObject* receiver)
{
    if (receiver == slider_)
    {
        updateLabels();
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
    lLabel_->setText(toString(slider_->getLValue()));
    rLabel_->setText(toString(slider_->getRValue()));
}

void LabeledRangeSlider::render(const Point& offset) const
{
    Box::render(offset);
}

} // namespace gw1k
