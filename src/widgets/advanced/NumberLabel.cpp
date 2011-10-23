#include "widgets/advanced/NumberLabel.h"

#include "utils/StringHelpers.h"

namespace gw1k
{


NumberLabel::NumberLabel(
    const Point& pos,
    const Point& size,
    float number,
    int faceSize,
    const std::string& fontName,
    bool autoSize,
    const char* colorScheme)
:   Label(pos, size, "", faceSize, fontName, autoSize, colorScheme),
    number_(number),
    precision_(3)
{
    setNumber(number);
}


NumberLabel::~NumberLabel()
{

}


void
NumberLabel::setPrecision(int precision)
{
    precision_ = std::max(precision, 0);
}


void
NumberLabel::setNumber(float number)
{
    number_ = number;
    setText(toStr(number));
}


void
NumberLabel::setNumber(int number)
{
    number_ = number;
    setPrecision(0);
    setText(toStr(number));
}


std::string
NumberLabel::toStr(float f) const
{
    return toString(f, precision_);
}


} // namespace gw1k
