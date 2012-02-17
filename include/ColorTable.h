#ifndef GW1K_COLORTABLE_H_
#define GW1K_COLORTABLE_H_

#include "Color4i.h"

namespace gw1k
{


class Renderable;


struct ColorTable
{

    enum ColorState { STATE_NORMAL, STATE_HOVERED, STATE_CLICKED };

    /**
     * Creates a ColorTable with all colors fields set to null (i.e., no color).
     */
    ColorTable();

    /**
     * Creates of copy of the given ColorTable.
     */
    ColorTable(const ColorTable& ct);

    ~ColorTable();

    ColorTable& operator=(const ColorTable& rhs);

    /**
     * Gets foreground and background colours specified for the given state.
     */
    void queryColors(Color4i*& fg, Color4i*& bg, ColorState state) const;

    void queryColors(Color4i*& fg, Color4i*& bg, const Renderable* r) const;

    Color4i* fgCol;
    Color4i* bgCol;
    Color4i* hoveredFgCol;
    Color4i* hoveredBgCol;
    Color4i* clickedFgCol;
    Color4i* clickedBgCol;

};


} // namespace gw1k

#endif // GW1K_COLORTABLE_H_
