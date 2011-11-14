#include "ColorTable.h"

#include "utils/Helpers.h"

namespace gw1k
{


ColorTable::ColorTable()
:   fgCol(0),
    bgCol(0),
    hoveredFgCol(0),
    hoveredBgCol(0),
    clickedFgCol(0),
    clickedBgCol(0)
{}


ColorTable::~ColorTable()
{
    DELETE_PTR(fgCol);
    DELETE_PTR(bgCol);
    DELETE_PTR(hoveredFgCol);
    DELETE_PTR(hoveredBgCol);
    DELETE_PTR(clickedFgCol);
    DELETE_PTR(clickedBgCol);
}


void
ColorTable::queryColors(Color4i*& fg, Color4i*& bg, ColorState state) const
{
    switch (state)
    {
    case STATE_CLICKED:
        fg = clickedFgCol ? clickedFgCol : (hoveredFgCol ? hoveredFgCol : fgCol);
        bg = clickedBgCol ? clickedBgCol : (hoveredBgCol ? hoveredBgCol : bgCol);
        break;
    case STATE_HOVERED:
        fg = hoveredFgCol ? hoveredFgCol : fgCol;
        bg = hoveredBgCol ? hoveredBgCol : bgCol;
        break;
    case STATE_NORMAL:
        fg = fgCol;
        bg = bgCol;
        break;
    }
}


void
ColorTable::queryColors(Color4i*& fg, Color4i*& bg, const GuiObject* o) const
{
    ColorState s = o->isClicked() ?
        STATE_CLICKED : (o->isHovered() ? STATE_HOVERED : STATE_NORMAL);
    queryColors(fg, bg, s);
}


} // namespace gw1k
