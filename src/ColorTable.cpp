#include "ColorTable.h"

#include "Renderable.h"
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


ColorTable::ColorTable(const ColorTable& ct)
{
    *this = ct;
}


ColorTable::~ColorTable()
{
    DELETE_PTR(fgCol);
    DELETE_PTR(bgCol);
    DELETE_PTR(hoveredFgCol);
    DELETE_PTR(hoveredBgCol);
    DELETE_PTR(clickedFgCol);
    DELETE_PTR(clickedBgCol);
}


ColorTable&
ColorTable::operator=(const ColorTable& rhs)
{
    setColor(rhs.fgCol, fgCol);
    setColor(rhs.bgCol, bgCol);
    setColor(rhs.hoveredFgCol, hoveredFgCol);
    setColor(rhs.hoveredBgCol, hoveredBgCol);
    setColor(rhs.clickedFgCol, clickedFgCol);
    setColor(rhs.clickedBgCol, clickedBgCol);
    return *this;
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
ColorTable::queryColors(Color4i*& fg, Color4i*& bg, const Renderable* r) const
{
    ColorState state;
    if (r->isEmbedded() && r->choosesEmbeddedColorsByParentStatus())
    {
        GuiObject* parent = r->getNonEmbeddedParent();
        state = (parent->isClicked() ? ColorTable::STATE_CLICKED :
            (parent->isHovered() ?
                ColorTable::STATE_HOVERED : ColorTable::STATE_NORMAL));
    }
    else
    {
        state = r->isClicked() ?
            STATE_CLICKED : (r->isHovered() ? STATE_HOVERED : STATE_NORMAL);
    }
    queryColors(fg, bg, state);
}


} // namespace gw1k
