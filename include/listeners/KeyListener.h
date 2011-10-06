#ifndef GW1K_KEYLISTENER_H_
#define GW1K_KEYLISTENER_H_

#include "../Gw1kConstants.h"

namespace gw1k
{


class KeyListener
{

public:

    virtual void keyEvent(int key, StateEvent ev, GuiObject* receiver) = 0;

};


} // namespace gw1k

#endif // GW1K_KEYLISTENER_H_
