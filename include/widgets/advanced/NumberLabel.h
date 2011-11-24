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

    void setPrecision(int precision);

    void setNumber(float number);

    /**
     * Forwards number to setNumber(float), but sets the number of decimal
     * places to zero.
     */
    void setNumber(int number);

    void setPreamble(const std::string& preamble);

    void setUnit(const std::string& unit);

    void setNumberSpace(int n);

private:

    std::string toStr(float f) const;

private:

    float number_;

    int precision_;

    std::string preamble_;

    std::string unit_;

    unsigned int numberSpace_;
};


} // namespace gw1k

#endif // GW1K_NUMBERLABEL_H_
