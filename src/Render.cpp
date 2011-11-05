#include "Render.h"

#include <cmath>
#include <GL/gl.h>

//#define GW1K_ENABLE_GL_ERROR_CHECKS
#include "GLErrorCheck.h"


namespace
{

const double pi = std::atan(1.) * 4.;

struct P2 { float x, y; };

P2* circles[3] = { 0, 0, 0 };
int circleSizes[3] = { 50, 200, 1000 };

void setupCircle(int idx)
{
    P2* a = new P2[circleSizes[idx]];

    float t = 0.f;
    float dt = 2. * pi / circleSizes[idx];

    for (int i = 0; i != circleSizes[idx]; ++i, t += dt)
    {
        a[i].x = std::cos(t);
        a[i].y = std::sin(t);
    }

    circles[idx] = a;
}

P2* getCircle(int idx, int& size)
{
    if (!circles[idx])
    {
        setupCircle(idx);
    }
    size = circleSizes[idx];
    return circles[idx];
}


} // namespace


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
}


void drawTriangle(
    const geom::Point2D& p0,
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


void
fillTriangle(
    const geom::Point2D& p0,
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


void drawEllipse(
    const geom::Point2D& center,
    const geom::Point2D& radius,
    int sizeIdx)
{
    glPushMatrix();
    {
        int size;
        P2* a = getCircle(sizeIdx, size);

        glTranslatef(center.x, center.y, 0.f);
        glScalef(radius.x, radius.y, 1.f);

        glBegin(GL_LINE_LOOP);
        {
            for (int i = 0; i != size; ++i)
            {
                glVertex2f(a[i].x, a[i].y);
            }
        }
        glEnd();
    }
    glPopMatrix();
}


void fillEllipse(
    const geom::Point2D& center,
    const geom::Point2D& radius,
    int sizeIdx)
{
    glPushMatrix();
    {
        int size;
        P2* a = getCircle(sizeIdx, size);

        glTranslatef(center.x, center.y, 0.f);
        glScalef(radius.x, radius.y, 1.f);


        glBegin(GL_TRIANGLE_FAN);
        {
            glVertex2f(0.f, 0.f);

            for (int i = 0; i != size - 1; ++i)
            {
                glVertex2f(a[i].x, a[i].y);
                glVertex2f(a[i+1].x, a[i+1].y);
            }

            glVertex2f(a[size - 1].x, a[size - 1].y);
            glVertex2f(a[0].x, a[0].y);
        }
        glEnd();
    }
    glPopMatrix();
}


void
setGLColor(const Color4i* c)
{
    glColor4f(c->rf, c->gf, c->bf, c->af);
}


} // namespace gw1k
