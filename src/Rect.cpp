#include "Rect.h"

namespace gw1k
{


Rect::Rect()
{}


Rect::Rect(const Rect& other)
:   pos_(other.pos_),
    size_(other.size_),
    end_(pos_ + size_)
{}


Rect::Rect(const Point& pos, const Point& size)
:   pos_(pos),
    size_(size),
    end_(pos + size)
{}


Rect::~Rect()
{}


void
Rect::pos(const Point& pos)
{
    pos_ = pos;
    end_ = pos_ + size_;
}


void
Rect::pos(int x, int y)
{
    pos_.x = x;
    pos_.y = y;
    end_ = pos_ + size_;
}


const Point&
Rect::pos() const
{
    return pos_;
}


void
Rect::size(const Point& size)
{
    size_ = size;
    end_ = pos_ + size_;
}


void
Rect::size(int width, int height)
{
    size_.x = width;
    size_.y = height;
    end_ = pos_ + size_;
}


const Point&
Rect::size() const
{
    return size_;
}


const Point&
Rect::end() const
{
    return end_;
}


bool
Rect::containsPoint(const Point& p) const
{
    return (p.x >= pos_.x) && (p.x < end_.x) && (p.y >= pos_.y) && (p.y < end_.y);
}


} // namespace gw1k
