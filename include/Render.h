#ifndef GW1K_RENDER_H_
#define GW1K_RENDER_H_

#include "Color4i.h"

#include <Point2D.h>

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

void drawEllipse(const geom::Point2D& center,
                 const geom::Point2D& radius,
                 int sizeIdx = 1);

void fillEllipse(const geom::Point2D& center,
                 const geom::Point2D& radius,
                 int sizeIdx = 1);

void setGLColor(const Color4i* c);



} // namespace gw1k

#endif // GW1K_RENDER_H_
