#ifndef GW1K_RENDER_H_
#define GW1K_RENDER_H_

#include "geometry/include/Point2D.h"

namespace gw1k
{


void drawRect(const geom::Point2D& p0,
              const geom::Point2D& p1);

void drawRect(const geom::Point2D& p0,
              const geom::Point2D& p1,
              const geom::Point2D& p2,
              const geom::Point2D& p3);

void fillRect(const geom::Point2D& p0,
              const geom::Point2D& p1);

void fillRect(const geom::Point2D& p0,
              const geom::Point2D& p1,
              const geom::Point2D& p2,
              const geom::Point2D& p3);

void drawTriangle(const geom::Point2D& p0,
                  const geom::Point2D& p1,
                  const geom::Point2D& p2);

void fillTriangle(const geom::Point2D& p0,
                  const geom::Point2D& p1,
                  const geom::Point2D& p2);


} // namespace gw1k

#endif // GW1K_RENDER_H_
