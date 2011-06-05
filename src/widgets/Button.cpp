#include "widgets/Button.h"

#include "Helpers.h"

namespace gw1k
{


Button::Button(
    const geom::Point2D& pos,
    const geom::Point2D& size,
    const std::string& text)
:   Label(pos, size, text, size.y * 0.75f)
{

}


Button::~Button()
{}

}
