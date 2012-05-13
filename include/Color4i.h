#ifndef GW1K_COLOR4I_H_
#define GW1K_COLOR4I_H_

#include "utils/Helpers.h"
#include "MathHelper.h"


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


// TODO Put in extra .cpp
/**
 * Copies the value(s) of col to dest.
 *
 * The following cases are handled:
 * - col != 0, dest == 0: Create new Color4i in dest and copy col's values
 * - col == 0, dest != 0: Delete dest and set it to 0 as well
 * - col != 0, dest != 0: Copy col's values to dest
 * If dest is 0, the method will simply return.
 */
inline void
setColor(const Color4i* col, Color4i*& dest)
{
    if (col && !dest)
    {
        dest = new Color4i(*col);
    }
    else if (!col && dest)
    {
        delete dest;
        dest = 0;
    }
    else if (col && dest)
    {
        *dest = *col;
    }
}


inline void
multiplyColors(gw1k::Color4i* target, const gw1k::Color4i* mult)
{
    if ((target != 0) && (mult != 0))
    {
        Color4i t(
            round(target->rf * mult->rf * 255.f),
            round(target->gf * mult->gf * 255.f),
            round(target->bf * mult->bf * 255.f),
            round(target->af * mult->af * 255.f)
        );
        *target = t;
    }
}


} // namespace gw1k


#endif // GW1K_COLOR4I_H_
