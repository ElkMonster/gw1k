#ifndef GW1K_KEYEVENTPROVIDER_H_
#define GW1K_KEYEVENTPROVIDER_H_

#include "../listeners/KeyListener.h"

#include <list>

namespace gw1k
{


typedef std::list<KeyListener*>::iterator KeyListnrIter;


class KeyEventProvider
{

public:

    KeyEventProvider();

    virtual ~KeyEventProvider();

public:

    void addKeyListener(KeyListener* kl);

    void removeKeyListener(KeyListener* kl);

protected:

    std::list<KeyListener*> keyListeners_;

};


} // namespace gw1k

#endif // GW1K_KEYEVENTPROVIDER_H_

