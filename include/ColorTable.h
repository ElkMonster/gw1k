#ifndef GW1K_COLORTABLE_H_
#define GW1K_COLORTABLE_H_

#include "Color4i.h"

namespace gw1k
{


struct ColorTable
{
    ColorTable();

    ~ColorTable();

    Color4i* fgCol;
    Color4i* bgCol;
    Color4i* hoveredFgCol;
    Color4i* hoveredBgCol;
    Color4i* clickedFgCol;
    Color4i* clickedBgCol;
};


} // namespace gw1k

#endif // GW1K_COLORTABLE_H_
