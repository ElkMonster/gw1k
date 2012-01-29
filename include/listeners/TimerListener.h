#ifndef GW1K_TIMERLISTENER_H_
#define GW1K_TIMERLISTENER_H_

namespace gw1k
{


class GuiObject;


class TimerListener
{

public:

    /**
     *
     */
    virtual void timerExpired(int token) = 0;

};


} // namespace gw1k

#endif // GW1K_TIMERLISTENER_H_
