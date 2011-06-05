#ifndef GW1K_ACTION_H_
#define GW1K_ACTION_H_

#include "GuiObject.h"

namespace gw1k
{


struct Action
{

    virtual void operator(GuiObject* actuator) = 0;

};


} // namespace gw1k

#endif // GW1K_ACTION_H_
