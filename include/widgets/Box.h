#ifndef GW1K_BOX_H_
#define GW1K_BOX_H_

#include "../Renderable.h"
#include "../Point.h"
#include "../Rect.h"

namespace gw1k
{


class Box : public Renderable
{

public:

    Box(const Point& pos, const Point& size);

    virtual ~Box();

public:

    virtual void render(const Point& offset) const;

    virtual void renderFg(const Point& offset) const;

    virtual void renderBg(const Point& offset) const;

};


} // namespace gw1k

#endif // GW1K_BOX_H_
