#ifndef GW1K_WIBOX_H_
#define GW1K_WIBOX_H_

#include "Box.h"
#include "../Renderable.h"

namespace gw1k
{


class WiBox : public Box
{

public:

    WiBox(const Point& pos, const Point& size, const char* colorScheme = 0);

    virtual ~WiBox();

public:

    virtual void renderFg(const Point& offset) const;

    virtual void renderBg(const Point& offset) const;

    virtual void setColors(const char* colorScheme);

};


} // namespace gw1k

#endif // GW1K_WIBOX_H_
