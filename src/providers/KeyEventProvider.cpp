#include "providers/KeyEventProvider.h"

#include <algorithm>

namespace gw1k
{


KeyEventProvider::KeyEventProvider()
{

}


KeyEventProvider::~KeyEventProvider()
{

}


void
KeyEventProvider::addKeyListener(KeyListener* kl)
{
    keyListeners_.push_back(kl);
}


void
KeyEventProvider::removeKeyListener(KeyListener* kl)
{

    KeyListnrIter i = std::find(keyListeners_.begin(), keyListeners_.end(), kl);

    if (i != keyListeners_.end())
    {
        keyListeners_.remove(kl);
    }
}


} // namespace gw1k
