#ifndef GW1K_BUTTON_H_
#define GW1K_BUTTON_H_

#include "Label.h"
#include "../listeners/MouseListener.h"

#include <string>

namespace gw1k
{


class Button : public Label
{

public:

    Button(const geom::Point2D& pos,
           const geom::Point2D& size,
           const std::string& text);

    virtual ~Button();

};


} // namespace gw1k

#endif // GW1K_BUTTON_H_
