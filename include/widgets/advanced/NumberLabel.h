#ifndef GW1K_NUMBERLABEL_H_
#define GW1K_NUMBERLABEL_H_

#include "../Label.h"

namespace gw1k
{

class NumberLabel : public Label
{

public:

    NumberLabel(const Point& pos,
                const Point& size,
                float number,
                int faceSize = -1,
                const std::string& fontname = "arial.ttf",
                bool autoSize = false,
                const char* colorScheme = 0);

    NumberLabel(const Point& pos,
                const Point& size,
                float number,
                const std::string& preamble,
                const std::string& unit,
                int faceSize = -1,
                const std::string& fontName = "arial.ttf",
                bool autoSize = false,
                const char* colorScheme = 0);

    ~NumberLabel();

public:

    /**
     * Sets the number of digits to display after the decimal point
     */
    void setPrecision(int precision);

    /**
     * Sets the number to display.
     */
    void setNumber(float number);

    /**
     * Forwards number to setNumber(float), but sets the number of decimal
     * places to zero.
     */
    void setNumber(int number);

    /**
     * Sets the string to display to the left of the number.
     */
    void setPreamble(const std::string& preamble);

    /**
     * Sets the string to display to the right of the number.
     */
    void setUnit(const std::string& unit);

    /**
     * Sets the number of characters that the number should at least fill. If
     * the number is shorter than n, spaces are used to fill the remaining space.
     * This allows to align the number to the right.
     */
    void setNumberSpace(int n);

private:

    std::string toStr(float f) const;

private:

    float number_;

    int precision_;

    std::string preamble_;

    std::string unit_;

    /**
     * The number of characters that the displayed number string should have. If
     * the number is shorter than numberSpace_, spaces are used to fill the
     * remaining space. If the number is longer, numberSpace_ is ignored.
     */
    unsigned int numberSpace_;
};


} // namespace gw1k

#endif // GW1K_NUMBERLABEL_H_
