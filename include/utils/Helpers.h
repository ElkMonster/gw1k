#ifndef GW1K_HELPERS_H_
#define GW1K_HELPERS_H_

#include "../Point.h"

#include <geometry/include/Point2D.h>



/** If ptr != 0, deletes the object referenced by ptr and sets ptr to 0  */
#define DELETE_PTR(ptr) if (ptr) { delete (ptr); (ptr) = 0; }


namespace gw1k
{


inline geom::Point2D
pointToGeomPoint2D(const Point& p)
{
    return geom::Point2D(p.x, p.y);
}



} // namespace gw1k

#endif // GW1K_HELPERS_H_
