#ifndef GW1K_WINDOWSTACK_H_
#define GW1K_WINDOWSTACK_H_

#include <vector>
#include "Point.h"


namespace gw1k
{

class WindowStack
{

public:

    //WindowStack()

public:

    void pushGlScissor(const Point& pos, const Point& size);

    void popGlScissor();

    void pushGlScissorOffset(const Point& offset);

    void popGlScissorOffset();

private:

    void setGlScissors(const int* i4) const;

private:

    std::vector<int*> stack_;

    std::vector<Point> offsetStack_;

    Point p1_;

    Point p2_;

    Point offset_;

};

} // namespace gw1k

#endif // GW1K_WINDOWSTACK_H_
