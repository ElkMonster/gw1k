#ifndef GW1K_RESIZEDEVENTPROVIDER_H_
#define GW1K_RESIZEDEVENTPROVIDER_H_

#include "../listeners/ResizedListener.h"

#include <list>

namespace gw1k
{


typedef std::list<ResizedListener*>::iterator ResizedListnrIter;


class ResizedEventProvider
{

public:

    ResizedEventProvider();

    virtual ~ResizedEventProvider();

public:

    void addResizedListener(ResizedListener* dl);

    void removeResizedListener(ResizedListener* dl);

protected:

    void informResizedListeners(const Point& delta,
                                Orientation orientation,
                                GuiObject* receiver);

protected:

    std::list<ResizedListener*> resizedListeners_;

};


} // namespace gw1k

#endif // GW1K_RESIZEDEVENTPROVIDER_H_
