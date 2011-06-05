#ifndef GW1K_RECT_H_
#define GW1K_RECT_H_

#include "Point.h"

namespace gw1k
{


class Rect
{

public:

    Rect();

    Rect(const Point& pos, const Point& size);

    ~Rect();

public:

    void pos(const Point& pos);

    void pos(int x, int y);

    const Point& pos() const;

    void size(const Point& size);

    void size(int width, int height);

    const Point& size() const;

    const Point& end() const;

    bool containsPoint(const Point& p) const;


private:

    Point pos_;

    Point size_;

    Point end_;

};


} // namespace gw1k

#endif // GW1K_RECT_H_
