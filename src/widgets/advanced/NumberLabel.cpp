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
    precision_(3),
    numberSpace_(0)
{
    setNumber(number);
}


NumberLabel::NumberLabel(
    const Point& pos,
    const Point& size,
    float number,
    const std::string& preamble,
    const std::string& unit,
    int faceSize,
    const std::string& fontName,
    bool autoSize,
    const char* colorScheme)
:   Label(pos, size, "", faceSize, fontName, autoSize, colorScheme),
    number_(number),
    precision_(3),
    preamble_(preamble),
    unit_(unit),
    numberSpace_(0)
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
    setNumber(number_);
}


void
NumberLabel::setNumber(float number)
{
    number_ = number;

    std::string n = toStr(number);
    std::stringstream s;
    s << preamble_;
    if (numberSpace_ > n.length())
    {
        s.width(numberSpace_);
    }
    s << std::right << n << unit_;

    setText(s.str());
}


void
NumberLabel::setNumber(int number)
{
    setPrecision(0);
    setNumber(static_cast<float>(number));
}


void
NumberLabel::setPreamble(const std::string& preamble)
{
    preamble_ = preamble;
    setNumber(number_);
}


void
NumberLabel::setUnit(const std::string& unit)
{
    unit_ = unit;
    setNumber(number_);
}


void
NumberLabel::setNumberSpace(int n)
{
    numberSpace_ = n;
    setNumber(number_);

}


std::string
NumberLabel::toStr(float f) const
{
    return toString(f, precision_);
}


} // namespace gw1k
