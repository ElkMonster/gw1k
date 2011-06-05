#ifndef GW1K_HELPERS_H_
#define GW1K_HELPERS_H_

#include "Point.h"

#include "geometry/include/Point2D.h"



/** Deletes the object referenced by ptr only if ptr != 0 */
#define DELETE_PTR(ptr) if (ptr) delete (ptr)


/**
 * Prevents unused variable warning given by GCC when put in a variable
 * declaration like so: int PREVENT_UNUSED_WARNING x;
 */
//#ifdef __GNUC__
#define PREVENT_UNUSED_WARNING __attribute__((unused))
//#else
//#define PREVENT_UNUSED_WARNING
//#endif


namespace gw1k
{


inline geom::Point2D
pointToGeomPoint2D(const Point& p)
{
    return geom::Point2D(p.x, p.y);
}



} // namespace gw1k

#endif // GW1K_HELPERS_H_
