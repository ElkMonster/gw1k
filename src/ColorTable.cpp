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


} // namespace gw1k
