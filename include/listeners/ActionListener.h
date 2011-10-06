#ifndef GW1K_ACTIONLISTENER_H_
#define GW1K_ACTIONLISTENER_H_

namespace gw1k
{


class ActionListener
{

public:

    void actionPerformed(GuiObject* receiver) = 0;

};


} // namespace gw1k

#endif // GW1K_ACTIONLISTENER_H_
