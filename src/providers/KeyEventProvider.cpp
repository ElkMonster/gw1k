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
    keyListeners_.remove(kl);
}


} // namespace gw1k
