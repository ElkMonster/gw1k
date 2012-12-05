#ifndef GW1K_RESIZEDLISTENER_H_
#define GW1K_RESIZEDLISTENER_H_

#include "../Point.h"
#include "../Gw1kConstants.h"

namespace gw1k
{


class GuiObject;


class ResizedListener
{

public:

    virtual void resized(const Point& delta,
                          Orientation orientation,
                          GuiObject* receiver) = 0;

};

} // namespace gw1k

#endif // GW1K_RESIZEDLISTENER_H_


