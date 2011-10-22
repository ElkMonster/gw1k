#include "providers/ActionEventProvider.h"

#include <algorithm>

namespace gw1k
{


typedef std::list<ActionListener*>::iterator ALIter;


ActionEventProvider::ActionEventProvider()
{

}


ActionEventProvider::~ActionEventProvider()
{

}


void
ActionEventProvider::addActionListener(ActionListener* al)
{
    actionListeners_.push_back(al);
}


void
ActionEventProvider::removeActionListener(ActionListener* al)
{
    ALIter i = std::find(actionListeners_.begin(), actionListeners_.end(), al);

    if (i != actionListeners_.end())
    {
        actionListeners_.remove(al);
    }
}


void
ActionEventProvider::informActionListeners(GuiObject* sender)
{
    for (ALIter i = actionListeners_.begin(); i != actionListeners_.end(); ++i)
    {
        (*i)->actionPerformed(sender);
    }
}


} // namespace gw1k
