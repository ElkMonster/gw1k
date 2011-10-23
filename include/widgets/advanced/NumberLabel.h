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

    ~NumberLabel();

public:

    void setPrecision(int precision);

    void setNumber(float number);

    /**
     * Forwards number to setNumber(float), but sets the number of decimal
     * places to zero.
     */
    void setNumber(int number);

private:

    std::string toStr(float f) const;

private:

    float number_;

    int precision_;

};


} // namespace gw1k

#endif // GW1K_NUMBERLABEL_H_
