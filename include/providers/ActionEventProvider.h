#ifndef GW1K_ACTIONEVENTPROVIDER_H_
#define GW1K_ACTIONEVENTPROVIDER_H_

#include "../listeners/ActionListener.h"

#include <list>

namespace gw1k
{


class ActionEventProvider
{

public:

    ActionEventProvider();

    virtual ~ActionEventProvider();

public:

    void addActionListener(ActionListener* al);

    void removeActionListener(ActionListener* al);

protected:

    virtual void informActionListeners(GuiObject* sender);

protected:

    std::list<ActionListener*> actionListeners_;

};


} // namespace gw1k

#endif // GW1K_ACTIONEVENTPROVIDER_H_


