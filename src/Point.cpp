#include "Point.h"


namespace gw1k
{


Point::Point()
:   x(0),
    y(0)
{}


Point::Point(int x, int y)
:   x(x),
    y(y)
{}


Point::Point(const Point& other)
:   x(other.x),
    y(other.y)
{}


Point::~Point()
{}


Point&
Point::operator+=(const Point& rhs)
{
    x += rhs.x;
    y += rhs.y;
    return *this;
}


Point&
Point::operator-=(const Point& rhs)
{
    x -= rhs.x;
    y -= rhs.y;
    return *this;
}


const Point
Point::operator+(const Point& other) const
{
    return Point(x + other.x, y + other.y);
}


const Point
Point::operator-(const Point& other) const
{
    return Point(x - other.x, y - other.y);
}


const Point
Point::operator*(int factor) const
{
    return Point(x * factor, y * factor);
}


const Point
Point::operator/(int divisor) const
{
    return Point(x / divisor, y / divisor);
}


const Point
Point::operator-() const
{
    return Point(-x, -y);
}


bool
Point::operator==(const Point& rhs) const
{
    return (x == rhs.x) && (y == rhs.y);
}


bool
Point::operator!=(const Point& rhs) const
{
    return !((x == rhs.x) && (y == rhs.y));
}


std::ostream&
operator<<(std::ostream& out, const Point& point)
{
    out << "Point(" << point.x << ", " << point.y << ")";
    return out;
}


const Point
min(const Point& p1, const Point& p2)
{
    return Point(std::min(p1.x, p2.x), std::min(p1.y, p2.y));
}

const Point
max(const Point& p1, const Point& p2)
{
    return Point(std::max(p1.x, p2.x), std::max(p1.y, p2.y));
}


} // namespace gw1k

