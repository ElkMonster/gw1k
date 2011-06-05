#include "Render.h"

#include <GL/gl.h>

//#define GW1K_ENABLE_GL_ERROR_CHECKS
#include "GLErrorCheck.h"

namespace gw1k
{


void drawRect(
    const geom::Point2D& p0,
    const geom::Point2D& p1)
{
    geom::Point2D w(p1.x - p0.x, 0);
    geom::Point2D h(0, p1.y - p0.y);
    //std::cout << "drawRect " << p0 << ", " << p1 << ", size = " << (w + h) <<std::endl;
    drawRect(p0, p0 + w, p1, p0 + h);
}


void drawRect(
    const geom::Point2D& p0,
    const geom::Point2D& p1,
    const geom::Point2D& p2,
    const geom::Point2D& p3)
{
    glBegin(GL_LINE_LOOP);
    {
        glVertex3f(p0.x, p0.y, 0.f);
        glVertex3f(p1.x, p1.y, 0.f);
        glVertex3f(p2.x, p2.y, 0.f);
        glVertex3f(p3.x, p3.y, 0.f);
    }
    glEnd();
    printIfGLError();
}


void fillRect(
    const geom::Point2D& p0,
    const geom::Point2D& p1)
{
    geom::Point2D w(p1.x - p0.x, 0);
    geom::Point2D h(0, p1.y - p0.y);
    fillRect(p0, p0 + w, p1, p0 + h);
}


void fillRect(
    const geom::Point2D& p0,
    const geom::Point2D& p1,
    const geom::Point2D& p2,
    const geom::Point2D& p3)
{
    glBegin(GL_QUADS);
    {
        glVertex3f(p0.x, p0.y, 0.f);
        glVertex3f(p1.x, p1.y, 0.f);
        glVertex3f(p2.x, p2.y, 0.f);
        glVertex3f(p3.x, p3.y, 0.f);
    }
    glEnd();
    printIfGLError();
}


void drawTriangle(const geom::Point2D& p0,
                  const geom::Point2D& p1,
                  const geom::Point2D& p2)
{
    glBegin(GL_LINES);
    {
        glVertex3f(p0.x, p0.y, 0.f);
        glVertex3f(p1.x, p1.y, 0.f);

        glVertex3f(p1.x, p1.y, 0.f);
        glVertex3f(p2.x, p2.y, 0.f);

        glVertex3f(p2.x, p2.y, 0.f);
        glVertex3f(p0.x, p0.y, 0.f);
    }
    glEnd();
}


void fillTriangle(const geom::Point2D& p0,
                  const geom::Point2D& p1,
                  const geom::Point2D& p2)
{
    glBegin(GL_TRIANGLES);
    {
        glVertex3f(p0.x, p0.y, 0.f);
        glVertex3f(p1.x, p1.y, 0.f);
        glVertex3f(p2.x, p2.y, 0.f);
    }
    glEnd();
}


} // namespace gw1k
