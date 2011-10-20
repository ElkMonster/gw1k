#include "WindowStack.h"

#include "WManager.h"

#define GW1K_ENABLE_GL_ERROR_CHECKS
#include "GLErrorCheck.h"

#include <GL/gl.h>
#include <iostream>

#define MSG(x) std::cout << x << std::endl

namespace gw1k
{


void
WindowStack::pushGlScissor(const Point& pos, const Point& size)
{
    //MSG("push");
    if (stack_.size() == 0)
    {
        glEnable(GL_SCISSOR_TEST);
        p1_ = pos - offset_;
        p2_ = p1_ + size;
    }
    else
    {
        p1_.x = std::max(p1_.x, pos.x - offset_.x);
        p1_.y = std::max(p1_.y, pos.y - offset_.y);

        Point d = pos + size - offset_;
        p2_.x = std::min(p2_.x, d.x);
        p2_.y = std::min(p2_.y, d.y);
    }

    // Offscreen objects (i.e., objects outside of the current scissor window)
    // generate a negative newSize, so catch this case by setting size to (0,0).
    // In order to be able to still find bugs in non-offscreen cases, a negative
    // size paramenter is left untouched and will thus lead to (visible) errors,
    // while the offscreen case works fine.
    Point newSize = ((p1_.x > p2_.x) || (p1_.y > p2_.y)) ? Point() : p2_ - p1_;


    //MSG("push: set scissor to " << p1_ << ", " << newSize);

    int* a = new int[4];
    a[0] = p1_.x;
    // Transform y coordinate because glScissor()'s assumes the origin to be
    // located bottom-left
    a[1] = WManager::getInstance()->winSize_.y - p1_.y - newSize.y;
    a[2] = newSize.x;
    a[3] = newSize.y;
    stack_.push_back(a);

    setGlScissors(a);
    PRINT_IF_GL_ERROR;
}


void
WindowStack::popGlScissor()
{
    //MSG("pop");

    delete[] stack_.back();
    stack_.pop_back();

    if (stack_.size() == 0)
    {
        glDisable(GL_SCISSOR_TEST);
    }
    else
    {
        int* a = stack_.back();
        setGlScissors(a);
        PRINT_IF_GL_ERROR;
        p1_.x = a[0];
        // Undo transformation that was performed in pushGlScissor() to conform
        // to glScissor()
        p1_.y = WManager::getInstance()->winSize_.y - a[1] - a[3];
        p2_ = p1_ + Point(a[2], a[3]);
    }
}


void
WindowStack::pushGlScissorOffset(const Point& offset)
{
    offsetStack_.push_back(offset);
    offset_ += offset;
}


void
WindowStack::popGlScissorOffset()
{
    offset_ -= offsetStack_.back();
    offsetStack_.pop_back();
}


void
WindowStack::setGlScissors(const int* i4) const
{
    //MSG("scissors set to (" << i4[0] << ", " << i4[1] << "), (" << i4[2] << ", " << i4[3] << ")");
    glScissor(i4[0], i4[1], i4[2], i4[3]);
}


} // namespace gw1k
