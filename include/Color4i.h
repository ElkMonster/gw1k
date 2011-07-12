#ifndef GW1K_COLOR4I_H_
#define GW1K_COLOR4I_H_

#include "Helpers.h"


#define F_FAC (1.f / 255.f)

namespace gw1k
{


struct Color4i
{

    Color4i(int red, int green, int blue, int alpha = 255)
    :   r(red), g(green), b(blue), a(alpha),
        rf(red * F_FAC), gf(green * F_FAC), bf(blue * F_FAC), af(alpha * F_FAC)
    {}


    Color4i(int rgb, int alpha = 255)
    :   r(rgb), g(rgb), b(rgb), a(alpha),
        rf(rgb * F_FAC), gf(rf), bf(rf), af(alpha * F_FAC)
    {}


    Color4i(const Color4i& c)
    :   r(c.r), g(c.g), b(c.b), a(c.a), rf(c.rf), gf(c.gf), bf(c.bf), af(c.af)
    {}

    Color4i(const Color4i* c)
    :   r(c->r), g(c->g), b(c->b), a(c->a), rf(c->rf), gf(c->gf), bf(c->bf), af(c->af)
    {}

    Color4i alpha(int alpha) const
    {
        Color4i c = *this;
        c.a = alpha;
        c.af = alpha * F_FAC;
        return c;
    }

    int r, g, b, a;
    float rf, gf, bf, af;
};


namespace color
{

    static const Color4i* const red = new Color4i(255, 0, 0, 255);
    static const Color4i* const green = new Color4i(0, 255, 0, 255);
    static const Color4i* const blue = new Color4i(0, 0, 255, 255);
    static const Color4i* const black = new Color4i(0, 0, 0, 255);

} // namespace color


} // namespace gw1k


#endif // GW1K_COLOR4I_H_
