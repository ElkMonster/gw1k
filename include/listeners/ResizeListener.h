#ifndef GW1K_RESIZELISTENER_H_
#define GW1K_RESIZELISTENER_H_

#include "../Point.h"

namespace gw1k
{


class GuiObject;


class ResizeListener
{

public:

    virtual void notifyResized(GuiObject* which, const Point& newSize) = 0;

};


} // namespace gw1k

#endif // GW1K_RESIZELISTENER_H_

